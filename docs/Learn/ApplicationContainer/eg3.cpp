// #include<string.h>
// #include<iostream>
// #include<map>
// #include<stdlib.h>
// using namespace std;
// class Container
// {
//     typedef struct _bag
//     {
//         void *ptr;
//         int size;
//     }Bag;
//     map<string,Bag> dataSet;
//     public:
//     template<class Whatever>
//     void set(string keyName,Whatever something)
//     {
//         cout<<sizeof(something)<<endl;
//         void *ptr;
//         ptr=malloc(sizeof(something));
//         memcpy(ptr,&something,sizeof(something));
//         Bag bag;
//         bag.ptr=ptr;
//         bag.size=sizeof(something); 
//         dataSet.insert(pair<string,Bag>(keyName,bag));
//     }
//     template<class Cool>
//     void get(string keyName,Cool anything,bool *success)
//     {   
//         cout<<sizeof(*anything)<<endl;
//         auto iterator=dataSet.find(keyName);
//         if(iterator==dataSet.end()) 
//         {
//             if(success) *success=false;
//             return;
//         }
//         Bag bag;
//         bag=iterator->second;
//         if(bag.size!=sizeof(*anything))
//         {
//             if(success) *success=false;
//             return;
//         }
//         memcpy(anything,bag.ptr,sizeof(*anything)); 
//         if(success) *success=true;
//     }
// };

// // error prone code
// class Bulb
// {
//     private:
//     int w;
//     public:
//     Bulb()
//     {
//         this->w=0;
//     }
//     Bulb(const Bulb &other)
//     {
//         this->w=other.w;
//     }
//     void setWattage(int w)
//     {
//         this->w=w;
//     }
//     int getWattage()
//     {
//         return this->w;
//     }
// };

// void extractAndPrint(Container &ctr)
// {
//     Bulb k;
//     bool success;
//     ctr.get("philips",&k,&success);
//     if(success)
//     {
//         cout<<k.getWattage()<<endl;
//     }
//     else
//     {
//         cout<<"philips does not exists"<<endl;
//     }
// }

// void keepData(Container &c)
// {
//     Bulb g;
//     g.setWattage(60);
//     c.set("philips",g);
// }

// int main()
// {
//     Container c;
//     keepData(c);
//     extractAndPrint(c);
//     return 0;
// }


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

// error prone code
// class Bulb
// {
//     private:
//     int w;
//     public:
//     Bulb()
//     {
//         this->w=0;
//     }
//     Bulb(const Bulb &other)
//     {
//         this->w=other.w;
//     }
//     void setWattage(int w)
//     {
//         this->w=w;
//     }
//     int getWattage()
//     {
//         return this->w;
//     }
// };

// void extractAndPrint(Container &ctr)
// {
//     Bulb *k;
//     bool success;
//     ctr.get("philips",&k,&success);
//     if(success)
//     {
//         cout<<k->getWattage()<<endl;
//     }
//     else
//     {
//         cout<<"philips does not exists"<<endl;
//     }
// }

// void keepData(Container &c)
// {
//     Bulb *g;
//     g=new Bulb();
//     g->setWattage(60);
//     c.set("philips",g);
//     g->setWattage(100); 
//     // this application would fall apart if the following line in introduced
//     // delete g;

// }

// int main()
// {
//     Container c;
//     keepData(c);
//     extractAndPrint(c);
//     return 0;
// }


// NOT a error prone code

class IntCollection
{
    int *ptr;
    unsigned int size;
    public:
    IntCollection()
    {
        this->ptr=NULL;
        this->size=0;
    }
    IntCollection(unsigned int size)
    {
        if(size<0)  
        {
            this->size=0;
            this->ptr=NULL;
        }
        else
        {
            this->size=size;
            this->ptr=new int[this->size];
            if(this->ptr==NULL) this->size=0;
        }
    }
    IntCollection(const IntCollection &other)
    {
        if(other.size==0)
        {
            this->size=0;
            this->ptr=NULL;
            return;
        }
        this->size=other.size;
        this->ptr=new int[this->size];
        if(this->ptr==NULL)
        {
            this->size=0;
            return;
        }
        int x;
        for(x=0;x<other.size;x++)
        {
           this->ptr[x]=other.ptr[x];
        }
    }
    void setValueAt(int index,int value)
    {
        if(index>=0 && index<this->size)
        {
            ptr[index]=value;
        }
    }
    int getValueAt(int index)
    {
        if(index>=0 && index<this->size) return ptr[index];
        else return 0;
    }
    int getSize()
    {
        return this->size;
    }
};


void extractAndPrint(Container &ctr)
{
    IntCollection k;
    bool succ;
    ctr.get("mycollection",&k,&succ);
    for(int i=0;i<k.getSize();i++)
    {
        cout<<k.getValueAt(i)<<endl;
    }
}

void keepData(Container &c)
{
    IntCollection ic(5);    
    ic.setValueAt(0,1000);
    ic.setValueAt(1,2000);
    ic.setValueAt(2,404);
    ic.setValueAt(3,940);
    ic.setValueAt(4,600);
    c.set("mycollection",ic);
}

int main()
{
    Container c;
    keepData(c);
    extractAndPrint(c);
    return 0;
}
