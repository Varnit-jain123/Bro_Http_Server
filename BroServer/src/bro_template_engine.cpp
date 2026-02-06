#include<bro_template_engine.h>
#include<bro_file_system_utility.h>
void TemplateEngine::createVMDFileName(const char *chtmlFileName,char *vmdFileName)
{
    char *dotPtr;
    const char *ptr;
    ptr=chtmlFileName;
    while(*ptr!='\0') ptr++;
    while(ptr>=chtmlFileName)
    {
        if(*ptr=='/')
        {
            chtmlFileName=ptr+1;
            break;
        }
        ptr--;
    }
    for(;*chtmlFileName!='\0';chtmlFileName++,vmdFileName++)
    {
        if(*chtmlFileName=='.') dotPtr=vmdFileName;
        *vmdFileName=*chtmlFileName;
    }
    *vmdFileName='\0';
    strcpy(dotPtr+1,"vmd");
}
void TemplateEngine::createVMDFile(const char *chtmlFileName,const char *pathToVMDFile)
{
    char *start,*end,*ptr;
    unsigned int x;
    FILE *f;
    int toRead;
    char buffer[513];
    long fileLength;
    f=fopen(chtmlFileName,"rb");
    if(f==NULL)
    {
        // code to send back 404 error
        return;
    }
    FILE *vmdFile;
    vmdFile=fopen(pathToVMDFile,"wb");
    if(vmdFile==NULL)
    {

        // code to send back 500 internal server error page
        fclose(f);
        return ;
    }
    fseek(f,0,SEEK_END);
    fileLength=ftell(f);
    // cout<<fileLength<<endl;
    rewind(f);
    unsigned int sp,ep;
    int ii;
    x=0;
    struct vmd vmd_record;
    while(1)
    {
        if((fileLength-x)>256) toRead=256;
        else toRead=fileLength-x;
        // cout<<"Reading : "<<toRead<<" number of bytes"<<endl;
        fread(buffer,toRead,1,f);
        buffer[toRead]='\0';
        x+=toRead;
        // printf("%s",buffer);
        // processing the contents of the buffer starts here
        start=buffer;
        while(1)
        {
            start=strstr(start,"${");
            if(start==NULL)
            {
                if(x==fileLength) break;
                if(buffer[toRead-1]=='$')
                {
                    buffer[0]='$';
                    if((fileLength-x)>255) toRead=255;
                    else toRead=fileLength-x;
                    fread(buffer+1,toRead,1,f);
                    x+=toRead;
                    buffer[1+toRead]='\0';
                    start=buffer;
                    continue;
                }
                else
                {
                    break;
                }
            }
            end=strstr(start+2,"}");
            if(end!=NULL)
            {
                // printf("-------------------------\n");
                for(ptr=start+1;ptr<end;ptr++) if(*ptr=='$' && *(ptr+1)=='{') break;
                if(*ptr=='$') start=ptr;
                // write vmd record to the file starts here
                for(ptr=start+2,ii=0;ptr<end;ptr++,ii++)
                {
                    vmd_record.var_name[ii]=*ptr;
                }
                vmd_record.var_name[ii]='\0';
                sp=(x-toRead)+(start-buffer);
                ep=(x-toRead)+(end-buffer);
                cout<<"1--->"<<sp<<","<<ep<<endl;
                vmd_record.start_position=sp;
                vmd_record.end_position=ep;
                fwrite(&vmd_record,sizeof(struct vmd),1,vmdFile);
                // write vmd record to the file ends here
                // for(ptr=start;ptr<=end;ptr++) printf("%c",*ptr);
                // printf("\n");
                // printf("-------------------------\n");
                start=end+1;
                continue;
            }
            // if } not found , then load the next chunk of 256 bytes
            // and store it in buffer from index 256
            if(x==fileLength) break;
            if((fileLength-x)>256) toRead=256;
            else toRead=fileLength-x;
            fread(buffer+256,toRead,1,f);
            buffer[256+toRead]='\0';
            x=x+toRead;
            end=strstr(buffer+256,"}");
            if(end==NULL) break;
            for(ptr=start+1;ptr<end;ptr++) if(*ptr=='$' && *(ptr+1)=='{') break;
            if(*ptr=='$') start=ptr;
            // write vmd record to the file starts here
            for(ptr=start+2,ii=0;ptr<end;ptr++,ii++)
            {
                vmd_record.var_name[ii]=*ptr;
            }
            vmd_record.var_name[ii]='\0';
            sp=(x-toRead)+(start-buffer)-256;
            ep=(x-toRead)+(end-buffer)-256;
            cout<<"2--->"<<sp<<","<<ep<<endl;
            vmd_record.start_position=sp;
            vmd_record.end_position=ep;
            fwrite(&vmd_record,sizeof(struct vmd),1,vmdFile);
            // write vmd record to the file ends here
            // for(ptr=start;ptr<=end;ptr++) printf("%c",*ptr);
            // printf("\n");
            strcpy(buffer,buffer+256);
            end=end-256;
            start=end+1;
        }
        // processing the contents of the buffer ends here
        if(x==fileLength) break;
    }
    fclose(f);
    fclose(vmdFile);
}
void TemplateEngine::processCHTMLFile(const char *chtmlFileName,Request &request,int clientSocketDescriptor)
{
    if(!FileSystemUtility::directoryExists("vmd_files"))
    {
        if(!FileSystemUtility::createDirectory("vmd_files"))
        {
            // later on
        }
    }
    char vmdFileName[257];
    createVMDFileName(chtmlFileName,vmdFileName);
    string folderName=string("vmd_files");
    string pathToVMDFile=folderName+string(PATH_SEPARATOR)+string(vmdFileName);
    bool generateVMDFile=false;
    if(FileSystemUtility::fileExists(pathToVMDFile.c_str()))
    {
        if(FileSystemUtility::getLastUpdatedTime(chtmlFileName)>FileSystemUtility::getLastUpdatedTime(pathToVMDFile.c_str()))
        {
            generateVMDFile=true;
        }
    }
    else
    {
        generateVMDFile=true;
    }
    if(generateVMDFile)
    {
        createVMDFile(chtmlFileName,pathToVMDFile.c_str());
    }
    // process the chtml file (pick up info from VMD file)
    FILE *chtmlFile=fopen(chtmlFileName,"rb");
    FILE *vmdFile=fopen(pathToVMDFile.c_str(),"rb");
    fseek(chtmlFile,0,SEEK_END);
    long fileLength=ftell(chtmlFile);
    rewind(chtmlFile);  
    long responseSize=fileLength;
    string data;
    struct vmd vmdRecord;
    while(1)
    {
        fread(&vmdRecord,sizeof(struct vmd),1,vmdFile);
        if(feof(vmdFile)) break;
        responseSize=responseSize-((vmdRecord.end_position-vmdRecord.start_position)+1);
        data=request.getCHTMLVariable(vmdRecord.var_name);
        responseSize=responseSize+data.length();
    }
    cout<<"Response size is : "<<responseSize<<endl;
    string mimeType;
    mimeType=string("text/html");
    char header[200];
    sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\nConnection: close\r\n\r\n",mimeType.c_str(),responseSize);
    send(clientSocketDescriptor,header,strlen(header),0);
    long bytesLeftToRead;
    int bytesToRead;
    char buffer[4096];
    bytesLeftToRead=fileLength;
    rewind(vmdFile);
    long tmpBytesLeftToRead;
    long bytesProcessedFromFile=0;
    while(1)
    {
        fread(&vmdRecord,sizeof(struct vmd),1,vmdFile);
        if(feof(vmdFile)) break;
        tmpBytesLeftToRead=vmdRecord.start_position-bytesProcessedFromFile;
        bytesToRead=4096;
        while(tmpBytesLeftToRead>0)
        {
            if(tmpBytesLeftToRead<bytesToRead) bytesToRead=tmpBytesLeftToRead;
            fread(buffer,bytesToRead,1,chtmlFile);
            if(feof(chtmlFile)) break; // this won't happen on our case
            bytesProcessedFromFile+=bytesToRead;
            send(clientSocketDescriptor,buffer,bytesToRead,0);
            tmpBytesLeftToRead=tmpBytesLeftToRead-bytesToRead;
        }
        fread(buffer,(vmdRecord.end_position-vmdRecord.start_position)+1,1,chtmlFile);
        bytesProcessedFromFile+=(vmdRecord.end_position-vmdRecord.start_position)+1;
        string data;
        if(request.containsCHTMLVariable(vmdRecord.var_name))
        {
            data=request.getCHTMLVariable(vmdRecord.var_name);
            send(clientSocketDescriptor,data.c_str(),data.length(),0);
        }
    }
    bytesLeftToRead-=bytesProcessedFromFile;
    bytesToRead=4096;
    while(bytesLeftToRead>0)
    {
        if(bytesLeftToRead<bytesToRead) bytesToRead=bytesLeftToRead;
        fread(buffer,bytesToRead,1,chtmlFile);
        if(feof(chtmlFile)) break; // this won't happen on our case 
        send(clientSocketDescriptor,buffer,bytesToRead,0);
        bytesLeftToRead=bytesLeftToRead-bytesToRead;
    }
    fclose(chtmlFile);
    fclose(vmdFile);
}
