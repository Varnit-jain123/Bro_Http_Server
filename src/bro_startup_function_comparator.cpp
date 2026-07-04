#include<bro_startup_function_comparator.h>
int StartupFunctionComparator::operator()(StartupFunction *e,StartupFunction *f)
{
    return !(e->getPriorityNumber()<f->getPriorityNumber());
}

