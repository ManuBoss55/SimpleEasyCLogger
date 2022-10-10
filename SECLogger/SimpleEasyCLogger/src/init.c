/*
 ============================================================================
 Name        : init.c
 Author      : Emanuele Cennamo
 Version     :
 Copyright   : apache 2.0 license
 Description : Logger library
 ============================================================================
 */

#include "init.h"
#include "queue.h"


Logger logger;
char *logger_folder = "./logger";

typedef struct {
	LogQueue *queue;
} Struttura_thread_logger;


static bool EndsWith(const char *str, const char *suffix){
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}


void *thread_logger_function(void *args) {
	Struttura_thread_logger *struttura_logger = (Struttura_thread_logger*) args;
	LoggerOperationState loggerOperationState = LOGGER_OK_SUCCESS;
	lNode node = PullAndWaitQueueLogger(struttura_logger->queue, &loggerOperationState);
	if(loggerOperationState != LOGGER_OK_SUCCESS && logger.start_routine_if_audit_failed) {
		char result[29+strlen(logger_enum_type_names[loggerOperationState])+1+1];
		sprintf(result, "Errore nel Sistema di Audit (%s)", logger_enum_type_names[loggerOperationState]);
		fprintf(stderr, "%s\n", result);
		logger.__start_routine_error_audit(logger.__arg_routin_error_audit);
	}
	while(node != NULL) {
		logger_general(node->level, node->file, node->function, node->line, node->time_log, node->message);
		DestroyQueueLoggerNode(node);
		node = PullAndWaitQueueLogger(struttura_logger->queue, &loggerOperationState);
		if(loggerOperationState != LOGGER_OK_SUCCESS && logger.start_routine_if_audit_failed) {
			char result[29+strlen(logger_enum_type_names[loggerOperationState])+1+1];
			sprintf(result, "Errore nel Sistema di Audit (%s)", logger_enum_type_names[loggerOperationState]);
			fprintf(stderr, "%s\n", result);
			logger.__start_routine_error_audit(logger.__arg_routin_error_audit);
		}
	}
	free(struttura_logger);
	pthread_exit(NULL);
}


void logger_init(bool print_with_color, LoggerLevel level_file, LoggerLevel level_console, char *file, const char **meaning_result, bool print_pid_and_tid, bool start_routine_if_audit_failed, void *(*__start_routine_error_audit) (void *), void *__restrict __arg_routin_error_audit, LoggerOperationState *loggerOperationState) {
	logger.init_logger = false;
	logger.meaning_result = meaning_result;
	logger.print_with_color = print_with_color;
	logger.print_pid_and_tid = print_pid_and_tid;

	if (file != NULL) {
		logger.file_log = fopen(file, "a");
		if (logger.file_log == NULL) {
			*loggerOperationState = LOGGER_ERROR_OPEN_FILE_FAILED;
			return;
		}
		if (logger.max_file_day > 0)
			checkAndDeleteNumberFile(loggerOperationState);
	} else {
		logger.file_log = NULL;
	}

	if (level_file < LOGGER_FATAL || level_file > LOGGER_TRACE || level_console < LOGGER_FATAL || level_console > LOGGER_TRACE) {
		*loggerOperationState = LOGGER_ERROR_SET_LEVEL_FAILED;
		return;
	}
	logger.level_log_file = level_file;
	logger.level_log_console = level_console;
	logger.init_logger = true;
	logger.start_routine_if_audit_failed = start_routine_if_audit_failed;
	logger.__start_routine_error_audit = __start_routine_error_audit;
	logger.__arg_routin_error_audit = __arg_routin_error_audit;

	logger.queue = InitQueueLogger(loggerOperationState);

	Struttura_thread_logger *struttura_logger = (Struttura_thread_logger*) malloc(sizeof(Struttura_thread_logger));
	struttura_logger->queue = logger.queue;
	pthread_create(&(logger.thread_id), NULL, thread_logger_function, (void*) struttura_logger);

	*loggerOperationState = LOGGER_OK_SUCCESS;
}


void logger_init_file_day(bool print_with_color, LoggerLevel level_file, LoggerLevel level_console, const char **meaning_result, bool print_pid_and_tid, int numero_giorni_da_salvare, int megabyte_massimi_da_salvare, int dimensione_in_megabyte_per_file, bool start_routine_if_audit_failed, void *(*__start_routine_error_audit) (void *), void *__restrict __arg_routin_error_audit, LoggerOperationState *loggerOperationState) {
	logger.init_logger = false;

	struct stat st = {0};
	if (stat(logger_folder, &st) == -1) {
	    mkdir(logger_folder, 0700);
	}

	if(megabyte_massimi_da_salvare <= dimensione_in_megabyte_per_file) {
		*loggerOperationState = LOGGER_ERROR_SETTING_MAX_MB_AND_MB_FOR_FILE;
		return;
	}

	logger.max_file_day = numero_giorni_da_salvare;
	logger.max_mb_to_save = megabyte_massimi_da_salvare;
	logger.max_mb_for_file = dimensione_in_megabyte_per_file;


	char* file_name = create_folder_date_and_file_name(loggerOperationState);
	logger_init(print_with_color, level_file, level_console, file_name, meaning_result, print_pid_and_tid, start_routine_if_audit_failed, __start_routine_error_audit, __arg_routin_error_audit, loggerOperationState);
	free(file_name);
}


void logger_destroy(LoggerOperationState *loggerOperationState) {
	StopWaitLogger(logger.queue, loggerOperationState);
	if(fclose(logger.file_log) == 0)
		*loggerOperationState = LOGGER_OK_SUCCESS;
	else
		*loggerOperationState = LOGGER_ERROR_CLOSE_FILE_FAILED;
	pthread_join(logger.thread_id, NULL);
	DestroyQueueLogger(logger.queue, loggerOperationState);
}


long int findSize(char *file_name) {
    FILE* fp = fopen(file_name, "r");
    if (fp == NULL) {
        return -1;
    }

    fseek(fp, 0L, SEEK_END);
    long int res = ftell(fp);

    fclose(fp);
    return res;
}


void set_level(LoggerLevel level_file, LoggerLevel level_console, LoggerOperationState *loggerOperationState) {
	if (level_file < LOGGER_FATAL || level_file > LOGGER_TRACE || level_console < LOGGER_FATAL || level_console > LOGGER_TRACE) {
		*loggerOperationState = LOGGER_ERROR_SET_LEVEL_FAILED;
		return;
	}
	logger.level_log_file = level_file;
	logger.level_log_console = level_console;
	*loggerOperationState = LOGGER_OK_SUCCESS;
}


void set_level_file(LoggerLevel level_file, LoggerOperationState *loggerOperationState) {
	if (level_file < LOGGER_FATAL || level_file > LOGGER_TRACE) {
		*loggerOperationState = LOGGER_ERROR_SET_LEVEL_FAILED;
		return;
	}
	logger.level_log_file = level_file;
	*loggerOperationState = LOGGER_OK_SUCCESS;
}


void set_level_console(LoggerLevel level_console, LoggerOperationState *loggerOperationState) {
	if (level_console < LOGGER_FATAL || level_console > LOGGER_TRACE) {
		*loggerOperationState = LOGGER_ERROR_SET_LEVEL_FAILED;
		return;
	}
	logger.level_log_console = level_console;
	*loggerOperationState = LOGGER_OK_SUCCESS;
}




/* ============================================================================================================== */




char *create_folder_date_and_file_name(LoggerOperationState *loggerOperationState) {
	char *len = "logger-";

	char *date = logger_get_time(true, false, loggerOperationState);
	char path_date[strlen(logger_folder)+1+strlen(len)+strlen(date)+1];
	sprintf(path_date,"%s/%s%s",logger_folder,len,date);
	struct stat st = {0};
	if (stat(path_date, &st) == -1) {
	    mkdir(path_date, 0700);
	}

	int old_file_log = 1;
	DIR *dir_day_log = opendir(path_date);
	if (dir_day_log) {
		struct dirent *dir;
		while ((dir = readdir(dir_day_log)) != NULL) {
			if(strncmp(dir->d_name, len, strlen(len)) == 0) {
				char temp[strlen(dir->d_name)];
				strcpy(temp, dir->d_name);

				char *last = strrchr(temp, '-');
				int number = atoi(strtok(last+2,"."));
				if(number >= old_file_log) {
					old_file_log = number;
				}
			}
		}

		char *file_name = FormatOutputLogger(loggerOperationState, "%s/%s%s-p%d.log", path_date, len, date, old_file_log);
		if(findSize(file_name) > logger.max_mb_for_file*1000000) {
			old_file_log += 1;
		}
		free(file_name);
	}
	closedir(dir_day_log);

	logger.old_file_log = old_file_log;
	char *file_name = FormatOutputLogger(loggerOperationState, "%s/%s%s-p%d.log", path_date, len, date, old_file_log);

	strcpy(logger.today,date);
	free(date);

	return file_name;
}


static int get_num_total_file_log() {
	char *len = "logger-";
	int number_log_in_folder = 0;
	DIR *dirLogger = opendir(logger_folder);
		if (dirLogger) {
		struct dirent *dirDayLogger;
		while ((dirDayLogger = readdir(dirLogger)) != NULL) {
			char direct_path[strlen(logger_folder)+1+strlen(dirDayLogger->d_name)+1];
			sprintf(direct_path,"%s/%s",logger_folder,dirDayLogger->d_name);
			DIR* isDirectory = opendir(direct_path);
			if (isDirectory) {
				struct dirent *indir;
				while ((indir = readdir(isDirectory)) != NULL) {
					if(strncmp(indir->d_name, len, strlen(len)) == 0 && EndsWith(indir->d_name,".log")) {
						number_log_in_folder += 1;
					}
				}
			}
			closedir(isDirectory);
		}
	}
	closedir(dirLogger);
	return number_log_in_folder;
}


static char* get_old_directory(int *number_of_logger_folder) {
	char *len = "logger-";
	char year[] = {"3000"};
	char month[] = {"12"};
	char day[] = {"31"};
	*number_of_logger_folder = 0;
	DIR *dirLogger = opendir(logger_folder);
	if (dirLogger) {
		struct dirent *dirDayLogger;
		while ((dirDayLogger = readdir(dirLogger)) != NULL) {
			char direct_path[strlen(logger_folder)+1+strlen(dirDayLogger->d_name)+1];
			sprintf(direct_path,"%s/%s",logger_folder,dirDayLogger->d_name);
			DIR* isDirectory = opendir(direct_path);
			if (isDirectory) {
				if(strncmp(dirDayLogger->d_name, len, strlen(len)) == 0) {
					*number_of_logger_folder += 1;

					char temp[strlen(dirDayLogger->d_name)];
					strcpy(temp, dirDayLogger->d_name);

					strtok(temp,"-");
					char *year_curr = strtok(NULL,"-");
					char *month_curr = strtok(NULL,"-");
					char *day_curr = strtok(NULL,"-");

					if(atoi(year_curr) <= atoi(year)) {
						strcpy(year,year_curr);
						if(atoi(month_curr) <= atoi(month)) {
							strcpy(month,month_curr);
							if(atoi(day_curr) <= atoi(day)) {
								strcpy(day,day_curr);
							}
						}
					}
				}
				closedir(isDirectory);
			}
		}
	}
	closedir(dirLogger);

	char *folder_name = (char*) malloc((strlen(len)+strlen(year)+1+strlen(month)+1+strlen(day)+1)*sizeof(char));
	sprintf(folder_name,"%s%s-%s-%s",len,year,month,day);
	return folder_name;
}


static void delete_old_folder(int *number_of_logger_folder, char *folder_name, LoggerOperationState *loggerOperationState) {
	if(logger.max_file_day < *number_of_logger_folder) {
		char path_name[strlen(logger_folder)+1+strlen(folder_name)+1];
		sprintf(path_name,"%s/%s",logger_folder,folder_name);
		DIR *dir_day_log = opendir(path_name);
		if (dir_day_log) {
			struct dirent *dir;
			while ((dir = readdir(dir_day_log)) != NULL) {
				char file_name[strlen(path_name)+1+strlen(dir->d_name)+1];
				sprintf(file_name,"%s/%s",path_name,dir->d_name);
				remove(file_name);
			}
		}
		closedir(dir_day_log);
		if(remove(path_name) != 0) {
			*loggerOperationState = LOGGER_ERROR_DELETE_OLD_FILE;
			return;
		}
		*number_of_logger_folder -= 1;
	}
}


static void delete_old_file(int number_log_in_folder, char *folder_name, float *file_da_eliminare) {
	char *len = "logger-";
	int diff_mb = (number_log_in_folder*logger.max_mb_for_file)-logger.max_mb_to_save;
	*file_da_eliminare = ceil(diff_mb/logger.max_mb_for_file);

	char path_name[strlen(logger_folder)+1+strlen(folder_name)+1];
	sprintf(path_name,"%s/%s",logger_folder,folder_name);
	int old_file_log = 100000;
	int number_log_in_day_folder = 0;
	DIR *dir_day_log = opendir(path_name);
	if (dir_day_log) {
		struct dirent *dir;
		while ((dir = readdir(dir_day_log)) != NULL) {
			if(strncmp(dir->d_name, len, strlen(len)) == 0 && EndsWith(dir->d_name,".log")) {
				char temp[strlen(dir->d_name)];
				strcpy(temp, dir->d_name);

				char *last = strrchr(temp, '-');
				int number = atoi(strtok(last+2,"."));

				number_log_in_day_folder += 1;
				if(number <= old_file_log) {
					old_file_log = number;
				}
			}
		}
	}
	closedir(dir_day_log);
	int ciclo_rimozione = 0;
	if(*file_da_eliminare>number_log_in_day_folder){
		ciclo_rimozione=number_log_in_day_folder;
	} else {
		ciclo_rimozione=*file_da_eliminare;
	}
	for (int var = 0; var < ciclo_rimozione; var++) {
		char file_name[strlen(path_name)+1+strlen(folder_name)+7+1];
		sprintf(file_name,"%s/%s-p%d.log",path_name,folder_name,var+old_file_log);
		remove(file_name);
		*file_da_eliminare -= 1;
	}
	if(ciclo_rimozione == number_log_in_day_folder) {
		remove(path_name);
	}
}


void checkAndDeleteNumberFile(LoggerOperationState *loggerOperationState) {
	int number_of_logger_folder = logger.max_file_day+1;

	int number_log_in_folder = get_num_total_file_log();
	int diff_mb = (number_log_in_folder*logger.max_mb_for_file)-logger.max_mb_to_save;
	float file_da_eliminare = ceil(diff_mb/logger.max_mb_for_file);

	int security_control = 0;
	while(logger.max_file_day < number_of_logger_folder || file_da_eliminare > 0) {
		char* folder_name = get_old_directory(&number_of_logger_folder);

		int number_log_in_folder = get_num_total_file_log();
		delete_old_folder(&number_of_logger_folder, folder_name, loggerOperationState);

		number_log_in_folder = get_num_total_file_log();
		delete_old_file(number_log_in_folder, folder_name, &file_da_eliminare);
		free(folder_name);

		security_control += 1;
		if (security_control >= 1000) {
			perror("Error in delete file in logger (check hyperparameters and name file/folder in logger folder ");
			return;
		}
    }
}

void verify_log_day(LoggerOperationState *loggerOperationState) {
	char *len = "logger-";
	char *date = logger_get_time(true, false, loggerOperationState);
	char path_date[strlen(logger_folder)+1+strlen(len)+strlen(date)+1];
	sprintf(path_date,"%s/%s%s",logger_folder,len,date);
	char *old_file_name = FormatOutputLogger(loggerOperationState, "%s/%s%s-p%d.log", path_date, len, date, logger.old_file_log);

	if(strcmp(logger.today,date) != 0) {
		fclose(logger.file_log);

		strcpy(logger.today,date);
		char *file_name = create_folder_date_and_file_name(loggerOperationState);

		logger.file_log = fopen(file_name, "a");
		if (logger.file_log == NULL) {
			*loggerOperationState = LOGGER_ERROR_OPEN_FILE_FAILED;
			return;
		}
		if (logger.max_file_day > 0)
			checkAndDeleteNumberFile(loggerOperationState);

		free(file_name);
	} else if(findSize(old_file_name) > logger.max_mb_for_file*1000000) {
		fclose(logger.file_log);
		logger.old_file_log += 1;
		char *file_name = FormatOutputLogger(loggerOperationState, "%s/%s%s-p%d.log", path_date, len, date, logger.old_file_log);
		logger.file_log = fopen(file_name, "a");
		if (logger.file_log == NULL) {
			*loggerOperationState = LOGGER_ERROR_OPEN_FILE_FAILED;
			return;
		}
		if (logger.max_file_day > 0)
			checkAndDeleteNumberFile(loggerOperationState);

		free(file_name);
	}

	free(date);
	free(old_file_name);
}