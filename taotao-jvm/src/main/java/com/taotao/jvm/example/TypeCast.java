package com.taotao.jvm.example;

public class TypeCast {

    public static void main(String[] args) {
//        int i = 10;
//        long l = i;
//
//        System.out.println(l);

        //=====
//        int i = 10;
//        float f = i;
//
//        System.out.println(f);

        //=====
        int i = 10;
        double d = i;

        if (i==10||d==10) {

        }

        System.out.println(d);
    }

    //=====
    public static void int2Long() {
        int i = 10;
        long l = i;

        System.out.println(l);
    }

    public static void int2Float() {
        int i = 10;
        float f = i;

        System.out.println(f);
    }

    public static void int2Double() {
        int i = 10;
        double d = i;

        System.out.println(d);
    }

    /**
     * 测试：
     * 1、long的值四个字节以内
     * 2、long的值超过四个字节
     */
    public static void long2Int() {
        long l = 10;
        int i = (int) l;

        System.out.println(i);
    }

    public static void long2Float() {
        long l = 10;
        float f = l;

        System.out.println(f);
    }

    public static void long2Double() {
        long l = 10;
        double d = l;

        System.out.println(d);
    }

    //=====
    public static void float2Int() {
        float f = 10;
        int i = (int) f;

        System.out.println(f);
    }

    public static void float2Long() {
        float f = 10;
        long l = (long) f;

        System.out.println(l);
    }

    public static void float2Double() {
        float f = 10;
        double d = f;

        System.out.println(d);
    }

    //=====
    public static void double2Int() {
        double d = 10;
        int i = (int) d;

        System.out.println(i);
    }

    public static void double2Long() {
        double d = 10;
        long l = (long) d;

        System.out.println(l);
    }

    public static void double2Float() {
        double d = 10;
        float f = (float) d;

        System.out.println(f);
    }

    //=====
    public static void int2Byte() {
        int i = 1;
        byte b = (byte) i;

        System.out.println(b);
//        System.out.println(Integer.toHexString(b));
    }

    public static void int2Char() {
        int i = 1;
        char c = (char) i;

        System.out.println(c);
    }

    public static void int2Short() {
        int i = 1;
        short s = (short) i;

        System.out.println((s));
    }

    //=====

    /**
     * 这个程序能不能运行，底层是怎么实现的
     */
    public static void short2Byte() {
        short s = 0x1234;
        byte b = (byte) s;

        System.out.println(b);
    }
}
