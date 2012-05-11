#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

int cfg_get_max_threads();

char* cfg_get_dbmasterhost();
int   cfg_get_dbmasterport();
char* cfg_get_dbmastername();
char* cfg_get_dbmasterlogin();
char* cfg_get_dbmasterpass();

int cfg_get_dbport();
char* cfg_get_dbhost();
char* cfg_get_dbpass();
char* cfg_get_dblogin();

char* cfg_get_user_agent();


#endif // CONFIG_H_INCLUDED
