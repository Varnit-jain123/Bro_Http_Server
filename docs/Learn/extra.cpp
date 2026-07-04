#include<iostream>
#include<time.h>
using namespace std;
class GMTDateTime 
{
    private:
        struct tm gmtDateTime;
        bool isValid;
    public:
        GMTDateTime()
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
        GMTDateTime(int day,int month,int year,int hour,int minute,int second)
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
        int getDay()
        {
            if(!this->isValid) return -1;
            return this->gmtDateTime.tm_mday;
        }
        int getMonth()
        {
            if(!this->isValid) return -1;
            return this->gmtDateTime.tm_mon+1;
        }
        int getYear()
        {
            if(!this->isValid) return -1;
            return this->gmtDateTime.tm_year+1900;
        }
        int getHour()
        {
            if(!this->isValid) return -1;
            return this->gmtDateTime.tm_hour;
        }
        int getMinute()
        {
            if(!this->isValid) return -1;
            return this->gmtDateTime.tm_min;
        }
        int getSecond()
        {   
            if(!this->isValid) return -1;
            return this->gmtDateTime.tm_sec;
        }
        int getWeekDay()
        {
            if(!this->isValid) return -1;
            return this->gmtDateTime.tm_wday;
        }
        int getYearDay()
        {
            if(!this->isValid) return -1;
            return this->gmtDateTime.tm_yday;
        }
        bool isValidDateTime()
        {
            return this->isValid;
        }
        string stringfy()
        {
            if(!this->isValid) return string("");
            char buffer[30];
            strftime(buffer,30,"%a, %d %b %Y %H:%M:%S GMT",&(this->gmtDateTime));
            return string(buffer);
        }
};
ostream & operator <<(ostream &out,GMTDateTime &gmtDateTime)
{
    out<<gmtDateTime.stringfy();
    return out;
}

int main()
{
    GMTDateTime now;
    cout<<now<<endl;
    GMTDateTime dt(13,7,2004,23,59,59);
    cout<<dt<<endl;
    cout<<dt.getWeekDay()<<endl;
    cout<<dt.getYearDay()<<endl;
    return 0;
}