package com.taotao.jvm.example.exception;

import java.io.FileNotFoundException;
import java.nio.file.FileAlreadyExistsException;

public class ExceptionHappen {

    public static void main(String[] args) throws FileNotFoundException, FileAlreadyExistsException {
        t1(true);
        t2(true);
    }

    public static void t1(boolean b) throws FileNotFoundException {
        if (b) {
            throw new FileNotFoundException();
        }
    }

    public static void t2(boolean b) throws FileAlreadyExistsException {
        if (b) {
            throw new FileAlreadyExistsException("Exception.txt");
        }
    }
}
