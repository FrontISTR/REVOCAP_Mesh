#pragma once

#include "MeshDB/kmbTypes.h"

#include <string>

namespace kmb {
	class StereolithographyIO {
	private:
		enum fileType{
			kBINARY,
			kASCII,
			kUNKNOWN
		};
	public:
		StereolithographyIO(void) {};
		~StereolithographyIO(void) {};
		template<typename MContainer>
		int loadPatch(std::string filename, MContainer* patch);
		template<typename MContainer>
		int savePatch(std::string filename, const MContainer* patch, kmb::bodyIdType bodyId);
		template<typename MContainer>
		int saveAsciiPatch(std::string filename, const MContainer* patch);
	protected:
		fileType checkFormat(std::string filename);
		int countFacet(std::string filename);
		template<typename MContainer>
		int loadAsciiPatch(std::string filename, MContainer* patch);
		template<typename MContainer>
		int loadBinaryPatch(std::string filename, MContainer* patch);
	};
}

