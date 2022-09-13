#include "MeshDB/kmbMeshBrep.h"
#include "MeshDB/kmbMeshData.h"
#include "MeshDB/kmbElementContainer.h"
#include "MeshDB/kmbBoundaryExtractor.h"
#include "MeshDB/kmbSurfaceOperation.h"

#include <sstream>

kmb::MeshBrep::MeshBrep(void)
{}

kmb::bodyIdType kmb::MeshBrep::getVolume(int index) const
{
	return volumes[index];
}

kmb::bodyIdType kmb::MeshBrep::getParent(std::string face) const
{
	std::map< std::string, kmb::bodyIdType >::const_iterator f = parents.find(face);
	if (f != parents.end()) {
		return f->second;
	}
	else {
		return kmb::Body::nullBodyId;
	}
}

size_t kmb::MeshBrep::getVolumeCount(void) const
{
	return volumes.size();
}

kmb::bodyIdType kmb::MeshBrep::getPatch(int index) const
{
	return patches[index];
}

size_t kmb::MeshBrep::getPatchCount(void) const
{
	return patches.size();
}


std::string kmb::MeshBrep::getFace(kmb::bodyIdType volume, int index) const
{
	std::map< kmb::bodyIdType, std::vector<std::string> >::const_iterator f = faces.find(volume);
	if (f == faces.end()) {
		return "";
	}else{
		return f->second[index];
	}
}

size_t kmb::MeshBrep::getFaceCount(kmb::bodyIdType volume) const
{
	std::map< kmb::bodyIdType, std::vector<std::string> >::const_iterator f = faces.find(volume);
	if (f == faces.end()) {
		return 0;
	}
	else {
		return f->second.size();
	}
}

kmb::bodyIdType kmb::MeshBrep::getSurface(kmb::bodyIdType volume, int index) const
{
	std::map< kmb::bodyIdType, std::vector<kmb::bodyIdType> >::const_iterator s = surfaces.find(volume);
	if (s == surfaces.end()) {
		return kmb::Body::nullBodyId;
	}
	else {
		return s->second[index];
	}
}

size_t kmb::MeshBrep::getSurfaceCount(kmb::bodyIdType volume) const
{
	std::map< kmb::bodyIdType, std::vector<kmb::bodyIdType> >::const_iterator s = surfaces.find(volume);
	if (s == surfaces.end()) {
		return 0;
	}
	else {
		return s->second.size();
	}
}

kmb::MeshBrep* kmb::MeshBrep::create3DFaceModel(kmb::MeshData *mesh, double angle)
{
	kmb::MeshBrep* brep = new kmb::MeshBrep();
	kmb::bodyIdType bodyCount = mesh->getBodyCount();
	kmb::BoundaryExtractor bext;
	kmb::SurfaceOperation surfOp;
	for (kmb::bodyIdType bodyId = 0; bodyId<bodyCount; bodyId++) {
		if (mesh->getDimension(bodyId) == 3) {
			brep->volumes.push_back(bodyId);
			bext.clear();
			bext.setMesh(mesh);
			bext.appendBody(bodyId);
			if (angle > 0.0) {
				std::stringstream ss;
				ss << "Face" << bodyId << "_";
				bext.getBoundaryFace(bodyId, ss.str().c_str());
				surfOp.setMesh(mesh);
				std::vector< std::string > subfaces;
				surfOp.divideFaceGroup(ss.str(), angle, subfaces);
				brep->faces.insert(std::make_pair(bodyId, subfaces));
				for (std::string face : subfaces) {
					brep->parents.insert(std::make_pair(face, bodyId));
				}
				mesh->deleteData(ss.str().c_str());
			}
			else {
				std::stringstream ss;
				ss << "Face" << bodyId;
				bext.getBoundaryFace(bodyId, ss.str().c_str());
				std::vector< std::string > subfaces;
				subfaces.push_back(ss.str());
				brep->parents.insert(std::make_pair(ss.str(), bodyId));
				brep->faces.insert(std::make_pair(bodyId, subfaces));
			}
		}
	}
	return brep;
}

// patch モデルから作成する
kmb::MeshBrep* kmb::MeshBrep::createPatchModel(kmb::MeshData *mesh, double angle)
{
	kmb::MeshBrep* brep = new kmb::MeshBrep();
	kmb::bodyIdType bodyCount = mesh->getBodyCount();
	kmb::SurfaceOperation surfOp;
	for (kmb::bodyIdType bodyId = 0; bodyId<bodyCount; bodyId++) {
		if (mesh->getDimension(bodyId) == 2) {
			brep->patches.push_back(bodyId);
			surfOp.setMesh(mesh);
			std::vector< kmb::bodyIdType > subsurfaces;
			surfOp.divideSurface(bodyId, angle, subsurfaces);
			brep->surfaces.insert(std::make_pair(bodyId, subsurfaces));
		}
	}
	return brep;
}

void kmb::MeshBrep::update3DFaceModel(kmb::MeshData *mesh, double angle)
{
	kmb::bodyIdType bodyCount = mesh->getBodyCount();
	kmb::BoundaryExtractor bext;
	kmb::SurfaceOperation surfOp;
	for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; bodyId++) {
		if (mesh->getDimension(bodyId) == 3 && this->getFaceCount(bodyId) == 0 ){
			this->volumes.push_back(bodyId);
			bext.clear();
			bext.setMesh(mesh);
			bext.appendBody(bodyId);
			if (angle > 0.0) {
				std::stringstream ss;
				ss << "Face" << bodyId << "_";
				bext.getBoundaryFace(bodyId, ss.str().c_str());
				surfOp.setMesh(mesh);
				std::vector< std::string > subfaces;
				surfOp.divideFaceGroup(ss.str(), angle, subfaces);
				this->faces.insert(std::make_pair(bodyId, subfaces));
				for (std::string face : subfaces) {
					this->parents.insert(std::make_pair(face, bodyId));
				}
				mesh->deleteData(ss.str().c_str());
			}
			else {
				std::stringstream ss;
				ss << "Face" << bodyId;
				bext.getBoundaryFace(bodyId, ss.str().c_str());
				std::vector< std::string > subfaces;
				subfaces.push_back(ss.str());
				this->parents.insert(std::make_pair(ss.str(), bodyId));
				this->faces.insert(std::make_pair(bodyId, subfaces));
			}
		}
	}
}

kmb::MeshBrep* kmb::MeshBrep::create3DSourfaceModelFromBoundary(kmb::MeshData *mesh, bool reverse)
{
	kmb::MeshBrep* brep = new kmb::MeshBrep();
	kmb::bodyIdType bodyCount = mesh->getBodyCount();
	for (kmb::bodyIdType bodyId = 0; bodyId < bodyCount; bodyId++) {
		if (mesh->getDimension(bodyId) == 3) {
			brep->volumes.push_back(bodyId);
		}
		if (mesh->getDimension(bodyId) == 2) {
			brep->patches.push_back(bodyId);
		}
	}
	if (brep->parents.size() > 0) {
		for (kmb::bodyIdType bodyId : brep->volumes) {
			kmb::BoundaryExtractor bext;
			bext.setReverseMode(reverse);
			bext.setMesh(mesh);
			bext.appendBody(bodyId);
			kmb::Face f;
			for (kmb::bodyIdType surfaceId : brep->patches) {
				kmb::ElementContainer* body = mesh->getBodyPtr(surfaceId);
				if (body != NULL) {
					// 最初の要素だけで判定する
					kmb::ElementContainer::const_iterator eIter = body->begin();
					if (!eIter.isFinished() && bext.getBoundaryFace(eIter, f)) {
						brep->surfaces.insert(std::make_pair<>(bodyId,surfaceId));
					}
				}
			}
		}
	}
	return brep;
}
