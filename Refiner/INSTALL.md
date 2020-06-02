# REVOCAP_Refiner のインストール方法

## Requirements

- cmake-3.0 以上

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
% cmake -DCMAKE_INSTALL_PREFIX=$HOME/local ../Refiner
```

### MinGW でインストールする場合

```txt
export PATH=/mingw64/bin:$PATH
cmake -G "MinGW Makefiles" ..
mingw32-make
```

### Doxygen

予め`doxygen`と`graphviz`をインストールする必要があります。

```txt
% cmake -DBUILD_DOC=ON ..
% make doc
% firefox doc/html/index.html
```

で参照することが出来ます。
