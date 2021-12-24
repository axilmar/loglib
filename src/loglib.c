#include <stdio.h>
#include <time.h>
#include "loglib.h"


//logger ptr
typedef LOGLIB_LOGGER* LOGGER_PTR;


//internal logger table
static LOGGER_PTR* logger_table = NULL;
static size_t logger_table_size = 0;


//on windows, also log in Visual Studio
#ifdef _WIN32


#include <Windows.h>


static void log_debug_output(LOGLIB_LOGGER* logger, const char* msg) {
    OutputDebugStringA(msg);
}


LOGLIB_LOGGER LOGLIB_debug_output_logger = { log_debug_output, ~(uint64_t)(0), NULL };


#endif


//log in stdout
static log_stdout(LOGLIB_LOGGER* logger, const char* msg) {
    fprintf(stdout, msg);
}


//log in stderr
static log_stderr(LOGLIB_LOGGER* logger, const char* msg) {
    fprintf(stderr, msg);
}


//log in file
static log_file(LOGLIB_LOGGER* logger, const char* msg) {
    if (logger->data) {
        fprintf((FILE*)logger->data, msg);
    }
}



//get datetime string
static void get_datetime_str(char* buffer, int size) {
    time_t timeval = time(NULL);
    struct tm tms;
    localtime_s(&tms, &timeval);
    strftime(buffer, size, "%Y-%m-%e %H:%M:%S", &tms);
}


//standard loggers
LOGLIB_LOGGER LOGLIB_stdout_logger = { log_stdout, ~(uint64_t)(0), NULL };
LOGLIB_LOGGER LOGLIB_stderr_logger = { log_stderr,            (0), NULL };
LOGLIB_LOGGER LOGLIB_file_logger   = { log_file  , ~(uint64_t)(0), NULL };


//Initializes the library.
void LOGLIB_init() {
    #ifdef _WIN32 
    logger_table_size = 4;
    logger_table = malloc(sizeof(LOGGER_PTR) * logger_table_size);
    logger_table[0] = &LOGLIB_debug_output_logger;
    logger_table[1] = &LOGLIB_stdout_logger;
    logger_table[2] = &LOGLIB_stderr_logger;
    logger_table[3] = &LOGLIB_file_logger;
#else
    logger_table_size = 3;
    logger_table = malloc(sizeof(LOGGER_PTR) * logger_table_size);
    logger_table[0] = &LOGLIB_stdout_logger;
    logger_table[1] = &LOGLIB_stderr_logger;
    logger_table[2] = &LOGLIB_file_logger;
#endif
}


//Cleans up the library.
void LOGLIB_cleanup() {
    free(logger_table);
}


//add a logger
void LOGLIB_add_logger(LOGLIB_LOGGER* logger) {
    //check if it exists
    for (size_t i = 0; i < logger_table_size; ++i) {
        if (logger_table[i] == logger) {
            return;
        }
    }

    //allocate one more entry
    logger_table = realloc(logger_table, sizeof(LOGGER_PTR) * (logger_table_size + 1));

    //set entry
    logger_table[logger_table_size] = logger;

    //one more entry
    ++logger_table_size;
}


//removes a logger.
void LOGLIB_remove_logger(LOGLIB_LOGGER* logger) {
    for (size_t i = 0; i < logger_table_size; ++i) {
        if (logger_table[i] == logger) {
            memmove(logger_table + i, logger_table + i + 1, sizeof(LOGGER_PTR) * (logger_table_size - 1 - i));
            --logger_table_size;
            return;
        }
    }
}


//Logs a message.
void LOGLIB_log_va(uint64_t type, const char* type_name, const char* format, va_list args) {
    //create the date & time string
    char datetime_buf[256];
    get_datetime_str(datetime_buf, sizeof(datetime_buf));

    //create the message
    char msg_buf[4096];
    vsnprintf(msg_buf, sizeof(msg_buf), format, args);

    //create the whole message
    char final_buf[256 + 4096 + 32];
    snprintf(final_buf, sizeof(final_buf), "[%s] [%s] %s\n", datetime_buf, type_name, msg_buf);

    //log the message
    for (size_t i = 0; i < logger_table_size; ++i) {
        LOGGER_PTR logger = logger_table[i];
        if (logger->flags & type) {
            logger->log(logger, final_buf);
        }
    }
}


//Logs a message.
void LOGLIB_log(uint64_t type, const char* type_name, const char* format, ...) {
    va_list args;
    va_start(args, format);
    LOGLIB_log_va(type, type_name, format, args);
    va_end(args);
}


//Logs information.
void LOGLIB_log_info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    LOGLIB_log_va(LOGLIB_LOG_INFORMATION, "INFORMATION", format, args);
    va_end(args);
}


//Logs a warning.
void LOGLIB_log_warning(const char* format, ...) {
    va_list args;
    va_start(args, format);
    LOGLIB_log_va(LOGLIB_LOG_WARNING, "WARNING", format, args);
    va_end(args);
}


//Logs an error.
void LOGLIB_log_error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    LOGLIB_log_va(LOGLIB_LOG_ERROR, "ERROR", format, args);
    va_end(args);
}


//Sets the log flags for all the loggers.
void LOGLIB_set_log_flags(uint64_t flags) {
    for (size_t i = 0; i < logger_table_size; ++i) {
        logger_table[i]->flags = flags;
    }
}


//add log flags
void LOGLIB_add_log_flags(uint64_t flags) {
    for (size_t i = 0; i < logger_table_size; ++i) {
        logger_table[i]->flags |= flags;
    }
}


//Removes the given log flags for all the loggers.
void LOGLIB_remove_log_flags(uint64_t flags) {
    for (size_t i = 0; i < logger_table_size; ++i) {
        logger_table[i]->flags = logger_table[i]->flags & ~flags;
    }
}

