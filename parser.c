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
    char* l_cBufferForNewEntries = (char*)malloc((URL_LENGTH*NUMBER_OF_ENTRIES_PER_PAGE + 1)*sizeof(char));
    int l_iIndex = 0;
    int l_iStartingPositionOfBuffer = 0;
    int l_iIterator = 0;
    int l_iCurrentToken = 0;


    UNUSED(p_iIndexOfNewEnd);

    /* Fill now with separation character */
    memset(l_cBufferForNewEntries, SEPARATION_CHARACTER, URL_LENGTH*NUMBER_OF_ENTRIES_PER_PAGE + 1);

    /* we create a buffer for new entries holding data like that : END_OF_URL1#END_OF_URL2#\0 */
    do
    {
        LOG_INFO("Search for a new token... Current is : %d", l_iCurrentToken);

        /* find the next token */
        l_cCurrentToken = strstr(p_structMemory.memory, TOKEN_DELIMITER_FOR_NEW_ENTRIES);
        if(l_cCurrentToken == NULL)
        {
            LOG_MSG("No more token in this page... End or parsing.");
            noMoreEntries = TRUE;
        }
        else
        {
            LOG_INFO("New token found, [%.8s]", l_cCurrentToken + strlen(TOKEN_DELIMITER_FOR_NEW_ENTRIES));

            /* because strstr gives the starting position of the searched sentence, but we
             * are looking for the data juste after. And clean the token in the the string  */
            memset(l_cCurrentToken, SEPARATION_CHARACTER, strlen(TOKEN_DELIMITER_FOR_NEW_ENTRIES));
            l_cCurrentToken += strlen(TOKEN_DELIMITER_FOR_NEW_ENTRIES);

            /* copy current token in the buffer, but use a reverse way in order to have the last record in first */
            do
            {
                LOG_INFO("position -%d- l_iCurrentToken, %d strlen %d index %d", (NUMBER_OF_ENTRIES_PER_PAGE - l_iCurrentToken - 1)*(8 + 1)+ l_iIndex, l_iCurrentToken, 8, l_iIndex);
                l_cBufferForNewEntries[(NUMBER_OF_ENTRIES_PER_PAGE - l_iCurrentToken - 1)
                        *(8 + 1)
                        + l_iIndex] 
                        = l_cCurrentToken[l_iIndex];
            }while(l_cCurrentToken[++l_iIndex] != '\"');            /* cause the end of a new address is a " */

            /* close the line adding a \0 */
            l_cBufferForNewEntries[URL_LENGTH*NUMBER_OF_ENTRIES_PER_PAGE] = '\0';

            l_iIndex = 0;
            l_iCurrentToken++;
        }
    }while(noMoreEntries == FALSE);
    LOG_MSG("End of this page. Leave the loop...");

    /* end of parsing, we have to add entries in the main buffer */
    /* l_iStartingPositionOfBuffer  = strlen(p_cNewUrlForThisSession); */
    l_iStartingPositionOfBuffer = 0;    /* cause we reset this buffer each time */
    LOG_INFO("l_iStartingPositionOfBuffer %d", l_iStartingPositionOfBuffer);
    LOG_INFO("New entries [%s] to add", l_cBufferForNewEntries);
    for(l_iIterator = 0; l_iIterator < (signed)strlen(l_cBufferForNewEntries) ; l_iIterator++)
    {
       p_cNewUrlForThisSession[l_iStartingPositionOfBuffer + l_iIterator] = l_cBufferForNewEntries[l_iIterator]; 
    }
    LOG_INFO("l_iIterator %d", l_iIterator);


    /* Release local memory */
    free(l_cBufferForNewEntries);
    l_cBufferForNewEntries = NULL;

    return EXIT_SUCCESS;
}



/**
 * @brief extractAndEraseFirstToken
 * In order to take the first token of the sentence and erase it. Thus, we assume to be able to
 * lost a record if, in the next step we can't download the target
 * @param p_cNewUrlForThisSession : the first sentence, where we have to found the token, if there is no more token, p_cFirstToken[0] == '\0'
 * @param p_cFirstToken : the first token, and also the removed one from the main sentence
 */
void extractAndEraseFirstToken(char* p_cNewUrlForThisSession, char* p_cFirstToken)
{
    unsigned short int l_iIterator;

    unsigned short int l_iSubIterator;
    char l_bNothingFound = TRUE;

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
                p_cFirstToken[l_iSubIterator] = p_cNewUrlForThisSession[l_iIterator + l_iSubIterator];
                /* and remove it */
                p_cNewUrlForThisSession[l_iIterator + l_iSubIterator] = SEPARATION_CHARACTER;
                l_iSubIterator++;
            }
            /* the previous ++ put us at the end of the buffer, set the end character */
            p_cFirstToken[l_iSubIterator] = '\0';

            l_bNothingFound = FALSE;

            /* dirty, but need to go out of the while loop */
            break;
        }
        l_iIterator++;
    }
    /* If there is no more token in this string */
    if(l_bNothingFound == TRUE)
    {
        p_cFirstToken[0] = '\0';
    }
}


/**
 * @brief extractDataFromAPage
 * After the URL download, we have to parse the page in order to extract usefull content,
 * save it in memory, and send back the address of the memory using the variable given
 * as a parameter
 * @param p_structMemory : data retrieved, the page is in the memory section
 * @param p_cDataOfAPage : pointer to a string where the function have to save the data
 */
void extractDataFromAPage(struct MemoryStruct p_structMemory, char** p_cDataOfAPage)
{
    char* l_cBeginning = NULL;
    char* l_cEnd = NULL;
    int l_iSize = 0;

    /* Find the initialization value of the loopo */
    l_cBeginning = strstr(p_structMemory.memory, TOKEN_DELIMITER_FOR_DATA_START);
    l_cBeginning += strlen(TOKEN_DELIMITER_FOR_DATA_START);

    /* Compute size of data to save */
    l_cEnd = strstr(p_structMemory.memory, TOKEN_DELIMITER_FOR_DATA_END);
    l_iSize = l_cEnd - l_cBeginning;

    LOG_INFO("Size of this page: %d", l_iSize);

    if(++l_iSize > (signed)p_structMemory.size)
    {
        LOG_WARNING("There is an error, instersting data are bigger than the page itself: %d", l_iSize);
    }
    else
    {
        /* Memory allocation */
        *p_cDataOfAPage = (char*)malloc(l_iSize*sizeof(char));

        /* Copy */
        memcpy(*p_cDataOfAPage, l_cBeginning, l_iSize);
        (*p_cDataOfAPage)[l_iSize - 1] = '\0';
    }
    /* LOG_INFO("Data: %s", (*p_cDataOfAPage)); */
}
