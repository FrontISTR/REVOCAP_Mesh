# REVOCAP_Mesh のインストール方法

## Requirements

- cmake-3.0 以上
- swig (ruby/C++インターフェース生成が有効なもの)
- OpenCASCADE Community Edition (oce)
- OpenGL, GLU
- GLEW
- ruby

> cmake-2.8でもビルドは出来ますが、拡張モジュールの名前が`libXXX.so`になります。

## Quick installation

```txt
% mkdir build
% cd build
% cmake ..
% make -j4
% make install
```
## Tips

### インストール先の変更

```txt
% cmake -DCMAKE_INSTALL_PREFIX=$HOME/local ..
```

### ruby拡張モジュールを作成しない場合

```txt
% cmake -DWITH_EXTENSION=OFF ..
```

XXXRubyディレクトリはコンパイルの対象から外します。

### OpenCASCADE(oce)のインストール先

OpenCASCADE(oce)をインストールする際、パスが通っているディレクトリの親ディレクトリを `CMAKE_INSTALL_PREFIX` に指定すると、自動でパスを見つけてくれます。

#### 例)

`$HOME/local/bin`にパスが通っている場合、OpenCASCADEのインストール先の指定は

```txt
% cmake -DCMAKE_INSTALL_PREFIX=$HOME/local ..
```

とすると`REVOCAP_Mesh`の`cmake`スクリプトがライブラリ・ヘッダファイルを自動的に探してくれます。
見つからない場合、`OCEConfig.cmake`ファイルがある場所を探し、そのディレクトリを環境変数`OCE_DIR`に設定してください。

```txt
% find /usr -name OCEConfig.cmake
./lib/x86_64-linux-gnu/OCEConfig.cmake
% export OCE_DIR="/usr/lib/x86_64-linux-gnu"
```

