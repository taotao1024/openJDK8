package com.taotao.mock.java.util;


import java.util.concurrent.ConcurrentHashMap;

public class ConcurrentHashMapTest {
    public void test() {
        ConcurrentHashMap<String, String> map = new ConcurrentHashMap<String, String>() {{
            this.put("key_1", "str_1");
            this.put("key_2", "str_2");
            this.put("key_3", "str_3");
        }};
        map.entrySet().forEach(System.out::println);
    }
}
