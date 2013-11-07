/* 
 * File:   main.c
 * Author: palmyman
 *
 * Created on November 7, 2013, 2:03 PM
 */

#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#endif /* __PROGTEST__ */

typedef struct TDateTime {
    int i, h, d, m;
    long long int y;
} TDATETIME;

int isLeapYear(long long int year) {
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0) && year % 4000 != 0) {
        return 1;
    }
    return 0;
}

int daysInMonth(int month, long long int year) {
    int days;
    switch (month) {
        case 0: days = 0;
            break;
        case 1: days = 31;
            break;
        case 2:
        {
            if (isLeapYear(year)) {
                days = 29;
            } else {
                days = 28;
            }
            break;
        }
        case 3: days = 31;
            break;
        case 4: days = 30;
            break;
        case 5: days = 31;
            break;
        case 6: days = 30;
            break;
        case 7: days = 31;
            break;
        case 8: days = 31;
            break;
        case 9: days = 30;
            break;
        case 10: days = 31;
            break;
        case 11: days = 30;
            break;
        case 12: days = 31;
            break;
    }
    return days;
}

int dateIsValid(const TDATETIME * date) {
    if (date->y < 1600 ||
            date->m < 1 ||
            date->d < 1 ||
            date->h < 1 ||
            date->i < 1 ||
            date->m > 12 ||
            date->d > daysInMonth(date->m, date->y) ||
            date->h > 23 ||
            date->i > 59)
        return 0;
    return 1;
}

int dayNumber(const TDATETIME * date) {
    int number = 0;
    int i;
    for (i = 0; i < date->m; i++) {
        number += daysInMonth(i, date->y);
    }
    number += date->d;
    return number;
}

int leapYearsInInterval(const TDATETIME * date1, const TDATETIME * date2) {
    long long int count1 = 0, count2 = 0, count3 = 0, limit, start, blockCount;
    int count = 0;
    int i;

    if (date2->y - date1->y < 4000) {
        for (i = date1->y; i < date2->y; i++) {
            count += isLeapYear(i);
        }
        return count;
    }

    limit = ((date1->y / 4000) + 1)*4000;
    start = ((date2->y / 4000) - 1)*4000;

    for (i = date1->y; i < limit; i++) {
        count1 +=isLeapYear(i);
    }

    blockCount = start / 4000 - limit / 4000;
    count2 = blockCount * 969;

    for (i = start; i < date2->y; i++) {
        count3 += isLeapYear(i);
    }

    return count1 + count2 + count3;
}

int comparator(const TDATETIME * date1, const TDATETIME * date2) {
    if (date1->y != date2->y) return (date1->y - date2->y);
    if (date1->m != date2->m) return (date1->m - date2->m);
    if (date1->d != date2->d) return (date1->d - date2->d);
    if (date1->h != date2->h) return (date1->h - date2->h);
    return (date1->i - date2->i);
}

long long int daysInInterval(const TDATETIME * date1, const TDATETIME * date2) {
    return (date2->y - date1->y) * 365 + dayNumber(date2) - dayNumber(date1) + leapYearsInInterval(date1, date2);
}

void incDate(TDATETIME * date) {
    if(date->i < 59) {
        date->i++;
    } else if (date->h < 23) {
        date->i = 0;
        date->h++;
    } else {
        date->i = 0;
        date->h = 0;
    }
}

int cuckooClock(int y1, int m1, int d1, int h1, int i1,
        int y2, int m2, int d2, int h2, int i2,
        long long int * cuckoo) {
    return 1;
}

#ifndef __PROGTEST__

int main(int argc, char** argv) {

    return (EXIT_SUCCESS);
}
#endif /* __PROGTEST__ */

