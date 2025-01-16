package com.taotao.mock.java.lang;

import java.util.UUID;

/**
 * https://blog.csdn.net/Soul_wh/article/details/111409565
 * <p>
 * jdk/src/hotspot/share/classfile/javaClasses.cpp
 */
public class FinalT3 {
    /**
     * 添加 ConstantValue 指令
     */
    public final int a = 1;
    public final String b = "2";
    public final Object c = new Object();
    public final int d;
    public final String e = UUID.randomUUID().toString();

    public FinalT3(int d) {
        this.d = 4;
    }
}
