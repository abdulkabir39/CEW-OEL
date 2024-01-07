#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>

#define BASE_ADDRESS "https://weather.visualcrossing.com/VisualCrossingWebServices/rest/services/timeline/"
#define API_KEY "GSF87S3N5CL3FLVMY4264S9RU"
int save_json_to_file(const char *filename, const char *json);

typedef struct {
    char *string;
    size_t size;
} Response;

size_t write_chunk(void *data, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;

    // Cast userdata to Response structure
	Response *response = (Response *) userdata;

    // Reallocate memory for the response string
	char *ptr = realloc(response->string, response->size + total_size + 1);
	if(ptr == NULL){
		fprintf(stderr, "Memory Allocation Failed.");
		return 0; // Return 0 to indicate failure.
	}

    // Update the response structure with the new string and size   
	response->string = ptr;

    // Copies the block of memory (data) to the response.
	memcpy(&(response->string[response->size]), data, total_size);
	response->size += total_size;

    // Ensure null-termination.
	response->string[response->size] = '\0';

    return total_size;
}



int get_data(char **location, char **date){
	CURL *curl;
	CURLcode result;
	char URL[128];

	Response response;
    response.string = NULL;
    response.size = 0;

	int success;


	strcat(URL, BASE_ADDRESS);
	strcat(URL, *location);
	strcat(URL, "/");
	strcat(URL, *date);
	strcat(URL, "?key=");
	strcat(URL, API_KEY);

	free(*location);
	free(*date); 
	printf("%s", URL);
	curl = curl_easy_init();
	
	if(curl == NULL){
		fprintf(stderr, "Curl Library Initialization Failure.");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, URL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_chunk);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &response);

	result = curl_easy_perform(curl);
	if(result != CURLE_OK){
		fprintf(stderr, "Error : %s\n", curl_easy_strerror(result));
		return -1;
	}

	printf("%u", result);
	curl_easy_cleanup(curl);
		success = save_json_to_file("output.json", response.string);
	free(response.string);

	if (success != 0) {
        fprintf(stderr, "Error: Failed to save JSON to file.\n");
    }

	return 0;
}

int save_json_to_file(const char *filename, const char *json) {
    FILE *file = fopen(filename, "w");

    // Checks if the file is opened succesfully or not.
    if (file == NULL) {
        fprintf(stderr, "Error opening file");
        return -1; // Return -1 to indicate failure
    }
    
    // Writes the string data to the file.
    fprintf(file, "%s", json);
    
    // Closes the file.
    fclose(file);

    return 0;	
}
int main(void*){
	printf("Getting Data\n");
	char *location, *date;
	location = malloc(20*sizeof(char));
	date = malloc(20*sizeof(char));
	if(location == NULL || date == NULL){
		fprintf(stderr, "Memory Allocation Failed.");
		return -1;
	}
	printf("Enter the location: ");
	scanf("%s", location);
	printf("Enter the timestamp: ");
	scanf("%s", date);

	return get_data(&location, &date);
}