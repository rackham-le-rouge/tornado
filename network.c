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
        curl_easy_setopt(l_curlHandler, CURLOPT_USERAGENT, "libcurl-agent/1.0");

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
void downloadNewEntries(char* p_cNewUrlForThisSession, char** p_cAlreadyDownloaded, int* p_iNumberOfAlreadyDownloaded)
{
        struct MemoryStruct l_structMemory;
        char* l_cURL = (char*)malloc(URL_LENGTH*sizeof(char));
        char l_bNoMoreToken;
        char* l_cCurrentToken = (char*)malloc(URL_LENGTH*sizeof(char));

        l_bNoMoreToken = FALSE;


        while(l_bNoMoreToken == FALSE)
        {
                /* Extract the good token in the string in order to know witch page have to be downloaded */
                extractAndEraseFirstToken(p_cNewUrlForThisSession, l_cCurrentToken);
                LOG_INFO("The first token is : [%s]\nThe remaining sentence is :[%s]", l_cCurrentToken, p_cNewUrlForThisSession);

                /* Build the URL to download */
                strcpy(l_cURL, URL_PREFIX);
                strcat(l_cURL, l_cCurrentToken);

                if(retrieveAnUrl(URL_PREFIX , &l_structMemory) == EXIT_SUCCESS)
                {
                        /* means that the page is stored in l_structMemory->memory */
                        LOG_INFO("retrieved %d", l_structMemory.size);

                        /* Implement here the function to extract usefull content and save the page on the hard drive */
                        LOG_INFO("Record retrieved... Not recorded yet on the hard drive...%s", " ");

                        /* Add a record to the p_cAlreadyDownloaded at the last moment, don't forget to update p_iNumberOfAlreadyDownloaded */
                        p_cAlreadyDownloaded = realloc(p_cAlreadyDownloaded, (*p_iNumberOfAlreadyDownloaded + 1)*sizeof(char*));
                        if(p_cAlreadyDownloaded != NULL)
                        {
                            p_cAlreadyDownloaded[*p_iNumberOfAlreadyDownloaded] =  (char*)malloc(URL_LENGTH*sizeof(char));
                            if(p_cAlreadyDownloaded[*p_iNumberOfAlreadyDownloaded] == NULL)
                            {
                                LOG_ERROR("realloc failed, no more memory avaible...%s", " ");
                                return;
                            }
                            /* memory is reserved, thus, we can update the size, even if the copy failed... */
                            p_iNumberOfAlreadyDownloaded++;

                            strcpy(p_cAlreadyDownloaded[*p_iNumberOfAlreadyDownloaded - 1], l_cCurrentToken);
                        }
                        else
                        {
                            /* realloc failed -> no more memory :/ */
                            LOG_ERROR("realloc failed, no more memory avaible...%s", " ");
                            return;
                        }

                        if(l_structMemory.memory)
                        {
                                free(l_structMemory.memory);
                        }
                }
                else
                {
                        LOG_ERROR("Network error on URL %s", l_cCurrentToken);
                }
        }

        /* Function to clean p_cNewUrlForThisSession and have a clean string */
        p_cNewUrlForThisSession = realloc(p_cNewUrlForThisSession, 1 * sizeof(char));

        free(l_cCurrentToken);
        free(l_cURL);
}


/**
  * @brief Main network function, this is for the site pooling. Pool the index page
  *        in order to get the needed URL (by calling a parser in order to extract URL from
  *        the page) and then download pages one by one.
  * @param p_iNumberOfAlreadyDownloaded : kind of index in the already downloaded buffer
  * @paramp_cAlreadyDownloaded : really long string with a tab behavior. Belonging all URL already downloaded
  */
void networkLoop(int* p_iNumberOfAlreadyDownloaded, char** p_cAlreadyDownloaded)
{
        struct MemoryStruct l_structMemory;
        char* l_cNewUrlForThisSession = (char*)malloc(sizeof(char)); 
        unsigned int l_iIndexOfNewEnd = 0;

        /* init. there is a realloc just after in order to set the good size */
        l_cNewUrlForThisSession[0] = '\0';

        /* One loop is one INDEX page pool. We have a lot of URL, we have to exact these ; find the new one by
           comparing with the older one ; and then download the newest ; store the new URL as a old one */ 
        if(retrieveAnUrl(URL_INDEX_OF_NEW, &l_structMemory) == EXIT_SUCCESS)
        {
                /* means that the page is stored in l_structMemory->memory */
                LOG_INFO("retrieved %d", l_structMemory.size);

                parserForNewEntries(l_structMemory, l_cNewUrlForThisSession, &l_iIndexOfNewEnd);
                downloadNewEntries(l_cNewUrlForThisSession,
                                    p_cAlreadyDownloaded,
                                    p_iNumberOfAlreadyDownloaded);
                /* Currently useless... We supposed to manage the l_cNewUrlForThisSession buffer properly and
                 * eventually leave some entries in order to download them after, when the program have more time.
                 * But we don't. Anyway... Still here, and the idea too, maybee one day, a brave soul... */
                l_iIndexOfNewEnd=0;

                if(l_structMemory.memory)
                {
                        free(l_structMemory.memory);
                }
        }
       else
        {
            LOG_ERROR("There is an error with the network, we cant load the page... Abort %s"," ");
        }
}




