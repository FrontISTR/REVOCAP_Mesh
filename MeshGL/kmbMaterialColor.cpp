/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : MaterialColor                                           #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2015/08/04     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/

#include "MeshGL/kmbMaterialColor.h"

kmb::MaterialColor kmb::MaterialColor::kRUBY(
	0.1745f, 0.01175f, 0.01175f, 1.0f,
	0.61424f, 0.04136f, 0.04136f, 1.0f,
	0.727811f, 0.626959f, 0.626959f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	76.8f);

kmb::MaterialColor kmb::MaterialColor::kEMERALD(
	0.0215f, 0.1745f, 0.0215f, 1.0f,
	0.07568f, 0.61424f, 0.07568f, 1.0f,
	0.633f, 0.727811f, 0.633f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	76.8f);

kmb::MaterialColor::MaterialColor(
	GLfloat _ambientr, GLfloat _ambientg, GLfloat _ambientb, GLfloat _ambienta,
	GLfloat _diffuser, GLfloat _diffuseg, GLfloat _diffuseb, GLfloat _diffusea,
	GLfloat _specularr,GLfloat _specularg,GLfloat _specularb,GLfloat _speculara,
	GLfloat _emissionr,GLfloat _emissiong,GLfloat _emissionb,GLfloat _emissiona,
	GLfloat _shininess)
{
	ambient[0] = _ambientr;
	ambient[1] = _ambientg;
	ambient[2] = _ambientb;
	ambient[3] = _ambienta;
	diffuse[0] = _diffuser;
	diffuse[1] = _diffuseg;
	diffuse[2] = _diffuseb;
	diffuse[3] = _diffusea;
	specular[0] = _specularr;
	specular[1] = _specularg;
	specular[2] = _specularb;
	specular[3] = _speculara;
	emission[0] = _emissionr;
	emission[1] = _emissiong;
	emission[2] = _emissionb;
	emission[3] = _emissiona;
	shininess = _shininess;
}

void kmb::MaterialColor::applyFront(void) const
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void kmb::MaterialColor::applyBack(GLfloat r) const
{
	glMaterialfv(GL_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_BACK, GL_EMISSION, emission);
	glMaterialf(GL_BACK, GL_SHININESS, shininess);
}

const kmb::MaterialColor& kmb::MaterialColor::ruby()
{
	return kmb::MaterialColor::kRUBY;
}

const kmb::MaterialColor& kmb::MaterialColor::emerald()
{
	return kmb::MaterialColor::kEMERALD;
}
