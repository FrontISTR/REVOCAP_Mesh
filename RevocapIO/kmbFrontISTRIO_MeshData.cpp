#include "RevocapIO/kmbFrontISTRIO_impl.hpp"

#include "MeshDB/kmbMeshData.h"

template int kmb::FrontISTRIO::loadMesh<>(std::string filename, kmb::MeshData* mesh);
template int kmb::FrontISTRIO::saveMesh<>(std::string filename, const kmb::MeshData* mesh);
template int kmb::FrontISTRIO::loadResult<>(std::string filename, kmb::MeshData* mesh);
template int kmb::FrontISTRIO::loadResultAscii<>(std::string filename, kmb::MeshData* mesh);
template int kmb::FrontISTRIO::loadResultBinary<>(std::string filename, kmb::MeshData* mesh);

