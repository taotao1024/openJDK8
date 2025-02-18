package com.taotao.jvm.jdk;

import com.taotao.jvm.hotspot.src.share.vm.classfile.BootClassLoader;
import com.taotao.jvm.hotspot.src.share.vm.oops.InstanceKlass;
import com.taotao.jvm.hotspot.src.share.vm.oops.MethodInfo;
import com.taotao.jvm.hotspot.src.share.vm.prims.JavaNativeInterface;
import com.taotao.jvm.hotspot.src.share.vm.runtime.JavaThread;
import com.taotao.jvm.hotspot.src.share.vm.runtime.Threads;

public class Main {

    public static void main(String[] args) {
        startJVM();
    }

    public static void startJVM() {
        String klass_1 = "com.taotao.jvm.example.HelloWorld";
        String klass_2 = "com.taotao.jvm.example.lamdba.Lamdba";

        // 通过AppClassLoader加载main函数所在的类
        InstanceKlass mainKlass = BootClassLoader.loadMainKlass(klass_2);

        // 找到main方法
        MethodInfo mainMethod = JavaNativeInterface.getMethodID(mainKlass, "main", "([Ljava/lang/String;)V");

        // 创建线程
        JavaThread thread = new JavaThread();

        Threads.getThreadList().add(thread);
        Threads.setCurrentThread(thread);

        // 执行main方法
        JavaNativeInterface.callStaticMethod(mainMethod);
    }
}
