/*
 ============================================================================
 Name        : logger.h
 Author      : Emanuele Cennamo
 Version     :
 Copyright   : apache 2.0 license
 Description : Logger library
 ============================================================================
 */


#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdbool.h>
#include <stdint.h>

typedef enum {
	LOGGER_FATAL = 1,
	LOGGER_ERROR = 2,
	LOGGER_WARN = 3,
	LOGGER_INFO = 4,
	LOGGER_DEBUG = 5,
	LOGGER_TRACE = 6
} LoggerLevel;

typedef enum {
	LOGGER_OK_SUCCESS,

	LOGGER_ERROR_GENERAL,
	LOGGER_ERROR_POINTER_FILE_NULL,
	LOGGER_ERROR_SET_LEVEL_FAILED,
	LOGGER_ERROR_CLOSE_FILE_FAILED,
	LOGGER_ERROR_ALLOCATION_FAILED,
	LOGGER_ERROR_NOT_SELECT_DATE_OR_TIME,
	LOGGER_ERROR_OPEN_FILE_FAILED,
	LOGGER_ERROR_CONCATENATE_FAILED,
	LOGGER_ERROR_SAFE_INIT_FAILED,
	LOGGER_ERROR_DELETE_OLD_FILE,
	LOGGER_ERROR_MUTEX_INITIALIZATION_FAILED,
	LOGGER_ERROR_EMPTY_QUEUE,
	LOGGER_ERROR_QUEUE_NULL_POINTER,
	LOGGER_ERROR_ACQUISITION_LOCK_FAILED,
	LOGGER_ERROR_SETTING_MAX_MB_AND_MB_FOR_FILE

} LoggerOperationState;

extern const char * logger_enum_type_names[];

#define logger_fatal(format, ...) logger_fatal(__FILE__, __func__, __LINE__, format, __VA_ARGS__)
#define logger_fatal_str(caller, user_cat, user_cid, result_operation, operation) logger_fatal_str(__FILE__, __func__, __LINE__, caller, user_cat, user_cid, result_operation, operation)
#define logger_fatal_str_dyn(caller, user_cat, user_cid, result_operation, operation_format, ...) logger_fatal_str_dyn(__FILE__, __func__, __LINE__, caller, user_cat, user_cid, result_operation, operation_format, __VA_ARGS__)
#define logger_fatal_str_redux(caller, result_operation, operation) logger_fatal_str_redux(__FILE__, __func__, __LINE__, caller, result_operation, operation)
#define logger_fatal_str_redux_dyn(caller, result_operation, operation_format, ...) logger_fatal_str_redux_dyn(__FILE__, __func__, __LINE__, caller, result_operation, operation_format, __VA_ARGS__)

#define logger_error(format, ...) logger_error(__FILE__, __func__, __LINE__, format, __VA_ARGS__)
#define logger_error_str(caller, user_cat, user_cid, result_operation, operation) logger_error_str(__FILE__, __func__, __LINE__, caller, user_cat, user_cid, result_operation, operation)
#define logger_error_str_dyn(caller, user_cat, user_cid, result_operation, operation_format, ...) logger_error_str_dyn(__FILE__, __func__, __LINE__, caller, user_cat, user_cid, result_operation, operation_format, __VA_ARGS__)
#define logger_error_str_redux(caller, result_operation, operation) logger_error_str_redux(__FILE__, __func__, __LINE__, caller, result_operation, operation)
#define logger_error_str_redux_dyn(caller, result_operation, operation_format, ...) logger_error_str_redux_dyn(__FILE__, __func__, __LINE__, caller, result_operation, operation_format, __VA_ARGS__)

#define logger_warn(format, ...) logger_warn(__FILE__, __func__, __LINE__, format, __VA_ARGS__)
#define logger_warn_str(caller, user_cat, user_cid, result_operation, operation) logger_warn_str(__FILE__, __func__, __LINE__, caller, user_cat, user_cid, result_operation, operation)
#define logger_warn_str_dyn(caller, user_cat, user_cid, result_operation, operation_format, ...) logger_warn_str_dyn(__FILE__, __func__, __LINE__, caller, user_cat, user_cid, result_operation, operation_format, __VA_ARGS__)
#define logger_warn_str_redux(caller, result_operation, operation) logger_warn_str_redux(__FILE__, __func__, __LINE__, caller, result_operation, operation)
#define logger_warn_str_redux_dyn(caller, result_operation, operation_format, ...) logger_warn_str_redux_dyn(__FILE__, __func__, __LINE__, caller, result_operation, operation_format, __VA_ARGS__)

#define logger_info(format, ...) logger_info(__FILE__, __func__, __LINE__, format, __VA_ARGS__)
#define logger_info_str(caller, user_cat, user_cid, result_operation, operation) logger_info_str(__FILE__, __func__, __LINE__, caller, user_cat, user_cid, result_operation, operation)
#define logger_info_str_dyn(caller, user_cat, user_cid, result_operation, operation_format, ...) logger_info_str_dyn(__FILE__, __func__, __LINE__, caller, user_cat, user_cid, result_operation, operation_format, __VA_ARGS__)
#define logger_info_str_redux(caller, result_operation, operation) logger_info_str_redux(__FILE__, __func__, __LINE__, caller, result_operation, operation)
#define logger_info_str_redux_dyn(caller, result_operation, operation_format, ...) logger_info_str_redux_dyn(__FILE__, __func__, __LINE__, caller, result_operation, operation_format, __VA_ARGS__)

#define logger_debug(format, ...) logger_debug(__FILE__, __func__, __LINE__, format, __VA_ARGS__)
#define logger_debug_str(caller, user_cat, user_cid, result_operation, operation) logger_debug_str(__FILE__, __func__, __LINE__, caller, user_cat, user_cid, result_operation, operation)
#define logger_debug_str_dyn(caller, user_cat, user_cid, result_operation, operation_format, ...) logger_debug_str_dyn(__FILE__, __func__, __LINE__, caller, user_cat, user_cid, result_operation, operation_format, __VA_ARGS__)
#define logger_debug_str_redux(caller, result_operation, operation) logger_debug_str_redux(__FILE__, __func__, __LINE__, caller, result_operation, operation)
#define logger_debug_str_redux_dyn(caller, result_operation, operation_format, ...) logger_debug_str_redux_dyn(__FILE__, __func__, __LINE__, caller, result_operation, operation_format, __VA_ARGS__)

#define logger_trace(format, ...) logger_trace(__FILE__, __func__, __LINE__, format, __VA_ARGS__)
#define logger_trace_str(caller, user_cat, user_cid, result_operation, operation) logger_trace_str(__FILE__, __func__, __LINE__, caller, user_cat, user_cid, result_operation, operation)
#define logger_trace_str_dyn(caller, user_cat, user_cid, result_operation, operation_format, ...) logger_trace_str_dyn(__FILE__, __func__, __LINE__, caller, user_cat, user_cid, result_operation, operation_format, __VA_ARGS__)
#define logger_trace_str_redux(caller, result_operation, operation) logger_trace_str_redux(__FILE__, __func__, __LINE__, caller, result_operation, operation)
#define logger_trace_str_redux_dyn(caller, result_operation, operation_format, ...) logger_trace_str_redux_dyn(__FILE__, __func__, __LINE__, caller, result_operation, operation_format, __VA_ARGS__)


/**
* @brief Inizializzazione Logger Con File giornaliero automatico
*
* Inizializzazione del Logger con creazione file giorno per giorno automaticamente
*
* @param [in] bool print_with_color: se true stampa con i colori
* @param [in] LoggerLevel level_file: imposta il livello di Log da salvare su file
* @param [in] LoggerLevel level_console: imposta il livello di Log da stampare sulla console
* @param [in] char *meaning_result: struttura con i significati dei valori int dei risultati, se NULL stampa i risultati numerici, altrimenti i significati
* @param [in] bool print_pid_and_tid: se true stampa l'id del processo e l'id del thread
* @param [in] int numero_giorni_da_salvare: imposta il numero di giorni massimo da salvare
* @param [in] int megabyte_massimi_da_salvare: imposta i MB complessivi massimi da salvare
* @param [in] int dimensione_in_megabyte_per_file: imposta i MB massimi per file (deve essere inferiore dei massimi)
* @param [in] bool start_routine_if_audit_failed: se true arresta il programma in caso di fallimeto
* @param [in] void *(*__start_routine_error_audit) (void *): callback che si avvia se il programma di audit fallisce
* @param [in] void *__restrict __arg_routin_error_audit: argomenti della routine __start_routine_error_audit, se non ci sono argomenti, mettere NULL
* @param [out] LoggerOperationState *loggerOperationState: risultato dell'operazione*
* @retval void
*/
void logger_init_file_day(bool print_with_color, LoggerLevel level_file, LoggerLevel level_console, const char **meaning_result, bool print_pid_and_tid, int numero_giorni_da_salvare, int megabyte_massimi_da_salvare, int dimensione_in_megabyte_per_file, bool start_routine_if_audit_failed, void *(*__start_routine_error_audit) (void *), void *__restrict __arg_routin_error_audit, LoggerOperationState *loggerOperationState);

/**
* @brief Distruzione Logger
*
* Distruzione e chiusura sicura del Logger
*
* @param [out] LoggerOperationState *loggerOperationState: risultato dell'operazione*
* @retval void
*/
void logger_destroy(LoggerOperationState*);

/**
* @brief Seleziona Livello di Log
*
* Seleziona il Livello del Log da stampare
*
* @param [in] LoggerLevel level_file: imposta il livello di Log da salvare su file
* @param [in] LoggerLevel level_console: imposta il livello di Log da stampare sulla console
* @param [out] LoggerOperationState *loggerOperationState: risultato dell'operazione*
* @retval void
*/
void set_level(LoggerLevel level_file, LoggerLevel level_console, LoggerOperationState *loggerOperationState);

/**
* @brief Costruttore Stringa da Stampare Redux
*
* Costruisce la stringa da stampare Redux
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation: Nome operazione effettuata
* @param [in] int result_operation: Risultato operazione effettuata
* @param [out] LoggerOperationState *loggerOperationState: risultato dell'operazione*
* @retval Stringa da Stampare
*/
char *get_str_redux_log(const char *caller, const char *operation, int result_operation, LoggerOperationState* loggerOperationState);

/**
* @brief Costruttore Stringa da Stampare
*
* Costruisce la stringa da stampare
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation: Nome operazione effettuata
* @param [in] int user_cat: Categoria Utente
* @param [in] uint32_t user_cid: Identificativo Utente
* @param [in] int result_operation: Risultato operazione effettuata
* @param [out] LoggerOperationState *loggerOperationState: risultato dell'operazione*
* @retval Stringa da Stampare
*/
char *get_log_str(const char *caller, const char *operation, int user_cat, uint32_t user_cid, int result_operation, LoggerOperationState* loggerOperationState);

/**
* @brief Costruttore Stringa da Stampare
*
* Costruisce la stringa da stampare
*
* @param [in] char *message: Messaggio
* @param [out] LoggerOperationState *loggerOperationState: risultato dell'operazione*
* @retval Stringa da Stampare
*/
char *get_log_general(const char * message, LoggerOperationState* loggerOperationState);

/**
* @brief Logger Fatal
*
* Stampa un log con categoria FATAL
*
* @param [in] char *fmt: formato stringa
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_fatal)(const char *file, const char *function, int line, const char *fmt, ...);

/**
* @brief Logger Fatal
*
* Stampa un log con categoria FATAL
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation: Nome operazione effettuata
* @param [in] int user_cat: Categoria Utente
* @param [in] uint32_t user_cid: Identificativo Utente
* @param [in] int result_operation: Risultato operazione effettuata
* @retval LoggerOperationState
*/
LoggerOperationState (logger_fatal_str)(const char *file, const char *function, int line, const char *caller, int user_cat, uint32_t user_cid, int result_operation, const char *operation);

/**
* @brief Logger Fatal Dyn
*
* Stampa un log con categoria FATAL con operation dinamica
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation_format: Nome operazione effettuata in modalità format
* @param [in] int user_cat: Categoria Utente
* @param [in] uint32_t user_cid: Identificativo Utente
* @param [in] int result_operation: Risultato operazione effettuata
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_fatal_str_dyn)(const char *file, const char *function, int line, const char *caller, int user_cat, uint32_t user_cid, int result_operation, const char *operation_format, ...);

/**
* @brief Logger Fatal Redux
*
* Stampa un log con categoria FATAL Redux
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation: Nome operazione effettuata
* @param [in] int result_operation: Risultato operazione effettuata
* @retval LoggerOperationState
*/
LoggerOperationState (logger_fatal_str_redux)(const char *file, const char *function, int line, const char *caller, int result_operation, const char *operation);

/**
* @brief Logger Fatal Redux Dyn
*
* Stampa un log con categoria FATAL Redux con operation dinamica
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation_format: Nome operazione effettuata in modalità format
* @param [in] int result_operation: Risultato operazione effettuata
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_fatal_str_redux_dyn)(const char *file, const char *function, int line, const char *caller, int result_operation, const char *operation_format, ...);

/**
* @brief Logger Error
*
* Stampa un log con categoria ERROR
*
* @param [in] char *fmt: formato stringa
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_error)(const char *file, const char *function, int line, const char *fmt, ...);

/**
* @brief Logger Error
*
* Stampa un log con categoria ERROR
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation: Nome operazione effettuata
* @param [in] int user_cat: Categoria Utente
* @param [in] uint32_t user_cid: Identificativo Utente
* @param [in] int result_operation: Risultato operazione effettuata
* @retval LoggerOperationState
*/
LoggerOperationState (logger_error_str)(const char *file, const char *function, int line, const char *caller, int user_cat, uint32_t user_cid, int result_operation, const char *operation);

/**
* @brief Logger Error Dyn
*
* Stampa un log con categoria ERROR con operation dinamica
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation_format: Nome operazione effettuata in modalità format
* @param [in] int user_cat: Categoria Utente
* @param [in] uint32_t user_cid: Identificativo Utente
* @param [in] int result_operation: Risultato operazione effettuata
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_error_str_dyn)(const char *file, const char *function, int line, const char *caller, int user_cat, uint32_t user_cid, int result_operation, const char *operation_format, ...);

/**
* @brief Logger Error Redux
*
* Stampa un log con categoria ERROR Redux
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation: Nome operazione effettuata
* @param [in] int result_operation: Risultato operazione effettuata
* @retval LoggerOperationState
*/
LoggerOperationState (logger_error_str_redux)(const char *file, const char *function, int line, const char *caller, int result_operation, const char *operation);

/**
* @brief Logger Error Redux Dyn
*
* Stampa un log con categoria Error Redux con operation dinamica
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation_format: Nome operazione effettuata in modalità format
* @param [in] int result_operation: Risultato operazione effettuata
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_error_str_redux_dyn)(const char *file, const char *function, int line, const char *caller, int result_operation, const char *operation_format, ...);

/**
* @brief Logger Warn
*
* Stampa un log con categoria WARN
*
* @param [in] char *fmt: formato stringa
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_warn)(const char *file, const char *function, int line, const char *fmt, ...);

/**
* @brief Logger Warn
*
* Stampa un log con categoria WARN
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation: Nome operazione effettuata
* @param [in] int user_cat: Categoria Utente
* @param [in] uint32_t user_cid: Identificativo Utente
* @param [in] int result_operation: Risultato operazione effettuata
* @retval LoggerOperationState
*/
LoggerOperationState (logger_warn_str)(const char *file, const char *function, int line, const char *caller, int user_cat, uint32_t user_cid, int result_operation, const char *operation);

/**
* @brief Logger Warn Dyn
*
* Stampa un log con categoria WARN con operation dinamica
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation_format: Nome operazione effettuata in modalità format
* @param [in] int user_cat: Categoria Utente
* @param [in] uint32_t user_cid: Identificativo Utente
* @param [in] int result_operation: Risultato operazione effettuata
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_warn_str_dyn)(const char *file, const char *function, int line, const char *caller, int user_cat, uint32_t user_cid, int result_operation, const char *operation_format, ...);

/**
* @brief Logger Warn Redux
*
* Stampa un log con categoria WARN Redux
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation: Nome operazione effettuata
* @param [in] int result_operation: Risultato operazione effettuata
* @retval LoggerOperationState
*/
LoggerOperationState (logger_warn_str_redux)(const char *file, const char *function, int line, const char *caller, int result_operation, const char *operation);

/**
* @brief Logger Warn Redux Dyn
*
* Stampa un log con categoria WARN Redux con operation dinamica
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation_format: Nome operazione effettuata in modalità format
* @param [in] int result_operation: Risultato operazione effettuata
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_warn_str_redux_dyn)(const char *file, const char *function, int line, const char *caller, int result_operation, const char *operation_format, ...);

/**
* @brief Logger Info
*
* Stampa un log con categoria INFO
*
* @param [in] char *fmt: formato stringa
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_info)(const char *file, const char *function, int line, const char *fmt, ...);

/**
* @brief Logger Info
*
* Stampa un log con categoria INFO
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation: Nome operazione effettuata
* @param [in] int user_cat: Categoria Utente
* @param [in] uint32_t user_cid: Identificativo Utente
* @param [in] int result_operation: Risultato operazione effettuata
* @retval LoggerOperationState
*/
LoggerOperationState (logger_info_str)(const char *file, const char *function, int line, const char *caller, int user_cat, uint32_t user_cid, int result_operation, const char *operation);

/**
* @brief Logger Info Dyn
*
* Stampa un log con categoria INFO con operation dinamica
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation_format: Nome operazione effettuata in modalità format
* @param [in] int user_cat: Categoria Utente
* @param [in] uint32_t user_cid: Identificativo Utente
* @param [in] int result_operation: Risultato operazione effettuata
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_info_str_dyn)(const char *file, const char *function, int line, const char *caller, int user_cat, uint32_t user_cid, int result_operation, const char *operation_format, ...);

/**
* @brief Logger Info Redux
*
* Stampa un log con categoria INFO Redux
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation: Nome operazione effettuata
* @param [in] int result_operation: Risultato operazione effettuata
* @retval LoggerOperationState
*/
LoggerOperationState (logger_info_str_redux)(const char *file, const char *function, int line, const char *caller, int result_operation, const char *operation);

/**
* @brief Logger Info Redux Dyn
*
* Stampa un log con categoria INFO Redux con operation dinamica
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation_format: Nome operazione effettuata in modalità format
* @param [in] int result_operation: Risultato operazione effettuata
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_info_str_redux_dyn)(const char *file, const char *function, int line, const char *caller, int result_operation, const char *operation_format, ...);

/**
* @brief Logger Debug
*
* Stampa un log con categoria DEBUG
*
* @param [in] char *fmt: formato stringa
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_debug)(const char *file, const char *function, int line, const char *fmt, ...);

/**
* @brief Logger Debug
*
* Stampa un log con categoria DEBUG
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation: Nome operazione effettuata
* @param [in] int user_cat: Categoria Utente
* @param [in] uint32_t user_cid: Identificativo Utente
* @param [in] int result_operation: Risultato operazione effettuata
* @retval LoggerOperationState
*/
LoggerOperationState (logger_debug_str)(const char *file, const char *function, int line, const char *caller, int user_cat, uint32_t user_cid, int result_operation, const char *operation);

/**
* @brief Logger Debug Dyn
*
* Stampa un log con categoria DEBUG con operation dinamica
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation_format: Nome operazione effettuata in modalità format
* @param [in] int user_cat: Categoria Utente
* @param [in] uint32_t user_cid: Identificativo Utente
* @param [in] int result_operation: Risultato operazione effettuata
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_debug_str_dyn)(const char *file, const char *function, int line, const char *caller, int user_cat, uint32_t user_cid, int result_operation, const char *operation_format, ...);

/**
* @brief Logger Debug Redux
*
* Stampa un log con categoria DEBUG Redux
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation: Nome operazione effettuata
* @param [in] int result_operation: Risultato operazione effettuata
* @retval LoggerOperationState
*/
LoggerOperationState (logger_debug_str_redux)(const char *file, const char *function, int line, const char *caller, int result_operation, const char *operation);

/**
* @brief Logger Debug Redux Dyn
*
* Stampa un log con categoria DEBUG Redux con operation dinamica
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation_format: Nome operazione effettuata in modalità format
* @param [in] int result_operation: Risultato operazione effettuata
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_debug_str_redux_dyn)(const char *file, const char *function, int line, const char *caller, int result_operation, const char *operation_format, ...);

/**
* @brief Logger Trace
*
* Stampa un log con categoria TRACE
*
* @param [in] char *fmt: formato stringa
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_trace)(const char *file, const char *function, int line, const char *fmt, ...);

/**
* @brief Logger Trace
*
* Stampa un log con categoria TRACE
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation: Nome operazione effettuata
* @param [in] int user_cat: Categoria Utente
* @param [in] uint32_t user_cid: Identificativo Utente
* @param [in] int result_operation: Risultato operazione effettuata
* @retval LoggerOperationState
*/
LoggerOperationState (logger_trace_str)(const char *file, const char *function, int line, const char *caller, int user_cat, uint32_t user_cid, int result_operation, const char *operation);

/**
* @brief Logger Trace Dyn
*
* Stampa un log con categoria TRACE con operation dinamica
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation_format: Nome operazione effettuata in modalità format
* @param [in] int user_cat: Categoria Utente
* @param [in] uint32_t user_cid: Identificativo Utente
* @param [in] int result_operation: Risultato operazione effettuata
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_trace_str_dyn)(const char *file, const char *function, int line, const char *caller, int user_cat, uint32_t user_cid, int result_operation, const char *operation_format, ...);

/**
* @brief Logger Trace Redux
*
* Stampa un log con categoria TRACE Redux
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation: Nome operazione effettuata
* @param [in] int result_operation: Risultato operazione effettuata
* @retval LoggerOperationState
*/
LoggerOperationState (logger_trace_str_redux)(const char *file, const char *function, int line, const char *caller, int result_operation, const char *operation);

/**
* @brief Logger Fatal Trace Dyn
*
* Stampa un log con categoria TRACE Redux con operation dinamica
*
* @param [in] char *caller: Nome thread chiamante
* @param [in] char *function: Nome funzione dove è stata chiamata
* @param [in] char *operation_format: Nome operazione effettuata in modalità format
* @param [in] int result_operation: Risultato operazione effettuata
* @param [in] ...: Argomenti variabili in ingresso
* @retval LoggerOperationState
*/
LoggerOperationState (logger_trace_str_redux_dyn)(const char *file, const char *function, int line, const char *caller, int result_operation, const char *operation_format, ...);


#endif /* __LOGGER_H__ */