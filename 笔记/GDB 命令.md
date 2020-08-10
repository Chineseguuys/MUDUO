## 一些比较偏但是很有用的命令

### x/ 

```
x/nfu
examine/nfu

n:表示要显示的字节的个数
f 表示显示的格式，常用的 x 表示16进制，d 表示 10进制，o 表示的是 8进制
u 表示一个地址单位的长度，往往和 n 进行配合使用。b 表示单字节，h 表示双字节，w 表示 4字节，g 表示四字节
```

#### 例子

```
(gdb) x/1xb 0x7fffffffd93c
0x7fffffffd93c:	0x55
(gdb) x/1xb 0x7fffffffd93d
0x7fffffffd93d:	0xff
(gdb) x/1xb 0x7fffffffd93e
0x7fffffffd93e:	0x55
(gdb) x/1xb 0x7fffffffd93f
0x7fffffffd93f:	0xff

```

> 这个命令很有用，非常方便的可以查看任意的虚拟内存空间内的数据值


### 查看数组存储的数据，并指定查看的长度

```c
(gdb) print *array@len
```
array 表示的是数组的首地址， len 表示要显示的长度

### 十六进制的方式查看一个变量的值

```c
(gdb) print/x variable_name
```

