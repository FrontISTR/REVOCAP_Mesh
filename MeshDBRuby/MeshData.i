/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : MeshData                                           #
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
/**--------------------------------------------------------------------
=begin
= RevocapMesh::MeshData

RevocapMesh::MeshData �N���X�̓��b�V���f�[�^�ɂ��ẴR���e�i�@�\�Ɋւ�����̂����������������N���X�ł���B
�h�n�X�N���v�g�͂��̃N���X�Ŏ�������Ă��郁�\�b�h�݂̂�p���Ď�������悤�ɂ���B
RevocapMesh::MeshDB �͂��̃N���X���p�����Ă���B
���̃��b�V���R���e�i�N���X�ɂ��Ă��A���̃N���X�Œ�`����ă��\�b�h����������ƁA�h�n�X�N���v�g�𗘗p���邱�Ƃ��ł���B

==���\�b�h�ꗗ

===�N���X���\�b�h

((<MeshData.new>))

===���b�V���Ǘ�

((<beginModel>))
((<endModel>))
((<clearModel>))

===���W�n

((<setCoordinate>))
((<translateCoordinate>))

===�ߓ_�Ǘ�

((<beginNode>))
((<addNode>))
((<addNodeWithId>))
((<endNode>))
((<getNodeCount>))
((<getNode>))
((<eachNode>))
((<eachNodeWithId>))
((<insertNode>))
((<insertNodeWithId>))
((<updateNode>))
((<getNodeContainerType>))

===�v�f�Ǘ�

((<beginElement>))
((<addElement>))
((<addElementWithId>))
((<endElement>))
((<insertElement>))
((<insertElementWithId>))
((<getElementCount>))
((<getElementCountByType>))
((<getDimension>))
((<getDegree>))
((<isUniqueElementType>))
((<getBodyName>))
((<setBodyName>))
((<getBodyIdByName>))
((<getElementContainerType>))
((<getElement>))
((<eachElement>))
((<eachElementWithId>))
((<getBodyCount>))
((<clearBody>))

===�f�[�^�Ǘ�

((<createData>))
((<getDataCount>))
((<hasData>))
((<renameData>))
((<deleteData>))
((<clearData>))
((<getDataMode>))
((<getDataValueType>))
((<getDataSpecType>))
((<setDataSpecType>))
((<getDataTargetBodyId>))
((<getDataContainerType>))
((<eachDataName>))
((<getIdCount>))
((<setDefaultSpecType>))
((<getValueAtId>))
((<setValueAtId>))
((<getValue>))
((<setValue>))
((<addId>))
((<hasId>))
((<deleteId>))
((<setTargetData>))
((<setMultiValuesAtId>))
((<eachId>))
((<eachIdWithValue>))
((<faceGroupToBody>))

=end
---------------------------------------------------------------*/

namespace kmb{

class MeshData
{
public:
/**--------------------------------------------------------------------
=begin
== �N���X���\�b�h
--- MeshData.new()
	MeshData �N���X�̃R���X�g���N�^�ł���BMeshData �I�u�W�F�N�g�̃C���X�^���X�𐶐�����B
=end
---------------------------------------------------------------------*/
	MeshData(void);
	virtual ~MeshData(void);

/**--------------------------------------------------------------------
=begin
==���b�V���Ǘ�

--- beginModel()
	���b�V���̓o�^���J�n����B
	beginNode addNode endNode beginElement addElement endElement
	���n�߂�O�ɌĂяo���B

--- endModel()
	���b�V���̓o�^���I������B
	* beginNode
	* addNode
	* endNode
	* beginElement
	* addElement
	* endElement
	�Ȃǂ̐ߓ_�E�v�f���A�����
	* createData
	* setValueAtId
	* setValue
	* addId
	�Ȃǃv���E�|�X�g�̂��߂̏���ǂݍ��񂾌�Ɏ��s����B
	�����ł͂��̃��\�b�h���Ă΂ꂽ���_�œ����f�[�^�̍œK�������s���B

=end
----------------------------------------------------------------------*/
	void beginModel(void);
	void endModel(void);

/**---------------------------------------------------------------------
=begin
--- clearModel()
	MeshDB �̂��ׂẴf�[�^���������A����������B
=end
------------------------------------------------------------------------*/
	void clearModel(void);

/**---------------------------------------------------------------------
=begin
--- setCoordinate(mtrx)
--- translateCoordinate(x,y,z)
	���W�n���w�肷��B

=end
------------------------------------------------------------------------*/
	void setCoordinate( const kmb::Matrix4x4& matrix );
	void translateCoordinate( double x, double y, double z );



/**----------------------------------------------------------------------
=begin
==�ߓ_�Ǘ�

MeshData �ł�3������ԓ��̓_�̍��W (x,y,z) ��ߓ_�Ƃ��ĊǗ�����B
MeshData �ł͐ߓ_�ɂ͂��ׂĐߓ_Id���t�^�����B

--- beginNode(size=0,containerType=nil)
	size �̐ߓ_�̓o�^���J�n����B
	id ���A���łȂ��ꍇ�� size �ɂ͋󂫔ԍ��̌����܂߂����̂��w�肷��B
	�Ⴆ�΁A�z��ł̎����Ȃ� 0 ���� size-1 �܂ł̐ߓ_Id���o�^�\�ɂȂ�B
	containerType �͖ړI�ɉ����Đߓ_�R���e�i��ύX����ꍇ�ɗp����B
	�f�t�H���g�ł͐ߓ_�R���e�i�Ƃ��Đߓ_ID���L�[�Ƃ����A�z�z���p����B

	containerType �̗�
	* �Q�����̐ߓ_�̘A�z�z�� "stl::map<id,Point2D*>"
	* �R���e�i�� stl::map ���g���i�f�t�H���g�j "stl::map<id,Point3D*>"
	* �R���e�i�� stl::vector ���g�� "stl::vector<Point3D*>"
	* OpenGL �� glDrawArray ����� glDrawElements �𗘗p�\�ȂR�����z�� "double_array"
=end
------------------------------------------------------------------------*/
	void beginNode(unsigned int size=0,const char* containerType=NULL);

/**----------------------------------------------------------------------
=begin
--- addNode(x,y,z)
--- addNodeWithId(x,y,z,id)
	addNode ���\�b�h�ł�
	���W (x,y,z) ��^���āA
	���̍��W�̐ߓ_��o�^���ĕt�^���ꂽ�ߓ_Id��Ԃ��B
	�ʏ�͌��ݓo�^����Ă���ߓ_�̍ő�ߓ_Id+1���t�^�����B
	addNodeWithId ���\�b�h�Őߓ_Id�����炩���ߗ^���ēo�^���邱�Ƃ��o����B
	���̏ꍇ�͊��ɓo�^����Ă���ꍇ�� -1 ��Ԃ��B
	�Q�����R���e�i�𗘗p���Ă��鎞�ɂ� z ���W�𖳎�����
=end
-------------------------------------------------------------------------*/
	kmb::nodeIdType addNode(double x,double y,double z);
	kmb::nodeIdType addNodeWithId(double x,double y,double z,kmb::nodeIdType id);

/**-----------------------------------------------------------------------
=begin
--- endNode()
	�ߓ_�̓o�^���I������B
=end
--------------------------------------------------------------------------*/
	void endNode(void);

/**----------------------------------------------------------------------
=begin
--- getNodeContainerType()
	�ߓ_�R���e�i�̖��O��Ԃ��B
=end
------------------------------------------------------------------------*/
	const char* getNodeContainerType(void);

/**------------------------------------------------------------------------
=begin
--- getNodeCount()
	MeshDB�ɓo�^����Ă���ߓ_�̌���Ԃ��B
	�ߓ_Id���s�A���̎��ɂ͂��̒l�Ɛߓ_Id�̍ő�l+1����v���Ă���Ƃ͌���Ȃ��B
=end
--------------------------------------------------------------------------*/
	size_t getNodeCount(void);
/**------------------------------------------------------------------------
=begin
--- getNode(nodeId)
	�ߓ_Id �� nodeId �̐ߓ_�̍��W�� [x,y,z] �`���� Array �ŕԂ��B
=end
--------------------------------------------------------------------------*/
	%extend{
		VALUE getNode(kmb::nodeIdType nodeId){
			VALUE ary = rb_ary_new();
			kmb::Node node;
			if( self->getNode(nodeId,node) ){
				rb_ary_store(ary,0,rb_float_new(node.x()));
				rb_ary_store(ary,1,rb_float_new(node.y()));
				rb_ary_store(ary,2,rb_float_new(node.z()));
				return ary;
			}
			return Qnil;
		}
	}
/**------------------------------------------------------------------------
=begin
--- eachNode
	�ߓ_�̃C�e���[�^�ł���B

	 mesh.eachNode{ |node|
	   x = node[0]
	   y = node[1]
	   z = node[2]
	 }

	�̂悤�Ɏg���B

--- eachNodeWithId
	�ߓ_�Ɛߓ_ID�̃C�e���[�^�ł���B

	 mesh.eachNodeWithId{ |node,id|
	   x = node[0]
	   y = node[1]
	   z = node[2]
	 }

	�̂悤�Ɏg���B
=end

�����Feach �n�̃��\�b�h�ł� yield �̑O�ɃC�e���[�^��i�߂Ă���
--------------------------------------------------------------------------*/
%extend{
	VALUE eachNode(void) const{
		if( self->getNodes() != NULL ){
			double x,y,z;
			kmb::Point3DContainer::const_iterator nIter = self->getNodes()->begin();
			while( !nIter.isFinished() )
			{
				if( nIter.getXYZ(x,y,z) ){
					VALUE ary = rb_ary_new();
					rb_ary_store(ary,0,rb_float_new(x));
					rb_ary_store(ary,1,rb_float_new(y));
					rb_ary_store(ary,2,rb_float_new(z));
					++nIter;
					rb_yield(ary);
				}else{
					++nIter;
				}
			}
		}
		return Qnil;
	}
	VALUE eachNodeWithId(void) const{
		if( self->getNodes() != NULL ){
			double x,y,z;
			kmb::Point3DContainer::const_iterator nIter = self->getNodes()->begin();
			while( !nIter.isFinished() )
			{
				const kmb::nodeIdType id = nIter.getId();
				if( nIter.getXYZ(x,y,z) ){
					VALUE ary = rb_ary_new();
					rb_ary_store(ary,0,rb_float_new(x));
					rb_ary_store(ary,1,rb_float_new(y));
					rb_ary_store(ary,2,rb_float_new(z));
					++nIter;
					rb_yield_values(2,ary,INT2FIX(id));
				}else{
					++nIter;
				}
			}
		}
		return Qnil;
	}
}
/**----------------------------------------------------------------------
=begin
--- insertNode(x,y,z)
--- insertNodeWithId(x,y,z,id)
	insertNode ���\�b�h�ł� endNode ���\�b�h��ǂ񂾌��
	���W (x,y,z) ��^���āA
	���̍��W�̐ߓ_��o�^���ĕt�^���ꂽ�ߓ_ID��Ԃ��B
	�ʏ�͌��ݓo�^����Ă���ߓ_�̍ő�ߓ_ID+1���t�^�����B
	insertNodeWithId ���\�b�h�Őߓ_ID�����炩���ߗ^���ēo�^���邱�Ƃ��o����B
	���̏ꍇ�͊��ɓo�^����Ă���ꍇ�� -1 ��Ԃ��B
=end
-------------------------------------------------------------------------*/
	kmb::nodeIdType insertNode(double x,double y,double z);
	kmb::nodeIdType insertNodeWithId(double x,double y,double z,kmb::nodeIdType id);

/**----------------------------------------------------------------------
=begin
--- updateNode(x,y,z,id)
	���łɓo�^����Ă���ߓ_�̍��W���X�V����
=end
-------------------------------------------------------------------------*/
	bool updateNode(double x,double y,double z, kmb::nodeIdType id);

/**--------------------------------------------------------------------
=begin
==�v�f�Ǘ�

MeshData �ł͗v�f�͗v�f�^�C�v�Ɛߓ_�z��ŊǗ�����B
�v�f�ɂ͂��ׂėv�fId���t�^�����B

MeshData �ň������Ƃ̂ł���v�f�^�C�v�͈ȉ��̂Ƃ���ł���B
�ȉ��̃��\�b�h�ŗv�f�^�C�v�������ɗ^����ꍇ�͂��������ׂđ啶���̕�����܂��� getElementTypeMap ��
�����鐮���l��^����B���ꂼ��̗v�f�̐ߓ_�z��̃R�l�N�e�B�r�e�B�ɂ��Ă͗v�f���C�u�����̍��Ő�������B

* SEGMENT       (����)
* SEGMENT2      (2���̐���)
* TRIANGLE      (�O�p�`)
* TRIANGLE2     (2���̎O�p�`)
* QUAD          (�l�p�`)
* QUAD2         (2���̎l�p�`)
* TETRAHEDRON   (�l�ʑ�)
* TETRAHEDRON2  (2���̎l�ʑ�)
* WEDGE         (�O�p��(���`�A�v���Y��))
* WEDGE2        (2���̎O�p��)
* PYRAMID       (�l�p��(�s���~�b�h))
* PYRAMID2      (2���̎l�p��)
* HEXAHEDRON    (�Z�ʑ�)
* HEXAHEDRON2   (2���̘Z�ʑ�)

�v�f�̏W�܂�͕����Ǘ����邱�Ƃ��ł���BMeshDB �ł͗v�f�̏W�܂�i�v�f�O���[�v�j�̂��Ƃ�
Body �ƌĂԁB�ȉ��� beginElement ���� endElement �܂ł̊Ԃ� addElement ��
�o�^���ꂽ�v�f�̏W����1�� Body �ɂȂ�B������ Body �����ʂ��邽�߂� BodyID ��
���ꂼ��� Body �ɗ^�����Ă���BBodyId �� beginElement �̖߂�l�ŗ^�����鑼�A
�܂��͕\�ʒ��o��̈敪���A���E�w�����Ȃǂœ�����B

--- beginElement(size=0)
	size �̗v�f�̓o�^���J�n����B�v�f�O���[�v(=Body)��ID��Ԃ��B
	id ���A���łȂ��ꍇ�� size �ɂ͋󂫔ԍ��̌����܂߂����̂��w�肷��B
	containerType �͖ړI�ɉ����ėv�f�R���e�i��ύX����ꍇ�ɗp����B
	�f�t�H���g�ł͗v�f�R���e�i�Ƃ��Đߓ_ID���L�[�Ƃ����A�z�z���p����B

	containerType �̗�
	* �R���e�i�� stl::map ���g���i�f�t�H���g�j "stl::map"
	* �|�C���^�̔z����g�� "element_array"
	* OpenGL �� glDrawElements �ŗ��p�\�ȎO�p�`�z����g�� "triangle_array"
=end
-----------------------------------------------------------------------*/
	kmb::bodyIdType beginElement(unsigned int size=0,const char* containerType=NULL);

/**---------------------------------------------------------------------
=begin
--- addElement(eType,ary)
--- addElementWithId(eType,ary,id)
	�v�f�^�C�v�������� eType �ŁA
	�ߓ_�z�� Array �^�� ary �ŗ^����ꂽ�v�f��o�^����
	�t�^���ꂽ�v�fID��Ԃ��B
	addElementWithId ��p����Ηv�f Id ���w�肵�ēo�^���邱�Ƃ��ł���B
	���ɂ��� Id ���g���Ă���ꍇ�͉������Ȃ��B
=end
------------------------------------------------------------------------*/
%extend{
	kmb::elementIdType addElement(VALUE eType,VALUE ary){
		kmb::elementType type = kmb::UNKNOWNTYPE;
		kmb::elementIdType elemId = kmb::Element::nullElementId;
		if( TYPE(eType) == T_STRING ){
			type = kmb::ElementBase::getType(StringValuePtr(eType));
		}else{
			type = static_cast<kmb::elementType>(FIX2INT(eType));
		}
		Check_Type(ary, T_ARRAY);
		const int len = kmb::Element::getNodeCount(type);
		if( len > 0 ){
			kmb::nodeIdType a[20];
			for(int i=0; i<len; ++i){
				a[i] = FIX2INT(rb_ary_entry(ary,i));
			}
			elemId = self->addElement(type,a);
		}
		return elemId;
	}
	kmb::elementIdType addElementWithId(VALUE eType,VALUE ary,VALUE id){
		kmb::elementType type = kmb::UNKNOWNTYPE;
		kmb::elementIdType elemId = kmb::Element::nullElementId;
		if( TYPE(eType) == T_STRING ){
			type = kmb::ElementBase::getType(StringValuePtr(eType));
		}else{
			type = static_cast<kmb::elementType>(FIX2INT(eType));
		}
		Check_Type(ary, T_ARRAY);
		const int len = kmb::Element::getNodeCount(type);
		if( len > 0 ){
			kmb::nodeIdType a[20];
			for(int i=0; i<len; ++i){
				a[i] = FIX2INT(rb_ary_entry(ary,i));
			}
			elemId = self->addElementWithId(type,a,FIX2INT(id));
		}
		return elemId;
	}
}

/**---------------------------------------------------------------------------
=begin
--- endElement(void)
    �v�f�̓o�^���I������B
=end
----------------------------------------------------------------------------*/
	void endElement(void);

/**--------------------------------------------------------------------------
=begin
--- insertElement(bodyId,eType,ary)
	endElement ������� bodyId �� Body ��
	addElement �Ɠ����`���ŗv�f��o�^����B
=end
----------------------------------------------------------------------------*/
%extend{
	kmb::elementIdType insertElement(kmb::bodyIdType bodyId,VALUE eType,VALUE ary){
		kmb::elementType type = kmb::UNKNOWNTYPE;
		if( TYPE(eType) == T_STRING ){
			type = kmb::ElementBase::getType(StringValuePtr(eType));
		}else{
			type = static_cast<kmb::elementType>(FIX2INT(eType));
		}
		Check_Type(ary, T_ARRAY);
		const int len = kmb::Element::getNodeCount(type);
		if( len > 0 ){
			kmb::nodeIdType a[20];
			for(int i=0; i<len; ++i){
				a[i] = FIX2INT(rb_ary_entry(ary,i));
			}
			return self->insertElement(bodyId,type,a);
		}
		return kmb::Element::nullElementId;
	}
}

/**--------------------------------------------------------------------------
=begin
--- insertElementWithId(bodyId,eType,ary,id)
	endElement ������� bodyId �� Body ��
	addElement �Ɠ����`���ŗv�f��o�^����B
=end
----------------------------------------------------------------------------*/
%extend{
	kmb::elementIdType insertElementWithId(kmb::bodyIdType bodyId,VALUE eType,VALUE ary,kmb::elementIdType elementId){
		kmb::elementType type = kmb::UNKNOWNTYPE;
		if( TYPE(eType) == T_STRING ){
			type = kmb::ElementBase::getType(StringValuePtr(eType));
		}else{
			type = static_cast<kmb::elementType>(FIX2INT(eType));
		}
		Check_Type(ary, T_ARRAY);
		const int len = kmb::Element::getNodeCount(type);
		if( len > 0 ){
			kmb::nodeIdType a[20];
			for(int i=0; i<len; ++i){
				a[i] = FIX2INT(rb_ary_entry(ary,i));
			}
			return self->insertElementWithId(bodyId,type,a,elementId);
		}
		return kmb::Element::nullElementId;
	}
}

/**--------------------------------------------------------------------------
=begin
--- getElementCount(bodyId)
	�^����ꂽ Body �Ɋ܂܂��v�f�̌���Ԃ��B
--- getElementCount()
	���ׂĂ� Body �̗v�f�̌��̍��v��Ԃ��B
--- getElementCountByType(bodyId)
	�^����ꂽ Body �Ɋ܂܂��v�f�̌����^�ʂ� Hash �ŕԂ��B
=end
---------------------------------------------------------------------------*/
	int getElementCount(kmb::bodyIdType bodyId) const;
	int getElementCount(void) const;
%extend{
	VALUE getElementCountByType(kmb::bodyIdType bodyId){
		VALUE hash = rb_hash_new();
		kmb::ElementContainer* body = self->getBodyPtr( bodyId );
		if( body != NULL ){
			for(int i = 0; i < kmb::ELEMENT_TYPE_NUM; ++i)
			{
				kmb::elementType type = static_cast<kmb::elementType>(i);
				std::string	str = kmb::ElementBase::getTypeString(type);
				int count = static_cast<int>( body->getCountByType( type ) );
				if( count > 0 ){
					rb_hash_aset( hash, rb_str_new2( str.c_str() ), INT2NUM(count) );
				}
			}
		}
		return hash;
	}
}

/**--------------------------------------------------------------------------
=begin
--- getDimension(bodyId)
--- getDimension()
	�v�f�O���[�v�̎����i�\�ʃ��b�V���Ȃ�2�A�̐σ��b�V���Ȃ�3�j
=end
---------------------------------------------------------------------------*/
	int getDimension(kmb::bodyIdType bodyId) const;
	int getDimension(void) const;

/**--------------------------------------------------------------------------
=begin
--- getDegree(bodyId)
	�v�f�O���[�v�̎����i2���v�f�Ȃ�2�j
=end
---------------------------------------------------------------------------*/
	int getDegree(kmb::bodyIdType bodyId) const;

/**--------------------------------------------------------------------------
=begin
--- isUniqueElementType(bodyId,etype)
	�v�f�̌^��\������������āA�v�f�O���[�v�����̗v�f�̌^�݂̂���
	�Ȃ邩�ǂ����𔻒f����B
=end
---------------------------------------------------------------------------*/
%extend{
	bool isUniqueElementType(kmb::bodyIdType bodyId,const char* etype) const{
		return self->isUniqueElementType( bodyId, kmb::ElementBase::getType(etype) );
	}
}

/**--------------------------------------------------------------------------
=begin
--- getBodyName(bodyId)
	�v�f�O���[�v�̖��O�̎擾
--- setBodyName(bodyId,name)
	�v�f�O���[�v�̖��O�̐ݒ�
--- getBodyIdByName(name)
	�v�f�O���[�v������ id �����߂�
	�Ȃ���� -a
=end
---------------------------------------------------------------------------*/
	const char* getBodyName(bodyIdType bodyId) const;
	void setBodyName(bodyIdType bodyId,const char* name);
	kmb::bodyIdType getBodyIdByName(const char* name) const;

/**--------------------------------------------------------------------------
=begin
--- getElementContainerType(bodyId)
	�v�f�O���[�v�̃R���e�i�^�C�v��
=end
---------------------------------------------------------------------------*/
	const char* getElementContainerType(bodyIdType bodyId) const;

/**-------------------------------------------------------------------------
=begin
--- getElement(elementId,bodyId=-1)
	bodyId �� Body �Ɋ܂܂��v�fID elementId
	�̗v�f�����o���B�߂�l�� Array �ő�ꐬ����
	�^�̕�����A��2�����ȉ��ɗv�f�̒��_�z�񂪒ǉ�����Ă���B
	bodyId ��^���Ȃ��Ă����o�����Ƃ͂ł��邪�A�����̂��ߗ]���Ȏ��Ԃ�������B

	�߂�l�̗�F
	["TETRAHEDRON",100,103,106,110]
=end
---------------------------------------------------------------------------*/
%extend{
	VALUE getElement(kmb::elementIdType elementId,kmb::bodyIdType bodyId=kmb::Body::nullBodyId){
		kmb::ElementContainer::const_iterator element = self->findElement(elementId,bodyId);
		if( !element.isFinished() ){
			VALUE ary = rb_ary_new();
			rb_ary_store(ary,0,rb_str_new2(element.getTypeString().c_str()));
			const int len = element.getNodeCount();
			for(int i=0; i<len; ++i){
				rb_ary_store(ary,i+1,INT2FIX(element.getCellId(i)));
			}
			return ary;
		}
		return Qnil;
	}
}

/**---------------------------------------------------------------------------
=begin
--- eachElement(bodyID)
	bodyID �� Body �̗v�f�̃C�e���[�^��^����B
	�C�e���[�^�̓����̃u���b�N�ł́A�v�f�� getElement �Ɠ����`���œ�����B

--- eachElementWithId(bodyID)
	bodyID �� Body �̗v�f�Ɨv�fID�̃C�e���[�^��^����B

=end
-----------------------------------------------------------------------------*/
%extend{
	VALUE eachElement(kmb::bodyIdType bodyID){
		const kmb::ElementContainer* body = self->getBodyPtr(bodyID);
		if( body ){
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( eIter != body->end() )
			{
				VALUE ary = rb_ary_new();
				rb_ary_store(ary,0,rb_str_new2(eIter.getTypeString().c_str()));
				const int len = eIter.getNodeCount();
				for(int i=0; i<len; ++i){
					rb_ary_store(ary,i+1,INT2FIX(eIter.getCellId(i)));
				}
				++eIter;
				rb_yield(ary);
			}
		}
		return Qnil;
	}
	VALUE eachElementWithId(kmb::bodyIdType bodyID){
		const kmb::ElementContainer* body = self->getBodyPtr(bodyID);
		if( body ){
			kmb::ElementContainer::const_iterator eIter = body->begin();
			while( eIter != body->end() )
			{
				VALUE ary = rb_ary_new();
				kmb::elementIdType elementID = eIter.getId();
				rb_ary_store(ary,0,rb_str_new2(eIter.getTypeString().c_str()));
				const int len = eIter.getNodeCount();
				for(int i=0; i<len; ++i){
					rb_ary_store(ary,i+1,INT2FIX(eIter.getCellId(i)));
				}
				++eIter;
				rb_yield_values(2,ary,INT2FIX(elementID));
			}
		}
		return Qnil;
	}
}

/**---------------------------------------------------------------------
=begin
--- getBodyCount()
	�o�^����Ă��� Body �̌���Ԃ��B
=end
------------------------------------------------------------------------*/
	kmb::bodyIdType getBodyCount(void) const;

/**-------------------------------------------------------------------------
=begin
--- clearBody(bodyId)
	bodyId �� Body �Ɋ܂܂�Ă���v�f�����ׂč폜����B
	�v�f���폜���Ă��ߖT�v�f�̃L���b�V���܂ł̓N���A���Ȃ��B
=end
---------------------------------------------------------------------------*/
	void clearBody(kmb::bodyIdType bodyId);


/**--------------------------------------------------------------------------
=begin
==�f�[�^�Ǘ�

���b�V���ɕt������v�������܂��̓|�X�g�����̂��߂̃f�[�^�̊Ǘ����s���B
�f�[�^�̎�ނ̓��[�h�ƌ^�ŕ��ނ����B
�\���o�|�����ʂ��邽�߂̃^�O���w��ł���B

���[�h�͈ȉ��̕�����ŗ^����
: "NODEGROUP"
  �ߓ_�O���[�v�i�ߓ_ID�̏W�����`���A�����ʂ͐ߓ_�O���[�v�S�̂ɗ^����j
: "ELEMENTGROUP"
  �v�f�O���[�v�i�v�fID�̏W�����`���A�����ʂ͗v�f�O���[�v�S�̂ɗ^����j
: "FACEGROUP"
  �v�f�̕\�ʃO���[�v�iFaceID�̏W�����`���A�����ʂ͗v�f�\�ʃO���[�v�S�̂ɗ^����j
: "BODYGROUP"
  �{�f�B�O���[�v�iBodyID�̏W�����`���A�����ʂ̓{�f�B�O���[�v�S�̂ɗ^����j
: "NODEVARIABLE"
  �ߓ_��̕����ʂ��`����i�ߓ_ID�ƕ����ʂ̃y�A��o�^����j
: "ELEMENTVARIABLE"
  �v�f��̕����ʂ��`����i�v�fID�ƕ����ʂ̃y�A��o�^����j
: "FACEVARIABLE"
  �v�f�̖ʏ�ɕ����ʂ��`����iFaceID�ƕ����ʂ̃y�A��o�^����j
: "BODYVARIABLE"
  �̈悲�Ƃɕ����ʂ��`����iBodyID�ƕ����ʂ̃y�A��o�^����j
: "GLOBAL"
  �O���[�v�� ID ��o�^�����ɁA�O���[�o���ȕ����ʂ�ݒ肷�邽�߂̃��[�h
: "UNKNWON"
  ��L�ȊO�̃��[�h�i�_���I�ɂ��̃��[�h�ɂȂ邱�Ƃ͖����B�f�o�b�O�p�B�j

�����ʂ̌^�͈ȉ��̕�����ŗ^����
: "INTEGER"
  �����l
: "SCALAR"
  �X�J���[�l
: "VECTOR2"
  2�����̃x�N�g���l
: "VECTOR3"
  3�����̃x�N�g���l
: "VECTOR4"
  4�����̃x�N�g���l
: "POINT3VECTOR3"
  3�����̎n�_�ƃx�N�g���̑g [[x,y,z],[vx,vy,vz]] �̌`��
: "TENSOR6"
  ���R�x6�̃e���\���l�i3�����̑Ώ̃e���\���j6�����̔z��ŗ^����
: "STRING"
  ������
: "ARRAY"
  �����ʂ̔z��
: "HASH"
  ��������L�[�Ƃ��A�����ʂ�l�Ƃ���A�z�z��
: "NONE"
  �����ʂȂ�

�Ⴆ�� BodyID = 1,2,3 �̃{�f�B�̑S�̂ɕ����� "pressure" �̃X�J���[�l 10.0 ��^����ɂ�

 mesh.createData("pressure","BODYGROUP","SCALAR")
 mesh.addId("pressure",1)
 mesh.addId("pressure",2)
 mesh.addId("pressure",3)
 mesh.setValue("pressure",10.0)

�̂悤�ɂ���B

ID�ƕ����ʂ̃y�A��^����ꍇ�́AaddId �� setValue �̑����
setValueAtId ���g���B

 mesh.createData("displace","NODEVARIABLE","VECTOR3")
 mesh.setValueAtId("displace", 0, [0.0, 0.1, 0.2])
 mesh.setValueAtId("displace", 1, [0.2, 0.1, 0.1])
 mesh.setValueAtId("displace", 2, [0.4, 0.0, 0.0])
 mesh.setValueAtId("displace", 3, [0.6, 0.0, 0.1])

�܂��́A�����̕����ʂ𓯎��ɗ^���������ɂ�

 mesh.createData("displace","NODEVARIABLE","VECTOR3")
 mesh.createData("mises","NODEVARIABLE","SCALAR")
 mesh.setTargetData(["displace","mises"])
 mesh.setMultiValuesAtId(0, [0.0, 0.1, 0.2, 0.01])
 mesh.setMultiValuesAtId(1, [0.2, 0.1, 0.1, 0.02])
 mesh.setMultiValuesAtId(2, [0.4, 0.0, 0.0, 0.03])
 mesh.setMultiValuesAtId(3, [0.6, 0.0, 0.1, 0.04])

�̂悤�ɂ���B

Global�ɕ����ʂ�^���鎞��

 mesh.createData("absTemp","GLOBAL","SCALAR")
 mesh.setValue("absTemp",-273.15)

�̂悤�ɂ���B

mode �� GROUP �n�Ŏw�肵������ setValueAtId �͉������Ȃ��B
mode �� VARIABLE �n�Ŏw�肵������ addId ����� setValue �͉������Ȃ��B
mode �� Global �Ɏw�肵������ addId ����� setValueAtId �͉������Ȃ��B

FaceGroup �� FaceVariable �͗v�f ID �Ƃ��̗v�f�� FaceID �̑g�ɑ΂��āA
�����ʂ�ݒ肷�邽�߂̃��[�h�ł���B
�Ⴆ�� "wall" �Ƃ��� FaceGroup �Ɋi�[����Ă��� FACE ���擾����ɂ�
���̂悤�ɂ���B

 mesh.eachId( "wall" ){ |faceId|
   # faceId = [elementId,localId]  ex, [1001,2] �̂悤�Ȍ`��
   element = mesh.faceToElement(faceId)
   # element = ['TRIANGLE',0,1,2] �̂悤�Ȍ`��
 }

stype �͉�̓R�[�h�����E�����̕��ނ�����ꍇ�Ȃǂ̂��߂ɁA
�f�[�^�ɗ^���邱�Ƃ̂ł��镶����̃^�O�ł���B���̕�������L�[�Ƃ���
Name( stype ) ������ƁA�^����ꂽ stype �Ɉ�v����
�f�[�^�̖��O�����̃C�e���[�^��������B

 mesh.createData("Const_1","NODEGROUP","ARRAY","Constraint")
 mesh.createData("wall","FACEGROUP","SCALAR","Boundary")

�Ȃǂ̂悤�ɂ��Ďg���B

stype ���قȂ�ꍇ�́A���O���d�����Ă��Ă��悢�B

--- createData(name,bmode,vtype,stype,bodyId)
	���[�h�� bmode �ŕ����ʂ̌^�� vtype �ŗ^������f�[�^ name �𐶐�����B
	stype bodyId �͏ȗ��\
	���łɑ��݂��Ă���ꍇ�͉������Ȃ�

=end
---------------------------------------------------------------------------*/

%extend{
	VALUE createData(const char* name,const char* bmode,const char* vtype,const char* stype="",int bodyId=kmb::Body::nullBodyId){
		kmb::DataBindings::bindingMode
			m = kmb::DataBindings::string2bindingMode(bmode);
		kmb::PhysicalValue::valueType
			v = kmb::PhysicalValue::string2valueType(vtype);
		self->createDataBindings(name,m,v,stype,bodyId);
		return Qnil;
	}
}

/**--------------------------------------------------------------------------
=begin
--- getDataCount()
--- getDataCount(specType)
	�o�^����Ă���f�[�^�̖��O�̌���Ԃ��BspecType ��^�������͂��� specType �ɓo�^����Ă���
	�f�[�^�̖��O�̌���Ԃ��B
=end
----------------------------------------------------------------------------*/
	size_t getDataCount(const char* stype=NULL);

/**--------------------------------------------------------------------------
=begin
--- hasData(name,stype=nil)
	�f�[�^ name �� spec type �� stype �̂��̂��f�[�^�x�[�X�ɓo�^����Ă��邩�ǂ�����������B
	stype=nil �̏ꍇ�� name �����Ō�������B
=end
----------------------------------------------------------------------------*/
	bool hasData(const char* name,const char* stype=NULL);

/**--------------------------------------------------------------------------
=begin
--- renameData(oldname,newname,stype=nil)
	�f�[�^ oldname �� newname �ɖ��̕ύX����B
	oldname ���o�^����Ă��āAnewname ���o�^����Ă��Ȃ����������s�����
	�߂�l�� true / false
=end
----------------------------------------------------------------------------*/
	bool renameData(const char* oldname,const char* newname,const char* stype=NULL);

/**--------------------------------------------------------------------------
=begin
--- deleteData(name,stype=nil)
	�f�[�^ name ���f�[�^�x�[�X����폜����
=end
----------------------------------------------------------------------------*/
	bool deleteData(const char* name,const char* stype=NULL);

/**--------------------------------------------------------------------------
=begin
--- clearData(name,stype=nil)
	�f�[�^ name �̃f�[�^�x�[�X����ɂ���B
	�f�[�^�x�[�X�̃R���e�i���폜����킯�ł͂Ȃ��B
=end
----------------------------------------------------------------------------*/
	bool clearData(const char* name,const char* stype=NULL);

/**--------------------------------------------------------------------------
=begin
--- getDataMode(name,stype=nil);
	�f�[�^ name �̃��[�h�𕶎���Ŏ擾����
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE getDataMode(const char* name,const char* stype=NULL){
		std::string str = kmb::DataBindings::bindingMode2string( self->getDataMode( name, stype ) );
		return rb_str_new2( str.c_str() );
	}
}

/**--------------------------------------------------------------------------
=begin
--- getDataValueType(name,stype=nil)
	�f�[�^ name �̕����ʂ̌^�𕶎���Ŏ擾����
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE getDataValueType(const char* name,const char* stype=NULL){
		std::string str = kmb::PhysicalValue::valueType2string( self->getDataValueType( name, stype ) );
		return rb_str_new2( str.c_str() );
	}
}

/**--------------------------------------------------------------------------
=begin
--- getDataSpecType(name)
	�f�[�^ name �̕����ʂ̎�ނ𕶎���Ŏ擾����
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE getDataSpecType(const char* name){
		std::string str = self->getDataSpecType( name );
		return rb_str_new2( str.c_str() );
	}
}

/**--------------------------------------------------------------------------
=begin
--- setDataSpecType(name,oldstype,newstype)
	Spec Type ��ݒ肷��
=end
----------------------------------------------------------------------------*/
	bool setDataSpecType(const char* name,const char* oldstype,const char* newstype);

/**--------------------------------------------------------------------------
=begin
--- getDataContainerType(name,stype)
	�R���e�i�^�C�v���擾����
=end
----------------------------------------------------------------------------*/
	const char* getDataContainerType(const char* name,const char* stype=NULL) const;

/**--------------------------------------------------------------------------
=begin
--- getDataTargetBodyId(name,stype=nil)
	�f�[�^ name �̑ΏۂƂ��� BodyId ��Ԃ�
=end
----------------------------------------------------------------------------*/
%extend{
	kmb::bodyIdType getDataTargetBodyId(const char* name,const char* stype=NULL){
		kmb::DataBindings* data = self->getDataBindingsPtr(name);
		if( data != NULL ){
			return data->getTargetBodyId();
		}else{
			return kmb::Body::nullBodyId;
		}
	}
}

/**------------------------------------------------------------------------
=begin
--- eachDataName
--- eachDataName(specType)
	�o�^����Ă��� Data (������) �̖��O���o�͂��� iterator ��^����B
=end
-------------------------------------------------------------------------*/
%extend{
	void eachDataName(void){
		const std::multimap< std::string, kmb::DataBindings* > mapper = self->getDataBindingsMap();
		std::multimap< std::string, kmb::DataBindings*>::const_iterator dIter = mapper.begin();
		while( dIter != mapper.end() ){
			std::string name = dIter->first;
			++dIter;
			rb_yield( rb_str_new2( name.c_str() ) );
		}
	}
	void eachDataName(const char* stype){
		const std::multimap< std::string, kmb::DataBindings* > mapper = self->getDataBindingsMap();
		std::multimap< std::string, kmb::DataBindings*>::const_iterator dIter = mapper.begin();
		while( dIter != mapper.end() ){
			kmb::DataBindings* binding = dIter->second;
			if( binding != NULL &&
				binding->getSpecType().compare(stype) == 0 )
			{
				std::string name = dIter->first;
				++dIter;
				rb_yield( rb_str_new2( name.c_str() ) );
			}else{
				++dIter;
			}
		}
	}
}

/**-------------------------------------------------------------------------
=begin
--- getIdCount(name,stype)
	name �̃O���[�v�ɓo�^���ꂽ id �̌����o�͂���
	NodeGroup / NodeVariable �Ȃ璸�_�̌��A
	ElementGroup / ElementVariable �Ȃ�v�f�̌�
	BodyGroup / BodyVariable �Ȃ痧�̂̌�
	FaceGroup / FaceVariable �Ȃ� FACE �̌�
	����ȊO�� 0 ��Ԃ�
=end
---------------------------------------------------------------------------*/
	int getIdCount(const char* name,const char* stype=NULL);

/**--------------------------------------------------------------------------
=begin
--- setDefaultSpecType(specType)
	�ȉ��̃��\�b�h�Q�ł� specType �̈����� nil �ł��肩�A
	���̃��\�b�h�� default �l���ݒ肳��Ă���Ƃ��ɂ́A�����ŗ^����ꂽ specType �̒�����T���B
=end
----------------------------------------------------------------------------*/
	void setDefaultSpecType(const char* specType);

/**---------------------------------------------------------------------------
=begin
--- getValueAtId(name,id,stype=nil)
	�O���[�v name �� id �ɒ�`���ꂽ�����ʂ��擾����BXXXVARIABLE ���[�h�̎������L���B
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE getValueAtId(const char* name,VALUE id,const char* stype=NULL){
		kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
		if( data ){
			switch( data->getValueType() )
			{
			case kmb::PhysicalValue::Scalar:
				{
					double v = 0.0;
					if( TYPE(id) == T_ARRAY ){
						int id0 = NUM2INT(rb_ary_entry(id,0));
						int id1 = NUM2INT(rb_ary_entry(id,1));
						if( data->getPhysicalValue(kmb::Face(id0,id1),&v) ){
							return rb_float_new(v);
						}
					}else{
						if( data->getPhysicalValue(FIX2INT(id),&v) ){
							return rb_float_new(v);
						}
					}
					break;
				}
			case kmb::PhysicalValue::Integer:
				{
					long l;
					if( TYPE(id) == T_ARRAY ){
						int id0 = NUM2INT(rb_ary_entry(id,0));
						int id1 = NUM2INT(rb_ary_entry(id,1));
						if( data->getPhysicalValue(kmb::Face(id0,id1),&l) ){
							return INT2FIX(l);
						}
					}else{
						if( data->getPhysicalValue(FIX2INT(id),&l) ){
							return INT2FIX(l);
						}
					}
					break;
				}
			case kmb::PhysicalValue::Vector2:
				{
					double v[2] = {0.0, 0.0};
					if( TYPE(id) == T_ARRAY ){
						int id0 = NUM2INT(rb_ary_entry(id,0));
						int id1 = NUM2INT(rb_ary_entry(id,1));
						if( data->getPhysicalValue(kmb::Face(id0,id1),v) ){
							return toVALUE( v[0], v[1] );
						}
					}else{
						if( data->getPhysicalValue(FIX2INT(id),v) ){
							return toVALUE( v[0], v[1] );
						}
					}
					break;
				}
			case kmb::PhysicalValue::Vector3:
				{
					double v[3] = {0.0, 0.0, 0.0};
					if( TYPE(id) == T_ARRAY ){
						int id0 = NUM2INT(rb_ary_entry(id,0));
						int id1 = NUM2INT(rb_ary_entry(id,1));
						if( data->getPhysicalValue(kmb::Face(id0,id1),v) ){
							return toVALUE( v[0], v[1], v[2] );
						}
					}else{
						if( data->getPhysicalValue(FIX2INT(id),v) ){
							return toVALUE( v[0], v[1], v[2] );
						}
					}
					break;
				}
			case kmb::PhysicalValue::Vector4:
				{
					double v[4] = {0.0, 0.0, 0.0, 0.0};
					if( TYPE(id) == T_ARRAY ){
						int id0 = NUM2INT(rb_ary_entry(id,0));
						int id1 = NUM2INT(rb_ary_entry(id,1));
						if( data->getPhysicalValue(kmb::Face(id0,id1),v) ){
							return toVALUE( v[0], v[1], v[2], v[3] );
						}
					}else{
						if( data->getPhysicalValue(FIX2INT(id),v) ){
							return toVALUE( v[0], v[1], v[2], v[3] );
						}
					}
					break;
				}
			case kmb::PhysicalValue::Vector2withInt:
				{
					double v[2] = {0.0, 0.0};
					long ival = 0;
					if( TYPE(id) == T_ARRAY ){
						int id0 = NUM2INT(rb_ary_entry(id,0));
						int id1 = NUM2INT(rb_ary_entry(id,1));
						kmb::Face f(id0,id1);
						if( data->getPhysicalValue(f,v) && data->getPhysicalValue(f,&ival) ){
							return toVALUE( v[0], v[1], ival );
						}
					}else{
						kmb::idType i = FIX2INT(id);
						if( data->getPhysicalValue(i,v) && data->getPhysicalValue(i,&ival) ){
							return toVALUE( v[0], v[1], ival );
						}
					}
					break;
				}
			default:
				{
					kmb::PhysicalValue* val = NULL;
					if( TYPE(id) == T_ARRAY ){
						int id0 = NUM2INT(rb_ary_entry(id,0));
						int id1 = NUM2INT(rb_ary_entry(id,1));
						val = data->getPhysicalValue(kmb::Face(id0,id1));
					}else{
						val = data->getPhysicalValue(FIX2INT(id));
					}
					if( val ){
						return toVALUE( val );
					}
					break;
				}
			}
		}
		return Qnil;
	}
}

/**--------------------------------------------------------------------------
=begin
--- setValueAtId(name,id,v,stype=nil)
	�O���[�v name �� id �ɕ����� v ��^����BXXXVARIABLE ���[�h�̎������L���B
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE setValueAtId(const char* name,VALUE id,VALUE v,const char* stype=NULL){
		kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
		kmb::PhysicalValue* val = createFromVALUE(v);
		if( val != NULL && data != NULL ){
			if( val->getType() == data->getValueType() ){
				if( TYPE(id) == T_ARRAY ){
					int id0 = NUM2INT(rb_ary_entry(id,0));
					int id1 = NUM2INT(rb_ary_entry(id,1));
					data->setPhysicalValue(kmb::Face(id0,id1),val);
				}else{
					data->setPhysicalValue(FIX2INT(id),val);
				}
			}else{
				delete val;
			}
		}
		return Qnil;
	}
}

/**---------------------------------------------------------------------------
=begin
--- getValue(name,stype=nil)
	�O���[�v name �S�̂ɒ�`���ꂽ�����ʂ��擾����BXXXGROUP ���[�h�AGlobal ���[�h�̎������L���B
=end
----------------------------------------------------------------------------*/
	%extend{
		VALUE getValue(const char* name,const char* stype=NULL){
			kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
			if( data != NULL ){
				kmb::PhysicalValue* val = data->getPhysicalValue();
				if( val != NULL ){
					return toVALUE( val );
				}
			}
			return Qnil;
		}
	}

/**-------------------------------------------------------------------------
=begin
--- setValue(name,v,stype=nil)
	�O���[�v name �S�̂ɕ����� v ���`����BXXXGROUP ���[�h�܂��� Global ���[�h�̎������L���B
=end
---------------------------------------------------------------------------*/
	%extend{
		VALUE setValue(const char* name,VALUE v,const char* stype=NULL){
			kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
			if( data != NULL ){
				kmb::PhysicalValue* val =
					(data->getValueType() == kmb::PhysicalValue::Array) ?
					createArrayFromVALUE(v) : createFromVALUE(v);
				if( val != NULL ){
					if( val->getType() == data->getValueType() ){
						data->setPhysicalValue(val);
					}else{
						delete val;
					}
				}
			}
			return Qnil;
		}
	}

/**--------------------------------------------------------------------------
=begin
--- addId(name,id,stype=nil)
	name �̃f�[�^�� id ��ǉ�����B
=end
---------------------------------------------------------------------------*/
	%extend{
		VALUE addId(const char* name,VALUE id,const char* stype=NULL){
			kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
			if( data != NULL ){
				if( TYPE(id) == T_ARRAY ){
					int id0 = NUM2INT(rb_ary_entry(id,0));
					int id1 = NUM2INT(rb_ary_entry(id,1));
					data->addId(kmb::Face(id0,id1));
				}else{
					data->addId(FIX2INT(id));
				}
			}
			return Qnil;
		}
	}

/**--------------------------------------------------------------------------
=begin
--- hasId(name,id,stype=nil)
	name �̃f�[�^�� id ���܂ނ��ǂ�����Ԃ�
=end
---------------------------------------------------------------------------*/
	%extend{
		bool hasId(const char* name,VALUE id,const char* stype=NULL){
			kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
			if( data != NULL ){
				if( TYPE(id) == T_ARRAY ){
					int id0 = NUM2INT(rb_ary_entry(id,0));
					int id1 = NUM2INT(rb_ary_entry(id,1));
					return data->hasId(kmb::Face(id0,id1));
				}else{
					return data->hasId(FIX2INT(id));
				}
			}
			return false;
		}
	}

/**--------------------------------------------------------------------------
=begin
--- deleteId(name,id,stype=nil)
	name �̃f�[�^���� id ���폜����
=end
---------------------------------------------------------------------------*/
	%extend{
		bool deleteId(const char* name,VALUE id,const char* stype=NULL){
			kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
			if( data != NULL ){
				if( TYPE(id) == T_ARRAY ){
					int id0 = NUM2INT(rb_ary_entry(id,0));
					int id1 = NUM2INT(rb_ary_entry(id,1));
					return data->deleteId(kmb::Face(id0,id1));
				}else{
					return data->deleteId(FIX2INT(id));
				}
			}
			return false;
		}
	}

/**--------------------------------------------------------------------------
=begin
--- setTargetData(names,stype=nil)
	setMultiValuesAtId �ŕ����̕����ʂ���x�ɗ^���邽�߂�
	�Ώۂ�ݒ肷��
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE setTargetData(VALUE names,const char* stype=NULL){
		Check_Type(names, T_ARRAY);
		const int len = RARRAY_LEN(names);
		self->clearTargetData();
		for(int i=0;i<len;++i)
		{
			VALUE item = rb_ary_entry(names,i);
			if(TYPE(item) == T_STRING)
			{
				self->appendTargetData( StringValueCStr(item), stype );
			}
		}
		return Qnil;
	}
}
/**--------------------------------------------------------------------------
=begin
--- setMultiValuesAtId(id,v)
	setTargetData �őΏۂ��^�����Ă��鎞��
	�O���[�v name �� id �ɕ����ʂ̔z�� v ��^����BNodeVariable or ElementVariable ���[�h�̎������L���B
	v �� setTargetData �œo�^���ꂽ�����ʂɗ^����l�����ɕ��ׂ� 1 �����z��B
	��Fdisplace, stress, mises �� 3, 6, 1 �����œo�^����Ă����ꍇ��
	3 + 6 + 1 = 10 ������ 1 �����z���^����B
=end
----------------------------------------------------------------------------*/
%extend{
	VALUE setMultiValuesAtId(VALUE id,VALUE v){
		Check_Type(v, T_ARRAY);
		const int len = RARRAY_LEN(v);
		if( len > 0 ){
			VALUE first = rb_ary_entry(v,0);
			if( TYPE(first) == T_FIXNUM ){
				long* values = NULL;
				values = new long[len];
				for(int i=0;i<len;++i)
				{
					VALUE item = rb_ary_entry(v,i);
					values[i] = FIX2INT(item);
				}
				self->setMultiPhysicalValues( FIX2INT(id), values );
				delete[] values;
			}else if( TYPE(first) == T_FLOAT ){
				double* values = NULL;
				values = new double[len];
				for(int i=0;i<len;++i)
				{
					VALUE item = rb_ary_entry(v,i);
					values[i] = NUM2DBL(item);
				}
				self->setMultiPhysicalValues( FIX2INT(id), values );
				delete[] values;
			}
		}
		return Qnil;
	}
}
/**-------------------------------------------------------------------------
=begin
--- eachId(name,stype=nil)
	name �O���[�v�� Id ���o�͂��� iterator
=end
---------------------------------------------------------------------------*/
%extend{
	void eachId(const char* name,const char* stype=NULL){
		kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
		if( data ){
			switch( data->getBindingMode() ){
				case kmb::DataBindings::NodeGroup:
				case kmb::DataBindings::ElementGroup:
				case kmb::DataBindings::BodyGroup:
				case kmb::DataBindings::NodeVariable:
				case kmb::DataBindings::ElementVariable:
				case kmb::DataBindings::BodyVariable:
				{
					kmb::DataBindings::iterator dIter = data->begin();
					kmb::DataBindings::iterator dEnd = data->end();
					while( dIter != dEnd ){
						rb_yield( INT2FIX( dIter.getId() ) );
						++dIter;
					}
					break;
				}
				case kmb::DataBindings::FaceGroup:
				case kmb::DataBindings::FaceVariable:
				{
					kmb::Face f;
					kmb::DataBindings::iterator dIter = data->begin();
					kmb::DataBindings::iterator dEnd = data->end();
					while( dIter != dEnd ){
						if( dIter.getFace( f ) ){
							rb_yield( toVALUE(f) );
						}
						++dIter;
					}
					break;
				}
				case kmb::DataBindings::Global:
				default:
					break;
			}
		}
	}
}

/**------------------------------------------------------------------------
=begin
--- eachIdWithValue(name,stype)
	name �O���[�v�� Id �ƕ����ʂ̑g���o�͂��� iterator
	�l�̌^�� Scalar �̎��� value �͒l���̂��́A
	Vector3 �܂��� Tensor6 �̏ꍇ�́A�z��ŗ^������B
	�ȉ��̂悤�ɂ��Ďg���B

	 mesh.eachIdWithValue("weight"){ |id,val|
	  puts "id = #{id} x = #{val[0]} y = #{val[1]} z = #{val[2]}"
	 }

=end
-------------------------------------------------------------------------*/
%extend{
	void eachIdWithValue(const char* name,const char* stype=NULL){
		kmb::DataBindings* data = self->getDataBindingsPtr(name,stype);
		if( data ){
			switch( data->getBindingMode() ){
				case kmb::DataBindings::NodeVariable:
				case kmb::DataBindings::ElementVariable:
				case kmb::DataBindings::BodyVariable:
				{
					kmb::DataBindings::iterator dIter = data->begin();
					switch( data->getValueType() ){
						case kmb::PhysicalValue::Scalar:
						{
							double v = 0.0;
							while( !dIter.isFinished() ){
								if( dIter.getValue(&v) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), rb_float_new(v) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Integer:
						{
							long l = 0;
							while( !dIter.isFinished() ){
								if( dIter.getValue(&l) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), INT2FIX(l) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector2:
						{
							double v[2] = {0.0,0.0};
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE(v[0],v[1]) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector3:
						{
							double v[3] = {0.0,0.0,0.0};
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE(v[0],v[1],v[2]) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector4:
						{
							double v[4] = {0.0,0.0,0.0,0.0};
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE(v[0],v[1],v[2],v[3]) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Tensor6:
						{
							double v[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE(v[0],v[1],v[2],v[3],v[4],v[5]) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector2withInt:
						{
							double v[2] = {0.0,0.0};
							long iVal = 0L;
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) && dIter.getValue(&iVal) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE(v[0],v[1],iVal) );
								}
								++dIter;
							}
							break;
						}
						default:
						{
							while( !dIter.isFinished() ){
								kmb::PhysicalValue* val = dIter.getValue();
								if( val ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE( val ) );
								}
								++dIter;
							}
							break;
						}
					}
				}
				break;
				case kmb::DataBindings::FaceVariable:
				{
					kmb::Face f;
					kmb::DataBindings::iterator dIter = data->begin();
					switch( data->getValueType() ){
						case kmb::PhysicalValue::Scalar:
						{
							double v=0.0;
							while( !dIter.isFinished() ){
								if( dIter.getValue(&v) && dIter.getFace( f ) ){
									rb_yield_values( 2, toVALUE( f ), rb_float_new(v) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Integer:
						{
							long l=0;
							while( !dIter.isFinished() ){
								if( dIter.getValue(&l) && dIter.getFace( f ) ){
									rb_yield_values( 2, toVALUE( f ), INT2FIX(l) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector2:
						{
							double v[2]={0.0,0.0};
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) && dIter.getFace( f ) ){
									rb_yield_values( 2, toVALUE( f ), toVALUE(v[0],v[1]) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector3:
						{
							double v[3]={0.0,0.0,0.0};
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) && dIter.getFace( f ) ){
									rb_yield_values( 2, toVALUE( f ), toVALUE(v[0],v[1],v[2]) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector4:
						{
							double v[4] = {0.0,0.0,0.0,0.0};
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE(v[0],v[1],v[2],v[3]) );
								}
								++dIter;
							}
							break;
						}
						case kmb::PhysicalValue::Vector2withInt:
						{
							double v[2] = {0.0,0.0};
							long iVal = 0L;
							while( !dIter.isFinished() ){
								if( dIter.getValue(v) && dIter.getValue(&iVal) ){
									rb_yield_values( 2, INT2FIX( dIter.getId() ), toVALUE(v[0],v[1],iVal) );
								}
								++dIter;
							}
							break;
						}
						default:
						{
							while( !dIter.isFinished() ){
								kmb::PhysicalValue* val = dIter.getValue();
								if( val && dIter.getFace( f ) ){
									rb_yield_values( 2, toVALUE( f ) , toVALUE( val ) );
								}
								++dIter;
							}
							break;
						}
					}
				}
				break;
				default:
					break;
			}
		}
	}
}

/**--------------------------------------------------------------------------
=begin
--- faceGroupToBody(name,stype=nil)
	FaceGroup �̃f�[�^�� body �ɕϊ�����B
=end
----------------------------------------------------------------------------*/
	kmb::bodyIdType faceGroupToBody(const char* name,const char* stype=NULL);
};

}
