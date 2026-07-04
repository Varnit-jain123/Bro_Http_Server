#include<bro_utilities.h>
BroUtilities::BroUtilities()
{

}
bool BroUtilities::isHexChar(int w)
{
    if(w>=48 && w<=57) return true;
    if(w>='a' && w<='f') return true;
    if(w>='A' && w<='F') return true;
    return false;
}
void BroUtilities::decode(char *encodedString,char *decodedString)
{
    char *ptr=encodedString;
    char *d=decodedString;
    int i,m;
    i=0;
    while(*ptr!='\0')
    {
        if(*ptr=='+')
        {
            d[i]=' ';
            i++;
            ptr++;
            continue;
        }
        if(*ptr!='%')
        {
            d[i]=*ptr;
            i++;
            ptr++;
            continue;
        }
        ptr++;
        if(isHexChar(*ptr) && isHexChar(*(ptr+1)))
        {
            sscanf(ptr,"%2x",&m);
            d[i]=m;
            i++;
            ptr+=2;
        }
        else
        {
            i=0;
            break;
        }
    }
    d[i]='\0';
}
void BroUtilities::loadMIMETypes(map<string,string> &mimeTypesMap)
{
    FILE *file;
    file=fopen("bro-data/mime.types","r");
    if(file==NULL) return;
    char *mimeType;
    char *extension;
    char line[200];
    int x;
    while(1)
    {
        fgets(line,200,file);
        if(feof(file)) break;
        if(line[0]=='#' || line[0]=='\r' || line[0]=='\n' || line[0]=='\0') continue;
        x=strlen(line)-1;
        while(1) // logic to remove '\r' and '\n' from end of string
        {
            if(line[x]=='\r' || line[x]=='\n') 
            {
                line[x]='\0';
                x--;
            }
            else 
            {
                break;
            }
        }
        mimeType=&line[0];
        for(x=0;line[x]!='\t';x++);
        line[x]='\0';
        x++;
        while(line[x]=='\t') x++;
        while(1)
        {
            extension=&line[x];
            while(line[x]!='\0' && line[x]!=' ') x++;
            if(line[x]=='\0')
            {
                // add entry to map and break the loop
                mimeTypesMap.insert(pair<string,string>(string(extension),string(mimeType)));
                //cout<<extension<<"   ,   "<<mimeType<<endl;
                break;
            }
            else
            {
                // place \0 on xth index, add entry to the map and increment the value of x
                line[x]='\0';
                mimeTypesMap.insert(pair<string,string>(string(extension),string(mimeType)));
                //cout<<extension<<"   ,   "<<mimeType<<endl;
                x++;
            }
        }
    }
    fclose(file);
}
