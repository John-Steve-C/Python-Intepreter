## 调试过程小记

- [x] 理解题意（11.26）

  ~~虽迟了2个星期，但到~~。就是接下来的一星期要多花时间，晚点做bookstore

  虽然我是从模仿开始的，但是接下来我逐渐掌握了方法。

- [x] New_Any类大致完成（11.28）

- [x] bad_cast 问题，因为antlrcpp::Any.as之前没有 if ，is进行判断，as的类型必须与变量的定义相同（或者用auto）

  问题的原因：Query时，应该判断x是不是变量名（string）！如果没有这么做，就会以数字作为变量名去查询变量

  ```cpp
  //todo:用 给定的 Any 类的 x 去构造对应的、正确的 New_Any
  New_Any EvalVisitor::Query(antlrcpp::Any x){
      //先判断x是不是变量名，不是就直接构造
      if (! x.is<string>()) { return x.as<New_Any>(); }
      //再去查询该变量是否存在
      auto temp = scope.varQuery(x.as<string>() );//pair
      if ( temp.first ) {//x对应的变量是否存在
          return temp.second;
      }
      else return New_Any();
  }
  ```

​		但是似乎问题不是这个？如果函数的定义是antlrcpp::Any, 返回一个New_Any 也是合法的？也就是说，New_Any转Any是合法的？

- [x] True标识符的判断，应当输出True而不是1

- [ ] 字符串存储时，不能带有原本两端的 ” “ 

  先进行格式化

- [x] 判断读入的类型，优先级：double>int>bool>string

- [x] 比较大小出错，原因有二：

  1. 没有把Any转化成New_Any，而重载的运算符只对New_Any生效，无法处理Any

  2. 赋值的时候要进行dfs递归处理，

     ```cpp
     正确的写法是：
     New_Any l = Query(visitArith_expr(arithArray[0]) );
     而不是
     New_Any l = Query(arithArray[0] );
     ```

     注意到antlr的实现是**对语法树的dfs**，先从根节点到子节点，再从子节点一个个返回。

---

#### 第一次提交：2021.11.29

![](https://s3.bmp.ovh/imgs/2021/11/1a28dbf69b4b8907.png)
注意到，新的大作业Bookstore 已经发布（QWQ) 

能做多少就做多少吧... **吃一堑长一智**

---

- [x] 字符串的乘法（连续+），输出为整数？

  两个参数都要判断，哪个才是字符串！

- [x] not 运算没有判断（仍然是Query）

---

2021.12.3  01:00  in dormitory

- [x] double 与 int2048 之间的加减（经过了好几天）

​		一部分是赋值存取的问题，12.0存进来会变成12，类型改变（通过setprecision解决，设定输出的位数）

​		是因为int2048转换为double没有重载吗！！事实上已经在int2048.hpp里面写了。

​		**问题在于：num[i]中存放的是压位后的8位数，不需要 -'0' ，同时不是乘10，而是乘base！！ ** ~~所以不能直接照抄代码啊QWQ~~

- [x] 负号的重载（前缀）和负数的计算（int 与 int 之间没有问题）。

  注意factor的存取，visitFactor函数要重写

  问题还是出在int 与 double 之间，又是转换函数的问题。

- [x] 整除，向下取整问题。

  首先，不能直接重载 // 运算符。使用转义字符\ 或者 **特判**。

- [x] double 与 int2048 之间的整除与浮点除

- [ ] 大整数类会 TLE , 明天咨询一下 zbl 如何解决。他已经拿到了20pts

