# 独占锁
synchronized关键字、自定义Lock接口
## synchronized关键字
 - synchronized关键字，由JVM实现的单机锁，锁依赖于Java对象里对象头的Mark Word。
   - Mark Word(标记字段)：大小是8个字节。默认存储的是对象的hashCode、锁状态标识以及GC分代年龄。 [Java对象](https://blog.csdn.net/weixin_44817884/article/details/136679299)
   - 通过模板解析器和C++实现
 - 每个对象自从创建起，都会关联一个锁对象，即Monitor对象【管程\监视器】，[objectMonitor.hpp](../../../openjdk-8u40/hotspot/src/share/vm/runtime/objectMonitor.hpp)。
 - 每个 Java 对象都可以关联一个 Monitor 对象，如果使用 synchronized 给对象上锁（重量级）之后，该对象头的 Mark Word 中就被设置指向 Monitor 对象的指针。
 - Synchronized锁配合Object的wait和notify等方法来实现线程通信

### 工作原理
![ObjectMonitor工作原理.png](../../images/JDK-%E9%94%81/ObjectMonitor%E5%B7%A5%E4%BD%9C%E5%8E%9F%E7%90%86.png)
 - 1、当多个线程同时访问一段同步代码时。首先假设线程A会进入EntryList队列中，会判断owner是否为空，主要通过CAS操作(比较和交换，比较新值和旧值的不同)。
 - 2、如果owner为null，直接把其赋值，指向自己owner=self，同时把可重入次数recursions=1，count+1获取锁成功。如果self=cur，说明是当前线程，
      锁重入了，recursions++即可。线程A进入owner区域，然后执行同步方法块；
 - 3、若线程B来获取锁。首先会放入EntryList队列中。然后去判断锁是否被占用，此时线程A正在使用该锁，那么会一直放在EntryList队列中，直到线程A释放锁，
      所有EntryList队列中竞争锁是非公平的；
 - 4、若持有monitor的线程调用wait()方法，将释放当前持有的monitor，owner变量恢复为null，count自减1，同时该线程进入WaitSet集合中等待被唤醒；
 - 5、线程从WaitSet集合中被唤醒notifyall后，会放入到EntryList队列中,参与锁的竞争；

### 锁升级
偏向锁
轻量级锁
重量级锁 mutex

锁在升级的过程中 会过渡到无锁情况

### Synchronized针对同步代码块和同步方法
 - 同步代码块：
   - 对象头会关联到一个monitor对象。进入一个方法的时候执行monitorEnter(同步代码块的开始位置)，获取当前对象的一个所有权_owner，monitor数值加1，
     当前这个线程就是monitor的owner，退出的时候对应monitorexit(插入到方法结束处和异常处)。monitorenter和monitorexit是一对，缺一不可。
   - 如果已经拥有owner，再次获得锁(可重入)，计数器加1，执行monitorexit时，计数器减1；
   - 互斥性体现在：是否能够获得monitor的所有权
 - 同步方法
    - 与同步代码块类似，多了一个标识位ACC_SYNCHRONIZED,一旦执行方法的时候，就会先判断是否存在 标志位，然后ACC_SYNCHRONIZED会隐式的调用
      monitorenter和monitorexit。归根到底，还是monitor的争夺。


## Lock接口
 - java.util.concurrent.locks.Lock完全是由Java编写，提供了锁获取和释放的控制权、可中断的获取锁以及超时获取锁等多种高级特性。
 - Lock只是一个接口，常见的实现类有：ReentrantLock（重入锁）、ReadLock（读锁）、WriteLock（写锁）；
   - 底层都是通过聚合了一个java 同步器(AbstractQueueSynchronizer, AQS)来完成线程的访问控制的。
 - ReentrantLock锁配合Condition实现多个条件下的线程通信

### AQS
 - 用来构建锁或其他同步组件的基础框架，使用了一个java volatile int state成员变量表示同步状态(代表共享资源)，通过内置的FIFO双向队列来完成资源获取线程的排队工作；
 - 定义为volatile能够保证多线程下的可见性，当state = 1时代表当前对象锁已经被占有，其他线程加锁会失败。加锁失败的线程会被放入一个FIFO队列中.
 - [AbstractQueuedLongSynchronizer.java](../../../openjdk-8u40/jdk/src/share/classes/java/util/concurrent/locks/AbstractQueuedLongSynchronizer.java)

### AQS 实现原理
 - 底层是一个FIFO双向队列，队列上是一个个节点Node，Node是内部类，封装了一些信息，主要有
   ```
   static final class Node {
      /**
       * 1. signal = -1, 后继节点的线程处于等待状态。如果当前线程释放了同步状态，
       * 将会通知后继节点，使后继节点的线程得以运行
       * 2. condition = -2, 节点在【等待队列】中，节点线程等待在Condition上，当其它
       * 线程对Condition调用了signal()方法后，该节点将会从等待队列中转移到同步队列，
       * 加入对同步状态的获取中
       */
      volatile int waitStatus;
      volatile Node prev;
      volatile Node next;
      // 获取同步状态的线程
      volatile Thread thread; 
      // 等待队列中的后继节点
      Node nextWaiter;
   }
   ```
 - 锁获取 public final void acquire(long arg)、acquireQueued(addWaiter(Node.EXCLUSIVE), arg))
 - 锁释放 public final boolean release(long arg)、protected boolean tryRelease(long arg)

# 多线程工具
 - 多线程工具 
   - Semaphore信号量
     - 使用场景：限流，尤其是公共资源有限的应用场景，例如数据库连接，停车场车位数等。
   - CountDownLatch闭锁
     - 使用场景：数据迁移，可以使一个或一批线程在闭锁上等待，等到其他线程执行完相应操作后，闭锁打开，这些等待的线程才可以继续执行
   - CyclicBarrier循环屏障
     - CyclicBarrier和CountDownLatch很相似，都可以使线程先等待然后再执行。不过CountDownLatch是使一批线程等待另一批线程执行完后再执行；
       而CyclicBarrier只是使等待的线程达到一定数目后再让它们继续执行。
     - 使用场景：游戏准备

# 分布式锁
基于数据库的版本控制、基于Redis的锁(redisson)、基于Zookeeper的锁(Curator)

# 参考
[ava中锁机制以及底层原理](https://blog.csdn.net/weixin_44817884/article/details/136677898)