package com.taotao.mock.java.time;

import java.time.Instant;

public class InstantTest {
    public static void main(String[] args) {
        System.out.println("当前时间的时间: " + Instant.now());
        System.out.println("指定时间的时间: " + Instant.ofEpochSecond(1736845019));
        System.out.println("指定时间的时间: " + Instant.ofEpochMilli(1736844997584L));

        System.out.println("当前时间的时间戳（秒）: " + Instant.now().getEpochSecond());
        System.out.println("当前时间的时间戳（毫秒）: " + Instant.now().toEpochMilli());
    }
}
