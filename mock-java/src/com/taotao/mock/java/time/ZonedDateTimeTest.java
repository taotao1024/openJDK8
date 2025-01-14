package com.taotao.mock.java.time;

import java.time.Instant;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Date;

public class ZonedDateTimeTest {
    public static void main(String[] args) {
        ZonedDateTime shanghai = ZonedDateTime.now(ZoneId.of("Asia/Shanghai"));
        System.out.println("上海当前时间: " + shanghai);
        System.out.println(DateTimeFormatter.RFC_1123_DATE_TIME
                .format(ZonedDateTime.now()));
        // 将 LocalDateTime 转换为 ZonedDateTime 并指定时区 输出成时间戳
        Instant instant = LocalDateTime.now()
                .atZone(ZoneId.systemDefault())
                .toInstant();
        System.out.println(instant.getEpochSecond());
        // 将时间戳转换为 Date 并输出
        System.out.println("转换后的 Date: " + Date.from(instant));

        // LocalDateTime+ZoneId
        LocalDateTime ldt = LocalDateTime.of(2024, 1, 14, 18, 4, 30);
        ZonedDateTime zdt1 = ldt.atZone(ZoneId.systemDefault());
        ZonedDateTime zdt2 = ldt.atZone(ZoneId.of("America/New_York"));
        System.out.println("组合时间 " + zdt1);
        System.out.println("组合时间 " + zdt2);
    }
}
