package com.taotao.jvm.hotspot.src.share.vm.oops;

import lombok.Data;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Data
public class InstanceKlass extends Klass {
    /**
     * 魔数 u4 Class唯一标识
     */
    private byte[] magic = new byte[4];
    /**
     * 主版本号 u2
     */
    private byte[] minorVersion = new byte[2];
    /**
     * 次版本号 u2
     */
    private byte[] majorVersion = new byte[2];
    /**
     * 常量池数组
     */
    private ConstantPool constantPool;
    /**
     * 当前类的访问修饰符 u2
     */
    private int accessFlag;
    /**
     * 当前类的全限定名 u2
     */
    private int thisClass;
    /**
     * 当前类的父类的全限定名 u2
     */
    private int superClass;
    /**
     * 当前类所实现的接口数量 u2
     */
    private int interfacesLength;
    /**
     * 接口数组 u2
     */
    private List<InterfaceInfo> interfaceInfos = new ArrayList<>();
    /**
     * 类的成员变量数量 u2
     */
    private int fieldsLength;
    /**
     * 成员变量数组
     */
    private List<FieldInfo> fields = new ArrayList<>();
    /**
     * 方法数量 u2
     */
    private int methodLength;
    /**
     * 方法数组
     */
    private MethodInfo[] methods;
    /**
     * 属性数量
     */
    private int attributeLength;
    /**
     * 属性数组
     */
    private Map<String, AttributeInfo> attributeInfos = new HashMap<>();

    public InstanceKlass() {
        constantPool = new ConstantPool();

        constantPool.setKlass(this);
    }

    public void initMethodsContainer() {
        methods = new MethodInfo[methodLength];
    }

    @Override
    public String toString() {
        return "InstanceKlass{ }";
    }
}
