#pragma once

#include <cfloat>
#include <ctime>
#include <cstdlib>

void irand(void)
{
	srand(static_cast<unsigned int>(time(NULL)));
}

double drand(double max=1.0,double min=0.0)
{
	return min + rand() * (max-min) / RAND_MAX;
}

int nrand(int nmax, int nmin = 0)
{
	long lrand = (rand() << 8) | (rand() & 0xff);
	return nmin + lrand % (nmax - nmin);
}

#include "MeshDB/kmbMeshData.h"

kmb::MeshData* createCubeMesh(double x0,double y0,double z0,double x1,double y1,double z1)
{
	kmb::MeshData* mesh = new kmb::MeshData();
	mesh->beginNode(8);
	mesh->addNode(x0,y0,z0);
	mesh->addNode(x1,y0,z0);
	mesh->addNode(x1,y1,z0);
	mesh->addNode(x0,y1,z0);
	mesh->addNode(x0,y0,z1);
	mesh->addNode(x1,y0,z1);
	mesh->addNode(x1,y1,z1);
	mesh->addNode(x0,y1,z1);
	mesh->endNode();
	mesh->beginElement(1);
	kmb::nodeIdType nodes[8] = {0,1,2,3,4,5,6,7};
	mesh->addElement(kmb::kHexahedron,nodes);
	mesh->endElement();
	return mesh;
}
