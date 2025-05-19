# C++ http框架调研

## httplib 框架

只需要添加头文件`httplib.h`到项目中即可使用。

```c++
#include "httplib.h"
```

## Drogon 框架

需要先编译源码

```shell
git clone git@github.com:drogonframework/drogon.git
cd drogon
git submodule update --init
mkdir build
cd build
cmake ..
make && sudo make install
```

