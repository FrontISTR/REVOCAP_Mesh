#include "MeshDB/kmbPatchBrep.h"

template<typename MContainer>
kmb::PatchBrep<MContainer>::PatchBrep(void)
	: brep()
{
}

template<typename MContainer>
kmb::PatchBrep<MContainer>::~PatchBrep(void)
{
}

template<typename MContainer>
int kmb::PatchBrep<MContainer>::getShellCount(void) const
{
	std::multimap<int, std::pair<const MContainer*, int> >::const_iterator bIter = brep.begin();
	int i = 0;
	while (bIter != brep.end()) {
		i++;
		bIter = brep.upper_bound(bIter->first);
	}
	return i;
}

template<typename MContainer>
int kmb::PatchBrep<MContainer>::getShell(int index) const
{
	std::multimap<int, std::pair<const MContainer*, int> >::const_iterator bIter = brep.begin();
	int i = 0;
	int b = -1;
	while (bIter != brep.end()) {
		if (index == i) {
			b = bIter->first;
			break;
		}
		i++;
		bIter = brep.upper_bound(bIter->first);
	}
	return b;
}

template<typename MContainer>
int kmb::PatchBrep<MContainer>::getSufraceCount(int shell) const
{
	return static_cast<int>(brep.count(shell));
}

template<typename MContainer>
int kmb::PatchBrep<MContainer>::getSurface(int shell, int index) const
{
	std::pair< std::multimap<int, std::pair<const MContainer*, int> >::const_iterator, std::multimap<int, std::pair<const MContainer*, int> >::const_iterator > range = brep.equal_range(shell);
	int i = 0;
	int b = -1;
	std::multimap<int, std::pair<const MContainer*, int> >::const_iterator bIter = range.first;
	while (bIter != range.second) {
		if (index == i) {
			b = bIter->second.second;
			break;
		}
		++i;
		++bIter;
	}
	return b;
}

template<typename MContainer>
const MContainer* kmb::PatchBrep<MContainer>::getMesh(int shell, int index) const
{
	std::pair< std::multimap<int, std::pair<const MContainer*, int> >::const_iterator, std::multimap<int, std::pair<const MContainer*, int> >::const_iterator > range = brep.equal_range(shell);
	int i = 0;
	const MContainer* mesh = NULL;
	std::multimap<int, std::pair<const MContainer*, int> >::const_iterator bIter = range.first;
	while (bIter != range.second) {
		if (index == i) {
			mesh = bIter->second.first;
			break;
		}
		++i;
		++bIter;
	}
	return mesh;
}


template<typename MContainer>
void kmb::PatchBrep< MContainer>::setSurface(int shell, const MContainer* mesh, int patch)
{
	brep.insert(std::make_pair(shell, std::make_pair(mesh,patch)));
}
