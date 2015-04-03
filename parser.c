/** 
 * @file        parser.c
 * @rogram      PasteBin aspirator, named after an aspirator compagny
 * @brief       Functions able to extract usefull informations from the downloaded html page
 * @description In order to parse downloaded data of the file
 * @date        2015 bitch
 * @licence     Beerware (rev 42)
 * @author      Jerome GRARD - Not your sister....
 */

#include "config.h"




/**
 * @brief parserForNewEntries
 * Function to extract url of the new pages posted on the most-recent-page
 * @param p_structMemory : structure holding the data of the page, and the size of it
 * @param p_NewUrlForThisSession : buffer to store all the new url
 * @param p_iIndexOfNewEnd : index of the new end of the buffer
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int parserForNewEntries(struct MemoryStruct p_structMemory, char* p_cNewUrlForThisSession, unsigned int* p_iIndexOfNewEnd)
{
    enum boolean noMoreEntries = FALSE;
    char* l_cCurrentToken = NULL;
    char l_cBufferForNewEntries[URL_LENGTH*NUMBER_OF_ENTRIES_PER_PAGE];
    int l_iIndex = 0;
    int l_iBufferForNewEntriesIndex = 0;

    do
    {
        /* find the next token */
        l_cCurrentToken = strstr(p_structMemory.memory, TOKEN_DELIMITER_FOR_NEW_ENTRIES);

        if(l_cCurrentToken == NULL)
        {
            noMoreEntries = TRUE;
        }
        else
        {
            /* copy current token in the buffer */
            do
            {
                l_cBufferForNewEntries[l_iBufferForNewEntriesIndex++] = l_cCurrentToken[l_iIndex];
            }while(l_cCurrentToken[l_iIndex++] != '\"');
            l_iIndex = 0;

            /* close this entrie by adding a # */
            l_cBufferForNewEntries[l_iBufferForNewEntriesIndex++] = SEPARATION_CHARACTER;
            l_cBufferForNewEntries[l_iBufferForNewEntriesIndex + 1] = '\0';
        }
    }while(noMoreEntries == FALSE);    

    /* We have to resize p_cNewUrlForThisSession according to the length of the string l_cBufferForNewEntries */
    p_cNewUrlForThisSession = realloc(p_cNewUrlForThisSession, 
                strlen((p_cNewUrlForThisSession) +
                        strlen(l_cBufferForNewEntries) +
                        1)*sizeof(char));    

    if(p_cNewUrlForThisSession == NULL)
    {
        LOG_ERROR("Realloc failed, there is no more memory avaible...%s", " ");
        return EXIT_FAILURE;
    }

    /* We have to fill the extended memory with the new URL */
    /* add at the end of p_cNewUrlForThisSession the content of l_cBufferForNewEntries and set the new size in p_iIndexOfNewEnd */
    return EXIT_SUCCESS;
}

