/*
 ============================================================================
 Name    :
 Author   :
 Version   :
 Copyright  :
 Description :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "logger.h"

/*
void func1(){

	logger_debug_str_redux("Thread Main", "Avvio Sessione", 0);
	logger_debug_str_redux("Thread Main", "Avvio Sessione", 0);
	logger_debug_str_redux("Thread Main", "Avvio Sessione", 0);
}*/

typedef enum
{
    TEST_1,
    TEST_2
} prova;

static const char *fruitNames[] = {
    [TEST_1] = "TEST_1",
    [TEST_2] = "TEST_2",
};

void Test(const char **funcVar)
{
    printf("%s", funcVar[TEST_2]);
}

void CallbackError()
{
    printf("\n\nERROREEEEEEEE\n\n");
}

int main(void)
{

    LoggerOperationState loggerOperationState        = LOGGER_OK_SUCCESS;
    int                  numeroGiorniDaSalvare       = 30; // 1 MESE
    int                  megabyteMassimiDaSalvare    = 3;  // 10 GB
    int                  dimensioneInMegabytePerFile = 1;  // 50 MB

    logger_init_file_day(true, LOGGER_INFO, LOGGER_INFO, fruitNames, true, numeroGiorniDaSalvare, megabyteMassimiDaSalvare, dimensioneInMegabytePerFile, false, ( void * )CallbackError, NULL, &loggerOperationState);
    logger_info_str_redux("Thread Main", TEST_1, "Init Logger");

    printf("Stato Operazione %i\n", loggerOperationState);

    logger_fatal("TEST 1: %s", "ttt");
    logger_fatal_str_dyn("", 111, 111, TEST_1, "TEST 2: %s", "ttt");
    logger_fatal_str_redux_dyn("", TEST_2, "TEST 3: %s", "ttt");
    logger_fatal("TEST 1: %s", "ttt");

    logger_error("TEST 1: %s", "ttt");
    logger_error_str_dyn("", 111, 111, TEST_1, "TEST 2: %s", "ttt");
    logger_error_str_redux_dyn("", TEST_2, "TEST 3: %s", "ttt");

    logger_warn("TEST 1: %s", "ttt");
    logger_warn_str_dyn("", 111, 111, TEST_1, "TEST 2: %s", "ttt");
    logger_warn_str_redux_dyn("", TEST_2, "TEST 3: %s", "ttt");

    logger_info("TEST 1: %s", "ttt");
    logger_info_str_dyn("", 111, 111, TEST_1, "TEST 2: %s", "ttt");
    logger_info_str_redux_dyn("", TEST_2, "TEST 3: %s", "ttt");

    logger_debug("TEST 1: %s", "ttt");
    logger_debug_str_dyn("", 111, 111, TEST_1, "TEST 2: %s", "ttt");
    logger_debug_str_redux_dyn("", TEST_2, "TEST 3: %s", "ttt");

    logger_trace("TEST 1: %s", "ttt");
    logger_trace_str_dyn("", 111, 111, TEST_1, "TEST 2: %s", "ttt");
    logger_trace_str_redux_dyn("", TEST_2, "TEST 3: %s", "ttt");

    logger_trace_str("", 111, 111, TEST_1, "Avvio Sessione");
    logger_trace_str(NULL, 111, 111, TEST_2, "Avvio Sessione");

    for (int i = 0; i < 3; i++)
    {

        logger_info_str("Thread Main", 123, 123, TEST_1, "Avvio Sessione");

        sleep(2);

        logger_warn_str("Thread Main", 123, 123, TEST_2, "Avvio Sessione");
    }

    logger_trace_str("Thread Main", 123, 123, TEST_1, "Avvio Sessione");

    logger_debug_str("Thread Main", 123, 123, TEST_2, "Avvio Sessione");

    logger_error_str("Thread Main", 123, 123, TEST_1, "Avvio Sessione");

    logger_trace_str_redux("Thread Main", TEST_2, "Avvio Sessione");

    logger_debug_str_redux("Thread Main", TEST_1, "Avvio Sessione");

    logger_error_str_redux("Thread Main", TEST_2, "Avvio Sessione");

    sleep(4);
    logger_destroy(&loggerOperationState);
    printf("Stato Operazione %i\n", loggerOperationState);

    return EXIT_SUCCESS;
}
