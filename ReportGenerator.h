#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#define MAX_ENTRIES 30

struct WeatherData {
    char datetime[11];
    float temp;
    float windspeed;
    float humidity;
    char description[50];
};

void calculateAverages(struct WeatherData data[], int size, float *avgTemp, float *avgWindSpeed, float *avgHumidity);
int readJsonFromFile(const char *filename, struct WeatherData data[], int *size);
void generateTextFile(const char *filename, struct WeatherData data[], int dataSize, float avgTemp, float avgWindSpeed, float avgHumidity);

#endif /* REPORTGENERATOR_H */

