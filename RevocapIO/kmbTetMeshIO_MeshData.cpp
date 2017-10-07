#include "RevocapIO/kmbTetMeshIO_impl.hpp"
#include "MeshDB/kmbMeshData.h"

template int kmb::TetMeshIO::loadMesh<kmb::MeshData>(std::string filename, kmb::MeshData* mesh);
template int kmb::TetMeshIO::saveMesh<kmb::MeshData>(std::string filename, const kmb::MeshData* mesh);
