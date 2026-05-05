#include <stdio.h>
#include <math.h>
#define IS_TRIANGLE(a, b, c) ((a)+(b)>(c) && (b)+(c)>(a) && (a)+(c)>(b))
#define IS_ACUTE_ANGLE(a, b, c) (\
	((a) >= (b) && (a) >= (c) ) ? ((b)*(b) + (c)*(c) > (a)*(a) ) : \
	((b) >= (a) && (b) >= (c) ) ? ((a)*(a) + (c)*(c) > (b)*(b) ) : \
	((a)*(a) + (b)*(b) > (c)*(c))))

int is_triangle(int a, int b,int c){
	if ((a+b>c) && (b+c > a) && (a+c >b)){
		printf("Треугольник существует\n");
		return 1;
		}
	else{
		printf("Треугольника не существует\n");
		return 0;
		}
}

int is_acute_angle(int a, int b, int c) {
    if (((a*a+b*b) > c*c) && ((b*b + c*c) > a*a) && ((a*a + c*c) > b*b)) {
        printf("Треугольник остроугольный\n");
        return 1;
	}
    else {
		printf("Треугольник не остроугольный\n");
		return 0;
	}	
}
int main(void) {
	
	int a,b,c;
	printf("Введите стороны треугольника\n");
	scanf("%d %d %d", &a, &b, &c);
	printf("-------------------------\n");
	printf("Проверка с помощью макросов:\n");
	if (IS_TRIANGLE(a, b, c)) {
		printf("Треугольник существует\n");
		if ((IS_ACUTE_ANGLE(a, b, c)) {
			printf("Треугольник остроугольный\n");
		} else {
			printf("Треугольник не остроугольный\n");
		}
	} else {
		printf("Треугольника не существует\n");
	}
	printf("-------------------------\n");
	printf("Проверка с помощью функций:\n");
	
	if (is_triangle(a,b,c)) {
    is_acute_angle(a,b,c);
	}
}
//5 4 6
