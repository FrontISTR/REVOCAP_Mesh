/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : MeshGL                                             #
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
=RevocapGL::MeshGL

==概要

RevocapMesh::MeshDB に格納されたメッシュデータを
OpenGL で高速に描画するためのクラスである。

==メソッド一覧

((<MeshGL.new>))
((<setPreferenceMode>))
((<clearCache>))
((<display>))
((<update>))
((<getLists>))
((<setMesh>))
((<drawEdge>))
((<drawEdgeWithScale>))
((<drawSurface>))
((<drawFaceGroup>))
((<drawAllNodes>))
((<drawSurfaceContour>))
((<drawFaceGroupContour>))
((<drawSurfaceNodeContour>))
((<drawSurfaceNodeContour2D>))
((<drawFaceGroupNodeContour>))
((<drawNodeContourOnSurface>))
((<drawNodeContour2DOnSurface>))
((<drawNodeContourOnFaceGroup>))
((<deformSurface>))
((<deformSurfaceNodeContour>))
((<deformSurfaceContour>))
((<deformFaceGroup>))
((<deformXYZFaceGroup>))
((<deformFaceGroupNodeContour>))
((<deformXYZFaceGroupNodeContour>))
((<drawIsosurface>))
((<drawSection>))
((<drawSectionNodeContour>))
((<drawSectionContour>))
((<drawSectionVector>))
((<drawIntersection>))
((<drawIntersectionContour>))
((<drawVector>))
((<drawField>))
((<drawSurfaceVector>))
((<drawFaceGroupVector>))
((<drawMarking>))
((<drawMarkingEdgeWithScale>))
((<setMarkSize>))
((<getMarkSize>))
((<setNodeOffset>))
((<setElementOffset>))
((<getDeformFaceGroupBoundingBox>))
((<getDeformXYZFaceGroupBoundingBox>))
=end
---------------------------------------------------------------------------*/

namespace kmb{

class MeshGL
{
public:
/**--------------------------------------------------------------------------
=begin
==メソッド
--- MeshGL.new(listSize=1)
	メッシュ専用描画クラス MeshGL のインスタンスを生成する。
	listSize は display メソッドの最後の引数で与えられる listIndex の個数を表す。
	内部の実装では listSize 個分の OpenGL の DisplayList を覚えておくことを可能にする。
	同じ形状の表示を異なるデバイスコンテキストに行い、それぞれで Cache したい場合は
	listSize をデバイスコンテキストの個数分与える。
=end
---------------------------------------------------------------------------*/
	MeshGL(size_t listSize=1);
	virtual ~MeshGL(void);
/**--------------------------------------------------------------------------
=begin
--- setPreferenceMode(mode)
	描画の優先モードを設定する
	現在設定可能なのは
	- "PRECISION" 精度優先モード
	- "SPEED" 速度優先モード
	である
--- getPreferenceMode
	現在の描画の優先モードを取得する
=end
---------------------------------------------------------------------------*/
%extend{
	void setPreferenceMode(const char* mode){
		if( strcmp(mode,"SPEED") == 0 ){
			self->setPreferenceMode( kmb::MeshGL::SPEED );
		}else if( strcmp(mode,"PRECISION") == 0 ){
			self->setPreferenceMode( kmb::MeshGL::PRECISION );
		}
	}
	const char* getPreferenceMode(void) const{
		switch( self->getPreferenceMode() ){
		case kmb::MeshGL::SPEED:
			return "SPEED";
		case kmb::MeshGL::PRECISION:
			return "PRECISION";
		default:
			return "";
		}
	}
}
/**--------------------------------------------------------------------------
=begin
--- clearCache
	描画用のキャッシュをクリアする。
	実装は display list の削除である。
=end
---------------------------------------------------------------------------*/
	void clearCache(void);
/**--------------------------------------------------------------------------
=begin
--- display(mesh,updateFlag,listIndex=0)
--- display(listIndex=0)
	ブロックを定義して、そのブロック内での GL の命令を DisplayList に登録・描画する。
	既に DisplayList に登録済みの場合はそのデータを描画する。
	登録されていないときは、一旦ブロック内の命令を DisplayList に登録してから描画する。
	戻り値は正常に描画されたかどうかを返す。
	listIndex の値は MeshGL が複数の Cache (= Display List) を管理する場合に、
	どの Cache を使うかを選択する。-1 を与えると Cache を使わずにブロック内をそのまま実行する。
	例：
	 meshGL.display{
	   meshGL.setMesh( mesh )
	   meshGL.drawFaceGroup("face_0")
	 }
	Display List を作り直す場合（display のブロック内が更新された場合）は、MeshGL の update メソッドを
	呼び出す
=end
---------------------------------------------------------------------------*/
%extend{
	bool display(int listIndex=0){
		const int len = static_cast<int>(self->getListSize());
		if( listIndex < 0 ){
			rb_yield( Qnil );
			::glFinish();
			return true;
		}else if( listIndex >= len ){
			return false;
		}
		GLuint list = self->getList(listIndex);
		if( self->isUpdate() ){
			if( list > 0 && ::glIsList( list ) == GL_TRUE ){
				glDeleteLists( list, 1 );
				glNewList(list,GL_COMPILE_AND_EXECUTE);
				rb_yield( Qnil );
				glEndList();
				::glFinish();
				self->update(false);
				return true;
			}else{
				list = glGenLists(1);
				if( list > 0 ){
					glNewList(list,GL_COMPILE_AND_EXECUTE);
					rb_yield( Qnil );
					glEndList();
					::glFinish();
					self->setList(listIndex,list);
					self->update(false);
					return true;
				}else{
					rb_yield( Qnil );
					::glFinish();
					return false;
				}
			}
		}else{
			if( list > 0 && ::glIsList( list ) == GL_TRUE ){
				glCallList( list );
				::glFinish();
				return true;
			}else{
				rb_yield( Qnil );
				::glFinish();
				return false;
			}
		}
	}
}
/**--------------------------------------------------------------------------
=begin
--- update
	次回描画するタイミングで Display List を作り直す
=end
---------------------------------------------------------------------------*/

	void update(void);

/**--------------------------------------------------------------------------
=begin
--- getLists
	現在利用している DisplayList の Index を配列で返す。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getLists(){
		VALUE ary = rb_ary_new();
		const int len = static_cast<int>(self->getListSize());
		for( int i=0; i<len; ++i ){
			rb_ary_store(ary,i,INT2NUM(self->getList(i)));
		}
		return ary;
	}
}
/**--------------------------------------------------------------------------
=begin
---setMesh(mesh)
	描画の対象となる MeshDB のオブジェクトを指定する。
=end
---------------------------------------------------------------------------*/
	void setMesh(kmb::MeshData* mesh);
/**--------------------------------------------------------------------------
=begin
---drawEdge(bodyId)
	エッジを表示する。１次元要素からなる Body の BodyId を与える。
---drawEdgeWithScale(bodyId,x,y,z,scale)
	エッジを (x,y,z) を中心に scale 倍して描画する。
=end
---------------------------------------------------------------------------*/
	void drawEdge(kmb::bodyIdType bodyId);
	void drawEdgeWithScale(kmb::bodyIdType bodyId,double x,double y,double z,double scale);
/**--------------------------------------------------------------------------
=begin
---drawSurface(bodyId)
---drawSurface2D(bodyId)
	サーフェスを表示する。２次元要素からなる Body の BodyId を与える。
---drawFaceGroup(faceName)
	上記と同様に bodyId の代わりに faceName を与えて描画する。
=end
---------------------------------------------------------------------------*/
	void drawSurface(kmb::bodyIdType bodyId);
	void drawSurface2D(kmb::bodyIdType bodyId);
	void drawFaceGroup(const char* faceName);
/**--------------------------------------------------------------------------
=begin
---drawAllNodes(r=-1.0)
    メッシュに登録されているすべての節点を表示する。
    r <= 0.0 の時は glVertex で描画する。点の大きさはあらかじめ glPointSize で指定する。
    r > 0.0 の時は gluSphere で描画する。
=end
---------------------------------------------------------------------------*/
	void drawAllNodes(double r);
/**--------------------------------------------------------------------------
=begin
---drawSurfaceContour(bodyId,name,colormap)
	MeshDB に name という名前で格納されている物理量（ElementVariable モード、Scalar 値）
	に対して、colormap で定義された色づけ方法によって、それぞれの要素に
	同じ色をつけることで、カラーコンター表示を行う。
=end
---------------------------------------------------------------------------*/
  void drawSurfaceContour(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);
  void drawSurfaceContour2D(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);
/**--------------------------------------------------------------------------
=begin
---drawFaceGeoupContour(facename,name,colormap,comp=-1)
	MeshDB に name という名前で格納されている物理量（ElementVariable モード、Integer 値）
	に対して、colormap で定義された色づけ方法によって、それぞれの要素に
	同じ色をつけることで、facename の FaceGroup のカラーコンター表示を行う。
=end
---------------------------------------------------------------------------*/
	void drawFaceGroupContour(const char* faceName, const char* physicalValue, kmb::ColorMap* colorMap,int comp=-1);
/**--------------------------------------------------------------------------
=begin
---drawSurfaceNodeContour(bodyId,name,colormap,comp=-1)
	MeshDB に name という名前で格納されている物理量（NodeVariable モード、Scalar 値）
	に対して、colormap で定義された色づけ方法によって、カラーコンター表示を行う。
	colormap の smoothness が SOLID のときには、それぞれの要素を同じ色の幅で分割して
	色をつける。SMOOTH のときには頂点ごとに色を与えて、ブレンドは OpenGL に任せる。
---drawFaceGroupNodeContour(facename,name,colormap,comp=-1)
	上記のメソッドについて、bodyId の代わりにそれぞれ FaceGroup の名前を与えて
	コンター表示を行う。
=end
---------------------------------------------------------------------------*/
	void drawSurfaceNodeContour(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);
	void drawSurfaceNodeContour2D(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);
	void drawFaceGroupNodeContour(const char* faceName, const char* physicalValue, kmb::ColorMap* colorMap,int comp=-1);

	void drawNodeContourOnSurface(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);
	void drawNodeContour2DOnSurface(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);
	void drawNodeContourOnFaceGroup(const char* faceName, const char* physicalValue, kmb::ColorMap* colorMap,int comp=-1);
	void drawNodeContourAllNodes(const char* physicalValue, kmb::ColorMap* colorMap,int comp=-1);

/**--------------------------------------------------------------------------
=begin
---deformSurface(bodyId,displacement,factor)
	bodyId で与えられる Surface を
	MeshDB に displacement という名前で格納されている物理量
	（モード：NodeVariable,値：Vector3）に対して、factor 倍で変形表示する。
---deformFaceGroup(faceName,displacement,factor)
	faceName で与えられる FaceGroup の face を
	MeshDB に displacement という名前で格納されている物理量
	（モード：NodeVariable,値：Vector3）に対して、factor 倍で変形表示する。
---deformXYZFaceGroup(faceName,displacementX,displacementY,displacementZ,factor)
	faceName で与えられる FaceGroup の face を
	MeshDB に displacementX, displacementY,displacementZ という名前で格納されている
	物理量（モード：NodeVariable,値：Scalar）を(x,y,z)座標の変形量だとして
	factor 倍で変形表示する。
=end
---------------------------------------------------------------------------*/
	void deformSurface(kmb::bodyIdType bodyId, const char* displacement, double factor);
	void deformFaceGroup(const char* faceName, const char* displacement, double factor);
	void deformXYZFaceGroup(const char* faceName, const char* displacementX, const char* displacementY, const char* displacementZ, double factor);
/**--------------------------------------------------------------------------
=begin
---deformSurfaceNodeContour(bodyId,displacement,factor,nodeValue,colorMap,comp=-1)
	bodyId で与えられる Surface を
	MeshDB に displacement という名前で格納されている物理量
	（モード：NodeVariable,値：Vector3）に対して、factor 倍で変形表示する。
	MeshDB に nodeValue という名前で格納されている物理量（NodeVariable モード）
	に対して、colormap で定義された色づけ方法によって、comp 成分のカラーコンター表示を行う。
---deformSurfaceContour(bodyId,displacement,factor,elementValue,colorMap,comp=-1)
	bodyId で与えられる Surface を
	MeshDB に displacement という名前で格納されている物理量
	（モード：NodeVariable,値：Vector3）に対して、factor 倍で変形表示する。
	MeshDB に elementValue という名前で格納されている物理量（ElementVariable モード）
	に対して、colormap で定義された色づけ方法によって、comp 成分のカラーコンター表示を行う。
---deformFaceGroupNodeContour(faceName,displacement,factor,nodeValue,colorMap,comp=-1)
	faceName で与えられる FaceGroup の face を
	MeshDB に displacement という名前で格納されている物理量
	（モード：NodeVariable,値：Vector3）に対して、factor 倍で変形表示する。
	MeshDB に nodeValue という名前で格納されている物理量（NodeVariable モード）
	に対して、colormap で定義された色づけ方法によって、comp 成分のカラーコンター表示を行う。
---deformXYZFaceGroupNodeContour(faceName,displacementX,displacementY,displacementZ,factor,nodeValue,colorMap,comp=-1)
	faceName で与えられる FaceGroup の face を
	MeshDB に displacementX, displacementY,displacementZ という名前で格納されている
	物理量（モード：NodeVariable,値：Scalar）を(x,y,z)座標の変形量だとして
	factor 倍で変形表示する。
	MeshDB に nodeValue という名前で格納されている物理量（NodeVariable モード）
	に対して、colormap で定義された色づけ方法によって、comp 成分のカラーコンター表示を行う。
=end
---------------------------------------------------------------------------*/
	void deformSurfaceNodeContour(kmb::bodyIdType bodyId, const char* displacement, double factor, const char* physicalValue, kmb::ColorMap* colorMap, int comp=-1);
	void deformSurfaceContour(kmb::bodyIdType bodyId, const char* displacement, double factor, const char* physicalValue, kmb::ColorMap* colorMap, int comp=-1);
	void deformFaceGroupNodeContour(const char* faceName, const char* displacement, double factor, const char* nodeValue, kmb::ColorMap* colorMap, int comp=-1);
	void deformXYZFaceGroupNodeContour(const char* faceName, const char* displacementX, const char* displacementY, const char* displacementZ, double factor, const char* physicalValue, kmb::ColorMap* colorMap);
/**--------------------------------------------------------------------------
=begin
---drawVector(vector,factor,colormap)
	vectorName で与えられる NodeVariable モードの Vector3 値の物理量に対して、
	長さが factor 倍の矢印でベクトル表示する。
	色は colorMap で与える。
---drawField(vector,size,colorMap)
	vectorName で与えられる NodeVariable モードの Vector3 値の物理量に対して、
	長さ size 一定の矢印でベクトル表示する。
	色は colorMap で与える。
---drawSurfaceVector(bodyId,vector,factor,colormap)
	vectorName で与えられる NodeVariable モードの Vector3 値の物理量に対して、
	bodyId で与えられた面上の節点について、長さが factor 倍の矢印でベクトル表示する。
	色は colorMap で与える。
---drawSurfaceField(bodyId,vector,size,colormap)
	vectorName で与えられる NodeVariable モードの Vector3 値の物理量に対して、
	bodyId で与えられた面上の節点について、長さが size 一定の矢印でベクトル表示する。
	色は colorMap で与える。
---drawFaceGroupVector(faceName,vectorName,factor,colorMap)
	vectorName で与えられる NodeVariable モードの Vector3 値の物理量に対して、
	faceName で与えられた面グループ上の節点について、長さが factor 倍の矢印でベクトル表示する。
	色は colorMap で与える。
=end
---------------------------------------------------------------------------*/
	void drawVector(const char* vectorName, double factor, kmb::ColorMap* colorMap);
	void drawField(const char* vectorName, double arrowSize, kmb::ColorMap* colorMap);
	void drawSurfaceVector(kmb::bodyIdType bodyId,const char* vectorName, double factor, kmb::ColorMap* colorMap);
	void drawSurfaceField(kmb::bodyIdType bodyId,const char* vectorName, double arrowSize, kmb::ColorMap* colorMap);
	void drawFaceGroupVector(const char* faceName,const char* vectorName, double factor, kmb::ColorMap* colorMap);
/**--------------------------------------------------------------------------
=begin
---drawIsosurface(bodyId,valueName,val)
	bodyId の Body の中で valueName で与えられる NodeVariable モードの物理量に対して、
	値が val の等値面を表示する。このメソッドでは色や Material はつけない。
=end
---------------------------------------------------------------------------*/
	void drawIsosurface(kmb::bodyIdType bodyId, const char* nodeValue, double val);
/**--------------------------------------------------------------------------
=begin
---drawSection(bodyId,plane)
	bodyId の Body を plane で切った切断面を表示する。
---drawSectionNodeContour(bodyId,plane,physicalValue,colorMap,comp=-1)
	節点値の physicalValue の値で colorMap に従いコンター表示をする。
---drawSectionContour(bodyId,plane,physicalValue,colorMap,comp=-1)
	要素値の physicalValue の値で colorMap に従いコンター表示をする。
	ただし Tetrahedron のみ
---drawSectionVector(bodyId,plane,vectorName,factor,colorMap)
	節点値の vectorName の値で colorMap に従い factor 倍のベクトル表示をする。
---drawSectionField(bodyId,plane,vectorName,size,colorMap)
	節点値の vectorName の値で colorMap に従い、長さ size 一定のベクトル表示をする。
=end
---------------------------------------------------------------------------*/
	void drawSection(kmb::bodyIdType bodyId,kmb::Plane* plane);
	void drawSectionNodeContour(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* physicalValue, kmb::ColorMap* colorMap,int comp=-1);
	void drawSectionContour(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* physicalValue, kmb::ColorMap* colorMap,int comp=-1);
	void drawSectionVector(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* vectorName, double factor, kmb::ColorMap* colorMap);
	void drawSectionField(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* vectorName, double arrowSize, kmb::ColorMap* colorMap);
/**--------------------------------------------------------------------------
=begin
--- drawTrajectory(vectorVal,x,y,z,delta,step,colorMap,tolerance=0.0)
	ベクトル値関数 vectorVal を (x,y,z) から delta 間隔で step 数だけ流線を書く
=end
---------------------------------------------------------------------------*/
	int drawTrajectory(const char* vectorVal,double x,double y,double z,double delta,int step, kmb::ColorMap* colorMap, double tolerance=0.0);

/**--------------------------------------------------------------------------
=begin
---drawIntersection(bodyId,plane)
	bodyId の Body を plane と交差する要素を除いた面を表示する

---drawIntersectionContour(bodyId,plane,physicalValue,colorMap,comp=-1);
	bodyId の Body を plane と交差する要素を除いた面を
	physicalValue の値でコンター表示する
=end
---------------------------------------------------------------------------*/
	void drawIntersection(kmb::bodyIdType bodyId,kmb::Plane* plane);
	void drawIntersectionContour(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);

/**--------------------------------------------------------------------------
=begin
---drawMarking(condName,mode)
	MeshDB に condName で格納されている境界条件について mode のマーカーを
	表示する。
	利用可能な mode については以下のとおり
	 * VECTOR 節点上に矢印
	 * TENSOR 節点上に交差面
	 * DOT 節点上に点
	 * CROSS 節点上に十字
	 * CDOT 要素の中心に点
	 * CCROSS 要素の中心に十字
	 * EDGE 要素の辺を表示
	 * DAGGER 節点上に十字
	 * CIRCLE 節点上に丸印
---drawMarkingEdgeWithScale(bodyId,mode,x,y,z,scale)
	MeshDB の１次元要素からなる Body を bodyId で与えて、それに対して mode の
	マーカーを表示する。
=end
---------------------------------------------------------------------------*/
	void drawMarking(const char* condName,const char* mode);
	void drawMarkingEdgeWithScale(kmb::bodyIdType bodyId,const char* mode,double x,double y,double z,double scale);
/**--------------------------------------------------------------------------
=begin
---setMarkSize(size)
	マーカーの基準サイズを設定する。
---getMarkSize
	マーカーの基準サイズを取得する。
---setNodeOffset(offset)
	ID 表示の時の節点 ID のオフセット値を設定する
---setElementOffset(offset)
	ID 表示の時の要素 ID のオフセット値を設定する
=end
---------------------------------------------------------------------------*/
	void setMarkSize(double size);
	double getMarkSize(void);
	void setNodeOffset(int offset);
	void setElementOffset(int offset);

/**--------------------------------------------------------------------------
=begin
---getDeformFaceGroupBoundingBox(faceName,displacement,factor)
	deformFaceGroup の同じ引数で表示される変形図の BoundingBox を
	[[max_x,max_y,max_z],[min_x,min_y,min_z]] 形式で返す。
---getDeformXYZFaceGroupBoundingBox(faceName,displacementX,displacementY,displacementZ,factor)
	deformXYZFaceGroup の同じ引数で表示される変形図の BoundingBox を
	[[max_x,max_y,max_z],[min_x,min_y,min_z]] 形式で返す。
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getDeformFaceGroupBoundingBox(const char* faceName, const char* displacement, double factor){
		kmb::BoundingBox boundingbox;
		self->getDeformFaceGroupBoundingBox(boundingbox,faceName,displacement,factor);
		const kmb::Point3D bmax = boundingbox.getMax();
		const kmb::Point3D bmin = boundingbox.getMin();
		VALUE bbox = rb_ary_new();
		VALUE bboxMax = rb_ary_new();
		VALUE bboxMin = rb_ary_new();
		rb_ary_store(bbox,0,bboxMax);
		rb_ary_store(bbox,1,bboxMin);
		rb_ary_store(bboxMax,0,rb_float_new(bmax.x()));
		rb_ary_store(bboxMax,1,rb_float_new(bmax.y()));
		rb_ary_store(bboxMax,2,rb_float_new(bmax.z()));
		rb_ary_store(bboxMin,0,rb_float_new(bmin.x()));
		rb_ary_store(bboxMin,1,rb_float_new(bmin.y()));
		rb_ary_store(bboxMin,2,rb_float_new(bmin.z()));
		return bbox;
	}
	VALUE getDeformXYZFaceGroupBoundingBox(const char* faceName, const char* displacementX, const char* displacementY, const char* displacementZ, double factor){
		kmb::BoundingBox boundingbox;
		self->getDeformXYZFaceGroupBoundingBox(boundingbox,faceName,displacementX,displacementY,displacementZ,factor);
		const kmb::Point3D bmax = boundingbox.getMax();
		const kmb::Point3D bmin = boundingbox.getMin();
		VALUE bbox = rb_ary_new();
		VALUE bboxMax = rb_ary_new();
		VALUE bboxMin = rb_ary_new();
		rb_ary_store(bbox,0,bboxMax);
		rb_ary_store(bbox,1,bboxMin);
		rb_ary_store(bboxMax,0,rb_float_new(bmax.x()));
		rb_ary_store(bboxMax,1,rb_float_new(bmax.y()));
		rb_ary_store(bboxMax,2,rb_float_new(bmax.z()));
		rb_ary_store(bboxMin,0,rb_float_new(bmin.x()));
		rb_ary_store(bboxMin,1,rb_float_new(bmin.y()));
		rb_ary_store(bboxMin,2,rb_float_new(bmin.z()));
		return bbox;
	}
}
};

}
