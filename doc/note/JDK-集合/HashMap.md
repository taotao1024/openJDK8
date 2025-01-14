## JDK-1.7的HashMap

![JDK-1.7HashMap.png](../../images/JDK-集合/JDK-1.7HashMap.png)

## JDK-1.8的HshMap

![JDK-1.8HashMap.png](../../images/JDK-集合/JDK-1.8HashMap.png)

当 Hash 冲突严重时，在桶上形成的链表会变的越来越长，这样在查询时的效率就会越来越低；时间复杂度为 O(N)。
因此 1.8 中重点优化了这个查询效率。