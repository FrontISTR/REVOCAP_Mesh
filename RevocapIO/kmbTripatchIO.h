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
};

}
