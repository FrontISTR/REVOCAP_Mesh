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
	// äÙâΩåvéZÇ…Ç®ÇØÇÈ 0 Ç∆Ç›Ç»ÇπÇÈëÂÇ´Ç≥
	static const double geometric;
	// êîílåvéZÇ…Ç®ÇØÇÈ 0 Ç∆å©Ç»ÇπÇÈëÂÇ´Ç≥
	static const double numeric;
};

}
