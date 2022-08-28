#pragma once

#include <string>

namespace kmb {

	class TetMeshIO {
	public:
		TetMeshIO(void) {};
		~TetMeshIO(void) {};
		template<typename MeshT>
		int loadMesh(std::string filename, MeshT* patch);
		template<typename MeshT>
		int saveMesh(std::string filename, const MeshT* patch);
	};
}
