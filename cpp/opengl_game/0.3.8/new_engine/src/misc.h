#ifndef __MISC_H_
#define __MISC_H_

<<<<<<< .mine


//void InitPaths(void);
//void CheckPaths();
//void CleanupLogs();
=======
//////////////////////////////////////////////////////////////////////////
// ��� ��������� �� ��������� ������ ����������������� � ������� ����� ���������
//#define DEBUG_STOP_ON_ALLOC_NUM 1119

//////////////////////////////////////////////////////////////////////////

>>>>>>> .r575
void EndupLogs();

void CheckDefaultConfig();
char* ExtFromPath(const char* path);
char* GetNameFromPath(const char* path);

void CreateLogFile(char *fname);
void sLog(const char* fname, const char* Event, const char* Format,...);

/**
*	������ ���� ��������� ���. ��� �� ��������. TODO!
*	�� �� � ��� ����� ������� ��� � �-� Log � �� ������ ������ ��������� ��������.
*/

class CDummy
{
public:
	CDummy();
	~CDummy();
};


#define CFILE_READ				0x01
#define CFILE_WRITE				0x02
#define CFILE_MAX_STRING_LENGTH	1024

typedef unsigned char		byte;
typedef byte*				pbyte;

class CFile
{
public:
	FILE *file;

	CFile(void) : file(NULL){}
	CFile(char *filename, int mode);

	bool Open(const char * filename, int mode);
	bool Close();
	bool ReadByte(pbyte buffer);
	bool Read(void* buffer, DWORD nbytes);
	bool WriteByte(pbyte buffer);
	bool WriteByte(byte buffer);
	bool Write(const void* buffer, DWORD nbytes);
	bool ReadString(char* buffer);
	bool WriteString(const char* buffer);
	bool ReadString(string &buffer);
	bool WriteString(const string buffer);
	bool Writeln(string buffer);
	DWORD Size();
	/**
	*	Reads characters until reaching 0x00
	*	data - is a pointer to a char passed by
	*	link(&) i.e. the function changes the value
	*	of data. If data is pointing to something,
	*	funtion realeses the memory using delete []
	*	then allocates new meory using new.
	*/
	// ������ ����������� ����������� �� ������� ����������. ������� ����� �� ����������� ��� � ����.
	bool ReadLine(char* &data);
	bool Eof();
	bool Seek(unsigned int offset, byte kind);
};


//////////////////////////////////////////////////////////////////////////

// �������� �� ��, ��� ��� ����� � ��������� ����� ������ ���� � ����� � ����������� ���������. 
// float � �������, ����� ���� 1e-35, ��� ����� 0, �� �� 0. ��� �� ����� �������� ������ ����� �� �����.
__INLINE bool IsNear(float a, float b, float accuracy)
{
	return fabs(a-b) < accuracy;
}

#endif // __MISC_H_