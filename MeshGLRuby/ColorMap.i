/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : ColorMap                                           #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
# Software Name : RevocapMesh version 1.2                              #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2008/03/14     #
#                                           T. Takeda   2008/03/14     #
#                                           K. Amemiya  2008/03/14     #
#                                                                      #
#      Contact Address: RSS21 Project, IIS, The University of Tokyo    #
#                                                                      #
#      "Innovative General-Purpose Coupled Analysis System"            #
#                                                                      #
----------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
=begin
=RevocapGL::ColorMap

==�T�v

RevocapMesh::MeshDB �Ɋi�[���ꂽ���b�V���f�[�^��
OpenGL �ō����ɕ`�悷��ۂ̕����ʂƐF�̑Ή����`����N���X�ł���B

==���\�b�h�ꗗ

((<ColorMap.new>))
((<setStep>))
((<getStep>))
((<setMinMax>))
((<getMinMax>))
((<getRGB>))
((<getRGBByStep>))
((<setRGBAByStep>))
((<setGLColor>))
((<setGLColorByStep>))
((<getContourValue>))
((<createColorCache>))

=end
---------------------------------------------------------------------------*/

namespace kmb{

class ColorMap
{
public:
/**--------------------------------------------------------------------------
=begin
==���\�b�h
---ColorMap.new(step=5)
	�X�e�b�v����^���� ColorMap �N���X�̃I�u�W�F�N�g�𐶐�����B
=end
---------------------------------------------------------------------------*/
	ColorMap(int s=5);
	virtual ~ColorMap(void);
/**--------------------------------------------------------------------------
=begin
---setStep(step)
	�X�e�b�v����ݒ肷��B
	�񕉂̒l�����ݒ�ł��Ȃ��B0 ��^����ƃX���[�Y�R���^�ƂȂ�B
=end
---------------------------------------------------------------------------*/
	void setStep(int s);
/**--------------------------------------------------------------------------
=begin
---getStep()
	�X�e�b�v�����擾����B
=end
---------------------------------------------------------------------------*/
	int getStep(void);
/**--------------------------------------------------------------------------
=begin
---setMinMax(min,max)
	�J���[�R���^�[�\���̍ő�ŏ��l��ݒ肷��B
---getMinMax
	���ݐݒ肳��Ă���ő�ŏ��l���擾����B
=end
---------------------------------------------------------------------------*/
	void setMinMax(double min,double max);
%extend{
	VALUE getMinMax(void){
		VALUE ary = rb_ary_new();
		double m0,m1;
		self->getMinMax(m0,m1);
		rb_ary_push(ary,rb_float_new(m0));
		rb_ary_push(ary,rb_float_new(m1));
		return ary;
	}
}
/**--------------------------------------------------------------------------
=begin
---getRGB(x)
	�l x �ɑ΂��� RGB �l���v�Z����B
	[r,g,b] �`���� Array �ŕԂ��B
---getRGBByStep(i)
	Step�� i �ɑ΂��� RGB �l���v�Z����B
	[r,g,b] �`���� Array �ŕԂ��B
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getRGB(double x){
		VALUE ary = rb_ary_new();
		float rgb[3];
		self->getRGB(x,rgb);
		rb_ary_push(ary,rb_float_new(rgb[0]));
		rb_ary_push(ary,rb_float_new(rgb[1]));
		rb_ary_push(ary,rb_float_new(rgb[2]));
		return ary;
	}
	VALUE getRGBByStep(int i){
		VALUE ary = rb_ary_new();
		float rgb[3];
		self->getRGBByStep(i,rgb);
		rb_ary_push(ary,rb_float_new(rgb[0]));
		rb_ary_push(ary,rb_float_new(rgb[1]));
		rb_ary_push(ary,rb_float_new(rgb[2]));
		return ary;
	}
}
/**--------------------------------------------------------------------------
=begin
---setRGBAByStep(i,[r,g,b,a])
	Step�� i �ɑ΂��� RGB �l��^����B
	[r,g,b,a] �`���� Array �ŗ^����B
=end
---------------------------------------------------------------------------*/
%extend{
	void setRGBAByStep(int i,VALUE ary){
		Check_Type(ary, T_ARRAY);
		int len = RARRAY_LEN(ary);
		float rgba[4];
		if(len == 4){
			rgba[0] = static_cast<float>( NUM2DBL( rb_ary_entry(ary,0) ) );
			rgba[1] = static_cast<float>( NUM2DBL( rb_ary_entry(ary,1) ) );
			rgba[2] = static_cast<float>( NUM2DBL( rb_ary_entry(ary,2) ) );
			rgba[3] = static_cast<float>( NUM2DBL( rb_ary_entry(ary,3) ) );
		}else if(len == 3){
			rgba[0] = static_cast<float>( NUM2DBL( rb_ary_entry(ary,0) ) );
			rgba[1] = static_cast<float>( NUM2DBL( rb_ary_entry(ary,1) ) );
			rgba[2] = static_cast<float>( NUM2DBL( rb_ary_entry(ary,2) ) );
			rgba[3] = 1.0f;
		}
		self->setRGBAByStep(i,rgba);
	}
}
/**--------------------------------------------------------------------------
=begin
--- setGLColor(v)
	�����l v �ɑ΂��� glColor �܂��� glMaterial �����s����B
--- setGLColorByStep(i)
	�X�e�b�v�� i  �ɑ΂��� glColor �܂��� glMaterial �����s����B
=end
---------------------------------------------------------------------------*/
	void setGLColor(double v);
	void setGLColorByStep(int i);
/**--------------------------------------------------------------------------
=begin
--- getContourValue()
	�R���^�[��臒l�� Array �ŕԂ��B
	Step = 0 �̂Ƃ��i�X���[�Y�R���^�j�� min �̒l�݂̂�Ԃ�
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getContourValue(void){
		VALUE ary = rb_ary_new();
		const int len = self->getStep() + 1;
		for(int i=0;i<len;++i)
		{
			rb_ary_push(ary,rb_float_new( self->getContourValue(i) ));
		}
		return ary;
	}
}
/**--------------------------------------------------------------------------
=begin
--- createColorCache( mesh, name, comp )
	mesh �Ɋi�[����Ă��� name �̃f�[�^�����̃J���[�}�b�v�� rgb �l�ɕϊ��������̂�
	colorname �̃f�[�^�Ɋi�[����
--- updateColorCache( mesh, name, comp )
	mesh �Ɋi�[����Ă��� name �̃f�[�^�����̃J���[�}�b�v�� rgb �l�ɕϊ��������̂�
	colorname �̃f�[�^���X�V����
	���ۂ̃f�[�^�̍X�V�� clearColorCache ���s������P�x�����s��
--- clearColorCache
	updateColorCache ���s���ăf�[�^���X�V�����Ƃ��ɂ��̑O�Ɏ��s����
=end
---------------------------------------------------------------------------*/
%extend{
	bool createColorCache( kmb::MeshData* mesh, const char* name, int comp=-1){
		if( mesh ){
			kmb::DataBindings* nodeValue = mesh->getDataBindingsPtr( name );
			if( nodeValue ){
				return self->createColorCache(nodeValue,comp);
			}
		}
		return false;
	}
	bool updateColorCache( kmb::MeshData* mesh, const char* name, int comp=-1){
		if( mesh ){
			kmb::DataBindings* nodeValue = mesh->getDataBindingsPtr( name );
			if( nodeValue ){
				return self->updateColorCache(nodeValue,comp);
			}
		}
		return false;
	}
}
	void clearColorCache(void);
};

}
