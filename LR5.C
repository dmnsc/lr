//724403 Romashko Maria
//LR 5 Chasy realnogo vremeni
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printbin(int i);
int bcdtoi(int i);
void printtime();

int tmiliseconds;

void printbin(int i)
{
    char str[10];
    itoa(i + 256, str, 2);
    printf("%s ", str + 1);
}

int bcdtoi(int i)
{
    return i % 16 + i / 16 * 10;
}

int rtc_read(int reg)
{
    outp(0x70, reg);
    return inp(0x71);
}

void printtime()
{
    char *weekday_names[] = {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};

    int seconds = rtc_read(0);
    int minutes = rtc_read(2);
    int hours = rtc_read(4);
    int weekday = rtc_read(6);
    int day = rtc_read(7);
    int month = rtc_read(8);
    int year = rtc_read(9);

    printf("Current time: %x:%02x:%02x %s %02x.%02x.20%02x    \r",
        hours, minutes, seconds, weekday_names[weekday], day, month, year);
    fflush(stdout);
}

void clock() 
{
	while (1)
	{
		printtime();
	}
}

// int readtime(int t)
// {
//     /*
//     outp(0x70, 0x0X);
//     t = bcdtoi(inp(0x71));
//     */
// };

//

int main()
{
    int t;
    int s;
    system("cls");
    printf("\nRomashko M.D. LR_5 724403 \n LR2 Chasy realnogo vremeni\n");
    printf("\n\n1 - Show time \n2 - Set time \n3 - Set delay \n4 Exit\n\n");
	
    while (1)
    {
        s = getch();
        switch (s)
        {
        case '1':
            printtime();
            break;
        case '2':
            printf("Set time");
            printf("\n");
            break;
        case '3':
            printf("Set delay");
			clock();
            printf("\n");
            break;
        case '4':
            printf("\n");
            system("pause");
            return 0;
        default:  printf("\nOshibka vvoda!\n");
            break;
        }
    }
    return 0;
}
