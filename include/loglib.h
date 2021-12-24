#ifndef LOGLIB_H
#define LOGLIB_H


#include <stdint.h>
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Buffer size for messages. 
 */
#ifndef LOGLIB_BUFFER_SIZE
#define LOGLIB_BUFFER_SIZE 4095
#endif


/**
 * log flags. 
 */
enum LOGLIB_LOG_FLAGS {
    /**
     * log information. 
     */
    LOGLIB_LOG_INFORMATION = 1,

    /**
     * log warnings.
     */
     LOGLIB_LOG_WARNING = 2,

     /**
      * log errors.
      */
      LOGLIB_LOG_ERROR = 4,

      /**
       * First user flag. 
       */
      LOGLIB_LOG_FLAG_USER = 8
};


/**
 * Function for logging.
 * @param logger logger object.
 * @param msg message to log.
 */
typedef void (*LOGLIB_LOG_FUNCTION)(struct LOGLIB_LOGGER* logger, const char* msg);


/**
 * Logger object. 
 */
typedef struct LOGLIB_LOGGER {
    /**
     * Log function. 
     */
    LOGLIB_LOG_FUNCTION log;

    /**
     * Logger flags. 
     */
    uint64_t flags;

    /**
     * Logger extra data. 
     */
    void* data;
} LOGLIB_LOGGER;


#ifdef _WIN32
/**
 * Windows logger that uses the function OutputDebugString to show messages in the attached IDE output. 
 */
extern LOGLIB_LOGGER LOGLIB_debug_output_logger;
#endif


/**
 * Stdout logger. 
 */
extern LOGLIB_LOGGER LOGLIB_stdout_logger;


/**
 * Stderr logger. 
 */
extern LOGLIB_LOGGER LOGLIB_stderr_logger;


/**
 * File logger.
 * Initially added, but its data member is null; set its data member to a FILE* in order to log messages into that file.
 */
extern LOGLIB_LOGGER LOGLIB_file_logger;


/**
 * Initializes the library. 
 */
void LOGLIB_init();


/** 
 * Cleans up the library.
 */
void LOGLIB_cleanup();


/**
 * Adds a logger.
 * @param logger logger to add.
 */
void LOGLIB_add_logger(LOGLIB_LOGGER* logger);


/**
 * Removes a logger.
 * @param logger logger to remove.
 */
void LOGLIB_remove_logger(LOGLIB_LOGGER* logger);


/**
 * Logs a message.
 * @param type log type.
 * @param type_name type name.
 * @param format format.
 * @param args arguments.
 */
void LOGLIB_log_va(uint64_t type, const char* type_name, const char* format, va_list args);


/**
 * Logs a message.
 * @param type log type.
 * @param type_name type name.
 * @param format format.
 * @param ... arguments.
 */
void LOGLIB_log(uint64_t type, const char* type_name, const char* format, ...);


/**
 * Logs information.
 * @param format format.
 * @param ... arguments.
 */
void LOGLIB_log_info(const char* format, ...);


/**
 * Logs a warning.
 * @param format format.
 * @param ... arguments.
 */
void LOGLIB_log_warning(const char* format, ...);


/**
 * Logs an error.
 * @param format format.
 * @param ... arguments.
 */
void LOGLIB_log_error(const char* format, ...);


/**
 * Sets the log flags for all the loggers. 
 */
void LOGLIB_set_log_flags(uint64_t flags);


/**
 * Adds the given log flags for all the loggers.
 */
void LOGLIB_add_log_flags(uint64_t flags);


/**
 * Removes the given log flags for all the loggers.
 */
void LOGLIB_remove_log_flags(uint64_t flags);


#ifdef __cplusplus
} //extern "C"
#endif


#endif //LOGLIB_H
