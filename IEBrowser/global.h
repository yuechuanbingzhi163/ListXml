#pragma once

#ifndef _GLOBAL_H
#define _GLOBAL_H

#define __DUILIB__

/************************************************************************/
/* 定义全局函数、成员                                                   */
/************************************************************************/

#include "MainFrame.h"

#define SEMAPHORE_MAINWND	 _T("duilib_mainframewnd")



extern CMainFrame*  g_pMainFrame;


//当前EXE文件的路径
extern  CString							g_szCurPath;
extern  CString							g_szLogPath;//日志文件路径	


//编码转换
extern char*	G_UnicodeToUtf8( const wchar_t* str	);
extern wchar_t* G_Utf8ToUnicode( const char* pUtf8	);

extern wchar_t* G_AnsiToUnicode( const char* str	);
extern char*	G_UnicodeToAnsi( const wchar_t* str	);


//* @brief : 获取当前EXE所在的路径 
extern CString  G_GetCurrentPath();
//获取当前EXE所在的路径 下的文件路径
extern CString  G_GetCurrentFullPath(CString strPathName);

extern void G_DbgWrite(TCHAR * v_szFormat, ...);


#endif