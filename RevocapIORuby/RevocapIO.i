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

RevocapIO とは RevocapMesh への入出力を定義したモジュールである。

==モジュール定数

((<VERSION>))
((<COPYRIGHT>))

== モジュール定数
以下の定数は RevocapGL モジュールで定義されている。
--- VERSION
	バージョン情報の文字列
--- COPYRIGHT
	著作権情報の文字列

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
== 概要
ADVENTURE_Tripatch の pcm 形式ファイルの読み書きをする
== メソッド
--- TripatchPcmIO.new
    コンストラクタ
--- loadFromFile(filename,mesh)
    pcm ファイル filename の内容を mesh に読み込む
--- saveToFile(filename,mesh,packOption=0)
    mesh の内容を filename に pcm 形式で出力する
     * packOption = 0 : そのまま出力
     * packOption = 1 : すべてを一つにまとめて出力
=end
---------------------------------------------------------------------*/
%include "../RevocapIO/kmbTripatchPcmIO.h"

/**-----------------------------------------------------------------
=begin
= RevocapIO::TetMeshMIO
== 概要
ADVENTURE_TetMesh の msh 形式ファイルの読み書きをする
4面体だけでなく、6面体についても ADVENTURE 形式のファイルの読み書きが可能
== メソッド
--- TetMeshMIO.new
    コンストラクタ
--- loadFromFile(filename,mesh)
    msh ファイル filename の内容を mesh に読み込む
--- saveToFile(filename,mesh)
    mesh の内容を filename に msh 形式で出力する
=end
---------------------------------------------------------------------*/
%include "../RevocapIO/kmbTetMeshMIO.h"

/**-----------------------------------------------------------------
=begin
= RevocapIO::HecmwIO
== 概要
HEC-MW の msh 形式・frs 形式ファイルの読み書きをする
== メソッド
--- HecmwIO.new
    コンストラクタ
--- loadFromFile(filename,mesh)
    msh ファイル filename の内容を mesh に読み込む
--- loadFromFRSFile(filename,mesh)
    frs ファイル filename の内容を mesh に読み込む
--- loadFromResFile(filename,mesh)
    res ファイル filename の内容を mesh に読み込む
--- saveToFile(filename,mesh)
    mesh の内容を filename に msh 形式で出力する
--- setSolutionType(soltype)
    解析の種類を設定する
    設定可能な値は、文字列 "STATIC" "HEAT" "EIGEN" "DYNAMIC" の4通り
    これらによって msh 形式のファイルの材料設定値の出力方法を切り替える
--- getSolutionType
    現在設定されている解析の種類の文字列を返す
--- setDataFlag(flag)
--- setResElementFlag(flag)
--- appendEquationToFile(filename,mesh,master,slave,thresh,outThresh)
    MPC データを出力する

=end
---------------------------------------------------------------------*/
%include "../RevocapIO/kmbHecmwIO.h"

/**-----------------------------------------------------------------
=begin
= RevocapIO::FFbIO
== 概要
FrontFlow/blue GF 形式の mesh boun flow ファイルの読み書きをする
== メソッド
--- FFbIO.new
    コンストラクタ
--- loadFromMeshFile(filename,mesh)
    GF 形式の mesh ファイル filename の内容を mesh に読み込む
--- loadFromBoundaryFile(filename,mesh)
    GF 形式の boundary ファイル filename の内容を mesh に読み込む
--- loadPostStep(filename,mesh,step=-1)
    GF 形式の flow ファイル filename の内容を mesh に読み込む
    step を指定することも可能。-1 の時は格納されている最初の step を読む
--- loadHeader(filename)
    GF 形式のファイルのヘッダを出力する
--- saveToMeshFile(filename,mesh)
    mesh の内容を filename に GF 形式の mesh ファイルで出力する
    戻り値は四面体モデルだったら 4、六面体モデルだったら 6、混合の場合は 8
--- saveToMeshFileVer6(filename,mesh)
    mesh の内容を filename に FrontFlow/blue ver.6 の GF 形式の mesh ファイルで出力する
--- saveToBoundaryFile(filename,mesh)
    mesh に定義された境界条件を filename に GF 形式の boundary ファイルで出力する
    標準以外に "CUSTOM" タグの節点グループも出力する
--- saveToBoundaryFileAcoustic(filename,mesh)
    mesh に定義された境界条件を filename に GF 形式の boundary ファイルで出力する
    Acoustic 用
=end
---------------------------------------------------------------------*/
%include "../RevocapIO/kmbFFbIO.h"

/**-----------------------------------------------------------------
=begin
= RevocapIO::RevocapCouplerIO
== 概要
REVOCAP_Coupler の分割ファイルの読み込み
== メソッド
--- RevocapCouplerIO.new
    コンストラクタ
--- loadFromPartitionFile(filename,mesh)
    REVOCAP_Coupler の分割ファイル filename の内容を mesh に読み込む
    mesh には global Id 格納する
--- loadLocalNodesFromPartitionFile(filename,mesh)
    REVOCAP_Coupler の分割ファイル filename の節点を mesh に読み込む
    mesh には localId で格納する
    globalIdは Data として name = "GlobalId" で参照する
=end
---------------------------------------------------------------------*/
%include "../RevocapIO/kmbRevocapCouplerIO.h"

/**-----------------------------------------------------------------
=begin
= RevocapIO::MicroAVSIO
== 概要
MicroAVS の UCD 形式の読み書き
== メソッド
--- MicroAVSIO.new
    コンストラクタ
--- loadFromFile(filename,mesh)
    MicroAVS UCD 形式の filename を mesh に読み込む
--- loadPostFromFile(filename,mesh)
    MicroAVS UCD 形式の filename の計算結果データだけを mesh に読み込む
--- saveToFile_V8(filename,mesh)
    mesh の内容を MicroAVS UCD Ver.8 形式で filename に出力する
--- saveToFile(filename,mesh)
    mesh の内容を MicroAVS UCD 形式で filename に出力する
--- setReadAsVector3(flag)
    ファイルを読み込むときに3つしか計算結果がない時に自動的にベクトル値で格納する
=end
---------------------------------------------------------------------*/
%include "../RevocapIO/kmbMicroAVSIO.h"

/**-----------------------------------------------------------------
=begin
= RevocapIO::RevocapNeutralIO
== 概要
REVOCAP_PrePost の YAML 形式ニュートラルファイルの読み書き
== メソッド
--- RevocapNeutralIO.new
    コンストラクタ
--- loadFromRNFFile(filename,mesh)
    REVOCAP_PrePost RNF 形式のファイル filename を mesh に読み込む
--- saveToRNFFile(filename,mesh)
    mesh の内容を REVOCAP_PrePost RNF 形式で filename に出力する
=end
---------------------------------------------------------------------*/
%include "../RevocapIO/kmbRevocapNeutralIO.h"

%include "../RevocapIO/kmbSTLIO.h"
