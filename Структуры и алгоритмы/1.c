#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
const int MAX = 10;
const int MIN = 0;
typedef char matrix_t[10][11]; 

matrix_t a={	 
	"0001001000",
	"0010010000",
	"0001010111",
	"0000001100",
	"0001100000",
	"0000101111",
	"0000101000",
	"0000101000",
	"0001101100",
	"0010000100"
};
int depth = 0;
int currentDepth = 0;
void fill(int x,int y)
{
	if((x<MIN) || (x>MAX) || (y<MIN) || (y>MAX) || (a[y][x] != '0')){
		return;
	}
		a[y][x]='*';
		fill(x+1,y);
		fill(x-1,y);
		fill(x,y+1);
		fill(x,y-1);
		

}
void printout(matrix_t s)
{
	for (int i=0;i<10;i++)puts(s[i]);
	}

 int main(int argc, char **argv)
{
	fill(4,0);
	printout(a);
	return 0;
}


//-fsanitize=address

