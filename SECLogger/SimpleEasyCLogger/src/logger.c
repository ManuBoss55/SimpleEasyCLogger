/*
 ============================================================================
 Name        : logger.c
 Author      : Emanuele Cennamo
 Version     :
 Copyright   : apache 2.0 license
 Description : Logger library
 ============================================================================
 */

#include "init.h"
#include "queue.h"

const char *logger_enum_type_names[] = {
    [LOGGER_OK_SUCCESS]                           = "LOGGER_OK_SUCCESS",
    [LOGGER_ERROR_GENERAL]                        = "LOGGER_ERROR_GENERAL",
    [LOGGER_ERROR_POINTER_FILE_NULL]              = "LOGGER_ERROR_POINTER_FILE_NULL",
    [LOGGER_ERROR_SET_LEVEL_FAILED]               = "LOGGER_ERROR_SET_LEVEL_FAILED",
    [LOGGER_ERROR_CLOSE_FILE_FAILED]              = "LOGGER_ERROR_CLOSE_FILE_FAILED",
    [LOGGER_ERROR_ALLOCATION_FAILED]              = "LOGGER_ERROR_ALLOCATION_FAILED",
    [LOGGER_ERROR_NOT_SELECT_DATE_OR_TIME]        = "LOGGER_ERROR_NOT_SELECT_DATE_OR_TIME",
    [LOGGER_ERROR_OPEN_FILE_FAILED]               = "LOGGER_ERROR_OPEN_FILE_FAILED",
    [LOGGER_ERROR_CONCATENATE_FAILED]             = "LOGGER_ERROR_CONCATENATE_FAILED",
    [LOGGER_ERROR_SAFE_INIT_FAILED]               = "LOGGER_ERROR_SAFE_INIT_FAILED",
    [LOGGER_ERROR_DELETE_OLD_FILE]                = "LOGGER_ERROR_DELETE_OLD_FILE",
    [LOGGER_ERROR_MUTEX_INITIALIZATION_FAILED]    = "LOGGER_ERROR_MUTEX_INITIALIZATION_FAILED",
    [LOGGER_ERROR_EMPTY_QUEUE]                    = "LOGGER_ERROR_EMPTY_QUEUE",
    [LOGGER_ERROR_QUEUE_NULL_POINTER]             = "LOGGER_ERROR_QUEUE_NULL_POINTER",
    [LOGGER_ERROR_ACQUISITION_LOCK_FAILED]        = "LOGGER_ERROR_ACQUISITION_LOCK_FAILED",
    [LOGGER_ERROR_SETTING_MAX_MB_AND_MB_FOR_FILE] = "LOGGER_ERROR_SETTING_MAX_MB_AND_MB_FOR_FILE",
};

void logger_general(int level, const char *file, const char *function, int line, char *timeLog, const char *message)
{
    LoggerOperationState loggerOperationState;

    if ((logger.level_log_file < level && logger.file_log != NULL) || logger.level_log_console < level)
        return;
    if (logger.today != NULL && strcmp(logger.today, "") != 0)
        verify_log_day(&loggerOperationState);

    char *outInfo = NULL;
    char *charLevel = NULL;

    charLevel = convert_level(level);
    outInfo   = FormatOutputLogger(&loggerOperationState, "[%s] [%s] [%s:%s():%d]", timeLog, charLevel, file, function, line);

    char logConsole[strlen(outInfo) + 1 + strlen(message) + 1];
    sprintf(logConsole, "%s %s", outInfo, message);

    char logColorPrint[10 + strlen(logConsole) + 7 + 1];
    sprintf(logColorPrint, "%s", logConsole);

    free(outInfo);
    free(charLevel);

    if (logger.print_with_color)
    {
        switch (level)
        {
        case LOGGER_FATAL:
            sprintf(logColorPrint, "\033[1;35m%s\033[0m", logConsole);
            break;
        case LOGGER_ERROR:
            sprintf(logColorPrint, "\033[1;31m%s\033[0m", logConsole);
            break;
        case LOGGER_WARN:
            sprintf(logColorPrint, "\033[1;33m%s\033[0m", logConsole);
            break;
        case LOGGER_INFO:
            sprintf(logColorPrint, "\033[1;32m%s\033[0m", logConsole);
            break;
        case LOGGER_DEBUG:
            sprintf(logColorPrint, "\033[1;34m%s\033[0m", logConsole);
            break;
        case LOGGER_TRACE:
            sprintf(logColorPrint, "\033[1;36m%s\033[0m", logConsole);
            break;
        default:
            break;
        }
    }

    if (level <= logger.level_log_file && logger.file_log != NULL)
    {
        fprintf(logger.file_log, "%s\n", logConsole);
        fflush(logger.file_log);
    }
    if(level <= logger.level_log_console)
    	printf("%s\n", log_color_print);
}

/* ============================================================================ */

LoggerOperationState(logger_fatal)(const char *file, const char *function, int line, const char *format, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_FATAL <= logger.level_log_file || LOGGER_FATAL <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, format);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, format, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, format);
        char *stringLogMessage = GetStringFormatOutputLogger(&loggerOperationState, size, format, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_general(stringLogMessage, &loggerOperationState);
        free(stringLogMessage);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_FATAL, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_fatal_str)(const char *file, const char *function, int line, const char *caller, int userCat, uint32_t userCid, int resultOperation, const char *operation)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_FATAL <= logger.level_log_file || LOGGER_FATAL <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_str(caller, operation, userCat, userCid, resultOperation, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_FATAL, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_fatal_str_dyn)(const char *file, const char *function, int line, const char *caller, int userCat, uint32_t userCid, int resultOperation, const char *operationFormat, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_FATAL <= logger.level_log_file || LOGGER_FATAL <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, operationFormat);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, operationFormat);
        char *stringLogOperation = GetStringFormatOutputLogger(&loggerOperationState, size, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_str(caller, stringLogOperation, userCat, userCid, resultOperation, &loggerOperationState);
        free(stringLogOperation);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_FATAL, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_fatal_str_redux)(const char *file, const char *function, int line, const char *caller, int resultOperation, const char *operation)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_FATAL <= logger.level_log_file || LOGGER_FATAL <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_str_redux_log(caller, operation, resultOperation, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_FATAL, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_fatal_str_redux_dyn)(const char *file, const char *function, int line, const char *caller, int resultOperation, const char *operationFormat, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_FATAL <= logger.level_log_file || LOGGER_FATAL <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, operationFormat);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, operationFormat);
        char *stringLogOperation = GetStringFormatOutputLogger(&loggerOperationState, size, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_str_redux_log(caller, stringLogOperation, resultOperation, &loggerOperationState);
        free(stringLogOperation);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_FATAL, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}

LoggerOperationState(logger_error)(const char *file, const char *function, int line, const char *format, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_ERROR <= logger.level_log_file || LOGGER_ERROR <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, format);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, format, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, format);
        char *stringLogMessage = GetStringFormatOutputLogger(&loggerOperationState, size, format, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_general(stringLogMessage, &loggerOperationState);
        free(stringLogMessage);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_ERROR, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_error_str)(const char *file, const char *function, int line, const char *caller, int userCat, uint32_t userCid, int resultOperation, const char *operation)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_ERROR <= logger.level_log_file || LOGGER_ERROR <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        LoggerOperationState loggerOperationState;
        char                *stringLog = get_log_str(caller, operation, userCat, userCid, resultOperation, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_ERROR, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_error_str_dyn)(const char *file, const char *function, int line, const char *caller, int userCat, uint32_t userCid, int resultOperation, const char *operationFormat, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_ERROR <= logger.level_log_file || LOGGER_ERROR <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, operationFormat);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, operationFormat);
        char *stringLogOperation = GetStringFormatOutputLogger(&loggerOperationState, size, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_str(caller, stringLogOperation, userCat, userCid, resultOperation, &loggerOperationState);
        free(stringLogOperation);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_ERROR, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_error_str_redux)(const char *file, const char *function, int line, const char *caller, int resultOperation, const char *operation)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_ERROR <= logger.level_log_file || LOGGER_ERROR <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_str_redux_log(caller, operation, resultOperation, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_ERROR, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_error_str_redux_dyn)(const char *file, const char *function, int line, const char *caller, int resultOperation, const char *operationFormat, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_ERROR <= logger.level_log_file || LOGGER_ERROR <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, operationFormat);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, operationFormat);
        char *stringLogOperation = GetStringFormatOutputLogger(&loggerOperationState, size, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_str_redux_log(caller, stringLogOperation, resultOperation, &loggerOperationState);
        free(stringLogOperation);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_ERROR, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}

LoggerOperationState(logger_warn)(const char *file, const char *function, int line, const char *format, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_WARN <= logger.level_log_file || LOGGER_WARN <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, format);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, format, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, format);
        char *stringLogMessage = GetStringFormatOutputLogger(&loggerOperationState, size, format, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_general(stringLogMessage, &loggerOperationState);
        free(stringLogMessage);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_WARN, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_warn_str)(const char *file, const char *function, int line, const char *caller, int userCat, uint32_t userCid, int resultOperation, const char *operation)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_WARN <= logger.level_log_file || LOGGER_WARN <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_str(caller, operation, userCat, userCid, resultOperation, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_WARN, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_warn_str_dyn)(const char *file, const char *function, int line, const char *caller, int userCat, uint32_t userCid, int resultOperation, const char *operationFormat, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_WARN <= logger.level_log_file || LOGGER_WARN <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, operationFormat);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, operationFormat);
        char *stringLogOperation = GetStringFormatOutputLogger(&loggerOperationState, size, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_str(caller, stringLogOperation, userCat, userCid, resultOperation, &loggerOperationState);
        free(stringLogOperation);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_WARN, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_warn_str_redux)(const char *file, const char *function, int line, const char *caller, int resultOperation, const char *operation)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_WARN <= logger.level_log_file || LOGGER_WARN <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_str_redux_log(caller, operation, resultOperation, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_WARN, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_warn_str_redux_dyn)(const char *file, const char *function, int line, const char *caller, int resultOperation, const char *operationFormat, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_WARN <= logger.level_log_file || LOGGER_WARN <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, operationFormat);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, operationFormat);
        char *stringLogOperation = GetStringFormatOutputLogger(&loggerOperationState, size, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_str_redux_log(caller, stringLogOperation, resultOperation, &loggerOperationState);
        free(stringLogOperation);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_WARN, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}

LoggerOperationState(logger_info)(const char *file, const char *function, int line, const char *format, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_INFO <= logger.level_log_file || LOGGER_INFO <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, format);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, format, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, format);
        char *stringLogMessage = GetStringFormatOutputLogger(&loggerOperationState, size, format, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_general(stringLogMessage, &loggerOperationState);
        free(stringLogMessage);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_INFO, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_info_str)(const char *file, const char *function, int line, const char *caller, int userCat, uint32_t userCid, int resultOperation, const char *operation)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_INFO <= logger.level_log_file || LOGGER_INFO <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_str(caller, operation, userCat, userCid, resultOperation, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_INFO, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_info_str_dyn)(const char *file, const char *function, int line, const char *caller, int userCat, uint32_t userCid, int resultOperation, const char *operationFormat, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_INFO <= logger.level_log_file || LOGGER_INFO <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, operationFormat);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, operationFormat);
        char *stringLogOperation = GetStringFormatOutputLogger(&loggerOperationState, size, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_str(caller, stringLogOperation, userCat, userCid, resultOperation, &loggerOperationState);
        free(stringLogOperation);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_INFO, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_info_str_redux)(const char *file, const char *function, int line, const char *caller, int resultOperation, const char *operation)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_INFO <= logger.level_log_file || LOGGER_INFO <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_str_redux_log(caller, operation, resultOperation, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_INFO, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_info_str_redux_dyn)(const char *file, const char *function, int line, const char *caller, int resultOperation, const char *operationFormat, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_INFO <= logger.level_log_file || LOGGER_INFO <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, operationFormat);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, operationFormat);
        char *stringLogOperation = GetStringFormatOutputLogger(&loggerOperationState, size, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_str_redux_log(caller, stringLogOperation, resultOperation, &loggerOperationState);
        free(stringLogOperation);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_INFO, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}

LoggerOperationState(logger_debug)(const char *file, const char *function, int line, const char *format, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_DEBUG <= logger.level_log_file || LOGGER_DEBUG <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, format);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, format, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, format);
        char *stringLogMessage = GetStringFormatOutputLogger(&loggerOperationState, size, format, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_general(stringLogMessage, &loggerOperationState);
        free(stringLogMessage);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_DEBUG, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_debug_str)(const char *file, const char *function, int line, const char *caller, int userCat, uint32_t userCid, int resultOperation, const char *operation)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_DEBUG <= logger.level_log_file || LOGGER_DEBUG <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_str(caller, operation, userCat, userCid, resultOperation, &loggerOperationState);

        PushAndSignalQueueLogger(logger.queue, LOGGER_DEBUG, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_debug_str_dyn)(const char *file, const char *function, int line, const char *caller, int userCat, uint32_t userCid, int resultOperation, const char *operationFormat, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_DEBUG <= logger.level_log_file || LOGGER_DEBUG <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, operationFormat);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, operationFormat);
        char *stringLogOperation = GetStringFormatOutputLogger(&loggerOperationState, size, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_str(caller, stringLogOperation, userCat, userCid, resultOperation, &loggerOperationState);
        free(stringLogOperation);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_DEBUG, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_debug_str_redux)(const char *file, const char *function, int line, const char *caller, int resultOperation, const char *operation)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_DEBUG <= logger.level_log_file || LOGGER_DEBUG <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_str_redux_log(caller, operation, resultOperation, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_DEBUG, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_debug_str_redux_dyn)(const char *file, const char *function, int line, const char *caller, int resultOperation, const char *operationFormat, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_DEBUG <= logger.level_log_file || LOGGER_DEBUG <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, operationFormat);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, operationFormat);
        char *stringLogOperation = GetStringFormatOutputLogger(&loggerOperationState, size, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_str_redux_log(caller, stringLogOperation, resultOperation, &loggerOperationState);
        free(stringLogOperation);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_DEBUG, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}

LoggerOperationState(logger_trace)(const char *file, const char *function, int line, const char *format, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_TRACE <= logger.level_log_file || LOGGER_TRACE <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, format);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, format, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, format);
        char *stringLogMessage = GetStringFormatOutputLogger(&loggerOperationState, size, format, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_general(stringLogMessage, &loggerOperationState);
        free(stringLogMessage);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_TRACE, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_trace_str)(const char *file, const char *function, int line, const char *caller, int userCat, uint32_t userCid, int resultOperation, const char *operation)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_TRACE <= logger.level_log_file || LOGGER_TRACE <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_str(caller, operation, userCat, userCid, resultOperation, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_TRACE, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_trace_str_dyn)(const char *file, const char *function, int line, const char *caller, int userCat, uint32_t userCid, int resultOperation, const char *operationFormat, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_TRACE <= logger.level_log_file || LOGGER_TRACE <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, operationFormat);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, operationFormat);
        char *stringLogOperation = GetStringFormatOutputLogger(&loggerOperationState, size, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_log_str(caller, stringLogOperation, userCat, userCid, resultOperation, &loggerOperationState);
        free(stringLogOperation);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_TRACE, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_trace_str_redux)(const char *file, const char *function, int line, const char *caller, int resultOperation, const char *operation)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_TRACE <= logger.level_log_file || LOGGER_TRACE <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_str_redux_log(caller, operation, resultOperation, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_TRACE, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
LoggerOperationState(logger_trace_str_redux_dyn)(const char *file, const char *function, int line, const char *caller, int resultOperation, const char *operationFormat, ...)
{
    LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
    if (logger.init_logger && (LOGGER_TRACE <= logger.level_log_file || LOGGER_TRACE <= logger.level_log_console))
    {

        char *timeLog = logger_get_time(true, true, &loggerOperationState);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_list args;
        va_start(args, operationFormat);
        int size = GetSizeFormatOutputLogger(&loggerOperationState, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        va_start(args, operationFormat);
        char *stringLogOperation = GetStringFormatOutputLogger(&loggerOperationState, size, operationFormat, args);
        va_end(args);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        char *stringLog = get_str_redux_log(caller, stringLogOperation, resultOperation, &loggerOperationState);
        free(stringLogOperation);
        if (loggerOperationState != LOGGER_OK_SUCCESS)
            return loggerOperationState;

        PushAndSignalQueueLogger(logger.queue, LOGGER_TRACE, file, function, line, timeLog, stringLog, &loggerOperationState);
    }
    return loggerOperationState;
}
