//724403 Romashko Maria
//LR 5 Chasy realnogo vremeni
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int delay_milliseconds;
int bcdtoi(int i);
unsigned char rtcRead(unsigned char reg);
void rtcWrite(unsigned char reg, unsigned char value);
void printTime();
void waitFreeClock(void);
void printMenu(void);
void printBin(int i);
void EnableClockUpdate(void);
void DisableClockUpdate(void);

void DisableClockUpdate(void)
{
    rtcWrite(0x0B, rtcRead(0x0B)|0x80);
}

void EnableClockUpdate(void)
{
    rtcWrite(0x0B, rtcRead(0x0B) & 0x7F);
}

void waitFreeClock(void)
{
    unsigned char c = 1;
    int i = 50;
    while (c && (i > 0))
    {
        c = rtcRead(0x0A) & 0x80;// TODO: use rtcRead() ??
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

void printTime()
{
    unsigned int seconds;
    unsigned int minutes;
    unsigned int hours;
    unsigned int weekday;
    unsigned int day;
    unsigned int month;
    unsigned int year;
    char *weekday_names[] = { "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday" };

    DisableClockUpdate();
    seconds = rtcRead(0);
    minutes = rtcRead(2);
    hours = rtcRead(4);
    weekday = rtcRead(6);
    day = rtcRead(7);
    month = rtcRead(8);
    year = rtcRead(9);
    EnableClockUpdate();

    printf("Current time: %x:%02x:%02x %s %02x.%02x.20%02x    \r",
        hours, minutes, seconds, weekday_names[weekday - 1], day, month, year);
    fflush(stdout);
}

void clock()
{ 
    while(!kbhit()) 
    {
        printTime();
        delay(100);
    }
}

void rtcSet()
{
    unsigned int newhour = 0x13;
    unsigned int newmin = 0x13;
    unsigned int newsec = 0x13;
    unsigned int newday = 0x13;
    unsigned int newmonth) = 0x13;
    unsigned int newyear = 0x13;
    printf(" New hour: "); 
    scanf("%x", newhour); 
    printf("\n");
    printf(" New minute: ");
    scanf("%x", newmin);
    printf("\n");
    printf(" New second: ");
    scanf("%x", newsec);
    printf("\n");
    printf(" New day: ");
    scanf("%x", newday);
    printf("\n");
    printf(" New month: ");
    scanf("%x", newmonth);
    printf("\n");
    printf(" New year: ");
    scanf("%x", newyear);
    printf("\n");
    DisableClockUpdate();
    rtcWrite(0x04, newhour);
    rtcWrite(0x02, newmin);
    rtcWrite(0x00, newsec);
    rtcWrite(0x07, newday);
    rtcWrite(0x08, newmonth);
    rtcWrite(0x09, newyear);
    EnableClockUpdate(); 
    printf("New time: %x:%02x:%02x  %02x.%02x.20%02x    \n",
        newhour, newmin, newsec, newday, newmonth, newyear);
}

void printMenu(void)
{
    system("cls");
    printf("\nRomashko M.D. LR_5 724403 \n LR2 Chasy realnogo vremeni\n");
    printf("\n\n1 - Show time \n2 - Set time \n3 - Set delay \n4 Exit\n\n");
}

int main()
{
    int t;
    int s;
    while (1)
    {
        printMenu();
        s = getch();
        switch (s)
        {
        case '1':
            clock();
            break;
        case '2':
            printf("Set time");
            rtcSet();
            printTime();
            printf("\n");
            system("pause");
            printf("\n");
            break;
        case '3':
            printf("Set delay");
            system("pause");
            printf("\n");
            break;
        case '4':
            printf("\n");
            system("pause");
            return 0;
        default:
            printf("\nPlease consider picking any menu option.\n");
            break;
        }
    }
    return 0;
}
