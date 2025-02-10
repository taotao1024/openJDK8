package com.taotao.jvm.hotspot.src.share.vm.intepreter;

import com.taotao.jvm.hotspot.src.share.vm.classfile.DescriptorStream2;
import com.taotao.jvm.hotspot.src.share.vm.memory.StackObj;
import com.taotao.jvm.hotspot.src.share.vm.oops.ConstantPool;
import com.taotao.jvm.hotspot.src.share.vm.oops.MethodInfo;
import com.taotao.jvm.hotspot.src.share.vm.runtime.JavaThread;
import com.taotao.jvm.hotspot.src.share.vm.runtime.JavaVFrame;
import com.taotao.jvm.hotspot.src.share.vm.runtime.StackValue;
import com.taotao.jvm.hotspot.src.share.vm.utilities.BasicType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class BytecodeInterpreter extends StackObj {

    private static Logger logger = LoggerFactory.getLogger(BytecodeInterpreter.class);

    public static void run(JavaThread thread, MethodInfo method) {
        // 得到字节码指令
        BytecodeStream code = method.getAttributes()[0].getCode();

        // 得到栈帧
        JavaVFrame frame = (JavaVFrame) thread.getStack().peek();

        int c;

        while (!code.end()) {
            c = code.getU1Code();

            switch (c) {
                case Bytecodes.LDC: {
                    logger.info("执行指令: LDC");

                    // 取出操作数
                    int operand = code.getU1Code();

                    // 取出常量池中的信息
                    int tag = method.getBelongKlass().getConstantPool().getTag()[operand];
                    switch (tag) {
                        case ConstantPool.JVM_CONSTANT_Float: {
                            break;
                        }
                        case ConstantPool.JVM_CONSTANT_String: {
                            int index = (int) method.getBelongKlass().getConstantPool().getDataMap().get(operand);

                            String content = (String) method.getBelongKlass().getConstantPool().getDataMap().get(index);
                            // 获取到操作数栈
                            frame.getStack().push(new StackValue(BasicType.T_OBJECT, content));

                            break;
                        }
                        case ConstantPool.JVM_CONSTANT_Class: {
                            break;
                        }
                        default: {
                            logger.error("未知类型");

                            break;
                        }
                    }

                    break;
                }
                case Bytecodes.RETURN: {
                    logger.info("执行指令: RETURN");

                    // pop出栈帧
                    thread.getStack().pop();

                    logger.info("\t 剩余栈帧数量: " + thread.getStack().size());

                    break;
                }
                case Bytecodes.GETSTATIC: {
                    logger.info("执行指令: GETSTATIC");

                    // 获取操作数
                    short operand = code.getUnsignedShort();

                    String className = method.getBelongKlass().getConstantPool().getClassNameByFieldInfo(operand);
                    String fieldName = method.getBelongKlass().getConstantPool().getFieldName(operand);

                    try {
                        Class<?> clazz = Class.forName(className.replace('/', '.'));

                        Field field = clazz.getField(fieldName);

                        frame.getStack().push(new StackValue(BasicType.T_OBJECT, field.get(null)));
                    } catch (ClassNotFoundException e) {
                        e.printStackTrace();
                    } catch (NoSuchFieldException e) {
                        e.printStackTrace();
                    } catch (IllegalAccessException e) {
                        e.printStackTrace();
                    }

                    break;
                }
                /**
                 * 调用虚方法
                 * 1、public修饰
                 * 2、protected修饰
                 */
                case Bytecodes.INVOKEVIRTUAL: {
                    logger.info("执行指令: INVOKEVIRTUAL");

                    // 取出操作数
                    short operand = code.getUnsignedShort();

                    // 获取类名、方法名、方法描述符
                    String className = method.getBelongKlass().getConstantPool().getClassNameByMethodInfo(operand);
                    String methodName = method.getBelongKlass().getConstantPool().getMethodNameByMethodInfo(operand);
                    String descriptorName = method.getBelongKlass().getConstantPool().getDescriptorNameByMethodInfo(operand);

                    /**
                     * 判断是系统类还是自定义的类
                     *  系统类走反射
                     *  自定义的类自己处理
                     */
                    if (className.startsWith("java")) {
                        DescriptorStream2 descriptorStream = new DescriptorStream2(descriptorName);
                        descriptorStream.parseMethod();

                        Object[] params = descriptorStream.getParamsVal(frame);
                        Class[] paramsClass = descriptorStream.getParamsType();
                        // this 不能作为参数 去取
                        Object obj = frame.getStack().pop().getObject();

                        try {
                            Method fun = obj.getClass().getMethod(methodName, paramsClass);

                            /**
                             * 处理：
                             *  1、无返回值
                             *  2、有返回值
                             */
                            if (BasicType.T_VOID == descriptorStream.getReturnElement().getType()) {
                                fun.invoke(obj, params);
                            } else {
                                descriptorStream.pushField(fun.invoke(obj, params), frame);
                            }
                        } catch (NoSuchMethodException e) {
                            e.printStackTrace();
                        } catch (IllegalAccessException e) {
                            e.printStackTrace();
                        } catch (InvocationTargetException e) {
                            e.printStackTrace();
                        }
                    } else {

                    }

                    break;
                }
                default: {
                    throw new Error("无效指令");
                }
            } /* end switch */
        } /* end while */
    }

}