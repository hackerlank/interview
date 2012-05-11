#include "stdafx.h"
#include "data.h"
#include "misc.h"

#include "interpret.h"

enum KEYWORD { kwGuiDef, kwFor, kwWhile, kwDo, kwIf, kwStatement, kwNo };
const LPSTR keywords[] = { "GUIDEFINITION", "for", "while", "do", "if" };
const int k_l = 5;
// condition type, for parse record
enum CONDITION { LESS, LESSEQUAL, MORE, MOREEQUAL, NOCOND };

extern GAMEDEFINITION gamedef;

// struct, which we will take as result of parsing
// it contains information, which we should use to build
// execution code
struct PARSE_RECORD {
	KEYWORD key;
	int min;
	CONDITION cnd;
	int max;
	vector<string> what_to_do;

	PARSE_RECORD()
	{
		key = kwNo;
		min = 0;
		max = 0;
		cnd = NOCOND;
		what_to_do.resize(0);
	}
};


// *****************
// Interpretes Parse record




// *****************
// Loads any file
// Recognizes format
// Main load routine
// *****************
bool Load(char* file_name)
{
	// input stream
	ifstream f;
	// opening for input
	f.open(file_name, ios::in);

	// if not opened
	if(!f.is_open())
	{
		// logging about fail
		char output[50];
		sprintf(output, "Failed to open file: %s", file_name);
		LogToFile(output);
		// exit
		return false;
	}
	// if opened
	else
	{
		// logging success
		char output[50];
		sprintf(output, "Loading file: %s", file_name);
		LogToFile(output);

		int lines = 0;
		// string, consists body operation of script/file
		// e.g. 'for(i=0;i<5;i++)' or 'Load(1.txt);Load(2.txt);Load(3.txt);'
		// i need this, cause 'f >> s' stops when get %20 (space) symbol, so
		// instead of 'for(i=0;i<5;i++)' we will get
		// for
		// (
		// i
		// =
		// 0
		// ..etc..
 		string body;
		// our result
		PARSE_RECORD result;
		// while not end of file
		while(!f.eof())
		{
			// reading string
			string s;
			f >> s;

			lines++;
			// if empty, skip iteration
			if(s.empty())
				continue;

			// concattenating 'body' of script/file
			if(s.find('{') == s.npos && s.find('}') == s.npos)
				body += s;
			else
			{
				// body ready
#ifdef _DEBUG_INTERPRET
				LogToFile("body: " + body);
#endif
				// now we need to recognize body
				// here we go

				// where we save current keyword/lexem/syntax block
				// default: statement, it case we won't find any cycles
				KEYWORD key = kwStatement;
				int key_pos = body.npos;
				int cnd_pos;
				int cnd_pos1;
				int cnd;

				// starting analyzing all 'body' string,
				// deleting already analyzed substrings
				int k;

					// first of all we find keyword
					for(k = 0; k < k_l; k++)
					{
						key_pos = body.find(keywords[k]);
						if(key_pos != body.npos)
						{
							// found key
							key = KEYWORD(k);
							// deleting substr with key from body
							string substr = body.substr(key_pos, strlen(keywords[k]));

							break;
						}
					}
					
					// if we find key
					if(key_pos != body.npos)
					{
						// writing it to result
						result.key = key;

						// what is the key?
						switch(key)
						{
						case kwFor:
						case kwWhile:
							{
									// in case of cycle we also need limits
									// e.g (for i = 0; i < 5; i++) limits are 0 and 5
									// 'min' is a number between '=' and ';'
									// 'max' is a number between colon of comparison and ';'

									// so, searching for min
									int eq_pos = body.find('=');
									// if '=' not found, error
									if(eq_pos == body.npos)
									{
										sprintf(output, "Syntax error (line %u): '=' expected", lines);
										LogToFile(output);
										// exiting
										return false;
									}
									// if '=' found
									else
									{
										// searching for first semicolon
										int sc_pos = body.find(';');
										// if ';' not found, error
										if(sc_pos == body.npos)
										{
											sprintf(output, "Syntax error (line %u): ';' expected", lines);
											LogToFile(output);
											// exiting
											return false;
										}
										// if ';' found
										else
										{
											// making 'min' substring
											string min = body.substr(eq_pos + 1, body.length() - eq_pos  - (body.length() - sc_pos) - 1);
											// writing result
											result.min = atoi(min.c_str());

											// okay, now we need 'max'
											// first, find condition
											cnd_pos = body.npos;
											cnd_pos1 = body.npos;
											
											cnd_pos = body.find('<');
											// if not found '<', searching '>'
											if(cnd_pos == body.npos)
											{
												cnd_pos = body.find('>');
												// if still not found, error
												if(cnd_pos == body.npos)
												{
													sprintf(output, "Syntax error (line %u): '>' or '<' expected", lines);
													LogToFile(output);
													// exiting
													return false;
												}
												// now check if there is +equal condition
												cnd_pos1 = body.find('=', cnd_pos);
												if(cnd_pos1 != body.npos)
												{
													//result.cnd = MOREEQUAL;
													cnd = 3;
													result.cnd = CONDITION(cnd);
												}
												else
												{
													cnd = 2;
													result.cnd = CONDITION(cnd);
												}
											}
											else
											{
												cnd_pos1 = body.find('=', cnd_pos);
												if(cnd_pos1 != body.npos)
												{
													cnd = 1;
													result.cnd = CONDITION(cnd);
												}
												else
												{
													cnd = 0;
													result.cnd = CONDITION(cnd);
												}
											}
										}
									
										// okay, now we know condition
										// search for max
										//    searching for second semicolon
										int sc_pos2 = body.find(';', cnd_pos);
										// if not found semicolon, error
										if(sc_pos2 == body.npos)
										{
											sprintf(output, "Syntax error (line %u): ';' expected", lines);
											LogToFile(output);
											// exiting
											return false;
										}										
										// if found semicolon
										else
										{
											int _dx;
											cnd_pos1 != body.npos ? _dx = 1 : _dx = 0;

											// making 'max' substring
											string max = body.substr(cnd_pos + _dx + 1, body.length() - (cnd_pos + _dx + 1)  - (body.length() - cnd_pos1) - 1);

											// writing result
											result.max = atoi(max.c_str());
										}
									}
									break;
								}
								case kwGuiDef:
									{
										gamedef.gui.LoadFromFile(file_name);
										break;
									}
								default: break;
							}

#ifdef _DEBUG_INTERPRET
							char buffer[100];
							itoa(k, buffer, 10);
							LogToFile("PARSE RECORD::key = " + string(buffer));
							itoa(result.min, buffer, 10);
							LogToFile("PARSE RECORD::min = " + string(buffer));
							itoa(result.max, buffer, 10);
							LogToFile("PARSE RECORD::max = " + string(buffer));
							itoa(result.cnd, buffer, 10);
							LogToFile("PARSE RECORD::cnd = " + string(buffer));
#endif
						}
						// if we not found key
						// this is a statement
						else
						{
							if(result.key == kwNo)
								result.key = kwStatement;

							// while body exists
							do
							{
								// searching for semicolon
								int sc_pos = body.find(';');
								// making operation string
								string op = body.substr(0, sc_pos);
								// adding operation to what-to-do list
								result.what_to_do.push_back(op);

								// erasing this substring to take new one
								body.erase(0, sc_pos+1);
							}							
							while(body.length() > 0);

						}




								
				// zeroing both input string and body
				s.resize(0);
				body.resize(0);

			}
		}

		// is there a loop?
		if(result.key == kwFor)
		{
			for(int i = result.min; i < result.max; i++)
			{
				for(std::vector<string>::iterator it = result.what_to_do.begin();
					it!= result.what_to_do.end(); it++)
				{
#ifdef _DEBUG_INTERPRET
					LogToFile("PARSE RECORD::what-to-do = " + string((*it)));
#endif
					string s = (*it);
					// here will be command check
					if(s.find("Load") != s.npos)
					{
						// this is loading file
						// it maybe anything - gui description file, script, etc..
						int p1 = s.npos;
						int p2 = s.npos;

						p1 = s.find("(");
						p2 = s.find(")");

						if(p1 == s.npos || p2 == s.npos)
						{
							sprintf(output, "Syntax error in %s: '(' or ')' expected", s);
							LogToFile(output);
						}
						else
						{
							string what_to_load;
							string final;
							// what to load, for example if string is
							// Load(1.txt); then what to load is
							// 1.txt
							what_to_load = s.substr(p1+1, p2-p1-1);
#ifdef _DEBUG_INTERPRET
							LogToFile("What-to-load = " + what_to_load);
#endif
							// there maybe Load(%i.txt) variant, so
							// we should search for '%'

							int pr = s.npos;

							pr = what_to_load.find('%');
							if(pr != what_to_load.npos)
							{
								final = what_to_load.substr(p1+2, pr-p1-1);

								char buf[100];
								itoa(i, buf, 10);
								final += string(buf);
								final += what_to_load.substr(pr+2, p2-pr);

#ifdef _DEBUG_INTERPRET
								LogToFile("Final = " + final);
#endif
								Load((char*)final.c_str());
							}
							else
							{
								Load((char*)what_to_load.c_str());
							}
						}
					}
				}
			}								
		}
		else
		{
				for(std::vector<string>::iterator it = result.what_to_do.begin();
					it!= result.what_to_do.end(); it++)
				{
#ifdef _DEBUG_INTERPRET
					LogToFile("PARSE RECORD::what-to-do = " + string((*it)));
#endif
					string s = (*it);
					// here will be command check
					if(s.find("Load") != s.npos)
					{
						// this is loading file
						// it maybe anything - gui description file, script, etc..
						int p1 = s.npos;
						int p2 = s.npos;

						p1 = s.find("(");
						p2 = s.find(")");

						if(p1 == s.npos || p2 == s.npos)
						{
							sprintf(output, "Syntax error in %s: '(' or ')' expected", s);
							LogToFile(output);
						}
						else
						{
							string what_to_load;
							string final;
							// what to load, for example if string is
							// Load(1.txt); then what to load is
							// 1.txt
							what_to_load = s.substr(p1+1, p2-p1-1);
#ifdef _DEBUG_INTERPRET
							LogToFile("What-to-load = " + what_to_load);
#endif
							// there maybe Load(%i.txt) variant, so
							// we should search for '%'

							int pr = s.npos;

							pr = what_to_load.find('%');
							if(pr != what_to_load.npos)
							{
								sprintf(output, "Syntax error in %s: '%' unexpected", what_to_load);
								LogToFile(output);
							}
							else
							{
								Load((char*)what_to_load.c_str());
							}
						}
					}
				}
		}
		return true;
	}
}

			
