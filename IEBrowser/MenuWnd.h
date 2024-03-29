#pragma once
#ifndef __MENUWND_H__
#define __MENUWND_H__


inline HBITMAP CreateMyBitmap(HDC hDC, int cx, int cy, COLORREF** pBits)
{
	LPBITMAPINFO lpbiSrc = NULL;
	lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
	if (lpbiSrc == NULL) return NULL;

	lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbiSrc->bmiHeader.biWidth = cx;
	lpbiSrc->bmiHeader.biHeight = cy;
	lpbiSrc->bmiHeader.biPlanes = 1;
	lpbiSrc->bmiHeader.biBitCount = 32;
	lpbiSrc->bmiHeader.biCompression = BI_RGB;
	lpbiSrc->bmiHeader.biSizeImage = cx * cy;
	lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
	lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
	lpbiSrc->bmiHeader.biClrUsed = 0;
	lpbiSrc->bmiHeader.biClrImportant = 0;

	HBITMAP hBitmap = CreateDIBSection (hDC, lpbiSrc, DIB_RGB_COLORS, (void **)pBits, NULL, NULL);
	delete [] lpbiSrc;
	return hBitmap;
}

class CShadowWnd : public CWindowWnd
{
private:
	bool m_bNeedUpdate;
	CPaintManagerUI m_pm;

public:
	CShadowWnd() { };
	LPCTSTR GetWindowClassName() const { return _T("UIShadow"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };

	void RePaint() 
	{
		RECT rcClient = { 0 };
		::GetClientRect(m_hWnd, &rcClient);
		DWORD dwWidth = rcClient.right - rcClient.left;
		DWORD dwHeight = rcClient.bottom - rcClient.top;

		HDC hDcPaint = ::GetDC(m_hWnd);
		HDC hDcBackground = ::CreateCompatibleDC(hDcPaint);
		COLORREF* pBackgroundBits;
		HBITMAP hbmpBackground = CreateMyBitmap(hDcPaint, dwWidth, dwHeight, &pBackgroundBits);
		::ZeroMemory(pBackgroundBits, dwWidth * dwHeight * 4);
		HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hDcBackground, hbmpBackground);
		CRenderEngine::DrawImageString(hDcBackground, &m_pm, rcClient, rcClient, _T("file='menu_bk.png' corner='40,8,8,8'"), NULL);

		RECT rcWnd = { 0 };
		::GetWindowRect(m_hWnd, &rcWnd);

		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		POINT ptPos   = { rcWnd.left, rcWnd.top };
		SIZE sizeWnd  = { dwWidth, dwHeight };
		POINT ptSrc   = { 0, 0 };
		UpdateLayeredWindow(m_hWnd, hDcPaint, &ptPos, &sizeWnd, hDcBackground, &ptSrc, 0, &bf, ULW_ALPHA);

		::SelectObject(hDcBackground, hOldBitmap);
		if( hDcBackground != NULL ) ::DeleteDC(hDcBackground);
		if( hbmpBackground != NULL ) ::DeleteObject(hbmpBackground);
		::ReleaseDC(m_hWnd, hDcPaint);

		m_bNeedUpdate = false;
	}

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if( uMsg == WM_CREATE ) SetTimer(m_hWnd, 9000, 10, NULL);
		else if( uMsg == WM_SIZE ) m_bNeedUpdate = true;
		else if( uMsg == WM_CLOSE ) KillTimer(m_hWnd, 9000);
		else if( uMsg == WM_TIMER ) {
			if( LOWORD(wParam) == 9000 && m_bNeedUpdate == true) {
				if( !::IsIconic(m_hWnd) ) RePaint();
			}
		}

		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}
};

class CMenuWnd : public CXMLWnd
{
public:
	CMenuWnd(LPCTSTR v_pStrSkinPath);
	~CMenuWnd();

	void Init(CControlUI* pOwner, POINT pt);
	void AdjustPostion();

	/*
	**@brief: 初始化
	*/
	LPCTSTR GetWindowClassName() const
	{
		return _T("#32770 (Dialog)");
	}

	CDuiString GetSkinFolder()
	{
		return _T("skin");
	}

	CDuiString GetSkinFile()
	{
		return m_pStrSkinPath;
	}

	/*
	**@brief：处理事件通知
	*/
	void OnFinalMessage(HWND hWnd);
	void Notify(TNotifyUI& msg);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CControlUI* m_pOwner;
	POINT m_ptPos;
	CDuiString m_pStrSkinPath;
	CShadowWnd* m_pShadowWnd;
};

#endif /// end __MENUWND_H__

