#ifndef __BRO_GMT_DATE_TIME__
#define __BRO_GMT_DATE_TIME__
    #include<bro_global.h>
    class GMTDateTime 
    {
        private:
            struct tm gmtDateTime;
            bool isValid;
        public:
            GMTDateTime();
            GMTDateTime(int day,int month,int year,int hour,int minute,int second);
            int getDay();
            int getMonth();
            int getYear();
            int getHour();
            int getMinute();
            int getSecond();
            int getWeekDay();
            int getYearDay();
            bool isValidDateTime();
            string stringfy();
            void addSeconds(int seconds);
            void addMinutes(int minutes);
            void addHours(int hours);
    };
    ostream & operator <<(ostream &out,GMTDateTime &gmtDateTime);
#endif