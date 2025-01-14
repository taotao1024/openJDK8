package com.taotao.mock.java.util.math;

public class IntegerTest {
    public static void main(String[] args) {
        // false 两个都是new的对象 对象引用不同 所以值不一样
        System.out.println(Integer.valueOf(100) == new Integer(100));
        // true 常量 和 new的对现象 相同 这里很奇怪 不清楚拆包原理
        System.out.println(100 == new Integer(100));
        // true 同上 valueOf 也是 new对象
        System.out.println(100 == Integer.valueOf(100));
        // treu 同上
        System.out.println(128 == Integer.valueOf(128));

        System.out.println("------------------------------------------");

        Integer a1 = 128;        Integer a2 = 128;
        Integer b1 = 127;        Integer b2 = 127;
        // false 这里应该和缓存机制有关 不清楚原理
        System.out.println(a1 == a2);
        // true 这里应该和缓存机制有关 不清楚原理
        System.out.println(b1 == b2);

        System.out.println("------------------------------------------");
        // true 缓存 相同的对象
        System.out.println(Integer.valueOf(125) == Integer.valueOf(125));
        // false 缓存 与 new对象不同
        System.out.println(Integer.valueOf(129) == Integer.valueOf(129));
    }
}
