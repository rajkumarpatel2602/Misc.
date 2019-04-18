/* just basics of 2D array function calling */


#include<stdio.h>
#include<string.h>

main()
{
	unsigned int a[4][3]={{10,20,30},{40,50,60},{70,80},{90}};
	//printf("%u\n",&(a[1][1]+1));
	//printf("%u\n",&(a[1]+1));//address of constant number is not allowed
	
	printf("%u\n",&a[1][1]+1);//address of constant number allowed
	printf("%u\n",&a[1]+1);//||
	//p1=a;
	//p2=a[0];
	//p3=a[0][0];
	int *p[5]
}
