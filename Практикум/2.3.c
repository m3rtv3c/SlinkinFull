#include <stdio.h>
#include <math.h>
int main(int argc, char **argv)
{
    int n;
    printf("Введите n: ");
    scanf("%d",&n);
    for(int i=6;i<n;i++)
    {
        int sum=1;
        for(int j=2;j<sqrt(i);j++)	
        if(i%j==0){
			if(j!=i/j){
				sum+=j+(i/j);
				}
			else
				sum+=j;
			}
        if(sum==i) 
        printf("%d\n",sum);
	}
    return 0;
}
