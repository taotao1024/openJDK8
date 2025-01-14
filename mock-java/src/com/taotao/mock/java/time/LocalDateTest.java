package com.taotao.mock.java.time;

import java.time.LocalDate;

/**
 * JDK 6：使用 java.util.Date 和 java.util.Calendar 类来处理时间和日期。这些类存在许多设计缺陷，使得时间的处理变得复杂和容易出错。<P>
 * JDK 8：引入了新的时间 API，位于 java.time 包中，主要包括 LocalDate, LocalTime, LocalDateTime, ZonedDateTime, Instant 等类。
 * 这些类是不可变的，线程安全，设计更为直观。
 */
public class LocalDateTest {
    public static void main(String[] args) {
        LocalDate today = LocalDate.now();
        System.out.println("今天时间: " + today);

        LocalDate futureDate = today.plusDays(5);
        System.out.println("今天时间+5: " + futureDate);
    }
}