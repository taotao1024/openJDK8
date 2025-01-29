package com.taotao.mock.jdk.jvm;

/**
 * klass:  Java类在JVM中的体现、JAVA类 --> C++类
 * 	Instanceklass
 * 	ArrayKlass
 * <p>
 * 字节码文件
 * 	class content 字节流 stream
 * 	ClassFileStream* cfs = stream(); // 直接内存中 解析完就释放内存
 * klass模型 方法区 通过C++的操作符重写 实现了将JVM堆区的Klass对象 移动到运行时数据区的堆区
 * class对象 堆区 mirrorklass
 * new Obj() 堆区
 * <p>
 * 堆
 * 程序计数器
 * 虚拟机栈
 * 本地方法栈 Java通过JNI技术 调用C++提供的动态连接库 操作硬件
 * 方法区 虚拟机规范
 * 不同类型、不同版 的虚拟机 对方法区的实现不同
 * 	永久代 JDK8之前 JVM的堆堆区
 * 	元空间 JDK8之后 直接内存中
 * 为什么JDK8中元空间取代了永久代：
 * 	永久代的缺点
 * 		1、永久代很难触发卸载机制
 * 		2、Class对象没有被使用
 * 		3、系统的三个类加载器 加载的类 不会被释放
 * 		4、释放的内存很少
 * 		5、动态字节码技术在修改字节码时 容易引起堆内存的OOM
 * 	元空间的优点：
 * 		1、
 * 	元空间存储
 * 		1、元空间没有整理算法、容易产生碎片 (中间件 实现内存合并算法)
 * <p>
 *
 * Java进程
 * 	代码段
 * 	数据区
 * 	栈
 * ------------------------
 * 	堆
 * 		JVM内存模型
 * 			虚拟机栈
 * 			本地方法栈
 * 			堆区
 * 			方法区
 * 			程序计数器 int pc;
 * <p>
 * 虚拟机栈
 * 	与虚拟机1：1
 * 	一个虚拟机栈中的栈帧 与方法调用次数 1：1
 * 	栈帧:
 * 		局部变量表
 * 			存储局部变量使用
 * 			1、局部变量表有多大
 * 				编译时确定
 * 			2、静态方法、非静态方法的区别
 * 				indx=0的位置 this指针
 * 			3、哪些数据会存储到局部变量表中
 * 				局部变量
 * 				参数
 * 		操作数栈
 * 			中间层
 * 			push
 * 			pop
 * 			dup
 * 		动态链接
 * 			直接引用 C++对应的内存地址
 * 		返回地址
 * 			保存现场、恢复现场
 * 		轻量级锁 syncxxxx LockRecord
 * <p>
 * 运行时数据区：
 * 	1、老年代为什么比新生代大
 * 		老年代存储的内容 比 新生代多
 * 		1、gc>15次的对象
 * 			对象头中定义分代年限仅占用4bit，4bit最大表示16。
 * 			0000-1111 -> 0-f
 * 		2、空间担保机制
 * 			年轻代的Eden区经过GC后，对象无法放到from区/to区时，会直接放到老年代。
 * 		3、动态年龄判断机制
 * 			新生代经过多次GC，经过from区/to区转换次数超过设定值的1/2时，会直接放到老年代。
 * 		4、大对象
 * 			年轻代Eden区 超过总容量1/2的对象，直接移到老年代。
 */
public class Test1 {
    long v = 1;
    byte a = 10;
    char c = 11;

    /**
     *  0 new #5 <com/luban/ziya/jvm/Test>
     *      1、堆空间申请了内存（不完全对象）构造方法还未执行
     *      2、内存地址压入栈
     *  3 dup
     *      0、duplicate
     *      1、赋值栈顶元素
     *      2、再次压入栈
     *  4 invokespecial #6 <com/luban/ziya/jvm/Test.<init> : ()V>
     *      0、执行方法
     *      1、构架环境
     *          1、创建栈帧
     *          2、传值
     *          3、保存现场
     *          4、给this指针赋值
     *      2、执行（完全对象）
     *  7 astore_1
     *      1、pop出元素 对象的指针
     *      2、赋值给index=1的位置的变量（局部变量表）
     *  8 getstatic #7 <java/lang/System.out : Ljava/io/PrintStream;>
     * 11 aload_1
     * 12 getfield #3 <com/luban/ziya/jvm/Test.a : B>
     * 15 invokevirtual #8 <java/io/PrintStream.println : (I)V>
     * 18 getstatic #7 <java/lang/System.out : Ljava/io/PrintStream;>
     * 21 aload_1
     * 22 invokevirtual #9 <com/luban/ziya/jvm/Test.add : ()I>
     * 25 invokevirtual #8 <java/io/PrintStream.println : (I)V>
     * 28 goto 28 (0)
     * @param args
     * @throws Exception
     */
    public static void main(String[] args) throws Exception {
        Test1 obj = new Test1();

        System.out.println(obj.a);
        System.out.println(obj.add());

        while (true) ;
    }

    public int add() {
        int a = 1;
        int b = 2;
        double c = 2;
        double d = 2;
        return a + b;
    }
}