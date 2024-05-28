#!/usr/bin/python3

#输入的默认是字符串

str = input("请输入:")

#所以有时候斋要强转数据
num1 = input("请输入第一个数:")
num2 = input("请输入第二个数:")
print(str)
print(num1+num2)
             
num1 = int(num1)
num2 = int(num2)
print(num1+num2)
