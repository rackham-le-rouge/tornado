/**
 * @file        io.c
 * @program     PasteBin aspirator, named after an aspirator compagny
 * @brief       IO header, for read/write on files
 * @description All functions to read write on the program configuration files
 * @date        2015 bitch
 * @licence     Beerware (rev 42)
 * @author      Jerome GRARD - Not your sister....
 */


#include "config.h"

int checkIfAFileExist(const char* p_cFileName)
{
    FILE* l_fileFile = NULL;

    if(p_cFileName == NULL)
    {
        return EXIT_FAILURE;
    }

    l_fileFile = fopen(p_cFileName, "r");
    if(l_fileFile == NULL)
    {
        return EXIT_FAILURE;
    }

    fclose(l_fileFile);

    return EXIT_SUCCESS;
}



int checkReadWriteFile(const char* p_cFileName)
{
    FILE* l_fileFile = NULL;

    if(p_cFileName == NULL)
    {
        return EXIT_FAILURE;
    }

    l_fileFile = fopen(p_cFileName, "w+");
    if(l_fileFile == NULL)
    {
        return EXIT_FAILURE;
    }

    fclose(l_fileFile);

    return EXIT_SUCCESS;
}


unsigned int loadAlreadyTakenPageFile(char** p_cAlreadyDownloaded)
{
    FILE* l_fileFile = NULL;
    unsigned int l_iLine = 0;
    char l_cCharacter = 0;
    unsigned int l_iIterator = 0;

    l_fileFile = fopen(FILE_ALREADY_DONE, "r");
    while(l_cCharacter != EOF)
    {
        l_cCharacter = fgetc(l_fileFile);
        if(l_cCharacter == '\n')
        {
            l_iLine++;
        }
    }

    rewind(l_fileFile);

    p_cAlreadyDownloaded = (char**)malloc(l_iLine*sizeof(char*));
    for(l_iIterator = 0; l_iIterator < l_iLine; l_iIterator++)
    {
        p_cAlreadyDownloaded[l_iIterator] = (char*)malloc(URL_LENGTH*sizeof(char));

        /* Fill with \0 for j and after read data from file */
        memset(p_cAlreadyDownloaded[l_iIterator], '\0', URL_LENGTH);
        fscanf(l_fileFile, "%s\n", p_cAlreadyDownloaded[l_iIterator]);
    } 

    fclose(l_fileFile);

    return l_iLine;
}

unsigned int checkConfigurationFiles(char** p_cAlreadyDownloaded)
{
    if(checkIfAFileExist(FILE_ALREADY_DONE) == EXIT_FAILURE)
    {
        /* Means that we are in the first start or something like that */
        LOG_INFO("%s is not currently here. Means you are starting for the first time ?", FILE_ALREADY_DONE);
        return 0;
    }
    else
    {
        return loadAlreadyTakenPageFile(p_cAlreadyDownloaded);
    }
}
