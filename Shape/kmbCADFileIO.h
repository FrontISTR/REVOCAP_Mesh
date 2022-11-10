/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : CADFileIO                                               #
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

#ifdef OPENCASCADE

#include <fstream>

class TopoDS_Shape;

namespace kmb{

class ShapeData;

class CADFileIO
{
public:
	CADFileIO(void);
	virtual ~CADFileIO(void);
	int readIGES(const char* filename,kmb::ShapeData* shapeData);
	int readSTEP(const char* filename,kmb::ShapeData* shapeData);
	int readSTL(const char* filename,kmb::ShapeData* shapeData);
	int readOCC(const char* filename,kmb::ShapeData* shapeData);
	int writeIGES(const char* filename,kmb::ShapeData* shapeData);
	int writeSTEP(const char* filename,kmb::ShapeData* shapeData);
	int writeSTL(const char* filename,kmb::ShapeData* shapeData);
	int writeOCC(const char* filename,kmb::ShapeData* shapeData);

	int saveToBrepRNF(const char* filename,const kmb::ShapeData* shapeData) const;
	int writeShapeHeader(std::ofstream &output) const;
	int writeShape(std::ofstream &output,const TopoDS_Shape &shape,int layer=0) const;
};

}

#endif
