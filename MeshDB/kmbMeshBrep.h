#pragma once

#include "MeshDB/kmbTypes.h"
#include <string>
#include <vector>
#include <map>

namespace kmb {

class MeshData;

class MeshBrep {
private:
	MeshBrep(void);
	// 元々の mesh に含まれている body
	std::vector< kmb::bodyIdType > volumes;
	std::vector< kmb::bodyIdType > patches;
	std::vector< kmb::bodyIdType > edges;
	std::map< kmb::bodyIdType, std::vector<std::string> > faces;
	std::map< kmb::bodyIdType, std::vector<kmb::bodyIdType> > surfaces;
public:
	kmb::bodyIdType getVolume(int index) const;
	size_t getVolumeCount(void) const;
	kmb::bodyIdType getPatch(int index) const;
	size_t getPatchCount(void) const;
	std::string getFace(kmb::bodyIdType volume, int index) const;
	size_t getFaceCount(kmb::bodyIdType volume) const;
	kmb::bodyIdType getSurface(kmb::bodyIdType volume, int index) const;
	size_t getSurfaceCount(kmb::bodyIdType volume) const;
	// volume しかないメッシュから作成する
	// 境界面は FaceGroup で作る
	static MeshBrep* create3DFaceModel(kmb::MeshData *mesh, double angle);
	// patch モデルから作成する
	static MeshBrep* createPatchModel(kmb::MeshData *mesh, double angle);
};

}