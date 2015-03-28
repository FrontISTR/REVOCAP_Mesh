/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : MeshGL                                                  #
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
#pragma once



#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbElementContainer.h"

#ifdef WIN32
#include <windows.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <vector>

namespace kmb{

class MeshData;
class ColorMap;
class BoundingBox;
class DataBindings;
class PhysicalValue;
class Plane;

class MeshGL
{
public:
	enum drawingPreference{
		PRECISION,
		SPEED
	};
protected:
	const MeshData* mesh;

	std::vector< GLuint > lists;
	bool updateList;
private:

	float rgb[3];
	float rgba[4];
	kmb::Node m0, m1, m2, m3, m4;

	double markSize;
	GLfloat pointSize;
	GLfloat lineWidth;
	int nodeOffset;
	int elementOffset;
	drawingPreference prefMode;
public:
	MeshGL(size_t listSize=1);
	virtual ~MeshGL(void);


	size_t getListSize(void) const;
	void setList(size_t index,GLuint list);
	GLuint getList(size_t index) const;
	void update(bool flag=true);
	bool isUpdate(void) const;
	void clearCache(void);


	void setPreferenceMode(drawingPreference mode);
	drawingPreference getPreferenceMode(void) const;

	void setMesh(const kmb::MeshData* mesh);

	void drawEdge(kmb::bodyIdType bodyId);

	void drawEdgeWithScale(kmb::bodyIdType bodyId,double x,double y,double z,double scale);


	void drawSurface(kmb::bodyIdType bodyId);
	void drawFaceGroup(const char* faceName);
	void drawSurface2D(kmb::bodyIdType bodyId);


	void deformSurface(kmb::bodyIdType bodyId, const char* displacement, double factor);
	void deformFaceGroup(const char* faceName, const char* displacement, double factor);
	void deformXYZFaceGroup(const char* faceName, const char* displacementX, const char* displacementY, const char* displacementZ, double factor);



	void drawSurfaceContour(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);
	void drawSurfaceContour2D(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);

	void drawFaceGroupContour(const char* faceName, const char* physicalValue, kmb::ColorMap* colorMap,int comp=-1);

	void drawSurfaceNodeContour(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);
	void drawSurfaceNodeContour2D(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);
	void drawFaceGroupNodeContour(const char* faceName, const char* physicalValue, kmb::ColorMap* colorMap,int comp=-1);

	void drawNodeContourOnSurface(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);
	void drawNodeContour2DOnSurface(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);
	void drawNodeContourOnFaceGroup(const char* faceName, const char* physicalValue, kmb::ColorMap* colorMap,int comp=-1);


	void deformSurfaceNodeContour(kmb::bodyIdType bodyId, const char* displacement, double factor, const char* physicalValue, kmb::ColorMap* colorMap, int ccomp=-1);
	void deformSurfaceContour(kmb::bodyIdType bodyId, const char* displacement, double factor, const char* physicalValue, kmb::ColorMap* colorMap, int ccomp=-1);
	void deformFaceGroupNodeContour(const char* faceName, const char* displacement, double factor, const char* physicalValue, kmb::ColorMap* colorMap, int comp=-1);
	void deformXYZFaceGroupNodeContour(const char* faceName, const char* displacementX, const char* displacementY, const char* displacementZ, double factor, const char* physicalValue, kmb::ColorMap* colorMap, int comp=-1);


	void drawAllNodes(double r=-1.0);
	void drawNodeContourAllNodes(const char* physicalValue, kmb::ColorMap* colorMap,int comp=-1);


	void drawVector(const char* vectorName, double factor, kmb::ColorMap* colorMap);
	void drawSurfaceVector(kmb::bodyIdType bodyId,const char* vectorName, double factor, kmb::ColorMap* colorMap);
	void drawFaceGroupVector(const char* faceName,const char* vectorName, double factor, kmb::ColorMap* colorMap);

	void drawField(const char* vectorName, double arrowSize, kmb::ColorMap* colorMap);
	void drawSurfaceField(kmb::bodyIdType bodyId,const char* vectorName, double arrowSize, kmb::ColorMap* colorMap);

	void drawIsosurface(kmb::bodyIdType bodyId, const char* nodeValue, double val);


	void drawSection(kmb::bodyIdType bodyId,kmb::Plane* plane);

	void drawSectionNodeContour(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* physicalValue, kmb::ColorMap* colorMap,int comp=-1);

	void drawSectionContour(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* physicalValue, kmb::ColorMap* colorMap,int comp=-1);
	void drawSectionVector(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* vectorName, double factor, kmb::ColorMap* colorMap);
	void drawSectionField(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* vectorName, double arrowSize, kmb::ColorMap* colorMap);

	void drawIntersection(kmb::bodyIdType bodyId,kmb::Plane* plane);

	void drawIntersectionContour(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);

	void getDeformFaceGroupBoundingBox(kmb::BoundingBox& bbox,const char* faceName, const char* displacement, double factor) const;
	void getDeformXYZFaceGroupBoundingBox(kmb::BoundingBox& bbox,const char* faceName, const char* displacementX, const char* displacementY, const char* displacementZ, double factor) const;

	int drawTrajectory(const char* vectorVal,double x,double y,double z,double delta,int step, kmb::ColorMap* colorMap, double tolerance=0.0);

	void volumeRendering(const char* physicalValue, kmb::ColorMap* colorMap,int repeatLevel,int numParticle,bool volCorrect=true,int comp=-1);

	/**
	 * condName : data の名前
	 * mode : marker の種類
	 * VECTOR 節点上に矢印
	 * TENSOR 節点上に交差面
	 * DOT 節点上に点
	 * CROSS 節点上に十字
	 * CDOT 要素の中心に点
	 * CCROSS 要素の中心に十字
	 * CTENSOR 要素の中心に交差面
	 * CSQUARE 要素の中心に四角
	 * EDGE 要素の辺を表示
	 * DAGGER 節点上に十字
	 * CIRCLE 節点上に丸印
	 * DELTA 節点上に三角
	 * SQUARE 節点上に四角
	 * LEFT 節点上に＜
	 * RIGHT 節点上に＞
	 * NCOUPLE 節点vs節点マッピングに対して対応節点上に十字
	 *  同じ Data でも DOT では master（自節点）にマーク、NCOUPLE では slave（相手節点）にマーク
	 */
	void drawMarking(const char* condName,const char* mode);
	void drawMarkingEdgeWithScale(kmb::bodyIdType bodyId,const char* mode,double x,double y,double z,double scale);

	void setMarkSize(double size);
	double getMarkSize(void) const;

	void setPointSize(GLfloat size);
	GLfloat getPointSize(void) const;

	void setLineWidth(GLfloat size);
	GLfloat getLineWidth(void) const;

	void setNodeOffset(int offset);
	int getNodeOffset(void) const;

	void setElementOffset(int offset);
	int getElementOffset(void) const;

private:

	void drawTensorMark(kmb::Point3D& point);
	void drawTensorMark(double x,double y,double z);

	void drawCrossMark(kmb::Point3D& point);
	void drawCrossMark(double x,double y,double z);
	void drawDotMark(kmb::Point3D& point);
	void drawDotMark(double x,double y,double z);
	void drawDaggerMark(double x,double y,double z);
	void drawCircleMark(double x,double y,double z);
	void drawNumericMark(double x,double y,double z,long l);
	void drawDeltaMark(double x,double y,double z);
	void drawSquareMark(double x,double y,double z);
	void drawLeftMark(double x,double y,double z);
	void drawRightMark(double x,double y,double z);
	void drawVectorMark(kmb::Point3D& point,kmb::Vector3D& vec);
	void drawVectorMark(double x,double y,double z,double v0,double v1,double v2);
	void drawElementEdge(kmb::ElementBase& element);
	void drawFaceEdge(kmb::ElementBase& element,int index);
	void drawVector(double x,double y,double z,double v0,double v1,double v2,double factor,kmb::ColorMap* colorMap);

	void drawArrow(double x,double y,double z,double v0,double v1,double v2,double arrowSize,kmb::ColorMap* colorMap);

	void drawTriangle
		(double x0,double y0,double z0,
		 double x1,double y1,double z1,
		 double x2,double y2,double z2);
	void drawTriangle2D
		(double x0,double y0,
		 double x1,double y1,
		 double x2,double y2);
	void drawTriangleByDividingPoint
		(kmb::Node* n00,kmb::Node* n01,double t00,double t01,
		 kmb::Node* n10,kmb::Node* n11,double t10,double t11,
		 kmb::Node* n20,kmb::Node* n21,double t20,double t21);
	void drawContourTriangle
		(double x0,double y0,double z0,double v0,
		 double x1,double y1,double z1,double v1,
		 double x2,double y2,double z2,double v2,
		 kmb::ColorMap* colorMap);
	void drawContourTriangle2D
		(double x0,double y0,double v0,
		 double x1,double y1,double v1,
		 double x2,double y2,double v2,
		 kmb::ColorMap* colorMap);
	void drawSolidContourTriangle2D
		(double x0,double y0,double v0,
		 double x1,double y1,double v1,
		 double x2,double y2,double v2,
		 kmb::ColorMap* colorMap);
	void drawSolidContourTriangle
		(kmb::Node* node0,kmb::Node* node1,kmb::Node* node2,
		double v0,double v1,double v2,kmb::ColorMap* colorMap);
	void drawQuad
		(double x0,double y0,double z0,
		 double x1,double y1,double z1,
		 double x2,double y2,double z2,
		 double x3,double y3,double z3);
	void drawQuad2D
		(double x0,double y0,
		 double x1,double y1,
		 double x2,double y2,
		 double x3,double y3);
	void drawQuadByDividingPoint
		(kmb::Node* n00,kmb::Node* n01,double t00,double t01,
		 kmb::Node* n10,kmb::Node* n11,double t10,double t11,
		 kmb::Node* n20,kmb::Node* n21,double t20,double t21,
		 kmb::Node* n30,kmb::Node* n31,double t30,double t31);
	void drawContourQuad
		(double x0,double y0,double z0,double v0,
		 double x1,double y1,double z1,double v1,
		 double x2,double y2,double z2,double v2,
		 double x3,double y3,double z3,double v3,
		 kmb::ColorMap* colorMap);
	void drawContourQuad2D
		(double x0,double y0,double v0,
		 double x1,double y1,double v1,
		 double x2,double y2,double v2,
		 double x3,double y3,double v3,
		 kmb::ColorMap* colorMap);

	void drawIsosurfTetrahedron(
		kmb::nodeIdType nodeId0, kmb::nodeIdType nodeId1, kmb::nodeIdType nodeId2, kmb::nodeIdType nodeId3,
		const kmb::DataBindings* isoData,double val,int comp=-1);
	void drawSectionTetrahedron(
		kmb::Node& n0,kmb::Node& n1,kmb::Node& n2,kmb::Node& n3,
		double t0, double t1, double t2, double t3);
	void drawIntersectionTetrahedron(
		kmb::Node& n0,kmb::Node& n1,kmb::Node& n2,kmb::Node& n3,
		double t0, double t1, double t2, double t3);




	void drawIsosurfNodeContourTetrahedron(
		kmb::nodeIdType nodeId0, kmb::nodeIdType nodeId1, kmb::nodeIdType nodeId2, kmb::nodeIdType nodeId3,
		const kmb::DataBindings* isoData, double iso,const kmb::DataBindings* data, kmb::ColorMap* colorMap);
	void drawIsosurfTetrahedronContour
		(kmb::Node* n0,kmb::Node* n1,kmb::Node* n2,kmb::Node* n3,
		double t0, double t1, double t2, double t3,
		double v0, double v1, double v2, double v3,
		kmb::ColorMap* colorMap);
	void drawInterpolatedVector(kmb::Node& n0,kmb::Node& n1,double t0, double t1,double v0[3], double v1[3],double factor,kmb::ColorMap* colorMap);
	void drawInterpolatedField(kmb::Node& n0,kmb::Node& n1,double t0, double t1,double v0[3], double v1[3],double arrowSize,kmb::ColorMap* colorMap);



	double calcPerpendicular(double v0,double v1,double v2,kmb::Vector3D &normal);

	double calcFrame(double v0,double v1,double v2,kmb::Vector3D &normal0,kmb::Vector3D &normal1);

	double getMeanLengthVector
		( kmb::PhysicalValue* val0,
		  kmb::PhysicalValue* val1,
		  kmb::PhysicalValue* val2,
		  kmb::PhysicalValue* val3);

	double getMiddleNode
		( kmb::Node* node0, kmb::Node* node1,
		  double t0, double t1,
		  double v0, double v1,
		  kmb::Node &middle );

	void getMiddleNodeVector
		( kmb::Node& node0, kmb::Node& node1,
		  double t0, double t1,
		  double v0[3], double v1[3],
		  kmb::Node &middle, double middleVec[3]);

};

}
