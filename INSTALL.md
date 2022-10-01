# REVOCAP_Mesh のインストール方法

## Requirements

- cmake-3.0 以上
- swig (ruby/C++インターフェース生成が有効なもの)
- OpenCASCADE or OpenCASCADE Community Edition
- OpenGL, GLU
- OpenMP
- GLEW (Ver.2.0.0 で動作確認済み)
- ruby (Ver.2.3 で動作確認済み)
- boost (Ver.1.61.0 で動作確認済み）

> cmake-2.8でもビルドは出来ますが、拡張モジュールの名前が`libXXX.so`になります。
> boost は WITH_TEST=ON の時だけ利用します。

## Quick installation

```txt
mkdir build
cd build
cmake ..
make -j4
make install
```

REVOCAP_Refinerだけを構築する場合

```txt
mkdir build
cd build
cmake -DWITH_EXTENSION=OFF -DWITH_OPENCASCADE=OFF ..
make -j4 RcapRefiner
```

## Tips

### インストール先の変更

```txt
cmake -DCMAKE_INSTALL_PREFIX=$HOME/local ..
```

### ruby拡張モジュールを作成しない場合

```txt
cmake -DWITH_EXTENSION=OFF ..
```

XXXRubyディレクトリはコンパイルの対象から外します。

### MinGW でインストールする場合

```txt
export PATH=/mingw64/bin:$PATH
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
```

### Mac でインストールする場合

Homebrew で環境構築します。

```sh
brew install fortran
brew install ruby
brew install opencascade
brew install swig
brew install boost
```

### OpenCASCADE を利用する場合

```txt
cmake -DWITH_OPENCASCADE=ON ..
```

### OCE を利用する場合

OCE をインストールする際、パスが通っているディレクトリの親ディレク
トリを `CMAKE_INSTALL_PREFIX` に指定すると、自動でパスを見つけてくれます。
`$HOME/local/bin`にパスが通っている場合、OpenCASCADEのインストール先の指定は

 ```txt
cmake -DCMAKE_INSTALL_PREFIX=$HOME/local ..
 ```
 
とすると`REVOCAP_Mesh`の`cmake`スクリプトがライブラリ・ヘッダファイルを自動的に探してくれます。

または、`OCEConfig.cmake`ファイルがある場所を探し、そのディレクトリを環境変数`OCE_DIR`に設定してください。

```txt
find /usr -name OCEConfig.cmake
./lib/x86_64-linux-gnu/OCEConfig.cmake
export OCE_DIR="/usr/lib/x86_64-linux-gnu"
cmake -DWITH_OCE=ON ..
```

### Doxygen

予め`doxygen`と`graphviz`をインストールする必要があります。

```txt
cmake -DBUILD_DOC ..
make doc
firefox doc/html/index.html
```

で参照することが出来ます。

## cmake を利用しない場合

トップディレクトリにある Makefile を使って make コマンドでビルドすることもできますが、
メンテナンスはしていません。Ruby の拡張ライブラリの作成はサポートしていません。

```txt
make Refiner
```