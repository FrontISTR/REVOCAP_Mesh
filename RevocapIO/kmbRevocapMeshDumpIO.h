/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : RevocapMeshDumpIO                                       #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
# Software Name : RevocapMesh version 1.2                              #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2008/03/14     #
#                                           T. Takeda   2008/03/14     #
#                                           K. Amemiya  2008/03/14     #
#                                                                      #
#      Contact Address: RSS21 Project, IIS, The University of Tokyo    #
#                                                                      #
#      "Innovative General-Purpose Coupled Analysis System"            #
#                                                                      #
----------------------------------------------------------------------*/
#pragma once


#include <istream>
#include <ostream>
#include "RevocapIO/kmbDefineIO.h"

namespace kmb{

class MeshData;
class DataBindings;

class RevocapMeshDumpIO
{
public:

	struct SizeInfo{
		unsigned char nodeCount;
		unsigned char nodeId;
		unsigned char xyz;
		unsigned char bodyCount;
		unsigned char bodyId;
		unsigned char elementCount;
		unsigned char elementId;
		unsigned char elementType;
		unsigned char dataCount;
		unsigned char bindingMode;
		unsigned char idCount;
		unsigned char valueType;
		unsigned char value;
		unsigned char intValue;
		unsigned char faceId;
	};

	RevocapMeshDumpIO(void);
	~RevocapMeshDumpIO(void);

	static bool checkTypeSize(void);
	int loadMeshFromFile(const char* filename,kmb::MeshData* mesh);

	int saveMeshToFile(const char* filename,const kmb::MeshData* mesh);



private:
	static const char* headerString;
	static const int headerLength;
protected:

	struct SizeInfo sInfo;
	bool getVersion(int &ver);
	bool setVersion(const int ver);



















	kmb::RevocapIO::ErrorCode getVersion(int &ver,std::istream &input);
	kmb::RevocapIO::ErrorCode setVersion(const int ver,std::ostream &output);
	kmb::RevocapIO::ErrorCode loadNodeData(kmb::MeshData* mesh,std::istream &input);
	kmb::RevocapIO::ErrorCode saveNodeData(const kmb::MeshData* mesh,std::ostream &output);
	kmb::RevocapIO::ErrorCode loadElementData(kmb::MeshData* mesh,std::istream &input);
	kmb::RevocapIO::ErrorCode saveElementData(const kmb::MeshData* mesh,std::ostream &output);
	kmb::RevocapIO::ErrorCode loadBindingData(kmb::MeshData* mesh,std::istream &input);
	kmb::RevocapIO::ErrorCode saveBindingData(const kmb::MeshData* mesh,std::ostream &output);
	kmb::RevocapIO::ErrorCode loadData(kmb::DataBindings* data,std::istream &input,size_t count);
	kmb::RevocapIO::ErrorCode saveData(const kmb::DataBindings* data,std::ostream &output);

	void setSizeInfoVersion1(void);
};

}
