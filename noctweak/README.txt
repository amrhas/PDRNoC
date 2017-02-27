NoCTweak: an On-Chip Network Simulator

Written on: 2012/03/25
By: Anh Tran, UC Davis

Please send an email to anhtrandavis@gmail.com to ask for your permission
before using this open-source code;
and please cite the below paper in your related publications:
Anh Tran and Bevan Baas,
"NoCTweak: a Highly Parameterizable Simulator for Early Exploration of
Performance and Energy of Networks On-Chip," 
Technical Report, VCL Lab, ECE Department, UC Davis, July 2012.

Wormhole router with optional pipeline stages:
	- 1 stage: 
		. type 1:	BF+LRC/SA+ST+LT
	- 2 stages: 
		. type 2_1: BF | LRC/SA+ST+LT
		. type 2_2: BF+LRC/SA | ST+LT
		. type 2_3: BF+LRC/SA+ST | LT
	- 3 stages: 
		. type 3_1: BF | LRC/SA | ST+LT
		. type 3_2: BF | LRC/SA+ST | LT
		. type 3_3: BF+LRC/SA | ST | LT
	- 4 stages: 
		. type 4:	BF | LRC/SA | ST | LT
	- 5 (not necessary)
	
+ Features;
	- report packet delay, network throughput
	- report average router power and its each components
	- report global clock tree + PLL depending on the number of routers
	- report energy per packet	

+ Updated:
	- 2012/06/26:
		. input voltage 
		. max frequency (min clock period) changes corresponding to the pipeline type and voltage 


