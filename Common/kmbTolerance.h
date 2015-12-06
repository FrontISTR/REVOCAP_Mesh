/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : Tolerance                                               #
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

namespace kmb{

class Tolerance
{
public:
	// �􉽌v�Z�ɂ����� 0 �Ƃ݂Ȃ���傫��
	static const double geometric;
	// ���l�v�Z�ɂ����� 0 �ƌ��Ȃ���傫��
	static const double numeric;
};

}
