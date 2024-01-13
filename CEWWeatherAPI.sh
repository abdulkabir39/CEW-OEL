#!/bin/bash

chmod +x WeatherProcessedData.sh
./WeatherProcessedData.sh

if [ $? -eq 0 ]; then
   	chmod +x ReportAnomalyEmail.sh
	./ReportAnomalyEmail.sh

	if [ $? -eq 0 ]; then
		echo "Compilation Successful!"
	else
		echo "An error has occurred in Report Generation / Anomaly Generation / Email Sending."
	fi
else
	echo "An error has occurred in Data Retrieving / Data Processing."
fi
