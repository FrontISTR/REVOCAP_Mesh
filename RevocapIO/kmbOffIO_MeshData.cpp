/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : OffIO                                                   #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2015/12/15     #
#                                                                      #
#      Contact Address: RSS21 Project, IIS, The University of Tokyo    #
#                                                                      #
#      "Innovative General-Purpose Coupled Analysis System"            #
#                                                                      #
----------------------------------------------------------------------*/
#include "RevocapIO/kmbOffIO_impl.hpp"
#include "MeshDB/kmbMeshData.h"

template int kmb::OffIO::loadPatch<>(const char* filename, kmb::MeshData* patch);
template int kmb::OffIO::savePatch<>(const char* filename, const kmb::MeshData* patch);
