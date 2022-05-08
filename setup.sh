#!/bin/bash

make clean
make
echo "------------------------"
echo "共享内存实现有界缓冲区"
echo "1.输入1进行单步测试"
echo "2.输入2进行自动测试"
echo "------------------------"
while read -p "请输入:" name
do
	if [ $name == 1 ] || [ $name == 2 ];then
		break
	fi
done

if [ $name == 1 ];then
	gnome-terminal --window -- ./producer
	gnome-terminal --window -- ./consumer
else
	gnome-terminal --window -- ./mul_test
	gnome-terminal --window -- ./mul_test
fi

ipcrm --all
