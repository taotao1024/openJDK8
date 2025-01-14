package com.taotao.mock.java.time;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.time.ZoneId;
import java.time.ZoneOffset;
import java.util.Date;

public class LocalDateTimeTest {
    public static void main(String[] args) {
        LocalDateTime now = LocalDateTime.now();
        System.out.println("当前的时间: " + now);
        System.out.println("转换到日期: " + now.toLocalDate());
        System.out.println("转换到时间: " + now.toLocalTime());
        // 偏移8小时
        System.out.println("原始时间+8时: " + LocalDateTime.now(ZoneOffset.ofTotalSeconds(8 * 60 * 60)));
        System.out.println("当前时间+5天: " + now.plusDays(5));
        System.out.println("当前时间+1月: " + now.plusMonths(1));
        System.out.println("当前时间-5天: " + now.minusDays(5));
        System.out.println("当前时间-1月: " + now.minusMonths(1));
        // ISO-8601 规范定义 日期和时间用T分割
        System.out.println("解析后的时间: " + LocalDateTime.parse("2023-01-14T10:30"));
        System.out.println("解析后的日期: " + LocalDate.parse("2023-01-14"));
        System.out.println("解析后的时间: " + LocalTime.parse("10:30:10"));
        // 时间转换
        LocalDateTime specificDateTime = LocalDateTime.of(2025, 1, 14, 10, 30);
        System.out.println("特定时间: " + specificDateTime);
        // 时间组合
        LocalDate date = LocalDate.of(2025, 1, 14);
        LocalTime time = LocalTime.of(14, 10, 30);
        System.out.println("组合时间: " + LocalDateTime.of(date, time));
        // 时间调整
        System.out.println("时间调整" + LocalDateTime.now().withHour(0).withMinute(0).withSecond(0).withNano(0));
        System.out.println("调整月份" + LocalDateTime.now().withMonth(1));
        System.out.println("调整月份" + LocalDateTime.now().withYear(1));
        System.out.println("调整月份" + LocalDateTime.now().withDayOfMonth(1));
        // 时间对比
        System.out.println(LocalDate.now().isAfter(LocalDate.now().plusDays(1)));
        System.out.println(LocalDate.now().isBefore(LocalDate.now().plusDays(1)));
        // 时间戳 将 LocalDateTime 转换为 ZonedDateTime，指定时区
        long timestampInSeconds = LocalDateTime.now()
                .atZone(ZoneId.systemDefault())
                .toEpochSecond();
        long timestampInMilliseconds = LocalDateTime.now()
                .atZone(ZoneId.systemDefault())
                .toInstant()
                .toEpochMilli();
        // 输出结果
        System.out.println("时间戳（秒）: " + timestampInSeconds);
        System.out.println("时间戳（秒）: " + new Date(timestampInSeconds));
        System.out.println("时间戳（毫秒）: " + timestampInMilliseconds);
        System.out.println("时间戳（毫秒）: " + new Date(timestampInMilliseconds));
    }
}
