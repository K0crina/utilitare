#!/bin/sh
echo 1
./tema task1 Ana -m -n=100 --file=teste/dir2/in1.txt --out=ref/data1.out
echo 2
./tema task1 Ana are mere --file=teste/dir1/dir4/in2.txt --out=ref/data2.out
echo 3
./tema task1 argument1 argument2 -ab --file=teste/dir1/in3.txt  --out=ref/data3.out
echo 4
./tema task1 argument2 -bc -d --file=teste/dir1/dir4/in4.txt --out=ref/data4.out
echo 5
./tema task1 argument --arg -b 100 --file=teste/in5.txt --out=ref/data5.out 
echo 6
./tema task2 teste --out=ref/data6.out
echo 7
./tema task2 teste -r --out=ref/data7.out
echo 8
./tema task2 teste -h --out=ref/data8.out
echo 9
./tema task2 teste -rh --out=ref/data9.out
echo 10
./tema task2 teste/dir1/ --out=ref/data10.out
echo 11
./tema task3 ./teste/dir1/dir4/dir6 ./teste/dir2 --out=ref/data11.out
echo 12
./tema task3 ./teste/dir1/dir4/dir7 ./teste/dir1/dir4/dir8 --out=ref/data12.out
echo 13
./tema task3 ./teste/dir3/dir9 ./teste/dir3 --out=ref/data13.out
echo 14
./tema task3 ./teste/dir1 ./teste/dir1/dir4/dir6 --out=ref/data14.out
echo 15
./tema task3 ./teste/dir1/dir4 ./teste/dir3/dir9 --out=ref/data15.out
echo 16
./tema task4 ./teste/dir1/dir8/data16_file1.in ./teste/dir1/dir8/data16_file2.in  --out ref/data16.out
echo 17
./tema task4 ./teste/dir1/dir8/dir10/data17_file1.in ./teste/dir1/dir8/dir10/data17_file2.in  --out ref/data17.out
echo 18
./tema task4 ./teste/dir1/dir4/dir6/dir11/data18_file1.in ./teste/dir1/dir4/dir6/dir11/data18_file2.in --out ref/data18.out
echo 19
./tema task4 ./teste/dir3/data19_file1.in ./teste/dir3/data19_file2.in --out ref/data19.out
echo 20
./tema task4 ./teste/dir3/dir9/data20_file1.in ./teste/dir3/dir9/data20_file2.in --out ref/data20.out