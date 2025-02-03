package com.taotao.mock.jdk.adjust;

import org.openjdk.jol.info.ClassLayout;

/**
 * JDK6以后 默认开启指针压缩
 * <p>
 * int数组
 */
public class CountSimpleObjectSize {

    static int[] arr = {0, 1, 2};

    public static void main(String[] args) {
        /**
         * 关闭指针压缩: -XX:-UseCompressedOops
         * 		8(mark Word)+8(指针类型)+4(数组)+4(对象头对齐填充)+4*3(实例数据区)+0(对齐填充)=40B
         * 	    OFFSET  SIZE   TYPE DESCRIPTION                               VALUE
         *       0     4        (object header)                           01 00 00 00 (00000001 00000000 00000000 00000000) (1)
         *       4     4        (object header)                           00 00 00 00 (00000000 00000000 00000000 00000000) (0)
         *       8     4        (object header)                           68 0b 33 fe (01101000 00001011 00110011 11111110) (-30209176)
         *      12     4        (object header)                           63 02 00 00 (01100011 00000010 00000000 00000000) (611)
         *      16     4        (object header)                           03 00 00 00 (00000011 00000000 00000000 00000000) (3)
         *      20     4        (alignment/padding gap) 对象头填充
         *      24    12    int [I.<elements>                             N/A
         *      36     4        (loss due to the next object alignment)
         * <p>
         * 开启指针压缩:  +XX:+UseCompressedOops
         * 		8(mark Word)+4(指针类型)+4(数组)+4*3(实例数据区)+4(对齐填充)=32B
         * 	    OFFSET  SIZE   TYPE DESCRIPTION                               VALUE
         *       0     4        (object header)                           01 00 00 00 (00000001 00000000 00000000 00000000) (1)
         *       4     4        (object header)                           00 00 00 00 (00000000 00000000 00000000 00000000) (0)
         *       8     4        (object header)                           68 0b 00 00 (01101000 00001011 00000000 00000000) (2920)
         *      12     4        (object header)                           03 00 00 00 (00000011 00000000 00000000 00000000) (3)
         *      16    12    int [I.<elements>                             N/A
         *      28     4        (loss due to the next object alignment)
         * Instance size: 32 bytes
         * Space losses: 0 bytes internal + 4 bytes external = 4 bytes total
         */
        CountSimpleObjectSize test1 = new CountSimpleObjectSize();

        System.out.printf(ClassLayout.parseInstance(arr).toPrintable());

        while (true);
    }
}
