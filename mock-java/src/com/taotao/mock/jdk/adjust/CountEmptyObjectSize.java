package com.taotao.mock.jdk.adjust;

import org.openjdk.jol.info.ClassLayout;

/**
 * JDK6以后 默认开启指针压缩
 * <p>
 * 指针压缩：64bit支持该技术 <p>
 * 	将8B 压缩到 4B
 * 	原始值 <p>
 * 	test1 16B  	0  -  0000 1000 <p>
 * 	test2 32B  16  -  0001 0000 <p>
 * 	test3 24B  48  -  0011 0000 <p>
 * <p>
 * 	存储的时候 >>3 压缩了后3位 <p>
 * 	test1 16B  	0  -  0000 0010 <p>
 * 	test2 32B  16  -  0000 0100 <p>
 * 	test3 24B  48  -  0000 1100 <p>
 * <p>
 * 	使用的时候 <<3 <p>
 * 	test1 16B  	0  -  0000 1000 <p>
 * 	test2 32B  16  -  0001 0000 <p>
 * 	test3 24B  48  -  0011 0000 <p>
 */
public class CountEmptyObjectSize {

    public static void main(String[] args) {
        /**
         * 关闭指针压缩: -XX:-UseCompressedOops
         * 		8(mark Word)+8(指针类型)+0(非数组)+0(实例数据区)+0(对齐填充)=16B
         * 	     OFFSET  SIZE   TYPE DESCRIPTION                               VALUE
         *       0     4        (object header)                           01 00 00 00 (00000001 00000000 00000000 00000000) (1)
         *       4     4        (object header)                           00 00 00 00 (00000000 00000000 00000000 00000000) (0)
         *       8     4        (object header)                           00 1c 88 06 (00000000 00011100 10001000 00000110) (109583360)
         *      12     4        (object header)                           9d 01 00 00 (10011101 00000001 00000000 00000000) (413)
         * Instance size: 16 bytes
         * Space losses: 0 bytes internal + 0 bytes external = 0 bytes total
         * <p>
         * 开启指针压缩:  +XX:+UseCompressedOops
         * 		8(mark Word)+4(指针类型)+0(非数组)+0(实例数据区)+4(对齐填充)=16B
         *       OFFSET  SIZE   TYPE DESCRIPTION                               VALUE
         *       0     4        (object header)                           01 00 00 00 (00000001 00000000 00000000 00000000) (1)
         *       4     4        (object header)                           00 00 00 00 (00000000 00000000 00000000 00000000) (0)
         *       8     4        (object header)                           28 0f 00 00 (00101000 00001111 00000000 00000000) (3880)
         *      12     4        (loss due to the next object alignment)
         * Instance size: 16 bytes
         * Space losses: 0 bytes internal + 4 bytes external = 4 bytes total
         */
        Object obj = new Object();

        System.out.println(ClassLayout.parseInstance(obj).toPrintable());
    }
}
