#pragma once 

#ifdef EZ_USEMFC
// for CString
#include <afxwin.h> 
#else
#include <Windows.h>  
#endif

namespace EZ{
	class CStrConv
	{
	public:
		CStrConv(void);
		~CStrConv(void);
	public:
		static char* utf162utf8(const wchar_t* utf16);
		static void  utf162utf8(const wchar_t* utf16,char* utf8);//?const char* or char*

		static wchar_t* utf82utf16(const char* utf8);
		static void     utf82utf16(const char* utf8, wchar_t* utf16);

		static char* utf162ansi(const wchar_t* utf16);
		static void  utf162ansi(const wchar_t* utf16,char* ansi);

		static wchar_t* ansi2utf16(const char* ansi);
		static void     ansi2utf16(const char* ansi,wchar_t* utf16);

		static char* utf82ansi(const char* utf8);
		static void	 utf82ansi(const char* utf8,char* ansi);

		static char* ansi2utf8(const char* ansi);
		static void  ansi2utf8(const char* ansi,char* utf8);
#ifdef EZ_USEMFC
		// for CString conversiton have to include files
		// note for unicode projects only!!
		// note release buffer manually after using!
		static char* cstring2ansi(CString &acstr);
		static char* cstring2utf8(CString &acstr);
		static wchar_t* cstring2utf16(CString &acstr);
#endif
		// for debug
		static void printbyte(byte* bt);
	};
}


