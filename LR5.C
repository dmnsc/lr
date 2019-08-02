//724403 Romashko Maria
//LR 5 Chasy realnogo vremeni
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printBin(int i);
int bcdtoi(int i);
void printTime();
void waitFreeClock(void);

int delay_milliseconds;


void DisableClockUpdate(void)
{
    unsigned char c;
    waitFreeClock();
    outp(0x70, 0x0B);
    c = inp(0x71);
    c |= 0x80;
    outp(0x70, 0x0B);
    outp(0x71, c);
}

void EnableClockUpdate(void)
{
    unsigned char c;
    waitFreeClock();
    outp(0x70, 0x0B);
    c = inp(0x71);
    c &= 0x7F;
    outp(0x70, 0x0B);
    outp(0x71, c);
}

void waitFreeClock(void)
{
    unsigned char c = 1;
    int i = 50;
    while (c && (i > 0))
    {
        outp(0x70, 0x0A);
        c = inp(0x71) & 0x80;
        i--;
    }
}

void printBin(int i)
{
    char str[10];
    itoa(i + 256, str, 2);
    printf("%s ", str + 1);
}

int bcdtoi(int i)
{
    return i % 16 + i / 16 * 10;
}

int rtcRead(int reg)
{
    outp(0x70, reg);
    return inp(0x71);
}

void printTime()
{
    char *weekday_names[] = { "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday" };

    int seconds = rtcRead(0);
    int minutes = rtcRead(2);
    int hours = rtcRead(4);
    int weekday = rtcRead(6);
    int day = rtcRead(7);
    int month = rtcRead(8);
    int year = rtcRead(9);

    printf("Current time: %x:%02x:%02x %s %02x.%02x.20%02x    \r",
        hours, minutes, seconds, weekday_names[weekday - 1], day, month, year);
    fflush(stdout);
}

void clock()
{
while (1)
{
printTime();
}
}

void delayMs(int delay)
{
    delay_milliseconds = 0;
    while (delay_milliseconds != delay)
    {
       // printf("%d time left \n", delay_milliseconds);
        //delay_milliseconds++;
        //fflush(stdout);
    }
    waitFreeClock();
}

void interrupt far int70_custom(void)
{
milliseconds++;
delay_milliseconds++;
if (milliseconds >= 500)
{
milliseconds %= 500;
UpdateTime();
}
outp(0x70, 0x0C);
inp(0x71);
outp(0x20, 0x20);
outp(0xA0, 0x20);
}

/*void interrupt int9_wait(void)
{
	inp(0x60);
	outp(0x20, 0x20);
}*/

void interrupt int9_any_key(void)
{
	unsigned char key = inp(0x60);
	if (key < 128)
		anykey = 1;
	outp(0x20, 0x20);
}

void rtcSet(int newmin, int newsec, int newhour) 
{
	outp(0x70, inp(0x0B)|0x80);
	scanf("%x : %x : %x \n", newhour, newmin, newsec);
	waitFreeClock();
	outp(0x71, 0x04);
	outp(0x70, newhour);
	waitFreeClock();
	outp(0x70, 0x02);
	outp(0x71, newmin);
	waitFreeClock();
	outp(0x71, 0x00);
	outp(0x70, newsece);
	waitFreeClock();
	outp(0x70, inp(0x0B) | 0x80);
	printTime();
	//outp(0x70, reg);
	//return inp(0x71);
}

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
            //clock();
            printTime();
            break;
        case '2':
            printf("Set time");
            printf("\n");
            break;
        case '3':
            printf("Set delay");
            delayMs(5000);
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
