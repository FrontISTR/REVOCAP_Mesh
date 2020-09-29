/**
 * shell は solid の境界面を構成する
 * shell は複数の surface からなる
 *
 */

#pragma once

#include <vector>
#include <map>

namespace kmb {

template<typename MContainer>
class PatchBrep {
public:
	PatchBrep(void);
	~PatchBrep(void);
	int getShellCount(void) const;
	int getShell(int index) const;
	int getSufraceCount(int shell) const;
	int getSurface(int shell, int index) const;
	const MContainer* getMesh(int shell, int index) const;
	void setSurface(int shell, const MContainer* mesh, int patch);
private:
	std::multimap<int, std::pair<const MContainer*,int> > brep;
};
}