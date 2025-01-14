package com.taotao.mock.java.util.math;

import java.math.BigInteger;

public class BigIntegerTest {
    public static void main(String[] args) {
        System.out.println(BigInteger.valueOf(100).equals(new BigInteger("100")));

        Number number1 = new Integer(10);
        Number number2 = new BigInteger("10");
        System.out.println(number2 == number1);
    }
}
