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

## Oop [oop.cpp](../../../openjdk-8u40/hotspot/src/share/vm/oops/oop.cpp)
