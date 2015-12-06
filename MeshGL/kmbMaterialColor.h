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
#pragma once

#ifdef WIN32
#include <windows.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

namespace kmb{

class MaterialColor{
public:
	static MaterialColor kRUBY;
	static MaterialColor kEMERALD;
protected:
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat emission[4];
	GLfloat shininess;
public:
	MaterialColor(
		GLfloat _ambientr, GLfloat _ambientg, GLfloat _ambientb, GLfloat _ambienta,
		GLfloat _diffuser, GLfloat _diffuseg, GLfloat _diffuseb, GLfloat _diffusea,
		GLfloat _specularr,GLfloat _specularg,GLfloat _specularb,GLfloat _speculara,
		GLfloat _emissionr,GLfloat _emissiong,GLfloat _emissionb,GLfloat _emissiona,
		GLfloat _shininess);
	void applyFront(void) const;
	void applyBack(GLfloat r) const;
	static const MaterialColor& ruby();
	static const MaterialColor& emerald();
};


}
