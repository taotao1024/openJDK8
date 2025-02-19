package com.taotao.jvm.example.lamdba.noparamslambda;

import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;
import java.lang.invoke.MethodType;

public class AppMain {

    public static void t1Show() {
        System.out.println("hello");
    }

    /**
     * 调用静态方法,测试成功
     */
    public static void t1() {
        MethodHandles.Lookup lookup = MethodHandles.lookup();

        MethodType mt = MethodType.methodType(void.class);

        try {
            MethodHandle mh = lookup.findStatic(AppMain.class,"t1Show", mt);

            mh.invoke();
        } catch (Exception e) {
            e.printStackTrace();
        } catch (Throwable throwable) {
            throwable.printStackTrace();
        }
    }

    public static void main(String[] args) {
        t1();
    }
}
