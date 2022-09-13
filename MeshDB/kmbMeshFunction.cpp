#include "MeshDB/kmbMeshFunction.h"
#include "MeshDB/kmbMeshData.h"
#include "MeshDB/kmbDataBindings.h"
#include "MeshDB/kmbElementEvaluator.h"
#include "Common/kmbCalculator.h"

namespace kmb {
	template<>
	kmb::BoundingBox MeshFunction::calcBoundingBox(const kmb::MeshData* mesh)
	{
		kmb::BoundingBox bbox;
		kmb::MeshData::nodeIterator pIter = mesh->beginNodeIterator();
		kmb::MeshData::nodeIterator pIterEnd = mesh->endNodeIterator();
		while (pIter != pIterEnd) {
			bbox.update(pIter.x(), pIter.y(), pIter.z());
			++pIter;
		}
		return bbox;
	}

	template<>
	bool MeshFunction::isUniqueElementType(const kmb::MeshData* mesh, kmb::bodyIdType bodyId, kmb::elementType etype)
	{
		return (mesh->getElementCountByType(bodyId, etype) == mesh->getElementCount(bodyId));
	}

	template<>
	kmb::nodeIdType MeshFunction::findNode(const kmb::MeshData* mesh, double x, double y, double z)
	{
		kmb::nodeIdType nodeId = kmb::nullNodeId;
		kmb::Minimizer min;
		kmb::Point3D pt;
		kmb::MeshData::nodeIterator pIter = mesh->beginNodeIterator();
		kmb::MeshData::nodeIterator pIterEnd = mesh->endNodeIterator();
		while (pIter != pIterEnd) {
			pIter.getPoint(pt);
			if (min.update(pt.distanceSq(x, y, z))) {
				nodeId = pIter.getId();
			}
			++pIter;
		}
		return nodeId;
	}

	template<>
	kmb::elementIdType MeshFunction::searchElement(const kmb::MeshData* mesh, kmb::bodyIdType bodyId, double x, double y, double z, double tolerance) {
		kmb::elementIdType findElementId = kmb::Element::nullElementId;
		const kmb::Body* body = mesh->getBodyPtr(bodyId);
		if (body == NULL || mesh->getNodes() == NULL) {
			return findElementId;
		}
		//------------ naive search -------------------//
		kmb::Minimizer minimizer;
		kmb::Node n0, n1, n2, n3;
		kmb::Point3D pt(x, y, z);

		if (body->isUniqueDim(1)) {
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while (!eIter.isFinished()) {
				kmb::elementIdType eid = eIter.getId();
				if (mesh->getNode(eIter[0], n0) &&
					mesh->getNode(eIter[1], n1))
				{
					double d = pt.distanceSqToSegment(n0, n1);
					if (minimizer.update(d)) {
						findElementId = eid;
					}
				}
				++eIter;
			}
			if (minimizer.getMin() <= tolerance * tolerance) {
				return findElementId;
			}
			else {
				return kmb::Element::nullElementId;
			}
		}
		else if (body->isUniqueDim(2)) {
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while (!eIter.isFinished()) {
				kmb::elementIdType eid = eIter.getId();
				switch (eIter.getType())
				{
				case kmb::kTriangle:
				case kmb::kTriangle2:
					if (mesh->getNode(eIter[0], n0) &&
						mesh->getNode(eIter[1], n1) &&
						mesh->getNode(eIter[2], n2))
					{
						double d = pt.distanceSqToTriangle(n0, n1, n2);
						if (minimizer.update(d)) {
							findElementId = eid;
						}
					}
					break;
				case kmb::kQuadrilateral:
				case kmb::kQuadrilateral2:
					if (mesh->getNode(eIter[0], n0) &&
						mesh->getNode(eIter[1], n1) &&
						mesh->getNode(eIter[2], n2) &&
						mesh->getNode(eIter[3], n3))
					{
						double d = kmb::Minimizer::getMin(
							pt.distanceSqToTriangle(n0, n1, n2),
							pt.distanceSqToTriangle(n2, n3, n0));
						if (minimizer.update(d)) {
							findElementId = eid;
						}
					}
					break;
				default:
					break;
				}
				++eIter;
			}
			if (minimizer.getMin() <= tolerance * tolerance) {
				return findElementId;
			}
			else {
				return kmb::Element::nullElementId;
			}
		}
		else if (body->isUniqueDim(3)) {
			kmb::ElementEvaluator* evaluator = new kmb::ElementEvaluator(mesh->getNodes());
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while (!eIter.isFinished()) {
				// ここで (x,y,z) を含むかどうかを判定する
				// 含む場合は距離 0 とする
				double dist = 0.0;
				if (evaluator->getMinInnerVolume(eIter, x, y, z) >= 0.0) {
					dist = 0.0;
				}
				else {
					dist = evaluator->getDistanceSqOnBoundary(eIter, x, y, z);
				}
				if (minimizer.update(dist)) {
					findElementId = eIter.getId();
				}
				++eIter;
			}
			if (minimizer.getMin() <= tolerance * tolerance) {
				return findElementId;
			}
			else {
				return kmb::Element::nullElementId;
			}
			return findElementId;
		}
		else {
			return kmb::Element::nullElementId;
		}
	}

	// 射影行列 mat
	// 画面上の範囲 range
	// 範囲の中に含まれるデータを result に格納する
	template<>
	int MeshFunction::filter_2d(kmb::MeshData* mesh, std::string target, std::string result, float mat[16], float range[4]) {
		int count = 0;
		kmb::DataBindings* target_data = mesh->getData(target);
		kmb::DataBindings* result_data = mesh->getData(result);
		if (target_data == nullptr || result_data == nullptr) {
			return count;
		}
		if (target_data->getBindingMode() == kmb::DataBindings::kFaceGroup) {
			if (result_data->getBindingMode() == kmb::DataBindings::kNodeGroup) {
				// FaceGroup 上の点で範囲内になるものを NodeGroup に格納する
				for (kmb::DataBindings::const_iterator iter = target_data->begin(); !iter.isFinished(); iter++) {
					kmb::Point3D pt;
					kmb::Face f;
					iter.getFace(f);
					kmb::ElementContainer::const_iterator element = mesh->findElement(f.getElementId());
					kmb::idType localId = f.getLocalFaceId();
					int num_node = element.getBoundaryNodeCount(localId);
					for (int i = 0; i < num_node; i++) {
						kmb::nodeIdType nodeId = element.getBoundaryNodeId(localId, i);
						if (!result_data->hasId(nodeId)) {
							mesh->getNode(nodeId, pt);
							double px = mat[0] * pt.x() + mat[4] * pt.y() + mat[8] * pt.z() + mat[12];
							double py = mat[1] * pt.x() + mat[5] * pt.y() + mat[9] * pt.z() + mat[13];
							double pz = mat[2] * pt.x() + mat[6] * pt.y() + mat[10] * pt.z() + mat[14];
							double pw = mat[3] * pt.x() + mat[7] * pt.y() + mat[11] * pt.z() + mat[15];
							double rx = px / pw;
							double ry = py / pw;
							if (range[0] <= rx && range[1] <= ry && rx <= range[2] && ry <= range[3]) {
								result_data->addId(nodeId);
								count++;
							}
						}
					}
				}
			}
			else if (result_data->getBindingMode() == kmb::DataBindings::kFaceGroup) {
				// FaceGroup 上の点で範囲内になるものを FaceGroup に格納する
				for (kmb::DataBindings::const_iterator iter = target_data->begin(); !iter.isFinished(); iter++) {
					kmb::Point3D pt;
					kmb::Face f;
					iter.getFace(f);
					kmb::ElementContainer::const_iterator element = mesh->findElement(f.getElementId());
					kmb::idType localId = f.getLocalFaceId();
					int num_node = element.getBoundaryNodeCount(localId);
					bool flag = true;
					for (int i = 0; i < num_node; i++) {
						kmb::nodeIdType nodeId = element.getBoundaryNodeId(localId, i);
						if (!result_data->hasId(nodeId)) {
							mesh->getNode(nodeId, pt);
							double px = mat[0] * pt.x() + mat[4] * pt.y() + mat[8] * pt.z() + mat[12];
							double py = mat[1] * pt.x() + mat[5] * pt.y() + mat[9] * pt.z() + mat[13];
							double pz = mat[2] * pt.x() + mat[6] * pt.y() + mat[10] * pt.z() + mat[14];
							double pw = mat[3] * pt.x() + mat[7] * pt.y() + mat[11] * pt.z() + mat[15];
							double rx = px / pw;
							double ry = py / pw;
							if ((range[0] <= rx && range[1] <= ry && rx <= range[2] && ry <= range[3]) == false) {
								flag = false;
								break;
							}
						}
					}
					if (flag) {
						result_data->addId(f);
					}
				}
			}
		}
		return count;
	}

}
