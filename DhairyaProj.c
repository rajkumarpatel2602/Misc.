/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

char matrix[10][10];
int col=10,row=10;
int row1,col1,row2,col2;
int pairMatch;
int matchedCase;
int gRow, gCol;
int topPosition;

void shiftNow(){

    int temp = topPosition;
    
    switch(matchedCase){

        case 3:
        case 4:
        case 6:
            for(int item=0, temp = topPosition; item<(topPosition); item++, temp--){
                matrix[temp+2][gCol] = matrix[temp-1][gCol];
            }
            for(int item=0; item<3; item++){
                matrix[item][gCol]=(rand()%4 + 35);
            }       
            break;
        
        case 1: 
            for(int item=0, temp = topPosition; item<(topPosition); item++, temp--){
                matrix[temp][gCol]    =matrix[temp-1][gCol];
                matrix[temp][gCol-1]  =matrix[temp-1][gCol-1];
                matrix[temp][gCol-2]  =matrix[temp-1][gCol-2];
            }
            
                matrix[0][gCol]      =(rand()%4 + 35);
                matrix[0][gCol-1]    =(rand()%4 + 35);
                matrix[0][gCol-2]    =(rand()%4 + 35);
            
            break;            
        
        case 2:
            for(int item=0, temp = topPosition; item<(topPosition); item++, temp--){
                matrix[temp][gCol]    =matrix[temp-1][gCol];
                matrix[temp][gCol+1]  =matrix[temp-1][gCol+1];
                matrix[temp][gCol+2]  =matrix[temp-1][gCol+2];
            }
            
                matrix[0][gCol]      =(rand()%4 + 35);
                matrix[0][gCol+1]    =(rand()%4 + 35);
                matrix[0][gCol+2]    =(rand()%4 + 35);
            
            break;
        
        case 5:
            for(int item=0, temp = topPosition; item<(topPosition); item++, temp--){
                matrix[temp][gCol-1]  =matrix[temp-1][gCol-1];
                matrix[temp][gCol]    =matrix[temp-1][gCol];
                matrix[temp][gCol+1]  =matrix[temp-1][gCol+1];
            }
            
                matrix[0][gCol-1]    =(rand()%4 + 35);
                matrix[0][gCol]      =(rand()%4 + 35);
                matrix[0][gCol+1]    =(rand()%4 + 35);
            
            break;
        
        default: printf("default case!\n"); break;
        
    }
    printmatrix();
}

void shiftMat(){
    
    if((matchedCase == 3) || (matchedCase == 4) || (matchedCase == 6)){
        printf("COLLUMN shift required\n");
        //colShift();//TBD
    } else {
        printf("ROW shift required\n");
        //rowShift();//TBD
    }
    
    if(pairMatch == 1){
        gRow = row1;
        gCol = col1;
    } else {
        gRow = row2;
        gCol = col2;
    }
    
    switch(matchedCase){
        case 1:
        case 2:
        case 5:
            topPosition = gRow;
            shiftNow();
            break;
        case 3://up shift 
            topPosition = gRow - 2;
            shiftNow();
            break;
        case 4://down shift
            topPosition = gRow;
            shiftNow();
            break;
        case 6://vertical central shift
            topPosition = gRow - 1;
            shiftNow();
            break;
    }
}

int vanishStrike(){
    int myRow, myCol;
    if(pairMatch == 1){
        myRow = row1;
        myCol = col1;
    }else{
        myRow = row2;
        myCol = col2;
    }
    printf("vanishing entry\n");
    switch(matchedCase){
        case 1://left check
            matrix[myRow][myCol]= 'X';
            matrix[myRow][myCol-1]= 'X';
            matrix[myRow][myCol-2]= 'X';
            break;
        case 2://right check
            matrix[myRow][myCol]= 'X';
            matrix[myRow][myCol+1]= 'X';
            matrix[myRow][myCol+2]= 'X';
            break;
        case 3://up check
            matrix[myRow][myCol]= 'X';
            matrix[myRow-1][myCol]= 'X';
            matrix[myRow-2][myCol]= 'X';
            break;
        case 4://down check
            matrix[myRow][myCol]= 'X';
            matrix[myRow+1][myCol]= 'X';
            matrix[myRow+2][myCol]= 'X';
            break;
        case 5://horizontal
            matrix[myRow][myCol-1]= 'X';
            matrix[myRow][myCol]= 'X';
            matrix[myRow][myCol+1]= 'X';
            break;
        case 6://vertical
            matrix[myRow-1][myCol]= 'X';
            matrix[myRow][myCol]= 'X';
            matrix[myRow+1][myCol]= 'X';
            break;
        default: printf("dummy statement\n");
    }
    printmatrix();
}

int printmatrix()
{
    int a,d;
 
    printf("Our output matrix is\n");
    for(a=0;a<row;a++)
    {
        for(d=0;d<col;d++)
        {
            printf("%c[%d][%d]\t",matrix[a][d], a, d);
        }
        printf("\n");
    }
}
void fillMat(){
    srand(time(NULL));
    int a,d;
    for(a=0;a<row;a++)
    {
        for(d=0;d<col;d++)
        {
            matrix[a][d] = ((rand()%4)+35);
            printf("%c", matrix[a][d]); //matrix[a][d]);
        }
    }
}

void checkPattern( ){
    
    int loopCount = 0;
    int myRow, myCol;
    bool patternMatched = false;
    
    printf("check swapped or not???\n");
    printmatrix();
    
    while((loopCount < 2) && !patternMatched){
        loopCount++;
        if(loopCount == 1){
            printf("First Iteration!\n");
            myRow = row1;
            myCol = col1;
        }
        else{
            printf("Second Iteration!\n");
            myRow = row2;
            myCol = col2;
        }
        
        for(int testCase=1; (testCase<=6) && (patternMatched == false); testCase++){
    
            switch(testCase){
                
                case 1://"left check"
                    if(myCol<2) break;
                    else{
                        printf("case1  row: %d, col : %d\n", myRow, myCol );
                        printf("\n%c %c %c\n", matrix[myRow][myCol],matrix[myRow][myCol-1],matrix[myRow][myCol-2]);
                        if ((matrix[myRow][myCol] == matrix[myRow][myCol-1]) && (matrix[myRow][myCol]== matrix[myRow][myCol-2]))
                        {
                            printf("left check done\n");
                            patternMatched = true;
                            matchedCase = 1;
                        }
                        break;
                    }
                    
                case 2://"right check"
                    if(myCol > ((col-1)-2)) break;
                    else{
                        printf("case2  row: %d, col : %d\n", myRow, myCol);
                        printf("\n%c %c %c\n", matrix[myRow][myCol],matrix[myRow][myCol+1],matrix[myRow][myCol+2]);
                        if ((matrix[myRow][myCol] == matrix[myRow][myCol+1]) && (matrix[myRow][myCol] == matrix[myRow][myCol+2]))
                        {
                            printf("right check done\n");
                            patternMatched = true;
                            matchedCase = 2;
                        }
                        break;
                    }
                    
                case 3://"upper check"
                    if(myRow<2) break;
                    else{
                        printf("case3  row: %d, col : %d\n", myRow, myCol);
                        printf("\n%c %c %c\n", matrix[myRow][myCol],matrix[myRow-1][myCol],matrix[myRow-2][myCol]);
                        if ((matrix[myRow][myCol] == matrix[myRow-1][myCol]) && (matrix[myRow][myCol] == matrix[myRow-2][myCol]))
                        {
                            printf("upper check done\n");
                            patternMatched = true;
                            matchedCase = 3;
                        }
                        break;
                    }
                    
                case 4://"down check"
                    if(myRow > ((row-1)-2)) break;
                    else{
                        printf("case4  row: %d, col : %d\n", myRow, myCol);
                        printf("\n%c %c %c\n", matrix[myRow][myCol],matrix[myRow+1][myCol],matrix[myRow+2][myCol]);
                        if ((matrix[myRow][myCol] == matrix[myRow+1][myCol]) && (matrix[myRow][myCol] == matrix[myRow+2][myCol])){
                            printf("down check done\n");
                            patternMatched = true;
                            matchedCase = 4;
                        }
                        break;
                    }
                    
                case 5://"horizontal check"
                    if((myCol ==0) || (myCol == (col-1))){
                        break;
                    }
                    else{
                        printf("case5  row: %d, col : %d\n", myRow, myCol);
                        printf("\n%c %c %c\n", matrix[myRow][myCol],matrix[myRow][myCol+1],matrix[myRow][myCol-1]);
                        if((matrix[myRow][myCol] == matrix[myRow][myCol+1]) && (matrix[myRow][myCol] == matrix[myRow][myCol-1])){
                            printf("horizontal check done\n");
                            patternMatched = true;
                            matchedCase = 5;
                        }
                        break;
                    }
                    
                case 6://"vertical check"
                    if((myRow ==0) || (myRow == (row-1))){
                        break;
                    }
                    else{
                        printf("case6  row: %d, col : %d\n", myRow, myCol);
                        printf("\n%c %c %c\n", matrix[myRow][myCol],matrix[myRow+1][myCol],matrix[myRow-1][myCol]);
                        if((matrix[myRow][myCol] == matrix[myRow+1][myCol]) && (matrix[myRow][myCol] == matrix[myRow-1][myCol])){
                            printf("verticle check done\n");
                            patternMatched = true;
                            matchedCase = 6;
                        }
                        break;
                    }
                    
                default: printf("default caught!"); break;
            }
            if(patternMatched == true){
                printf("pattern matched : matched pair %d\n", loopCount);
                pairMatch = loopCount;
                return;
            }
        }
    }
}

int swapposition()
{
   
here:   
   printf("Enter first position \n");
   scanf("%d",&row1);
   scanf("%d",&col1);
   printf("Enter second position \n");
   scanf("%d",&row2);
   scanf("%d",&col2);
   

   int temp = matrix[row1][col1];
   matrix[row1][col1] = matrix[row2][col2];
   matrix[row2][col2] = temp;
   
   if(( row1!=row2 ) && ( col1!=col2 )){
      printf("wrong input pal!\n TRY AGAIN!\n");
      printf("TIP :either row numbers or collumn numbers are allowed!\n");
      goto here;
   }
   
    if(matrix[row1][col1] == matrix[row2][col2])
   {
      printf("both symbols are same! sorry!\n");
      goto here;
   }
}

int main()
{
   printf("Enter the number  of rows and coloumn of matrix:->\n");
   scanf("%d%d", &row, &col);
   printf("%d %d\n",row,col);
   
   fillMat();
   printmatrix();
   while(1){
    swapposition();
    checkPattern();
    vanishStrike();
    shiftMat();
    printf("let's play again\n\n");
   }
   return 0;
}

