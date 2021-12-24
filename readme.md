# Loglib

C logging library.

## Example

```c
#include "loglib.h"

int main() {
    LOGLIB_init();
    LOGLIB_log_info("Data: %i", 1);
    LOGLIB_log_warning("Possible error: %i", 2);
    LOGLIB_log_error("Error: %i", 3);
    LOGLIB_cleanup();
}
```

## Standard loggers

- LOGLIB_debug_output_logger: only in Windows, it logs a message into Visual Studio output; activated by default.
- LOGLIB_stdout_logger; prints a message in the console; activated by default.
- LOGLIB_stderr_logger; prints a message in standard error; not active by default.
- LOGLIB_file_logger; prints a message into a file; not active by default.

## Logging in a file

The logger's `LOGLIB_file_logger`data member must be set to a FILE*. Example:

```c
LOGLIB_file_logger.data = fopen("log.txt");
```

## Customizing individual loggers

Each logger has its own log flags. Setting those individually allows customizing the behavior of each logger. For example, in order to print errors in stderr and all others in stdout:

```c
LOGLIB_stdout_logger.flags = LOGLIB_LOG_INFORMATION | LOBLIG_LOG_WARNING;
LOGLIB_stderr_logger.flags = LOGLIB_LOG_ERROR;
```

## Setting log flags for all loggers

```C
LOGLIB_set_log_flags(LOGLIB_LOG_INFORMATION | LOBLIG_LOG_WARNING);
LOGLIB_add_log_flags(LOGLIB_LOG_INFORMATION | LOBLIG_LOG_WARNING);
LOGLIB_remove_log_flags(LOGLIB_LOG_INFORMATION | LOBLIG_LOG_WARNING);
```

## Adding custom loggers

The struct `LOGLIB_LOGGER' has the following members:

1. log function.
2. log flags.
3. data pointer.

The function `LOGLIB_add_logger` adds a logger.

The function `LOGLIB_remove_logger` removes a logger.

```
LOGLIB_LOGGER my_custom_logger = {my_custom_log_function, ~(uint64_t)0, my_custom_data};
LOGLIB_add_logger(&my_custom_logger);
```

## Adding custom log types

The enum `LOGLIB_LOG_FLAGS` has a member value named `LOGLIB_LOG_FLAG_USER` which is the first value for custom types.

The log flags have 64 bits size, and so 61 bits are available for 61 custom log types.

```C
enum EXTRA_LOG_FLAGS {
  LOG_EXTRA_1 = LOGLIB_LOG_FLAG_USER << 0,
  LOG_EXTRA_2 = LOGLIB_LOG_FLAG_USER << 1
};

void log_extra_1(const char* format, ...) {
    va_list args;
    va_start(args, format);
    LOGLIB_log_va(LOG_EXTRA_1, "EXTRA 1", format, args);
    va_end(args);
}

void log_extra_2(const char* format, ...) {
    va_list args;
    va_start(args, format);
    LOGLIB_log_va(LOG_EXTRA_2, "EXTRA 2", format, args);
    va_end(args);
}

int main() {
    LOGLIB_init();
    LOGLIB_add_log_flags(LOG_EXTRA_1 | LOG_EXTRA_2);
    log_extra_1("extra 1 logging");
    log_extra_2("extra 2 logging");
    LOGLIB_cleanup();
}
```

## Disabling the logger at compile time

If `LOGLIB_DISABLED` is defined, then all logging code is removed and the logging functions do nothing.
