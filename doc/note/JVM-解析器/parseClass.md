# 解析
 - 类的加载不仅仅是找到类对应的Class文件，更重要的是解析出Class文件中包含的信息。
   然后将其转换为HotSpot_VM的内部表示方式(klass)，这虚拟机在运行的过程中才能方便的操作。

## 类的解析

Class文件格式
 - 每个Class文件都对应唯一一个类或接口的定义信息，但类或接口不一定
   定义在Class文件里（类或接口通过类加载器直接生成）。
 - 每个Class文件都可以看作是由字节流组成的，可按照1、2、4、8个字节(U1、U2、U4、U8)为单位进行读取。
 - 多字节数据项是按照Big-Endian的顺序进行存储的。
 - 项item用来描述类结构格式的内容，在Class文件中，各项按照严格的顺序连续存放，各项之
   间无任何填充或分割符。
 - Class文件格式采用一种类似于C语言结构体的伪结构来存储数据。

Class文件数据结构如下
 - 无符号数：
   - 基本数据类型，用u1、u2、u4和u8分别
     代表1个字节、2个字节、4个字节和8个字节的无符
     号数，可用来描述数字、索引引用、数量值和UTF
     8编码构成的字符串值。
 - 表：
   - 多个无符号数或者其他表作为数据项构成的复
     合数据类型，以“_info”结尾，用于描述有层次
     关系的复合结构的数据，整个Class文件本质上是
     一张表。
## 类方法解析

HotSpot VM通过Method与ConstMethod类保存方法的元信息。
 - Method用来保存方法中的一些常见信息，如运行时的解释入口和编译入口。
   - method没有子类，继承链如下：Method、Metadata、MetaspaceObj
 - ConstMethod用来保存方法中的不可变信息，如Java方法的字节码。

Method [method.cpp](../../../openjdk-8u40/hotspot/src/share/vm/oops/method.cpp)
 - Method实例表示一个Java方法，因为一个应用有
   成千上万个方法，所以保证Method类在内存中的布局
   紧凑非常重要。为了方便回收垃圾，Method把所有的
   指针变量和方法都放在了Method内存布局的前面。
 - Java方法本身的不可变数据如字节码等用
   ConstMethod表示，可变数据如Profile统计的性能数
   据等用MethodData表示，它们都可以在Method中通过
   指针访问
 - 如果是本地方法，Method实例的内存布局的最后
   是native_function和signature_handler属性，按照
   解释器的要求，这两个属性必须在固定的偏移处。

ConstMethod [constMethod.cpp](../../../openjdk-8u40/hotspot/src/share/vm/oops/constMethod.cpp)
 - 保存方法中不可变部分的信息，如方法的字节码和方法参数的大小等。