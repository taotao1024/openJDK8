package com.taotao.jvm.example.exception;

public class TestTry2 {

    public static void main(String[] args) {
        try {
            int a = 1 / 0;
        } catch (RuntimeException e) {
            e.printStackTrace();
        }

        try {
            int a = 1 / 0;
        } catch (RuntimeException e) {
            throw new RuntimeException("除零异常");
        }
    }
}
