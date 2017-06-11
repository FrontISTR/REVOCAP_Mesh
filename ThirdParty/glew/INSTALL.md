# GLEW のインストール

http://glew.sourceforge.net/ からソースコードをダウンロードしてインストールする。

## ubuntu の場合

```txt
% sudo apt-get install build-essential libxmu-dev libxi-dev libgl-dev libosmesa-dev git
% wget https://sourceforge.net/projects/glew/files/glew/2.0.0/glew-2.0.0.tgz
% tar xvfz glew-2.0.0.tgz
% cd glew-2.0.0
% make
% sudo make install
```
