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

==�T�v

RevocapMesh::MeshDB �Ɋi�[���ꂽ���b�V���f�[�^��
OpenGL �ō����ɕ`�悷�邽�߂̃N���X�ł���B

==���\�b�h�ꗗ

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
==���\�b�h
--- MeshGL.new(listSize=1)
	���b�V����p�`��N���X MeshGL �̃C���X�^���X�𐶐�����B
	listSize �� display ���\�b�h�̍Ō�̈����ŗ^������ listIndex �̌���\���B
	�����̎����ł� listSize ���� OpenGL �� DisplayList ���o���Ă������Ƃ��\�ɂ���B
	�����`��̕\�����قȂ�f�o�C�X�R���e�L�X�g�ɍs���A���ꂼ��� Cache �������ꍇ��
	listSize ���f�o�C�X�R���e�L�X�g�̌����^����B
=end
---------------------------------------------------------------------------*/
	MeshGL(size_t listSize=1);
	virtual ~MeshGL(void);
/**--------------------------------------------------------------------------
=begin
--- setPreferenceMode(mode)
	�`��̗D�惂�[�h��ݒ肷��
	���ݐݒ�\�Ȃ̂�
	- "PRECISION" ���x�D�惂�[�h
	- "SPEED" ���x�D�惂�[�h
	�ł���
--- getPreferenceMode
	���݂̕`��̗D�惂�[�h���擾����
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
	�`��p�̃L���b�V�����N���A����B
	������ display list �̍폜�ł���B
=end
---------------------------------------------------------------------------*/
	void clearCache(void);
/**--------------------------------------------------------------------------
=begin
--- display(mesh,updateFlag,listIndex=0)
--- display(listIndex=0)
	�u���b�N���`���āA���̃u���b�N���ł� GL �̖��߂� DisplayList �ɓo�^�E�`�悷��B
	���� DisplayList �ɓo�^�ς݂̏ꍇ�͂��̃f�[�^��`�悷��B
	�o�^����Ă��Ȃ��Ƃ��́A��U�u���b�N���̖��߂� DisplayList �ɓo�^���Ă���`�悷��B
	�߂�l�͐���ɕ`�悳�ꂽ���ǂ�����Ԃ��B
	listIndex �̒l�� MeshGL �������� Cache (= Display List) ���Ǘ�����ꍇ�ɁA
	�ǂ� Cache ���g������I������B-1 ��^����� Cache ���g�킸�Ƀu���b�N�������̂܂܎��s����B
	��F
	 meshGL.display{
	   meshGL.setMesh( mesh )
	   meshGL.drawFaceGroup("face_0")
	 }
	Display List ����蒼���ꍇ�idisplay �̃u���b�N�����X�V���ꂽ�ꍇ�j�́AMeshGL �� update ���\�b�h��
	�Ăяo��
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
	����`�悷��^�C�~���O�� Display List ����蒼��
=end
---------------------------------------------------------------------------*/

	void update(void);

/**--------------------------------------------------------------------------
=begin
--- getLists
	���ݗ��p���Ă��� DisplayList �� Index ��z��ŕԂ��B
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
	�`��̑ΏۂƂȂ� MeshDB �̃I�u�W�F�N�g���w�肷��B
=end
---------------------------------------------------------------------------*/
	void setMesh(kmb::MeshData* mesh);
/**--------------------------------------------------------------------------
=begin
---drawEdge(bodyId)
	�G�b�W��\������B�P�����v�f����Ȃ� Body �� BodyId ��^����B
---drawEdgeWithScale(bodyId,x,y,z,scale)
	�G�b�W�� (x,y,z) �𒆐S�� scale �{���ĕ`�悷��B
=end
---------------------------------------------------------------------------*/
	void drawEdge(kmb::bodyIdType bodyId);
	void drawEdgeWithScale(kmb::bodyIdType bodyId,double x,double y,double z,double scale);
/**--------------------------------------------------------------------------
=begin
---drawSurface(bodyId)
---drawSurface2D(bodyId)
	�T�[�t�F�X��\������B�Q�����v�f����Ȃ� Body �� BodyId ��^����B
---drawFaceGroup(faceName)
	��L�Ɠ��l�� bodyId �̑���� faceName ��^���ĕ`�悷��B
=end
---------------------------------------------------------------------------*/
	void drawSurface(kmb::bodyIdType bodyId);
	void drawSurface2D(kmb::bodyIdType bodyId);
	void drawFaceGroup(const char* faceName);
/**--------------------------------------------------------------------------
=begin
---drawAllNodes(r=-1.0)
    ���b�V���ɓo�^����Ă��邷�ׂĂ̐ߓ_��\������B
    r <= 0.0 �̎��� glVertex �ŕ`�悷��B�_�̑傫���͂��炩���� glPointSize �Ŏw�肷��B
    r > 0.0 �̎��� gluSphere �ŕ`�悷��B
=end
---------------------------------------------------------------------------*/
	void drawAllNodes(double r);
/**--------------------------------------------------------------------------
=begin
---drawSurfaceContour(bodyId,name,colormap)
	MeshDB �� name �Ƃ������O�Ŋi�[����Ă��镨���ʁiElementVariable ���[�h�AScalar �l�j
	�ɑ΂��āAcolormap �Œ�`���ꂽ�F�Â����@�ɂ���āA���ꂼ��̗v�f��
	�����F�����邱�ƂŁA�J���[�R���^�[�\�����s���B
=end
---------------------------------------------------------------------------*/
  void drawSurfaceContour(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);
  void drawSurfaceContour2D(kmb::bodyIdType bodyId,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);
/**--------------------------------------------------------------------------
=begin
---drawFaceGeoupContour(facename,name,colormap,comp=-1)
	MeshDB �� name �Ƃ������O�Ŋi�[����Ă��镨���ʁiElementVariable ���[�h�AInteger �l�j
	�ɑ΂��āAcolormap �Œ�`���ꂽ�F�Â����@�ɂ���āA���ꂼ��̗v�f��
	�����F�����邱�ƂŁAfacename �� FaceGroup �̃J���[�R���^�[�\�����s���B
=end
---------------------------------------------------------------------------*/
	void drawFaceGroupContour(const char* faceName, const char* physicalValue, kmb::ColorMap* colorMap,int comp=-1);
/**--------------------------------------------------------------------------
=begin
---drawSurfaceNodeContour(bodyId,name,colormap,comp=-1)
	MeshDB �� name �Ƃ������O�Ŋi�[����Ă��镨���ʁiNodeVariable ���[�h�AScalar �l�j
	�ɑ΂��āAcolormap �Œ�`���ꂽ�F�Â����@�ɂ���āA�J���[�R���^�[�\�����s���B
	colormap �� smoothness �� SOLID �̂Ƃ��ɂ́A���ꂼ��̗v�f�𓯂��F�̕��ŕ�������
	�F������BSMOOTH �̂Ƃ��ɂ͒��_���ƂɐF��^���āA�u�����h�� OpenGL �ɔC����B
---drawFaceGroupNodeContour(facename,name,colormap,comp=-1)
	��L�̃��\�b�h�ɂ��āAbodyId �̑���ɂ��ꂼ�� FaceGroup �̖��O��^����
	�R���^�[�\�����s���B
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
	bodyId �ŗ^������ Surface ��
	MeshDB �� displacement �Ƃ������O�Ŋi�[����Ă��镨����
	�i���[�h�FNodeVariable,�l�FVector3�j�ɑ΂��āAfactor �{�ŕό`�\������B
---deformFaceGroup(faceName,displacement,factor)
	faceName �ŗ^������ FaceGroup �� face ��
	MeshDB �� displacement �Ƃ������O�Ŋi�[����Ă��镨����
	�i���[�h�FNodeVariable,�l�FVector3�j�ɑ΂��āAfactor �{�ŕό`�\������B
---deformXYZFaceGroup(faceName,displacementX,displacementY,displacementZ,factor)
	faceName �ŗ^������ FaceGroup �� face ��
	MeshDB �� displacementX, displacementY,displacementZ �Ƃ������O�Ŋi�[����Ă���
	�����ʁi���[�h�FNodeVariable,�l�FScalar�j��(x,y,z)���W�̕ό`�ʂ��Ƃ���
	factor �{�ŕό`�\������B
=end
---------------------------------------------------------------------------*/
	void deformSurface(kmb::bodyIdType bodyId, const char* displacement, double factor);
	void deformFaceGroup(const char* faceName, const char* displacement, double factor);
	void deformXYZFaceGroup(const char* faceName, const char* displacementX, const char* displacementY, const char* displacementZ, double factor);
/**--------------------------------------------------------------------------
=begin
---deformSurfaceNodeContour(bodyId,displacement,factor,nodeValue,colorMap,comp=-1)
	bodyId �ŗ^������ Surface ��
	MeshDB �� displacement �Ƃ������O�Ŋi�[����Ă��镨����
	�i���[�h�FNodeVariable,�l�FVector3�j�ɑ΂��āAfactor �{�ŕό`�\������B
	MeshDB �� nodeValue �Ƃ������O�Ŋi�[����Ă��镨���ʁiNodeVariable ���[�h�j
	�ɑ΂��āAcolormap �Œ�`���ꂽ�F�Â����@�ɂ���āAcomp �����̃J���[�R���^�[�\�����s���B
---deformSurfaceContour(bodyId,displacement,factor,elementValue,colorMap,comp=-1)
	bodyId �ŗ^������ Surface ��
	MeshDB �� displacement �Ƃ������O�Ŋi�[����Ă��镨����
	�i���[�h�FNodeVariable,�l�FVector3�j�ɑ΂��āAfactor �{�ŕό`�\������B
	MeshDB �� elementValue �Ƃ������O�Ŋi�[����Ă��镨���ʁiElementVariable ���[�h�j
	�ɑ΂��āAcolormap �Œ�`���ꂽ�F�Â����@�ɂ���āAcomp �����̃J���[�R���^�[�\�����s���B
---deformFaceGroupNodeContour(faceName,displacement,factor,nodeValue,colorMap,comp=-1)
	faceName �ŗ^������ FaceGroup �� face ��
	MeshDB �� displacement �Ƃ������O�Ŋi�[����Ă��镨����
	�i���[�h�FNodeVariable,�l�FVector3�j�ɑ΂��āAfactor �{�ŕό`�\������B
	MeshDB �� nodeValue �Ƃ������O�Ŋi�[����Ă��镨���ʁiNodeVariable ���[�h�j
	�ɑ΂��āAcolormap �Œ�`���ꂽ�F�Â����@�ɂ���āAcomp �����̃J���[�R���^�[�\�����s���B
---deformXYZFaceGroupNodeContour(faceName,displacementX,displacementY,displacementZ,factor,nodeValue,colorMap,comp=-1)
	faceName �ŗ^������ FaceGroup �� face ��
	MeshDB �� displacementX, displacementY,displacementZ �Ƃ������O�Ŋi�[����Ă���
	�����ʁi���[�h�FNodeVariable,�l�FScalar�j��(x,y,z)���W�̕ό`�ʂ��Ƃ���
	factor �{�ŕό`�\������B
	MeshDB �� nodeValue �Ƃ������O�Ŋi�[����Ă��镨���ʁiNodeVariable ���[�h�j
	�ɑ΂��āAcolormap �Œ�`���ꂽ�F�Â����@�ɂ���āAcomp �����̃J���[�R���^�[�\�����s���B
=end
---------------------------------------------------------------------------*/
	void deformSurfaceNodeContour(kmb::bodyIdType bodyId, const char* displacement, double factor, const char* physicalValue, kmb::ColorMap* colorMap, int comp=-1);
	void deformSurfaceContour(kmb::bodyIdType bodyId, const char* displacement, double factor, const char* physicalValue, kmb::ColorMap* colorMap, int comp=-1);
	void deformFaceGroupNodeContour(const char* faceName, const char* displacement, double factor, const char* nodeValue, kmb::ColorMap* colorMap, int comp=-1);
	void deformXYZFaceGroupNodeContour(const char* faceName, const char* displacementX, const char* displacementY, const char* displacementZ, double factor, const char* physicalValue, kmb::ColorMap* colorMap);
/**--------------------------------------------------------------------------
=begin
---drawVector(vector,factor,colormap)
	vectorName �ŗ^������ NodeVariable ���[�h�� Vector3 �l�̕����ʂɑ΂��āA
	������ factor �{�̖��Ńx�N�g���\������B
	�F�� colorMap �ŗ^����B
---drawField(vector,size,colorMap)
	vectorName �ŗ^������ NodeVariable ���[�h�� Vector3 �l�̕����ʂɑ΂��āA
	���� size ���̖��Ńx�N�g���\������B
	�F�� colorMap �ŗ^����B
---drawSurfaceVector(bodyId,vector,factor,colormap)
	vectorName �ŗ^������ NodeVariable ���[�h�� Vector3 �l�̕����ʂɑ΂��āA
	bodyId �ŗ^����ꂽ�ʏ�̐ߓ_�ɂ��āA������ factor �{�̖��Ńx�N�g���\������B
	�F�� colorMap �ŗ^����B
---drawSurfaceField(bodyId,vector,size,colormap)
	vectorName �ŗ^������ NodeVariable ���[�h�� Vector3 �l�̕����ʂɑ΂��āA
	bodyId �ŗ^����ꂽ�ʏ�̐ߓ_�ɂ��āA������ size ���̖��Ńx�N�g���\������B
	�F�� colorMap �ŗ^����B
---drawFaceGroupVector(faceName,vectorName,factor,colorMap)
	vectorName �ŗ^������ NodeVariable ���[�h�� Vector3 �l�̕����ʂɑ΂��āA
	faceName �ŗ^����ꂽ�ʃO���[�v��̐ߓ_�ɂ��āA������ factor �{�̖��Ńx�N�g���\������B
	�F�� colorMap �ŗ^����B
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
	bodyId �� Body �̒��� valueName �ŗ^������ NodeVariable ���[�h�̕����ʂɑ΂��āA
	�l�� val �̓��l�ʂ�\������B���̃��\�b�h�ł͐F�� Material �͂��Ȃ��B
=end
---------------------------------------------------------------------------*/
	void drawIsosurface(kmb::bodyIdType bodyId, const char* nodeValue, double val);
/**--------------------------------------------------------------------------
=begin
---drawSection(bodyId,plane)
	bodyId �� Body �� plane �Ő؂����ؒf�ʂ�\������B
---drawSectionNodeContour(bodyId,plane,physicalValue,colorMap,comp=-1)
	�ߓ_�l�� physicalValue �̒l�� colorMap �ɏ]���R���^�[�\��������B
---drawSectionContour(bodyId,plane,physicalValue,colorMap,comp=-1)
	�v�f�l�� physicalValue �̒l�� colorMap �ɏ]���R���^�[�\��������B
	������ Tetrahedron �̂�
---drawSectionVector(bodyId,plane,vectorName,factor,colorMap)
	�ߓ_�l�� vectorName �̒l�� colorMap �ɏ]�� factor �{�̃x�N�g���\��������B
---drawSectionField(bodyId,plane,vectorName,size,colorMap)
	�ߓ_�l�� vectorName �̒l�� colorMap �ɏ]���A���� size ���̃x�N�g���\��������B
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
	�x�N�g���l�֐� vectorVal �� (x,y,z) ���� delta �Ԋu�� step ����������������
=end
---------------------------------------------------------------------------*/
	int drawTrajectory(const char* vectorVal,double x,double y,double z,double delta,int step, kmb::ColorMap* colorMap, double tolerance=0.0);

/**--------------------------------------------------------------------------
=begin
---drawIntersection(bodyId,plane)
	bodyId �� Body �� plane �ƌ�������v�f���������ʂ�\������

---drawIntersectionContour(bodyId,plane,physicalValue,colorMap,comp=-1);
	bodyId �� Body �� plane �ƌ�������v�f���������ʂ�
	physicalValue �̒l�ŃR���^�[�\������
=end
---------------------------------------------------------------------------*/
	void drawIntersection(kmb::bodyIdType bodyId,kmb::Plane* plane);
	void drawIntersectionContour(kmb::bodyIdType bodyId,kmb::Plane* plane,const char* physicalValue,kmb::ColorMap* colorMap,int comp=-1);

/**--------------------------------------------------------------------------
=begin
---drawMarking(condName,mode)
	MeshDB �� condName �Ŋi�[����Ă��鋫�E�����ɂ��� mode �̃}�[�J�[��
	�\������B
	���p�\�� mode �ɂ��Ă͈ȉ��̂Ƃ���
	 * VECTOR �ߓ_��ɖ��
	 * TENSOR �ߓ_��Ɍ�����
	 * DOT �ߓ_��ɓ_
	 * CROSS �ߓ_��ɏ\��
	 * CDOT �v�f�̒��S�ɓ_
	 * CCROSS �v�f�̒��S�ɏ\��
	 * EDGE �v�f�̕ӂ�\��
	 * DAGGER �ߓ_��ɏ\��
	 * CIRCLE �ߓ_��Ɋۈ�
---drawMarkingEdgeWithScale(bodyId,mode,x,y,z,scale)
	MeshDB �̂P�����v�f����Ȃ� Body �� bodyId �ŗ^���āA����ɑ΂��� mode ��
	�}�[�J�[��\������B
=end
---------------------------------------------------------------------------*/
	void drawMarking(const char* condName,const char* mode);
	void drawMarkingEdgeWithScale(kmb::bodyIdType bodyId,const char* mode,double x,double y,double z,double scale);
/**--------------------------------------------------------------------------
=begin
---setMarkSize(size)
	�}�[�J�[�̊�T�C�Y��ݒ肷��B
---getMarkSize
	�}�[�J�[�̊�T�C�Y���擾����B
---setNodeOffset(offset)
	ID �\���̎��̐ߓ_ ID �̃I�t�Z�b�g�l��ݒ肷��
---setElementOffset(offset)
	ID �\���̎��̗v�f ID �̃I�t�Z�b�g�l��ݒ肷��
=end
---------------------------------------------------------------------------*/
	void setMarkSize(double size);
	double getMarkSize(void);
	void setNodeOffset(int offset);
	void setElementOffset(int offset);

/**--------------------------------------------------------------------------
=begin
---getDeformFaceGroupBoundingBox(faceName,displacement,factor)
	deformFaceGroup �̓��������ŕ\�������ό`�}�� BoundingBox ��
	[[max_x,max_y,max_z],[min_x,min_y,min_z]] �`���ŕԂ��B
---getDeformXYZFaceGroupBoundingBox(faceName,displacementX,displacementY,displacementZ,factor)
	deformXYZFaceGroup �̓��������ŕ\�������ό`�}�� BoundingBox ��
	[[max_x,max_y,max_z],[min_x,min_y,min_z]] �`���ŕԂ��B
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
