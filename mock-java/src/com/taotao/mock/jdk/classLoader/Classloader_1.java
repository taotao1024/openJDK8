package com.taotao.mock.jdk.classLoader;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

/**
 *  结果：
 *      class com.qimingnan.classloader.Classloader_1_A
 *      sun.misc.Launcher$AppClassLoader@14dad5dc
 *
 *  没有走我们自定义的类加载器，为什么呢？
 *  因为双亲委派模型，自定义类加载器的父类能够加载到这个类
 */
public class Classloader_1 extends ClassLoader {

    public static void main(String[] args) {
        Classloader_1 classloader = new Classloader_1();

        try {
            Class<?> clazz = classloader.loadClass(Classloader_1_A.class.getName());

            System.out.println(clazz);
            System.out.println(clazz.getClassLoader());
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    public static final String SUFFIX = ".class";

    @Override
    protected Class<?> findClass(String className) throws ClassNotFoundException {
        System.out.println("Classloader_1 findClass");

        byte[] data = getData(className.replace('.', '/'));

        return defineClass(className, data, 0, data.length);
    }

    private byte[] getData(String name) {
        InputStream inputStream = null;
        ByteArrayOutputStream outputStream = null;

        File file = new File(name + SUFFIX);
        if (!file.exists()) return null;

        try {
            inputStream = new FileInputStream(file);
            outputStream = new ByteArrayOutputStream();

            int size = 0;
            byte[] buffer = new byte[1024];

            while ((size = inputStream.read(buffer)) != -1) {
                outputStream.write(buffer, 0, size);
            }

            return outputStream.toByteArray();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                inputStream.close();
                outputStream.close();
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        }

        return null;
    }
}

class Classloader_1_A {

}