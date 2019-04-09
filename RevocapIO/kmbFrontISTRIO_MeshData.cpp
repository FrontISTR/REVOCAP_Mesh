#include "RevocapIO/kmbFrontISTRIO_Generics.h"

#include "MeshDB/kmbMeshData.h"

template int kmb::FrontISTRIO::loadMesh<>(std::string filename, kmb::MeshData* mesh);
template int kmb::FrontISTRIO::saveMesh<>(std::string filename, const kmb::MeshData* mesh);
template void kmb::FrontISTRIO::writeMaterial<>(std::ostream& output, const kmb::MeshData* mesh, std::string mat);

