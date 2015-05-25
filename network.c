/** 
 * @file        network.c
 * @rogram      PasteBin aspirator, named after an aspirator compagny
 * @brief       Network stuff, in order to connect to the net and retreive pages
 * @description In order to download pages
 * @date        2015 bitch
 * @licence     Beerware (rev 42)
 * @author      Jerome GRARD - Not your sister....
 */

#include "config.h"


/**
 * @brief writeMemoryCallback is the called function, when the request is executed. It aim to reseize the allocated memory for
 * for the buffer. Usinf a realloc.
 * @param p_contents : retrieved data from the internet
 * @param p_size : size of the retreieved data in (blocks ?)
 * @param nmemb : size of one (block ?)
 * @param p_data : the structure used to carry the data
 * @return real size of the retrieved data (and thus of the extended memory location)
 */
size_t writeMemoryCallback(void* p_contents, size_t p_size, size_t nmemb, void* p_data)
{
        size_t l_sizeRealsize = p_size * nmemb;
        struct MemoryStruct* l_structMemory = (struct MemoryStruct*)p_data;

        l_structMemory->memory = realloc(l_structMemory->memory, l_structMemory->size + l_sizeRealsize + 1);
        if(l_structMemory->memory == NULL)
        {
                /* out of memory! */ 
                LOG_ERROR("Not enough memory (realloc returned NULL)%s", ".");
                return EXIT_FAILURE;
        }

        memcpy(&(l_structMemory->memory[l_structMemory->size]), p_contents, l_sizeRealsize);
        l_structMemory->size += l_sizeRealsize;
        l_structMemory->memory[l_structMemory->size] = 0;

        return l_sizeRealsize;
}

/**
 * @brief Retreive html for the given url, and store it in the struture
 * @param p_cUrlToGet : the url to download
 * @param p_structMemory : the memory location of the struture to fill with the data
 * @return EXIT_SUCCESS for a a success or EXIT_FAILURE in the other cases
 */
int retrieveAnUrl(const char* p_cUrlToGet, struct MemoryStruct* p_structMemory)
{
        CURL* l_curlHandler = NULL;
        CURLcode l_curlResponseCode = 0;

        /* Initialisation with useless values */
        p_structMemory->memory = malloc(1);
        p_structMemory->size = 0; 

        curl_global_init(CURL_GLOBAL_ALL);

        /* init the curl session */ 
        l_curlHandler = curl_easy_init();

        /* specify URL to get */ 
        curl_easy_setopt(l_curlHandler, CURLOPT_URL, p_cUrlToGet);

        /* send all data to this function  */ 
        curl_easy_setopt(l_curlHandler, CURLOPT_WRITEFUNCTION, writeMemoryCallback);

        /* we pass our struct to the callback function */ 
        curl_easy_setopt(l_curlHandler, CURLOPT_WRITEDATA, (void *)p_structMemory);

        /* some servers don't like requests that are made without a user-agent
           field, so we provide one */ 
        curl_easy_setopt(l_curlHandler, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux i686; rv:31.0) Gecko/20100101 Firefox/31.0 Iceweasel/31.6.0");    //libcurl-agent/1.0");

        /* get it! */ 
        l_curlResponseCode = curl_easy_perform(l_curlHandler);

        /* check for errors, on the other cases, the page is stored in the memory */ 
        if(l_curlResponseCode != CURLE_OK)
        {
                LOG_ERROR("Function curl_easy_perform() failed: %s", curl_easy_strerror(l_curlResponseCode));
                return EXIT_FAILURE;
        }

        /* cleanup curl stuff */ 
        curl_easy_cleanup(l_curlHandler);
        curl_global_cleanup();

        return EXIT_SUCCESS;
}



/**
  * @brief downloadNewEntries is the function made for download new pages taken from the index of the released commit on the site.
  *         the new entries is taken from a buffer gived as a parameter
  * @param p_cNewUrlForThisSession : long chain, storing all new pages to download
  * @param p_iIndexOfNewEnd : integer to know where is the end of the p_cNewUrlForThisSession string
  * @param p_cAlreadyDownloaded : double string table storing all the already downloaded pages. One line per page, storing a string corresponding to the end of the URL
  * @param p_iNumberOfAlreadyDownloaded : number of already downloaded pages since the first start of the program
  */
void downloadNewEntries(char* p_cNewUrlForThisSession, char*** p_cAlreadyDownloaded, int* p_iNumberOfAlreadyDownloaded, unsigned int p_iNumberOfToken)
{
        struct MemoryStruct l_structMemory;
        char* l_cURL = (char*)malloc((URL_LENGTH + strlen(URL_PREFIX) + 1)*sizeof(char));
        char l_bNoMoreToken;
        char* l_cCurrentToken = (char*)malloc(URL_LENGTH*sizeof(char));
        char* l_cDataOfAPage = NULL; 
        char l_bNotAlreadyDownloaded;
        int l_iIterator;
        int l_iCurrentToken = 0;          /* count only the downloaded one during this turn */
        int l_iRealCurrentToken = 0;      /* count the skipped ones */

        l_bNoMoreToken = FALSE;
        l_bNotAlreadyDownloaded = TRUE;
        memset(l_cURL, '\0', URL_LENGTH + strlen(URL_PREFIX) + 1);

        LOG_MSG("Download new entries - Entering into the function...");

        while(l_bNoMoreToken == FALSE)
        {
                do
                {
                    /* Extract the good token in the string in order to know witch page have to be downloaded */
                    extractAndEraseFirstToken(p_cNewUrlForThisSession, l_cCurrentToken);

                    /* Find if there is no more token */
                    if(l_cCurrentToken[0] == '\0')
                    {
                        /* means there is no more token, so put a TRUE and leave this function */
                        l_bNoMoreToken = TRUE;
                        LOG_MSG("There is no more token in this turn...");
                        break;
                    }
                    else
                    {
                        l_bNotAlreadyDownloaded = FALSE;
                    }

                    /* Find if we have already downloaded this URL */
                    for(l_iIterator = 0; l_iIterator < *p_iNumberOfAlreadyDownloaded ; l_iIterator++)
                    {
                        l_bNotAlreadyDownloaded = strcmp((*p_cAlreadyDownloaded)[l_iIterator], l_cCurrentToken) == 0 ? TRUE : l_bNotAlreadyDownloaded;
                    }

                    l_iRealCurrentToken++;
                }while(l_bNotAlreadyDownloaded == TRUE);


                if(l_bNoMoreToken != TRUE)
                {
                        /* Build the URL to download */
                        strcpy(l_cURL, URL_PREFIX);
                        strcat(l_cURL, l_cCurrentToken);

                        LOG_INFO("The first token is : [%s][Real %d][Down %d] -- The remaining sentence is :[%s]", l_cCurrentToken, l_iRealCurrentToken, l_iCurrentToken, p_cNewUrlForThisSession);

                        if(retrieveAnUrl(l_cURL, &l_structMemory) == EXIT_SUCCESS)
                        {
                                /* means that the page is stored in l_structMemory->memory */
                                LOG_INFO("retrieved %d", l_structMemory.size);

                                /* Implement here the function to extract usefull content and save the page on the hard drive */
                                extractDataFromAPage(l_structMemory, &l_cDataOfAPage);
                                if(l_cDataOfAPage == NULL)
                                {
                                    /* There is an error, page size is negative - Need to abort - dirty goto, but used for error bandling */
                                    goto jumpAfterSizeFail;
                                }

                                /* Save the just downloaded page */
                                saveDownloadedPage(l_cCurrentToken, l_cDataOfAPage);

                                /* Just wait a little bit in order to be forgotten by the website */
                                waitBetweenTwoURL();
       
                                /* The progressbar */
                                printProgressBar(p_iNumberOfToken, ((unsigned)l_iRealCurrentToken), l_cCurrentToken, *p_iNumberOfAlreadyDownloaded);

                                /* Release memory needed by the save function, don't use realloc */
                                if(l_cDataOfAPage != NULL)
                                {
                                    free(l_cDataOfAPage);
                                }
jumpAfterSizeFail:
                                /* Add a record to the p_cAlreadyDownloaded at the last moment, don't forget to update p_iNumberOfAlreadyDownloaded */
                                *p_cAlreadyDownloaded = realloc(*p_cAlreadyDownloaded, (*p_iNumberOfAlreadyDownloaded + 1)*sizeof(char*));
                                if(*p_cAlreadyDownloaded != NULL)
                                {
                                        (*p_cAlreadyDownloaded)[*p_iNumberOfAlreadyDownloaded] =  (char*)malloc(URL_LENGTH*sizeof(char));
                                        if((*p_cAlreadyDownloaded)[*p_iNumberOfAlreadyDownloaded] == NULL)
                                        {
                                                LOG_ERROR("realloc failed, no more memory avaible...%s"," ");
                                                return;
                                        }

                                        /* memory is reserved, thus, we can update the size, even if the copy failed... */
                                        (*p_iNumberOfAlreadyDownloaded)++;
                                        strcpy((*p_cAlreadyDownloaded)[*p_iNumberOfAlreadyDownloaded - 1], l_cCurrentToken);
                                }
                                else
                                {
                                        /* realloc failed -> no more memory :/ */
                                        LOG_ERROR("realloc failed, no more memory avaible...%s"," ");
                                        return;
                                }

                                if(l_structMemory.memory)
                                {
                                        free(l_structMemory.memory);
                                        l_structMemory.memory = NULL;
                                }
                        }
                        else
                        {
                                LOG_ERROR("Network error on URL %s", l_cCurrentToken);
                        }
                }
                /* Count the number of URL retrieved for this session */
                l_iCurrentToken++;

                if(l_iCurrentToken >= MAX_URL_BEFORE_SAVING)
                {
                    l_bNoMoreToken = TRUE;
                }
        }

        /* Release the Kra... memory */
        LOG_MSG("Release all memory of this turn...");
        free(l_cCurrentToken);
        free(l_cURL);

        l_cCurrentToken = NULL;
        l_cURL = NULL;
}


/**
  * @brief Main network function, this is for the site pooling. Pool the index page
  *        in order to get the needed URL (by calling a parser in order to extract URL from
  *        the page) and then download pages one by one.
  * @param p_iNumberOfAlreadyDownloaded : kind of index in the already downloaded buffer
  * @paramp_cAlreadyDownloaded : really long string with a tab behavior. Belonging all URL already downloaded
  */
void networkLoop(int* p_iNumberOfAlreadyDownloaded, char*** p_cAlreadyDownloaded)
{
        struct MemoryStruct l_structMemory;
        char l_cLogMessage[URL_LENGTH];
        char* l_cNewUrlForThisSession = (char*)malloc(URL_LENGTH*NUMBER_OF_ENTRIES_PER_PAGE*sizeof(char)); 
        unsigned int l_iIndexOfNewEnd = 0;
        unsigned int l_iNumberOfToken = 0;
        int l_iOldNumberOfAlreadyDownloaded = 0;
        char l_bStopStalking = FALSE;

        /* init.*/ 
        l_cNewUrlForThisSession[0] = '\0';

        LOG_MSG("Network loop - Entering... ");

        while(l_bStopStalking == FALSE)
        {
                /* A mecanism to stop the loop FIXME*/
                if(URL_LENGTH > 100) l_bStopStalking = TRUE;

                /* One loop is one INDEX page pool. We have a lot of URL, we have to exact these ; find the new one by
                   comparing with the older one ; and then download the newest ; store the new URL as a old one */ 
                if(retrieveAnUrl(URL_INDEX_OF_NEW, &l_structMemory) == EXIT_SUCCESS)
                {
                        /* means that the page is stored in l_structMemory->memory */
                        LOG_INFO("retrieved %d", l_structMemory.size);

                        parserForNewEntries(l_structMemory, l_cNewUrlForThisSession, &l_iIndexOfNewEnd, &l_iNumberOfToken);
                        l_iOldNumberOfAlreadyDownloaded = *p_iNumberOfAlreadyDownloaded;
                        downloadNewEntries(l_cNewUrlForThisSession,
                                            p_cAlreadyDownloaded,
                                            p_iNumberOfAlreadyDownloaded,
                                            l_iNumberOfToken);
                        /* Currently useless... We supposed to manage the l_cNewUrlForThisSession buffer properly and
                         * eventually leave some entries in order to download them after, when the program have more time.
                         * But we don't. Anyway... Still here, and the idea too, maybee one day, a brave soul... */
                        l_iIndexOfNewEnd = 0;

                        /* Save downloaded pages in the record - We have to put a * because p_cAlreadyDownloaded is a pointer on the real l_cAlreadyDownloaded array of string
                         * thus we have to just send the array of string, not the pointer on the variable who store the pointer because we don't have to modify it */
                        LOG_MSG("Save the new visited pages...");
                        strcpy(l_cLogMessage, " saving ");
                        printProgressBar(l_iNumberOfToken, l_iNumberOfToken, l_cLogMessage, *p_iNumberOfAlreadyDownloaded);
                        saveAlreadyTakenPageFile(*p_cAlreadyDownloaded, l_iOldNumberOfAlreadyDownloaded, *p_iNumberOfAlreadyDownloaded);

                        /* FIXME - wait another records */
                        strcpy(l_cLogMessage, "waiting");
                        printProgressBar(l_iNumberOfToken, l_iNumberOfToken, l_cLogMessage, *p_iNumberOfAlreadyDownloaded);
                        waitBetweenTwoTurn();

                        if(l_structMemory.memory)
                        {
                                free(l_structMemory.memory);
                                l_structMemory.memory = NULL;
                        }
                }
               else
                {
                    LOG_ERROR("There is an error with the network, we cant load the page... Abort.%s"," ");
                }
        }

        LOG_MSG("Network loop broken by user. Program's ending is near...");
}




