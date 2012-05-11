#include "StdAfx.h"
#include "proto.h"

map<string, Proto*> prototypes;

// ��������� �������� � map
void AddProto(char* name, Proto* proto)
{
	if (name && proto)
	{
		prototypes[string(name)] = proto;
	}
}

// ������� �������� �� ��������� � map'�, ����������� ��� ������� ���������� ������
void RemoveProto(map<string, Proto*>::iterator it)
{
	Proto* proto = it->second;
	DELETESINGLE(proto);
}

// ������� �������� �� �����
void RemoveProto(char* name)
{
	if (name)
	{
		map<string, Proto*>::iterator it = prototypes.find(name);
		if (it->first == string(name))
		{
			RemoveProto(it);
			prototypes.erase(it);
		}
	}
}

// ������� ��� ��������� ��������
void RemoveAllProto()
{
	for(map<string, Proto*>::iterator it = prototypes.begin();
		it != prototypes.end();
		it++)
	{
		RemoveProto(it);
	}
	prototypes.clear();
}

//////////////////////////////////////////////////////////////////////////

Proto* GetProto(string proto_name)
{
	map<string, Proto*>::iterator it = prototypes.find(proto_name);
	if (it != prototypes.end())
		return it->second;
	else
		return NULL;
}
