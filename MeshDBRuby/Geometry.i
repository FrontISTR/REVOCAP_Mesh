/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.7                          #
# Ruby Module Name : RevocapMesh                                       #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2016/10/04     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/

%{
#include "Geometry/kmbGeometry2D.h"
#include "Geometry/kmbGeometry3D.h"
#include "Geometry/kmbGeometry4D.h"
#include "Geometry/kmbRegion.h"
#include "Geometry/kmbBoxRegion.h"
%}

namespace kmb{

class Tuple2D
{
public:
	Tuple2D(void);
	Tuple2D(const double x, const double y);
	Tuple2D(const Tuple2D &other);
	virtual ~Tuple2D(void);
	double x() const;
	double y() const;
	void x(double x);
	void y(double y);
	double getCoordinate(int i) const;
	void setCoordinate(int i,double v);
	void setCoordinate(double x,double y);
	void set(const Tuple2D& other);
	void addCoordinate(const double x,const double y);
	void scale(const double r);
	void zero(void);
	bool isZero(void) const;
};

class Vector2D;

class Point2D : public Tuple2D
{
public:
	Point2D(void);
	Point2D(const double x, const double y);
	Point2D(const Tuple2D &other);
	Point2D operator+(const Vector2D& other);
	Point2D operator-(const Vector2D& other);
	bool operator==(const Point2D& other) const;
	double distance(const Point2D& other) const;
	double distance(double x,double y) const;
	double distanceSq(const Point2D& other) const;
	double distanceSq(double x,double y) const;
	// distance from segment (not length of foot of a perpendicular)
	double distanceSqToSegment(const Point2D& a,const Point2D& b) const;
	// distance from segment (not length of foot of a perpendicular)
	// also return segment parameter t
	double distanceSqToSegment(const Point2D& a,const Point2D& b,double& t) const;
	double distanceToSegment(const Point2D& a,const Point2D& b) const;
	// this と other を m:n に内分した点を返す
	Point2D dividingPoint(const Point2D& other,double m,double n) const;

	// 三角形の面積
	// a,b,c が半時計回りの時に正
	static double area(const Point2D& a,const Point2D& b,const Point2D &c);
	static Point2D getCenter(const Point2D& a,const Point2D& b);
	static Point2D getCenter(const Point2D& a,const Point2D& b,const Point2D &c);
	// 角度（ラジアン）
	// -PI < theta <= PI
	static double angle(const Point2D &a,const Point2D &b,const Point2D &c);
	// 0 <= theta < 2*PI
	static double angle2(const Point2D &a,const Point2D &b,const Point2D &c);
	static double cos(const Point2D &a,const Point2D &b,const Point2D &c);
	static double sin(const Point2D &a,const Point2D &b,const Point2D &c);
	// 面積座標
	static void calcMinorCoordinate( const kmb::Point2D& a, const kmb::Point2D& b, const kmb::Point2D& c, const kmb::Point2D& x, double coordinate[3]);
	static Point2D infinity;
	// ２つの線分 a0a1 と b0b1 が交差しているか
	static bool intesectSegments(const kmb::Point2D& a0, const kmb::Point2D& a1, const kmb::Point2D& b0, const kmb::Point2D& b1);
};

class Vector2D : public Tuple2D
{
public:
	Vector2D(void) : Tuple2D();
	Vector2D(const double x, const double y);
	// p - q
	/// p が始点, q が終点
	Vector2D(const Point2D& p,const Point2D& q);
	Vector2D(const Tuple2D &other);
	virtual ~Vector2D(void);
	/// スカラー積
	Vector2D scalar(const double s) const;
	double lengthSq() const;
	double length() const;
	double abs(void) const; // length と同じ意味
	double normalize();
	void rotate(double angle);
///// static
	static double cos(const Vector2D &v0,const Vector2D &v1);
	static double sin(const Vector2D &v0,const Vector2D &v1);
	static double angle(const Vector2D &v0,const Vector2D &v1);
	static double angle2(const Vector2D &v0,const Vector2D &v1);
	static double abs(const double v[2]);
};

//
// 2x2 行列
// 

class Matrix2x2
{
public:
	Matrix2x2(void);
	Matrix2x2(double m[4]);
	Matrix2x2(
		double m00,double m01,
		double m10,double m11);
	virtual ~Matrix2x2(void);
	virtual const char* getContainerType(void) const;
	int init(int rowSize, int colSize);
	int getSize(void) const;
	double get(int i,int j) const;
	bool set(int i,int j,double val);
	bool add(int i,int j,double val);
	double operator()(int i,int j) const;

	bool identity(void);
	bool zero(void);
	bool transpose(void);
	void transpose(const Matrix2x2& other);
	static Matrix2x2 createRotation(double angle);
	// v0 が1列目になるように Gram-Schmidt 直交化行列を作る
	static Matrix2x2 createSchmidtRotation(const Vector2D v0,bool column=true);
	double determinant(void) const;
	double trace(void) const;
	static double determinant(
		double m00,double m01,
		double m10,double m11);
	static double trace(
		double m00,double m01,
		double m10,double m11);
	// get inverse matrix
	Matrix2x2* getInverse(void) const;
	/// this * x = b なる方程式の解の x を返す
	bool solve(const Vector2D& b,Vector2D& x) const;
	bool solveSafely(const Vector2D& b,Vector2D& x,double thresh=1.0e-6) const;
	/// 行列の掛け算
	// 右から縦ベクトルを掛ける
	Vector2D operator*(const Vector2D& vect);
	Matrix2x2 operator*(const Matrix2x2& other);
	/// 行列の足し算、引き算
	Matrix2x2 operator+(const Matrix2x2& other);
	Matrix2x2 operator-(const Matrix2x2& other);
};

class Tuple3D
{
public:
	Tuple3D(void);
	Tuple3D(const double x, const double y, const double z);
	Tuple3D(const Tuple3D &other);
	virtual ~Tuple3D(void);
	double x() const;
	double y() const;
	double z() const;
	void x(const double x);
	void y(const double y);
	void z(const double z);
	double getCoordinate(const int i) const;
	void setCoordinate(const int i,const double v);
	void setCoordinate(const double x,const double y,const double z);
	// 代入演算子にすると、継承の時におかしな動作をする
	void set(const Tuple3D& other);
	void addCoordinate(const int i,const double v);
	void addCoordinate(const double x,const double y,const double z);
	void scale(const double r);
	void zero(void);
	bool isZero(void) const;
};

class Vector3D;

class Point3D : public Tuple3D
{
public:
	Point3D(void);
	Point3D(const double x, const double y, const double z);
	Point3D(const Tuple3D &other);
	virtual ~Point3D(void);
	Point3D operator+(const Vector3D& other) const;
	Point3D operator-(const Vector3D& other) const;
	bool operator==(const Point3D& other) const;
	// this と other を m:n に内分した点を返す
	Point3D dividingPoint(const Point3D& other,double m,double n) const;
	// this を 0 として other を 1 とした時に t となる点を返す
	// t : 1-t に内分した点を返すのと同じ意味
	// t = m / (m+n) として dividingPoint をしたのと同じ意味
	Point3D proportionalPoint(const Point3D& other,double t) const;

	// ２点間の距離
	double distance(const Point3D& other) const;
	double distance(double x,double y,double z) const;
	double distanceSq(const Point3D& other) const;
	double distanceSq(double x,double y,double z) const;
	// 線分（直線ではない）AB との間の距離 注：垂線の足の長さではない
	// a から b へのパラメータ t も返す
	double distanceSqToSegment(const Point3D& a,const Point3D& b) const;
	double distanceSqToSegment(const Point3D& a,const Point3D& b,double& t) const;
	double distanceToSegment(const Point3D& a,const Point3D& b) const;
	// 三角形 ABC との距離
	double distanceSqToTriangle(const Point3D& a,const Point3D& b,const Point3D& c,double* t=NULL) const;
	double distanceToTriangle(const Point3D& a,const Point3D& b,const Point3D& c) const;
	double distanceToTriangle(const Point3D& a, const Point3D& b, const Point3D& c, Point3D& nearest,int &flag) const;
	// 垂線の足
	// この点を origin を原点とする uv 平面に射影した垂線の足が au + bv 
	bool getFootOfPerpendicular( const Point3D &origin, const Vector3D &u, const Vector3D &v, double &a, double &b) const;

	// 四面体の体積
	static double volume(const Point3D& a,const Point3D& b,const Point3D &c,const Point3D &d);
	// abc の向きで正かどうか
	static bool positive(const Point3D& a,const Point3D& b,const Point3D &c,const Point3D &x);
	// 三角形の面積
	static double area(const Point3D& a,const Point3D& b,const Point3D &c);
	static Vector3D areaVector(const Point3D& a,const Point3D& b,const Point3D &c);
	// 中点、重心
	static Point3D getCenter(const Point3D& a,const Point3D& b);
	static Point3D getCenter(const Point3D& a,const Point3D& b,const Point3D &c);
	static Point3D getCircumCenter(const Point3D& a,const Point3D& b,const Point3D &c);
	static Point3D getCenter(const Point3D& a,const Point3D& b,const Point3D &c,const Point3D &d);
	static Point3D getCircumCenter(const Point3D& a,const Point3D& b,const Point3D &c,const Point3D &d);

	// 角度（ラジアン）∠ABC
	static double angle(const Point3D &a,const Point3D &b,const Point3D &c);
	static double cos(const Point3D &a,const Point3D &b,const Point3D &c);
	static double sin(const Point3D &a,const Point3D &b,const Point3D &c);
	// 体積座標
	static void calcMinorCoordinate( const kmb::Point3D& a, const kmb::Point3D& b, const kmb::Point3D& c, const kmb::Point3D& d, const kmb::Point3D& x, double coordinate[4]);
	// 法線
	static Vector3D calcNormalVector( const kmb::Point3D& a, const kmb::Point3D& b, const kmb::Point3D& c );
	static Vector3D calcNormalVector( const kmb::Point3D& a, const kmb::Point3D& b, const kmb::Point3D& c, const kmb::Point3D& d );
	static Vector3D calcNormalVector(
		const double x0, const double y0, const double z0,
		const double x1, const double y1, const double z1,
		const double x2, const double y2, const double z2);
	static Vector3D calcNormalVector(
		const double x0, const double y0, const double z0,
		const double x1, const double y1, const double z1,
		const double x2, const double y2, const double z2,
		const double x3, const double y3, const double z3);
	static Point3D infinity;
};

class Vector3D : public Tuple3D
{
public:
	Vector3D(void);
	Vector3D(const double x, const double y, const double z);
	// p - q
	// 始点が q 終点が p
	Vector3D(const Point3D& endPoint,const Point3D& startPoint);
	Vector3D(const Tuple3D &other) : Tuple3D(other){};
	virtual ~Vector3D(void){};
	Vector3D operator+(const Vector3D& other) const;
	Vector3D operator-(const Vector3D& other) const;
	Vector3D operator-(void) const;
	/// スカラー積
	Vector3D scalar(const double s) const;
	/// 内積
	double operator*(const Vector3D& other) const;
	/// 外積
	Vector3D operator%(const Vector3D& other) const;
	double lengthSq(void) const;
	double length(void) const;
	double abs(void) const; // length と同じ意味
	double normalize();
///// static
	static double triple(const Vector3D &v0,const Vector3D &v1,const Vector3D &v2);
	static double cos(const Vector3D &v0,const Vector3D &v1);
	static double sin(const Vector3D &v0,const Vector3D &v1);
	static double angle(const Vector3D &v0,const Vector3D &v1);
	static double abs(const double v[3]);
	static double inner(const double v0[3],const double v1[3]);
};

//
// 3x3 行列
// 
//	m[0] = m00;	m[3] = m01;	m[6] = m02;
//	m[1] = m10;	m[4] = m11;	m[7] = m12;
//	m[2] = m20;	m[5] = m21;	m[8] = m22;
//
// 添え字のつき方に注意せよ
//

class Matrix3x3
{
public:
	Matrix3x3(void);
	// 注意：以下の２つのコンストラクタは引数の順番が違う
	Matrix3x3(double m[9]);
	Matrix3x3(
		double m00,double m01,double m02,
		double m10,double m11,double m12,
		double m20,double m21,double m22);
	virtual ~Matrix3x3(void){};
	virtual const char* getContainerType(void) const;
	int init(int rowSize, int colSize);
	int getSize(void) const;
	double get(int i,int j) const;
	bool set(int i,int j,double val);
	bool add(int i,int j,double val);
	double operator()(int i,int j) const;

	bool identity(void);
	bool zero(void);
	bool transpose(void);
	void transpose(const Matrix3x3& other);
	static Matrix3x3 createRotation(double angle,const char* axis="z");
	static Matrix3x3 createRotation(double angle,const Vector3D& axis);
	static Matrix3x3 createReflection(const Vector3D& axis);
	// v0 が1列目、v1 が2列目になるように Gram-Schmidt 直交化行列を作る
	static Matrix3x3 createSchmidtRotation(const Vector3D v0,const Vector3D v1,bool column=true);
	double determinant(void) const;
	double trace(void) const;
	static double determinant(
		double m00,double m01,double m02,
		double m10,double m11,double m12,
		double m20,double m21,double m22);
	static double trace(
		double m00,double m01,double m02,
		double m10,double m11,double m12,
		double m20,double m21,double m22);
	// get inverse matrix
	Matrix3x3* getInverse(void) const;
	/// this * x = b なる方程式の解の x を返す
	bool solve(const Vector3D& b,Vector3D& x) const;
	bool solveSafely(const Vector3D& b,Vector3D& x,double thresh=1.0e-6) const;
	/// 行列の掛け算（射影変換を施す）
	// 右から縦ベクトルを掛ける tuple = this * tuple
	void convert(Tuple2D& tuple) const;
	void convert(Tuple3D& tuple) const;
	Vector2D operator*(const Vector2D& vect);
	Vector3D operator*(const Vector3D& vect);
	Matrix3x3 operator*(const Matrix3x3& other);
	/// 行列の足し算、引き算
	Matrix3x3 operator+(const Matrix3x3& other);
	Matrix3x3 operator-(const Matrix3x3& other);
};

class Tuple4D
{
public:
	Tuple4D(void);
	Tuple4D(const double x, const double y, const double z,const double w);
	Tuple4D(const Tuple4D &other);
	virtual ~Tuple4D(void);
	double x() const;
	double y() const;
	double z() const;
	double w() const;
	void x(const double x);
	void y(const double y);
	void z(const double z);
	void w(const double w);
	double getCoordinate(const int i) const;
	void setCoordinate(const int i,const double v);
	void setCoordinate(const double x,const double y,const double z,const double w);
	void set(const Tuple4D& other);
	void addCoordinate(const double x,const double y,const double z,const double w);
	void scale(const double r);
	void zero(void);
	bool isZero(void) const;
};

class Vector4D;

class Point4D : public Tuple4D
{
public:
	Point4D(void);
	Point4D(const double x, const double y, const double z, const double w);
	Point4D(const Tuple4D &other);
	Point4D operator+(const Vector4D& other) const;
	Point4D operator-(const Vector4D& other) const;
	bool operator==(const Point4D& other) const;
	// ２点間の距離
	double distance(const Point4D& other) const;
	double distance(double x,double y,double z,double w) const;
	double distanceSq(const Point4D& other) const;
	double distanceSq(double x,double y,double z,double w) const;
	static Point4D infinity;
};

class Vector4D : public Tuple4D
{
public:
	Vector4D(void);
	Vector4D(const double x, const double y, const double z,const double w);
	Vector4D(const Tuple4D &other);
	virtual ~Vector4D(void);
	Vector4D operator+(const Vector4D& other) const;
	Vector4D operator-(const Vector4D& other) const;
	/// スカラー積
	Vector4D scalar(const double s) const;
	double lengthSq(void) const;
	double length(void) const;
	double abs(void) const; // length と同じ意味
	double normalize();
};

// matrix の並び順
// m[0] m[4] m[8]  m[12]
// m[1] m[5] m[9]  m[13]
// m[2] m[6] m[10] m[14]
// m[3] m[7] m[11] m[15]


class Matrix4x4
{
public:
	Matrix4x4(void);
	Matrix4x4(double m[16]);
	// 代入される順番に注意
	// m[0] m[4] m[8]  m[12]  = m00 m01 m02 m03
	// m[1] m[5] m[9]  m[13]    m10 m11 m12 m13
	// m[2] m[6] m[10] m[14]    m20 m21 m22 m23
	// m[3] m[7] m[11] m[15]    m30 m31 m32 m33
	Matrix4x4(
		double m00,double m01,double m02,double m03,
		double m10,double m11,double m12,double m13,
		double m20,double m21,double m22,double m23,
		double m30,double m31,double m32,double m33);
	Matrix4x4(const Matrix4x4 &other);
	Matrix4x4(const Matrix3x3 &other);

	virtual ~Matrix4x4(void){};
	virtual const char* getContainerType(void) const;
	int init(int rowSize, int colSize);
	int getSize(void) const;
	double get(int i,int j) const;
	bool set(int i,int j,double val);
	bool add(int i,int j,double val);

	bool identity(void);
	bool zero(void);
	/// 転置行列
	bool transpose(void);
	void transpose(const Matrix4x4& other);
	/// 行列式
	double determinant(void) const;
	double trace(void) const;
	static double determinant(
		double m00,double m01,double m02,double m03,
		double m10,double m11,double m12,double m13,
		double m20,double m21,double m22,double m23,
		double m30,double m31,double m32,double m33);
	static double trace(
		double m00,double m01,double m02,double m03,
		double m10,double m11,double m12,double m13,
		double m20,double m21,double m22,double m23,
		double m30,double m31,double m32,double m33);
	/// this * x = b なる方程式の解の x を返す
	bool solve(const Vector4D& b,Vector4D& x) const;
	bool solveSafely(const Vector4D& b,Vector4D& x,double thresh=1.0e-6) const;
	/// 行列の掛け算
	// [A t] * v = (a*v + t) / (s*v + r)
	// [s r]
	void convert(Tuple3D& tuple) const;
	void convert(Tuple4D& tuple) const;
	void convert(double &x, double &y, double &z) const;
	Vector3D operator*(const Vector3D& vect);
	Vector4D operator*(const Vector4D& vect);
	Matrix4x4 operator*(const Matrix4x4& other);
	/// 行列の足し算、引き算
	Matrix4x4 operator+(const Matrix4x4& other);
	Matrix4x4 operator-(const Matrix4x4& other);

	/// AFFINE type 
	bool isAffineType(void) const;
	// 角度はラジアンで与える
	static Matrix4x4 createRotation(const Vector3D& axis, double angle);
	static Matrix4x4* createLookAt(const Vector3D& eye, const Vector3D& center, const Vector3D& up);
	static Matrix4x4* createAffine(const Matrix3x3& transform, const Vector3D& displace);
	bool getLookAt(Vector3D& eye, Vector3D& center, Vector3D& up) const;

	// 平行移動成分
	void translate(double x,double y,double z);
	// スカラー倍（平行移動成分も変わる）
	void scale(double r);
};

class BoxRegion;

class Region
{
public:
	enum locationType{
		UNKNOWN = -1,
		INSIDE,
		OUTSIDE,
		ON_BOUNDARY
	};
	Region(void);
	virtual ~Region(void);
	/// 判定関数
	virtual locationType intersect(const Point3D &point) const = 0;
	virtual bool intersect(const BoxRegion& box) const = 0;
	/// 距離関数
	virtual double distanceSq(const Point3D &point) const = 0;
	virtual double distanceSq(const double x,const double y,const double z) const;
	virtual double distance(const Point3D& point) const;
	virtual double distance(const double x,const double y,const double z) const;
};

class BoxRegion : public Region
{
public:
	BoxRegion(void);
	BoxRegion(const Point3D &l,const Point3D &u);
	BoxRegion(double x0,double y0,double z0,double x1,double y1,double z1);
	BoxRegion(const BoxRegion &other);
	virtual ~BoxRegion(void);

	void setMinMax(double x0,double y0,double z0,double x1,double y1,double z1);
	void setMinMax(kmb::Point3D& minPoint,kmb::Point3D& maxPoint);
	double minX(void) const;
	double minY(void) const;
	double minZ(void) const;
	double maxX(void) const;
	double maxY(void) const;
	double maxZ(void) const;
	void minX(double x);
	void minY(double y);
	void minZ(double z);
	void maxX(double x);
	void maxY(double y);
	void maxZ(double z);
	double centerX(void) const;
	double centerY(void) const;
	double centerZ(void) const;
	double rangeX(void) const;
	double rangeY(void) const;
	double rangeZ(void) const;
	// rangeX rangeY rangeZ の大きい方を返す
	double range(void) const;
	double diameter(void) const;
	double diameterSq(void) const;
	const Point3D& getMin(void) const;
	const Point3D& getMax(void) const;
	void getCenter(kmb::Point3D& center) const;
	double getVolume(void) const;

	virtual kmb::Region::locationType intersect(const kmb::Point3D &point) const;
	virtual kmb::Region::locationType intersect(const double x,const double y,const double z) const;
	virtual double distanceSq(const kmb::Point3D &point) const;
	virtual double distanceSq(const double x,const double y,const double z) const;
	// Box 同士の判定関数
	bool intersect(const kmb::BoxRegion& box) const;
	double intersectVolume(const BoxRegion& box) const;
	double distanceSq(const kmb::BoxRegion& box) const;
	// max min の両方を拡大する
	void expand(double x,double y,double z);
	void expand(double ratio);
	// 平行移動
	void translate(double x,double y,double z);
	// center を変えずに range を変える
	void setRange(double rangeX,double rangeY,double rangeZ);
	// diameter が ratio 倍になるように x y z を同じ長さだけ広げる
	void expandDiameter(double ratio);
	// 直線と重なっている部分
	// 重なっていなければ、mint_t = max_t = 0.0 を返す
	void crossOnLine(const kmb::Point3D& origin, const kmb::Vector3D& dir, double &min_t, double &max_t) const;
	// 三角形との判定
	// ab bc ca が6面と交差するかどうかの判定
	// 今の実装では intersectArea > 0.0 とした方が速い
	bool intersect(const kmb::Point3D &a,const kmb::Point3D &b,const kmb::Point3D &c) const;
	// intersectArea から Area を除いたもの
	bool intersect3(const kmb::Point3D &a,const kmb::Point3D &b,const kmb::Point3D &c) const;
	// 三角形との判定（面積つき）
	// 法線ベクトルがほしい場合もここで面積を計算してから法線を新たに計算するほうが速い
	// 理由：Vector3D 型を戻り値にするオーバーヘッドが大きい
	double intersectArea(const kmb::Point3D &a,const kmb::Point3D &b,const kmb::Point3D &c) const;
};

}
