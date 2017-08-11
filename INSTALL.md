# REVOCAP_Mesh のインストール方法

REVOCAP_Refiner のインストール方法については Refiner/INSTALL.md をご覧ください。

## Requirements

- cmake-3.8 以上
- swig (ruby/C++インターフェース生成が有効なもの)
- OpenCASCADE Community Edition
- OpenGL, GLU
- GLEW (Ver.2.0.0 で動作確認済み)
- ruby (Ver.2.3 で動作確認済み)
- boost (Ver.1.61.0 で動作確認済み）

> boost は WITH_TEST=ON の時だけ利用します。

## Quick installation

```txt
% mkdir build
% cd build
% cmake ..
% make -j4
% make install
```

REVOCAP_Refinerだけを構築する場合

```txt
% make -j4 RcapRefiner
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

### MinGW でインストールする場合

```txt
export PATH=/mingw64/bin:$PATH
cmake -G "MinGW Makefiles" ..
mingw32-make
```

### OpenCASCADE を利用しない場合

```txt
% cmake -DWITH_OPENCASCADE=OFF ..
```

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

### Doxygen

予め`doxygen`と`graphviz`をインストールする必要があります。

```txt
% cmake -DWITH_DOC ..
% make doc
% firefox doc/html/index.html
```

で参照することが出来ます。
