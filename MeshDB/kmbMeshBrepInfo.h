/*----------------------------------------------------------------------
#                                                                      #
# Software Name : Advance/REVOCAP version 3.2                          #
# Class Name : MeshBrepInfo                                            #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2013/09/23     #
#                                                                      #
----------------------------------------------------------------------*/
/**
 *
 * ���b�V���̊K�w���E�\�����Ǘ�����
 *
 * entity : bodyId �܂��� FaceGroup �̎��ʎq
 * entity -> parent : �e entity
 * entity -> children : �q entity �̃��X�g
 * entity -> outline : �ȈՕ\���p�� entity
 *
 */
#pragma once

#include <string>
#include <vector>
#include <map>

#include "MeshDB/kmbTypes.h"

namespace kmb{

class MeshData;

class MeshBrepItem{
	friend class MeshBrepInfo;
public:
	kmb::bodyIdType getBodyId(void) const;
	const char* getGroup(void) const;
private:


	kmb::bodyIdType bodyId;
	std::string faceGroup;
	int dim;

	kmb::bodyIdType outlineId;
	MeshBrepItem* parent;
	std::vector< const MeshBrepItem* > children;
	MeshBrepItem(void);
	~MeshBrepItem(void);
};

class MeshBrepInfo
{
protected:
	MeshBrepInfo(void);
	int topDim;
	std::map< kmb::bodyIdType, MeshBrepItem* > mapperById;
	std::map< std::string, MeshBrepItem* > mapperByGroup;
	std::vector< const MeshBrepItem* > volumes;
	std::vector< const MeshBrepItem* > surfaces;
	std::vector< const MeshBrepItem* > edges;
public:
	~MeshBrepInfo(void);
	int getTopDim(void) const;
	size_t getCount(void) const;
	size_t getVolumeCount(void) const;
	size_t getSurfaceCount(void) const;
	size_t getEdgeCount(void) const;

	const MeshBrepItem* getParent(kmb::bodyIdType bodyId) const;
	const MeshBrepItem* getParent(const char* faceGroup) const;
	kmb::bodyIdType getOutline(kmb::bodyIdType bodyId) const;
	kmb::bodyIdType getOutline(const char* faceGroup) const;
	const std::vector< const MeshBrepItem* >* getChildren(kmb::bodyIdType bodyId) const;
	const std::vector< const MeshBrepItem* >* getChildren(const char* faceGroup) const;
	size_t getChildren(kmb::bodyIdType bodyId,std::vector<kmb::bodyIdType>& children) const;

	const std::vector< const MeshBrepItem* >* getVolumes(void) const;
	const std::vector< const MeshBrepItem* >* getSurfaces(void) const;
	const std::vector< const MeshBrepItem* >* getEdges(void) const;
	size_t getVolumes(std::vector<kmb::bodyIdType>& bodyIds) const;
	size_t getSurfaces(std::vector<kmb::bodyIdType>& bodyIds) const;
	size_t getEdges(std::vector<kmb::bodyIdType>& bodyIds) const;




	static MeshBrepInfo* create3DModel(kmb::MeshData *mesh,double angle=120.0,bool ridge=true);


	static MeshBrepInfo* create3DFaceModel(kmb::MeshData *mesh,double angle=120.0,bool ridge=true);



	static MeshBrepInfo* create3DModelWithBoundary(kmb::MeshData *mesh,bool reverse=false);


	static MeshBrepInfo* createPatchModel(kmb::MeshData *mesh,bool outline=false);


	static MeshBrepInfo* createSurfaceModel(kmb::MeshData *mesh,double angle=120.0,bool ridge=true);
};

}