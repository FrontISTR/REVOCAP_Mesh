#include "RevocapIO/kmbStereolithographyIO_impl.hpp"
#include "MeshDB/kmbMeshData.h"

template int kmb::StereolithographyIO::loadPatch<>(std::string filename, kmb::MeshData* patch);
template int kmb::StereolithographyIO::savePatch<>(std::string filename, const kmb::MeshData* patch, kmb::bodyIdType bodyId);
template int kmb::StereolithographyIO::saveAsciiPatch<>(std::string filename, const kmb::MeshData* patch);
