/**
 * @file        tornado.h
 * @program     tornado
 * @brief       the main header
 * @description Header of the main file of the aspirator Tornado !
 * @date        2015 bitch
 * @licence     Beerware (rev 42)
 * @author      Jerome GRARD - Not your sister...
 */



#ifndef TORNADO_H
#define TORNADO_H


/*

	Header of the main file

*/


void waitBetweenTwoTurn(void);
void waitBetweenTwoURL(void);
void getWindowsSize(unsigned int* p_iRow, unsigned int* p_iCol);
void printProgressBar(unsigned int p_iMax, unsigned int p_iCurrentValue, char* p_cCurrentToken, unsigned int p_iAllRecords);

#endif
