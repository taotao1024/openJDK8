package com.taotao.jvm.example.operation;

public class AddAdd {

    public static void main(String[] args) {
//        int v = 1;
//        System.out.println(v++);
//        System.out.println(++v);

        //=====
//        byte v = 1;
//        System.out.println(v++);
//        System.out.println(++v);

        //=====
//        float v = 1;
//        System.out.println(v++);
//        System.out.println(++v);

        //=====
//        long v = 1;
//        System.out.println(v++);
//        System.out.println(++v);

        //=====
        double v = 1;
        System.out.println(v++);
        System.out.println(++v);
    }

    public static void intAdd() {
        int v = 1;
        System.out.println(v++);
        System.out.println(++v);
    }

    //=====
    public static void byteAdd() {
        byte v = 1;
        System.out.println(v++);
        System.out.println(++v);
    }

    public static void floatAdd() {
        float v = 1;
        System.out.println(v++);
        System.out.println(++v);
    }

    public static void longAdd() {
        long v = 1;
        System.out.println(v++);
        System.out.println(++v);
    }

    public static void doubleAdd() {
        double v = 1;
        System.out.println(v++);
        System.out.println(++v);
    }

    public static void doubleAdd2() {
        double v = 1;
        System.out.println(++v);
    }
}
