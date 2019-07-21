/**
 *  @file    enums.h
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#ifndef ENUMS_H
#define ENUMS_H

enum BlossomStates
{
    UNDEFINED_STATE = 0,
    CHANGED_STATE = 1,
    SKIPPED_STATE = 2,
    ERROR_INIT_STATE = 3,
    ERROR_PRECHECK_STATE = 4,
    ERROR_EXEC_STATE = 5,
    ERROR_POSTCHECK_STATE = 6,
    ERROR_CLOSE_STATE = 7
};

#endif // ENUMS_H
