#ifndef DATE_H
#define DATE_H

struct Date
{
    int d, m, y;
};

const int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int getDifference(Date dt1, Date dt2)
{

    long int n1 = dt1.y*365 + dt1.d;
 
    for (int i=0; i<dt1.m - 1; i++)
        n1 += monthDays[i];

 
    long int n2 = dt2.y*365 + dt2.d;
    for (int i=0; i<dt2.m - 1; i++)
        n2 += monthDays[i];
 
    return (n2 - n1);
}

#endif