/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : DIBRendering                                            #
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
#ifdef REVOCAP_SUPPORT_DIB

#ifdef WIN32
#pragma once

#define _AFXDLL
#include <afx.h>
#include <afxwin.h>
#include <GL/gl.h>
#include <GL/glu.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

namespace kmb{

class DIBRendering
{
public:
	DIBRendering(void);
	virtual ~DIBRendering(void);
	virtual bool saveBMPFile(const char* szFile );
	virtual void setContext(void);
	virtual void getSize(int size[2]) const;
	virtual bool init(int width,int height);
	virtual void releaseContext(void);
protected:
	HGLRC m_hRC;
	HDC m_hDC;
	BITMAPINFOHEADER* m_PBIH;
	void* m_PBits;
	void clear(void);
};

}
#endif

#endif
