/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : PatchOperation                                          #
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

#include "MeshGen/kmbEdgeStitcher.h"
#include "Common/kmbCalculator.h"
#include "Geometry/kmbPoint3DContainer.h"
#include "MeshDB/kmbElementContainer.h"
#include "MeshDB/kmbMeshDB.h"
#include "MeshDB/kmbMatching.h"
#include "MeshGen/kmbMeshSmoother.h"
#include <cmath>

// local function
bool addTriangle( kmb::ElementContainer* triangles, kmb::nodeIdType n0, kmb::nodeIdType n1, kmb::nodeIdType n2 )
{
	if( triangles && n0 != n1 && n1 != n2 && n2 != n0 ){
		kmb::nodeIdType tri[3] = {n0, n1, n2};
		triangles->addElement( kmb::TRIANGLE, tri );
		return true;
	}else{
		return false;
	}
}

kmb::EdgeStitcher::EdgeStitcher(void)
: mesh(NULL)
, points(NULL)
, f0(kmb::nullNodeId)
, fend(kmb::nullNodeId)
, b0(kmb::nullNodeId)
, bend(kmb::nullNodeId)
, normalCheck(false)
{
}

kmb::EdgeStitcher::~EdgeStitcher(void)
{
}

void
kmb::EdgeStitcher::setMesh(kmb::MeshData* mesh)
{
	this->mesh = mesh;
	if( mesh ){
		this->points = mesh->getNodes();
	}
}

bool
kmb::EdgeStitcher::setEdges(kmb::bodyIdType forwardId,kmb::bodyIdType backwardId)
{
	if( mesh ){
		kmb::ElementContainer* f = mesh->getBodyPtr(forwardId);
		kmb::ElementContainer* b = mesh->getBodyPtr(backwardId);
		if( f != NULL && f->isUniqueType( kmb::SEGMENT )  &&
			b != NULL && b->isUniqueType( kmb::SEGMENT )  )
		{
			this->forward.setEdges( f );
			this->backward.setEdges( b );
			normal.zero();
			return true;
		}
	}
	return false;
}

bool
kmb::EdgeStitcher::setEdges(const char* forward,const char* backward)
{
	if( mesh ){
		kmb::DataBindings* fData = mesh->getDataBindingsPtr(forward);
		kmb::DataBindings* bData = mesh->getDataBindingsPtr(backward);
		if( fData && fData->getBindingMode() == kmb::DataBindings::FaceGroup && fData->getTargetBodyId() != kmb::Body::nullBodyId &&
			bData && bData->getBindingMode() == kmb::DataBindings::FaceGroup && bData->getTargetBodyId() != kmb::Body::nullBodyId )
		{
			kmb::ElementContainer* fBody = mesh->getBodyPtr( fData->getTargetBodyId() );
			kmb::ElementContainer* bBody = mesh->getBodyPtr( bData->getTargetBodyId() );
			this->forward.setEdgesByFaceGroup( fData, fBody );
			this->backward.setEdgesByFaceGroup( bData, bBody );
			normal.zero();
			return true;
		}
	}
	return false;
}

bool
kmb::EdgeStitcher::setEdgesWithinDistance(kmb::bodyIdType forward,kmb::bodyIdType backward,double distance)
{
	if( mesh ){
		kmb::ElementContainer* f = mesh->getBodyPtr(forward);
		kmb::ElementContainer* b = mesh->getBodyPtr(backward);
		if( f != NULL && f->isUniqueType( kmb::SEGMENT )  &&
			b != NULL && b->isUniqueType( kmb::SEGMENT )  )
		{
			// f �� b �̋����� distance �ȉ�
			// �̂��̂�o�^����

			kmb::ElementContainerMap fnear;
			kmb::ElementContainerMap bnear;
			kmb::elementIdType nearestId;
			kmb::Point3D p0, p1;
			kmb::nodeIdType cell[2] = {kmb::nullNodeId,kmb::nullNodeId};
			double param = 0.0;

			kmb::ElementContainer::iterator eIterf = f->begin();
			while( !eIterf.isFinished() )
			{
				if( points->getPoint( eIterf.getCellId(0), p0 ) &&
					points->getPoint( eIterf.getCellId(1), p1 ) )
				{
					if( kmb::Matching::getDistanceEdgeToNodeWithParam( points, b, &p0, nearestId, param ) < distance &&
						kmb::Matching::getDistanceEdgeToNodeWithParam( points, b, &p1, nearestId, param ) < distance )
					{
						cell[0] = eIterf.getCellId(0);
						cell[1] = eIterf.getCellId(1);
						fnear.addElement( kmb::SEGMENT, cell );
					}
				}
				++eIterf;
			}

			kmb::ElementContainer::iterator eIterb = b->begin();
			while( !eIterb.isFinished() )
			{
				if( points->getPoint( eIterb.getCellId(0), p0 ) &&
					points->getPoint( eIterb.getCellId(1), p1 ) )
				{
					if( kmb::Matching::getDistanceEdgeToNodeWithParam( points, f, &p0, nearestId, param ) < distance &&
						kmb::Matching::getDistanceEdgeToNodeWithParam( points, f, &p1, nearestId, param ) < distance )
					{
						cell[0] = eIterb.getCellId(0);
						cell[1] = eIterb.getCellId(1);
						bnear.addElement( kmb::SEGMENT, cell );
					}
				}
				++eIterb;
			}
			// Polygon �͓����� edges �̕��������
			this->forward.setEdges( &fnear );
			this->backward.setEdges( &bnear );
			kmb::nodeIdType initF=kmb::nullNodeId, endF=kmb::nullNodeId, initB=kmb::nullNodeId, endB=kmb::nullNodeId;
			// �A�����`�F�b�N
			if( !this->forward.getEndPoints(initF,endF) || !this->backward.getEndPoints(initB,endB) ){
				// �ǂ��炩���A���ł͂Ȃ�
				return false;
			}
			normal.zero();
			return true;
		}
	}
	return false;
}

bool
kmb::EdgeStitcher::setEdgesWithInitNodes(kmb::bodyIdType forward,kmb::nodeIdType forwardInit,kmb::bodyIdType backward,kmb::nodeIdType backwardInit)
{
	if( setEdges(forward,backward) ){
		setInitialNode( forwardInit, backwardInit );
		return true;
	}
	return false;
}

kmb::bodyIdType
kmb::EdgeStitcher::stitchEdgesByTriangles(void)
{
	if( mesh && points && forward.getSize() > 0 && backward.getSize() > 0 ){
		setInitialNode();
		if( normalCheck ){
			setInitNormal();
		}else{
			normal.zero();
		}
		kmb::ElementContainerMap* result = NULL;
		result = new kmb::ElementContainerMap();
		while( this->stitchByTriangle(result) ){
		}
		if( result->getCount() > 0 ){
			return mesh->appendBody( result );
		}else{
			delete result;
		}
	}
	return kmb::Body::nullBodyId;
}

kmb::bodyIdType
kmb::EdgeStitcher::stitchEdgesByQuads(void)
{
	if( mesh && points && forward.getSize() > 0 && backward.getSize() > 0 ){
		setInitialNode();
		if( normalCheck ){
			setInitNormal();
		}else{
			normal.zero();
		}
		kmb::ElementContainerMap* result = NULL;
		result = new kmb::ElementContainerMap();
		while( this->stitchByQuad(result) ){
		}
		if( result->getCount() > 0 ){
			return mesh->appendBody( result );
		}else{
			delete result;
		}
	}
	return kmb::Body::nullBodyId;
}

void
kmb::EdgeStitcher::setNormalCheck(bool check)
{
	this->normal.zero();
	this->normalCheck = check;
}

bool
kmb::EdgeStitcher::getNormalCheck(void) const
{
	return this->normalCheck;
}

void
kmb::EdgeStitcher::setInitNormal(void)
{
	// �����@���x�N�g���� + edgesbackward �̖@���x�N�g���� - edgesforward �̖@���x�N�g���̘a
	kmb::nodeIdType n0 = b0;
	kmb::nodeIdType n1 = backward.getNodeId( n0, -1 );
	kmb::nodeIdType n2 = backward.getNodeId( n1, -1 );
	do{
		normal += points->calcVerticalVector(n0,n1,n2);
		n1 = n2;
		n2 = backward.getNodeId( n1, -1 );
	}while( n2 != kmb::nullNodeId && n2 != b0 );
	n0 = f0;
	n1 = forward.getNodeId( n0, 1 );
	n2 = forward.getNodeId( n1, 1 );
	do{
		normal -= points->calcVerticalVector(n0,n1,n2);
		n1 = n2;
		n2 = forward.getNodeId( n1, 1 );
	}while( n2 != kmb::nullNodeId && n2 != f0 );
}

void
kmb::EdgeStitcher::setInitialNode( kmb::nodeIdType forwardInit, kmb::nodeIdType backwardInit )
{
	this->f0 = forwardInit;
	this->b0 = backwardInit;
	if( f0 != kmb::nullNodeId && b0 != kmb::nullNodeId ){
		return;
	}
	kmb::nodeIdType initF=kmb::nullNodeId, endF=kmb::nullNodeId, initB=kmb::nullNodeId, endB=kmb::nullNodeId;
	// �A�����`�F�b�N
	if( !forward.getEndPoints(initF,endF) || !backward.getEndPoints(initB,endB) ){
		// �ǂ��炩���A���ł͂Ȃ�
		return;
	}
	if( initF == kmb::nullNodeId && endB == kmb::nullNodeId ){
		// �������[�v�̎�
		if( f0 != kmb::nullNodeId ){
			initF = f0;
		}else if( b0 != kmb::nullNodeId ){
			endB = b0;
		}else{
			initF = forward.getEdges()->begin().getCellId(0);
		}
	}
	if( initF == kmb::nullNodeId && endB != kmb::nullNodeId ){
		// backward �����[�v����Ȃ�
		// forward �̒����� backward �̍ŋߓ_��T��
		kmb::Point3D node, target;
		if( points->getPoint( endB, node ) ){
			kmb::Minimizer min;
			kmb::ElementContainer::const_iterator eIter = forward.getEdges()->begin();
			while( !eIter.isFinished() ){
				if( points->getPoint( eIter.getCellId(0), target ) ){
					if( min.update( node.distanceSq( target ) ) ){
						initF = eIter.getCellId(0);
					}
				}
				++eIter;
			}
		}
	}
	if( initF != kmb::nullNodeId && endB == kmb::nullNodeId ){
		// backward �̒����� forward �̍ŋߓ_��T��
		kmb::Point3D node, target;
		if( points->getPoint( initF, node ) ){
			kmb::Minimizer min;
			kmb::ElementContainer::const_iterator eIter = backward.getEdges()->begin();
			while( !eIter.isFinished() ){
				if( points->getPoint( eIter.getCellId(0), target ) ){
					if( min.update( node.distanceSq( target ) ) ){
						endB = eIter.getCellId(0);
					}
				}
				++eIter;
			}
		}
	}
	f0 = initF;
	b0 = endB;
}

// ���p�` forward �̓_ f0 ����������
// ���p�` backward �̓_ b0 ���t������
// �O�p�`�����i1�X�e�b�v�����j
// fend bend �ɓ��B�����炻��ȏ�͐i�߂Ȃ�
bool
kmb::EdgeStitcher::stitchByTriangle( kmb::ElementContainer* result )
{
	if( result == NULL ){
		return false;
	}
	kmb::nodeIdType f1 = (f0!=fend) ? forward.getNodeId(f0,1) : kmb::nullNodeId;
	kmb::nodeIdType b1 = (b0!=bend) ? backward.getNodeId(b0,-1) : kmb::nullNodeId;
	if( normalCheck && normal * points->calcVerticalVector( f0, b0, b1) < 0.0 ){
		b1 = kmb::nullNodeId;
	}
	if( normalCheck && normal * points->calcVerticalVector( f0, b0, f1) < 0.0 ){
		f1 = kmb::nullNodeId;
	}
	if( f1 == kmb::nullNodeId ){
		if( b1 == kmb::nullNodeId ){
			return false;
		}else{
			// b1 �̂ݎg����
			addTriangle( result, f0, b0, b1 );
			if( normalCheck ){
				normal = points->calcVerticalVector( f0, b0, b1 );
			}
			if( bend == kmb::nullNodeId ){
				bend = b0;
			}
			b0 = b1;
		}
	}else{
		if( b1 == kmb::nullNodeId ){
			// f1 �̂ݎg����
			addTriangle( result, f0, b0, f1 );
			if( normalCheck ){
				normal = points->calcVerticalVector( f0, b0, f1 );
			}
			if( fend == kmb::nullNodeId ){
				fend = f0;
			}
			f0 = f1;
		}else{
			// �����g���鎞�͊p�x�Ŕ��f
			kmb::Point3D pf0,pf1,pb0,pb1;
			if( points->getPoint(f0,pf0) &&
				points->getPoint(f1,pf1) &&
				points->getPoint(b0,pb0) &&
				points->getPoint(b1,pb1) )
			{
				// f0 f1 b1 b0 ����Ȃ�l�p�`�� cos ����ԏ������Ȃ�Ƃ���𕪊�����悤�ȎO�p�`�ɂ���
				kmb::nodeIdType nextId = kmb::nullNodeId;
				kmb::Minimizer min;
				if( min.update( Point3D::cos( pb0, pf0, pf1 ) ) ){
					nextId = b1;
				}
				if( min.update( Point3D::cos( pf1, pb1, pb0 ) ) ){
					nextId = b1;
				}
				if( min.update( Point3D::cos( pb1, pb0, pf0 ) ) ){
					nextId = f1;
				}
				if( min.update( Point3D::cos( pf0, pf1, pb1 ) ) ){
					nextId = f1;
				}
				if( nextId == b1 ){
					addTriangle( result, f0, b0, b1 );
					if( normalCheck ){
						normal = points->calcVerticalVector( f0, b0, b1 );
					}
					if( bend == kmb::nullNodeId ){
						bend = b0;
					}
					b0 = b1;
				}else{
					addTriangle( result, f0, b0, f1 );
					if( normalCheck ){
						normal = points->calcVerticalVector( f0, b0, f1 );
					}
					if( fend == kmb::nullNodeId ){
						fend = f0;
					}
					f0 = f1;
				}
			}else{
				return false;
			}
		}
	}
	return true;
}

// ���p�` forward �̓_ f0 ����������
// ���p�` backward �̓_ b0 ���t������
// �l�p�`�����
// fend bend �ɓ��B�����炻��ȏ�͐i�߂Ȃ�
// �I�������������鎞�ɂ͖@���x�N�g���ƕӂ̒����Ŕ��f����
bool
kmb::EdgeStitcher::stitchByQuad( kmb::ElementContainer* result )
{
	if( result == NULL ){
		return false;
	}
	kmb::nodeIdType f1 = (f0!=fend) ? forward.getNodeId(f0,1) : kmb::nullNodeId;
	kmb::nodeIdType f2 = (f1!=fend) ? forward.getNodeId(f1,1) : kmb::nullNodeId;
	kmb::nodeIdType b1 = (b0!=bend) ? backward.getNodeId(b0,-1) : kmb::nullNodeId;
	kmb::nodeIdType b2 = (b1!=bend) ? backward.getNodeId(b1,-1) : kmb::nullNodeId;
	unsigned int flag = 0;
	// ���_��i�߂邩�ǂ����̔���
	if( f1 != kmb::nullNodeId ) flag |= 0x01;
	if( f2 != kmb::nullNodeId ) flag |= 0x02;
	if( b1 != kmb::nullNodeId ) flag |= 0x04;
	if( b2 != kmb::nullNodeId ) flag |= 0x08;
	if( (flag&0x09)==0x09 && normalCheck && normal * points->calcNormalVector( f0, b0, f1 ) <= 0.0 ){
		f1 = kmb::nullNodeId;
		f2 = kmb::nullNodeId;
		flag &= ~0x03;
	}else if( (flag&0x06)==0x06 && normalCheck && normal * points->calcNormalVector( f1, b0, f2 ) <= 0.0 ){
		f2 = kmb::nullNodeId;
		flag &= ~0x02;
	}
	if( (flag&0x06)==0x06 && normalCheck && normal * points->calcNormalVector( f0, b0, b1 ) <= 0.0 ){
		b1 = kmb::nullNodeId;
		b2 = kmb::nullNodeId;
		flag &= ~0x0c;
	}else if( (flag&0x09)==0x09 && normalCheck && normal * points->calcNormalVector( f0, b1, b2 ) <= 0.0 ){
		b2 = kmb::nullNodeId;
		flag &= ~0x08;
	}
	kmb::nodeIdType nodes[4] = {f0,b0,kmb::nullNodeId,kmb::nullNodeId};
	switch(flag){
		case 0x00: // �ǂ���̕ӂ����łɏI�_
		case 0x01: // f1 forward 1�񂵂��i�߂Ȃ�
		case 0x04: // b1 backward 1�񂵂��i�߂Ȃ�
			return false;
		case 0x03: // f1 f2
			nodes[2] = f2;
			nodes[3] = f1;
			result->addElement( kmb::QUAD, nodes );
			// ���[�v�̎��̂��߂ɏI���_���o���Ă���
			if( fend == kmb::nullNodeId ){
				fend = f0;
			}
			f0 = f2;
			normal = points->calcNormalVector( nodes[0], nodes[1], nodes[2], nodes[3] );
			return true;
		case 0x05: // f1 b1
			nodes[2] = b1;
			nodes[3] = f1;
			result->addElement( kmb::QUAD, nodes );
			// ���[�v�̎��̂��߂ɏI���_���o���Ă���
			if( fend == kmb::nullNodeId ){
				fend = f0;
			}
			if( bend == kmb::nullNodeId ){
				bend = b0;
			}
			f0 = f1;
			b0 = b1;
			normal = points->calcNormalVector( nodes[0], nodes[1], nodes[2], nodes[3] );
			return true;
		case 0x0c: // b1 b2
			nodes[2] = b1;
			nodes[3] = b2;
			result->addElement( kmb::QUAD, nodes );
			// ���[�v�̎��̂��߂ɏI���_���o���Ă���
			if( bend == kmb::nullNodeId ){
				bend = b0;
			}
			b0 = b2;
			normal = points->calcNormalVector( nodes[0], nodes[1], nodes[2], nodes[3] );
			return true;
		case 0x07: // f1 f2 b1
			{
				// f1 b1 �� f1 f2 �̔�r
				double f1b1 = points->distanceSq(f1,b1) + points->distanceSq(b1,b0);
				double f1f2 = points->distanceSq(f1,f2) + points->distanceSq(f2,b0);
				if( fabs(f1b1) <= fabs(f1f2) ){
					// f1 b1
					nodes[2] = b1;
					nodes[3] = f1;
					result->addElement( kmb::QUAD, nodes );
					// ���[�v�̎��̂��߂ɏI���_���o���Ă���
					if( fend == kmb::nullNodeId ){
						fend = f0;
					}
					if( bend == kmb::nullNodeId ){
						bend = b0;
					}
					f0 = f1;
					b0 = b1;
					normal = points->calcNormalVector( nodes[0], nodes[1], nodes[2], nodes[3] );
					return true;
				}else{
					// f1 f2
					nodes[2] = f2;
					nodes[3] = f1;
					result->addElement( kmb::QUAD, nodes );
					// ���[�v�̎��̂��߂ɏI���_���o���Ă���
					if( fend == kmb::nullNodeId ){
						fend = f0;
					}
					f0 = f2;
					normal = points->calcNormalVector( nodes[0], nodes[1], nodes[2], nodes[3] );
					return true;
				}
			}
		case 0x0d: // f1 b1 b2
			{
				// f1 b1 �� b1 b2 �̔�r
				double f1b1 = points->distanceSq(f0,f1) + points->distanceSq(f1,b1);
				double b2b1 = points->distanceSq(f0,b2) + points->distanceSq(b2,b1);
				if( fabs(f1b1) <= fabs(b2b1) ){
					// f1 b1
					nodes[2] = b1;
					nodes[3] = f1;
					result->addElement( kmb::QUAD, nodes );
					// ���[�v�̎��̂��߂ɏI���_���o���Ă���
					if( fend == kmb::nullNodeId ){
						fend = f0;
					}
					if( bend == kmb::nullNodeId ){
						bend = b0;
					}
					f0 = f1;
					b0 = b1;
					normal = points->calcNormalVector( nodes[0], nodes[1], nodes[2], nodes[3] );
					return true;
				}else{
					// b1 b2
					nodes[2] = b1;
					nodes[3] = b2;
					result->addElement( kmb::QUAD, nodes );
					// ���[�v�̎��̂��߂ɏI���_���o���Ă���
					if( bend == kmb::nullNodeId ){
						bend = b0;
					}
					b0 = b2;
					normal = points->calcNormalVector( nodes[0], nodes[1], nodes[2], nodes[3] );
					return true;
				}
			}
		case 0x0f: // f1 f2 b1 b2
			{
				// f1 b1 �� f1 f2 �� b1 b2 �̔�r
				double f1b1 = points->distanceSq(f0,f1) + points->distanceSq(f1,b1) + points->distanceSq(b1,b0);
				double f1f2 = points->distanceSq(f0,f1) + points->distanceSq(f1,f2) + points->distanceSq(f2,b0);
				double b2b1 = points->distanceSq(f0,b2) + points->distanceSq(b2,b1) + points->distanceSq(b1,b0);
				if( fabs(f1b1) <= fabs(f1f2) ){
					if( fabs(f1b1) <= fabs(b2b1) ){
						// f1 b1
						nodes[2] = b1;
						nodes[3] = f1;
						result->addElement( kmb::QUAD, nodes );
						// ���[�v�̎��̂��߂ɏI���_���o���Ă���
						if( fend == kmb::nullNodeId ){
							fend = f0;
						}
						// ���[�v�̎��̂��߂ɏI���_���o���Ă���
						if( bend == kmb::nullNodeId ){
							bend = b0;
						}
						f0 = f1;
						b0 = b1;
					}else{
						// b1 b2
						nodes[2] = b1;
						nodes[3] = b2;
						result->addElement( kmb::QUAD, nodes );
						// ���[�v�̎��̂��߂ɏI���_���o���Ă���
						if( bend == kmb::nullNodeId ){
							bend = b0;
						}
						b0 = b2;
					}
				}else{
					if( fabs(f1f2) <= fabs(b2b1) ){
						// f1 f2
						nodes[2] = f2;
						nodes[3] = f1;
						result->addElement( kmb::QUAD, nodes );
						// ���[�v�̎��̂��߂ɏI���_���o���Ă���
						if( fend == kmb::nullNodeId ){
							fend = f0;
						}
						f0 = f2;
					}else{
						// b1 b2
						nodes[2] = b1;
						nodes[3] = b2;
						result->addElement( kmb::QUAD, nodes );
						// ���[�v�̎��̂��߂ɏI���_���o���Ă���
						if( bend == kmb::nullNodeId ){
							bend = b0;
						}
						b0 = b2;
					}
				}
				normal = points->calcNormalVector( nodes[0], nodes[1], nodes[2], nodes[3] );
				return true;
			}
		default:
			break;
	}
	return false;
}

kmb::bodyIdType
kmb::EdgeStitcher::stitchByQuadsWithNodeCount(int fCount,int bCount)
{
	if( mesh && points && forward.getSize() > 0 && backward.getSize() > 0 ){
		setInitialNode();
		if( normalCheck ){
			setInitNormal();
		}else{
			normal.zero();
		}
		std::vector<kmb::nodeIdType> appendNodes;
		kmb::ElementContainerMap* result = NULL;
		result = new kmb::ElementContainerMap();
		// �ߓ_�̌��ɉ����ĂȂ�����ς���
		// 4�_�ȏ�ߓ_��ǉ�����ꍇ�́A����������ƍH�v���Ȃ��ƁA
		// �����Ƃ���ɂ������_��ǉ����Ă��܂��B
		if( forward.getSize() == 4 && backward.getSize() == 6 ){
			// forward �̕������Ȃ��ꍇ�́A�K���ɓ_��ǉ�����
			kmb::Point3D backCenter;
			kmb::Point3D forwardCenter;
			if( backward.getCenter( points, backCenter ) && forward.getCenter( points, forwardCenter ) ){
				// �d�S�Ɉ�ԋ߂��Ƃ���
				kmb::nodeIdType a0 = forward.getNearestNode( points, backCenter );
				kmb::nodeIdType nodes[4] = {kmb::nullNodeId,kmb::nullNodeId,kmb::nullNodeId,kmb::nullNodeId};
				if( a0 != kmb::nullNodeId ){
					kmb::nodeIdType a_next = forward.getNodeId( a0, 1 );
					kmb::nodeIdType a_prev = forward.getNodeId( a0, -1 );
					kmb::Point3D p_next,p_prev,p0;
					if( points->getPoint(a0,p0) && points->getPoint(a_next,p_next) && points->getPoint(a_prev,p_prev) ){
						kmb::Vector3D diffCenter( backCenter, forwardCenter );
						kmb::Vector3D v_next( p_next, p0 );
						kmb::Vector3D v_prev( p_prev, p0 );
						// �d�S�̂���Ƃ̕����𒲂ׂ�
						if( v_next * diffCenter < v_prev * diffCenter ){
							nodes[0] = a0;
							nodes[1] = points->addPoint(p0);
							nodes[2] = points->addPoint(p_next);
							nodes[3] = a_next;
							// forward �� [f0,f0,f1,f1] ��ǉ�
							// [f0,f1,f2,f3] �Ƃ݂Ȃ�
							// nodes[1] �� nodes[2] �͂Q�d�_
							forward.deleteSegment( nodes[0], nodes[3] );
							forward.addSegment( nodes[0], nodes[1] );
							forward.addSegment( nodes[1], nodes[2] );
							forward.addSegment( nodes[2], nodes[3] );
						}else{
							nodes[0] = a_prev;
							nodes[1] = points->addPoint(p_prev);
							nodes[2] = points->addPoint(p0);
							nodes[3] = a0;
							// forward �� [f-1,f-1,f0,f0] ��ǉ�
							// [f-3,f-2,f-1,f0] �Ƃ݂Ȃ�
							// nodes[1] �� nodes[2] �͂Q�d�_
							forward.deleteSegment( nodes[0], nodes[3] );
							forward.addSegment( nodes[0], nodes[1] );
							forward.addSegment( nodes[1], nodes[2] );
							forward.addSegment( nodes[2], nodes[3] );
						}
						// nodes[1] �� nodes[2] �͂��Ƃ� smoothing ����
						appendNodes.push_back( nodes[1] );
						appendNodes.push_back( nodes[2] );
						result->addElement( kmb::QUAD, nodes );
						++fCount;
					}
				}
			}
		}
		if( forward.getSize() == backward.getSize() && fCount == bCount ){
			fend = f0;
			bend = b0;
			kmb::nodeIdType nodes[4] = {f0,b0,kmb::nullNodeId,kmb::nullNodeId};
			while( f0 != kmb::nullNodeId && b0 != kmb::nullNodeId ){
				kmb::nodeIdType f1 = forward.getNodeId(f0,1);
				kmb::nodeIdType b1 = backward.getNodeId(b0,-1);
				nodes[0] = f0;
				nodes[1] = b0;
				nodes[2] = b1;
				nodes[3] = f1;
				result->addElement( kmb::QUAD, nodes );
				f0 = ( f1 == fend ) ? kmb::nullNodeId : f1;
				b0 = ( b1 == bend ) ? kmb::nullNodeId : b1;
			}
		}
		if( result->getCount() > 0 ){
			kmb::bodyIdType bodyId = mesh->appendBody( result );
			kmb::MeshSmoother smoother(mesh);
			// �Q�� smoothing ��������
			for(int i=0;i<2;++i){
				smoother.init();
				smoother.appendBody(bodyId);
				std::vector< kmb::nodeIdType >::iterator nIter = appendNodes.begin();
				while( nIter != appendNodes.end() ){
					smoother.smoothingNode(*nIter);
					++nIter;
				}
				smoother.commit();
			}
			return bodyId;
		}else{
			delete result;
		}
	}
	return kmb::Body::nullBodyId;
}
