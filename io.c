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
unsigned int loadAlreadyTakenPageFile(char*** p_cAlreadyDownloaded)
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

    *p_cAlreadyDownloaded = (char**)malloc(l_iLine*sizeof(char*));
    for(l_iIterator = 0; l_iIterator < l_iLine; l_iIterator++)
    {
        (*p_cAlreadyDownloaded)[l_iIterator] = (char*)malloc(URL_LENGTH*sizeof(char));

        /* Fill with \0 for j and after read data from file */
        memset((*p_cAlreadyDownloaded)[l_iIterator], '\0', URL_LENGTH);
        fscanf(l_fileFile, "%s\n", (*p_cAlreadyDownloaded)[l_iIterator]);
    } 

    fclose(l_fileFile);

    return l_iLine;
}

/** @brief
  * Open file FILE_ALREADY_DONE defined in the config.h and save all new downloaded pages in it.
  * @param p_cAlreadyDownloaded : pointer to the array of string filled with
  * the pages' end URL
  * @param p_iAlreadyDownloaded : number of records in the array of string
  * @return Return the state, EXIT_SUCCESS or EXIT_FAILURE
  */
unsigned int saveAlreadyTakenPageFile(char** p_cAlreadyDownloaded, int p_iOldAlreadyDownloaded, int p_iAlreadyDownloaded)
{
    FILE* l_fileFile = NULL;
    int l_iIterator = 0;


    if(p_iOldAlreadyDownloaded > p_iAlreadyDownloaded)
    {
        LOG_ERROR("Already downloaded entries (%d) are greater than before (%d), WTF ! Leave this mess", p_iAlreadyDownloaded, p_iOldAlreadyDownloaded);
        return EXIT_FAILURE;
    }
    if(p_iOldAlreadyDownloaded == p_iAlreadyDownloaded)
    {
        LOG_INFO("No more entries downloaded on this turn, nothing to add at the file. Leave this function.%s", " ");
        return EXIT_SUCCESS;
    }

    LOG_INFO("Append records from the %d to the %d.", p_iOldAlreadyDownloaded, p_iAlreadyDownloaded);

    /* Append mode */
    l_fileFile = fopen(FILE_ALREADY_DONE, "a");
    if(l_fileFile != NULL)
    {
        for(l_iIterator = p_iOldAlreadyDownloaded; l_iIterator < p_iAlreadyDownloaded; l_iIterator++)
        {
            LOG_INFO("Add the %dth [%s]", l_iIterator, p_cAlreadyDownloaded[l_iIterator]);
            fprintf(l_fileFile, "%s\n", p_cAlreadyDownloaded[l_iIterator]);
        }
        LOG_INFO("Wrote %d new entries.", l_iIterator);
    }
    else
    {
        LOG_ERROR("File %s isn't ready for a saving operation of the already taken pages... Abort.", FILE_ALREADY_DONE);
        return EXIT_FAILURE;
    }

    fclose(l_fileFile);

    return EXIT_SUCCESS;
}


/** @brief saveDownloadedPage
  * Save the usefull content of a page on the hard drive
  * @param p_cFileName : name of the downloaded page to save
  * @param p_cDataOfAPage : string holding the data of the page
  * @return EXIT_SUCCESS if success or EXIT_FAILURE in the other cases.
  */
int saveDownloadedPage(char* p_cFileName, char* p_cDataOfAPage)
{
    FILE* l_fileFile = NULL;
    char* l_cFileName = (char*)malloc((strlen(p_cFileName) + strlen(RAW_PAGES_DIRECTORY) + 2)*sizeof(char));

    if(p_cFileName == NULL)
    {
        return EXIT_FAILURE;
    }

    strcpy(l_cFileName, RAW_PAGES_DIRECTORY);
    l_cFileName[strlen(RAW_PAGES_DIRECTORY)] = '/';
    l_cFileName[strlen(RAW_PAGES_DIRECTORY) + 1] = '\0';
    strcat(l_cFileName, p_cFileName);

    l_fileFile = fopen(l_cFileName, "w+");
    if(l_fileFile != NULL)
    {
       /* in a secured version, use fnprintf FIXME */
       fprintf(l_fileFile, "%s", p_cDataOfAPage); 
       fclose(l_fileFile);

       free(l_cFileName);
       return EXIT_SUCCESS;
    }

    free(l_cFileName);
    return EXIT_FAILURE;
}

/** @brief createDirectory
  * Create a directory
  * @param p_cName : directory' name
  * @return EXIT_SUCCESS if success or EXIT_FAILURE in the other cases.
  */
int createDirectory(char* p_cName)
{
    LOG_INFO("Going to create directory %s", p_cName);
    return mkdir(p_cName, 0755);
}


/** @brief
  * checkConfigurationFiles is the main function to tests all needed files.
  * that's here we proceed to all calls for checks existance or rights on the needed files.
  * @param p_cAlreadyDownloaded : an array of strings designed to be filled by a called function...
  * @return the number of entries (already visited pages) loaded by the load functions
  */
unsigned int checkConfigurationFiles(char*** p_cAlreadyDownloaded)
{
    char l_cDirectory[] = RAW_PAGES_DIRECTORY; 


    if(checkIfAFileExist(FILE_ALREADY_DONE) == EXIT_FAILURE)
    {
        /* Means that we are in the first start or something like that */
        LOG_INFO("%s is not currently here. Means you are starting for the first time ?", FILE_ALREADY_DONE);
        return 0;
    }

    /* Doesn't care about the ret code, because EEXIST seems to be K.O, this 
     * function always send back -1 instead of a smarter code */
    createDirectory(l_cDirectory);

    return loadAlreadyTakenPageFile(p_cAlreadyDownloaded);    
}
