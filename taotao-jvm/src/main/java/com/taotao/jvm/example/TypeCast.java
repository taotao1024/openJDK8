package com.taotao.jvm.example;

public class TypeCast {

    public static void main(String[] args) {
        int2Long();
        int2Float();
        int2Double();
        long2Int();
        long2Float();
        long2Double();
        float2Int();
        float2Long();
        float2Double();
        double2Int();
        double2Long();
        double2Float();
        int2Byte();
        int2Char();
        int2Short();
        short2Byte();
    }

    /**
     *  0 bipush 10
     *  2 istore_0
     *  3 iload_0
     *  4 i2l
     *  5 lstore_1
     *  6 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     *  9 lload_1
     * 10 invokevirtual #21 <java/io/PrintStream.println : (J)V>
     * 13 return
     */
    public static void int2Long() {
        int i = 10;
        long l = i;

        System.out.println(l);
    }

    /**
     *  0 bipush 10
     *  2 istore_0
     *  3 iload_0
     *  4 i2f
     *  5 fstore_1
     *  6 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     *  9 fload_1
     * 10 invokevirtual #24 <java/io/PrintStream.println : (F)V>
     * 13 return
     */
    public static void int2Float() {
        int i = 10;
        float f = i;

        System.out.println(f);
    }

    /**
     *  0 bipush 10
     *  2 istore_0
     *  3 iload_0
     *  4 i2d
     *  5 dstore_1
     *  6 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     *  9 dload_1
     * 10 invokevirtual #15 <java/io/PrintStream.println : (D)V>
     * 13 return
     */
    public static void int2Double() {
        int i = 10;
        double d = i;

        System.out.println(d);
    }

    /**
     * 测试：
     * 1、long的值四个字节以内
     * 2、long的值超过四个字节
     * <p>
     *  0 ldc2_w #27 <10>
     *  3 lstore_0
     *  4 lload_0
     *  5 l2i
     *  6 istore_2
     *  7 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     * 10 iload_2
     * 11 invokevirtual #29 <java/io/PrintStream.println : (I)V>
     * 14 return
     */
    public static void long2Int() {
        long l = 10;
        int i = (int) l;

        System.out.println(i);
    }

    /**
     *  0 ldc2_w #27 <10>
     *  3 lstore_0
     *  4 lload_0
     *  5 l2f
     *  6 fstore_2
     *  7 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     * 10 fload_2
     * 11 invokevirtual #24 <java/io/PrintStream.println : (F)V>
     * 14 return
     */
    public static void long2Float() {
        long l = 10;
        float f = l;

        System.out.println(f);
    }

    /**
     *  0 ldc2_w #27 <10>
     *  3 lstore_0
     *  4 lload_0
     *  5 l2d
     *  6 dstore_2
     *  7 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     * 10 dload_2
     * 11 invokevirtual #15 <java/io/PrintStream.println : (D)V>
     * 14 return
     */
    public static void long2Double() {
        long l = 10;
        double d = l;

        System.out.println(d);
    }

    /**
     *  0 ldc #32 <10.0>
     *  2 fstore_0
     *  3 fload_0
     *  4 f2i
     *  5 istore_1
     *  6 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     *  9 fload_0
     * 10 invokevirtual #24 <java/io/PrintStream.println : (F)V>
     * 13 return
     */
    public static void float2Int() {
        float f = 10;
        int i = (int) f;

        System.out.println(f);
    }

    /**
     *  0 ldc #32 <10.0>
     *  2 fstore_0
     *  3 fload_0
     *  4 f2l
     *  5 lstore_1
     *  6 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     *  9 lload_1
     * 10 invokevirtual #21 <java/io/PrintStream.println : (J)V>
     * 13 return
     */
    public static void float2Long() {
        float f = 10;
        long l = (long) f;

        System.out.println(l);
    }

    /**
     *  0 ldc #32 <10.0>
     *  2 fstore_0
     *  3 fload_0
     *  4 f2d
     *  5 dstore_1
     *  6 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     *  9 dload_1
     * 10 invokevirtual #15 <java/io/PrintStream.println : (D)V>
     * 13 return
     */
    public static void float2Double() {
        float f = 10;
        double d = f;

        System.out.println(d);
    }

    /**
     *  0 ldc2_w #7 <10.0>
     *  3 dstore_0
     *  4 dload_0
     *  5 d2i
     *  6 istore_2
     *  7 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     * 10 iload_2
     * 11 invokevirtual #29 <java/io/PrintStream.println : (I)V>
     * 14 return
     */
    public static void double2Int() {
        double d = 10;
        int i = (int) d;

        System.out.println(i);
    }

    /**
     *  0 ldc2_w #7 <10.0>
     *  3 dstore_0
     *  4 dload_0
     *  5 d2l
     *  6 lstore_2
     *  7 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     * 10 lload_2
     * 11 invokevirtual #21 <java/io/PrintStream.println : (J)V>
     * 14 return
     */
    public static void double2Long() {
        double d = 10;
        long l = (long) d;

        System.out.println(l);
    }

    /**
     *  0 ldc2_w #7 <10.0>
     *  3 dstore_0
     *  4 dload_0
     *  5 d2f
     *  6 fstore_2
     *  7 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     * 10 fload_2
     * 11 invokevirtual #24 <java/io/PrintStream.println : (F)V>
     * 14 return
     */
    public static void double2Float() {
        double d = 10;
        float f = (float) d;

        System.out.println(f);
    }

    /**
     *  0 iconst_1
     *  1 istore_0
     *  2 iload_0
     *  3 i2b
     *  4 istore_1
     *  5 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     *  8 iload_1
     *  9 invokevirtual #29 <java/io/PrintStream.println : (I)V>
     * 12 return
     */
    public static void int2Byte() {
        int i = 1;
        byte b = (byte) i;

        System.out.println(b);
        System.out.println(Integer.toHexString(b));
    }

    /**
     *  0 iconst_1
     *  1 istore_0
     *  2 iload_0
     *  3 i2c
     *  4 istore_1
     *  5 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     *  8 iload_1
     *  9 invokevirtual #33 <java/io/PrintStream.println : (C)V>
     * 12 return
     */
    public static void int2Char() {
        int i = 1;
        char c = (char) i;

        System.out.println(c);
    }

    /**
     *  0 iconst_1
     *  1 istore_0
     *  2 iload_0
     *  3 i2s
     *  4 istore_1
     *  5 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     *  8 iload_1
     *  9 invokevirtual #29 <java/io/PrintStream.println : (I)V>
     * 12 return
     */
    public static void int2Short() {
        int i = 1;
        short s = (short) i;

        System.out.println((s));
    }

    /**
     * 这个程序能不能运行，底层是怎么实现的
     * <p>
     *  0 sipush 4660
     *  3 istore_0
     *  4 iload_0
     *  5 i2b
     *  6 istore_1
     *  7 getstatic #9 <java/lang/System.out : Ljava/io/PrintStream;>
     * 10 iload_1
     * 11 invokevirtual #29 <java/io/PrintStream.println : (I)V>
     * 14 return
     */
    public static void short2Byte() {
        short s = 0x1234;
        byte b = (byte) s;

        System.out.println(b);
    }
}
