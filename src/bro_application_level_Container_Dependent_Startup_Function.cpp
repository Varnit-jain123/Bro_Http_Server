#include<bro_application_level_Container_Dependent_Startup_Function.h>
ApplicationLevelContainerDependentStartupFunction::ApplicationLevelContainerDependentStartupFunction(int priorityNumber,void (*startupFunction)(ApplicationLevelContainer &),ApplicationLevelContainer *p2ApplicationLevelContainer)
{
    this->priorityNumber=priorityNumber;
    this->startupFunction=startupFunction;
    this->p2ApplicationLevelContainer=p2ApplicationLevelContainer;
}
int ApplicationLevelContainerDependentStartupFunction::getPriorityNumber()
{
    return this->priorityNumber;
}
void ApplicationLevelContainerDependentStartupFunction::run()
{
    startupFunction(*p2ApplicationLevelContainer);
}

