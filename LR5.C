//724403 Romashko Maria
//LR 5 Chasy realnogo vremeni
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void rtcWrite(unsigned char reg, unsigned char value);
void printTime(void);
void waitFreeClock(void);
void printMenu(void);
void printBin(int i);
void EnableClockUpdate(void);
void DisableClockUpdate(void);
void interrupt(*int70_normal)(void);
unsigned int bcdtoi(int i);
unsigned char rtcRead(unsigned char reg);
void printRegisters(void);
void printRegistersRealtime(void);
void clock(void);
void rtcSet(void);
unsigned char countWeekday(unsigned char year, unsigned char month, unsigned char day );
unsigned int delay_milliseconds;
char *weekday_names[] = { "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday" };

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
        c = rtcRead(0x0A) & 0x80;
        i--;
    }
}

void printBin(int i)
{
    char str[10];
    itoa(i + 256, str, 2);
    printf("%s ", str + 1);
}

unsigned int bcdtoi(int i)
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

void printRegistersRealtime()
{
    while (!kbhit())
    {
        printRegisters();
        delay(100);
        system("cls");
    }
}

void rtcSet()
{
    unsigned int newhour;
    unsigned int newmin;
    unsigned int newsec;
    unsigned int newday;
    unsigned int newmonth;
    unsigned int newyear;
    unsigned int newweekday;
    printf("\n");
    printf("New hour: "); 
    scanf("%x", &newhour);
    printf("\n");
    printf("New minute: ");
    scanf("%x", &newmin);
    printf("\n");
    printf("New second: ");
    scanf("%x", &newsec);
    printf("\n");
    printf("New day: ");
    scanf("%x", &newday);
    printf("\n");
    printf("New month: ");
    scanf("%x", &newmonth);
    printf("\n");
    printf("New year: ");
    scanf("%x", &newyear);
    printf("\n");
    DisableClockUpdate();
    rtcWrite(0x04, newhour);
    rtcWrite(0x02, newmin);
    rtcWrite(0x00, newsec);
    rtcWrite(0x07, newday);
    rtcWrite(0x08, newmonth);
    rtcWrite(0x09, newyear);
    EnableClockUpdate(); 
    newweekday = countWeekday(newyear, newmonth, newday);
    printf("New time: %x:%02x:%02x %s %02x.%02x.20%02x    \n",
        newhour, newmin, newsec, weekday_names[newweekday - 1], newday, newmonth, newyear);
}

unsigned char countWeekday(unsigned char year, unsigned char month, unsigned char day)
{
    unsigned char newWeekday;
    unsigned char monthCode;
    unsigned char yearCode;
    if (month == 1 || month == 10)               monthCode = 1;
    if (month == 5)                              monthCode = 2;
    if (month == 8)                              monthCode = 3;
    if (month == 2 || month == 3 || month == 11) monthCode = 4;
    if (month == 6)                              monthCode = 5;
    if (month == 12 || month == 9)               monthCode = 6;
    if (month == 4 || month == 7)                monthCode = 0;
    yearCode = (6 + year + year / 4) % 7;
    if (year % 4 != 0)
    {
        newWeekday = (day + monthCode + yearCode) % 7;
    }
    if (year % 4 = 0)
    {
        newWeekday = (day + monthCode + yearCode) % 7 - 1;
    }
    return newWeekday;
}

void interrupt far int70_custom(void)
{
    outp(0x70, 0x0C);
    inp(0x71);
    outp(0x20, 0x20);
    outp(0xA0, 0x20);
}

void printMenu(void)
{
    system("cls");
    printf("\nRomashko M.D. LR_5 724403 \n LR2 Chasy realnogo vremeni\n");
    printf("\n\n1 - Show time \n2 - Set time \n3 - Set delay \n4 - Show registers (real time) \n5 Exit\n\n");
}

void printRegisters(void)
{
    printf("SECONDS.(00):");
    printBin(rtcRead(0));//sec
    printf("    MINUTES.(02):");
    printBin(rtcRead(2));//min
    printf("    HOURS.(04):");
    printBin(rtcRead(4));//hour
    printf("\n");

    printf("WEEKDAY.(06):");
    printBin(rtcRead(6));//weekday
    printf("        DAY.(07):");
    printBin(rtcRead(7));//day
    printf("    MONTH.(08):");
    printBin(rtcRead(8));//month
    printf("\n");
    printf("YEAR.(09):");
    printBin(rtcRead(9));//year
    printf("\n");

    printf("\n");
    printf("0x0A:");
    printBin(rtcRead(0x0A));//A
    printf("    0x0B:");
    printBin(rtcRead(0x0B));//B
    printf("\n0x0C:");
    printBin(rtcRead(0x0C));//C
    printf("    0x0E:");
    printBin(rtcRead(0x0D));//E
    printf("\n");
}

int main()
{
    int t;
    int s;
    int70_normal = getvect(0x70);
    setvect(0x70, int70_custom);
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
            system("cls");
            printRegistersRealtime();
            break;
        case '5':
            printf("\n");
            system("pause");
            return 0;
        default:
            printf("\nPlease consider picking any menu option.\n");
            break;
        }
    }
    setvect(0x70, int70_normal);
    return 0;
}

/*-rtcSet function complete
- new function countWeekday added(to find day of the week without reading registers, used in rtcSet function.Has an issue of running into an error if the year is--)
- new printRegister function added
- new printRegistersRealtime function added*/
