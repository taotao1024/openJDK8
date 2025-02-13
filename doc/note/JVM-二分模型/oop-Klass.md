# 二分模型
HotSpot采用oop-Klass模型表示Java的对象和类。
oop（ordinary object pointer）指普通的对象指针，Klass表示对象的具体类型。

为何要设计一个一分为二的对象模型呢？
这是因为HotSpot的设计者不想让每个对象中都含有一个vtable（虚函数表），所以就把对象模型拆成Klass和oop。
其中，oop中不含有任何虚函数，自然就没有虚函数表，而Klass中含有虚函数表，可以进行方法的分发。

## Klass [klass.hpp](../../../openjdk-8u40/hotspot/src/share/vm/oops/klass.hpp)

![Klass的继承体系.png](../../images/JVM-%E4%BA%8C%E5%88%86%E6%A8%A1%E5%9E%8B/Klass%E7%9A%84%E7%BB%A7%E6%89%BF%E4%BD%93%E7%B3%BB.png)

- 一个C++的Klass类实例表示一个Java类的元数据（相当于java.lang.Class对象），主要提供两个功能： 
  - 1、实现Java语言层面的类；
  - 2、提供对多态方法的支持，即通过vtbl指针实现多态。

在HotSpot中，Java对象使用oop实例来表示，不提供任何虚函数的功能。
oop实例保存了对应Klass的指针，通过Klass完成所有的方法调用并获取类型信息，Klass基于C++的虚函数提供对Java多态的支持。

- Klass子类如下
  - InstanceKlass
    - InstanceRefKlass
      - 表示java.lang.ref.Reference类 需要使用C++类InstanceRefKlass的实例来表示
    - InstanceMirrorKlass
      - 表示java.lang.Class类的InstanceMirrorKlass
    - InstanceClassLoaderKlass
      - 表示java.lang.ClassLoader类的InstanceClassLoaderKlass
      - 没有添加新的字段，但增加了新的oop遍历方法，在垃圾回收阶段遍历类加载器加载的所有类来标记引用的所有对象
  - ArrayKlass
    - 数组类没有对应的Class文件，是由虚拟机直接创建的。 
    - TypeArrayKlass [typeArrayKlass.cpp](../../../openjdk-8u40/hotspot/src/share/vm/oops/typeArrayKlass.cpp)
      - 表示组件类型是java基本类型
      - HotSpot VM在初始化时就会创建Java中8个基本类型的一维数组实 例TypeArrayKlass。
    - ObjArrayKlass [objArrayKlass.cpp](../../../openjdk-8u40/hotspot/src/share/vm/oops/objArrayKlass.cpp)
      - 表示组件类型是对象类型
## Oop [oop.cpp](../../../openjdk-8u40/hotspot/src/share/vm/oops/oop.cpp)
 - oopDesc类的别名为oop，因此HotSpot VM中一般使用oop表示oopDesc类型。
 - Java对象用oop来表示，在Java创建对象的时候创建。
 - 也就是说，在Java应用程序运行过程中每创建一个Java对象，在HotSpot VM内部都会创建一个oop实例来表示Java对象。

![oopDesc类的继承关系.png](../../images/JVM-%E4%BA%8C%E5%88%86%E6%A8%A1%E5%9E%8B/oopDesc%E7%B1%BB%E7%9A%84%E7%BB%A7%E6%89%BF%E5%85%B3%E7%B3%BB.png)

- oopDesc子类如下 [oop.hpp](../../../openjdk-8u40/hotspot/src/share/vm/oops/oop.hpp)
  - instanceOopDesc [instanceOop.cpp](../../../openjdk-8u40/hotspot/src/share/vm/oops/instanceOop.cpp)
    - instanceOopDesc类的实例表示除数组对象外的其他对象
  - markOopDesk [markOop.cpp](../../../openjdk-8u40/hotspot/src/share/vm/oops/markOop.cpp)
    - markOopDesc类的实例并不能表示一个具体的Java对象，而是通过一个字的各个位来表示Java对象的头信息。
    - 包含的信息有哈希码、GC分代年龄、偏向锁标记、线程持有的锁、偏向线程ID和偏向时间戳 等。
    - 因此表示普通Java类对象的instanceOopDesc实例和表示数组对象的objArrayOopDesc与typeArrayOopDesc实例都含有markOopDesc实例。
  - arrayOopDesc
    - objArrayOopDesc
      - 组件类型为基本类型、二维及二维以上的数组
    - typeArrayOopDesc
      - 组件类型为对象类型