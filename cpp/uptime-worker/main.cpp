#define _MULTI_THREADED
#include <iostream>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <stdio.h>

#include "config.h"
#include "work.h"
#include "data_in.h"
#include "db.h"


/* MAIN PROCESS
    1. Get bunch of site names from DB
    2. Create working process for each of the site
    3. Repeat

*/

/*  WORKING PROCESS
    1. Initialize CURL
    2. Sending request
    3. Error handling
    4. Write result to DB
*/

/*
    TODO
    1. Config
    2. Logging
    3. 301-follow
    4. Scalability
*/

volatile int thread_count = 0;

int main(int argc, char *argv[])
{
    for(;;)
    {
        int sites_count = 0;

        data_in** data = db_get_sites(&sites_count);

        int i = 0;
        while(i < sites_count)
        {
            static int tl_flag;

            if(thread_count == cfg_get_max_threads()) {

                if(!tl_flag) {
                    printf("Reached thread limit, waiting. \n");
                    tl_flag = 1;
                }

                continue;

            } else {

                tl_flag = 0;
            }

            pthread_t thread;

            int t_ret = pthread_create(&thread, NULL, work_function, (void*)data[i]);

            if(t_ret != 0)
            {
                fprintf(stderr, "Failed to create thread for site %s. \n\n", data[i]->site);
                continue;
            }

            thread_count++;

            t_ret = pthread_detach(thread);

            if(t_ret != 0)
            {
                fprintf(stderr, "Failed to detach thread for site %s. \n\n", data[i]->site);
            }

            i++;
        }

        if(data)
            free(data);

        sleep(1);
    }

	return 0;
}

