#include "StdAfx.h"
#include "highscores.h"
#include "../misc.h"
#include <string>
#include <map>
#include "http_client.h"
#include "md5.h"

#define CHECKSUMS_ON

#ifdef CHECKSUMS_ON
#include <sstream>
#include "crc32.h"
static const char HS_VALIDATION_URI[] = "/validate_client/";
#endif

static const size_t HS_NICKNAME_MAX_LEN = 16;
static const char HS_SALT[] = "UBW";
static const char HS_HOST[] = "highscores.iichantra.ru";
static const char HS_SUBMIT_URI[] =  "/submit/";
// Это значение нужно менять с каждым релизом
// Чтобы хайскоры записывались в таблицу привязанную к этому релизу
// Чексуммы тоже будут привязаны к названию релиза
static const char HS_TABLE[] = "2012_winter_v12";

using namespace std;

bool score_locked = false;
char* answer = NULL;
map<int, pair<int, char*> > protected_variables;

void Highscores::LockScore()
{
	score_locked = true;
}

bool Highscores::GetScoreLock()
{
	return score_locked;
}

char* Highscores::GetAnswer()
{
	return answer;
}

void Highscores::Free()
{
	if (answer ) DELETEARRAY( answer );
	for ( map<int, pair<int, char*> >::iterator iter = protected_variables.begin(); iter != protected_variables.end(); ++iter )
	{
		DELETEARRAY( iter->second.second );
	}
}

// Функция, получающая с сервера список файлов для проверки
std::string hsGetValidationList()
{
	std::string content;

	map<string, string> params;
	params.insert(pair<string, string>("version", std::string(HS_TABLE)));

	bool send_result = http_send_request(HS_HOST, HS_VALIDATION_URI, content, "POST", NULL, &params);

	if(!send_result) {
		Log(DEFAULT_LOG_NAME, logLevelError, "Cannot get validation list: http_send_request failed");
		return "";
	}

	return content;
}

void SetAnswer(const char* new_answer)
{
	if ( answer ) DELETEARRAY( answer );
	answer = StrDupl( new_answer );
}

char* Highscores::GetScoreTable( int mode )
{
	std::string content;
	
	stringstream mode_string;
	mode_string << mode;

	map<string, string> params;
	params.insert(pair<string, string>("version", std::string(HS_TABLE)));
	params.insert(pair<string, string>("mode", mode_string.str()));

	bool send_result = http_send_request(HS_HOST, "/api/highscores/", content, "GET", NULL, &params);

	if(!send_result) {
		Log(DEFAULT_LOG_NAME, logLevelError, "Cannot get score table: http_send_request failed");
		return "ERROR_SEND";
	}

	char* result = StrDupl( content.c_str() );

	return result;
}

bool hsSendScore(const char* nickname, const int score, const int seconds, const int stat, const int mode, const char* char_name)
{
	if (score_locked)
	{
		SetAnswer("Your score is compromised");
		return false;
	}

	if(score < 0) {
		Log(DEFAULT_LOG_NAME, logLevelError, "Cannot send score: score must be positive or zero");
		SetAnswer("Cannot send score: score must be positive or zero");
		return false;
	}

	if(seconds < 0) {
		Log(DEFAULT_LOG_NAME, logLevelError, "Cannot send score: seconds must be positive");
		SetAnswer("Cannot send score: seconds must be positive");
		return false;
	}

	/*if(strlen(nickname) > HS_NICKNAME_MAX_LEN) {
		Log(DEFAULT_LOG_NAME, logLevelError, "Cannot send score: nickname is too long");
		SetAnswer("Cannot send score: nickname is too long");
		return false;
	}*/

#ifdef CHECKSUMS_ON
	// Получаем список файлов на валидацию
	// Будем считать crc32
	std::string validation_list = hsGetValidationList();
	stringstream stream (stringstream::in | stringstream::out);
	stream << validation_list;

	//char checksum_buffer[256];
	//std::string checksum_string;
	stringstream checksum (stringstream::in | stringstream::out);
	while(!stream.eof()) {
		string line;
		getline(stream, line);

		if(line.empty() || line == "\n")
			continue;

		uint32_t file_crc32 = crc32f(line.c_str());
		if(file_crc32 != 0xFFFFFFFF && file_crc32 != 0x00000000) {
			
			if(!checksum.str().empty())
				checksum << "|";

			checksum << line << ":" << file_crc32;
		}
	}
#endif

	stringstream token (stringstream::in | stringstream::out);
	token << nickname << "$$$";
	token << score << "$$$";
	token << seconds << "$$$";
	token << HS_TABLE << "$$$";
	token << char_name << "$$$";
	token << mode << "$$$";
	token << stat << "$$$";
#ifdef CHECKSUMS_ON
	token << checksum.str();
	//Log(DEFAULT_LOG_NAME, logLevelInfo,  checksum.str().c_str());
#endif

	string out;
	for (unsigned int x = 0; x < token.str().length(); x++) {
		out += token.str()[x] ^ HS_SALT[x % strlen(HS_SALT)];
	}
	string tmp = token.str();

	map<string, string> params;
	params.insert(pair<string, string>("token", out));
//	params.insert(pair<string, string>("up_rows", "100"));
//	params.insert(pair<string, string>("down_rows", "100"));

	std::string content;

	bool send_result = http_send_request(HS_HOST, HS_SUBMIT_URI, content, "POST", NULL, &params);
	
	if(!send_result) {
		SetAnswer( "Cannot send score: http_send_request failed" );
		Log(DEFAULT_LOG_NAME, logLevelError, "Cannot send score: http_send_request failed");
		return false;
	}

	SetAnswer( content.c_str() );

	/*if(content != "OK") {
		Log(DEFAULT_LOG_NAME, logLevelError, "Cannot send highscores: server response is %s", content.c_str());
		return false;
	}*/

	Log(DEFAULT_LOG_NAME, logLevelInfo, "Score succesfully sent");
	return true;
}

bool Highscores::ProtectedVariable( int num, int& new_value, bool modify )
{
	char* tmp;
	map<int, pair<int, char*> >::const_iterator it = protected_variables.find( num );
	if ( it != protected_variables.end() )
	{
		tmp = md5( it->second.first );
		if ( strcmp( tmp, it->second.second ) != 0 )
		{
			score_locked = true;
			return false;
		}
		DELETEARRAY( tmp );
	}
	if (modify)
	{
		if ( it != protected_variables.end() )
		{
			tmp = static_cast<char*>(it->second.second);
			DELETEARRAY( tmp );
		}
		protected_variables[num] = pair<int, char*>( new_value, md5(new_value) );
	}
	else
	{
		if ( it != protected_variables.end() )
		{
			new_value = it->second.first;
		}
		else
		{
			new_value = 0;
		}
	}
	return true;
}
