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
void printMenu(void);

unsigned int delay_milliseconds;

void DisableClockUpdate(void)
{
    unsigned char c;
    waitFreeClock(); // TODO: unnecessary here
    outp(0x70, 0x0B);// TODO: use rtcRead()
    c = inp(0x71);
    c |= 0x80;
    outp(0x70, 0x0B);// TODO: use rtcWrite()
    outp(0x71, c);
}

void EnableClockUpdate(void)
{
    unsigned char c;
    waitFreeClock(); // TODO: unnecessary here
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
        // TODO: use rtcRead()
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
    unsigned int seconds;
    unsigned int minutes;
    unsigned int hours;
    unsigned int weekday;
    unsigned int day;
    unsigned int month;
    unsigned int year;

    // TODO: Clock update must be disabled here
    DisableClockUpdate();
    //rtcWrite(0x0B, rtcRead(0x0B) | 0x80);
    //???
    seconds = rtcRead(0);
    minutes = rtcRead(2);
    hours = rtcRead(4);
    weekday = rtcRead(6);
    day = rtcRead(7);
    month = rtcRead(8);
    year = rtcRead(9);
    // TODO: Clock update must be enabled back here
    EnableClockUpdate();
    //rtcWrite(0x0B, rtcRead(0x0B) & 0x80);
    //???

    printf("Current time: %x:%02x:%02x %s %02x.%02x.20%02x    \r",
        hours, minutes, seconds, weekday_names[weekday - 1], day, month, year);
    fflush(stdout);
}

void clock()
{ // TODO: formatting!!!
while(!kbhit()) // TODO: loop exit condition!!!
{
printTime();
delay(100); // TODO: delay at least 100ms here
}
}

void rtcSet() // TODO: use fixed value for debug
{
    ////
    int newhour;
    printf(" New hours: \n");
    scanf("%x", newhour); //"%x" for hex value??
    rtcWrite(0x0B, rtcRead(0x0B) | 0x80);
    //DisableClockUpdate();  // use DisableClockUpdate

    outp(0x71, 0x04); //hours
    outp(0x70, newhour); // NB! newhour variable encoding!!! -- new hour variable
    // TODO: use rtcWrite()

    //EnableClockUpdate();
    rtcWrite(0x0B, rtcRead(0x0B) & 0x80); //Enabled??????
    ////
    printTime();
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
        default:
            printf("\nPlease consider picking any menu option.\n");
            break;
        }
    }
    return 0;
}
