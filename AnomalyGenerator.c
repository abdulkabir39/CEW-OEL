#include "AnomalyGenerator.h"


// Function to check and log anomalies
void checkAndLogAnomalies(WeatherData *data, int numDays) {
    FILE *logFile = fopen("AnomalyLog.txt", "w");

    if (!logFile) {
        perror("Error opening AnomalyLog.txt");
        return;
    }

    fprintf(logFile, "\nAnomaly Log:\n\n");

    for (int i = 0; i < numDays; ++i) {
    	if (data[i].humidity >= 70 || data[i].humidity <= 30 || data[i].windspeed >= 25){
		fprintf(logFile, "Date: %s\n", data[i].datetime);
		
		if (data[i].humidity >= 70){
			fprintf(logFile, "   Humidity: %.2f - Very humid, lots of moisture in the air\n", data[i].humidity);
		} else if (data[i].humidity <= 30){
			fprintf(logFile, "   Humidity: %.2f - Very dry weather\n", data[i].humidity);
		}
        	
        	if (data[i].windspeed >= 25){
        		fprintf(logFile, "   Windspeed: %.2f - Very Windy weather\n", data[i].windspeed);
        	}
        fprintf(logFile, "\n");
    	}
    }	
    fclose(logFile);
}

int main() {
    // Open the JSON file
    FILE *jsonFile = fopen("processdata.json", "r");

    if (!jsonFile) {
        perror("Error opening processdata.json");
        return 1;
    }

    // Get the file size
    fseek(jsonFile, 0, SEEK_END);
    long fileSize = ftell(jsonFile);
    fseek(jsonFile, 0, SEEK_SET);

    // Read the entire file into a buffer
    char *jsonBuffer = malloc(fileSize + 1);

    if (!jsonBuffer) {
        perror("Error allocating memory for JSON data");
        fclose(jsonFile);
        return 1;
    }

    fread(jsonBuffer, 1, fileSize, jsonFile);
    fclose(jsonFile);

    // Null-terminate the buffer
    jsonBuffer[fileSize] = '\0';

    // Parse JSON data
    cJSON *json = cJSON_Parse(jsonBuffer);

    if (!json) {
        printf("Error parsing JSON data.\n");
        free(jsonBuffer);
        return 1;
    }

    // Get the "result" array
    cJSON *resultArray = cJSON_GetObjectItemCaseSensitive(json, "result");

    if (!cJSON_IsArray(resultArray)) {
        printf("Error: 'result' is not an array.\n");
        free(jsonBuffer);
        cJSON_Delete(json);
        return 1;
    }

    int numDays = cJSON_GetArraySize(resultArray);

    // Allocate memory for weather data
    WeatherData *weatherData = malloc(numDays * sizeof(WeatherData));

    if (!weatherData) {
        perror("Error allocating memory for weather data");
        free(jsonBuffer);
        cJSON_Delete(json);
        return 1;
    }

    // Parse each day's data
    for (int i = 0; i < numDays; ++i) {
        cJSON *dayData = cJSON_GetArrayItem(resultArray, i);

        // Parse individual fields
        strcpy(weatherData[i].datetime, cJSON_GetObjectItemCaseSensitive(dayData, "datetime")->valuestring);
        weatherData[i].humidity = cJSON_GetObjectItemCaseSensitive(dayData, "humidity")->valuedouble;
        weatherData[i].windspeed = cJSON_GetObjectItemCaseSensitive(dayData, "windspeed")->valuedouble;
    }

    // Check and log anomalies
    checkAndLogAnomalies(weatherData, numDays);

    // Free allocated memory
    free(weatherData);
    free(jsonBuffer);

    // Cleanup cJSON
    cJSON_Delete(json);

    return 0;
}

