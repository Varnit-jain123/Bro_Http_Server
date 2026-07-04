#ifndef __BRO_CONTAINER__
#define __BRO_CONTAINER__
    #include<bro_global.h>
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
        void set(string keyName,Whatever something,bool *success,__container_operation_failure_reason__ *reason);
        template<class Cool>
        void get(string keyName,Cool anything,bool *success,__container_operation_failure_reason__ *reason);
        template<class Cool>
        void remove(string keyName,Cool anything,bool *success,__container_operation_failure_reason__ *reason);
        bool contains(string keyName);
    };

    template<class Whatever>
    void Container::set(string keyName,Whatever something,bool *success,__container_operation_failure_reason__ *reason)
    {
        auto iterator=dataSet.find(keyName);
        if(iterator!=dataSet.end()) 
        {
            if(reason) *reason={__KEY_EXISTS__};
            if(success) *success=false;
            return;
        }
        // cout<<sizeof(something)<<endl;
        void *ptr;
        ptr=malloc(sizeof(something));
        if(ptr==NULL)
        {
            if(reason) *reason={__OUT_OF_MEMORY__};
            if(success)*success=false;
            return;
        }
        memcpy(ptr,&something,sizeof(something));
        Bag bag;
        bag.ptr=ptr;
        bag.size=sizeof(something); 
        dataSet.insert(pair<string,Bag>(keyName,bag));
        if(success) *success=true;
    }
    template<class Cool>
    void Container::get(string keyName,Cool anything,bool *success,__container_operation_failure_reason__ *reason)
    {   
        // cout<<sizeof(*anything)<<endl;
        auto iterator=dataSet.find(keyName);
        if(iterator==dataSet.end()) 
        {
            if(reason) *reason={__KEY_DOES_NOT_EXIST__};
            if(success) *success=false;
            return;
        }
        Bag bag;
        bag=iterator->second;
        if(bag.size!=sizeof(*anything))
        {
            if(reason) *reason={__VALUE_SIZE_MISMATCH__};
            if(success) *success=false;
            return;
        }
        memcpy(anything,bag.ptr,sizeof(*anything)); 
        if(success) *success=true;
    }

    template<class Cool>
    void Container::remove(string keyName,Cool anything,bool *success,__container_operation_failure_reason__ *reason)
    {   
        // cout<<sizeof(*anything)<<endl;
        auto iterator=dataSet.find(keyName);
        if(iterator==dataSet.end()) 
        {
            if(reason) *reason={__KEY_DOES_NOT_EXIST__};
            if(success) *success=false;
            return;
        }
        Bag bag;
        bag=iterator->second;
        if(bag.size!=sizeof(*anything))
        {
            if(reason) *reason={__VALUE_SIZE_MISMATCH__};
            if(success) *success=false;
            return;
        }
        memcpy(anything,bag.ptr,sizeof(*anything)); 
        free(bag.ptr); // to release the memory by bro server programmer
        if(success) *success=true;
    }
    

#endif
