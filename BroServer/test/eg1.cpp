#include<bro_container.h>
#include<bro_gmt_date_time.h>
#include<iostream>
using namespace std;
int main()
{
    Container c;
    int x;
    x=10;
    c.set(string("abcd"),x,NULL,NULL);
    int y;
    c.get(string("abcd"),&y,NULL,NULL);
    cout<<y<<endl;
    GMTDateTime now;
    cout<<now<<endl;
    return 0;
}