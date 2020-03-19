#include "RevocapIO/kmbFrontISTRIO_Generics.h"

#include "MeshDB/kmbMeshData.h"

template int kmb::FrontISTRIO::loadMesh<>(std::string filename, kmb::MeshData* mesh);
template int kmb::FrontISTRIO::saveMesh<>(std::string filename, const kmb::MeshData* mesh);

