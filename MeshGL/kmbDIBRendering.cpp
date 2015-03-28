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
#include "MeshGL/kmbDIBRendering.h"

kmb::DIBRendering::DIBRendering(void)
: m_PBIH(NULL)
, m_PBits(NULL)
{
}

kmb::DIBRendering::~DIBRendering(void)
{
	clear();
}

void
kmb::DIBRendering::clear(void)
{
	if( m_PBIH ){
		delete m_PBIH;
	}

	if( m_hDC ){
		HBITMAP tmp_hbitmap = (HBITMAP)::SelectObject(m_hDC, NULL);
		if (tmp_hbitmap) {
			::DeleteObject(tmp_hbitmap);
		}
		DeleteDC( m_hDC );
	}

	::wglDeleteContext(m_hRC);
}

void
kmb::DIBRendering::getSize(int size[2]) const
{
	if( m_PBIH ){
		size[0] = m_PBIH->biWidth;
		size[1] = m_PBIH->biHeight;
	}else{
		size[0] = 0;
		size[1] = 0;
	}
}

bool
kmb::DIBRendering::init(int width,int height)
{
	clear();

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_BITMAP |
		PFD_SUPPORT_OPENGL,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	m_hDC = ::CreateCompatibleDC(NULL);


	m_PBIH = new BITMAPINFOHEADER;
	int iSize = sizeof(BITMAPINFOHEADER);
	::memset(m_PBIH, 0, iSize);

	m_PBIH->biSize = sizeof(BITMAPINFOHEADER);
	m_PBIH->biWidth = width;
	m_PBIH->biHeight = height;
	m_PBIH->biPlanes = 1;
	m_PBIH->biBitCount = 24;
	m_PBIH->biCompression = BI_RGB;
	m_PBIH->biSizeImage = width* height * 3;


	HBITMAP hbitmap = ::CreateDIBSection(
		m_hDC,
		(BITMAPINFO*)m_PBIH, DIB_RGB_COLORS,
		&m_PBits, NULL, 0
	);

	if (hbitmap) {
		::SelectObject(m_hDC, hbitmap);
	}else{
		return false;
	}

	int  iPixelFormat;

	iPixelFormat = ::ChoosePixelFormat(m_hDC, &pfd);

	BOOL success = ::SetPixelFormat(m_hDC, iPixelFormat, &pfd);

	m_hRC = ::wglCreateContext(m_hDC);

	::wglMakeCurrent(m_hDC, m_hRC);

	::glViewport(0, 0, width, height);

	::wglMakeCurrent(NULL, NULL);

	return true;
}

bool
kmb::DIBRendering::saveBMPFile(const char* szFile )
{
	if (!m_PBIH)
		return false;

	DWORD            dwResult;
	HANDLE           hfile;
	BITMAPFILEHEADER bmfHeader = {0};

	hfile = CreateFile((LPCTSTR)szFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE) {
		return false;
	}

	bmfHeader.bfType    = *(LPWORD)"BM";
	bmfHeader.bfSize    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_PBIH->biSizeImage;
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	WriteFile(hfile, &bmfHeader, sizeof(BITMAPFILEHEADER), &dwResult, NULL);

	WriteFile(hfile, m_PBIH, sizeof(BITMAPINFOHEADER), &dwResult, NULL);

	WriteFile(hfile, m_PBits, m_PBIH->biSizeImage, &dwResult, NULL);

	CloseHandle(hfile);

	return true;
}

void
kmb::DIBRendering::setContext(void)
{
	::wglMakeCurrent(m_hDC, m_hRC);
}

void
kmb::DIBRendering::releaseContext(void)
{
	::SwapBuffers(m_hDC);

	::wglMakeCurrent(NULL, NULL);
}

#endif
#endif
