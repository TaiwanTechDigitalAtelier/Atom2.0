#include "Marlin.h"
#include "cmdbuffer.h"
#include "cardreader.h"
#include "ultralcd.h"
#include "stepper.h"
#include "temperature.h"
#include "language.h"

#ifdef SDSUPPORT

CardReader::CardReader()
{
    filesize = 0;
    sdpos = 0;
    start_print = false;
    sdprinting = false;
    cardOK = false;
    saving = false;
    logging = false;
    autostart_atmillis=0;
    workDirDepth = 0;
    memset(workDirParents, 0, sizeof(workDirParents));
    
    autostart_stilltocheck=true; //the sd start is delayed, because otherwise the serial cannot answer fast enought to make contact with the hostsoftware.
    lastnr=0;
    //power to SD reader
#if SDPOWER > -1
    SET_OUTPUT(SDPOWER);
    WRITE(SDPOWER,HIGH);
#endif //SDPOWER
    
    autostart_atmillis=millis()+5000;
}

char *createFilename(char *buffer,const dir_t &p) //buffer>12characters
{
    char *pos=buffer;
    for (uint8_t i = 0; i < 11; i++)
    {
        if (p.name[i] == ' ')
        {
            continue;
        }
        if (i == 8)
        {
            *pos++ = '.';
        }
        *pos++ = p.name[i];
    }
    *pos++ = 0;
    return buffer;
}

void CardReader::lsDive(const char *prepend,SdFile parent)
{
    dir_t p;
    uint8_t cnt=0;
    datetime_u currDatetime = { 0 };
    datetime_u newestDatetime = { 0 };

    while (parent.readDir(p, longFilename) > 0)
    {
        if (DIR_IS_SUBDIR(&p) && lsAction!=LS_Count && lsAction!=LS_GetFilename) // hence LS_SerialPrint
        {
            char path[13*2];
            char lfilename[13];
            createFilename(lfilename,p);
            
            path[0] = 0;
            if (strlen(prepend)==0) //avoid leading / if already in prepend
            {
                strcat(path,"/");
            }
            strcat(path,prepend);
            strcat(path,lfilename);
            strcat(path,"/");
            
            //Serial.print(path);
            
            SdFile dir;
            if (!dir.open(parent,lfilename, O_READ))
            {
                if(lsAction==LS_SerialPrint)
                {
                    SERIAL_ECHO_START;
                    SERIAL_ECHOLN(MSG_SD_CANT_OPEN_SUBDIR);
                    SERIAL_ECHOLN(lfilename);
                }
            }
            lsDive(path,dir);
            //close done automatically by destructor of SdFile
        }
        else
        {
            if (p.name[0] == DIR_NAME_FREE)
            {
                break;
            }
            if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.'|| p.name[0] == '_')
            {
                continue;
            }
            if (longFilename[0] != '\0' && (longFilename[0] == '.' || longFilename[0] == '_'))
            {
                continue;
            }
            if ( p.name[0] == '.')
            {
                if ( p.name[1] != '.')
                    continue;
            }
            
            if (!DIR_IS_FILE_OR_SUBDIR(&p))
            {
                continue;
            }
            
            filenameIsDir = DIR_IS_SUBDIR(&p);
            if(!filenameIsDir)
            {
                if(p.name[8]!='G') continue;
                if(p.name[9]=='~') continue;
            }
            //if(cnt++!=nr) continue;
            createFilename(filename,p);
            if(lsAction==LS_SerialPrint)
            {
                SERIAL_PROTOCOL(prepend);
                SERIAL_PROTOCOLLN(filename);
            }
            else if(lsAction==LS_Count)
            {
				if (!filenameIsDir)
				{
					currDatetime.part.date = p.creationDate;
					currDatetime.part.time = p.creationTime;
					if (currDatetime.value > newestDatetime.value)
					{
						newestFileNumber = nrFiles;
						newestDatetime.value = currDatetime.value;
					}
				}
                nrFiles++;
            }
            else if(lsAction==LS_GetFilename)
            {
                if(cnt==nrFiles)
                {
                    return;
                }
                cnt++;
            }
        }
    }
}

void CardReader::ls()
{
    lsAction=LS_SerialPrint;
    if (lsAction==LS_Count)
    {
        nrFiles=0;
    }
    root.rewind();
    lsDive("",root);
}

void CardReader::initsd()
{
    cardOK = false;
    if(root.isOpen())
    {
        root.close();
    }
#ifdef SDSLOW
    if (!card.init(SPI_HALF_SPEED,SDSS))
#else
    if (!card.init(SPI_FULL_SPEED,SDSS))
#endif
    {
        //if (!card.init(SPI_HALF_SPEED,SDSS))
        SERIAL_ECHO_START;
        SERIAL_ECHOLN_CPGM(MSG_SD_INIT_FAIL);
    }
    else if (!volume.init(&card))
    {
        SERIAL_ERROR_START;
        SERIAL_ERRORLN_CPGM(MSG_SD_VOL_INIT_FAIL);
    }
    else if (!root.openRoot(&volume))
    {
        SERIAL_ERROR_START;
        SERIAL_ERRORLN_CPGM(MSG_SD_OPENROOT_FAIL);
    }
    else
    {
        cardOK = true;
        SERIAL_ECHO_START;
        SERIAL_ECHOLN_CPGM(MSG_SD_CARD_OK);
    }
    workDir=root;
    curDir=&root;
    /*
    if(!workDir.openRoot(&volume))
    {
        SERIAL_ECHOLN_CPGM(MSG_SD_WORKDIR_FAIL);
    }
     */
}

void CardReader::setroot()
{
    /*if(!workDir.openRoot(&volume))
    {
        SERIAL_ECHOLN_CPGM(MSG_SD_WORKDIR_FAIL);
    }*/
    workDir=root;
    
    curDir=&workDir;
}
void CardReader::release()
{
    start_print = false;
    sdprinting = false;
    cardOK = false;
}

void CardReader::startFileprint()
{
    if(cardOK && isFileOpen())
    {
        start_print = true;
        sdprinting = true;
    }
}

void CardReader::pauseSDPrint()
{
    if(sdprinting)
    {
        sdprinting = false;
    }
}


void CardReader::openLogFile(char* name)
{
    logging = true;
    openFile(name, false);
}

void CardReader::openFile(char* name,bool read)
{
    if(!cardOK)
    {
        return;
    }
    file.close();
    sdprinting = false;
    
    SdFile myDir;
    curDir=&root;
    char *fname=name;
    
    char *dirname_start,*dirname_end;
    if (name[0]=='/')
    {
        dirname_start=strchr(name,'/')+1;
        while (dirname_start)
        {
            dirname_end=strchr(dirname_start,'/');
            //SERIAL_ECHO("start:");SERIAL_ECHOLN((int)(dirname_start-name));
            //SERIAL_ECHO("end  :");SERIAL_ECHOLN((int)(dirname_end-name));
            if(dirname_end && dirname_end>dirname_start)
            {
                char subdirname[13];
                strncpy(subdirname, dirname_start, dirname_end-dirname_start);
                subdirname[dirname_end-dirname_start]=0;
                SERIAL_ECHOLN(subdirname);
                if (!myDir.open(curDir,subdirname,O_READ))
                {
                    SERIAL_PROTOCOL_CPGM(MSG_SD_OPEN_FILE_FAIL);
                    SERIAL_PROTOCOL(subdirname);
                    SERIAL_PROTOCOLLN_CPGM(".");
                    return;
                }
                else
                {
                    //SERIAL_ECHOLN("dive ok");
                }
                curDir=&myDir;
                dirname_start=dirname_end+1;
            }
            else // the reminder after all /fsa/fdsa/ is the filename
            {
                fname=dirname_start;
                //SERIAL_ECHOLN("remaider");
                //SERIAL_ECHOLN(fname);
                break;
            }
        }
    }
    else //relative path
    {
        curDir=&workDir;
    }
    if(read)
    {
        if (file.open(curDir, fname, O_READ))
        {
            filesize = file.fileSize();
            SERIAL_PROTOCOL_CPGM(MSG_SD_FILE_OPENED);
            SERIAL_PROTOCOL(fname);
            SERIAL_PROTOCOL_CPGM(MSG_SD_SIZE);
            SERIAL_PROTOCOLLN(filesize);
            sdpos = 0;
            
            SERIAL_PROTOCOLLN_CPGM(MSG_SD_FILE_SELECTED);
			if (longFilename[0])
			{ 
				lcd_setfilename(longFilename);
			}
			else
			{
				lcd_setfilename(fname);
			}
        }
        else
        {
            SERIAL_PROTOCOL_CPGM(MSG_SD_OPEN_FILE_FAIL);
            SERIAL_PROTOCOL(fname);
            SERIAL_PROTOCOLLN_CPGM(".");
        }
    }
    else
    { //write
        if (!file.open(curDir, fname, O_CREAT | O_APPEND | O_WRITE | O_TRUNC))
        {
            SERIAL_PROTOCOL_CPGM(MSG_SD_OPEN_FILE_FAIL);
            SERIAL_PROTOCOL(fname);
            SERIAL_PROTOCOLLN_CPGM(".");
        }
        else
        {
            saving = true;
            SERIAL_PROTOCOL_CPGM(MSG_SD_WRITE_TO_FILE);
            SERIAL_PROTOCOLLN(name);
			lcd_setfilename(fname);
        }
    }
}

void CardReader::removeFile(char* name)
{
    if(!cardOK)
        return;
    file.close();
    sdprinting = false;
    
    SdFile myDir;
    curDir=&root;
    char *fname=name;
    
    char *dirname_start,*dirname_end;
    if(name[0]=='/')
    {
        dirname_start=strchr(name,'/')+1;
        while(dirname_start)
        {
            dirname_end=strchr(dirname_start,'/');
            //SERIAL_ECHO("start:");SERIAL_ECHOLN((int)(dirname_start-name));
            //SERIAL_ECHO("end  :");SERIAL_ECHOLN((int)(dirname_end-name));
            if(dirname_end && dirname_end>dirname_start)
            {
                char subdirname[13];
                strncpy(subdirname, dirname_start, dirname_end-dirname_start);
                subdirname[dirname_end-dirname_start]=0;
                SERIAL_ECHOLN(subdirname);
                if(!myDir.open(curDir,subdirname,O_READ))
                {
                    SERIAL_PROTOCOL_CPGM("open failed, File: ");
                    SERIAL_PROTOCOL(subdirname);
                    SERIAL_PROTOCOLLN_CPGM(".");
                    return;
                }
                else
                {
                    //SERIAL_ECHOLN("dive ok");
                }
                
                curDir=&myDir;
                dirname_start=dirname_end+1;
            }
            else // the reminder after all /fsa/fdsa/ is the filename
            {
                fname=dirname_start;
                //SERIAL_ECHOLN("remaider");
                //SERIAL_ECHOLN(fname);
                break;
            }
            
        }
    }
    else //relative path
    {
        curDir=&workDir;
    }
    if (file.remove(curDir, fname))
    {
        SERIAL_PROTOCOL_CPGM("File deleted:");
        SERIAL_PROTOCOL(fname);
        sdpos = 0;
    }
    else
    {
        SERIAL_PROTOCOL_CPGM("Deletion failed, File: ");
        SERIAL_PROTOCOL(fname);
        SERIAL_PROTOCOLLN_CPGM(".");
    }
    
}

void CardReader::getStatus()
{
    if(cardOK)
    {
        SERIAL_PROTOCOL_CPGM(MSG_SD_PRINTING_BYTE);
        SERIAL_PROTOCOL(sdpos);
        SERIAL_PROTOCOL_CPGM("/");
        SERIAL_PROTOCOLLN(filesize);
    }
    else
    {
        SERIAL_PROTOCOLLN_CPGM(MSG_SD_NOT_PRINTING);
    }
}
void CardReader::write_command(CMDBUFFER *cmd)
{
	sprintf(cmdbuffer, "%c%d", cmd->code, cmd->value);
	strcat(cmdbuffer, "\r\n\0");
    file.write(cmdbuffer);
    if (file.writeError)
    {
        SERIAL_ERROR_START;
        SERIAL_ERRORLN_CPGM(MSG_SD_ERR_WRITE_TO_FILE);
    }
}


void CardReader::checkautostart(bool force)
{
    if (!force)
    {
        if(!autostart_stilltocheck)
            return;
        if(autostart_atmillis<millis())
            return;
    }
    autostart_stilltocheck = false;
    if (!cardOK)
    {
        initsd();
        if(!cardOK) //fail
        {
            return;
        }
    }
    
    char autoname[30];
    sprintf_P(autoname, PSTR("auto%i.g"), lastnr);
    for (int8_t i=0; i<(int8_t)strlen(autoname); i++)
    {
        autoname[i]=tolower(autoname[i]);
    }
    dir_t p;
    
    root.rewind();
    
    bool found = false;
    while (root.readDir(p, NULL) > 0)
    {
        for (int8_t i=0;i<(int8_t)strlen((char*)p.name);i++)
        {
            p.name[i]=tolower(p.name[i]);
        }
        //Serial.print((char*)p.name);
        //Serial.print(" ");
        //Serial.println(autoname);
        if (p.name[9]!='~') //skip safety copies
        {
            if(strncmp((char*)p.name,autoname,5)==0)
            {
				char cmd[30];
				sprintf_P(cmd, PSTR("M23 %s"), autoname);
				enquecommand(cmd);
				enquecommand_P(PSTR("M24"));
				found = true;
            }
        }
    }
    if(!found)
    {
        lastnr=-1;
    }
    else
    {
        lastnr++;
    }
}

void CardReader::closefile()
{
    file.sync();
    file.close();
    saving = false;
    logging = false;
}

void CardReader::getfilename(const uint8_t nr)
{
    curDir = &workDir;
    lsAction = LS_GetFilename;
    nrFiles = nr;
    curDir->rewind();
    lsDive("",*curDir);
}

uint16_t CardReader::getnrfilenames()
{
    curDir = &workDir;
    lsAction = LS_Count;
    nrFiles = 0;
    curDir->rewind();
    lsDive("",*curDir);
    //SERIAL_ECHOLN(nrFiles);
    return nrFiles;
}

void CardReader::chdir(const char * relpath)
{
    SdFile newfile;
    SdFile *parent = &root;
    
    if (workDir.isOpen())
    {
        parent = &workDir;
    }
    
    if (!newfile.open(*parent,relpath, O_READ))
    {
        SERIAL_ECHO_START;
        SERIAL_ECHO_CPGM(MSG_SD_CANT_ENTER_SUBDIR);
        SERIAL_ECHOLN(relpath);
    }
    else
    {
        if (workDirDepth < MAX_DIR_DEPTH)
        {
            for (int d = ++workDirDepth; d--;)
            {
                workDirParents[d+1] = workDirParents[d];
            }
            workDirParents[0]=*parent;
        }
        workDir=newfile;
    }
}

void CardReader::updir()
{
    if (workDirDepth > 0)
    {
        --workDirDepth;
        workDir = workDirParents[0];
        for (uint16_t d = 0; d < workDirDepth; d++)
        {
            workDirParents[d] = workDirParents[d+1];
        }
    }
}

void CardReader::printingHasFinished()
{
    start_print = false;
    sdprinting = false;
    file.close();
    st_synchronize();
    quickStop();
    autotempShutdown();
}
#endif //SDSUPPORT


