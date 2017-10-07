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

#include "MeshGL/kmbFBORendering.h"
#include "Common/kmbCommon.h"
#include <cstdio>

kmb::FBORendering::FBORendering(void)
: framebuffer_name(0)
, texture_name(0)
, renderbuffer_name(0)
{
}

kmb::FBORendering::~FBORendering(void)
{
	clear();
}

void kmb::FBORendering::clear(void)
{
	if( framebuffer_name != 0 ){
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, framebuffer_name );
		glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 0, 0 );
		glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0 );
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
		glDeleteFramebuffersEXT( 1, &framebuffer_name );
		glDeleteTextures( 1, &texture_name );
		glDeleteTextures( 1, &renderbuffer_name );
		framebuffer_name = 0;
		texture_name = 0;
		renderbuffer_name = 0;
	}
}

void
kmb::FBORendering::getSize(int size[2]) const
{
	setContext();
	GLint viewport[ 4 ];
	glGetIntegerv( GL_VIEWPORT, viewport );
	size[0] = viewport[2];
	size[1] = viewport[3];
}

// glutInit
// glutCreateWindow
// を実行してから呼び出す
bool
kmb::FBORendering::init(int width,int height)
{
	GLenum result = ::glewInit();
	if( result != GLEW_OK ){
		puts("glewInit() failure");
		return false;
	}
	if(!GLEW_EXT_framebuffer_object){
		puts("GLEW_EXT_framebuffer_object is not supported");
		return false;
	}
	if( !GLEW_EXT_texture_object ) {
		puts("GLEW_EXT_texture_object is not supported");
		return false;
	}
	if( texture_name != 0 ){
		puts("texture_object is already initialized");
		return false;
	}
	if( renderbuffer_name != 0 ){
		puts("renderbuffer_object is already initialized");
		return false;
	}
	if( framebuffer_name != 0 ){
		puts("framebuffer_object is already initialized");
		return false;
	}

	::glShadeModel( GL_SMOOTH );
	::glEnable( GL_DEPTH_TEST );
	::glCullFace( GL_BACK );
	::glEnable( GL_CULL_FACE );
	::glEnable( GL_NORMALIZE );

	::glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	::glGenTextures( 1, &texture_name );
	::glBindTexture( GL_TEXTURE_2D, texture_name );
	::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	::glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
	::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	::glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );

	::glGenRenderbuffersEXT( 1, &renderbuffer_name );
	::glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, renderbuffer_name );
	::glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height );

	::glGenFramebuffersEXT( 1, &framebuffer_name );
	::glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, framebuffer_name );

	::glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture_name, 0 );
	::glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderbuffer_name );

	::glViewport( 0, 0, width, height );

	::glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

	return true;
}


void
kmb::FBORendering::setContext(void) const
{
	if( framebuffer_name != 0 ){
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, framebuffer_name );
	}
}

void
kmb::FBORendering::releaseContext(void)
{
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
}

bool
kmb::FBORendering::saveBMPFile(const char* szFilename )
{
	setContext();
	GLint viewport[ 4 ];
	glGetIntegerv( GL_VIEWPORT, viewport );
	uint32_t width = viewport[2];
	uint32_t height = viewport[3];
	unsigned int color = 3;

	FILE* output = fopen(szFilename,"wb");
	if(output==NULL){
		return false;
	}
	int res = width*color%4;
	if(res > 0){
		res = 4 - res;
	}
	uint32_t imageSize = height*8*(width*color+res);
	uint32_t bfsize = (40 + 14)*8 + imageSize;
	uint32_t offset = 54;
	uint32_t headersize = 40;
	uint16_t tmp = 1U;
	char header[2] = {'B','M'};
	fwrite(header,sizeof(unsigned char),2,output);
	fwrite(&bfsize,sizeof(uint32_t),1,output);
	uint16_t zero = 0U;
	fwrite(&zero,sizeof(uint16_t),1,output);
	fwrite(&zero,sizeof(uint16_t),1,output);
	fwrite(&offset,sizeof(uint32_t),1,output);
	fwrite(&headersize,sizeof(uint32_t),1,output);
	fwrite(&width,sizeof(uint32_t),1,output);
	fwrite(&height,sizeof(uint32_t),1,output);
	tmp = 1U;
	fwrite(&tmp,sizeof(uint16_t),1,output);
	tmp = color*8U;
	fwrite(&tmp,sizeof(uint16_t),1,output);
	uint32_t zeroL = 0UL;
	fwrite(&zeroL,sizeof(uint32_t),1,output);
	fwrite(&imageSize,sizeof(uint32_t),1,output);
	fwrite(&zeroL,sizeof(uint32_t),1,output);
	fwrite(&zeroL,sizeof(uint32_t),1,output);
	fwrite(&zeroL,sizeof(uint32_t),1,output);
	fwrite(&zeroL,sizeof(uint32_t),1,output);

	GLubyte* image = new GLubyte[width*3];

	for(unsigned int i=0;i<height;++i){
		::glReadPixels(0,i,width,1,GL_BGR,GL_UNSIGNED_BYTE,image);
		for(unsigned int j=0;j<width;++j){
			fwrite(image+(3*j),sizeof(unsigned char),3,output);
		}
		for(int j=0;j<res;++j){
			fwrite(&zero,sizeof(unsigned char),1,output);
		}
	}

	delete[] image;

	fclose(output);
	return true;
}

#endif // REVOCAP_SUPPORT_GLEW
