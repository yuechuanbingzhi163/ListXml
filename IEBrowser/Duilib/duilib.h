
#pragma once

#include "Resource.h"

#include "../DuiLib/UIlib.h"
using namespace DuiLib;

#ifdef __STATIC_LIB__
#	ifdef _DEBUG
#		ifdef _UNICODE
#			pragma comment(lib, "../Lib/DuiLib_sud.lib")
#		else
#			pragma comment(lib, "../Lib/DuiLib_sd.lib")
#		endif
#	else
#		ifdef _UNICODE
#			pragma comment(lib, "../Lib/DuiLib_su.lib")
#		else
#			pragma comment(lib, "../Lib/DuiLib_s.lib")
#		endif
#	endif
#else
#	ifdef _DEBUG
#		ifdef _UNICODE
#			pragma comment(lib, "../Lib/DuiLib_ud.lib")
#		else
#			pragma comment(lib, "../Lib/DuiLib_d.lib")
#		endif
#	else
#		ifdef _UNICODE
#			pragma comment(lib, "../Lib/DuiLib_u.lib")
#		else
#			pragma comment(lib, "../Lib/DuiLib.lib")
#		endif
#	endif
#endif



// 以XML生成界面的窗口基类
class CXMLWnd : public WindowImplBase
{
public:
    explicit CXMLWnd(LPCTSTR pszXMLName) 
        : m_strXMLName(pszXMLName){}

public:
    virtual LPCTSTR GetWindowClassName() const
    {
        return _T("POMELOUPDATE");
    }

    virtual CDuiString GetSkinFile()
	{
        return m_strXMLName;
    }

    virtual CDuiString GetSkinFolder()
    {
        return _T("");
    }

	virtual UILIB_RESOURCETYPE GetResourceType() const
	{
#ifdef __STATIC_LIB__
		return UILIB_ZIPRESOURCE;
#else
		return UILIB_FILE;
#endif
	}

	virtual LPCTSTR GetResourceID() const
	{
		return MAKEINTRESOURCE(IDR_ZIPRES_SKIN);
	}

protected:
    CDuiString m_strXMLName;    // XML的名字
};