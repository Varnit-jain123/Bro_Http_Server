#include<bro_application_level_Container_Dependent_Function.h>
ApplicationLevelContainerDependentFunction::ApplicationLevelContainerDependentFunction(void(*mappedFunction)(Request &,Response &,ApplicationLevelContainer &),ApplicationLevelContainer *p2ApplicationLevelContainer)
{
this->mappedFunction=mappedFunction;
this->p2ApplicationLevelContainer=p2ApplicationLevelContainer;
}
void ApplicationLevelContainerDependentFunction::doService(Request &request,Response &response)
{
this->mappedFunction(request,response,*p2ApplicationLevelContainer);
}

