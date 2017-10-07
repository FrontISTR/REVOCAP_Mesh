#include "RevocapIO/kmbTripatchIO_impl.hpp"
#include "MeshDB/kmbMeshData.h"

template int kmb::TripatchIO::loadPatch<>(std::string filename, kmb::MeshData* patch);
template int kmb::TripatchIO::savePatch<>(std::string filename, const kmb::MeshData* patch);
template int kmb::TripatchIO::savePatchPacked<>(std::string filename, const kmb::MeshData* patch);
