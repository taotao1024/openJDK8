package com.taotao.jvm.example.lamdba.noparamslambda;

@FunctionalInterface
public interface Operator {
    /**
     * 入参应和被lambda调用的方法一致，在本例中是Operation中的operate方法
     * @return 返回值应和被lambda调用的方法一致，在本例中是Operation中的operate方法
     */
    void toOperate();
}
