#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <string.h>
#include "Fetch_API.h" 

// Save retrieved data to a Raw Data JSON file
size_t write_callback(void *contents, size_t size, size_t nmemb, struct MemoryStruct *mem) {
    size_t realsize = size * nmemb;
    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        fprintf(stderr, "Not enough memory (realloc returned NULL)\n");
        return 0;
    }
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

int main() {
    const char *url = "https://weather.visualcrossing.com/VisualCrossingWebServices/rest/services/timeline/24.8607,67.0011/next14days?unitGroup=metric&key=ZW9AR97RBK5CAKJJ8RDE9DR4P";

    // Initialize libcurl
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error initializing cURL.\n");
        return 1;
    }


    curl_easy_setopt(curl, CURLOPT_URL, url);

    // MemoryStruct to hold the received data
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

    // Performing the request to the Visual Crossing api
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "cURL failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        free(chunk.memory);
        return 1;
    }

    curl_easy_cleanup(curl);

    // Parsing Data
    cJSON *json = cJSON_Parse(chunk.memory);
    if (!json) {
        fprintf(stderr, "Error parsing JSON data.\n");
        free(chunk.memory);
        return 1;
    }


    FILE *jsonFile = fopen("unprocessed_data.json", "w");

	if (!jsonFile) {
	    fprintf(stderr, "Error opening file for writing.\n");
	    cJSON_Delete(json);
	    free(chunk.memory);
	    return 1;
	}

	char *jsonString = cJSON_Print(json);
	fprintf(jsonFile, "%s", jsonString);

	cJSON_Delete(json);
	free(jsonString);
	free(chunk.memory);
	fclose(jsonFile);
    return 0;
}


