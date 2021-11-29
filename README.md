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

- [ ] 字符串的乘法（连续+）

- [ ] 

