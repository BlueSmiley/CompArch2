#include <stdio.h>
#include <math.h>
#include <stdbool.h>

//assume word of size 4
#define WORD_SIZE 4;

typedef struct cache{
	int n;
	int k;
	int l;
} cache;

typedef struct set{
	int* tags;
	bool* data;
	int* times;
}set;

set* makeSets(cache c){
	set* r = (set*)calloc(c.n, sizeof(set));
	for(int i = 0; i < c.n; i++){
		//k tags and corresponding booleans to see if data has been loaded
		r[i].tags = (int*)calloc(c.k, sizeof(int));
		r[i].data = (bool*)calloc(c.k, sizeof(bool));
		r[i].times = (int*)calloc(c.k, sizeof(int));
	}
	return r;
}


//free memory used if ur a responsible c programmer. I'm not :)
void findHits(cache c,int* address, int address_len){
	set* r = makeSets(c);
	int hits = 0;
	int misses = 0;
	int offset_len = c.l/WORD_SIZE;
	int setno_len = 0;
	//brute force to get log2 of n
	for(int acc = 1; acc < c.n ; acc = acc*2){
		setno_len++;
	}
	
	for( int i = 0; i < address_len ; i++){
		int addr = address[i];
		addr = addr >> offset_len; // we can ignore offset length
		int mask = 0;
		for( int temp = setno_len; temp > 0; temp--){
			mask = (mask << 1) + 1;
		}
		int setno = addr & mask;
		set s = r[setno];
		int tag = addr >> setno_len;
		bool miss = true;
		int min = -1;
		int min_index = 0;
		
		
		for( int j = 0; j< c.k ; j++){
			if(s.tags[j] == tag && s.data[j] == true){
				miss = false;
				s.times[j] = i; //update lru timestamp
				break;
			}
			if(min == -1 || s.times[j] < min){
				min = s.times[j];
				min_index = j;
			}
		}
		
		if(miss){
			misses++;
			//evict lru data and update timestamp
			s.times[min_index] = i;
			s.data[min_index] = true;
			s.tags[min_index] = tag;
		}
		else{
			hits++;
		}
		
	}
	printf("Hits:%d, Misses:%d \n",hits,misses);
	
	return;
}
	
int main(){
	int address[] = 
		{0x0000,0x0004,0x000c,0x2200,0x00d0,0x00e0,0x1130,0x0028,
		0x113c,0x2204,0x0010,0x0020,0x0004,0x0040,0x2208,0x0008,
		0x00a0,0x0004,0x1104,0x0028,0x000c,0x0084,0x000c,0x3390,
		0x00b0,0x1100,0x0028,0x0064,0x0070,0x00d0,0x0008,0x3394};
	int len = sizeof(address)/sizeof(address[0]);
	cache c;
	c.n = 8;
	c.k = 1;
	c.l =16;
	findHits(c,address,len);
	c.n = 4;
	c.k = 2;
	c.l =16;
	findHits(c,address,len);
	c.n = 2;
	c.k = 4;
	c.l =16;
	findHits(c,address,len);
	c.n = 1;
	c.k = 8;
	c.l =16;
	findHits(c,address,len);
	return 0;
}
