# Orbbec SDK lib

OrbbecSDK lib&driver  由于库文件、深度引擎和驱动文件，都是专用代码，不属于mit协议，因此在放在另外一个单独仓库.

OrbbecSDK lib&driver github仓库链接 : [git@github.com: orbbec/OrbbecSDK-lib.git](git@github.com:orbbec/OrbbecSDK-Lib.git)

### 编译说明

在整个工程编译时cmake会调用cmake/DependentLibDownload.cmake  从Lib&driver github仓库自动下载lib和driver目录文件到本工程lib和misc/driver目录再进行编译。
