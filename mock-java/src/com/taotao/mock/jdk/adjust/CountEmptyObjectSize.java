package com.taotao.mock.jdk.adjust;

import org.openjdk.jol.info.ClassLayout;

public class CountEmptyObjectSize {

    public static void main(String[] args) {
        /**
         * 	关闭指针压缩：
         * 		8(mark Word)+8(指针类型)+0(非数组)+0(实例数据区)+0(对齐填充)=16B
         * 	开启指针压缩：
         * 		8(mark Word)+4(指针类型)+0(非数组)+0(实例数据区)+4(对齐填充)=16B
         */
        Object obj = new Object();

        System.out.println(ClassLayout.parseInstance(obj).toPrintable());
    }
}
