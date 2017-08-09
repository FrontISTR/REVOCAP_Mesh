#pragma once

#include <string>

namespace kmb{

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
};

}
