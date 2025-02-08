package com.taotao.mock.jdk.classLoad;

/**
 * 1、类加载子系统
 * 2、内存模块
 * 3、执行引擎
 * 4、垃圾收集器
 * 5、JIT 热点代码缓存
 * <p>
 * {@link sun.jvm.hotspot.oops.Klass}
 * {@link openjdk-8u40/hotspot/src/share/vm/oops/klass.cpp}
 * <p>
 * klass:  Java类在JVM中的体现、JAVA类 --> C++类
 *   非ArrayKlass {@link sun.jvm.hotspot.oops.InstanceKlass}
 *                {@link openjdk-8u40/hotspot/src/share/vm/oops/instanceKlass.cpp}
 *     InstanceKlass       普通的类在JVM中对应的C++类 元信息 方法区
 *      InstanceMirrorKlass 对应的是Class对象 镜像类  堆区
 *              Mirror 镜子
 *      InstanceRefKlass
 *      InstanceClassLoaderKlass
 *   ArrayKlass {@link sun.jvm.hotspot.oops.ArrayKlass}
 *              {@link openjdk-8u40/hotspot/src/share/vm/oops/arrayKlass.cpp}
 *     基本类型数组 boolean、byte、char、short、int、float、long、double
 *     引用类型数组 ObjArrayKlass
 * <p>
 * Klass继承体系中涉及的C++类主要提供了两个功能：
 *  1、提供C++层面的Java类型（包括Java类和Java数组）表示方式，也就是用C++类的对象来描述Java类型；
 *  2、方法分派。
 * <p>
 * klass模型的存储问题
 *      根类加载器
 *      扩展类加载器
 *      应用类加载器
 *      方法区、元空间、meta space
 * <p>
 * 对象头: mark word、klass pointer(类型指针)、array length(数组长度)、padding(对其填充)
 * <p>
 * 运行HSDB工具
 * https://blog.csdn.net/qq_21202769/article/details/128924079
 * java -classpath "%JAVA_HOME%/lib/sa-jdi.jar" sun.jvm.hotspot.HSDB
 * java -classpath "D:\001.soft\018.JDK\001.JDK-8\jdk-1.8\lib\sa-jdi.jar" sun.jvm.hotspot.HSDB
 * <p>
 * 如何使用HSDB查看一个Java类对应的C++类(非数组)
 * 1、类向导 tools -> inspector
 * 2、对象 stack memory
 * <p>
 * 数组没有元信息 是动态生成的
 * <p>
 * Hotspot源码查看工具 source insight
 * <p>
 *类加载的过程：
 * <p>
 * 加载    、连接     、初始化、使用、卸载
 * 加载、验证、准备、解析、初始化、使用、卸载
 * <p>
 *
 * 加载：
 * 1、通过类的全限定名称获取存储该类的class文件
 * 2、解析成运行时数据(元数据) instanceKlass实例		存放在方法区
 * 3、生成该对象的Class对象 instanceMirrorKlass 	存放在堆区
 * <p>
 * 验证：
 * 1、文件格式验证
 * 2、元数据验证
 * 3、字节码验证
 * 4、符号引用验证
 * <p>
 * 准备：
 * 1、为静态变量分配内存、赋初值
 * 2、实例变量在创建对象时完成赋值的 不存在赋初值
 * 3、被final修饰 在编译时会加上ConstantValue属性 在准备阶段 直接完成赋值没有赋初值这一步
 * <p>
 * 解析： 符号引用(常量池的索引#01 #02 #03) 变成 直接引用(内存地址 0x0001)
 * 1、类、接口解析
 * 2、字段解析
 * 3、方法解析
 * 4、接口方法解析
 * <p>
 * PS1 发生时机
 * 1、类加载后马上解析
 * 2、使用的时候
 * PS2 解析什么
 * 1、继承的类、实现的接口、属性、方法
 * PS3 如何避免重复解析
 * 1、借助缓存 ConstantPoolCache 运行时常量池缓存
 *   常量池缓存（hashtable）
 *     key 常量池索引
 *     value String -> ConstantPoolCacheEntry
 *                     klass
 *                     method
 * <p>
 * 常量池分为三种
 * 1、静态常量池
 * 2、运行时常量池
 * 3、字符串常量池
 * <p>
 * 初始化：
 * 1、执行静态代码块 执行clinit方法 静态代码块只会生成一个
 * 2、完成静态变量的赋值
 */
public class HelloJdk {
    private int a = 0;
    private static int b = 20;
    /**
     * 准备阶段不会对C进行操作
     */
    private static int c;

    public static void main(String[] args) throws InterruptedException {
        int[] intArr = new int[1];
        HelloJdk[] objArr = new HelloJdk[1];

        Class<HelloJdk> helloJdkClass = HelloJdk.class;

        System.out.println("HelloJdk");

        while (true);
    }
}
