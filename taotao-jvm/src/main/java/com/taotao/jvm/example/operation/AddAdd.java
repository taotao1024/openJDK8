package com.taotao.jvm.example.operation;

public class AddAdd {

    public static void main(String[] args) {
        intAdd();
        byteAdd();
        floatAdd();
        longAdd();
        doubleAdd();
        doubleAdd2();
    }

    /**
     *  0 iconst_1
     *  1 istore_0
     *  2 getstatic #27 <java/lang/System.out : Ljava/io/PrintStream;>
     *  5 iload_0
     *  6 iinc 0 by 1
     *  9 invokevirtual #33 <java/io/PrintStream.println : (I)V>
     * 12 getstatic #27 <java/lang/System.out : Ljava/io/PrintStream;>
     * 15 iinc 0 by 1
     * 18 iload_0
     * 19 invokevirtual #33 <java/io/PrintStream.println : (I)V>
     * 22 return
     */
    public static void intAdd() {
        int v = 1;
        System.out.println(v++);
        System.out.println(++v);
    }

    /**
     *  0 iconst_1
     *  1 istore_0
     *  2 getstatic #27 <java/lang/System.out : Ljava/io/PrintStream;>
     *  5 iload_0
     *  6 iload_0
     *  7 iconst_1
     *  8 iadd
     *  9 i2b
     * 10 istore_0
     * 11 invokevirtual #33 <java/io/PrintStream.println : (I)V>
     * 14 getstatic #27 <java/lang/System.out : Ljava/io/PrintStream;>
     * 17 iload_0
     * 18 iconst_1
     * 19 iadd
     * 20 i2b
     * 21 istore_0
     * 22 iload_0
     * 23 invokevirtual #33 <java/io/PrintStream.println : (I)V>
     * 26 return
     */
    public static void byteAdd() {
        byte v = 1;
        System.out.println(v++);
        System.out.println(++v);
    }

    /**
     *  0 fconst_1
     *  1 fstore_0
     *  2 getstatic #27 <java/lang/System.out : Ljava/io/PrintStream;>
     *  5 fload_0
     *  6 dup
     *  7 fconst_1
     *  8 fadd
     *  9 fstore_0
     * 10 invokevirtual #39 <java/io/PrintStream.println : (F)V>
     * 13 getstatic #27 <java/lang/System.out : Ljava/io/PrintStream;>
     * 16 fload_0
     * 17 fconst_1
     * 18 fadd
     * 19 dup
     * 20 fstore_0
     * 21 invokevirtual #39 <java/io/PrintStream.println : (F)V>
     * 24 return
     */
    public static void floatAdd() {
        float v = 1;
        System.out.println(v++);
        System.out.println(++v);
    }

    /**
     *  0 lconst_1
     *  1 lstore_0
     *  2 getstatic #27 <java/lang/System.out : Ljava/io/PrintStream;>
     *  5 lload_0
     *  6 dup2
     *  7 lconst_1
     *  8 ladd
     *  9 lstore_0
     * 10 invokevirtual #42 <java/io/PrintStream.println : (J)V>
     * 13 getstatic #27 <java/lang/System.out : Ljava/io/PrintStream;>
     * 16 lload_0
     * 17 lconst_1
     * 18 ladd
     * 19 dup2
     * 20 lstore_0
     * 21 invokevirtual #42 <java/io/PrintStream.println : (J)V>
     * 24 return
     */
    public static void longAdd() {
        long v = 1;
        System.out.println(v++);
        System.out.println(++v);
    }

    /**
     *  0 dconst_1
     *  1 dstore_0
     *  2 getstatic #27 <java/lang/System.out : Ljava/io/PrintStream;>
     *  5 dload_0
     *  6 dup2
     *  7 dconst_1
     *  8 dadd
     *  9 dstore_0
     * 10 invokevirtual #45 <java/io/PrintStream.println : (D)V>
     * 13 getstatic #27 <java/lang/System.out : Ljava/io/PrintStream;>
     * 16 dload_0
     * 17 dconst_1
     * 18 dadd
     * 19 dup2
     * 20 dstore_0
     * 21 invokevirtual #45 <java/io/PrintStream.println : (D)V>
     * 24 return
     */
    public static void doubleAdd() {
        double v = 1;
        System.out.println(v++);
        System.out.println(++v);
    }

    /**
     *  0 dconst_1
     *  1 dstore_0
     *  2 getstatic #27 <java/lang/System.out : Ljava/io/PrintStream;>
     *  5 dload_0
     *  6 dconst_1
     *  7 dadd
     *  8 dup2
     *  9 dstore_0
     * 10 invokevirtual #45 <java/io/PrintStream.println : (D)V>
     * 13 return
     */
    public static void doubleAdd2() {
        double v = 1;
        System.out.println(++v);
    }
}
