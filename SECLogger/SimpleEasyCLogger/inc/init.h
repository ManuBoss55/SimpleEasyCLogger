/*
 ============================================================================
 Name        : init.h
 Author      : Emanuele Cennamo
 Version     :
 Copyright   : apache 2.0 license
 Description : Logger library
 ============================================================================
 */


#ifndef __INIT_H__
#define __INIT_H__

#include "lib.h"
#include "queue.h"

typedef struct {
	bool init_logger;
	int level_log_file;
	int level_log_console;

	FILE *file_log;
	char today[12];
	int old_file_log;

	bool print_with_color;
	const char **meaning_result;
	bool print_pid_and_tid;

	long int max_file_day;
	long int max_mb_for_file;
	long int max_mb_to_save;

	bool start_routine_if_audit_failed;
	void *(*__start_routine_error_audit) (void *);
	void *__restrict __arg_routin_error_audit;

	LogQueue *queue;
	pthread_t thread_id;
} Logger;

extern Logger logger;

extern char * logger_folder;

/**
* @brief Inizializzazione Logger
*
* Inizializzazione del Logger
*
* @param [in] bool print_with_color: se true stampa con i colori
* @param [in] LoggerLevel level_file: imposta il livello di Log da salvare su file
* @param [in] LoggerLevel level_console: imposta il livello di Log da stampare sulla console
* @param [in] char *file: puntatore al nome del file
* @param [in] char *meaning_result: struttura con i significati dei valori int dei risultati, se NULL stampa i risultati numerici, altrimenti i significati
* @param [in] bool print_pid_and_tid: se true stampa l'id del processo e l'id del thread
* @param [in] int start_routine_if_audit_failed: se true arresta il programma in caso di fallimeto
* @param [in] void *(*__start_routine) (void *): callback che si avvia se il programma di aduti fallisce
* @param [out] LoggerOperationState *loggerOperationState: risultato dell'operazione*
* @retval void
*/
void logger_init(bool print_with_color, LoggerLevel level_file, LoggerLevel level_console, char *file, const char **meaning_result, bool print_pid_and_tid, bool start_routine_if_audit_failed, void *(*__start_routine_error_audit) (void *), void *__restrict __arg_routin_error_audit, LoggerOperationState *loggerOperationState);
void logger_general(int level, const char *file, const char *function, int line, char *time_log, const char *message);


void set_level_file(LoggerLevel level_file, LoggerOperationState *loggerOperationState);
void set_level_console(LoggerLevel level_console, LoggerOperationState *loggerOperationState);
void checkAndDeleteNumberFile(LoggerOperationState *loggerOperationState);
void verify_log_day(LoggerOperationState *loggerOperationState);
char *create_folder_date_and_file_name(LoggerOperationState *loggerOperationState);
long int findSize(char *file_name);


int GetSizeFormatOutputLogger(LoggerOperationState *loggerOperationState, const char *formatstring, va_list args);
char *GetStringFormatOutputLogger(LoggerOperationState *loggerOperationState, int Size, const char *formatstring, va_list args);
char *FormatOutputLogger(LoggerOperationState *loggerOperationState, const char *formatstring, ...);


char *logger_get_time(bool print_date, bool print_time, LoggerOperationState *loggerOperationState);
char *convert_level(int level);


#endif /* __INIT_H__ */
