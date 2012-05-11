#ifndef DATA_IN_H_INCLUDED
#define DATA_IN_H_INCLUDED

typedef struct {

    char site[513];

    char dbname[64];
    char dbhost[64];
    char dblogin[64];
    char dbpass[64];
    int  dbport;

    int timeout;

} data_in;


#endif // DATA_IN_H_INCLUDED
