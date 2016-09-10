# REVOCAP_Refiner のインストール方法

## Requirements

- cmake-2.8 以上

## Quick installation

```txt
% mkdir build
% cd build
% cmake ../Refiner
% make -j4
% make install
```
## Tips

### インストール先の変更

```txt
% cmake -DCMAKE_INSTALL_PREFIX=$HOME/local ..
```

### Doxygen

予め`doxygen`と`graphviz`をインストールする必要があります。

```txt
% cmake -DBUILD_DOC ..
% make doc
% firefox doc/html/index.html
```

で参照することが出来ます。
