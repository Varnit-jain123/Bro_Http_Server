#include<bro_header_utility.h>
HeaderUtility::HeaderUtility(){}
void HeaderUtility::parseHeader(const char *header,map<string,string> &headerFieldsMap)
{
    const char *sp,*ep,*colonPtr,*ptr;
    ptr=header;
    while(*ptr)
    {
        colonPtr=NULL;
        sp=ptr;
        while(*ptr && *ptr!='\r' && *ptr!='\n') 
        {
            if(colonPtr==NULL && *ptr==':') colonPtr=ptr;
            ptr++;
        }
        ep=ptr-1;
        if(*ptr=='\r') ptr+=2;
        else ptr+=1;
        if(sp==ptr || colonPtr==NULL) //never happen
        {
            ptr++;
            continue;
        }
        string fieldName(sp,colonPtr-sp);
        if(*(colonPtr+1)==' ') sp=colonPtr+2;
        else sp=colonPtr+1;
        string fieldValue(sp,ep-sp+1);
        transform(fieldName.begin(),fieldName.end(),fieldName.begin(),::tolower);
        // cout<<"["<<fieldName<<"]["<<fieldValue<<"]"<<endl;
        // headerFieldsMap.insert(pair<string,string>(fieldName,fieldValue));
        headerFieldsMap.insert({fieldName,fieldValue});
        if(*ptr=='\r' && *(ptr+1)=='\n') break; //header ends
    }
}

