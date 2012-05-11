#include <string>
#ifdef __FreeBSD__
#include <libpq-fe.h>
#endif
#ifdef __linux__
#include <postgresql/libpq-fe.h>
#endif
#include <stdio.h>
#include <string.h>
#include "data_in.h"
#include "db.h"
#include "config.h"
#include <iostream>
#include <stdarg.h>
#include <stdlib.h>

using namespace std;

PGconn* dbmaster_connect()
{
    char* dbhost = cfg_get_dbmasterhost();
    int   dbport = cfg_get_dbmasterport();
    char* dbname = cfg_get_dbmastername();
    char* dblogin = cfg_get_dbmasterlogin();
    char* dbpass = cfg_get_dbmasterpass();

    char conn_str[128];

    memset(&conn_str, '\0', 128);

    sprintf(conn_str, "host=%s port=%u dbname=%s user=%s password=%s", dbhost, dbport, dbname, dblogin, dbpass);
    PGconn* conn = PQconnectdb(conn_str);
    return conn;
}

PGconn* db_connect(char* dbhost, int dbport, char* dbname, char* dblogin, char* dbpass)
{
    char conn_str[128];

    memset(&conn_str, '\0', 128);

    sprintf(conn_str, "host=%s port=%u dbname=%s user=%s password=%s", dbhost, dbport, dbname, dblogin, dbpass);
    PGconn* conn = PQconnectdb(conn_str);
    return conn;
}

bool db_disconnect(PGconn* conn)
{
    PQfinish(conn);
    return true;
}

PGresult* db_query(PGconn* conn, const char *format, ...)
{
	va_list argv;
	char *ptrQuery = NULL;
	PGresult *result;

	va_start(argv, format);
	vasprintf(&ptrQuery, format, argv);
	va_end(argv);

	if (!ptrQuery)
		return(0);

	result = PQexec(conn, ptrQuery);

	free(ptrQuery);

	return(result);
}


bool db_insert_uptime(data_in* data, int http_code, long delay)
{
    if(!data)
        return false;

    PGconn * conn = db_connect(data->dbhost, data->dbport, data->dbname, data->dblogin, data->dbpass);

    if(PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Failed to connect to postgres.\n\n");
        return false;
    }

    PGresult* result = db_query(conn, "INSERT INTO public.http_uptime (site, response, delay, crawler_ip, error) VALUES ('%s', %d, %d, null, null);", data->site, http_code, delay);

    if(!strlen(PQcmdTuples(result)))
    {
        fprintf(stderr, "Failed to insert uptime to postgres.\n\n");
        PQfinish(conn);
        return false;
    }

    PQclear(result);

    db_disconnect(conn);

    conn = dbmaster_connect();

    if(PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Failed to connect to postgres.\n\n");
        return false;
    }

    result = db_query(conn, "UPDATE public.http_sites SET last_update = NOW(), updating = FALSE WHERE site = '%s';", data->site);

    PQclear(result);

    db_disconnect(conn);

    return true;
}


data_in** db_get_sites(int* count)
{
    PGconn * conn = dbmaster_connect();

        if(PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Failed to connect to postgres.\n\n");
        return NULL;
    }
    PGresult* result;

    result = db_query(conn, "SELECT http_sites.id, http_sites.added, http_sites.site, http_sites.timeout, http_sites.period, http_sites.last_update, http_sites.updating, http_sites.dbname FROM http_sites WHERE http_sites.last_update < (now() - '00:01:00'::interval) AND NOT http_sites.updating ORDER BY http_sites.last_update FOR UPDATE;");
    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        cout << PQerrorMessage(conn) << "\n";
        fprintf(stderr, "PQresultStatus != PGRES_COMMAND_OK\n\n");
        PQfinish(conn);
        return NULL;
    }
    int i, nFields, nRows;

    nFields = PQnfields(result);
    nRows = PQntuples(result);

    data_in** p_result = NULL;

    if(nRows > 0) {

        p_result = (data_in**)malloc(sizeof(data_in*)*nRows);

        string id_str;

        for (i = 0; i < nRows; i++)
        {
                char* id = PQgetvalue(result, i, 0);
                //char* added = PQgetvalue(result, i, 1);
                char* site = PQgetvalue(result, i, 2);
                char* timeout = PQgetvalue(result, i, 3);

                p_result[i] = (data_in*)malloc(sizeof(data_in)*1);
                memset(p_result[i], '\0', sizeof(data_in));

                // 1. Setting site
                strcpy(p_result[i]->site, site);

                // 2. Setting timeout
                p_result[i]->timeout = atoi(timeout);

                strcpy(p_result[i]->dbhost, cfg_get_dbhost());
                strcpy(p_result[i]->dbpass, cfg_get_dbpass());
                strcpy(p_result[i]->dblogin, cfg_get_dblogin());
                strcpy(p_result[i]->dbname, site);

                p_result[i]->dbport = cfg_get_dbport();

                //strcpy(p_result[i], site);
                if(i > 0 && i < nRows-1)
                    id_str += ",";
                id_str += string(id);

        }

        PGresult* result2 = db_query(conn, "UPDATE public.http_sites SET updating = TRUE WHERE id IN (%s);", id_str.c_str());
        PQclear(result2);

    }

    PQclear(result);

    db_disconnect(conn);

    int* p_count = count;
    *p_count = nRows;

    return p_result;
}
