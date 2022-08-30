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
				// ‚±‚±‚Å (x,y,z) ‚ðŠÜ‚Þ‚©‚Ç‚¤‚©‚ð”»’è‚·‚é
				// ŠÜ‚Þê‡‚Í‹——£ 0 ‚Æ‚·‚é
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

}
