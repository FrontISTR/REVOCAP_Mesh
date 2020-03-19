#pragma once

#include <string>
#include <vector>

namespace kmb{

class PatchBrep;

class TripatchIO
{
public:
	TripatchIO(void){};
	~TripatchIO(void){};
	template<typename MContainer>
	int loadPatch(std::string filename, MContainer* patch);
	template<typename MContainer>
	int savePatch(std::string filename, const MContainer* patch);
	// すべてを一つにまとめて出力
	template<typename MContainer>
	int savePatchPacked(std::string filename, const MContainer* patch);
	// Brep 構造の Surface をまとめて Shell ごとの出力
	int savePatchBrep(std::string filename,kmb::PatchBrep &brep);
};

}
