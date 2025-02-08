# openJDK8
JDK源码学习

JDK 镜像版本-openjdk-8u40-src-b25-10_feb_2015

# [openjdk-8u40](openjdk-8u40)目录结构

| 一级目录 | 二级目录 | 三级目录 | 多级目录 | 描述 |
|:-------|:------|:----:|:-----:|:---------------------------------------------------------------|
| [common](openjdk-8u40/common) | - | - | - | |
| [corba](openjdk-8u40/corba) | - | - | - | CORBA的实现 |
| [hotspot](openjdk-8u40/hotspot) | - | - | - | hotspot虚拟机实现 |
| ├─ | [agent](openjdk-8u40/hotspot/agent) | - | - | Serviceability Agent的客户端实现 |
| ├─ | [make](openjdk-8u40/hotspot/make) | - | - | 编译HotSpot项目的各种配置文件 |
| ├─ | [src](openjdk-8u40/hotspot/src) | - | - | hotspot源代码 |
| │ | ├─ | [cpu](openjdk-8u40/hotspot/src/cpu) | - | 依赖具体处理器架构的代码有：sparc、x86、zero |
| │ | ├─ | [os](openjdk-8u40/hotspot/src/os) | - | 依赖操作系统的代码如：基于Linux内核的操作系统、基于UNIX的操作系统(POSIX)、以及Windows和Solaris |
| │ | ├─ | [os_cpu](openjdk-8u40/hotspot/src/os_cpu) | - | 依赖操作系统和处理器架构的代码如:linux_x86 |
| │ | └─ | [share](openjdk-8u40/hotspot/src/share).[vm](openjdk-8u40/hotspot/src/share/vm) | - | 独立于操作系统和处理器架构的代码 |
| │ | └─ | [share](openjdk-8u40/hotspot/src/share).[tools](openjdk-8u40/hotspot/src/share/tools) | - | 独立于操作系统和处理器架构的代码 |
| │ | | └─ | [adlc](openjdk-8u40/hotspot/src/share/vm/adlc) | 平台描述文件 |
| │ | | └─ | [asm](openjdk-8u40/hotspot/src/share/vm/asm) | 编译器 |
| │ | | └─ | [c1](openjdk-8u40/hotspot/src/share/vm/c1) | C1编译器、既Client编译器 |
| │ | | └─ | [ci](openjdk-8u40/hotspot/src/share/vm/ci) | 动态编译器 |
| │ | | └─ | [classfile](openjdk-8u40/hotspot/src/share/vm/classfile) | Clas文件解析和类的链接等 |
| │ | | └─ | [code](openjdk-8u40/hotspot/src/share/vm/code) | 生成机器码 |
| │ | | └─ | [compiler](openjdk-8u40/hotspot/src/share/vm/compiler) | 调用动态编译器的接口 |
| │ | | └─ | [gc_implementation](openjdk-8u40/hotspot/src/share/vm/gc_implementation) | 垃圾收集器和具体实现 |
| │ | | └─ | [gc_interface](openjdk-8u40/hotspot/src/share/vm/gc_interface) | GC接口 |
| │ | | └─ | [interpreter](openjdk-8u40/hotspot/src/share/vm/interpreter) | 解释器 |
| │ | | └─ | [libadt](openjdk-8u40/hotspot/src/share/vm/libadt) | 抽象数据对象 |
| │ | | └─ | [memory](openjdk-8u40/hotspot/src/share/vm/memory) | 内存管理 |
| │ | | └─ | [oops](openjdk-8u40/hotspot/src/share/vm/oops) | JVM内部对象 |
| │ | | └─ | [opto](openjdk-8u40/hotspot/src/share/vm/opto) | C2编译器、既Server编译器 |
| │ | | └─ | [precompiled](openjdk-8u40/hotspot/src/share/vm/precompiled) | 核心代码 |
| │ | | └─ | [prims](openjdk-8u40/hotspot/src/share/vm/prims) | HotSpot VM对外接口 |
| │ | | └─ | [runtime](openjdk-8u40/hotspot/src/share/vm/runtime) | 运行时数据区相关代码 |
| │ | | └─ | [services](openjdk-8u40/hotspot/src/share/vm/services) | JMX接口 |
| │ | | └─ | [shark](openjdk-8u40/hotspot/src/share/vm/shark) |  |
| │ | | └─ | [trace](openjdk-8u40/hotspot/src/share/vm/trace) |  |
| │ | | └─ | [utilities](openjdk-8u40/hotspot/src/share/vm/utilities) | 内部工具类和公共函数 |
| └─ | [test](openjdk-8u40/hotspot/test) | - | - | hotspot单元测试用例 |
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

# HotSpotVM 启动和编译

## 启动
HotSpot VM一般通过java.exe或javaw.exe调 [main.c](openjdk-8u40/jdk/src/share/bin/main.c)#main()函数启动。main()函数的调用链如下
```angular2html
main()                      # main.c
JLI_Launch()                # java.c 
JVMInit()                   # java_md_solinux.c
ContinueInNewThread()       # java.c
ContinueInNewThread0()      # java_md_solinux.c
pthread_join()              # pthread_join.c
```
执行main()函数的线程最终会调用pthread_join()函数进行阻塞，由另外一个线程调用JavaMain()函数去执行Java程序的main()方法。
 - [main.c](openjdk-8u40/jdk/src/share/bin/main.c)#main()
   - 项目入口
 - [java.c](openjdk-8u40/jdk/src/share/bin/java.c)#JLI_Launch()
   - libjvm.so的加载、参数解析、Classpath的获取和设置、系统属性设置、JVM初始化等。
 - [java_md_macosx.c](openjdk-8u40/jdk/src/macosx/bin/java_md_macosx.c)#JVMInit()
 - [java.c](openjdk-8u40/jdk/src/share/bin/java.c)#ContinueInNewThread
   - 调用ContinueInNewThread0()函数创建一个JVM实例
 - [java_md_macosx.c](openjdk-8u40/jdk/src/macosx/bin/java_md_macosx.c)#ContinueInNewThread0()
   - 在Linux系统中，创建一个pthread_t线程， 然后使用这个新创建的线程执行JavaMain()函数。

## 编译
- [Ubuntu22环境下编译OpenJDK12源码](https://blog.csdn.net/qq_25825005/article/details/127162939)
- [JDK源码下载Github](https://github.com/taotao1024/openJDK8.git)
- [环境构建](https://www.bilibili.com/video/BV1V7411U78L/)
- ![IDEA设置](doc/images/JDK-编译/JDK源码路径设置.png)
- ![IDEA设置](doc/images/JDK-编译/JDK调试设置.png)