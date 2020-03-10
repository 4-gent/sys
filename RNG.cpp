/*File Name: RNG.c
*
*This is meant to create a random integer
*
*Author: Mj Burog
*/

#include <stdio.h>
#include <unistd.h>
#include <cstdlib>

int main(){
	int num = rand() % 9000 + 1000;
	int x = 0;
	
	while (x < 100){
	printf("%d\n", num);
	x += 1;
	num = rand() % 9000 + 1000;
	}
return 0;
}
