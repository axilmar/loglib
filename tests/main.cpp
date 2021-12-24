#include <cstdlib>
#include "loglib.h"


int main() {
    LOGLIB_init();
    LOGLIB_log_info("some info: %i %i", 1, 2);
    LOGLIB_log_warning("some warning: %i %i", 3, 4);
    LOGLIB_log_error("some error: %i %i", 5, 6);
    char big_buffer[8192];
    for (size_t i = 0; i < sizeof(big_buffer) - 1; ++i) {
        big_buffer[i] = '0' + (i % 10);
    }
    big_buffer[sizeof(big_buffer) - 1] = '\0';
    LOGLIB_log_error("really long text: %s", big_buffer);
    LOGLIB_cleanup();
    system("pause");
    return 0;
}
