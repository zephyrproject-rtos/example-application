/*
 * Copyright (c) 2025 Osama Salah-ud-Din.
*/

#include <zephyr/kernel.h>
#include <string.h>
#include <iostream>

/*
    Logging macro with formatted uptime
    and trimmed file path (removes top 2 folders)
*/
#define MYLOG(fmt, ...) do {                                            \
    char __mylog_msg[512];                                              \
                                                                        \
    /* Get full path from __FILE__ */                                   \
    const char* __full_path = __FILE__;                                 \
    const char* __short_file = __full_path;                             \
                                                                        \
    /* Strip top 2 folders from path */                                 \
    int __slashes = 0;                                                  \
    for (const char* p = __full_path; *p; ++p) {                        \
        if (*p == '/') {                                                \
            __slashes++;                                                \
            if (__slashes == 2) __short_file = p + 1;                   \
        }                                                               \
    }                                                                   \
                                                                        \
    /* Get uptime in milliseconds */                                    \
    int64_t __uptime_ms = k_uptime_get();                               \
                                                                        \
    /* Convert to hh:mm:ss.mmm format */                                \
    int64_t __hours = __uptime_ms / (1000 * 60 * 60);                   \
    int64_t __minutes = (__uptime_ms / (1000 * 60)) % 60;               \
    int64_t __seconds = (__uptime_ms / 1000) % 60;                      \
    int64_t __milliseconds = __uptime_ms % 1000;                        \
                                                                        \
    /* Format and print the log message */                              \
    printk("[%02lld:%02lld:%02lld.%03lld] %s:%d - ",                    \
             __hours, __minutes, __seconds, __milliseconds,             \
             __short_file, __LINE__);                                   \
    snprintf(__mylog_msg, sizeof(__mylog_msg), fmt, ##__VA_ARGS__);     \
    printk("%s\n", __mylog_msg);                                          \
} while (0)

