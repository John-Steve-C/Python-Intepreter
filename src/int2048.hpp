#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

// Integer 1:
// 实现一个有符号的大整数类，只需支持简单的加减

// Integer 2:
// 实现一个有符号的大整数类，支持加减乘除，并重载相关运算符

// 请不要使用除了以下头文件之外的其它头文件
#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <complex>

// 请不要使用 using namespace std;

namespace sjtu {
    class int2048 {
        // todo
    public:
        int len, flag;//len是压位后的数组长度!
        //num[] store data:from 0 to len-1
        //flag = 1(+/0), -1(-)
        std::vector<long long> num;
        static const int base = 1000000000;
        static const int width = 9;

        // 构造函数,以及它的重载
        int2048() {
            num.clear();
            num.push_back(0);//默认初值为0
            len = 1;
            flag = 1;
        }

        int2048(long long x) {
            num.clear();
            len = 1;
            if (x == 0) {
                num.push_back(0);
                flag = 1;
            } else {
                if (x < 0) {
                    flag = -1;
                    x = -x;
                } else flag = 1;//0默认为+0
                while (x) {
                    num.push_back(x % base);
                    x /= base;
                    len++;
                }
                len--;
            }
        }

        int2048(const std::string &s) {
            read(s);
        }

        int2048(const int2048 &temp) {//复制构造函数
//            format(temp);
//            std::memset(num,0,sizeof(num));
            num.clear();
//            num.push_back(0);
            len = temp.len;
            flag = temp.flag;

            for (int i = 0; i <= len - 1; ++i) {
                num.push_back(temp.num[i]);
            }
        }

        // 以下给定函数的形式参数类型仅供参考，可自行选择使用常量引用或者不使用引用
        // 如果需要，可以自行增加其他所需的函数
        // =================================== Integer1 ===================================

        // 读入一个大整数
        void read(const std::string &s) {

            num.clear();
            len = 1;
            if (s == "0") {
                num.push_back(0);
                flag = 1;
                return;
            }

            if (s[0] == '-')
                flag = -1;
            else
                flag = 1;
            int l = s.length(), k = 0;
            long long t = 0;

            if (flag > 0) {
                len = l / width + (l % width ? 1 : 0);
                for (int i = l - 1; i >= 0; i -= width) {
                    k = i - width + 1;
                    if (k < 0) k = 0;//防止越界
                    t = 0;
                    for (int j = k; j <= i; ++j)//压位存储
                        t = t * 10 + s[j] - '0';
                    num.push_back(t);
                }
            } else {
                l--;
                len = l / width + (l % width ? 1 : 0);
                for (int i = l; i >= 1; i -= width) {//s[0]不用考虑
                    k = i - width + 1;
                    if (k < 1) k = 1;
                    t = 0;
                    for (int j = k; j <= i; ++j)
                        t = t * 10 + s[j] - '0';
                    num.push_back(t);
                }
            }
            if (num[len - 1] == 0) flag = 1;//
        }

        // 输出储存的大整数
        void print() {
            if (len == 1 && num[0] == 0) {
                printf("0");
                return;
            }//prevent '-0'

            if (flag == -1) printf("-");
            printf("%lld", num[len - 1]);//最高位不用补0
            for (int i = len - 2; i >= 0; --i) {//中间要补0
                printf("%09lld", num[i]);//输出9位,不够的会补0
                //"%4d"则是输出(末尾)4位,不够的补空格
            }
        }

        //比较绝对值大小
        int cmp(int2048 a, int2048 b) {//abs compare
            // a>b-->1,a<b-->-1,a=b-->0
            int la = a.len, lb = b.len;
            if (la > lb) return 1;
            if (la < lb) return -1;
            for (int i = la - 1; i >= 0; --i) {
                if (a.num[i] > b.num[i]) return 1;
                if (a.num[i] < b.num[i]) return -1;
            }
            return 0;
        }

        //交换
        void swap(int2048 &a, int2048 &b) {
            int2048 temp;
            temp = a;
            a = b;
            b = temp;
        }

        //无符号加(保证ans = abs(a) + abs(b) )
        int2048 &ADD(const int2048 &b) {
            long long x = 0;
            for (int i = 0; i < len || i < b.len; ++i) {
                long long temp;
                if (i >= len) num.push_back(0);//扩展下标,补0
                if (i >= b.len)
                    temp = num[i] + x;//防止vector下标越界
                else
                    temp = num[i] + b.num[i] + x;//x进位
                x = temp / base;
                temp %= base;
                num[i] = temp;
            }
            if (len < b.len) len = b.len;
            if (x != 0) {
                len++;
                num.push_back(x);
            }

            return *this;
        }

        //无符号减(保证ans = abs(a) - abs(b) )
        int2048 &MINUS(const int2048 &b) {
            for (int i = 0; i < len || i < b.len; ++i) {
                long long temp;
                if (i >= b.len)//同理,防止下标越界
                    temp = num[i];
                else
                    temp = num[i] - b.num[i];
                if (temp < 0) {
                    temp += base;
                    num[i + 1]--;
                }
                num[i] = temp;
            }
            while (!num[len - 1] && len - 1 > 0) {//len = 1时不用删0
                len--;
                num.pop_back();//注意要把空间也释放掉,以保证push_back位置正确
            }

            return *this;
        }

        // 加上一个大整数
        int2048 &add(const int2048 &b) {//要考虑符号,4种情况
            if (flag * b.flag > 0) {//a*b>0,不变号
                ADD(b);
            } else {//a,b异号
                int f = cmp(*this, b);

                if (f == 1) {//和的符号 与 绝对值大的 相同
                    MINUS(b);
                } else {
                    int2048 t(b);
                    swap(*this, t);
                    MINUS(t);
                }
            }
            return *this;//必须有返回值
        }

        // 输出两个大整数之和
        friend int2048 add(int2048 a, const int2048 &b) {
            int2048 c(a);
            c.add(b);
            return c;
        }

        // 减去一个大整数
        int2048 &minus(const int2048 &b) {//注意分类讨论的严密性,
            int f = cmp(*this, b);
            if ((f == 1) && (flag * b.flag > 0)) {  //a>b>0 or a<b<0
                MINUS(b);
            } else if ((f == -1) && (flag * b.flag > 0)) {//b>a>0 or b<a<0
                int2048 t(b);
                t.flag *= -1;//变号
                swap(*this, t);
                MINUS(t);
            } else if (f == 0) { //abs(a) = abs(b)
                if (flag * b.flag > 0) // ab同号,相减为0
                    MINUS(b);
                else ADD(b); //ab异号,绝对值相加,符号不变
            } else {//a>0,b<0 or a<0,b>0
                ADD(b);//不变号
            }

            return *this;//必须有返回值
        }

        // 输出两个大整数之差
        friend int2048 minus(int2048 a, const int2048 &b) {
            int2048 c(a);
            c.minus(b);
            return c;
        }

        // =================================== Integer2 ===================================

        //=赋值,把右侧的值赋给左侧(不需要重载也可以),
        // 不知道为什么连等(a=b=1)会segmentation fault
        /*int2048 &operator=(const int2048 &b){
            std::memset(num,0,sizeof(num));//要记得清空num!
            flag = b.flag;
            len = b.len;
            for (int i = 1; i <= len;++i)
                num[i] = b.num[i];
        }*/

        //等价于a.add(b)
        int2048 &operator+=(int2048 b) {
            return add(b);
        }

        friend int2048 operator+(int2048 a, const int2048 &b) {
            int2048 c(a);
            c.add(b);
            return c;
        }

        //等价于a.minus(b)
        int2048 &operator-=(int2048 b) {
            return minus(b);//不是void的函数一定要有返回值！！！不然会RE
        }

        friend int2048 operator-(int2048 a, const int2048 &b) {
            int2048 c(a);
            c.minus(b);
            return c;
        }

        //高精度乘法
        int2048 &operator*=(const int2048 &b) {//20*15不对
            int i, j;
            int2048 ans(0);//必须新开一个ans来临时存储乘积,否则会把原本的a覆盖

            if (*this == int2048(0) || b == int2048(0)) {
                ans.flag = 1;
                ans.len = 1;
                return *this = ans;
            }
            if (flag * b.flag > 0) ans.flag = 1;//符号判断
            else ans.flag = -1;

            ans.num.resize(len + b.len, 0);//重设容器的大小
            for (i = 0; i < len; ++i) {//列竖式时b在上,a在下
                for (j = 0; j < b.len; ++j) {
                    ans.num[i + j] += num[i] * b.num[j];//注意下标
                    ans.num[i + j + 1] += ans.num[i + j] / base;//进位x
                    ans.num[i + j] %= base;
                }
            }

            while (!ans.num.back() && ans.num.size() > 1) {//前导0
                ans.num.pop_back();
            }
            ans.len = ans.num.size();

            return *this = ans;//赋值
        }

        friend int2048 operator*(int2048 a, const int2048 &b) {
            int2048 c(a);
            c *= b;
            return c;
        }

        int2048 &operator/=(const int2048 &divisor) {
//            if (b == 0) return error;

            int2048 ans(0), a(*this), b(divisor), product(0), multiple(0);
            int maxsize = a.len - b.len;
            b.flag = a.flag = 1;//无符号运算

            if (a < b) {
                if (flag * divisor.flag > 0)
                    return *this = int2048(0);
                else
                    return *this = int2048(-1);//注意取整问题
            }

            while (a >= b) {
                multiple.num.clear();
                multiple.num.resize(maxsize + 1, 0);
                multiple.num[maxsize] = 1;//multiple = 10^(maxsize-1)
                multiple.flag = 1;
                multiple.len = maxsize + 1;

                product = b * multiple;//把b移动到当前位置

                /*while (a >= product) {
                    a -= product;//用减法来模拟试商
                    ans += multiple;//当前位数的统计
                }*/

                int l = 0, r = base - 1, mid;//用二分来试商
                int2048 cur = 0, t;//试出来的商
                while (l <= r) {
                    mid = (l + r) >> 1;
                    t = int2048(mid);
                    if (t * product <= a) {
                        cur = cur >= t ? cur : t;
                        l = mid + 1;
                    } else r = mid - 1;
                }
                a -= product * cur;//用二分来试商
                ans += multiple * cur;//当前位数的统计

                maxsize--;
            }

            if (flag * divisor.flag > 0) ans.flag = 1;//符号判断
            else {
                ans.flag = -1;
                if (ans * divisor != *this) ans -= 1;//手动下取整,负数需要特判
            }

            return *this = ans;
        }

        friend int2048 operator/(int2048 a, const int2048 &b) {
            int2048 c(a);
            c /= b;
            return c;
        }

        //重载输入输出-流运算符
        friend std::istream &operator>>(std::istream &input, int2048 &b) {
            std::string s;
            input >> s;
            b.read(s);

            return input;
        }

        friend std::ostream &operator<<(std::ostream &output, const int2048 &b) {
            int2048(b).print();//重新声明一个int2048
//            b.print(); //不能这样写,因为b是const,而print函数会修改他的值

            return output;
        }

        inline friend bool operator==(const int2048 &a, const int2048 &b) {
            if (a.len != b.len) return 0;
            if (a.flag != b.flag) return 0;
            for (int i = 0; i < a.len; ++i)
                if (a.num[i] != b.num[i]) return 0;

            return 1;
        }

        inline friend bool operator!=(const int2048 &a, const int2048 &b) {
            if (a == b) return 0;
            else return 1;
        }

        inline friend bool operator<(const int2048 &a, const int2048 &b) {
            if (a.flag < 0 && b.flag > 0) return 1;
            if (a.flag > 0 && b.flag < 0) return 0;
            if (a.flag < 0 && b.flag < 0) {
                if (a.len < b.len) return 0;
                if (a.len > b.len) return 1;

                for (int i = a.len - 1; i >= 0; --i) {//负数与正数相反,
                    if (a.num[i] < b.num[i]) return 0;
                    if (a.num[i] > b.num[i]) return 1;

                }

                return 0;//a==b
            } else {//a>0,b>0
                if (a.len > b.len) return 0;
                if (a.len < b.len) return 1;

                for (int i = a.len - 1; i >= 0; --i) {//正数,从最高位到最低位进行比较
                    if (a.num[i] < b.num[i]) return 1;
                    if (a.num[i] > b.num[i]) return 0;
                }

                return 0;//a==b
            }

        }

        inline friend bool operator>(const int2048 &a, const int2048 &b) {
            if (a.flag < 0 && b.flag > 0) return 0;
            if (a.flag > 0 && b.flag < 0) return 1;
            if (a.flag < 0 && b.flag < 0) {
                if (a.len < b.len) return 1;
                if (a.len > b.len) return 0;

                for (int i = a.len - 1; i >= 0; --i) {//负数与正数相反,
                    if (a.num[i] < b.num[i]) return 1;
                    if (a.num[i] > b.num[i]) return 0;

                }

                return 0;//a==b
            } else {//a>0,b>0
                if (a.len > b.len) return 1;
                if (a.len < b.len) return 0;

                for (int i = a.len - 1; i >= 0; --i) {//正数,从最高位到最低位进行比较
                    if (a.num[i] < b.num[i]) return 0;
                    if (a.num[i] > b.num[i]) return 1;
                }

                return 0;//a==b
            }
        }

        inline friend bool operator<=(const int2048 &a, const int2048 &b) {
            if (a > b) return 0;
            else return 1;
        }

        inline friend bool operator>=(const int2048 &a, const int2048 &b) {
            if (a < b) return 0;
            else return 1;
        }

        //重载int2048->double的类型转换,直接按位转换即可
        operator double() const {
            double ans = 0;
            for (int i = 0; i < len; ++i) {
                //num存的是压位后的数据！要乘base
                ans = ans * base + num[i];
            }
            if (flag == -1)
                return -ans;
            else
                return ans;
        }
    };
}

#endif