#ifndef EMAIL_SENDER_H
#define EMAIL_SENDER_H

#include <stdio.h>

// Function to read file content and provide it to cURL
size_t read_callback(void *ptr, size_t size, size_t nmemb, FILE *stream);

// Function to send an email with a report and anomaly file attachment
int Email(const char *to, const char *report_path, const char *anomaly_path);

#endif // EMAIL_SENDER_H

