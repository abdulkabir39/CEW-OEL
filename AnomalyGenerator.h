#ifndef ANOMALY_CHECKER_H
#define ANOMALY_CHECKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

typedef struct {
    char datetime[20];
    float humidity;
    float windspeed;
} WeatherData;

void checkAndLogAnomalies(WeatherData *data, int numDays);

#endif // ANOMALY_CHECKER_H
