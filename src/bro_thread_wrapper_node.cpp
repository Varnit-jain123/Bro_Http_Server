#include<bro_thread_wrapper_node.h>
BroThreadWrapperNode::BroThreadWrapperNode()
{
    this->th=NULL;
    this->next=NULL;
    this->completedStatus=false;
}
void BroThreadWrapperNode::setCompletedStatus(bool completedStatus)
{
    lock_guard<mutex> lock(this->completedStatusMutex);
    this->completedStatus=completedStatus;
}
bool BroThreadWrapperNode::isComplete()
{
    lock_guard<mutex> lock(this->completedStatusMutex);
    return this->completedStatus;
}