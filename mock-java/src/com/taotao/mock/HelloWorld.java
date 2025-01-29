package com.taotao.mock;

import java.nio.ByteBuffer;

/**
 * JVM入口
 * openjdk-8u40-src-b25-10_feb_2015/jdk/src/share/bin/java.c
 * <p>
 * 类加载器入口
 * FindBootStrapClass(env, "sun/launcher/LauncherHelper"))
 */
public class HelloWorld {
    public static void main(String[] args) {
        System.out.println("Hello World");
        // JVM堆内存
        ByteBuffer.allocate(1024);
        // 操作系统直接内存
        ByteBuffer.allocateDirect(1024);
    }
}