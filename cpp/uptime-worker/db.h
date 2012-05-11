#ifndef DB_H_INCLUDED
#define DB_H_INCLUDED

data_in** db_get_sites(int* count);
bool db_insert_uptime(data_in* site, int http_code, long delay);

#endif // DB_H_INCLUDED
