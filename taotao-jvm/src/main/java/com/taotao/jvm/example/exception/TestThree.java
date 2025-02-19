package com.taotao.jvm.example.exception;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.concurrent.TimeUnit;

public class TestThree {

    public static void main(String[] args) throws InterruptedException, FileNotFoundException {
        TimeUnit.SECONDS.sleep(1);

        File text = new File("text");
        FileInputStream fileInputStream = new FileInputStream(text);
    }
}
