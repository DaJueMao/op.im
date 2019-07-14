/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-12 14:42
 *   Filename        : log.h
 *   Description     : 
 * *******************************************************/

#ifndef INCLUDE_LOG_H
#define INCLUDE_LOG_H

#include <stdio.h>
#include <iostream>

#define NORMAL 0
#define NOTICE 1
#define WARNING 2
#define FATAL 4

void print(std::string message, int size);

#endif
