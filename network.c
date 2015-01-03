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


int retrieveAnUrl(const char* p_cUrlToGet)
{
        CURL* l_curlHandler = NULL;
        CURLcode l_curlResponseCode = 0;
        struct MemoryStruct l_structMemory;


        l_structMemory.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
        l_structMemory.size = 0;    /* no data at this point */ 

        curl_global_init(CURL_GLOBAL_ALL);

        /* init the curl session */ 
        l_curlHandler = curl_easy_init();

        /* specify URL to get */ 
        curl_easy_setopt(l_curlHandler, CURLOPT_URL, p_cUrlToGet);

        /* send all data to this function  */ 
        curl_easy_setopt(l_curlHandler, CURLOPT_WRITEFUNCTION, writeMemoryCallback);

        /* we pass our struct to the callback function */ 
        curl_easy_setopt(l_curlHandler, CURLOPT_WRITEDATA, (void *)&l_structMemory);

        /* some servers don't like requests that are made without a user-agent
           field, so we provide one */ 
        curl_easy_setopt(l_curlHandler, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        /* get it! */ 
        l_curlResponseCode = curl_easy_perform(l_curlHandler);

        /* check for errors */ 
        if(l_curlResponseCode != CURLE_OK)
        {
                LOG_ERROR("curl_easy_perform() failed: %s", curl_easy_strerror(l_curlResponseCode));
                return EXIT_FAILURE;
        }
        else
        {
                /* Data is stored in memory, we can parse it ! */
        }

        /* cleanup curl stuff */ 
        curl_easy_cleanup(l_curlHandler);
        if(l_structMemory.memory)
        {
                free(l_structMemory.memory);
        }
        curl_global_cleanup();

        return EXIT_SUCCESS;
}



void networkLoop(int p_iNumberOfAlreadyDownloaded, char** p_cAlreadyDownloaded)
{
        UNUSED(p_iNumberOfAlreadyDownloaded);
        UNUSED(p_cAlreadyDownloaded);
        retrieveAnUrl(URL_INDEX_OF_NEW);
}
