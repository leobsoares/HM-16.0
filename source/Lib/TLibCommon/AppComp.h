/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AppComp.h
 * Author: leo
 *
 * Created on 20 de Janeiro de 2016, 15:53
 */

#ifndef __APPCOMP__
#define __APPCOMP__

//defines the type of approximation
// 0- no approximation
// 1- trunc_adder
// 2- trunc_adder_with_carry
// 3- copy_adder
// 4- aca_adder
// 5- etaI_adder
// 6- etaII_adder
// 7- etaIV_adder
// 8- zeros
#define APPSATD4 0 

//defines the approximation parameter k
// 1- one bit (or coefficients)
// 2- two bits (or coefficients)
// 3- three bits (or coefficients)
// 4- four bits (or coefficients)
// 5- five bits (or coefficients)
// 6- six bits (or coefficients)
// 7- seven bits (or coefficients)
// 8- eigth coefficients
// 9- nine coefficients
// 10- ten coefficients
// 11- eleven coefficients
// 12- twelve coefficients
// 13- thirteen coefficients
// 14- fourteen coefficients
// 15- fifteen coefficients
#define KSATD4 15

//defines the number of bits

#define nbits 32

inline int trunc_adder(int x, int y, int k){
//truncation adder
//performs the addition between x + y and truncates the k LSB
	
	return ((x>>k)+(y>>k))<<k;

}

inline int trunc_adder_with_carry(int x, int y, int k){
//truncation adder with carry estimation
//performs the addition between x + y, truncates the k LSB, and estimates carry in for the precise addition by copying one of the operands
	
	int carry = 1 << (k-1);
	
	carry = (carry & x) << 1;
	
	return (((x>>k)+(y>>k))<<k) + carry;
}
	
inline int copy_adder(int x, int y, int k){
//copy adder with carry estimation
//performs the addition between x + y, copies the k LSB from one of the operands to the k LSB sum bits, and estimates the carry in for the precise addition by copying one of the operands

	int carry = 1 << (k-1);
	int app_sum = (0xFFFFFFFF >> (nbits-k)) & x;
	
	carry = (carry & x) << 1;
	
	return (((x>>k)+(y>>k))<<k) + app_sum + carry;

}	

inline int aca_adder(int x, int y, int k){
//almost correct adder
//performs the adition between x + y considering n-k+1 blocks of k bits
	
	int i;
	int nadders = nbits - k;
	int current_adder = 0;
	int sum_bit = 1 << k; 
	int sum_result = 0;
		
	for(i=0;i<k;i++){
		current_adder = (current_adder << 1) + 1;
	}
	

	sum_result = (x + y) & current_adder;

	for(i=0;i<nadders;i++){
		current_adder = current_adder << 1;
		sum_result = sum_result + (((x & current_adder) + (y & current_adder)) & sum_bit);
		sum_bit = sum_bit << 1;
	}	

	return sum_result;
}

inline int etaI_adder(int x, int y, int k){
//error-tolerant adder I
//performs the addition between x + y and doesn't use carry chain propagation in k LSB addition. The rule for the k LSB approximate addition is to compute each bit from the left to the right direction by using half adders until the first carry generate is found. When this occurs the remaining LSB bits are set to "1"

	int g, p;
	int control = 0;
	int check = 1 << (k-1);
	int i;	
	int sum_result = 0;

	sum_result = ((x >> k) + (y >> k)) << k;
	
	g = ((x & y) << (nbits-k)) >> (nbits-k);
	p = ((x ^ y) << (nbits-k)) >> (nbits-k);
	
	for(i=0;i<k;i++){
		if((g & check) == check || control == 1){
			control = 1;
			sum_result = sum_result + check;
		}
		else{
			sum_result = sum_result + (p & check);
		}
		check = check >> 1;
	}

	return sum_result;
}

inline int etaII_adder(int x, int y, int k){
//error-tolerant adder II
//performs the adition between x + y considering n/k-1 blocks of 2k bits and a k-bit adder
	
	int i;
	int nadders = nbits/k-1;
	int current_adder = 0;
	int current_sum = 0;
	int sum_result = 0;
		
	for(i=0;i<k;i++){
		current_sum = (current_sum << 1) + 1;
		current_adder = (current_adder << 1) + 1; 
	}
	
	for(i=0;i<k;i++){
		current_adder = (current_adder << 1) + 1; 
	}

	sum_result = (x + y) & current_sum;

	for(i=0;i<nadders;i++){
		current_sum = current_sum << k;
		sum_result = sum_result + (((x & current_adder) + (y & current_adder)) & current_sum);
		current_adder = current_adder << k;
	}	

	return sum_result;
}

inline int etaIV_adder(int x, int y, int k){
//error-tolerant adder IV
//performs the adition between x + y considering  a k-bit adder, n-k/2k adders of 2k bits, and a residual adder
	
	int i;
	int nadders = (nbits-k)/(2*k);
	int residual = (nbits-k)%(2*k);
	int initial_adder = 0;
	int current_adder = 0;
	int current_sum = 0;
	int residual_adder = 0;
	int residual_sum = 0;
	int sum_result = 0;
		
	for(i=0;i<k;i++){
		initial_adder = (initial_adder << 1) + 1;
		current_adder = (current_adder << 1) + 1;
		current_sum = (current_sum << 1) + (1 << k);
	}
	
	for(i=0;i<k;i++){		
		current_sum = (current_sum << 1) + (1 << k);
		current_adder = (current_adder << 1) + 1; 
	}

	for(i=0;i<k;i++){
		current_adder = (current_adder << 1) + 1;
		residual_adder = (residual_adder << 1) + (1 << (nbits-residual-k));
	}

	for(i=0;i<residual;i++){
		residual_adder = (residual_adder << 1) + (1 << (nbits-residual-k));
		residual_sum = (residual_sum << 1) + (1 << (nbits-residual));
	}

	sum_result = (x + y) & initial_adder;

	for(i=0;i<nadders;i++){
		sum_result = sum_result + (((x & current_adder) + (y & current_adder)) & current_sum);
		current_adder = current_adder << (2*k);
		current_sum = current_sum << (2*k);
	}	
	
	sum_result = sum_result + (((x & residual_adder) + (y & residual_adder)) & residual_sum);

	return sum_result;
}

#endif /* __APPCOMP__ */

