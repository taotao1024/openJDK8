package com.taotao.jvm.hotspot.src.share.vm.oops;

import lombok.Data;

import java.util.ArrayList;
import java.util.List;

@Data
public class InstanceKlass extends Klass {
    /**
     * 魔数
     */
    private byte[] magic = new byte[4];
    /**
     * 主版本号
     */
    private byte[] minorVersion = new byte[2];
    /**
     * 次版本号
     */
    private byte[] majorVersion = new byte[2];
    /**
     * 常量池
     */
    private ConstantPool constantPool;
    /**
     * 访问标志
     */
    private int accessFlag;
    private int thisClass;
    private int superClass;

    private int interfacesLength;
    private List<InterfaceInfo> interfaceInfos = new ArrayList<>();

    private int fieldsLength;
    private List<FieldInfo> fields = new ArrayList<>();

    private int methodLength;
    private MethodInfo[] methods;

    private int attributeLength;
    private List<AttributeInfo> attributeInfos = new ArrayList<>();

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
