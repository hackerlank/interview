#include "StdAfx.h"
#include "http_client.h"
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#ifdef __APPLE__
	// TODO: SDL_net ���������� ��� framework. ���������, ��� ��� ����� � �������� ����������.
	#include "SDL_net.h"
#else
	#include "SDL/SDL_net.h"
#endif
#include "../misc.h"
#include "../defines.h"

#define HTTP_PORT 80 // Thanks, cap!

/* ���� ����� ������������ � highscores. ����� ������ �������� ������ �� ��������� ���������.
   ����� ���� �� �������� ������ iichantra, �� ����� ��������� ���� �� ������� ������ ��������� */
#define HTTP_USER_AGENT "iichantra/SDL"

#define HTTP_RESPONSE_BUFFER_SIZE 10192

// @see http://www.zedwood.com/article/111/cpp-urlencode-function
std::string char2hex( char dec )
{
    char dig1 = (dec&0xF0)>>4;
    char dig2 = (dec&0x0F);
    if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48inascii
    if (10<= dig1 && dig1<=15) dig1+=97-10; //a,97inascii
    if ( 0<= dig2 && dig2<= 9) dig2+=48;
    if (10<= dig2 && dig2<=15) dig2+=97-10;

    std::string r;
    r.append( &dig1, 1);
    r.append( &dig2, 1);
    return r;
}

// @see http://www.zedwood.com/article/111/cpp-urlencode-function
std::string urlencode(const std::string &c)
{
    
    std::string escaped="";
    int max = static_cast<int>(c.length());
    for(int i=0; i<max; i++)
    {
        if ( (48 <= c[i] && c[i] <= 57) ||//0-9
             (65 <= c[i] && c[i] <= 90) ||//abc...xyz
             (97 <= c[i] && c[i] <= 122) || //ABC...XYZ
             (c[i]=='~' || c[i]=='!' || c[i]=='*' || c[i]=='(' || c[i]==')' || c[i]=='\'')
        )
        {
            escaped.append( &c[i], 1);
        }
        else
        {
            escaped.append("%");
            escaped.append( char2hex(c[i]) );//converts char 255 to string "ff"
        }
    }
    return escaped;
}

/* �� ������ ������ ���� ������� SDL � SDLnet ��� ������ ���� ���������������� */

// ������ �������������:
// http_send_request("127.0.0.1", "/index.php", content) 
bool http_send_request(const std::string host, const std::string uri, std::string& content, const char method[] = "GET", std::map<std::string, std::string>* headers = NULL, std::map<std::string, std::string>* params = NULL)
{
	IPaddress ip;
	TCPsocket sock;
	Uint16 port;

	port = (Uint16)HTTP_PORT;

	if(SDLNet_ResolveHost(&ip, host.c_str(), port) == -1) {
		Log(DEFAULT_NET_LOG_NAME, logLevelError, "Failed to resolve host %s", host.c_str());
		return false;
	}

	sock = SDLNet_TCP_Open(&ip);
	if(!sock) {
		Log(DEFAULT_NET_LOG_NAME, logLevelError, "Failed to open TCP socket to host %s on port %d. Error: %s", host.c_str(), HTTP_PORT, SDLNet_GetError());
		return false;
	}

	stringstream msg (stringstream::in | stringstream::out);
	// ������ �������� HTTP/1.1, ������ ��� ����� �������� � ������, � ��� �����.
	// � 1.0 �� ����� ����� ����� �������� (�� ��������� Content-Length)
	// PS. �� ������� ���� ��� ����������� ��� apache/mod_Wsgi/django, ������ ����� �� �������� �� � ����
	// � ������ ��� ���������� ������� Content-Length ��������� -_-
	if (method[0] == 'G')
	{
		msg << method << " " << uri << "?";
		if ( params != NULL) {
			std::map<std::string, std::string>::iterator it;
			std::string content;

			for(it = params->begin(); it != params->end(); it++) {
				if(it != params->begin()) {
					content += "&";
				}
				content += it->first + "=" + urlencode(it->second);
			}

			msg << content;
		}
		msg << " HTTP/1.0\r\n";
	}
	else
	{
		msg << method << " " << uri << " HTTP/1.0\r\n";
	}
	msg << "Host: " << host << "\r\n";
	msg << "User-Agent: " << HTTP_USER_AGENT << "\r\n";
	if(headers != NULL) {
		std::map<std::string, std::string>::iterator it;

		for(it = headers->begin(); it != headers->end(); it++) {
			msg << it->first << ": " << it->second << "\r\n";
		}	
	}

	if(method[0] == 'P' )
	{
		if ( params != NULL) {
			msg << "Content-Type: application/x-www-form-urlencoded\r\n";

			std::map<std::string, std::string>::iterator it;
			std::string content;

			for(it = params->begin(); it != params->end(); it++) {
				if(it != params->begin()) {
					content += "&";
				}
				content += it->first + "=" + urlencode(it->second);
			}

			msg << "Content-Length: " << content.length() << "\r\n";
			msg << "\r\n";
			msg << content;
		} else {
			msg << "\r\n";
		}
	}
	else {
		msg << "\r\n";
	}

	std::string tmp = msg.str();

	//Log( DEFAULT_LOG_NAME, logLevelError, "%s", msg.str().c_str() );

	int result = SDLNet_TCP_Send(sock, msg.str().c_str(), static_cast<int>(msg.str().length()));

	if((unsigned)result < msg.str().length()) {
		Log(DEFAULT_NET_LOG_NAME, logLevelError, "Failed to send message. Response length: %d", result);
		SDLNet_TCP_Close(sock);
		return false;
	}

	// �������� ���������
	char response_buffer[HTTP_RESPONSE_BUFFER_SIZE];
	result = 1;
	stringstream response (stringstream::in | stringstream::out);
	while ( result > 0 )
	{
		memset(response_buffer, '\0', HTTP_RESPONSE_BUFFER_SIZE);
		result = SDLNet_TCP_Recv(sock, response_buffer, HTTP_RESPONSE_BUFFER_SIZE-1);
		response << response_buffer << "\n";
	}

	if(result < 0) {
		Log(DEFAULT_NET_LOG_NAME, logLevelError, "Failed to get http response");
		return false;
	}

	/* ������� ����� �� �������:
		HTTP/1.1 200 OK
		Date: Thu, 06 Jan 2011 00:05:56 GMT
		Server: Apache/2.2.16 (FreeBSD) DAV/2 mod_wsgi/3.2 Python/2.6.6 PHP/5.3.2 with Suhosin-Patch mod_ssl/2.2.16 OpenSSL/0.9.8k
		Content-Length: 2
		Connection: close
		Content-Type: text/plain

		OK
	*/
	

	//Log( DEFAULT_LOG_NAME, logLevelError, "%s", response_buffer );

	 content = "";
	 bool content_flag = false;

	 while(!response.eof()) {
		 string line;
		 getline(response, line);
		 
		 if(startswith(line, "HTTP") && !endswith(line, "200 OK\r")) {
			 Log(DEFAULT_NET_LOG_NAME, logLevelError, "Expected 200 OK but received %s", line.c_str());
			 return false;
		 }

		 if(line == "\r") {
			 // ������ ���������� �������
			 content_flag = true;
		 }

		 if(content_flag) {
			 if(!content.empty()) {
				 // ��������� ������� ������
				 content += "\n";
			 }

			 if(line != "\r")
				content += line;
		 }
	}

	SDLNet_TCP_Close(sock);

	return true;
}
