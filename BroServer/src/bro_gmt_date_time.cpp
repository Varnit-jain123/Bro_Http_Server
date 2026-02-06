#include<bro_gmt_date_time.h>
    GMTDateTime::GMTDateTime()
    {
        time_t elapsedSeconds;
        elapsedSeconds=time(NULL);
        if(elapsedSeconds==-1)
        {
            this->isValid=false;
            this->gmtDateTime.tm_sec=0;
            this->gmtDateTime.tm_min=0;
            this->gmtDateTime.tm_hour=0;
            this->gmtDateTime.tm_mday=0;
            this->gmtDateTime.tm_mon=0;
            this->gmtDateTime.tm_year=0;
            return;
        }
        struct tm *gmt=gmtime(&elapsedSeconds);
        this->gmtDateTime=*gmt;
        this->isValid=true;
    }
    //  month (0-11)
    // year (year-1900)
    GMTDateTime::GMTDateTime(int day,int month,int year,int hour,int minute,int second)
    {
        struct tm tmptm;
        tmptm.tm_mday=day;
        tmptm.tm_mon=month-1;
        tmptm.tm_year=year-1900;
        tmptm.tm_hour=hour;
        tmptm.tm_min=minute;
        tmptm.tm_sec=second;
        time_t t=mktime(&tmptm);
        if(mktime(&tmptm)==-1) // mktime will set wday part in the structure
        {
            this->isValid=false;
            this->gmtDateTime.tm_sec=0;
            this->gmtDateTime.tm_min=0;
            this->gmtDateTime.tm_hour=0;
            this->gmtDateTime.tm_mday=0;
            this->gmtDateTime.tm_mon=0;
            this->gmtDateTime.tm_year=0;
            return;
        }
        this->isValid=true;
        this->gmtDateTime=tmptm;
    }
    int GMTDateTime::getDay()
    {
        if(!this->isValid) return -1;
        return this->gmtDateTime.tm_mday;
    }
    int GMTDateTime::getMonth()
    {
        if(!this->isValid) return -1;
        return this->gmtDateTime.tm_mon+1;
    }
    int GMTDateTime::getYear()
    {
        if(!this->isValid) return -1;
        return this->gmtDateTime.tm_year+1900;
    }
    int GMTDateTime::getHour()
    {
        if(!this->isValid) return -1;
        return this->gmtDateTime.tm_hour;
    }
    int GMTDateTime::getMinute()
    {
        if(!this->isValid) return -1;
        return this->gmtDateTime.tm_min;
    }
    int GMTDateTime::getSecond()
    {   
        if(!this->isValid) return -1;
        return this->gmtDateTime.tm_sec;
    }
    int GMTDateTime::getWeekDay()
    {
        if(!this->isValid) return -1;
        return this->gmtDateTime.tm_wday;
    }
    int GMTDateTime::getYearDay()
    {
        if(!this->isValid) return -1;
        return this->gmtDateTime.tm_yday;
    }
    bool GMTDateTime::isValidDateTime()
    {
        return this->isValid;
    }
    string GMTDateTime::stringfy()
    {
        if(!this->isValid) return string("");
        char buffer[30];
        strftime(buffer,30,"%a, %d %b %Y %H:%M:%S GMT",&(this->gmtDateTime));
        return string(buffer);
    }
    void GMTDateTime::addSeconds(int seconds)
    {
        if(!this->isValid) return;
        this->gmtDateTime.tm_sec+=seconds;
        mktime(&(this->gmtDateTime)); // to update the structure after addition of seconds
    }
    void GMTDateTime::addMinutes(int minutes)
    {
        if(!this->isValid) return;
        this->gmtDateTime.tm_min+=minutes;
        mktime(&(this->gmtDateTime)); // to update the structure after addition of minutes
    }
    void GMTDateTime::addHours(int hours)
    {
        if(!this->isValid) return;
        this->gmtDateTime.tm_hour+=hours;
        mktime(&(this->gmtDateTime)); // to update the structure after addition of hours
    }
ostream & operator <<(ostream &out,GMTDateTime &gmtDateTime)
{
    out<<gmtDateTime.stringfy();
    return out;
}
