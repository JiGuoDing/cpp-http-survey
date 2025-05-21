# C++ http框架调研

## httplib 框架

只需要添加头文件`httplib.h`到项目中即可使用。

使用简单，但性能相对较差。

```c++
#include "httplib.h"
```

连接串行执行，1000个并发就会出现连接失败的情况。

## Drogon 框架

性能优异

需要先编译源码

```sh
git clone git@github.com:drogonframework/drogon.git
cd drogon
git submodule update --init
mkdir build
cd build
cmake ..
make && sudo make install
```

包含头文件

```c++
#include <drogon/drogon.h>
```

## Seastar 框架

性能优异

需要先编译源码

```sh
sudo ./install-dependencies.sh
./configure.py --mode=release --prefix=/usr/local
sudo ninja -C build/release install
```
