package com.taotao.mock;

/**
 * JVM入口
 * openjdk-8u40-src-b25-10_feb_2015/jdk/src/share/bin/java.c
 *
 * 类加载器入口
 * FindBootStrapClass(env, "sun/launcher/LauncherHelper"))
 */
public class HelloWorld {
    public static void main(String[] args) {
        System.out.println("Hello World");
    }
}