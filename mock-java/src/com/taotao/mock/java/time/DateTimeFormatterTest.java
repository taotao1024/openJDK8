package com.taotao.mock.java.time;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;
import java.time.format.FormatStyle;

public class DateTimeFormatterTest {
    public static void main(String[] args) {
        LocalDateTime now = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        String formattedDateTime = now.format(formatter);
        System.out.println("格式化后的时间: " + formattedDateTime);


        System.out.println(DateTimeFormatter.ISO_LOCAL_DATE_TIME.format(LocalDateTime.now()));
        System.out.println(DateTimeFormatter.BASIC_ISO_DATE.format(LocalDateTime.now()));
        System.out.println(DateTimeFormatter.RFC_1123_DATE_TIME.format(ZonedDateTime.now()));

        System.out.println("格式化后的时间: " + LocalDateTime.now()
                .format(DateTimeFormatter.ofLocalizedDateTime(FormatStyle.SHORT)));

        System.out.println("格式化后的时间: " + LocalDateTime.now()
                .format(DateTimeFormatter.ofLocalizedDateTime(FormatStyle.MEDIUM)));

        System.out.println("格式化后的时间: " + LocalDateTime.now()
                .format(DateTimeFormatter.ofLocalizedDateTime(FormatStyle.LONG)));

        System.out.println("格式化后的时间: " + ZonedDateTime.now()
                .format(DateTimeFormatter.ofLocalizedDateTime(FormatStyle.FULL)));

        System.out.println(LocalDate.parse("2025 01 14", DateTimeFormatter.ofPattern("yyyy MM dd")));
    }
}
