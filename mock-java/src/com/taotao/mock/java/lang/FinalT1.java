package com.taotao.mock.java.lang;


public class FinalT1 {

    public static void main(String[] args) {
        FinalT1.InnerClass.getInstance();
    }

    static class InnerClass {

        private static final InnerClass INSTANCE = new InnerClass();

        private static String ITEM_STA = "Hello";

        private InnerClass() {
            System.out.println("InnerStr = " + ITEM_STA);
        }

        public static InnerClass getInstance() {
            return INSTANCE;
        }
    }
}
