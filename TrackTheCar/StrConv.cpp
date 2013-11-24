
#include "StdAfx.h"
#include "StrConv.h"
using namespace std;

EZ::CStrConv::CStrConv(void)
{
}

EZ::CStrConv::~CStrConv(void)
{
}

#pragma region stringconvertion
char* EZ::CStrConv::utf162utf8(const wchar_t* utf16)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, NULL, 0, 0, 0);
	if (len>1)
	{ 
		char* utf8 = new char[len+1];
		memset(utf8,0,len+1);
		WideCharToMultiByte(CP_UTF8, 0, utf16, -1, utf8, len, 0, 0);
		return utf8;
	}
	return NULL;
}

void EZ::CStrConv::utf162utf8(const wchar_t* utf16,char* utf8)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, NULL, 0, 0, 0);
	if (len>1)
	{ 
		utf8 = new char[len+1];
		memset(utf8,0,len+1);
		WideCharToMultiByte(CP_UTF8, 0, utf16, -1, utf8, len, 0, 0);
		return;
	}
	utf8 = NULL; // can't ....
}

wchar_t* EZ::CStrConv::utf82utf16(const char* utf8)
{

	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	if (len>1)
	{ 
		wchar_t *utf16 = new wchar_t[len+1];
		memset(utf16,0,len+1);
		MultiByteToWideChar(CP_UTF8, 0, utf8, -1, utf16, len);
		return utf16;
	}
	return NULL;
}

void EZ::CStrConv::utf82utf16(const char* utf8, wchar_t* utf16)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	if (len>1)
	{ 
		utf16 = new wchar_t[len+1];
		memset(utf16,0,len+1);
		MultiByteToWideChar(CP_UTF8, 0, utf8, -1, utf16, len);
		return;
	}
	utf16 = NULL;
}

char* EZ::CStrConv::utf162ansi(const wchar_t* utf16)
{
	int len = WideCharToMultiByte(CP_ACP,0,utf16,-1,NULL,0,0,0); // "i am 二货" len是10.。。怎么会是10?
	if(len > 1)
	{
		char* ansi = new char[len+1];
		memset(ansi,0,len+1);
		WideCharToMultiByte(CP_ACP,0,utf16,-1,ansi,len,0,0);
		return ansi;
	}
	return NULL;
}


void EZ::CStrConv::utf162ansi(const wchar_t* utf16,char* ansi)
{
	int len = WideCharToMultiByte(CP_ACP,0,utf16,-1,NULL,0,0,0); // "i am 二货" len是10.。。怎么会是10?
	if(len > 1)
	{
		ansi = new char[len+1];
		memset(ansi,0,len+1);
		WideCharToMultiByte(CP_ACP,0,utf16,-1,ansi,len,0,0);
		return;
	}
	ansi = NULL;
}

wchar_t* EZ::CStrConv::ansi2utf16(const char* ansi)
{
	int len = MultiByteToWideChar (CP_ACP, 0, ansi, -1, NULL, 0);  // "i am 二货" len是8
	if(len>1)
	{
		wchar_t* utf16 = new wchar_t[len+1];
		memset(utf16,0,len+1);
		MultiByteToWideChar (CP_ACP, 0 , ansi, -1, utf16, len); 
		return utf16;
	}
	return NULL;
}

void EZ::CStrConv::ansi2utf16(const char* ansi,wchar_t* utf16)
{
	int len = MultiByteToWideChar (CP_ACP, 0, ansi, -1, NULL, 0);  // "i am 二货" len是8
	if(len>1)
	{
		utf16 = new wchar_t[len+1];
		memset(utf16,0,len+1);
		MultiByteToWideChar (CP_ACP, 0 , ansi, -1, utf16, len); 
		return;
	}
	utf16 = NULL;
}

char* EZ::CStrConv::utf82ansi(const char* utf8)
{
	wchar_t *t_16 = utf82utf16(utf8);
	if( t_16 == NULL)
	{
		return NULL;
	}
	char* re = utf162ansi(t_16);
	delete t_16;
	return re;
}

void EZ::CStrConv::utf82ansi(const char* utf8,char* ansi)
{
	wchar_t *t_16 = utf82utf16(utf8);
	if( t_16 == NULL)
	{
		ansi = NULL;
	}
	ansi = utf162ansi(t_16);
	delete t_16;
	return;
}

char* EZ::CStrConv::ansi2utf8(const char* utf8)
{
	wchar_t* t_16 = utf82utf16(utf8);
	if( t_16 == NULL)
	{
		return NULL;
	}
	char* re = utf162utf8(t_16);
	delete t_16;
	return re;
}

void EZ::CStrConv::ansi2utf8(const char* ansi,char* utf8)
{
	wchar_t* t_16 = utf82utf16(utf8);
	if( t_16 == NULL)
	{
		utf8 = NULL;
	}
	utf8 = utf162utf8(t_16);
	delete t_16;
	return;
}

#ifdef EZ_USEMFC
// you have to release the cstring buffer by yourself
char* EZ::CStrConv::cstring2ansi(CString &acstr)
{
#ifdef _UNICODE
	wchar_t* pw = acstr.GetBuffer(acstr.GetLength());
	return utf162ansi(pw);
#else
	char* pc = acstr.GetBuffer(acstr.GetLength());
	return pc;
#endif

}

// you have to release the cstring buffer by yourself
// and delete the pointer because we new it in the ansi2tuf8 function
char* EZ::CStrConv::cstring2utf8(CString &acstr)
{
#ifdef _UNICODE
	wchar_t* pw = acstr.GetBuffer(acstr.GetLength());
	return utf162utf8(pw);
#else
	char* pc = acstr.GetBuffer(acstr.GetLength());
	return ansi2utf8(pc);
#endif
}

wchar_t* EZ::CStrConv::cstring2utf16(CString &acstr)
{
#ifdef _UNICODE
	wchar_t* pw = acstr.GetBuffer(acstr.GetLength());
	return pw;
#else
	char* pc = acstr.GetBuffer(acstr.GetLength());
	return ansi2utf16(pc);
#endif
}
#endif

#pragma endregion

// for debug
void EZ::CStrConv::printbyte(byte* bt)
{
	char* s = (char*)bt;
	int len = strlen(s);
	for(int i=0; i<len; i++)
	{
		printf("%02x",bt[i]);
	}
	printf("\n");
}
