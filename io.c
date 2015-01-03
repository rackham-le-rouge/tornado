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


/** @brief
  * checkIfAFileExist : check existance of a file by trying to opening it
  * @param p_cFileName : file to test
  * @return EXIT_SUCCESS if there is a success or EXIT_FAILURE in the other case
  */
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



/** @brief
  * Check if you can read & write the file provided as a parameter by trying
  * to fopen it with w+ mode
  * @param p_cFileName : name of the file to test
  * @return EXIT_SUCCESS if success or EXIT_FAILURE in the other cases.
  */
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



/** @brief
  * Open file FILE_ALREADY_DONE defined in the config.h and load its data
  * in the memory at the address given by parameters. This function also
  * do the memory allocation.
  * @param p_cAlreadyDownloaded : pointer to the array of string to fill with
  * the file datas
  * @return Return the number of records read from the file (equivalent to the
  * number of lines in the file
  */
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


/** @brief
  * checkConfigurationFiles is the main function to tests all needed files.
  * that's here we proceed to all calls for checks existance or rights on the needed files.
  * @param p_cAlreadyDownloaded : an array of strings designed to be filled by a called function...
  * @return the number of entries (already visited pages) loaded by the load functions
  */
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
