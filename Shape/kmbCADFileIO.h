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

class TopoDS_Shape;

namespace kmb{

class ShapeData;

class CADFileIO
{
public:
	CADFileIO(void);
	virtual ~CADFileIO(void);
	void readIGES(const char* filename,kmb::ShapeData* shapeData);
	void readSTEP(const char* filename,kmb::ShapeData* shapeData);
	void readSTL(const char* filename,kmb::ShapeData* shapeData);
	void readOCC(const char* filename,kmb::ShapeData* shapeData);
	void writeIGES(const char* filename,kmb::ShapeData* shapeData);
	void writeSTEP(const char* filename,kmb::ShapeData* shapeData);
	void writeSTL(const char* filename,kmb::ShapeData* shapeData);
	void writeOCC(const char* filename,kmb::ShapeData* shapeData);

	int saveToBrepRNF(const char* filename,const kmb::ShapeData* shapeData) const;
	int writeShapeHeader(std::ofstream &output) const;
	int writeShape(std::ofstream &output,const TopoDS_Shape &shape,int layer=0) const;
};

}

#endif
