# Distributed Shared Memory based on SDN
This project aims to realise a distributed shared memory based on software defined network. The DSM is written in C++ and the SDN controller is written in python.
## Compile the static library
Has been illustrated in other documentation.
## The main Interfaces
share_int x;//创建int型共享对象x
x.create(const char *cname);//为共享对象指定标识符
x.get(const char *cname);//获取标识符为cname的共享对象
x=1;//用操作普通变量的方式操作对象：赋值
x=x+1//;用操作普通变量的方式操作对象：加法
x=x-1;// 用操作普通变量的方式操作对象：减法
x.Array(void *p,int size);//将size大小的int型数组写到对象中
x.(int x,int y);//为x中的array[x]赋值y
x.getValue();//获取x的值
x.newValue();//从主节点获取x值
share_char y;// 创建char型共享对象
y.create(const char *cname);// 为共享对象指定标识符
y.get(const char *cname);// 获取标识符为cname的共享对象
y=1; //用操作普通变量的方式操作对象：赋值
y=y+1;// 用操作普通变量的方式操作对象：加法
y=y-1; //用操作普通变量的方式操作对象：减法
y.Array(void *p,int size); //将size大小的char型数组写到对象中
y.(int x,char cy); //为y中的array[x]赋值c
y.getValue();//获取x的值
y.newValue();//从主节点获取x值
share_start();//起始函数
share_end();//结束函数
## Start a DSM system

#include”mylib.h”\\DSM的静态库头文件
#include”Python2.7/python.h”
#define share_p1 “share_project1”\\共享对象标识符
#define share_p2 “share_project2” \\共享对象标识符
int main(){
share_int p1;\\声明int型共享对象p1
share_char p2;\\声明char型共享对象p2
share_start();\\调用share_start()函数，创建本地链表,并发起一个线程
p1.create(share_p1);\\创建int型共享对象，为共享对象指定标识符share_p1，将共享对象设为仅本地可写，并将对象指针保存在share型对象k1中,将对象k1的对象指针与标识符share_p1加入到链表的一个节点中
p2.create(share_p2);\\ 创建char型共享对象，为共享对象指定标识符share_p2，将共享对象设为仅本地可写，并将对象指针保存在share型对象k2中,将对象k2的对象指针与标识符share_p2加入到链表的一个节点中
share_end();\\结束函数
} 
    

