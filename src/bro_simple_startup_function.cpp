#include<bro_simple_startup_function.h>
SimpleStartupFunction::SimpleStartupFunction(int priorityNumber,void (*startupFunction)(void))
{
    this->priorityNumber=priorityNumber;
    this->startupFunction=startupFunction;
}
int SimpleStartupFunction::getPriorityNumber()
{
    return this->priorityNumber;
}
void SimpleStartupFunction::run()
{
    startupFunction();
}

