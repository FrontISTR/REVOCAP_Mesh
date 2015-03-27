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

FrameBufferObject を使った OffScreenRendering

===メソッド一覧

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
	FBORendeing クラスのオブジェクトを生成する。
=end
----------------------------------------------------------------------*/
	FBORendering(void);
	virtual ~FBORendering(void);
/*----------------------------------------------------------------------
=begin
---init(width,height)
	領域の大きさを指定して初期化する。成功したら true を返す。
	初期化に失敗した場合（ハードウェアが対応していない場合など）は false を返す。
=end
----------------------------------------------------------------------*/
	bool init(int width,int height);
/*----------------------------------------------------------------------
=begin
---clear
	オフスクリーン用に確保したメモリの開放
=end
----------------------------------------------------------------------*/
	void clear(void);

/*----------------------------------------------------------------------
=begin
---getSize()
	領域の大きさを取得する。
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
	描画用のコンテキストをオフスクリーンに設定する。
---releaseContext()
	描画用のコンテキストをデフォルトに戻す。
=end
----------------------------------------------------------------------*/
	void setContext(void);
	void releaseContext(void);
/*----------------------------------------------------------------------
=begin
---saveBMPFile(file)
	オフスクリーンの描画内容を BMP ファイルに出力する。
=end
----------------------------------------------------------------------*/
	bool saveBMPFile(const char* szFile );
};

}

#endif
