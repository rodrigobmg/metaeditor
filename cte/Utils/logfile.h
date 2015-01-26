#ifndef LOGFILE_H
#define LOGFILE_H

/**
* @modulo LogFile
* @autor  Rodrigo Monteiro 1313493
* @descrição Utilitário para escrever os logs de execução
* @versão
*   12/2014 1.0 Módulo inicial
*   12/2014 1.1 Módulo finalizado
**/

#include <stdio.h>
#include <stdlib.h>

const int MAX_STRING_SIZE = 260;
const int MAX_BUFFER = 65536;

/**
 * @brief The LogFile class
 */
class LogFile
{
public:
    /**
     * @brief LogFile ctor
     * @param LogFileName the name of logfile
     * @param LogDirectoryName the directory to place it
     * @param bWithDate print the logs with date ?
     */
    explicit LogFile(const char *LogFileName, const char *LogDirectoryName, bool bWithDate);
    /**
     * @brief LogFile dtor
     */
    ~LogFile();

    /**
     * @brief write a text in logfile
     * @param fmt the format of string and arguments
     */
    void write(const char *fmt, ...);

private:
    FILE* m_file;
    char  m_fileName[MAX_STRING_SIZE];
    char  m_directory[MAX_STRING_SIZE];
    char  m_buffer[MAX_BUFFER];
    bool  m_withDate;
};

#endif // LOGFILE_H
