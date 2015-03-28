/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Module Name : RevocapIO                                         #
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

/**-----------------------------------------------------------------
=begin
= RevocapIO

RevocapIO �Ƃ� RevocapMesh �ւ̓��o�͂��`�������W���[���ł���B

==���W���[���萔

((<VERSION>))
((<COPYRIGHT>))

== ���W���[���萔
�ȉ��̒萔�� RevocapGL ���W���[���Œ�`����Ă���B
--- VERSION
	�o�[�W�������̕�����
--- COPYRIGHT
	���쌠���̕�����

=end
---------------------------------------------------------------------*/

%module RevocapIO

#ifndef VERSION
namespace kmb{
#define VERSION "RevocapIO version 1.6.05 (2013/3/08)"
#define COPYRIGHT "IIS, The University of Tokyo CISS, Large Scale Assembly, Structural Correspondence, Multi Dynamics Simulator"
}
#endif

%{
#ifdef _MSC_VER
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#endif
#include "MeshDB/kmbMeshData.h"
#include "RevocapIO/kmbTripatchPcmIO.h"
#include "RevocapIO/kmbTetMeshMIO.h"
#include "RevocapIO/kmbFortranIO.h"
#include "RevocapIO/kmbHecmwIO.h"
#include "RevocapIO/kmbFFbIO.h"
#include "RevocapIO/kmbRevocapCouplerIO.h"
#include "RevocapIO/kmbMicroAVSIO.h"
#include "RevocapIO/kmbSTLIO.h"
#include "RevocapIO/kmbRevocapNeutralIO.h"
%}

%include "../MeshDB/kmbTypes.h"
%include "../Geometry/kmbIdTypes.h"
%include "../Common/kmbCommon.h"

/**-----------------------------------------------------------------
=begin
= RevocapIO::TripatchPcmIO
== �T�v
ADVENTURE_Tripatch �� pcm �`���t�@�C���̓ǂݏ���������
== ���\�b�h
--- TripatchPcmIO.new
    �R���X�g���N�^
--- loadFromFile(filename,mesh)
    pcm �t�@�C�� filename �̓��e�� mesh �ɓǂݍ���
--- saveToFile(filename,mesh,packOption=0)
    mesh �̓��e�� filename �� pcm �`���ŏo�͂���
     * packOption = 0 : ���̂܂܏o��
     * packOption = 1 : ���ׂĂ���ɂ܂Ƃ߂ďo��
=end
---------------------------------------------------------------------*/
%include "../RevocapIO/kmbTripatchPcmIO.h"

/**-----------------------------------------------------------------
=begin
= RevocapIO::TetMeshMIO
== �T�v
ADVENTURE_TetMesh �� msh �`���t�@�C���̓ǂݏ���������
4�ʑ̂����łȂ��A6�ʑ̂ɂ��Ă� ADVENTURE �`���̃t�@�C���̓ǂݏ������\
== ���\�b�h
--- TetMeshMIO.new
    �R���X�g���N�^
--- loadFromFile(filename,mesh)
    msh �t�@�C�� filename �̓��e�� mesh �ɓǂݍ���
--- saveToFile(filename,mesh)
    mesh �̓��e�� filename �� msh �`���ŏo�͂���
=end
---------------------------------------------------------------------*/
%include "../RevocapIO/kmbTetMeshMIO.h"

/**-----------------------------------------------------------------
=begin
= RevocapIO::HecmwIO
== �T�v
HEC-MW �� msh �`���Efrs �`���t�@�C���̓ǂݏ���������
== ���\�b�h
--- HecmwIO.new
    �R���X�g���N�^
--- loadFromFile(filename,mesh)
    msh �t�@�C�� filename �̓��e�� mesh �ɓǂݍ���
--- loadFromFRSFile(filename,mesh)
    frs �t�@�C�� filename �̓��e�� mesh �ɓǂݍ���
--- loadFromResFile(filename,mesh)
    res �t�@�C�� filename �̓��e�� mesh �ɓǂݍ���
--- saveToFile(filename,mesh)
    mesh �̓��e�� filename �� msh �`���ŏo�͂���
--- setSolutionType(soltype)
    ��͂̎�ނ�ݒ肷��
    �ݒ�\�Ȓl�́A������ "STATIC" "HEAT" "EIGEN" "DYNAMIC" ��4�ʂ�
    �����ɂ���� msh �`���̃t�@�C���̍ޗ��ݒ�l�̏o�͕��@��؂�ւ���
--- getSolutionType
    ���ݐݒ肳��Ă����͂̎�ނ̕������Ԃ�
--- setDataFlag(flag)
--- setResElementFlag(flag)
--- appendEquationToFile(filename,mesh,master,slave,thresh,outThresh)
    MPC �f�[�^���o�͂���

=end
---------------------------------------------------------------------*/
%include "../RevocapIO/kmbHecmwIO.h"

/**-----------------------------------------------------------------
=begin
= RevocapIO::FFbIO
== �T�v
FrontFlow/blue GF �`���� mesh boun flow �t�@�C���̓ǂݏ���������
== ���\�b�h
--- FFbIO.new
    �R���X�g���N�^
--- loadFromMeshFile(filename,mesh)
    GF �`���� mesh �t�@�C�� filename �̓��e�� mesh �ɓǂݍ���
--- loadFromBoundaryFile(filename,mesh)
    GF �`���� boundary �t�@�C�� filename �̓��e�� mesh �ɓǂݍ���
--- loadPostStep(filename,mesh,step=-1)
    GF �`���� flow �t�@�C�� filename �̓��e�� mesh �ɓǂݍ���
    step ���w�肷�邱�Ƃ��\�B-1 �̎��͊i�[����Ă���ŏ��� step ��ǂ�
--- loadHeader(filename)
    GF �`���̃t�@�C���̃w�b�_���o�͂���
--- saveToMeshFile(filename,mesh)
    mesh �̓��e�� filename �� GF �`���� mesh �t�@�C���ŏo�͂���
    �߂�l�͎l�ʑ̃��f���������� 4�A�Z�ʑ̃��f���������� 6�A�����̏ꍇ�� 8
--- saveToMeshFileVer6(filename,mesh)
    mesh �̓��e�� filename �� FrontFlow/blue ver.6 �� GF �`���� mesh �t�@�C���ŏo�͂���
--- saveToBoundaryFile(filename,mesh)
    mesh �ɒ�`���ꂽ���E������ filename �� GF �`���� boundary �t�@�C���ŏo�͂���
    �W���ȊO�� "CUSTOM" �^�O�̐ߓ_�O���[�v���o�͂���
--- saveToBoundaryFileAcoustic(filename,mesh)
    mesh �ɒ�`���ꂽ���E������ filename �� GF �`���� boundary �t�@�C���ŏo�͂���
    Acoustic �p
=end
---------------------------------------------------------------------*/
%include "../RevocapIO/kmbFFbIO.h"

/**-----------------------------------------------------------------
=begin
= RevocapIO::RevocapCouplerIO
== �T�v
REVOCAP_Coupler �̕����t�@�C���̓ǂݍ���
== ���\�b�h
--- RevocapCouplerIO.new
    �R���X�g���N�^
--- loadFromPartitionFile(filename,mesh)
    REVOCAP_Coupler �̕����t�@�C�� filename �̓��e�� mesh �ɓǂݍ���
    mesh �ɂ� global Id �i�[����
--- loadLocalNodesFromPartitionFile(filename,mesh)
    REVOCAP_Coupler �̕����t�@�C�� filename �̐ߓ_�� mesh �ɓǂݍ���
    mesh �ɂ� localId �Ŋi�[����
    globalId�� Data �Ƃ��� name = "GlobalId" �ŎQ�Ƃ���
=end
---------------------------------------------------------------------*/
%include "../RevocapIO/kmbRevocapCouplerIO.h"

/**-----------------------------------------------------------------
=begin
= RevocapIO::MicroAVSIO
== �T�v
MicroAVS �� UCD �`���̓ǂݏ���
== ���\�b�h
--- MicroAVSIO.new
    �R���X�g���N�^
--- loadFromFile(filename,mesh)
    MicroAVS UCD �`���� filename �� mesh �ɓǂݍ���
--- loadPostFromFile(filename,mesh)
    MicroAVS UCD �`���� filename �̌v�Z���ʃf�[�^������ mesh �ɓǂݍ���
--- saveToFile_V8(filename,mesh)
    mesh �̓��e�� MicroAVS UCD Ver.8 �`���� filename �ɏo�͂���
--- saveToFile(filename,mesh)
    mesh �̓��e�� MicroAVS UCD �`���� filename �ɏo�͂���
--- setReadAsVector3(flag)
    �t�@�C����ǂݍ��ނƂ���3�����v�Z���ʂ��Ȃ����Ɏ����I�Ƀx�N�g���l�Ŋi�[����
=end
---------------------------------------------------------------------*/
%include "../RevocapIO/kmbMicroAVSIO.h"

/**-----------------------------------------------------------------
=begin
= RevocapIO::RevocapNeutralIO
== �T�v
REVOCAP_PrePost �� YAML �`���j���[�g�����t�@�C���̓ǂݏ���
== ���\�b�h
--- RevocapNeutralIO.new
    �R���X�g���N�^
--- loadFromRNFFile(filename,mesh)
    REVOCAP_PrePost RNF �`���̃t�@�C�� filename �� mesh �ɓǂݍ���
--- saveToRNFFile(filename,mesh)
    mesh �̓��e�� REVOCAP_PrePost RNF �`���� filename �ɏo�͂���
=end
---------------------------------------------------------------------*/
%include "../RevocapIO/kmbRevocapNeutralIO.h"

%include "../RevocapIO/kmbSTLIO.h"
