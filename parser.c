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
    int l_iStartingPositionOfBuffer = 0;
    int l_iIterator = 0;

    /* we create a buffer for new entries holding data like that : END_OF_URL1#END_OF_URL2#\0 */
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
            }while(l_cCurrentToken[l_iIndex++] != '\"');            /* cause the end of a new address is a " */
            l_iIndex = 0;

            /* close this entrie by adding a # */
            l_cBufferForNewEntries[l_iBufferForNewEntriesIndex++] = SEPARATION_CHARACTER;
            l_cBufferForNewEntries[l_iBufferForNewEntriesIndex + 1] = '\0';
        }
    }while(noMoreEntries == FALSE);

    /* We have to resize p_cNewUrlForThisSession according to the length of the string l_cBufferForNewEntries */
    p_cNewUrlForThisSession = realloc(p_cNewUrlForThisSession, (
                        strlen(p_cNewUrlForThisSession) +
                        strlen(l_cBufferForNewEntries) +
                        1 ) * sizeof(char));    

    if(p_cNewUrlForThisSession == NULL)
    {
        LOG_ERROR("Realloc failed, there is no more memory avaible...%s", " ");
        return EXIT_FAILURE;
    }

    /* end of parsing, we have to add entries in the main buffer */
    l_iStartingPositionOfBuffer  = strlen(p_cNewUrlForThisSession);
    LOG_INFO("New entries [%s] to add after ...%c]", l_cBufferForNewEntries, p_cNewUrlForThisSession[l_iStartingPositionOfBuffer - 1]);
    for(l_iIterator = 0; l_iIterator < (signed)strlen(l_cBufferForNewEntries) ; l_iIterator++)
    {
       p_cNewUrlForThisSession[l_iStartingPositionOfBuffer + l_iIterator] = l_cBufferForNewEntries[l_iIterator]; 
    }

    /* the new end of the buffer, in order to avoid overflow - INDEX OF THE LAST LETTER IN THE BUFFER \0 excluded*/
    *p_iIndexOfNewEnd = l_iStartingPositionOfBuffer + l_iIterator;
    LOG_INFO("New end of the buffer is %d and last character is %c", *p_iIndexOfNewEnd, p_cNewUrlForThisSession[*p_iIndexOfNewEnd]);
    /* put the end character */
    p_cNewUrlForThisSession[*p_iIndexOfNewEnd + 1] = '\0';

    return EXIT_SUCCESS;
}



/**
 * @brief extractAndEraseFirstToken
 * In order to take the first token of the sentence and erase it. Thus, we assume to be able to
 * lost a record if, in the next step we can't download the target
 * @param p_cNewUrlForThisSession : the first sentence, where we have to found the token
 * @param p_cFirstToken : the first token, and also the removed one from the main sentence
 */
void extractAndEraseFirstToken(char* p_cNewUrlForThisSession, char* p_cFirstToken)
{
    unsigned short int l_iIterator;
    unsigned short int l_iSubIterator;

    l_iIterator = 0;
    l_iSubIterator = 0;


    /* Scan the sentence until the end... */
    while(p_cNewUrlForThisSession[l_iIterator] != '\0')
    {
        /* ...and avoid the ##### at the beginning of the sentence */
        if(p_cNewUrlForThisSession[l_iIterator] != SEPARATION_CHARACTER)
        {
            /* When found something different from SEPARATION_CHARACTER, start the copy */
            while(p_cNewUrlForThisSession[l_iIterator + l_iSubIterator] != SEPARATION_CHARACTER)
            {
                /* take it */
                p_cFirstToken[l_iIterator] = p_cNewUrlForThisSession[l_iIterator + l_iSubIterator];
                /* and remove it */
                p_cNewUrlForThisSession[l_iIterator + l_iSubIterator] = SEPARATION_CHARACTER;
                l_iSubIterator++;
            }
            /* dirty, but need to go out of the while loop */
            break;
        }
        l_iIterator++;
    }
}
