#pragma once

#include <string>

namespace kmb {

	class TetMeshIO {
	public:
		TetMeshIO(void) {};
		~TetMeshIO(void) {};
		template<typename MContainer>
		int loadMesh(std::string filename, MContainer* patch);
		template<typename MContainer>
		int saveMesh(std::string filename, const MContainer* patch);
	};
}
