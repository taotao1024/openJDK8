package com.taotao.mock.jdk.adjust;

import org.openjdk.jol.info.ClassLayout;

/**
 * JDK6以后 默认开启指针压缩
 * <p>
 * 有实例数据
 */
public class CountObjectSize {

    short s = 10;
    int i = 20;
    long l = 10;

    public static void main(String[] args) {
        /**
         * 关闭指针压缩: -XX:-UseCompressedOops
         * 		8(mark Word)+8(指针类型)+0(非数组)+4*2+2(实例数据区)+2(对齐填充)=32B
         * 	    OFFSET  SIZE    TYPE DESCRIPTION                               VALUE
         *       0     4         (object header)                           01 00 00 00 (00000001 00000000 00000000 00000000) (1)
         *       4     4         (object header)                           00 00 00 00 (00000000 00000000 00000000 00000000) (0)
         *       8     4         (object header)                           a8 4f 88 08 (10101000 01001111 10001000 00001000) (143151016)
         *      12     4         (object header)                           53 02 00 00 (01010011 00000010 00000000 00000000) (595)
         *      16     8    long CountObjectSize.l                         10
         *      24     4     int CountObjectSize.i                         20
         *      28     2   short CountObjectSize.s                         10
         *      30     2         (loss due to the next object alignment)
         * Instance size: 32 bytes
         * Space losses: 0 bytes internal + 2 bytes external = 2 bytes total
         * <p>
         * 开启指针压缩:  +XX:+UseCompressedOops
         * 		8(mark Word)+4(指针类型)+0(非数组)+4*2+2(实例数据区)+6(对齐填充)=32B
         * 	    OFFSET  SIZE    TYPE DESCRIPTION                               VALUE
         *       0     4         (object header)                           01 00 00 00 (00000001 00000000 00000000 00000000) (1)
         *       4     4         (object header)                           00 00 00 00 (00000000 00000000 00000000 00000000) (0)
         *       8     4         (object header)                           28 08 06 00 (00101000 00001000 00000110 00000000) (395304)
         *      12     4     int CountObjectSize.i                         20
         *      16     8    long CountObjectSize.l                         10
         *      24     2   short CountObjectSize.s                         10
         *      26     6         (loss due to the next object alignment)
         * Instance size: 32 bytes
         * Space losses: 0 bytes internal + 6 bytes external = 6 bytes total
         */
        CountObjectSize object = new CountObjectSize();

        System.out.println(ClassLayout.parseInstance(object).toPrintable());
    }
}