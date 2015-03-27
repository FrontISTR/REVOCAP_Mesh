================================================================================

  README of REVOCAP_PrePost (Mesh ������)

  2011.03.23  version 1.5

  The University of Tokyo, CISS Project
  ��������̂Â���V�~�����[�V�����V�X�e��

  ���₢���킹��
    e-mail:  software@ciss.iis.u-tokyo.ac.jp

================================================================================

���̃t�@�C���� REVOCAP_Mesh-1.5.XX.tgz ��W�J���Ăł���t�H���_�ƃt�@�C���̐������ł��B

== �t�H���_�̍\��

 +- README.txt             ( ���̃t�@�C�� )
 +- Geometry               ( �􉽏����p���C�u���� )
 +- Matrix                 ( �s�񏈗��p���C�u���� )
 +- MeshDB                 ( ���b�V�������p���C�u���� )
 +- MeshDBRuby             ( MeshDB Ruby �g�����C�u�����쐬 )
 +- MeshGL                 ( ���b�V���\���p���C�u���� )
 +- MeshGLRuby             ( MeshGL Ruby �g�����C�u�����쐬 )
 +- MeshGen                ( ���b�V�������p���C�u���� )
 +- MeshGenRuby            ( MeshGen Ruby �g�����C�u�����쐬 )
 +- RevocapIO              ( ���b�V�����o�͗p���C�u���� )
 +- RevocapIORuby          ( RevocapIO Ruby �g�����C�u�����쐬 )
 +- Shape                  ( CAD �`�󏈗��p���C�u���� )
 +- ShapeRuby              ( Shape Ruby �g�����C�u�����쐬 )
 +- Makefile               ( ���s�̍쐬�p�̃��C�N�t�@�C�� )
 +- MakefileRuby.in        ( Ruby �g�����C�u�����쐬�p�̃��C�N�t�@�C�� )
 +- OPTIONS                ( Makefile �̃I�v�V�����w��p�̃t�@�C�� )
 +- VERSION                ( �o�[�W�����L�ڃt�@�C�� )
 +- Documents              ( �h�L�������g�� )

���g�p�̑O�ɁA�g�p���������Ɋւ��镶��
Documents/CISS_lic_agree2011_ja.pdf (�p���Documents/CISS_lic_agree2011_en.pdf)
�̓��e�����m�F���������B

Linux ���Ń\�[�X����r���h����ꍇ

 * g++ gcc
 * Ruby (ruby,ruby-dev,rubygems�Ȃǃr���h�ɕK�v�ȃp�b�P�[�W)
 * OpenCASCADE
 * swig

�����炩���߃C���X�g�[�����Ă����Ă��������BOPTIONS �t�@�C���ɃR���p�C������
�I�v�V�����ݒ�̏����L�����Ă��������B

REVOCAP_PrePost �Ŏg���ꍇ

make �R�}���h�� RubyExtension �^�[�Q�b�g�����s���Ă��������B
RevocapMesh.so RevocapGL.so RevocapMeshGen.so RevocapIO.so RevocapShape.so
�������ł�����AREVOCAP_PrePost �� lib/{RUBY_PLATFORM} �f�B���N�g���ɃR�s�[���Ă��������B
RUBY_PLATFORM �� i486-linux i386-mswin32 �Ȃǂ� Ruby �̎��s����\��������ł��B

�ڍׂ̓}�j���A�����������������B
