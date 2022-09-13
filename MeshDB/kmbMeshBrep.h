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
	// facegroup => body Id
	std::map< std::string, kmb::bodyIdType > parents;
public:
	// FaceGroup から Volume を取得する
	kmb::bodyIdType getParent(std::string face) const;
	// volume の添え字のアクセス
	kmb::bodyIdType getVolume(int index) const;
	size_t getVolumeCount(void) const;
	kmb::bodyIdType getPatch(int index) const;
	size_t getPatchCount(void) const;
	std::string getFace(kmb::bodyIdType volume, int index) const;
	size_t getFaceCount(kmb::bodyIdType volume) const;
	kmb::bodyIdType getSurface(kmb::bodyIdType volume, int index) const;
	size_t getSurfaceCount(kmb::bodyIdType volume) const;
	// mesh が更新された時に brep も更新する
	void update3DFaceModel(kmb::MeshData *mesh, double angle);
	// volume しかないメッシュから作成する
	// 境界面は FaceGroup で作る
	static MeshBrep* create3DFaceModel(kmb::MeshData *mesh, double angle);
	// 既に境界面があるが親子関係が失われているメッシュ
	// 親子関係を復元する
	// reverse = true の時は逆向きも許す
	static MeshBrep* create3DSourfaceModelFromBoundary(kmb::MeshData *mesh, bool reverse = false);
	// patch モデルから作成する
	static MeshBrep* createPatchModel(kmb::MeshData *mesh, double angle);
};

}