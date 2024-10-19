#include "message_log.h"

static char * get_message(void);
static char * get_date(void);

static char * get_message(void)
{
    // pass
}

void write_message(char *msg)
{
    //
}

void get_report_log(void)
{
    //
}

FILE * open_report_log(void)
{
    FILE * file;
    file = fopen("data/report-log.txt", "a");
    
    if(file == NULL)
    {
        printf("Unable to create or open file.\n");
    }
    
    char *date = get_date();
    if (date != NULL) {
        fputs(date, file);  
    }

    return file;
}

static char * get_date(void)
{
    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    char buffer[11]; 
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", tm_info);

    char date[50]; 
    snprintf(date, sizeof(date), "Date: %s", buffer);

    return date;
}