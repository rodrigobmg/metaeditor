#include "logfile.h"
#include <string.h>

#if _WIN32
    #undef UNICODE
    #include <windows.h>
    #pragma warning (disable : 4996 ) //_CRT_SECURE_NO_WARNINGS
#elif __linux || __APPLE__
    #include <sys/stat.h> //for mkdir
    #include <sys/types.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <stdarg.h>
#endif

LogFile::LogFile(const char* LogFileName,const char* LogDirectoryName, bool bWithDate)
{
    if (strcmp(LogFileName, "") == 0)
    {
        strcpy(this->m_fileName , "LOGDATA");
    }
    else
    {
        strcpy(this->m_fileName , LogFileName);
    }


    if (!strcmp(LogDirectoryName, ""))
    {
        strcpy(this->m_directory , "Logs");
    }
    else
    {
        strcpy(this->m_directory  , LogDirectoryName);
    }

    this->m_withDate = bWithDate;

#if _WIN32
    CreateDirectory(LogDirectoryName, NULL);
#elif __linux || __APPLE__
    mkdir(m_directory, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#else
    #pragma error ("Não é possível criar o diretório")
#endif
}

LogFile::~LogFile()
{
}

void LogFile::write(const char *fmt, ...)
{
    va_list argptr;
    char szLogFileName[MAX_STRING_SIZE];
#ifdef WIN32
    SYSTEMTIME strSystime;
    GetLocalTime(&strSystime);
    sprintf(szLogFileName, "%s\\%s %04d-%02d-%02d.txt",
        &this->m_directory[0] , &this->m_fileName [0],
        strSystime.wYear, strSystime.wMonth, strSystime.wDay);
#else
    sprintf(szLogFileName, "%s/%s.txt", this->m_directory, this->m_fileName);
#endif

    va_start(argptr, fmt);
    vsprintf(this->m_buffer , fmt, argptr);
    va_end(argptr);

    if ( (this->m_file = fopen(szLogFileName, "a+"))!=0)
    {
#if WIN32
        if (this->m_withDate ==0)
        {
            fprintf(this->m_file , "%s\n", this->m_buffer);
        }
        else
        {
            fprintf(this->m_file , "%d:%d:%d  %s\n", strSystime.wHour, strSystime.wMinute, strSystime.wSecond, this->m_buffer);
        }
#else
        fprintf(this->m_file, "%s\n", this->m_buffer);
#endif

        fclose(this->m_file );
    }
}
#if _WIN32 || _WIN64
#define UNICODE
#endif
