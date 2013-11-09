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
    int i, h, d, m, y;
} TDATETIME;

typedef struct TDayTime {
    int i, h;
    long long int d;
} TDAYTIME;

int isLeapYear(int year) {
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0) && year % 4000 != 0) {
        return 1;
    }
    return 0;
}

int daysInMonth(int month, int year) {
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
            date->h < 0 ||
            date->i < 0 ||
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

int leapYearsInInterval(const TDATETIME * date) {
    long long int count1 = 0, count2 = 0, count3 = 0, limit, start, blockCount;
    int count = 0;
    int i;

    if (date->y - 1600 < 4000) {
        for (i = 1600; i < date->y; i++) {
            count += isLeapYear(i);
        }
        return count;
    }

    limit = ((1600 / 4000) + 1)*4000;
    start = ((date->y / 4000) - 1)*4000;

    for (i = 1600; i < limit; i++) {
        count1 += isLeapYear(i);
    }

    blockCount = start / 4000 - limit / 4000;
    count2 = blockCount * 969;

    for (i = start; i < date->y; i++) {
        count3 += isLeapYear(i);
    }

    return count1 + count2 + count3;
}

int comparator(const TDAYTIME * date1, const TDAYTIME * date2) {
    if (date1->d != date2->d) return (date1->d - date2->d);
    if (date1->h != date2->h) return (date1->h - date2->h);
    return (date1->i - date2->i);
}

TDAYTIME convertForm(const TDATETIME * date) {
    TDAYTIME time;
    time.d = (date->y - 1600) * 365 + dayNumber(date) + leapYearsInInterval(date);
    time.h = date->h;
    time.i = date->i;
    return time;
}

void incTime(TDAYTIME * time) {
    if (time->i < 59) {
        time->i++;
    } else if (time->h < 23) {
        time->i = 0;
        time->h++;
    } else {
        time->i = 0;
        time->h = 0;
        time->d++;
    }
}

int isCuckooTime(const TDAYTIME * time) {
    if (time->i == 30) return 1;
    if (time->i == 0) {
        if (time->h == 0) return 12;
        if (time->h > 12) return (time->h - 12);
        return time->h;
    }
    return 0;
}

int cuckooClock(int y1, int m1, int d1, int h1, int i1,
        int y2, int m2, int d2, int h2, int i2,
        long long int * cuckoo) {
    TDATETIME date1, date2;

    date1.y = y1;
    date1.m = m1;
    date1.d = d1;
    date1.h = h1;
    date1.i = i1;

    date2.y = y2;
    date2.m = m2;
    date2.d = d2;
    date2.h = h2;
    date2.i = i2;

    if (!dateIsValid(&date1) || !dateIsValid(&date2)) {
        printf("Invalid date!!\n");
        return 0;
    }

    TDAYTIME time1, time2;
    time1 = convertForm(&date1);
    time2 = convertForm(&date2);

    if (comparator(&time1, &time2) > 0) {
        printf("Date1 later than date2!!\n");
        return 0;
    }
    
    * cuckoo = 0;
    if (0) {
        * cuckoo += ((time2.d - time1.d - 1) * 90);
        time1.d = time2.d - 1;
    }

    while (comparator(&time1, &time2) <= 0) {
        * cuckoo += isCuckooTime(&time1);
        incTime(&time1);
    }

    printf("cuckoo = %lld\n", * cuckoo);

    return 1;
}

#ifndef __PROGTEST__

int main(int argc, char** argv) {
    long long int cuckoo;
    int res;

    res = cuckooClock(2013, 10, 1, 13, 15,
            2013, 10, 1, 18, 45, &cuckoo);
    /* res = 1, cuckoo = 26 */

    res = cuckooClock(2013, 10, 1, 13, 15,
            2013, 10, 2, 11, 20, &cuckoo);
    /* res = 1, cuckoo = 165 */

    res = cuckooClock(2013, 1, 1, 13, 15,
            2013, 10, 5, 11, 20, &cuckoo);
    /* res = 1, cuckoo = 49845 */

    res = cuckooClock(2012, 1, 1, 13, 15,
            2012, 10, 5, 11, 20, &cuckoo);
    /* res = 1, cuckoo = 50025 */

    res = cuckooClock(1900, 1, 1, 13, 15,
            1900, 10, 5, 11, 20, &cuckoo);
    /* res = 1, cuckoo = 49845 */

    res = cuckooClock(2013, 10, 1, 0, 0,
            2013, 10, 1, 12, 0, &cuckoo);
    /* res = 1, cuckoo = 102 */

    res = cuckooClock(2013, 10, 1, 0, 15,
            2013, 10, 1, 0, 25, &cuckoo);
    /* res = 1, cuckoo = 0 */

    res = cuckooClock(2013, 10, 1, 12, 0,
            2013, 10, 1, 12, 0, &cuckoo);
    /* res = 1, cuckoo = 12 */

    res = cuckooClock(2013, 11, 1, 12, 0,
            2013, 10, 1, 12, 0, &cuckoo);
    /* res = 0, cuckoo = N/A */

    res = cuckooClock(2013, 10, 32, 12, 0,
            2013, 11, 10, 12, 0, &cuckoo);
    /* res = 0, cuckoo = N/A */

    res = cuckooClock(2100, 2, 29, 12, 0,
            2100, 2, 29, 12, 0, &cuckoo);
    /* res = 0, cuckoo = N/A */

    res = cuckooClock(2400, 2, 29, 12, 0,
            2400, 2, 29, 12, 0, &cuckoo);
    /* res = 1, cuckoo = 12 */
    return (EXIT_SUCCESS);
}
#endif /* __PROGTEST__ */

