package com.taotao.jvm.example;

/**
 * 待执行的java类
 */
public class HelloWorld {
    /**
     *  0 getstatic #7 <java/lang/System.out : Ljava/io/PrintStream;>
     *  3 ldc #13 <hello, world>
     *  5 invokevirtual #15 <java/io/PrintStream.println : (Ljava/lang/String;)V>
     *  8 getstatic #7 <java/lang/System.out : Ljava/io/PrintStream;>
     * 11 ldc #21 <hello, world 222>
     * 13 invokevirtual #15 <java/io/PrintStream.println : (Ljava/lang/String;)V>
     * 16 return
     * @param args
     */
    public static void main(String[] args) {
        System.out.println("hello, world");
        System.out.println("hello, world 222");
        // System.out.println(Thread.currentThread().getContextClassLoader().getResource(""));
    }
}