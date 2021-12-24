#include <cstdlib>
#include "loglib.h"


int main() {
    LOGLIB_init();
    LOGLIB_remove_log_flags(LOGLIB_LOG_WARNING | LOGLIB_LOG_ERROR);
    LOGLIB_log_info("some info: %i %i", 1, 2);
    LOGLIB_log_warning("some warning: %i %i", 3, 4);
    LOGLIB_log_error("some error: %i %i", 5, 6);
    LOGLIB_cleanup();
    system("pause");
    return 0;
}
