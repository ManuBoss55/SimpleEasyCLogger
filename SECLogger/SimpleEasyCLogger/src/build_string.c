/*
 ============================================================================
 Name        : build_string.c
 Author      : Emanuele Cennamo
 Version     :
 Copyright   : apache 2.0 license
 Description : Logger library
 ============================================================================
 */

#include "init.h"


int GetSizeFormatOutputLogger(LoggerOperationState *loggerOperationState, const char *formatstring, va_list args) {
	int Size = vsnprintf(NULL, 0, formatstring, args);
	if (Size == -1) {
		*loggerOperationState = LOGGER_ERROR_CONCATENATE_FAILED;
	} else {
		*loggerOperationState = LOGGER_OK_SUCCESS;
	}
	return Size;
}

char* GetStringFormatOutputLogger(LoggerOperationState *loggerOperationState, int Size, const char *formatstring, va_list args) {
	char *buffer = (char*) malloc((Size + 1) * sizeof(char));
	if (buffer == NULL) {
		*loggerOperationState = LOGGER_ERROR_ALLOCATION_FAILED;
		return NULL;
	}
	if (vsnprintf(buffer, Size + 1, formatstring, args) == -1) {
		free(buffer);
		*loggerOperationState = LOGGER_ERROR_CONCATENATE_FAILED;
	} else {
		*loggerOperationState = LOGGER_OK_SUCCESS;
	}
	return buffer;
}

char* FormatOutputLogger(LoggerOperationState *loggerOperationState, const char *formatstring, ...) {
	va_list args;
	va_start(args, formatstring);
	int Size = GetSizeFormatOutputLogger(loggerOperationState, formatstring, args);
	va_end(args);
	if(*loggerOperationState != LOGGER_OK_SUCCESS)
		return NULL;

	va_start(args, formatstring);
	char* buffer = GetStringFormatOutputLogger(loggerOperationState, Size, formatstring, args);
	va_end(args);
	if(*loggerOperationState != LOGGER_OK_SUCCESS) {
		free(buffer);
		return NULL;
	}
	return buffer;
}

char *logger_get_time(bool print_date, bool print_time, LoggerOperationState *loggerOperationState){
	if (!print_date && !print_time){
		*loggerOperationState = LOGGER_ERROR_NOT_SELECT_DATE_OR_TIME;
		return NULL;
	}

	char *buf;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm tm_info;
	localtime_r(&tv.tv_sec, &tm_info);

	char result[24];
	int dim = 0;
	if(print_date && print_time) {
		dim = 20;
		char stime[dim];
		strftime(stime, dim, "%Y-%m-%d %H:%M:%S", &tm_info);
		sprintf(result, "%s.%u",stime,(uint32_t)(tv.tv_usec/1000));
	} else if(print_date) {
		dim = 11;
		char stime[dim];
		strftime(stime, dim, "%Y-%m-%d", &tm_info);
		sprintf(result, "%s",stime);
	} else {
		dim = 9;
		char stime[dim];
		strftime(stime, dim, "%H:%M:%S", &tm_info);
		sprintf(result, "%s.%u",stime,(uint32_t)(tv.tv_usec/1000));
	}
	*loggerOperationState = LOGGER_OK_SUCCESS;

	if (!(buf = (char *)malloc((strlen(result) + 1)*sizeof(char)))) {
		*loggerOperationState = LOGGER_ERROR_ALLOCATION_FAILED;
		return NULL;
	}

	sprintf(buf, "%s",result);

	return buf;
}


char *get_log_general(const char * message, LoggerOperationState *loggerOperationState){
	char *string_log;

	pid_t pid = getpid();
	pthread_t thread_id =  pthread_self();

	if (logger.print_pid_and_tid) {
		string_log = FormatOutputLogger(loggerOperationState, "[pid:%i - pthread: %lu] %s", pid, thread_id, message);
	} else {
		string_log = FormatOutputLogger(loggerOperationState, "%s", message);
	}
	return string_log;
}


char *get_str_redux_log(const char *caller, const char *operation, int result_operation, LoggerOperationState *loggerOperationState){
	char *string_log;

	pid_t pid = getpid();
	pthread_t thread_id =  pthread_self();

	if (caller == NULL || strcmp(caller, "") == 0) {
		if (logger.print_pid_and_tid) {
			if (logger.meaning_result != NULL)
				string_log = FormatOutputLogger(loggerOperationState, "[pid:%i - pthread: %lu] [Operation: %s] [Result: %s]", pid, thread_id, operation, logger.meaning_result[result_operation]);
			else
				string_log = FormatOutputLogger(loggerOperationState, "[pid:%i - pthread: %lu] [Operation: %s] [Result: %i]", pid, thread_id, operation, result_operation);
		} else {
			if (logger.meaning_result != NULL)
				string_log = FormatOutputLogger(loggerOperationState, "[Operation: %s] [Result: %s]", operation, logger.meaning_result[result_operation]);
			else
				string_log = FormatOutputLogger(loggerOperationState, "[Operation: %s] [Result: %i]", operation, result_operation);
		}
	} else {
		if (logger.print_pid_and_tid) {
			if (logger.meaning_result != NULL)
				string_log = FormatOutputLogger(loggerOperationState, "[%s - pid:%i - pthread: %lu] [Operation: %s] [Result: %s]", caller, pid, thread_id, operation, logger.meaning_result[result_operation]);
			else
				string_log = FormatOutputLogger(loggerOperationState, "[%s - pid:%i - pthread: %lu] [Operation: %s] [Result: %i]", caller, pid, thread_id, operation, result_operation);
		} else {
			if (logger.meaning_result != NULL)
				string_log = FormatOutputLogger(loggerOperationState, "[%s] [Operation: %s] [Result: %s]", caller, operation, logger.meaning_result[result_operation]);
			else
				string_log = FormatOutputLogger(loggerOperationState, "[%s] [Operation: %s] [Result: %i]", caller, operation, result_operation);
		}
	}
	return string_log;
}


char *get_log_str(const char *caller, const char *operation, int user_cat, uint32_t user_cid, int result_operation, LoggerOperationState *loggerOperationState){
	char *string_log;

	pid_t pid = getpid();
	pthread_t thread_id =  pthread_self();

	if (caller == NULL || strcmp(caller, "") == 0) {
		if (logger.print_pid_and_tid) {
			if (logger.meaning_result != NULL)
				string_log = FormatOutputLogger(loggerOperationState, "[pid:%i - pthread: %lu] [Operation: %s] [CAT:%i CID:%u] [Result: %s]", pid, thread_id, operation, user_cat, user_cid, logger.meaning_result[result_operation]);
			else
				string_log = FormatOutputLogger(loggerOperationState, "[pid:%i - pthread: %lu] [Operation: %s] [CAT:%i CID:%u] [Result: %i]", pid, thread_id, operation, user_cat, user_cid, result_operation);
		} else {
			if (logger.meaning_result != NULL)
				string_log = FormatOutputLogger(loggerOperationState, "[Operation: %s] [CAT:%i CID:%u] [Result: %s]", operation, user_cat, user_cid, logger.meaning_result[result_operation]);
			else
				string_log = FormatOutputLogger(loggerOperationState, "[Operation: %s] [CAT:%i CID:%u] [Result: %i]", operation, user_cat, user_cid, result_operation);
		}
	} else {
		if (logger.print_pid_and_tid) {
			if (logger.meaning_result != NULL)
				string_log = FormatOutputLogger(loggerOperationState, "[%s - pid:%i - pthread: %lu] [Operation: %s] [CAT:%i CID:%u] [Result: %s]", caller, pid, thread_id, operation, user_cat, user_cid, logger.meaning_result[result_operation]);
			else
				string_log = FormatOutputLogger(loggerOperationState, "[%s - pid:%i - pthread: %lu] [Operation: %s] [CAT:%i CID:%u] [Result: %i]", caller, pid, thread_id, operation, user_cat, user_cid, result_operation);
		} else {
			if (logger.meaning_result != NULL)
				string_log = FormatOutputLogger(loggerOperationState, "[%s] [Operation: %s] [CAT:%i CID:%u] [Result: %s]", caller, operation, user_cat, user_cid, logger.meaning_result[result_operation]);
			else
				string_log = FormatOutputLogger(loggerOperationState, "[%s] [Operation: %s] [CAT:%i CID:%u] [Result: %i]", caller, operation, user_cat, user_cid, result_operation);
		}
	}
	return string_log;
}


char *convert_level(int level){
	char *char_level;
	LoggerOperationState loggerOperationState;
	switch (level) {
	case LOGGER_FATAL:
		char_level = FormatOutputLogger(&loggerOperationState, "%s", "FATAL");
		break;
	case LOGGER_ERROR:
		char_level = FormatOutputLogger(&loggerOperationState, "%s", "ERROR");
		break;
	case LOGGER_WARN:
		char_level = FormatOutputLogger(&loggerOperationState, "%s", "WARN");
		break;
	case LOGGER_INFO:
		char_level = FormatOutputLogger(&loggerOperationState, "%s", "INFO");
		break;
	case LOGGER_DEBUG:
		char_level = FormatOutputLogger(&loggerOperationState, "%s", "DEBUG");
		break;
	case LOGGER_TRACE:
		char_level = FormatOutputLogger(&loggerOperationState, "%s", "TRACE");
		break;
	default:
		char_level = FormatOutputLogger(&loggerOperationState, "%s", "INFO");
		break;
	}
	return char_level;
}
