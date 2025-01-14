时间API，主要如下

- 本地日期和时间类：LocalDateTime，LocalDate，LocalTime；
- 带时区的日期和时间类：ZonedDateTime；
- 时刻类：Instant；
- 时区：ZoneId，ZoneOffset；
- 时间间隔：Duration。

### Instant [InstantTest.java](..%2F..%2F..%2Fmock-java%2Fsrc%2Fcom%2Ftaotao%2Fmock%2Fjava%2Ftime%2FInstantTest.java)

获取时间戳 ```Instant.now()``` 、```System.currentTimeMillis()``` 其中 ```Instant``` 额外提供如下功能

1. plusXxx()和minusXxx()增减方法，方便我们进行时间的操作。
2. 附加上一个时区，创建出对应的ZonedDateTime对象。
3. 关联上指定的ZoneId，得到对应的ZonedDateTime，进而获得对应时区的LocalDateTime。
4. 提供LocalDateTime、ZoneId、Instant、ZonedDateTime之间互相转换

### LocalDateTime [LocalDateTimeTest.java](..%2F..%2F..%2Fmock-java%2Fsrc%2Fcom%2Ftaotao%2Fmock%2Fjava%2Ftime%2FLocalDateTimeTest.java)

LocalDateTime是JDK 8之后出现的，用来表示本地日期和时间的类。
通过now()方法，默认获取到本地时区的日期和时间。与之前的旧API不同，LocalDateTime、LocalDate和LocalTime默认会严格按照ISO-8601规定的日期和时间格式进行打印。

1. 当前时间、时间、日期 now()
2. 指定时区、时间加减 pushXxx()、minusXxx()
3. 时间转换、组合 parse()、of()
4. 时间调整 withXxx()、with()
5. 时间对比 isBefore()、isAfter() [DurationTest.java](..%2F..%2F..%2Fmock-java%2Fsrc%2Fcom%2Ftaotao%2Fmock%2Fjava%2Ftime%2FDurationTest.java)

### ZonedDateTime [ZonedDateTimeTest.java](..%2F..%2F..%2Fmock-java%2Fsrc%2Fcom%2Ftaotao%2Fmock%2Fjava%2Ftime%2FZonedDateTimeTest.java)

ZonedDateTime相当于是LocalDateTime + ZoneId，其中ZoneId是java.time引入的新的时区类，它与旧的java.util.TimeZone是有区别的。在ZonedDateTime中也提供了plusDays()等加减操作，使用起来也非常地方便