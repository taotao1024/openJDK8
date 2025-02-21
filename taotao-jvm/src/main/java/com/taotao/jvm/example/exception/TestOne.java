package com.taotao.jvm.example.exception;

public class TestOne {
    /**
     * 0 new #7 <java/lang/RuntimeException>
     * 3 dup
     * 4 invokespecial #9 <java/lang/RuntimeException.<init> : ()V>
     * 7 athrow
     *
     * @param args
     */
    public static void main(String[] args) {
        throw new RuntimeException();
    }
}
