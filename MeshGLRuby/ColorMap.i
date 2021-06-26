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

==概要

RevocapMesh::MeshDB に格納されたメッシュデータを
OpenGL で高速に描画する際の物理量と色の対応を定義するクラスである。

==メソッド一覧

((<ColorMap.new>))
((<setStep>))
((<getStep>))
((<setMinMax>))
((<getMinMax>))
((<setAlpha>))
((<getAlpha>))
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
==メソッド
---ColorMap.new(step=5)
	ステップ数を与えて ColorMap クラスのオブジェクトを生成する。
=end
---------------------------------------------------------------------------*/
	ColorMap(int s=5);
	virtual ~ColorMap(void);
/**--------------------------------------------------------------------------
=begin
---setStep(step)
	ステップ数を設定する。
	非負の値しか設定できない。0 を与えるとスムーズコンタとなる。
=end
---------------------------------------------------------------------------*/
	void setStep(int s);
/**--------------------------------------------------------------------------
=begin
---getStep()
	ステップ数を取得する。
=end
---------------------------------------------------------------------------*/
	int getStep(void);
/**--------------------------------------------------------------------------
=begin
---setMinMax(min,max)
	カラーコンター表示の最大最小値を設定する。
---getMinMax
	現在設定されている最大最小値を取得する。
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
---setAlpha(min,max)
	透明度を設定する。
---getAlpha
	透明度を取得する。
=end
---------------------------------------------------------------------------*/
	void setAlpha(float a);
	float getAlpha(void) const;
/**--------------------------------------------------------------------------
=begin
---getRGB(x)
	値 x に対する RGB 値を計算する。
	[r,g,b] 形式の Array で返す。
---getRGBByStep(i)
	Step数 i に対する RGB 値を計算する。
	[r,g,b] 形式の Array で返す。
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
	Step数 i に対する RGB 値を与える。
	[r,g,b,a] 形式の Array で与える。
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
	実数値 v に対する glColor または glMaterial を実行する。
--- setGLColorByStep(i)
	ステップ数 i  に対する glColor または glMaterial を実行する。
=end
---------------------------------------------------------------------------*/
	void setGLColor(double v);
	void setGLColorByStep(int i);
/**--------------------------------------------------------------------------
=begin
--- getContourValue()
	コンターの閾値を Array で返す。
	Step = 0 のとき（スムーズコンタ）は min の値のみを返す
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
	mesh に格納されている name のデータをこのカラーマップで rgb 値に変換したものを
	colorname のデータに格納する
--- updateColorCache( mesh, name, comp )
	mesh に格納されている name のデータをこのカラーマップで rgb 値に変換したもので
	colorname のデータを更新する
	実際のデータの更新は clearColorCache を行った後１度だけ行う
--- clearColorCache
	updateColorCache を行ってデータを更新したときにその前に実行する
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
