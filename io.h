/**
 * @file        io.h
 * @program     PasteBin aspirator, named after an aspirator compagny
 * @brief       IO header, for read/write on files
 * @description All functions to read write on the program configuration files
 * @date        2015 bitch
 * @licence     Beerware (rev 42)
 * @author      Jerome GRARD - Not your sister....
 */




#ifndef _IO_H_
#define _IO_H_


int checkIfAFileExist(const char* p_cFileName);
int checkReadWriteFile(const char* p_cFileName);
unsigned int loadAlreadyTakenPageFile(char*** p_cAlreadyDownloaded);
unsigned int checkConfigurationFiles(char*** p_cAlreadyDownloaded);
unsigned int saveAlreadyTakenPageFile(char** p_cAlreadyDownloaded, int p_iOldAlreadyDownloaded, int p_iAlreadyDownloaded);







#endif
