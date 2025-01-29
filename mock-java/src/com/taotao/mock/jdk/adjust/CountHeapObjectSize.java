package com.taotao.mock.jdk.adjust;

import org.openjdk.jol.info.ClassLayout;

import java.util.ArrayList;
import java.util.List;

/**
 * Created By ziya
 * 2020/11/21
 */
public class CountHeapObjectSize {
    byte[] arr = new byte[1024];

    List<CountHeapObjectSize> list = new ArrayList<CountHeapObjectSize>();

    public static void main(String[] args) {
        CountHeapObjectSize obj = new CountHeapObjectSize();

        for (int i = 0; i < 100; i++) {
            obj.list.add(new CountHeapObjectSize());
        }

        System.out.println(ClassLayout.parseInstance(obj).toPrintable());
    }
}
