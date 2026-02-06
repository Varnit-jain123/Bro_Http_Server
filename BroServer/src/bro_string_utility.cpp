#include<bro_string_utility.h>
StringUtility::StringUtility()
{
    
}
void StringUtility::toLowerCase(char *str)
{
    if(str==NULL) return;
    int i=0;
    while(str[i]!='\0')
    {
        if(str[i]>='A' && str[i]<='Z')    str[i]=str[i]+32;
        i++;
    }
}

