#include "message_log.h"

static void get_date(char *date, size_t size);
static void get_time(char *buffer, size_t size);

uint8_t write_message(char *msg)
{
    FILE * file;
    file = fopen("data/report-log.txt", "a");
    
    if(file == NULL)
    {
        printf("Unable to create or open file.\n");
        return -1;
    }

    char time_buffer[9]; 
    get_time(time_buffer, sizeof(time_buffer));
    fputs(time_buffer, file);
    fputs("\n", file);
    fputs(msg, file);
    fputs("\n", file);
    fclose(file);
    return 0;
}

void get_report_log(void)
{
    //
}

uint8_t add_date_to_log(void)
{
    FILE * file;
    file = fopen("data/report-log.txt", "a");
    
    if(file == NULL)
    {
        printf("Unable to create or open file.\n");
        return -1;
    }
    
    char date_buffer[50]; 
    get_date(date_buffer, sizeof(date_buffer));
    fputs(date_buffer, file);  
    
    fclose(file);
    return 0;
}

static void get_date(char *date, size_t size)
{
    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    char buffer[11]; 
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", tm_info);
 
    snprintf(date, size, "Date: %s", buffer);

}

static void get_time(char *buffer, size_t size)
{
    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    strftime(buffer, size, "%H:%M:%S", tm_info);
}
