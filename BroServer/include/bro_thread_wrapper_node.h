#ifndef __BRO_THREAD_WRAPPER_NODE__
#define __BRO_THREAD_WRAPPER_NODE__
    #include<iostream>
    #include<mutex>
    #include<thread>
    using namespace std;
    class BroThreadWrapperNode
    {
        bool completedStatus;
        mutex completedStatusMutex;
        public:
            thread *th;
            BroThreadWrapperNode *next;
            BroThreadWrapperNode();
            void setCompletedStatus(bool completedStatus);
            bool isComplete();
    };
#endif