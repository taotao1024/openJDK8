package com.taotao.mock.utils;

import java.util.Map;

public final class ThreadUtil {

    public static void main(String[] args) {
        outputThreadStack2Log();
    }

    public static void outputThreadStack2Log() {
        StackTraceElement[] stackTraceElements = Thread.currentThread().getStackTrace();
        StringBuilder sb = new StringBuilder("Current Thread Dump：\n");
        for (int i = 2; i < stackTraceElements.length; i++) {
            sb.append(stackTraceElements[i].toString()).append("\n");
        }
        print("Thread**** " + sb.toString());
    }

    public static void outputMapObject(Map<String, Object> map) {
        for (Map.Entry<String, Object> entry : map.entrySet()) {
            String key = entry.getKey();
            Object value = entry.getValue();
            print("****Key: " + key + ", Value: " + value);
        }
    }

    public static void outputMapString(Map<String, String> map) {
        for (Map.Entry<String, String> entry : map.entrySet()) {
            String key = entry.getKey();
            String value = entry.getValue();
            print("****Key: " + key + ", Value: " + value);
        }
    }

    private static void print(String str) {
        System.out.println(str);
    }
}
