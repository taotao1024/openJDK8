package com.taotao.mock.jdk.adjust;

import org.openjdk.jol.info.ClassLayout;

public class CountObjectSize {

    short s = 10;
    int i = 20;
    long l = 10;

    public static void main(String[] args) {
        CountObjectSize object = new CountObjectSize();


        System.out.println(object.s);

        System.out.println(ClassLayout.parseInstance(object).toPrintable());
    }
}