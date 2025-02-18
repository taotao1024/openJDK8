package com.taotao.jvm.example.lamdba;

public class TestLamda {

    public static void main(String[] args) {
        CustomLambda obj = () -> {
            System.out.println("hello");
        };
        obj.run();
    }

    /**
     *  0 invokedynamic #12 <run, BootstrapMethods #0>
     *  5 astore_0
     *  6 aload_0
     *  7 invokeinterface #16 <com/taotao/jvm/example/lamdba/CustomLambda.run : ()V> count 1
     * 12 return
     */
    public static void test1() {
        CustomLambda obj = () -> {
            System.out.println("hello");
        };
        obj.run();
    }

    /**
     *  0 invokedynamic #20 <run, BootstrapMethods #1>
     *  5 astore_0
     *  6 aload_0
     *  7 iconst_1
     *  8 invokeinterface #23 <com/taotao/jvm/example/lamdba/CustomLambda1.run : (I)V> count 2
     * 13 return
     */
    public static void test2() {
        CustomLambda1 obj1 = (x) -> {
            System.out.println("hello#" + x);
        };
        obj1.run(1);
    }

    /**
     *  0 invokedynamic #28 <run, BootstrapMethods #2>
     *  5 astore_0
     *  6 aload_0
     *  7 iconst_1
     *  8 iconst_2
     *  9 invokeinterface #31 <com/taotao/jvm/example/lamdba/CustomLambda2.run : (II)V> count 3
     * 14 return
     */
    public static void test3() {
        CustomLambda2 obj = (x, y) -> {
            System.out.println("hello#" + x + "#" + y);
        };
        obj.run(1, 2);
    }
}
