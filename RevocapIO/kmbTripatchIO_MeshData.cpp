#include "RevocapIO/kmbTripatchIO_Generics.h"
#include "MeshDB/kmbMeshData.h"

template int kmb::TripatchIO::loadPatch<>(std::string filename, kmb::MeshData* patch);
template int kmb::TripatchIO::savePatch<>(std::string filename, const kmb::MeshData* patch);
