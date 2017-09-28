/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : STLIO                                                   #
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
 * 同じ座標を同じ節点番号に割り当てながら読み込む
 * この処理は遅いので、StereolithographyIO で読み込んで
 * メモリ上で節点番号の圧縮処理を行うことを推奨する
 */
#pragma once

namespace kmb{

class MeshData;

class STLIO
{
private:
	enum fileType{
		BINARY,
		ASCII,
		UNKNOWN
	};
	double thresh;
public:
	STLIO(void);
	~STLIO(void);
	int loadFromFile(const char* filename,kmb::MeshData* mesh);
	int saveToFile(const char* filename,const kmb::MeshData* mesh);
protected:
	fileType checkFormat(const char* filename);
	int loadBinaryFile(const char* filename,kmb::MeshData* mesh);
	int loadAsciiFile(const char* filename,kmb::MeshData* mesh);
};

}
