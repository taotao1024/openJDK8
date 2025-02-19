package com.taotao.jvm.example.exception;

public class TestTry {

    /**
     *  0 iconst_1
     *  1 iconst_0
     *  2 idiv
     *  3 istore_1
     *  4 goto 12 (+8)
     *  7 astore_1
     *      栈顶的引用对象存储到1的位置
     *  8 aload_1
     *  9 invokevirtual #9 <java/lang/RuntimeException.printStackTrace : ()V>
     * 12 return
     * @param args
     */
    public static void main(String[] args) {
        try {
            int a = 1 / 0;
        } catch (RuntimeException e) {
            // 异常表
            e.printStackTrace();
        }
    }
}
