#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

COORD coord= {0,0}; // a global variable which determines origin
static int xys[119][119];//static variable to store x and y
void initialize()
{
    for (int i = 0; i < 119; i++)
        for (int j = 0; j < 119; j++)
            xys[i][j] = 127;
}
void gotoxy(int x,int y)// function that moves a cursor to required position
{
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

int random_range(int l,int u)//generates random integer within a given range
{
    int t;
    t=rand();
    t%=(u-l+1);
    t+=l;
    return t;
}

void print_random(int n, int N) //function that generates a sample of N occurrence of the breaking the stick at random points and writes into a text file BreakingCoordinates.txt
{
    FILE *f1 = fopen("BreakingCoordinates.txt","w");
    if(f1==NULL)
    {
        printf("Error");
        exit(1);
    }
    int x,y,i=0;
    srand((unsigned)time(0));//seed the random function with the initial time
    x=random_range(1,n-1);//randomly generate x-coordinate
    y=random_range(1,n-1);//randomly generate y-coordinate
    fprintf(f1,"%d%c%d",x,',',y);//write to text file
    for(;i<N-1;i++)
    {
        x=random_range(1,n-1);//randomly generate x-coordinate
        y=random_range(1,n-1);//randomly generate y-coordinate
        fprintf(f1,"%c%d%c%d",'\n',x,',',y);//write to text file
    }
    fclose(f1);
}
int istriangle(int a,int b,int c,int n)//checks if a triangle will be formed or not
{
    int max;
    max=( a > b ) ? a :b;
    max=( c > max ) ? c :max;
    if(max<=n/2)//the max should be less than n/2 in order to form a triangle
        return 1;
    else
        return 0;
}
void print_result(int n) //function takes the file BreakingCoordinates.txt and generates the sides of triangle then determines if the sides will form a triangle and writes into a file TrianleYesOrNo.txt
{
    FILE *f1 = fopen("BreakingCoordinates.txt","r");
    FILE *f2 = fopen("TriangleYesOrNo.txt","w");
    int x,y,a,b,c;
    if(f1==NULL||f2==NULL)
    {
        printf("Error");
        exit(1);
    }
    while(!feof(f1))
    {
        fscanf(f1,"%d,%d",&x,&y);
        a=( x > y ) ? y :x; //determines
        c=n-x-y+a;          // the
        b=n-a-c;            //sides
        fprintf(f2,"%d%c%d-----%d%c%d%c%d---",x,',',y,a,',',b,',',c);//writes the side to text file
        if(istriangle(a,b,c,n)==1)
            fprintf(f2,"y\n");
        else
            fprintf(f2,"n\n");
    }
    fclose(f1);
    fclose(f2);
}

void SetColor(int col);//change color according to color code

void plot(int n, int option)
{
    system("cls");//clears screen
    FILE *f1 = fopen("BreakingCoordinates.txt","r");
    if(f1==NULL)
    {
        printf("Error");
        exit(1);
    }
    int x,y,a,b,c;
    while(!feof(f1))
    {
        fscanf(f1,"%d,%d",&x,&y);
        a=( x > y ) ? y :x; //determines
        c=n-x-y+a;          // the
        b=n-a-c;            //sides
        if(n>119)
        {                                //scales
            x*=119;                     //the
            x=x/(n-1);                  //range
            y*=119;                     //according
            y=y/(n-1);
            n=119;                  //to screen
        }
        y=n-y;//rotates y and shifts it in order to place origin at the bottom left position
        gotoxy(x,y);//goes to point (x,y)
        if(istriangle(a,b,c,n)==1)
        {
            SetColor(2);//change color to green if triangle is formed
            xys[x][y]=0;
        }
        else
        {
            SetColor(4);//change color to red if triangle is formed
            xys[x][y]=255;
        }
        printf("*");//print '*' at the corresponding point
        if(option>0)
            option=1;
        Sleep(100*option);
    }
    gotoxy(0,n+1);
    SetColor(4);
    printf("*--Triangle not possible\n");   //legend
    SetColor(2);                            //of the
    printf("*--Triangle possible");         //plot
    SetColor(15);//change color back to white
}
void saveplot(int N)//save the plot as pgm file
{
    int i, j, temp = 0;
    FILE* plotfile;
    plotfile = fopen("plotfile.pgm", "wb");

    // Writing Magic Number to the File
    fprintf(plotfile, "P2\n");

    // Writing Width and Height
    fprintf(plotfile, "%d %d\n", N, N);

    // Writing the maximum gray value
    fprintf(plotfile, "255\n");
    int count = 0;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            temp = xys[i][j];

            // Writing the gray values in the 2D array to the file
            fprintf(plotfile, "%d ", temp);
        }
        fprintf(plotfile, "\n");
    }
    fclose(plotfile);
}

int main()
{
    int n,N,option;
    char enter;
menu:printf("What do you want to do?\n1.Plot for a specific value of n\n2.Test the program for different cases\nEnter your choice ");
    scanf("%d",&option);//generate menu
    if(option==1)
    {
stlen:  printf("\nEnter stick length ");
        scanf("%d",&n);
        if(n<=1)
        {
            printf("Stick length should be greater than 1");//handle exception of stick length
            goto stlen;
        }
        printf("\nEnter the number of trials ");
        scanf("%d",&N);
        printf("\nDo you want to look at plot on real time?(1/0)");
        scanf("%d",&option);
        initialize();
        print_random(n,N);
        print_result(n);
        plot(n,option);
        saveplot(n);
        scanf("%c",&enter);//waits till
        scanf("%c",&enter);//enter is pressed
        system("cls"); //clears screen

    }
    else if(option==2)
    {
        int i;
        printf("\nEnter the number of tests(If you want to manually populate the file then press 0) ");
        scanf("%d",&N);
        if(N>0)
        {
            FILE *f1 = fopen("Test.txt","w");
            printf("Now enter the nos.");
            scanf("%d",&n);
            fprintf(f1,"%d",n);
            for(i=0;i<N-1;i++)
            {
                scanf("%d",&n);
                fprintf(f1,",%d",n);
            }
            fclose(f1);
        }
        else
        {
            printf("Guidelines for manual input to file:\n1.Please provide only integers greater than 1\n2.The numbers should be comma separated\n3.DON'T give an extra charecter after the last no.\nOnce You are ready hit enter");
            scanf("%c",&enter);//waits till
            scanf("%c",&enter);//enter is pressed
        }
        FILE *f1 = fopen("Test.txt","r");
        fscanf(f1,"%d,",&n);
        if(n<=1)
        {
            printf("Stick length should be greater than 1");//handle exception of stick length
            goto loop;
        }
        printf("\nEnter the number of trials for %d ",n);
        scanf("%d",&N);
        print_random(n,N);
        print_result(n);
        printf("Please check the file. Hit enter to continue");
        scanf("%c",&enter);//waits till
loop:   scanf("%c",&enter);//enter is pressed
        while(!feof(f1))
        {
            int pos;
            pos=ftell(f1);
            fscanf(f1,"%d,",&n);
            if(pos==ftell(f1))
            {
                printf("You enter wrong charecter. Please try again with comma separated numbers.\nAborting.....");//handles exception of wrong charecter
                break;
            }
            if(n<=1)
            {
                printf("Stick length should be greater than 1");//handle exception of stick length
                continue;
            }
            printf("\nEnter the number of trials for %d ",n);
            scanf("%d",&N);
            print_random(n,N);
            print_result(n);
            printf("Please check the file. Hit enter to continue");
            scanf("%c",&enter);//waits till
            scanf("%c",&enter);//enter is pressed
        }
        fclose(f1);
    }
    else
    {
        printf("Enter a valid option");//handle an exception of option
        goto menu;
    }
    return 0;
}
void SetColor(int col)
{
     WORD wColor;
     //This handle is needed to get the current background attribute

     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     //csbi is used for wAttributes word

     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //To mask out all but the background attribute, and to add the color
          wColor = (csbi.wAttributes & 0xF0) + (col & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
}
