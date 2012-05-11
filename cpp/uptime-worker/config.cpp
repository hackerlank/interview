#include "config.h"

#define MAX_THREADS 40
#define DBHOST "localhost"
#define DBPORT 5432
#define DBLOGIN ""
#define DBNAME ""
#define DBPASS ""
#define USER_AGENT "Mozilla/5.0 (Windows; U; Windows NT 5.1; rv:1.7.3) Gecko/20041001 Firefox/0.10.1 muzino"

int cfg_get_max_threads()
{
    return MAX_THREADS;
}

char* cfg_get_dbmasterhost()
{
    return (char*)DBHOST;
}

char* cfg_get_dbhost()
{
    return (char*)DBHOST;
}

int   cfg_get_dbmasterport()
{
    return DBPORT;
}

int cfg_get_dbport()
{
    return DBPORT;
}

char* cfg_get_dbmastername()
{
    return (char*)DBNAME;
}

char* cfg_get_dblogin()
{
    return (char*)DBLOGIN;
}

char* cfg_get_dbmasterlogin()
{
    return (char*)DBLOGIN;
}

char* cfg_get_dbmasterpass()
{
    return (char*)DBPASS;
}

char* cfg_get_dbpass()
{
    return (char*)DBPASS;
}

char* cfg_get_user_agent()
{
    return (char*)USER_AGENT;
}
