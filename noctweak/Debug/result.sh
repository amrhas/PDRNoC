#!/bin/bash

firvlues=(
0.01
0.03
0.05
0.08
0.1
0.12
0.15
0.18
0.2
0.22
0.25
0.28
0.3
0.32
0.35
0.38
0.4
0.42
0.45
0.48
0.5
0.52
0.55
0.58
0.6
0.62
0.65
0.68
0.7
0.72
0.75
0.78
0.8
)

reconfig_time=(
#570.34
#285.188
142.612
63.402
35.68
22.848
15.878
11.675
9.069
7.08
5.818
4.814
4.05
3.457
2.986
2.61
2.294
1.82
1.481
)

size_reconfig=(
# 2
# 3
# 4
# 5
# 6
# 7
# 8
# 9
# 10
# 11
# 12
13
14
15
16
18
20
)

for m in ${size_reconfig[@]}; do
for i in ${firvlues[@]}; do
echo $i
./noctweak -platform synthetic -dimx $m -dimy $m -fir $i -nvc 1 -routing xy -sim 100000 -plength fixed -length 10 -bsize $1 -freq 100 >> result_100Hz_bsize${1}_syn_$m.txt 
 #| grep "Average network"
 #| awk "/Average network|reconfig/"
done
done

# count=0
# for i in ${reconfig_time[@]}; do
# echo $(echo "scale=2; ${reconfig_time[${count}]}*1000" | bc)
 # ./noctweak -platform reconfig -dimx ${size_reconfig[${count}]} -dimy ${size_reconfig[${count}]} -fir .1 -routing xy -simmode packet -sim $((5*${2})) -rc_num $2 -plength fixed -length 10 -reconfig_time1 $(echo "scale=2; ${reconfig_time[${count}]}*1000" | bc) -fir_rc .1 -freq 100 >> result_reconfig_${1}.txt 
 # count=$(( ${count} + 1 ))
 # #| grep "Average network"
 # #| awk "/Average network|reconfig/"
# done
