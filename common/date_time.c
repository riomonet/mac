#include "date_time.h"


struct date_time date_today() {
    time_t unix_time = time(NULL);
    struct tm *parts = localtime(&unix_time);
    struct date_time dt;
    snprintf(dt.date,32,"%.2d/%.2d/%d",
             parts->tm_mon + 1,
             parts->tm_mday,
             parts->tm_year + 1900);
    snprintf(dt.time,32, "%.2d:%.2d",
             parts->tm_hour,
             parts->tm_min);
    return dt;
}
