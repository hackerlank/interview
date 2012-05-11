#include "data_in.h"
#include "config.h"
#include "work.h"
#include "db.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

#define CURL_STATICLIB
#include <curl/curl.h>

#ifdef _WINDOWS
#pragma comment (lib, "libcurl.lib")
#endif

extern volatile int thread_count;

using namespace std;

size_t curl_writer( void *ptr, size_t size, size_t nmemb, void *stream)
{

	return 0;
}

void* work_function(void* ptr)
{
    CURL *curl = NULL;
    curl = curl_easy_init();

	if(curl && ptr) {

        data_in* data = (data_in*)ptr;

        // Making URL
	    string site = (string)(data->site);

	    string::size_type loc = site.find( "http://", 0 );
	    if(loc == string::npos)
            site = (string)("http://") + site;

        static char* user_agent = cfg_get_user_agent();

        // CURL options
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
		curl_easy_setopt(curl, CURLOPT_URL, site.c_str());
		curl_easy_setopt(curl, CURLOPT_HEADER , 1 );
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, data->timeout);

        // Sending request
		curl_easy_perform(curl);

		long http_code = 0;
		double total_time = 0;

		CURLcode result = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            		 result = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

        int delay = total_time * 1000;

		char* result_str;

		asprintf(&result_str, "%s: %d (%d ms)", site.c_str(), (int)http_code, delay);

		cout << result_str << "\n";

		free(result_str);

		curl_easy_cleanup(curl);

		// Connect to db
		db_insert_uptime((data_in*)ptr, http_code, delay);
	}

    if(ptr)
        free((data_in*)ptr);

	thread_count--;

	pthread_exit(NULL);
	return NULL;
}
