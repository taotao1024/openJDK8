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

