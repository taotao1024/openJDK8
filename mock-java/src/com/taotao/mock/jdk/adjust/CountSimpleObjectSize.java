package com.taotao.mock.jdk.adjust;

import org.openjdk.jol.info.ClassLayout;

public class CountSimpleObjectSize {

    static int[] arr = {0, 1, 2};

    public static void main(String[] args) {
        CountSimpleObjectSize test1 = new CountSimpleObjectSize();

        System.out.printf(ClassLayout.parseInstance(arr).toPrintable());

        while (true);
    }
}
