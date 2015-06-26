#include "stdafx.h"
#include "MenuWnd.h"

CMenuWnd::CMenuWnd(LPCTSTR v_pStrSkinPath) : CXMLWnd(v_pStrSkinPath), m_pOwner(NULL), m_pShadowWnd(NULL)
{
	m_pStrSkinPath = v_pStrSkinPath;

}

CMenuWnd::~CMenuWnd()
{
}

void CMenuWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CMenuWnd::Init(CControlUI* pOwner, POINT pt)
{
	if( pOwner == NULL ) return;

	m_pOwner = pOwner;
	m_ptPos = pt;
	Create(pOwner->GetManager()->GetPaintWindow(), NULL, WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
	HWND hWndParent = m_hWnd;
	while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
	AdjustPostion();
	::ShowWindow(m_hWnd, SW_SHOW);
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

void CMenuWnd::AdjustPostion()
{
	CDuiRect rcWnd;
	GetWindowRect(m_hWnd, &rcWnd);
	int nWidth = rcWnd.GetWidth();
	int nHeight = rcWnd.GetHeight();
	rcWnd.left = m_ptPos.x;
	rcWnd.top = m_ptPos.y;
	rcWnd.right = rcWnd.left + nWidth;
	rcWnd.bottom = rcWnd.top + nHeight;
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;

	if( rcWnd.bottom > rcWork.bottom ) {
		if( nHeight >= rcWork.GetHeight() ) {
			rcWnd.top = 0;
			rcWnd.bottom = nHeight;
		}
		else {
			rcWnd.bottom = rcWork.bottom;
			rcWnd.top = rcWnd.bottom - nHeight;
		}
	}
	if( rcWnd.right > rcWork.right ) {
		if( nWidth >= rcWork.GetWidth() ) {
			rcWnd.left = 0;
			rcWnd.right = nWidth;
		}
		else {
			rcWnd.right = rcWork.right;
			rcWnd.left = rcWnd.right - nWidth;
		}
	}
	::SetWindowPos(m_hWnd, NULL, rcWnd.left, rcWnd.top, rcWnd.GetWidth(), rcWnd.GetHeight(), SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

void CMenuWnd::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("itemselect") ) 
	{
		Close();
	}
	else if( msg.sType == _T("itemclick") ) 
	{
		if( m_pOwner ) m_pOwner->GetManager()->SendNotify(msg.pSender, _T("MenuNotify"), 0, 0, true);
// 		if( msg.pSender->GetName() == _T("menu_Delete") ) 
// 		{
// 			if( m_pOwner ) m_pOwner->GetManager()->SendNotify(m_pOwner, _T("menu_Delete"), 0, 0, true);
// 		}
	}
}


LRESULT CMenuWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_pShadowWnd != NULL ) 
		m_pShadowWnd->Close();
	bHandled = FALSE;

	return 0;
}

LRESULT CMenuWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( (HWND)wParam == m_hWnd ) 
		bHandled = TRUE;
	else if( m_pShadowWnd != NULL && (HWND)wParam == m_pShadowWnd->GetHWND() ) 
	{
		CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		::SetFocus( m_hWnd );
		bHandled = TRUE;
	}
	else 
	{
		Close();
		bHandled = FALSE;
	}

	return 0;
}

LRESULT CMenuWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( wParam == VK_ESCAPE ) Close();

	return 0;
}

LRESULT CMenuWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( !::IsIconic(*this) ) 
	{
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		HRGN hRgn = ::CreateRectRgn(rcWnd.left + 8, rcWnd.top + 8, rcWnd.right - 8, rcWnd.bottom - 8);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
	if( m_pShadowWnd != NULL ) 
	{
		RECT rcWnd = { 0 };
		::GetWindowRect(m_hWnd, &rcWnd );
		::SetWindowPos(*m_pShadowWnd, m_hWnd, rcWnd.left, rcWnd.top, rcWnd.right -rcWnd.left,
			rcWnd.bottom -rcWnd.top, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	}

	bHandled = FALSE;

	return 0;
}

LRESULT CMenuWnd::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_pShadowWnd != NULL ) 
	{
		RECT rcWnd = { 0 };
		::GetWindowRect(m_hWnd, &rcWnd );
		::SetWindowPos(*m_pShadowWnd, m_hWnd, rcWnd.left, rcWnd.top, rcWnd.right -rcWnd.left,
			rcWnd.bottom -rcWnd.top, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
	}

	bHandled = FALSE;
	
	return 0;
}

LRESULT CMenuWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	switch( uMsg ) 
	{
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_KILLFOCUS:     lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break; 
	case WM_KEYDOWN:       lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEWHEEL:    break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_MOVE:          lRes = OnMove(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}

	if( bHandled ) return lRes;
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}