/** 
 * @file        parser.h
 * @rogram      PasteBin aspirator, named after an aspirator compagny
 * @brief       Prototypes of functions able to extract usefull informations from the downloaded html page
 * @description In order to parse downloaded data of the file
 * @date        2015 bitch
 * @licence     Beerware (rev 42)
 * @author      Jerome GRARD - Not your sister....
 */



#ifndef PARSER_H
#define PARSER_H


/*

	Header of the main file

*/

int parserForNewEntries(struct MemoryStruct p_structMemory, char* p_cNewUrlForThisSession, unsigned int* p_iIndexOfNewEnd);
void extractAndEraseFirstToken(char* p_cNewUrlForThisSession, char* p_cFirstToken);

#endif
