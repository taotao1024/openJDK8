# 类的连接与初始化
 - **类的生命周期** 可以分为5个阶段，分别为加载、连接、初始化、使用和卸载。
 - **类的加载过程** 包括加载、连接和初始化三个阶段。
   - 加载：功将Class文件中的元数据信息转换成了C++内部的表示形式。
   - 连接：类的连接包括对类的验证、准备、解释。
 
## 连接
在连接类之前要保证已经对类进行了解析，如果类没有被装载，那么最终会调用
ClassFileParser::parseClassFile()函数装载类。然后调用
InstanceKlass::initialize()函数进行类的连接。
```c++
static void initialize_class(Symbol* class_name, TRAPS) {
  // 类的初始化前，首先要调用SystemDictionary::resolve_or_fail()函数以保证类被正确装载。
  Klass* klass = SystemDictionary::resolve_or_fail(class_name, true, CHECK);
  // 类的初始化
  InstanceKlass::cast(klass)->initialize(CHECK);
}
```

## 类的验证
类在连接过程中会涉及验证。HotSpot VM会遵守Java虚拟机的规范，对Class文件中包含的信息进行合
法性验证，以保证HotSpot VM的安全。
 - 文件格式验证：包括魔数和版本号等；
 - 元数据验证：对程序进行语义分析，如是否有父
类，是否继承了不被继承的类，是否实现了父类或
者接口中所有要求实现的方法；
 - 字节码验证：指令级别的语义验证，如跳转指令不
会跳转到方法体以外的代码上；
 - 符号引用验证：符号引用转化为直接引用的时候，
可以看作对类自身以外的信息进行匹配验证，如通
过全限定名是否能找到对应的类等。