package com.taotao.jvm.example.exception;

import java.io.FileNotFoundException;
import java.nio.file.FileAlreadyExistsException;

public class ExceptionHappen2 {

    public static void main(String[] args) {
        try {
            t1(true);
            t2(true);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (FileAlreadyExistsException e) {
            e.printStackTrace();
        }
    }

    public static void t1(boolean b) throws FileNotFoundException {
        if (b) {
            throw new FileNotFoundException();
        }
    }

    public static void t2(boolean b) throws FileAlreadyExistsException {
        if (b) {
            throw new FileAlreadyExistsException("ziya.txt");
        }
    }
}
