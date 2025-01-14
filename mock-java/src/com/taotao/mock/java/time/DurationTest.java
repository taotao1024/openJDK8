package com.taotao.mock.java.time;

import java.time.Duration;
import java.time.LocalDate;
import java.time.LocalDateTime;

public class DurationTest {

    public static void main(String[] args) {
        // 时间间隔
        LocalDateTime time1 = LocalDateTime.of(2021, 1, 1, 14, 30, 10);
        LocalDateTime time2 = LocalDateTime.of(2025, 1, 14, 15, 36, 20);
        // PT35377H6M10S 表示35377小时6分钟10秒
        System.out.println("时间间隔: " + Duration.between(time1, time2));

        // 日期间隔
        LocalDate date1 = LocalDate.of(2021, 1, 1);
        LocalDate date2 = LocalDate.of(2025, 1, 14);
        // P4Y13D 表示4年13天
        System.out.println("日期间隔: " + date1.until(date2));
    }

}
