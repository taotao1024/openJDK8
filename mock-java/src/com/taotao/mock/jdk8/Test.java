package com.taotao.mock.jdk8;

import java.util.Arrays;
import java.util.List;

public class Test {


    public static void main(String[] args) {
        List<String> list = Arrays.asList("111", "222", "333", "444");
//        list.forEach(item -> System.out.println(item));
//        list.forEach(System.out::println);
//        list.stream().forEach(System.out::println);

        Runnable stream = list::stream;
        Runnable test1 = new Test()::test1;
        Runnable test2 = Test::test2;
        Runnable aNew = Test::new;

        int i = 123123;
        System.out.println(123123);
        Runnable println = System.out::println;
        println.run();
    }

    public void test1() {
        System.out.println("test2 is running");
    }

    public static void test2() {
        System.out.println("test2 is running");
    }
}
