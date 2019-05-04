#include<stdio.h>
#include<stdint.h>

/* just a comment added */

main()
{
	int a[3][4]={11,22,33,44,55,66,77,88,99};
	int rows,cols,i,j;
	//printf("a=%d\n\n",sizeof(a));
	//printf("b=%d\n\n",sizeof(a[0]));
	//printf("c=%d\n\n",sizeof(a[0][0]));
	rows=sizeof(a)/sizeof(a[0]);
	cols=sizeof(a[0])/sizeof(a[0][0]);
	
	for(i=0;i<rows;i++,printf("\n"))
		for(j=0;j<cols;j++)
			printf("%3d",a[i][j]);
}
