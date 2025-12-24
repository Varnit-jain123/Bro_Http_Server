#include<string.h>
#include<iostream>
#include<map>
#include<stdlib.h>
using namespace std;
class Container
{
    typedef struct _bag
    {
        void *ptr;
        int size;
    }Bag;
    map<string,Bag> dataSet;
    public:
    template<class Whatever>
    void set(string keyName,Whatever something)
    {
        cout<<sizeof(something)<<endl;
        void *ptr;
        ptr=malloc(sizeof(something));
        memcpy(ptr,&something,sizeof(something));
        Bag bag;
        bag.ptr=ptr;
        bag.size=sizeof(something); 
        dataSet.insert(pair<string,Bag>(keyName,bag));
    }
    template<class Cool>
    void get(string keyName,Cool anything,bool *success)
    {   
        cout<<sizeof(*anything)<<endl;
        auto iterator=dataSet.find(keyName);
        if(iterator==dataSet.end()) 
        {
            if(success) *success=false;
            return;
        }
        Bag bag;
        bag=iterator->second;
        if(bag.size!=sizeof(*anything))
        {
            if(success) *success=false;
            return;
        }
        memcpy(anything,bag.ptr,sizeof(*anything)); 
        if(success) *success=true;
    }
};
void extractAndPrint(Container &ctr)
{
    int a;
    unsigned b;
    char c;
    unsigned char d;
    float e;
    double f;
    long double g;
    short h;
    unsigned short i;
    long int j;
    unsigned long int k;
    long long int l;
    unsigned long long int m;
    wchar_t n;
    bool asuccess;
    bool bsuccess;
    bool csuccess;
    bool dsuccess;
    bool esuccess;
    bool fsuccess;
    bool gsuccess;
    bool hsuccess;
    bool isuccess;
    bool jsuccess;
    bool ksuccess;
    bool lsuccess;
    bool msuccess;
    bool nsuccess;
    ctr.get("int",&a,&asuccess);
    ctr.get("uint",&b,&bsuccess);
    ctr.get("char",&c,&csuccess);
    ctr.get("uchar",&d,&dsuccess);
    ctr.get("float",&e,&esuccess);
    ctr.get("double",&f,&fsuccess);
    ctr.get("long double",&g,&gsuccess);
    ctr.get("short",&h,&hsuccess);
    ctr.get("ushort",&i,&isuccess);
    ctr.get("lint",&j,&jsuccess);
    ctr.get("ulint",&k,&ksuccess);
    ctr.get("llint",&l,&lsuccess);
    ctr.get("ullint",&m,&msuccess);
    ctr.get("wchar",&n,&nsuccess);
    cout<<"*************************"<<endl;
    if(asuccess) cout<<a<<endl;
    if(bsuccess) cout<<b<<endl;
    if(csuccess) cout<<c<<endl;
    if(dsuccess) cout<<d<<endl;
    if(esuccess) cout<<e<<endl;
    if(fsuccess) cout<<f<<endl;
    if(gsuccess) cout<<g<<endl;
    if(hsuccess) cout<<h<<endl;
    if(isuccess) cout<<i<<endl;
    if(jsuccess) cout<<j<<endl;
    if(ksuccess) cout<<k<<endl;
    if(lsuccess) cout<<l<<endl;
    if(msuccess) cout<<m<<endl;
    if(nsuccess) cout<<n<<endl;
    
}

void keepData(Container &c)
{
    c.set("int",10);
    c.set("uint",(unsigned int)2147483655);
    c.set("char",'A');
    c.set("uchar",(unsigned char)200);
    c.set("float",2.335f);
    c.set("double",55.443);
    c.set("long double",(long double)554.23456);
    c.set("short",(short int)43);
    c.set("ushort",(unsigned short int)543);
    c.set("lint",(long int)-777);
    c.set("ulint",(unsigned long int)54543);
    c.set("llint",(long long int)-4854);
    c.set("ullint",(unsigned long long int)30303);
    c.set("wchar",(wchar_t)'A');
}

int main()
{
    Container c;
    // keepData(c);
    // cout<<"-----------------------------"<<endl;
    // extractAndPrint(c);

    // int x;
    // x=5000;
    // c.set("salary",x);
    // int r;
    // bool success;
    // c.get("cartoon",&r,&success);
    // if(success)
    // {
    //     cout<<"The value of cartoon is : "<<r<<endl;
    // }
    // else
    // {
    //     cout<<"cartoon does not exists"<<endl;
    // }

    // int x;
    // x=5000;
    // c.set("salary",x);
    // int r;
    // bool success;
    // c.get("salary",&r,&success);
    // if(success)
    // {
    //     cout<<"The value of salary is : "<<r<<endl;
    // }
    // else
    // {
    //     cout<<"salary does not exists"<<endl;
    // }

    char m;
    m='M';
    c.set("gender",m);
    bool success;
    int whatever;
    c.get("gender",&whatever,&success);
    if(success)
    {
        cout<<"value against gender is : "<<whatever<<endl;
    }
    else
    {
        cout<<"gender not found"<<endl;
    }

    return 0;
}
