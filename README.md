# C++ http框架调研

## httplib 框架

只需要添加头文件`httplib.h`到项目中即可使用。

使用简单，但性能相对较差。

```c++
#include "httplib.h"
```

| 连续请求个数 | 单个请求数据量 | 平均传输时间 | 吞吐量      |
|--------|---------|--------|----------|
| 10     | 30MB    | 4523ms | 6.63MB/s |
| 100    | 30MB    | 3510ms | 8.55MB/s |
| 1000   | 30MB    |        |          |

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

| 连续请求个数 | 单个请求数据量 | 平均传输时间 | 吞吐量      |
|--------|---------|--------|----------|
| 10     | 30MB    | 3912ms | 7.67MB/s |
| 100    | 30MB    | 3548ms | 8.45MB/s |
| 1000   | 30MB    |        |          |


## Seastar 框架

性能优异

需要先编译源码

```sh
sudo ./install-dependencies.sh
./configure.py --mode=release --prefix=/usr/local
sudo ninja -C build/release install
```
