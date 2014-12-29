/**
 * @file        tornado.c
 * @rogram      PasteBin aspirator, named after an aspirator compagny
 * @brief       Main part of the program
 * @description All the useless thing to start a program, like the administrative part used to start a compagny
 * @date        2015 bitch
 * @licence     Beerware (rev 42)
 * @author      Jerome GRARD - Not your sister....
 */


/**
 *   ERRATUM
 *
 *i.e : l_cCurrentCharacter   == l is for local (g : global and p : parameter) // c is for char (i : integer, f : float etc...)
 *                              CurrentCharacter = if you don't get it, turn off your computer. This thing isn't made for you... Go watch TV.
 */


/** To save the current PID to put it at the begin of each line */
int g_iCurrentPID;

#include "config.h"


/** Revision and version number */
char rev[] = "0.1";



/** Main
  * @brief Main function of the program, this is the starting point
  * @param argc : number of parameters gived to the program
  * @param argv : 2D array to store all the parameters gived to the program
  */
int main(int argc, char** argv)
{
    char** l_cAlreadyDownloaded = NULL;
    UNUSED(argc);
    UNUSED(argv);

    LOG_INFO( "\n-------------------------------------\n"
                "         Tornado %s starting\n"
                "-------------------------------------", rev)

    LOG_INFO("number file : %u", checkConfigurationFiles(l_cAlreadyDownloaded));

    exit(EXIT_SUCCESS);
    return EXIT_SUCCESS;	

}
