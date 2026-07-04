#include<bro_simple_function.h>
SimpleFunction::SimpleFunction(void (*mappedFunction)(Request &,Response &))
{
    this->mappedFunction=mappedFunction;
}
void SimpleFunction::doService(Request &request,Response &response)
{
    this->mappedFunction(request,response);
}

