/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : FBORendering                                       #
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

/*----------------------------------------------------------------------
=begin
=RevocapGL::FBORendering

FrameBufferObject ���g���� OffScreenRendering

===���\�b�h�ꗗ

((<FBORendering.new>))
((<init>))
((<getSize>))
((<setContext>))
((<releaseContext>))
((<saveBMPFile>))

=end
----------------------------------------------------------------------*/

namespace kmb{

class FBORendering
{
public:
/*----------------------------------------------------------------------
=begin
---FBORendering.new
	FBORendeing �N���X�̃I�u�W�F�N�g�𐶐�����B
=end
----------------------------------------------------------------------*/
	FBORendering(void);
	virtual ~FBORendering(void);
/*----------------------------------------------------------------------
=begin
---init(width,height)
	�̈�̑傫�����w�肵�ď���������B���������� true ��Ԃ��B
	�������Ɏ��s�����ꍇ�i�n�[�h�E�F�A���Ή����Ă��Ȃ��ꍇ�Ȃǁj�� false ��Ԃ��B
=end
----------------------------------------------------------------------*/
	bool init(int width,int height);
/*----------------------------------------------------------------------
=begin
---clear
	�I�t�X�N���[���p�Ɋm�ۂ����������̊J��
=end
----------------------------------------------------------------------*/
	void clear(void);

/*----------------------------------------------------------------------
=begin
---getSize()
	�̈�̑傫�����擾����B
=end
----------------------------------------------------------------------*/
%extend{
	VALUE getSize(void){
		VALUE ary = rb_ary_new();
		int s[2];
		self->getSize(s);
		rb_ary_push(ary,INT2NUM( s[0] ) );
		rb_ary_push(ary,INT2NUM( s[1] ) );
		return ary;
	}
}
/*----------------------------------------------------------------------
=begin
---setContext()
	�`��p�̃R���e�L�X�g���I�t�X�N���[���ɐݒ肷��B
---releaseContext()
	�`��p�̃R���e�L�X�g���f�t�H���g�ɖ߂��B
=end
----------------------------------------------------------------------*/
	void setContext(void);
	void releaseContext(void);
/*----------------------------------------------------------------------
=begin
---saveBMPFile(file)
	�I�t�X�N���[���̕`����e�� BMP �t�@�C���ɏo�͂���B
=end
----------------------------------------------------------------------*/
	bool saveBMPFile(const char* szFile );
};

}

#endif
