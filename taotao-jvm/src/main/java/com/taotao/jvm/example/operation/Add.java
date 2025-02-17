package com.taotao.jvm.example.operation;

public class Add {

    public static void main(String[] args) {
        addInt();
        addFloat();
        addLong();
        addDouble();
    }

    /**
     *  0 iconst_1
     *  1 istore_0
     *  2 iconst_2
     *  3 istore_1
     *  4 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     *  7 iload_0
     *  8 iload_1
     *  9 iadd
     * 10 invokevirtual #21 <java/io/PrintStream.println : (I)V>
     * 13 return
     */
    public static void addInt() {
        int a = 1;
        int b = 2;

        System.out.println(a + b);
    }

    /**
     *  0 fconst_1
     *  1 fstore_0
     *  2 fconst_2
     *  3 fstore_1
     *  4 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     *  7 fload_0
     *  8 fload_1
     *  9 fadd
     * 10 invokevirtual #24 <java/io/PrintStream.println : (F)V>
     * 13 return
     */
    public static void addFloat() {
        float a = 1;
        float b = 2;

        System.out.println(a + b);
    }

    /**
     *  0 lconst_1
     *  1 lstore_0
     *  2 ldc2_w #27 <2>
     *  5 lstore_2
     *  6 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     *  9 lload_0
     * 10 lload_2
     * 11 ladd
     * 12 invokevirtual #29 <java/io/PrintStream.println : (J)V>
     * 15 return
     */
    public static void addLong() {
        long a = 1;
        long b = 2;

        System.out.println(a + b);
    }

    /**
     *  0 dconst_1
     *  1 dstore_0
     *  2 ldc2_w #7 <2.0>
     *  5 dstore_2
     *  6 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     *  9 dload_0
     * 10 dload_2
     * 11 dadd
     * 12 invokevirtual #15 <java/io/PrintStream.println : (D)V>
     * 15 return
     */
    public static void addDouble() {
        double a = 1;
        double b = 2;

        System.out.println(a + b);
    }

}
