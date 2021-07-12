#pragma once

#include "MeshDB/kmbTypes.h"

#include <string>
#include <vector>

namespace kmb {
	template<typename MContainer> class PatchBrep;
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
		// Brep 構造の Surface をまとめて Shell ごとの出力
		template<typename MContainer>
		int savePatchBrep(std::string filename, kmb::PatchBrep<MContainer> &brep);
		template<typename MContainer>
		int saveAsciiPatchBrep(std::string filename, kmb::PatchBrep<MContainer> &brep);
	protected:
		fileType checkFormat(std::string filename);
		int countFacet(std::string filename);
		// 複数 region に対応
		int countFacet(std::string filename,std::vector<int> &sizes);
		template<typename MContainer>
		int loadAsciiPatch(std::string filename, MContainer* patch);
		template<typename MContainer>
		int loadBinaryPatch(std::string filename, MContainer* patch);
	};
}

