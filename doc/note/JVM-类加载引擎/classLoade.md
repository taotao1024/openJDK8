# 类的加载
 - 类的加载就是将javac编译的.class文件加载到HotSpot_VM内存中。
 - 通过Klass和ConstantPool等实例保存Class文件中的元数据信息，方便虚拟机运行Java方法，并执行反射等操作。
 - 被HotSpot VM装载后都以InstanceKlass实例表示（其实还可能是更具体的InstanceRefKlass、InstanceMirrorKlass
   和InstanceClassLoaderKlass实例）。主要的类加载器有引导类加载器/启动类加载器（Bootstrap ClassLoader）、
   扩展类加载器（Extension ClassLoader）、应用类加载器/系统类加载器（Application ClassLoader）。
 - 暂定

## 类加载器  
 - 引导类加载器 [classLoader.cpp](../../../openjdk-8u40/hotspot/src/share/vm/classfile/classLoader.cpp)
   - 引导类加载器由ClassLoader类实现，这个ClassLoader类是用C++语言编写的，负责将<JAVA_HOME>/lib目录、
     -Xbootclasspath选项指定的目录和系统属性sun.boot.class.path指定的目录下的核心类库加载到内存中。
 - 扩展类加载器 [Launcher.java](../../../openjdk-8u40/jdk/src/share/classes/sun/misc/Launcher.java)
   - 扩展类加载器由sun.misc.Launcher$ExtClassLoader类实现，负责将<JAVA_HOME >/lib/ext目录或者由系统变量
     Djava.ext.dir指定的目录中的类库加载到内存中。
   - 用Java语言编写的扩展类加载器的实现代码
 - 应用类加载器 [Launcher.java](../../../openjdk-8u40/jdk/src/share/classes/sun/misc/Launcher.java)
   - 应用类加载器由sun.misc.Launcher$AppClassLoader类实现，负责将系统环境变量-classpath、-cp和系统属性
     java.class.path指定的路径下的类库加载到内存中。
   - 用Java语言编写的扩展类加载器的实现代码
 - **类加载器实例化**
   - HotSpot_VM在启动时会初始化一个重要的变量 [systemDictionary.cpp](../../../openjdk-8u40/hotspot/src/share/vm/classfile/systemDictionary.cpp)
   - ```_java_system_loader```属性用于在```compute_java_system_loader()```函数中进行初始化，
     - 其中 HotSpot调用Java静态方法ClassLoader#getSystemClassLoader()方法，获取到应用类加载器、扩展类加载器。
     - 引导类加载器 由C++加载。
   - 调用链路如下：
    ```angular2html
    JavaMain()                                          # java.c
    InitializeJVM()                                     # java.c
    JNI_CreateJavaVM()                                  # jni.cpp
    Threads::create_vm()                                # thread.cpp
    SystemDictionary::compute_java_system_loader()      # systemDictionary.cpp
    ```
   - 类加载流程(java -> JNI -> C -> C++)
     - Launcher#loadClass(java) -> ClassLoader#ClassLoader(java) -> ClassLoader#findLoadedClass(java) 
       -> ClassLoader #native.findLoadedClass0(java) -> JNICALL Java_java_lang_ClassLoader_findLoadedClass0(c) 
       -> JVM_FindLoadedClass(c++) -> SystemDictionary::find_instance_or_array_klass(C++) -> Dictionary::find(C++)
# 触发类的装载

## 下面的情况下会导致类初始化，因此必须在发生这5种情况之前对类进行加载。
 - 当虚拟机启动时加载主类。
 - 使用java.lang.reflect包的方法对类进行反射调用时，如果类还没有初始化，则需要进行初始化。
 - new一个类的对象，调用类的静态成员（除了由final修饰的常量外）和静态方法，无论是解析执行还
   是编译执行的情况下，都会在处理new、getstatic、putstatic、invokestatic字节码指令时对
   类进行初始化。在第9章中会介绍使用new字节码指令创建对象的过程，其中就会有触发类装载的逻辑判断。
 - 当初始化一个类时，如果其父类没有被初始化，则先初始化其父类。后续在介绍函数
   InstanceKlass::initialize_impl()时会看到这个判断逻辑。
 - 在使用JDK 7的动态语言支持时，如果一个java.lang.invoke.MethodHandle对象最后的解析
   结果是REF_getStatic、REF_putStatic、REF_invokeStatic的方法句柄，并且这个方法句柄
   所对应的类没有进行初始化，则需要先进行初始化。
## 类的装载
 - ClassLoader.loadClass()
   - loadClass()方法只是将Class文件装载到HotSpot VM中
 - java.lang.Class.forName()
   - forName()方法会完成类的装载、链接和初始化过程。
 