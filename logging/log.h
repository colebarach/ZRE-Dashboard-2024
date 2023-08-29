#ifndef LOG_H
#define LOG_H

// Log ------------------------------------------------------------------------------------------------------------------------
// 
// Description: Collection of logging macros, used to standardize output across a project.
//
// Created: 23.08.28
// Updated: 23.08.28

// Libraries ------------------------------------------------------------------------------------------------------------------

// C Standard Libraries
#include <stdio.h>

// Constants ------------------------------------------------------------------------------------------------------------------

#define LOG_INFO  printf("[INFO ] "); printf
#define LOG_ERROR printf("[ERROR] "); printf
#define LOG_WARN  printf("[INFO ] "); printf
#define LOG_FATAL printf("[FATAL] "); printf

#endif // LOG_H