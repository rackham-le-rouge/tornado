
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
#include <sys/ioctl.h>

/* Some constants */
#define DEBUG_LEVEL	                   0
#define FILE_ALREADY_DONE               "alreadyDone.dat"
#define URL_LENGTH                      9 /* At least add 1 to the real URL lenght for the \0 */
#define URL_INDEX_OF_NEW                "http://pastebin.com/archive"
#define URL_PREFIX                      "http://pastebin.com/"
#define TOKEN_DELIMITER_FOR_NEW_ENTRIES "border=\"0\" /><a href=\"/"
#define TOKEN_DELIMITER_FOR_DATA_START  "onkeydown=\"return catchTab(this,event)\">"
#define TOKEN_DELIMITER_FOR_DATA_END    "</textarea>"
#define RAW_PAGES_DIRECTORY             "records"
#define NUMBER_OF_ENTRIES_PER_PAGE      151
#define SEPARATION_CHARACTER            '#'     /* only one character allowed */
#define WAIT_TIME_MAX                   15
#define WAIT_TIME_MIN                   5
#define WAIT_TIME_URL_MAX               2
#define WAIT_TIME_URL_MIN               1
#define MAX_URL_BEFORE_SAVING_INIT      9
#define MAX_URL_NUMBER_OF_TURN_TO_FIX   10
#define MAX_URL_MAXIMUM_PER_TURN        50
#define PROGRESS_BAR_BODY_CHARACTER     '-'
#define PROGRESS_BAR_HEAD_CHARACTER     '>' 
#define SCREEN_SIZE_COLUMN_MIN          20

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
#else
#define LOG_ERROR(p_sStr, ...)
#endif

#if(DEBUG_LEVEL > 1)
#define LOG_WARNING(p_sStr, ...)      fprintf(stderr, "%s WARN [%d]:" p_sStr "\n", __FILE__, __LINE__, __VA_ARGS__);
#else
#define LOG_WARNING(p_sStr, ...)
#endif

#if(DEBUG_LEVEL > 2)
#define LOG_INFO(p_sStr, ...)      fprintf(stderr, "%s INFO [%d]:" p_sStr "\n", __FILE__, __LINE__, __VA_ARGS__);
#else
#define LOG_INFO(p_sStr, ...)
#endif


#if(DEBUG_LEVEL > 3)
#define LOG_MSG(p_sStr)      fprintf(stderr, "%s INFO [%d]:" p_sStr "\n", __FILE__, __LINE__);
#else
#define LOG_MSG(p_str)
#endif

#define LOG_PRINT(p_sStr, ...)      fprintf(stdout,  p_sStr "\n", __VA_ARGS__);


#include "parser.h"
#include "network.h"
#include "io.h"
#include "tornado.h"



#endif
