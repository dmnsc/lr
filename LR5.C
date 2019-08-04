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

unsigned char rtcRead(unsigned char reg)
{
    outp(0x70, reg);
    return inp(0x71);
}

void rtcWrite(unsigned char reg, unsigned char value)
{
    outp(0x70, reg);
    outp(0x71, value);
}

// rtcWrite(0x0B, rtcRead(0x0B) | 0x80);
// outp(0x61, inp(0x61) | 0x01);

void printTime()
{
    char *weekday_names[] = { "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday" };

    // TODO: Clock update must be disabled here
    rtcWrite(0x0B, rtcRead(0x0B) | 0x80);
    //???
    int seconds = rtcRead(0);
    int minutes = rtcRead(2);
    int hours = rtcRead(4);
    int weekday = rtcRead(6);
    int day = rtcRead(7);
    int month = rtcRead(8);
    int year = rtcRead(9);
    // TODO: Clock update must be enabled back here
    rtcWrite(0x0B, rtcRead(0x0B) & 0x80);
    //???

    printf("Current time: %x:%02x:%02x %s %02x.%02x.20%02x    \r",
        hours, minutes, seconds, weekday_names[weekday - 1], day, month, year);
    fflush(stdout);
}

void clock()
{
while(!kbhit()) // TODO: loop exit condition!!!
{
printTime();
delay(100); // TODO: delay at least 100ms here
}
}

void rtcSet()
{
    ////
    int newhour;
    printf(" New hours: \n");
    scanf("%x", newhour);
    rtcWrite(0x0B, rtcRead(0x0B) | 0x80); // use DisableClockUpdate
    outp(0x71, 0x04);
    outp(0x70, newhour); // NB! newhour variable encoding!!!

    rtcWrite(0x0B, rtcRead(0x0B) & 0x80);
    ////
    printTime();
}

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
            clock();
            //printTime();
            break;
        case '2':
            printf("Set time");
			rtcSet();
            printf("\n");
            break;
        case '3':
            printf("Set delay");
          //  delayMs(5000);
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
