/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_Refiner version 1.1                          #
# Program Name : rcapRefiner                                           #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/
/*
 * 実装における注意点：
 *  細分される要素は refineElement の中では記憶しておく必要はないが、
 *  getOriginal で親を調べるときにどの要素を使って細分したのかを調べるために
 *  appendBody で rcapRefinerDoc.mesh に記録しておく。clearRefiner でラッパコンテナを削除している。
 *  細分後の要素は呼び出し側に返して、このライブラリの中では記憶しない。
 *
 *  nodeOffset がある場合、nodeContainer には nodeOffset でずらして登録する
 *
 *  境界条件は細分前と細分後のデータの両方を rcapRefinerDoc.refiner または rcapRefinerDoc.boundaryRefiner
 *  が保持している。どちらが保持するかどうかは境界条件の種類に依存する。
 *  Commit したときに rcapRefinerDoc.mesh に登録されている境界条件を細分前のデータから
 *  細分後のデータに入れ替えている。
 */

#include "rcapRefiner.h"
#include "rcapRefinerStruct.h"

#include "Geometry/kmbIdTypes.h"
#include "Geometry/kmbPoint3DContainerMArray.h"
#include "Common/kmbCalculator.h"

#include "MeshDB/kmbMeshDB.h"
#include "MeshDB/kmbElement.h"
#include "MeshDB/kmbElementContainerNArray.h"
#include "MeshDB/kmbElementContainerMixedArray.h"
#include "MeshDB/kmbBoundaryExtractor.h"
#include "MeshDB/kmbIntegerMapBindings.h"
#include "MeshDB/kmbNodeMapperBindings.h"
#include "MeshDB/kmbElementEvaluator.h"
#include "MeshDB/kmbVector2WithIntBindings.h"

#include "MeshGen/kmbMeshRefiner.h"
#include "MeshGen/kmbMiddleNodeManager.h"
#include "MeshGen/kmbMeshSmoother.h"

#include "RevocapIO/kmbFFbIO.h"
#include "RevocapIO/kmbHecmwIO.h"
#include "RevocapIO/kmbRevocapNeutralIO.h"
#include "RevocapIO/kmbRevocapCouplerIO.h"

#include "Shape/kmbRnfShapeIO.h"
#include "Shape/kmbSurface3D.h"
#include "Shape/kmbMiddleNodeManagerWithShape.h"

#include <stdio.h>
#include <string.h>
#include <vector>
#include <typeinfo>

#if defined _MSC_VER && _MSC_VER >= 1400
#define strcpy(ss0,ss1) strcpy_s(ss0,sizeof(ss1),ss1)
#endif

#ifdef _MSC_VER
#pragma warning(disable:4996) // fill_n の unsafe 警告を出さない for VC
#endif

struct rcapRefinerInfo rcapRefinerDoc;

void rcapGetVersion( void )
{
	puts("REVOCAP_Refiner Version 1.2.01 (2016/09/30)");
}

void rcapInitRefiner(const int32_t* node_Offset,const int32_t* element_Offset)
{
	REVOCAP_Debug("rcapInitRefiner start\n");
	rcapRefinerDoc.mesh = NULL;
	rcapRefinerDoc.refiner = NULL;
	rcapRefinerDoc.boundaryRefiner = NULL;
	rcapRefinerDoc.middleMan = NULL;
	rcapRefinerDoc.smoother = NULL;
	rcapRefinerDoc.nodeOffset = *node_Offset;
	rcapRefinerDoc.elementOffset = *element_Offset;
	// Refiner で offset 値の差分は吸収しているため
	// 内部のメッシュはすべて 0 始まりとしてよい
	// maxElementId は offset 後の値
	rcapRefinerDoc.maxElementId = -1;
	rcapRefinerDoc.maxRefinedElementId = -1;

	rcapRefinerDoc.mesh = new kmb::MeshDB();
	rcapRefinerDoc.mesh->beginNode(100000,"double_marray");
	rcapRefinerDoc.mesh->endNode();

	rcapRefinerDoc.middleMan = new kmb::MiddleNodeManager();

	rcapRefinerDoc.refiner = new kmb::MeshRefiner();
	rcapRefinerDoc.refiner->setMiddleNodeManager(rcapRefinerDoc.middleMan);
	rcapRefinerDoc.refiner->setMesh(rcapRefinerDoc.mesh);

	rcapRefinerDoc.boundaryRefiner = new kmb::MeshRefiner();
	rcapRefinerDoc.boundaryRefiner->setMiddleNodeManager(rcapRefinerDoc.middleMan);
	rcapRefinerDoc.boundaryRefiner->setMesh(rcapRefinerDoc.mesh);

	REVOCAP_Debug("rcapInitRefiner finish\n");
}

void rcapClearRefiner( void )
{
	REVOCAP_Debug("rcapClearRefiner\n");
	if(rcapRefinerDoc.mesh){
		rcapRefinerDoc.mesh->removeAllBodies();
	}
	if(rcapRefinerDoc.refiner){
		rcapRefinerDoc.refiner->clear();
	}
	if(rcapRefinerDoc.boundaryRefiner){
		rcapRefinerDoc.boundaryRefiner->clear();
	}
	if(rcapRefinerDoc.smoother){
		rcapRefinerDoc.smoother->clear();
	}
	rcapRefinerDoc.maxRefinedElementId = -1;
	rcapRefinerDoc.maxElementId = -1;
}

void rcapTermRefiner( void )
{
	REVOCAP_Debug("rcapTermRefiner start\n");
	if(rcapRefinerDoc.boundaryRefiner){
		delete rcapRefinerDoc.boundaryRefiner;
		rcapRefinerDoc.boundaryRefiner = NULL;
	}
	if(rcapRefinerDoc.refiner){
		delete rcapRefinerDoc.refiner;
		rcapRefinerDoc.refiner = NULL;
	}
	if(rcapRefinerDoc.middleMan){
		delete rcapRefinerDoc.middleMan;
		rcapRefinerDoc.middleMan = NULL;
	}
	if(rcapRefinerDoc.smoother){
		delete rcapRefinerDoc.smoother;
		rcapRefinerDoc.smoother = NULL;
	}
	if(rcapRefinerDoc.mesh){
		delete rcapRefinerDoc.mesh;
		rcapRefinerDoc.mesh = NULL;
	}
	std::vector< kmb::Surface3D* >::iterator sIter = rcapRefinerDoc.surfaces.begin();
	while( sIter != rcapRefinerDoc.surfaces.end () ){
		kmb::Surface3D* surf = *sIter;
		if( surf ){
			delete surf;
		}
		++sIter;
	}
	rcapRefinerDoc.surfaces.clear();
	REVOCAP_Debug("rcapTermRefiner finish\n");
}

void rcapSetNode64( const int32_t* _num, float64_t* coords, int32_t* globalIds, int32_t* localIds )
{
	REVOCAP_Debug("rcapSetNode64 start\n");
	if(rcapRefinerDoc.mesh == NULL || _num==NULL){
		return;
	}
	int32_t num = *_num;
	kmb::MiddleNodeManagerWithShape* middleNodeManagerWithShape
		= dynamic_cast<kmb::MiddleNodeManagerWithShape*>( rcapRefinerDoc.middleMan );
	if( localIds==NULL || localIds[0] < rcapRefinerDoc.nodeOffset ){
		// localId が定義されていないときは 0 から順に局所節点番号を付与する
		REVOCAP_Debug("rcapSetNode64 localIds is NULL\n");
		for(int i=0;i<num;++i){
			rcapRefinerDoc.mesh->insertNode(coords[3*i],coords[3*i+1],coords[3*i+2]);
		}
		if( globalIds != NULL && globalIds[0] >= rcapRefinerDoc.nodeOffset ){
			if( middleNodeManagerWithShape != NULL ){
				// fitting data があるときは
				// globalId => localId に変換
				std::map< kmb::nodeIdType, kmb::nodeIdType > imapper;
				for(int i=0;i<num;++i){
					imapper.insert( std::pair< kmb::nodeIdType, kmb::nodeIdType >( globalIds[i]-rcapRefinerDoc.nodeOffset, static_cast<kmb::nodeIdType>(i) ) );
				}
				middleNodeManagerWithShape->replaceNodeIds( imapper );
			}
		}
	}else{
		// localId が定義されているときはその番号で節点を格納する
		for(int i=0;i<num;++i){
			rcapRefinerDoc.mesh->insertNodeWithId(coords[3*i],coords[3*i+1],coords[3*i+2],localIds[i]-rcapRefinerDoc.nodeOffset);
		}
		if( globalIds != NULL && globalIds[0] >= rcapRefinerDoc.nodeOffset ){
			if( middleNodeManagerWithShape != NULL ){
				// fitting data があるときは
				// globalId => localId に変換
				std::map< kmb::nodeIdType, kmb::nodeIdType > imapper;
				for(int i=0;i<num;++i){
					imapper.insert( std::pair< kmb::nodeIdType, kmb::nodeIdType >( globalIds[i]-rcapRefinerDoc.nodeOffset, localIds[i]-rcapRefinerDoc.nodeOffset ) );
				}
				middleNodeManagerWithShape->replaceNodeIds( imapper );
			}
		}
	}
	// fitting データがあればそれに座標を合わせておく
	if( middleNodeManagerWithShape != NULL ){
		kmb::Point3D pt;
		kmb::Vector2WithIntBindings<kmb::nodeIdType>* mapping = reinterpret_cast< kmb::Vector2WithIntBindings<kmb::nodeIdType>* >(rcapRefinerDoc.mesh->getDataBindingsPtr("fitting") );
		if( mapping ){
			kmb::Vector2WithIntBindings<kmb::nodeIdType>::iterator mIter = mapping->begin();
			kmb::Vector2WithIntBindings<kmb::nodeIdType>::iterator endIter = mapping->end();
			while( mIter != endIter ){
				kmb::nodeIdType nodeId = mIter.getId();
				if( middleNodeManagerWithShape->getPointOnSurface(nodeId,pt) > 0 ){
					kmb::Point3D ptOrg;
					rcapRefinerDoc.mesh->getNode(nodeId,ptOrg);
					REVOCAP_Debug("pre fitting node %d : (%f %f %f) => (%f %f %f)\n",
						nodeId, ptOrg.x(), ptOrg.y(), ptOrg.z(), pt.x(), pt.y(), pt.z());
					rcapRefinerDoc.mesh->updateNode( pt.x(), pt.y(), pt.z(), nodeId );
				}
				++mIter;
			}
		}
	}
	REVOCAP_Debug("rcapSetNode64 finish\n");
}

void rcapSetNode32( const int32_t* _num, float32_t* coords, int32_t* globalIds, int32_t* localIds )
{
	REVOCAP_Debug("rcapSetNode32 start\n");
	if(rcapRefinerDoc.mesh == NULL||_num==NULL){
		return;
	}
	int32_t num = *_num;
	kmb::MiddleNodeManagerWithShape* middleNodeManagerWithShape
		= dynamic_cast<kmb::MiddleNodeManagerWithShape*>( rcapRefinerDoc.middleMan );
	if( localIds==NULL || localIds[0] < rcapRefinerDoc.nodeOffset ){
		REVOCAP_Debug("rcapSetNode32 localIds is NULL\n");
		for(int i=0;i<num;++i){
			rcapRefinerDoc.mesh->insertNode(coords[3*i],coords[3*i+1],coords[3*i+2]);
		}
		if( globalIds != NULL && globalIds[0] >= rcapRefinerDoc.nodeOffset ){
			if( middleNodeManagerWithShape != NULL ){
				// fitting data
				// globalId => localId に変換
				printf("REVOCAP_Refiner : convert fitting data to global Ids\n");
				std::map< kmb::nodeIdType, kmb::nodeIdType > imapper;
				for(int i=0;i<num;++i){
					imapper.insert( std::pair< kmb::nodeIdType, kmb::nodeIdType >( globalIds[i]-rcapRefinerDoc.nodeOffset, static_cast<kmb::nodeIdType>(i) ) );
				}
				middleNodeManagerWithShape->replaceNodeIds( imapper );
			}
		}
	}else{
		for(int i=0;i<num;++i){
			rcapRefinerDoc.mesh->insertNodeWithId(coords[3*i],coords[3*i+1],coords[3*i+2],localIds[i]-rcapRefinerDoc.nodeOffset);
		}
		if( globalIds != NULL && globalIds[0] >= rcapRefinerDoc.nodeOffset ){
			if( middleNodeManagerWithShape != NULL ){
				// fitting data
				// globalId => localId に変換
				printf("REVOCAP_Refiner : convert fitting data to global Ids\n");
				std::map< kmb::nodeIdType, kmb::nodeIdType > imapper;
				for(int i=0;i<num;++i){
					imapper.insert( std::pair< kmb::nodeIdType, kmb::nodeIdType >( globalIds[i]-rcapRefinerDoc.nodeOffset, localIds[i]-rcapRefinerDoc.nodeOffset ) );
				}
				middleNodeManagerWithShape->replaceNodeIds( imapper );
			}
		}
	}
	// fitting データがあればそれに座標を合わせておく
	if( middleNodeManagerWithShape != NULL ){
		kmb::Point3D pt;
		kmb::Vector2WithIntBindings<kmb::nodeIdType>* mapping = reinterpret_cast< kmb::Vector2WithIntBindings<kmb::nodeIdType>* >(rcapRefinerDoc.mesh->getDataBindingsPtr("fitting") );
		if( mapping ){
			kmb::Vector2WithIntBindings<kmb::nodeIdType>::iterator mIter = mapping->begin();
			kmb::Vector2WithIntBindings<kmb::nodeIdType>::iterator endIter = mapping->end();
			while( mIter != endIter ){
				kmb::nodeIdType nodeId = mIter.getId();
				if( middleNodeManagerWithShape->getPointOnSurface(nodeId,pt) > 0 ){
					kmb::Point3D ptOrg;
					rcapRefinerDoc.mesh->getNode(nodeId,ptOrg);
					printf("pre fitting node %d : (%f %f %f) => (%f %f %f)\n",
						nodeId, ptOrg.x(), ptOrg.y(), ptOrg.z(), pt.x(), pt.y(), pt.z());
					rcapRefinerDoc.mesh->updateNode( pt.x(), pt.y(), pt.z(), nodeId );
				}
				++mIter;
			}
		}
	}
	REVOCAP_Debug("rcapSetNode32 finish\n");
}

int32_t rcapGetNodeCount( void )
{
	REVOCAP_Debug("rcapGetNodeCount start\n");
	if(rcapRefinerDoc.mesh){
		REVOCAP_Debug("rcapGetNodeCount ContainerType %s\n", rcapRefinerDoc.mesh->getNodes()->getContainerType() );
		return static_cast<int32_t>(rcapRefinerDoc.mesh->getNodeCount());
	}else{
		return 0;
	}
}

void rcapGetNode64( const int32_t* _num, int32_t* localIds, float64_t* coords )
{
	REVOCAP_Debug("rcapGetNode64 start\n");
	if( rcapRefinerDoc.mesh == NULL || _num == NULL ){
		return;
	}
	int32_t num = *_num;
	kmb::Point3D point;
	for(int32_t i=0;i<num;++i){
		rcapRefinerDoc.mesh->getNode(static_cast<kmb::nodeIdType>(localIds[i]-rcapRefinerDoc.nodeOffset),point);
		coords[3*i]   = point.x();
		coords[3*i+1] = point.y();
		coords[3*i+2] = point.z();
	}
	REVOCAP_Debug("rcapGetNode64 finish\n");
}

void rcapGetNode32( const int32_t* _num, int32_t* localIds, float32_t* coords )
{
	REVOCAP_Debug("rcapGetNode32 start\n");
	if( rcapRefinerDoc.mesh == NULL || _num == NULL ){
		return;
	}
	int32_t num = *_num;
	kmb::Point3D point;
	for(int32_t i=0;i<num;++i){
		rcapRefinerDoc.mesh->getNode(static_cast<kmb::nodeIdType>(localIds[i]-rcapRefinerDoc.nodeOffset),point);
		coords[3*i]   = static_cast<float>(point.x());
		coords[3*i+1] = static_cast<float>(point.y());
		coords[3*i+2] = static_cast<float>(point.z());
	}
	REVOCAP_Debug("rcapGetNode32 finish\n");
}

void rcapGetNodeSeq64( const int32_t* _num, const int32_t* _initId, float64_t* coords )
{
	if( rcapRefinerDoc.mesh == NULL || _num == NULL || _initId == NULL ){
		return;
	}
	int32_t num = *_num;
	int32_t initId = *_initId;
	REVOCAP_Debug("rcapGetNodeSeq64 start num = %d initId = %d\n", num, initId);
	int32_t ini = initId - rcapRefinerDoc.nodeOffset;
	kmb::Point3D point;
	for(int32_t i=ini;i<ini+num;++i){
		rcapRefinerDoc.mesh->getNode(static_cast<kmb::nodeIdType>(i),point);
		coords[3*i]   = point.x();
		coords[3*i+1] = point.y();
		coords[3*i+2] = point.z();
	}
	REVOCAP_Debug("rcapGetNodeSeq64 finish\n");
}

void rcapGetNodeSeq32( const int32_t* _num, const int32_t* _initId, float32_t* coords )
{
	if( rcapRefinerDoc.mesh == NULL || _num == NULL || _initId == NULL ){
		return;
	}
	int32_t num = *_num;
	int32_t initId = *_initId;
	REVOCAP_Debug("rcapGetNodeSeq32 start num = %d initId = %d\n", num, initId);
	int32_t ini = initId - rcapRefinerDoc.nodeOffset;
	kmb::Point3D point;
	for(int32_t i=ini;i<ini+num;++i){
		rcapRefinerDoc.mesh->getNode(static_cast<kmb::nodeIdType>(i),point);
		coords[3*i]   = static_cast<float>(point.x());
		coords[3*i+1] = static_cast<float>(point.y());
		coords[3*i+2] = static_cast<float>(point.z());
	}
	REVOCAP_Debug("rcapGetNodeSeq32 finish\n");
}

void rcapSetCADFilename( const char* filename )
{
	REVOCAP_Debug("rcapSetCADFilename start\n");
	if( rcapRefinerDoc.refiner == NULL ){
		printf("REVOCAP_Refiner Warning : call rcapSetCADFilename before rcapInitRefiner\n");
		return;
	}

	kmb::RnfShapeIO rnfshape;
	rnfshape.loadFromFile( filename, rcapRefinerDoc.surfaces );

	// このファイルは節点番号は必ず 0 から
	kmb::RevocapNeutralIO rnf;
	rnf.loadFromRNFFile( filename, rcapRefinerDoc.mesh );

	kmb::MiddleNodeManager* orgMiddleNodeManager = rcapRefinerDoc.middleMan;

	kmb::MiddleNodeManagerWithShape* middleNodeManagerShape = new kmb::MiddleNodeManagerWithShape();
	middleNodeManagerShape->setMappingData( reinterpret_cast< kmb::Vector2WithIntBindings<kmb::nodeIdType>* >(rcapRefinerDoc.mesh->getDataBindingsPtr("fitting") ) );
	middleNodeManagerShape->setSurfaces( &rcapRefinerDoc.surfaces );
	middleNodeManagerShape->setNearestFlag(true);
	rcapRefinerDoc.middleMan = middleNodeManagerShape;

	rcapRefinerDoc.refiner->setMiddleNodeManager(rcapRefinerDoc.middleMan);
	rcapRefinerDoc.boundaryRefiner->setMiddleNodeManager(rcapRefinerDoc.middleMan);

	if( orgMiddleNodeManager ){
		delete orgMiddleNodeManager;
	}
}

void rcapWriteFittingFile( const char* filename )
{
	REVOCAP_Debug("rcapWriteFittingFile start\n");
	if( rcapRefinerDoc.refiner == NULL ){
		printf("REVOCAP_Refiner Warning : call rcapWriteFittingFile before rcapInitRefiner\n");
		return;
	}
	kmb::MiddleNodeManagerWithShape* middleNodeManagerShape = dynamic_cast<kmb::MiddleNodeManagerWithShape*>(rcapRefinerDoc.middleMan);
	if( middleNodeManagerShape == NULL ){
		printf("REVOCAP_Refiner Warning : no shape data in rcapWriteFittingFile\n");
		return;
	}
	if( rcapRefinerDoc.surfaces.size() == 0 ){
		printf("REVOCAP_Refiner Warning : no shape data in rcapWriteFittingFile\n");
		return;
	}
	kmb::RevocapNeutralIO rnfIO;
	kmb::RnfShapeIO rnfshape;
	rnfIO.saveHeader( filename );
	rnfshape.appendSurfaceHeaderToFile( filename );
	std::vector< kmb::Surface3D* >::iterator sIter = rcapRefinerDoc.surfaces.begin();
	while( sIter != rcapRefinerDoc.surfaces.end() ){
		kmb::Surface3D* surf = *sIter;
		if( surf ){
			rnfshape.appendSurfaceToFile( filename, surf );
		}
		++sIter;
	}
	// ファイル出力
	rnfIO.appendDataToRNFFile( filename, rcapRefinerDoc.mesh, "fitting" );
}

void rcapSetSecondFitting( const int32_t* flag )
{
	if( *flag != 0 ){
		rcapRefinerDoc.refiner->setSecondFitting(true);
		rcapRefinerDoc.boundaryRefiner->setSecondFitting(true);
	}else{
		rcapRefinerDoc.refiner->setSecondFitting(false);
		rcapRefinerDoc.boundaryRefiner->setSecondFitting(false);
	}
}

void rcapSetSmoothing( const int32_t* flag )
{
	if( *flag != 0 ){
		rcapRefinerDoc.smoother = new kmb::MeshSmoother( rcapRefinerDoc.mesh );
	}else{
		if( rcapRefinerDoc.smoother ){
			delete rcapRefinerDoc.smoother;
			rcapRefinerDoc.smoother = NULL;
		}
	}
}

void rcapSetPartitionFilename( const char* filename )
{
	kmb::RevocapCouplerIO couplerIO;
	couplerIO.loadLocalNodesFromPartitionFile(filename,rcapRefinerDoc.mesh);
	kmb::DataBindings* data = rcapRefinerDoc.mesh->getDataBindingsPtr("GlobalId");
	kmb::NodeMapperBindings<kmb::nodeIdType>* nodeMapper
		= dynamic_cast<kmb::NodeMapperBindings<kmb::nodeIdType>*>(data);
	kmb::MiddleNodeManagerWithShape* middleNodeManagerWithShape
		= dynamic_cast<kmb::MiddleNodeManagerWithShape*>( rcapRefinerDoc.middleMan );
	if( nodeMapper != NULL && middleNodeManagerWithShape != NULL ){
		middleNodeManagerWithShape->replaceNodeIds( nodeMapper->getNodeMapper() );
	}
}

int32_t rcapRefineElement( const int32_t* _num, const int8_t* _etype, int32_t* nodeArray, int32_t* resultNodeArray )
{
	if( _num == NULL || _etype == NULL ){
		return -1;
	}
	int32_t num = *_num;
	int8_t etype = *_etype;
	REVOCAP_Debug("rcapRefineElement start\n");
	int32_t refinedNum = rcapGetRefineElementCount(_num,_etype);
	// NULL 値を入れた時は個数だけを返す
	if( resultNodeArray == NULL || resultNodeArray[0] < 0 ||
		nodeArray == NULL || nodeArray[0] < 0 )
	{
		REVOCAP_Debug("rcapRefineElement return refined element count %d\n",refinedNum);
		return refinedNum;
	}
	kmb::elementType elemType = static_cast<kmb::elementType>(etype);

	// 引数の配列をそのまま使うため
	// kmb::ElementContainerNArray で実装する
	// 元の要素コンテナは mesh に登録する
	kmb::ElementContainerNArray* orgElements = new kmb::ElementContainerNArray( elemType, num, reinterpret_cast<kmb::nodeIdType*>(nodeArray), false, rcapRefinerDoc.nodeOffset );
	orgElements->setOffsetId( rcapRefinerDoc.maxElementId+1 );
	rcapRefinerDoc.mesh->appendBody( orgElements );

	// 引数の配列をそのまま使うため
	// kmb::ElementContainerNArray で実装する
	// 細分後の要素コンテナはこのメソッド内部変数
	REVOCAP_Debug("rcapRefineElement new element container wrapper %d\n",refinedNum);
	kmb::ElementContainerNArray* refineElements = new kmb::ElementContainerNArray( elemType, refinedNum, reinterpret_cast<kmb::nodeIdType*>(resultNodeArray), true, rcapRefinerDoc.nodeOffset );
	refineElements->setOffsetId( rcapRefinerDoc.maxRefinedElementId+1 );

	rcapRefinerDoc.refiner->refineBody( orgElements, refineElements );
	if( rcapRefinerDoc.smoother ){
		REVOCAP_Debug_X("rcapSetSmoothing\n");
		rcapRefinerDoc.smoother->appendBody( refineElements );
		rcapRefinerDoc.smoother->init();
		rcapRefinerDoc.middleMan->smoothingMiddleNodes( rcapRefinerDoc.smoother );
		rcapRefinerDoc.smoother->commit();
		rcapRefinerDoc.smoother->init();
		rcapRefinerDoc.middleMan->smoothingMiddleNodes( rcapRefinerDoc.smoother );
		rcapRefinerDoc.smoother->commit();
	}

	rcapRefinerDoc.maxElementId = orgElements->getMaxId();
	rcapRefinerDoc.maxRefinedElementId = refineElements->getMaxId();
	REVOCAP_Debug("rcapRefineElement finish\n");
	int32_t count = static_cast<int32_t>(refineElements->getCount());

	// 細分後の要素コンテナは mesh に登録しないので、ここで delete しておく
	delete refineElements;
	return count;
}

int32_t rcapGetRefineElementCount( const int32_t* _num, const int8_t* _etype )
{
	int32_t num = *_num;
	int8_t etype = *_etype;
	int32_t refinedNum = 0;
	switch( etype ){
		case kmb::SEGMENT:		refinedNum = 2*num;	break;
		case kmb::SEGMENT2:		refinedNum = 2*num;	break;
		case kmb::TRIANGLE:		refinedNum = 4*num;	break;
		case kmb::TRIANGLE2:	refinedNum = 4*num;	break;
		case kmb::QUAD:			refinedNum = 4*num;	break;
		case kmb::QUAD2:		refinedNum = 4*num;	break;
		case kmb::TETRAHEDRON:	refinedNum = 8*num;	break;
		case kmb::TETRAHEDRON2:	refinedNum = 8*num;	break;
		case kmb::HEXAHEDRON:	refinedNum = 8*num;	break;
		case kmb::HEXAHEDRON2:	refinedNum = 8*num;	break;
		case kmb::WEDGE:		refinedNum = 8*num;	break;
		case kmb::WEDGE2:		refinedNum = 8*num;	break;
		case kmb::PYRAMID:		refinedNum = 10*num;	break;
		case kmb::PYRAMID2:		refinedNum = 10*num;	break;
		default:	break;
	}
	return refinedNum;
}

int32_t rcapRefineElementMulti( const int32_t* _num, int8_t* etypeArray, int32_t* nodeArray, int32_t* refinedNum, int8_t* resultEtypeArray, int32_t* resultNodeArray )
{
	int32_t num = *_num;
	REVOCAP_Debug("rcapRefineElementMulti start\n");
	if( num <= 0 || etypeArray == NULL || etypeArray[0] < 0 ){
		return 0;
	}
	int32_t refinedNodeArraySize = 0;
	REVOCAP_Debug("rcapRefineElementMulti argument refinedNum = %d\n", *refinedNum);

	// 細分した節点配列の個数と細分した要素の個数を計算する
	refinedNodeArraySize = rcapGetRefineElementMultiCount(_num,etypeArray,refinedNum);
	// 初期化されているときに refinedElementSize = 0 とは限らないことに注意
	if( resultNodeArray == NULL || resultNodeArray[0] < 0 || nodeArray == NULL || nodeArray[0] < 0 ){
		return refinedNodeArraySize;
	}

	int32_t refinedElementSize = *refinedNum;
	std::fill( resultNodeArray, resultNodeArray+refinedNodeArraySize, kmb::nullNodeId );

	REVOCAP_Debug("rcapRefineElementMulti Org Element Count = %d\n", num);
	kmb::ElementContainerMixedArray* orgElements = new kmb::ElementContainerMixedArray( num, reinterpret_cast<char*>(etypeArray), reinterpret_cast<kmb::nodeIdType*>(nodeArray), false, rcapRefinerDoc.nodeOffset );
	orgElements->setOffsetId( rcapRefinerDoc.maxElementId+1 );
	// ここで間接的に etypeArray, nodeArray を Refiner が保持していることに注意する
	rcapRefinerDoc.mesh->appendBody( orgElements );

	if( resultEtypeArray != NULL && resultEtypeArray[0] >= 0 ){
		// resultEtypeArray に細分した要素の型を代入する場合。
		REVOCAP_Debug("rcapRefineElementMulti Refined Element Count = %d\n", refinedElementSize);
		kmb::ElementContainerMixedArray* refineElements = new kmb::ElementContainerMixedArray( refinedElementSize, reinterpret_cast<char*>(resultEtypeArray), reinterpret_cast<kmb::nodeIdType*>(resultNodeArray), true, rcapRefinerDoc.nodeOffset );
		refineElements->setOffsetId( rcapRefinerDoc.maxRefinedElementId+1 );

		rcapRefinerDoc.refiner->refineBody( orgElements, refineElements );
		if( rcapRefinerDoc.smoother ){
			rcapRefinerDoc.smoother->appendBody( refineElements );
			rcapRefinerDoc.smoother->init();
			rcapRefinerDoc.middleMan->smoothingMiddleNodes( rcapRefinerDoc.smoother );
			rcapRefinerDoc.smoother->commit();
			rcapRefinerDoc.smoother->init();
			rcapRefinerDoc.middleMan->smoothingMiddleNodes( rcapRefinerDoc.smoother );
			rcapRefinerDoc.smoother->commit();
		}

		rcapRefinerDoc.maxElementId = orgElements->getMaxId();
		rcapRefinerDoc.maxRefinedElementId = refineElements->getMaxId();
		refinedNodeArraySize = static_cast<int32_t>(refineElements->getNodeTableSize());
		delete refineElements;
	}else{
		// resultEtypeArray は無視する場合。
		REVOCAP_Debug("rcapRefineElementMulti Refined Element Count = %d (no etype)\n", refinedElementSize);
		char* rEtypeArray = new char[ refinedElementSize ];
		kmb::ElementContainerMixedArray* refineElements = new kmb::ElementContainerMixedArray( refinedElementSize, rEtypeArray, reinterpret_cast<kmb::nodeIdType*>(resultNodeArray), true, rcapRefinerDoc.nodeOffset );
		refineElements->setOffsetId( rcapRefinerDoc.maxRefinedElementId+1 );

		rcapRefinerDoc.refiner->refineBody( orgElements, refineElements );
		if( rcapRefinerDoc.smoother ){
			rcapRefinerDoc.smoother->appendBody( refineElements );
			rcapRefinerDoc.smoother->init();
			rcapRefinerDoc.middleMan->smoothingMiddleNodes( rcapRefinerDoc.smoother );
			rcapRefinerDoc.smoother->commit();
			rcapRefinerDoc.smoother->init();
			rcapRefinerDoc.middleMan->smoothingMiddleNodes( rcapRefinerDoc.smoother );
			rcapRefinerDoc.smoother->commit();
		}

		rcapRefinerDoc.maxElementId = orgElements->getMaxId();
		rcapRefinerDoc.maxRefinedElementId = refineElements->getMaxId();
		refinedNodeArraySize = static_cast<int32_t>(refineElements->getNodeTableSize());
		delete refineElements;
		delete[] rEtypeArray;
	}
	REVOCAP_Debug("rcapRefineElementMulti finish\n");
	*refinedNum = refinedElementSize;
	return refinedNodeArraySize;
}

int32_t rcapGetRefineElementMultiCount( const int32_t* _num, int8_t* etypeArray, int32_t* refinedNum )
{
	int32_t num = *_num;
	if( num <= 0 || etypeArray == NULL || etypeArray[0] < 0 ){
		return 0;
	}
	int32_t refinedNodeArraySize = 0;
	int32_t refinedElementSize = 0;

	refinedElementSize = 0;
	for(int32_t i=0;i<num;++i){
		kmb::elementType etype = static_cast<kmb::elementType>( etypeArray[i] );
		switch( etype ){
		case kmb::SEGMENT:
			refinedElementSize += 2;
			refinedNodeArraySize += 2*2;
			break;
		case kmb::SEGMENT2:
			refinedElementSize += 2;
			refinedNodeArraySize += 2*3;
			break;
		case kmb::TRIANGLE:
			refinedElementSize += 4;
			refinedNodeArraySize += 4*3;
			break;
		case kmb::TRIANGLE2:
			refinedElementSize += 4;
			refinedNodeArraySize += 4*6;
			break;
		case kmb::QUAD:
			refinedElementSize += 4;
			refinedNodeArraySize += 4*4;
			break;
		case kmb::QUAD2:
			refinedElementSize += 4;
			refinedNodeArraySize += 4*8;
			break;
		case kmb::TETRAHEDRON:
			refinedElementSize += 8;
			refinedNodeArraySize += 8*4;
			break;
		case kmb::TETRAHEDRON2:
			refinedElementSize += 8;
			refinedNodeArraySize += 8*10;
			break;
		case kmb::HEXAHEDRON:
			refinedElementSize += 8;
			refinedNodeArraySize += 8*8;
			break;
		case kmb::HEXAHEDRON2:
			refinedElementSize += 8;
			refinedNodeArraySize += 8*20;
			break;
		case kmb::WEDGE:
			refinedElementSize += 8;
			refinedNodeArraySize += 8*6;
			break;
		case kmb::WEDGE2:
			refinedElementSize += 8;
			refinedNodeArraySize += 8*15;
			break;
		case kmb::PYRAMID:
			refinedElementSize += 10;
			refinedNodeArraySize += (6*5 + 4*4);
			break;
		case kmb::PYRAMID2:
			refinedElementSize += 10;
			refinedNodeArraySize += (6*13 + 4*10);
			break;
		default:
			break;
		}
	}
	*refinedNum = refinedElementSize;
	return refinedNodeArraySize;
}

void rcapCommit(void)
{
	// ここで境界面に制限した条件の更新を行う
	if(rcapRefinerDoc.boundaryRefiner && rcapRefinerDoc.boundaryRefiner->getDataCount() > 0){
		kmb::BoundaryExtractor bext;
		bext.setMesh( rcapRefinerDoc.mesh );
		kmb::bodyIdType bodyCount = rcapRefinerDoc.mesh->getBodyCount();
		for(kmb::bodyIdType bodyId = 0; bodyId < bodyCount; ++bodyId){
			bext.appendBody(bodyId);
		}
		kmb::bodyIdType boundaryId = bext.getBoundary();
		REVOCAP_Debug("rcapCommit boundary surface extract %zd\n", rcapRefinerDoc.mesh->getElementCount(boundaryId) );
		kmb::ElementContainer* boundaryBody = rcapRefinerDoc.mesh->getBodyPtr(boundaryId);
		REVOCAP_Debug("boundary element count => %zd\n", boundaryBody->getCount());
		rcapRefinerDoc.boundaryRefiner->refineBody( boundaryBody, NULL );
		rcapRefinerDoc.boundaryRefiner->commitData();
	}
	if(rcapRefinerDoc.refiner){
		rcapRefinerDoc.refiner->commitData();
	}
}

int8_t rcapGetOriginal( int32_t localNodeId, int32_t* originalNodeArray )
{
	if( rcapRefinerDoc.refiner != NULL ){
		kmb::elementType etype = rcapRefinerDoc.refiner->getOriginal(localNodeId-rcapRefinerDoc.nodeOffset,reinterpret_cast<kmb::nodeIdType*>(originalNodeArray));
		int len = kmb::Element::getNodeCount( static_cast<kmb::elementType>(etype) );
		for(int i=0;i<len;++i){
			originalNodeArray[i] += rcapRefinerDoc.nodeOffset;
		}
		return static_cast<int8_t>(etype);
	}
	return RCAP_UNKNOWNTYPE;
}

int32_t rcapGetMiddle( int8_t etype, int32_t* originalNodeArray )
{
	int len = kmb::Element::getNodeCount( static_cast<kmb::elementType>(etype) );
	kmb::Element* element = kmb::Element::create( static_cast<kmb::elementType>(etype) );
	for(int i=0;i<len;++i){
		element->setNodeId( i, originalNodeArray[i] - rcapRefinerDoc.nodeOffset );
	}
	kmb::nodeIdType nodeId = rcapRefinerDoc.refiner->getMiddle( *element );
	delete element;
	if( nodeId == kmb::nullNodeId ){
		return -1;
	}else{
		return nodeId + rcapRefinerDoc.nodeOffset;
	}
}

void rcapAppendNodeGroup( const char dataname[80], const int32_t* num, const int32_t* nodeArray )
{
	REVOCAP_Debug("Append Node Group [%s] %d\n",dataname,*num);
	kmb::DataBindings* data = rcapRefinerDoc.mesh->createDataBindings( dataname, kmb::DataBindings::NodeGroup, kmb::PhysicalValue::None, "NG" );
	if( data ){
		for(int i=0;i<*num;++i){
			data->addId( static_cast<kmb::nodeIdType>(nodeArray[i]) - rcapRefinerDoc.nodeOffset );
		}
		rcapRefinerDoc.refiner->appendData( dataname, "NG" );
	}
}

int32_t rcapGetNodeGroupCount( const char dataname[80] )
{
	return static_cast<int32_t>(rcapRefinerDoc.mesh->getIdCount( dataname, "NG" ));
}

void rcapGetNodeGroup( const char dataname[80], const int32_t* num, int32_t* nodeArray )
{
	REVOCAP_Debug("Get Node Group [%s]\n",dataname);
	const kmb::DataBindings* data = rcapRefinerDoc.mesh->getDataBindingsPtr( dataname, "NG" );
	if( data && data->getBindingMode() == kmb::DataBindings::NodeGroup ){
		kmb::DataBindings::const_iterator dIter = data->begin();
		int32_t i = 0;
		while( !dIter.isFinished() ){
			if( i > *num ){
				break;
			}
			nodeArray[i] = dIter.getId() + rcapRefinerDoc.nodeOffset;
			++dIter;
			++i;
		}
	}
}

void rcapAppendBNodeGroup( const char dataname[80], const int32_t* num, int32_t* nodeArray )
{
	REVOCAP_Debug("Append Boundary Node Group [%s]\n",dataname);
	kmb::DataBindings* data = rcapRefinerDoc.mesh->createDataBindings( dataname, kmb::DataBindings::NodeGroup, kmb::PhysicalValue::None, "BNG" );
	if( data ){
		for(int32_t i=0;i<*num;++i){
			data->addId( static_cast<kmb::nodeIdType>(nodeArray[i]) - rcapRefinerDoc.nodeOffset );
		}
		rcapRefinerDoc.boundaryRefiner->appendData( dataname, "BNG" );
	}
}

int32_t rcapGetBNodeGroupCount( const char dataname[80] )
{
	return static_cast<int32_t>(rcapRefinerDoc.mesh->getIdCount( dataname, "BNG" ));
}

void rcapGetBNodeGroup( const char dataname[80], const int32_t* num, int32_t* nodeArray )
{
	REVOCAP_Debug("Get Boundary Node Group [%s]\n",dataname);
	const kmb::DataBindings* data = rcapRefinerDoc.mesh->getDataBindingsPtr( dataname, "BNG" );
	if( data && data->getBindingMode() == kmb::DataBindings::NodeGroup ){
		kmb::DataBindings::const_iterator dIter = data->begin();
		int32_t i = 0;
		while( !dIter.isFinished() ){
			if( i > *num ){
				break;
			}
			nodeArray[i] = dIter.getId() + rcapRefinerDoc.nodeOffset;
			++dIter;
			++i;
		}
	}
}

void rcapAppendBNodeVarInt( const char dataname[80], const int32_t* num, int32_t* nodeArray, int32_t* nodeVars )
{
	REVOCAP_Debug("Append Boundary Node Variable Int [%s]\n",dataname);
	kmb::DataBindings* data = new kmb::IntegerMapBindings();
	if( data ){
		rcapRefinerDoc.mesh->setDataBindingsPtr( dataname, data, "BNVI" );
		for(int32_t i=0;i<*num;++i){
			long l = static_cast<long>(nodeVars[i]);
			data->setPhysicalValue( static_cast<kmb::nodeIdType>(nodeArray[i]) - rcapRefinerDoc.nodeOffset, &l );
		}
		rcapRefinerDoc.boundaryRefiner->appendData( dataname, "BNVI" );
	}
}

int32_t rcapGetBNodeVarIntCount( const char dataname[80] )
{
	return static_cast<int32_t>(rcapRefinerDoc.mesh->getIdCount( dataname, "BNVI" ));
}

void rcapGetBNodeVarInt( const char dataname[80], const int32_t* num, int32_t* nodeArray, int32_t* nodeVars  )
{
	REVOCAP_Debug("Get Boundary Node Variable Int [%s]\n",dataname);
	const kmb::DataBindings* data = rcapRefinerDoc.mesh->getDataBindingsPtr( dataname, "BNVI" );
	if( data && data->getBindingMode() == kmb::DataBindings::NodeVariable
		&& data->getValueType() == kmb::PhysicalValue::Integer )
	{
		kmb::DataBindings::const_iterator dIter = data->begin();
		int32_t i = 0;
		while( !dIter.isFinished() ){
			if( i > *num ){
				break;
			}
			long l = 0;
			nodeArray[i] = dIter.getId() + rcapRefinerDoc.nodeOffset;
			dIter.getValue(&l);
			nodeVars[i] = static_cast< int32_t >(l);
			++dIter;
			++i;
		}
	}
}

void rcapAppendElementGroup( const char dataname[80], const int32_t* num, int32_t* elementArray )
{
	REVOCAP_Debug("Append Element Group [%s]\n",dataname);
	kmb::DataBindings* data = rcapRefinerDoc.mesh->createDataBindings( dataname, kmb::DataBindings::ElementGroup, kmb::PhysicalValue::None, "EG" );
	if( data ){
		for(int32_t i=0;i<*num;++i){
			data->addId( static_cast<kmb::elementIdType>(elementArray[i]-rcapRefinerDoc.elementOffset) );
		}
		rcapRefinerDoc.refiner->appendData( dataname, "EG" );
	}
}

int32_t rcapGetElementGroupCount( const char dataname[80] )
{
	return static_cast<int32_t>(rcapRefinerDoc.mesh->getIdCount( dataname, "EG" ));
}

void rcapGetElementGroup( const char dataname[80], const int32_t* num, int32_t* elementArray )
{
	const kmb::DataBindings* data = rcapRefinerDoc.mesh->getDataBindingsPtr( dataname, "EG" );
	if( data && data->getBindingMode() == kmb::DataBindings::ElementGroup){
		kmb::DataBindings::const_iterator dIter = data->begin();
		int32_t i = 0;
		while( !dIter.isFinished() ){
			if( i > *num ){
				break;
			}
			elementArray[i] = dIter.getId() + rcapRefinerDoc.elementOffset;
			++dIter;
			++i;
		}
	}
}

void rcapAppendFaceGroup( const char dataname[80], const int32_t* num, int32_t* faceArray )
{
	REVOCAP_Debug("Append Face Group [%s]\n",dataname);
	kmb::DataBindings* data = rcapRefinerDoc.mesh->createDataBindings( dataname, kmb::DataBindings::FaceGroup, kmb::PhysicalValue::None, "FG" );
	if( data ){
		for(int32_t i=0;i<*num;++i){
			kmb::Face f( static_cast<kmb::elementIdType>(faceArray[2*i]-rcapRefinerDoc.elementOffset), faceArray[2*i+1] );
			data->addId( f );
		}
		rcapRefinerDoc.refiner->appendData( dataname, "FG" );
	}
}

int32_t rcapGetFaceGroupCount( const char dataname[80] )
{
	return static_cast<int32_t>(rcapRefinerDoc.mesh->getIdCount( dataname, "FG" ));
}

void rcapGetFaceGroup( const char dataname[80], const int32_t* num, int32_t* faceArray )
{
	const kmb::DataBindings* data = rcapRefinerDoc.mesh->getDataBindingsPtr( dataname, "FG" );
	if( data && data->getBindingMode() == kmb::DataBindings::FaceGroup ){
		kmb::DataBindings::const_iterator dIter = data->begin();
		int32_t i = 0;
		kmb::Face f;
		while( !dIter.isFinished() ){
			if( i > *num ){
				break;
			}
			dIter.getFace( f );
			faceArray[2*i] = f.getElementId() + rcapRefinerDoc.elementOffset;
			faceArray[2*i+1] = f.getLocalFaceId();
			++dIter;
			++i;
		}
	}
}

void rcapSetInterpolateMode( const char mode[32] )
{
	rcapRefinerDoc.refiner->setInterpolationMode(mode);
	rcapRefinerDoc.boundaryRefiner->setInterpolationMode(mode);
}

void rcapGetInterpolateMode( char mode[32] )
{
	const char* m = rcapRefinerDoc.refiner->getInterpolationMode();
	strcpy(mode,m);
}

// local function for quality report
void rcapQualityReport_local( const char name[80], std::ostream &output )
{
	output << "===== REVOCAP Refiner Mesh Quality Report =====" << std::endl;
	if( strcmp(name,"AspectRatio")==0 ){
		kmb::MinMaxWithId<kmb::elementIdType> minmax;
		kmb::Point3DContainer* nodes = rcapRefinerDoc.mesh->getNodes();
		kmb::ElementEvaluator evaluator(nodes);
		kmb::bodyIdType bCount = rcapRefinerDoc.mesh->getBodyCount();
		for(kmb::bodyIdType bodyId = 0;bodyId<bCount;++bodyId){
			kmb::ElementContainer* body = rcapRefinerDoc.mesh->getBodyPtr(bodyId);
			if( body && body->isUniqueDim(3) ){
				kmb::ElementContainer::iterator eIter = body->begin();
				double ratio = 0.0;
				while( !eIter.isFinished() ){
					ratio = evaluator.getAspectRatio( eIter );
					minmax.update(ratio,eIter.getId());
					++eIter;
				}
			}
		}
		output << "Aspect Ratio" << std::endl;
		output << "max value " << minmax.getMax() << " at " << minmax.getMaxId() << std::endl;
		output << "min value " << minmax.getMin() << " at " << minmax.getMinId() << std::endl;
	}
	output << "===== REVOCAP Refiner Mesh Quality Report End =====" << std::endl;
}

void rcapQualityReport( const char mode[80], const char* filename )
{
	if( filename && strlen(filename) > 0 ){
		std::ofstream output( filename, std::ios_base::out );
		if( !output.fail() ){
			rcapQualityReport_local(mode,output);
			output.close();
			return;
		}
	}
	rcapQualityReport_local(mode,std::cerr);
	return;
}

/* rcapxxx_  すべて小文字 */
#if defined FORTRAN90 || defined FORTRAN_CALL_C_DOWNCASE_
//void rcapgetversion_( void ){ rcapGetVersion(); }
//void rcapinitrefiner_( int32_t* nodeOffset, int32_t* elementOffset ){ rcapInitRefiner(*nodeOffset,*elementOffset); }
//void rcapclearrefiner_( void ){ rcapClearRefiner(); }
//void rcaptermrefiner_( void ){ rcapTermRefiner(); }

//void rcapsetcadfilename_( const char* filename ){ rcapSetCADFilename( filename ); }
//void rcapsetsecondfitting_( int32_t* flag ){ rcapSetSecondFitting( *flag ); }
//void rcapsetsmoothing_( int32_t* flag ){ rcapSetSmoothing( *flag ); }
//void rcapsetpartitionfilename_( const char* filename ){ rcapSetPartitionFilename( filename ); }

//void rcapsetnode64_( const int32_t &num, float64_t* coords, int32_t* globalIds, int32_t* localIds ){
//	rcapSetNode64(num,coords,globalIds,localIds);
//}
//void rcapsetnode32_( const int32_t &num, float32_t* coords, int32_t* globalIds, int32_t* localIds ){
//	rcapSetNode32(num,coords,globalIds,localIds);
//}
//int32_t rcapgetnodecount_( void ){ return static_cast<int32_t>(rcapGetNodeCount()); }
//void rcapgetnode64_( int32_t* num, int32_t* localIds, float64_t* coords ){
//	rcapGetNode64(static_cast<size_t>(*num),localIds,coords);
//}
//void rcapgetnode32_( int32_t* num, int32_t* localIds, float32_t* coords ){
//	rcapGetNode32(static_cast<size_t>(*num),localIds,coords);
//}
//void rcapgetnodeseq64_( int32_t* num, int32_t* initId, float64_t* coords ){
//	rcapGetNodeSeq64(static_cast<size_t>(*num),static_cast<size_t>(*initId),coords);
//}
//void rcapgetnodeseq32_( int32_t* num, int32_t* initId, float32_t* coords ){
//	rcapGetNodeSeq32(static_cast<size_t>(*num),static_cast<size_t>(*initId),coords);
//}

//int32_t rcapgetrefineelementcount_( int32_t* num, int8_t* etype ){
//	return static_cast<int32_t>(rcapGetRefineElementCount(static_cast<size_t>(*num),*etype));
//}

//int32_t rcaprefineelement_( int32_t* num, int8_t* etype, int32_t* nodeArray, int32_t* resultNodeArray ){
//	return rcapRefineElement(*num,*etype,nodeArray,resultNodeArray);
//}

//int32_t rcapgetrefineelementmulticount_( int32_t* num, int8_t* etypeArray, int32_t* refinedNum ){
//	size_t s = static_cast<size_t>(*refinedNum);
//	int32_t res = static_cast<int32_t>(rcapGetRefineElementMultiCount(static_cast<size_t>(*num),etypeArray,&s));
//	*refinedNum = static_cast<int32_t>(s);
//	return res;
//}

//int32_t rcaprefineelementmulti_( int32_t* num, int8_t* etypeArray, int32_t* nodeArray, int32_t* refinedNum, int8_t* resultEtypeArray, int32_t* resultNodeArray ){
//	size_t s = static_cast<size_t>(*refinedNum);
//	int32_t res = static_cast<int32_t>(rcapRefineElementMulti(static_cast<size_t>(*num),etypeArray,nodeArray,&s,resultEtypeArray,resultNodeArray));
//	*refinedNum = static_cast<int32_t>(s);
//	return res;
//}

//void rcapcommit_( void ){
//	rcapCommit();
//}

//void rcapappendnodegroup_( const char dataname[80], const int32_t &num, const int32_t* nodeArray ){
//	rcapAppendNodeGroup(dataname,num,nodeArray);
//}
//int32_t rcapgetnodegroupcount_( const char dataname[80] ){
//	return static_cast<int32_t>(rcapGetNodeGroupCount(dataname));
//}
//void rcapgetnodegroup_( const char dataname[80], int32_t* num, int32_t* nodeArray ){
//	rcapGetNodeGroup( dataname, static_cast<size_t>(*num), nodeArray );
//}

//void rcapappendbnodegroup_( const char dataname[80], int32_t* num, int32_t* nodeArray ){
//	rcapAppendBNodeGroup( dataname, static_cast<size_t>(*num), nodeArray );
//}

//int32_t rcapgetbnodegroupcount_( const char dataname[80] ){
//	return static_cast< int32_t >(rcapGetBNodeGroupCount( dataname ));
//}

//void rcapgetbnodegroup_( const char dataname[80], int32_t* num, int32_t* nodeArray ){
//	rcapGetBNodeGroup( dataname, static_cast<size_t>(*num), nodeArray );
//}

//void rcapappendbnodevarint_( const char dataname[80], int32_t* num, int32_t* nodeArray, int32_t* nodeVars ){
//	rcapAppendBNodeVarInt( dataname, static_cast<size_t>(*num), nodeArray, nodeVars );
//}

//int32_t rcapgetbnodevarintcount_( const char dataname[80] ){
//	return static_cast< int32_t >(rcapGetBNodeVarIntCount( dataname ));
//}

//void rcapgetbnodevarint_( const char dataname[80], int32_t* num, int32_t* nodeArray, int32_t* nodeVars  ){
//	rcapGetBNodeVarInt( dataname, static_cast<size_t>(*num), nodeArray, nodeVars  );
//}

//void rcapappendelementgroup_( const char dataname[80], int32_t* num, int32_t* elementArray ){
//	rcapAppendElementGroup(dataname,static_cast<size_t>(*num),elementArray);
//}

//int32_t rcapgetelementgroupcount_( const char dataname[80] ){
//	return static_cast<int32_t>(rcapGetElementGroupCount(dataname));
//}

//void rcapgetelementgroup_( const char dataname[80], int32_t* num, int32_t* elementArray ){
//	rcapGetElementGroup( dataname, static_cast<size_t>(*num), elementArray );
//}

//void rcapappendfacegroup_( const char dataname[80], int32_t* num, int32_t* faceArray ){
//	rcapAppendFaceGroup(dataname,static_cast<size_t>(*num),faceArray);
//}

//int32_t rcapgetfacegroupcount_( const char dataname[80] ){
//	return static_cast<int32_t>(rcapGetFaceGroupCount(dataname));
//}

//void rcapgetfacegroup_( const char dataname[80], int32_t* num, int32_t* faceArray ){
//	rcapGetElementGroup( dataname, static_cast<size_t>(*num), faceArray );
//}

//void rcapsetinterpolatemode_( const char mode[32] ){
//	rcapSetInterpolateMode(mode);
//}
//void rcapgetinterpolatemode_( char mode[32] ){
//	rcapGetInterpolateMode(mode);
//}

//int8_t rcapgetoriginal_( int32_t* localNodeId, int32_t* originalNodeArray ){
//	return rcapGetOriginal(*localNodeId,originalNodeArray);
//}

//int32_t rcapgetmiddle_( int8_t *etype, int32_t* originalNodeArray ){
//	return rcapGetMiddle(*etype,originalNodeArray);
//}

//void rcapqualityreport_( const char mode[80], const char* filename ){
//	rcapQualityReport(mode,filename);
//}

#endif
