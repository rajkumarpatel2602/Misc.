#include<stdio.h>

main()
{
	int a[10][10];
	int i,j,rows,cols;
	srand(getpid());

	rows=sizeof(a)/sizeof(a[0]);
	cols=sizeof(a[0])/sizeof(a[0][0]);

	for(i=0;i<rows;i++,printf("\n"))
		for(j=0;j<cols;j++)
		{
			a[i][j]=rand()%100;
			printf("%3d",a[i][j]);
		}
}
