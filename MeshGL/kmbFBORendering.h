/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : FBORendering                                            #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/
#ifdef REVOCAP_SUPPORT_GLEW

#pragma once

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glew32.lib")
#endif


#ifdef __APPLE__
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

namespace kmb{

class FBORendering
{
private:
	GLuint framebuffer_name;
	GLuint texture_name;
	GLuint renderbuffer_name;
public:
	FBORendering(void);
	virtual ~FBORendering(void);
	virtual bool init(int width,int height);
	virtual bool saveBMPFile(const char* szFilename );
	virtual void setContext(void) const;
	virtual void releaseContext(void);
	virtual void getSize(int size[2]) const;
	void clear(void);
};

}

#endif
