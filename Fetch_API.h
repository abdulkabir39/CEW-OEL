#ifndef WEATHER_API_H
#define WEATHER_API_H

#include <curl/curl.h>

// Structure to hold memory and size
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Function to handle data retrieval callback
size_t write_callback(void *contents, size_t size, size_t nmemb, struct MemoryStruct *mem);

#endif /* WEATHER_API_H */

