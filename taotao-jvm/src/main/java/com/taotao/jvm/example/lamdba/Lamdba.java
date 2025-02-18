package com.taotao.jvm.example.lamdba;

public class Lamdba {
    /**
     * 0 invokedynamic #7 <run, BootstrapMethods #0>
     * 5 astore_1
     * 6 return
     *
     * @param args
     */
    public static void main(String[] args) {
        /**
         * 0 getstatic #11 <java/lang/System.out : Ljava/io/PrintStream;>
         * 3 ldc #17 <hello world>
         * 5 invokevirtual #19 <java/io/PrintStream.println : (Ljava/lang/String;)V>
         * 8 return
         */
        Runnable runnable = () -> System.out.println("hello world");
        runnable.run();
    }
}
