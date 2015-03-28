/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : MicroAVSIO                                              #
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







#pragma once

#include "MeshDB/kmbTypes.h"
#include <fstream>

namespace kmb{

class MeshData;
class StructuredGrid;

class MicroAVSIO
{
private:
	enum dataType{
		DATA = 0x01,
		GEOM = 0x02,
		DATA_GEOM = 0x03,
		Undefined
	};
	dataType dtype;
	kmb::nodeIdType nodeOffset;
	kmb::elementIdType elementOffset;

	bool asVector3;
	bool summary;

	int version;
	int nodeCount;
	int elementCount;
	int nodeDimCount;
	int elementDimCount;
protected:



	int getVersion(std::ifstream &input) const;

	int readHeader(std::ifstream &input);
	int skipGeom(std::ifstream &input);
	int readGeom(std::ifstream &input,kmb::MeshData* mesh);
	int readData(std::ifstream &input,kmb::MeshData* mesh);

	void getDataName(std::string &line);
#ifndef REVOCAP_SUPPORT_RUBY

	int readCoord(int n,std::string filename,std::string filetype,int skip,int offset,int stride,kmb::StructuredGrid* grid);
	int readVariable(std::string label,int n,std::string filename,std::string filetype,int skip,int offset,int stride,kmb::StructuredGrid* grid);
#endif
public:
	MicroAVSIO(void);
	virtual ~MicroAVSIO(void);
	int loadFromFile(const char* filename,kmb::MeshData* mesh);
#ifndef REVOCAP_SUPPORT_RUBY
	int loadFromFile(const char* filename,kmb::StructuredGrid* grid);
#endif

	int loadPostFromFile(const char* filename,kmb::MeshData* mesh);
	int saveToFile(const char* filename,kmb::MeshData* mesh);
	int saveToFile_V8(const char* filename,kmb::MeshData* mesh);

	void setReadAsVector3( bool flag );

	void setSummary( bool flag );
};

}
