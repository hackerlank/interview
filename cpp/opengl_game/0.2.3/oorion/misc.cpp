#include "stdafx.h"
#include "misc.h"

string NameFromPath(string path)
{
	string ret = "";

	for(int i = path.length(); i > 0; i--)
		if(path[i] == '.')
			for(int j = i-1; path[j] != 92; j--)
				ret.push_back(path[j]);

	for(int z = 0; z < ret.length() / 2; z++)
	{
		char temp;

		temp = ret[z];
		ret[z] = ret[ret.length()-z-1];
		ret[ret.length()-z-1] = temp;
	}

	return ret;
}

// TODO: fix it (doesnt work)
char* NameFromPath(char path[MAX_PATH])
{

	char temp[MAX_PATH];
	char ret[MAX_PATH];

	strcpy(temp, path);

	strrev(temp);

	int l = strlen(temp);

	for(int i = 0; i < l; i++)
		if(temp[i] == '.')
			for (int j = i+1; temp[j] != 92; j++)
				ret[j-i-1] = temp[j];

	return ret;
}


string IntToString(int intValue)
{ 
	char *myBuff; 
	string strRetVal;

	// Create a new char array 
	myBuff = new char[100]; 

	// Set it to empty 
	memset(myBuff,'\0',100); 

	// Convert to string 
	itoa(intValue,myBuff,10); 

	// Copy the buffer into the string object 
	strRetVal = myBuff; 
   
	// Delete the buffer 
	delete[] myBuff; 

	return(strRetVal); 
}

bool FileExists(const std::string& fileName)
{
  std::fstream fin;
  fin.open(fileName.c_str(),std::ios::in);
  if(!fin.fail())
  {
    fin.close();
    return true;
  }
  fin.close();
  return false;
}

void SetLength(LPCVOID array, UINT new_length)
{
	void* temp_array = new LPCVOID[new_length];
	memcpy(temp_array, array, sizeof(array));
	array = temp_array;	
}