
/*******************************************************

		Program configuration
		Set parameters here and
		recompile in order to apply
		Need a 'make clean' !!!

*******************************************************/



#ifndef CONFIG_H
#define CONFIG_H

/* Some includes */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <curl/curl.h>

/* Some constants */
#define	DEBUG_LEVEL	                    3
#define FILE_ALREADY_DONE               "alreadyDone.dat"
#define URL_LENGTH                      8
#define URL_INDEX_OF_NEW                "http://pastebin.com/archive"
#define URL_PREFIX                      "http://pastebin.com/"
#define TOKEN_DELIMITER_FOR_NEW_ENTRIES "border=\"0\" /><a href=\"/"
#define NUMBER_OF_ENTRIES_PER_PAGE      50
#define SEPARATION_CHARACTER            '#'

/* Logic stuff */
enum boolean
{
    TRUE,
    FALSE,
    DONT_KNOW
};


/* Structures */
struct MemoryStruct {
  char *memory;
  size_t size;
};



/* Debug part */
#define UNUSED(x) (void)(x)

#if(DEBUG_LEVEL > 0)
#define LOG_ERROR(p_sStr, ...)      fprintf(stderr, "%s ERR [%d]:" p_sStr "\n", __FILE__, __LINE__, __VA_ARGS__);
#endif

#if(DEBUG_LEVEL > 1)
#define LOG_WARNING(p_sStr, ...)      fprintf(stderr, "%s WARN [%d]:" p_sStr "\n", __FILE__, __LINE__, __VA_ARGS__);
#endif

#if(DEBUG_LEVEL > 2)
#define LOG_INFO(p_sStr, ...)      fprintf(stderr, "%s INFO [%d]:" p_sStr "\n", __FILE__, __LINE__, __VA_ARGS__);
#endif

#include "parser.h"
#include "network.h"
#include "io.h"
#include "tornado.h"



#endif
