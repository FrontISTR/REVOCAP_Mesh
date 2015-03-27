/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : ContainerOnDisk                                         #
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
#ifdef REVOCAP_SUPPORT_DUMP_CONTAINER
/*
 * �o�C�i���t�@�C����Ƀf�[�^���i�[���ăX�g���[���Ńf�[�^�𑖍�����
 * �t�@�C���A�N�Z�X�̔r���������܂��i�t�@�C�������C���X�^���X��1��1�Ή��ɂ���Ηǂ��j
 *
 * �t�@�C���F�w�b�_�̎d�l
 * MAGIC NUMBER        (4byte)  char * 4
 * ������              (80byte) char * 80
 * �w�b�_�̑傫��      (4byte)  unsinged int : 4 + 80 + 4 + 4 + 4
 * Unit �̑傫��       (4byte)  unsigned int : double*3 �Ȃ�� 8*3 = 24 �ς̏ꍇ�� 0
 * Unit �̌�         (4byte)  unsigned int
 */

#pragma once

#include <fstream>

namespace kmb{

class ContainerOnDisk
{
private:
	static unsigned int HEADER_MAGIC_NUMBER;
	unsigned int headerSize;
	std::fstream fileBindings;
protected:
	unsigned int dataUnitSize;
	unsigned long dataSize;
	unsigned long indexCounter;
	long maxIndex;
public:
	ContainerOnDisk(void);
	virtual ~ContainerOnDisk(void);
	bool initialize(const char* filename, const char headerString[80],unsigned int dataUnitSize,unsigned long dataSize);
	void terminate(void);


	bool setData( unsigned long index, const char* data, unsigned long num=1 );
	bool getData( unsigned long index,       char* data, unsigned long num=1 ) const;



	bool setDataOffset( unsigned long index, const char* data, unsigned int offset, unsigned int size );
	bool getDataOffset( unsigned long index,       char* data, unsigned int offset, unsigned int size ) const;

	unsigned long getSize(void) const;
	long getMaxIndex(void) const;
	bool good(void) const;
	void debug(void);
};

}

#endif
