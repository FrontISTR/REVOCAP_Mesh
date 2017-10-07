================================================================================

  README of REVOCAP_PrePost (Mesh 処理部)

  2011.03.23  version 1.5

  The University of Tokyo, CISS Project
  次世代ものづくりシミュレーションシステム

  お問い合わせ先
    e-mail:  software@ciss.iis.u-tokyo.ac.jp

================================================================================

このファイルは REVOCAP_Mesh-1.5.XX.tgz を展開してできるフォルダとファイルの説明書です。

== フォルダの構造

 +- README.txt             ( このファイル )
 +- Geometry               ( 幾何処理用ライブラリ )
 +- Matrix                 ( 行列処理用ライブラリ )
 +- MeshDB                 ( メッシュ処理用ライブラリ )
 +- MeshDBRuby             ( MeshDB Ruby 拡張ライブラリ作成 )
 +- MeshGL                 ( メッシュ表示用ライブラリ )
 +- MeshGLRuby             ( MeshGL Ruby 拡張ライブラリ作成 )
 +- MeshGen                ( メッシュ生成用ライブラリ )
 +- MeshGenRuby            ( MeshGen Ruby 拡張ライブラリ作成 )
 +- RevocapIO              ( メッシュ入出力用ライブラリ )
 +- RevocapIORuby          ( RevocapIO Ruby 拡張ライブラリ作成 )
 +- Shape                  ( CAD 形状処理用ライブラリ )
 +- ShapeRuby              ( Shape Ruby 拡張ライブラリ作成 )
 +- Makefile               ( 実行体作成用のメイクファイル )
 +- MakefileRuby.in        ( Ruby 拡張ライブラリ作成用のメイクファイル )
 +- OPTIONS                ( Makefile のオプション指定用のファイル )
 +- VERSION                ( バージョン記載ファイル )
 +- Documents              ( ドキュメント類 )

ご使用の前に、使用許諾条件に関する文書
Documents/CISS_lic_agree2011_ja.pdf (英語版Documents/CISS_lic_agree2011_en.pdf)
の内容をご確認ください。

Linux 環境でソースからビルドする場合

 * g++ gcc
 * Ruby (ruby,ruby-dev,rubygemsなどビルドに必要なパッケージ)
 * OpenCASCADE
 * swig

をあらかじめインストールしておいてください。OPTIONS ファイルにコンパイラ等の
オプション設定の情報を記入してください。

REVOCAP_PrePost で使う場合

make コマンドで RubyExtension ターゲットを実行してください。
RevocapMesh.so RevocapGL.so RevocapMeshGen.so RevocapIO.so RevocapShape.so
が生成できたら、REVOCAP_PrePost の lib/{RUBY_PLATFORM} ディレクトリにコピーしてください。
RUBY_PLATFORM は i486-linux i386-mswin32 などの Ruby の実行環境を表す文字列です。

詳細はマニュアルをご覧ください。
