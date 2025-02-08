# openJDK8
JDK源码学习

JDK 镜像版本-openjdk-8u40-src-b25-10_feb_2015

[openjdk-8u40](openjdk-8u40)目录结构

| 一级目录 | 二级目录 | 三级目录 | 多级目录 | 描述 |
|:-------|:------|:----:|:-----:|---------:|
| [common](openjdk-8u40/common) | - | - | - | |
| [corba](openjdk-8u40/corba) | - | - | - | CORBA的实现 |
| [hotspot](openjdk-8u40/hotspot) | - | - | - | hotspot虚拟机实现 |
| [jaxp](openjdk-8u40/jaxp) | - | - | - | JAXP的实现 提供处理XML的API |
| [jaxws](openjdk-8u40/jaxws) | - | - | - | JAX-WS的实现 提供WebService的API |
| [jdk](openjdk-8u40/jdk) | - | - | - | Java类库实现 |
| ├─ | [classes](openjdk-8u40/jdk/src/share/classes) | - | - | Java类库 |
| │ | ├─ | [com](openjdk-8u40/jdk/src/share/classes/com) | - | 不关注 |
| │ | ├─ | [java](openjdk-8u40/jdk/src/share/classes/java) | - | 重点关注 |
| │ | │ | ├─ | [io](openjdk-8u40/jdk/src/share/classes/java/io) | 文件流、网络流 |
| │ | │ | ├─ | [lang](openjdk-8u40/jdk/src/share/classes/java/lang) | 基础语法和包装类型 |
| │ | │ | ├─ | [math](openjdk-8u40/jdk/src/share/classes/java/math) | 函数 |
| │ | │ | ├─ | [net](openjdk-8u40/jdk/src/share/classes/java/net) | 网络通讯 |
| │ | │ | ├─ | [nio](openjdk-8u40/jdk/src/share/classes/java/nio) | 文件流、网络流 |
| │ | │ | ├─ | [time](openjdk-8u40/jdk/src/share/classes/java/time) | 日期和时间 |
| │ | │ | └─ | [util](openjdk-8u40/jdk/src/share/classes/java/util) | 容器、集合、多线程、工具 |
| │ | ├─ | [javax](openjdk-8u40/jdk/src/share/classes/javax) | - | 不关注 |
| │ | ├─ | [jdk](openjdk-8u40/jdk/src/share/classes/jdk) | - | 不关注 |
| │ | ├─ | [org](openjdk-8u40/jdk/src/share/classes/org) | - | 不关注 |
| │ | └─ | [sun](openjdk-8u40/jdk/src/share/classes/sun) | - | 不关注 |
| [langtools](openjdk-8u40/langtools) | - | - | - | 各种工具 |
| [make](openjdk-8u40/make) | - | - | - | - |
| [nashorn](openjdk-8u40/nashorn) | - | - | - | JavaScript的实现 |
| [test](openjdk-8u40/test) | - | - | - | - |




- [Ubuntu22环境下编译OpenJDK12源码](https://blog.csdn.net/qq_25825005/article/details/127162939)
- [JDK源码下载Github](https://github.com/taotao1024/openJDK8.git)
- [环境构建](https://www.bilibili.com/video/BV1V7411U78L/)
- ![IDEA设置](doc/images/JDK-编译/JDK源码路径设置.png)
- ![IDEA设置](doc/images/JDK-编译/JDK调试设置.png)
