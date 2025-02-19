package com.taotao.jvm.example.lamdba.noparams;

import java.lang.invoke.CallSite;
import java.lang.invoke.LambdaMetafactory;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;
import java.lang.invoke.MethodType;
import java.lang.reflect.Method;

public class NoParamMain {

    public static void main(String[] args) {
        String methodName="operate";
        try {
            //通过全类名，获取类的实例
            Class clazz=Class.forName(Operation.class.getName());
            //获取到类的对象，要求该类必须有无参构造
            Operation o = (Operation)clazz.newInstance();
            //获取方法对象
            Method method= clazz.getDeclaredMethod(methodName, null);
            MethodHandles.Lookup lookup=MethodHandles.lookup();
            //指定方法不以反射运行
            MethodHandle mh=lookup.unreflect(method);
            //获取方法的类型
            MethodType type=mh.type();
            //将方法的实例对象类型加到方法类型工厂里
//            MethodType factoryType=MethodType.methodType(void.class);
            MethodType mt = MethodType.methodType(void.class);

            //移除方法里的实例对象类型
//            type=type.dropParameterTypes(0,1);
            //获取代理对象，注意，第二个参数的字符串必须为函数式接口里的方法名
            CallSite callSite = LambdaMetafactory.metafactory(lookup,"toOperate", mt, type, mh, type);
            MethodHandle methodHandle = callSite.getTarget();

            methodHandle.invoke(new Operation());

//            methodHandle.invokeExact(o);
//            operator.toOperate();
        } catch (Throwable throwable) {
            throwable.printStackTrace();
        }
    }
}
