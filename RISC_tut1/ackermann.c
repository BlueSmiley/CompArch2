#define _POSIX_C_SOURCE 200809L

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

int proc_calls = 0; 
int curr_window_depth = 0;
int max_window_depth = 0;
int overflows_6 = 0;
int underflows_6 = 0;
int overflows_8 = 0;
int underflows_8 = 0;
int overflows_16 = 0;
int underflows_16 = 0;

int used_register_6 = 1;
int used_register_8 = 1;
int used_register_16 = 1;
	
int ackermann(int x, int y){
	proc_calls++;
	curr_window_depth++;
	used_register_6++;
	used_register_8++;
	used_register_16++;
	
	if(curr_window_depth > max_window_depth)
		max_window_depth = curr_window_depth;
		
	if(used_register_6>5){
		used_register_6--;
		overflows_6++;
	}
	if(used_register_8>7){
		used_register_8--;
		overflows_8++;
	}
	if(used_register_16>15){
		used_register_16--;
		overflows_16++;
	}
	
	int res;
	int tempres;
	if (x == 0) {
		res = y+1;
		
		used_register_6--;
		used_register_8--;
		used_register_16--;
		if(used_register_6<=0){
			used_register_6++;
			underflows_6++;
		}
		if(used_register_8<=0){
			used_register_8++;
			underflows_8++;
		}
		if(used_register_16<=0){
			used_register_16++;
			underflows_16++;
		}
		return res;
	} else if (y == 0){ 
		res = ackermann(x-1, 1);
		
		curr_window_depth--;
		
		used_register_6--;
		used_register_8--;
		used_register_16--;
		if(used_register_6<=0){
			used_register_6++;
			underflows_6++;
		}
		if(used_register_8<=0){
			used_register_8++;
			underflows_8++;
		}
		if(used_register_16<=0){
			used_register_16++;
			underflows_16++;
		}
		return res;
	} else
	 { 
		tempres = ackermann(x, y-1);
		
		curr_window_depth--;
		res = ackermann(x-1, tempres);

		curr_window_depth--;
		
		used_register_6--;
		used_register_8--;
		used_register_16--;
		if(used_register_6<=0){
			used_register_6++;
			underflows_6++;
		}
		if(used_register_8<=0){
			used_register_8++;
			underflows_8++;
		}
		if(used_register_16<=0){
			used_register_16++;
			underflows_16++;
		}
		return res;
	} 
} 

int main(){
struct timespec spec;

clock_gettime(CLOCK_MONOTONIC, &spec);
double starttime = spec.tv_sec*1000 + round(spec.tv_nsec/1.0e6);
ackermann(3,6);
clock_gettime(CLOCK_MONOTONIC, &spec);
double endtime = spec.tv_sec*1000 + round(spec.tv_nsec/1.0e6);
double elapsedtime = endtime - starttime;
printf("calls:%d max depth:%d overflows:%d %d %d underflows:%d %d %d\n",
	proc_calls,max_window_depth,overflows_6,overflows_8,overflows_16,
	underflows_6,underflows_8,underflows_16);
printf("%lf",elapsedtime);
return 0;
}

//calls:172233 
//max depth:511 
//overflows6:84885 
//overflows8:83911
//overflows16:80142 
//underflows6:84885
//underflows8:83911 
//underflow16:80142