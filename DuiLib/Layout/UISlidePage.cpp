#include "stdafx.h"
#include "UISlidePage.h"


namespace DuiLib
{
	ISlidePageItemUI::ISlidePageItemUI(): m_nX(0), m_nY(0)
	{

	}
	ISlidePageItemUI::~ISlidePageItemUI()
	{

	}
	void ISlidePageItemUI::SetPos(RECT rc)
	{
		CDuiRect rcNew(rc);
		rcNew.Offset(m_nX, m_nY);
		CContainerUI::SetPos(rcNew);
	}
	void ISlidePageItemUI::Move(int nX, int nY)
	{
		m_nX += nX;
		m_nY += nY;
		CDuiRect rcMove = GetPos();
		rcMove.Offset(nX, nY);
		CContainerUI::SetPos(rcMove);
	}

//  这边实现放这里有问题T_Parent会解析有问题，找不到命名空间
// 	template<typename T_Parent>
// 	CSlidePageUI<T_Parent>::CSlidePageUI() : m_pThisPage(NULL)
// 			, m_pNextPage(NULL)
// 			, m_nSpeed(100)
// 			, m_nDefautPage(0)
// 			, m_bAutoMove(true)
// 			, m_nMaxDistanceToNext(0)
// 			, m_eAlignPoint(TOP_LEFT)
// 			, m_dwProAutoMoveTime(0)
// 	{
// 
// 	}
// 	template<typename T_Parent>
// 	CSlidePageUI<T_Parent>::~CSlidePageUI()
// 	{
// 
// 	}
// 	template<typename T_Parent>
// 	void CSlidePageUI<typename T_Parent>::DoInit()
// 	{
// 		T_Parent::DoInit();
// 		for (int i = 0; i < GetCount(); ++i)
// 		{
// 			ISlidePageItemUI* pPage = (ISlidePageItemUI*)GetItemAt(i);
// 			m_vecPage.push_back(pPage);
// 		}
// 		m_pThisPage = m_vecPage[static_cast<UINT>(m_nDefautPage) < m_vecPage.size() ? m_nDefautPage : 0];
// 	}
// 
// 	template<typename T_Parent>
// 	void CSlidePageUI<typename T_Parent>::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
// 	{
// 		if (_tcscmp(pstrName, _T("speed")) == 0)
// 		{
// 			LPTSTR pstr = NULL;
// 			m_nSpeed = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
// 		}
// 		else if (_tcscmp(pstrName, _T("selectid")) == 0)
// 		{
// 			LPTSTR pstr = NULL;
// 			m_nDefautPage = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
// 		}
// 		else if (_tcscmp(pstrName, _T("automove")) == 0)
// 		{
// 			m_bAutoMove = _tcscmp(pstrValue, _T("true")) == 0;
// 		}
// 		else if (_tcscmp(pstrName, _T("distance")) == 0)
// 		{
// 			LPTSTR pstr = NULL;
// 			m_nMaxDistanceToNext = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
// 		}
// 		else if (_tcscmp(pstrName, _T("alignpoint")) == 0)
// 		{
// 			if (_tcscmp(pstrValue, _T("topleft")) == 0)
// 				m_eAlignPoint = TOP_LEFT;
// 			else if (_tcscmp(pstrValue, _T("topright")) == 0)
// 				m_eAlignPoint = TOP_RIGHT;
// 			else if (_tcscmp(pstrValue, _T("topcenter")) == 0)
// 				m_eAlignPoint = TOP_CENTER;
// 			else if (_tcscmp(pstrValue, _T("bottomleft")) == 0)
// 				m_eAlignPoint = BOTTOM_LEFT;
// 			else if (_tcscmp(pstrValue, _T("bottomright")) == 0)
// 				m_eAlignPoint = BOTTOM_RIGHT;
// 			else if (_tcscmp(pstrValue, _T("bottomcenter")) == 0)
// 				m_eAlignPoint = BOTTOM_CENTER;
// 			else if (_tcscmp(pstrValue, _T("leftcenter")) == 0)
// 				m_eAlignPoint = LEFT_CENTER;
// 			else if (_tcscmp(pstrValue, _T("rightcenter")) == 0)
// 				m_eAlignPoint = RIGHT_CENTER;
// 			else if (_tcscmp(pstrValue, _T("center")) == 0)
// 				m_eAlignPoint = CENTER;
// 		}
// 		else
// 			T_Parent::SetAttribute(pstrName, pstrValue);
// 	}
// 
// 	// 移动到子页面
// 	template<typename T_Parent>
// 	void CSlidePageUI<typename T_Parent>::SelectItem(UINT unIndex)
// 	{
// 		if (unIndex < m_vecPage.size())
// 		{
// 			m_pNextPage = m_vecPage[unIndex];
// 			Invalidate();
// 		}
// 	}
// 		
// 	// 移动页面
// 	template<typename T_Parent>
// 	void CSlidePageUI<typename T_Parent>::Move(int x, int y)
// 	{
// 		for (VEC_PAGE_ITER iter = m_vecPage.begin(); iter != m_vecPage.end(); ++iter)
// 		{
// 			ISlidePageItemUI* pPage = *iter;
// 			pPage->Move(x, y);
// 		}
// 	}
// 
// 	// 移动完毕
// 	template<typename T_Parent>
// 	void CSlidePageUI<typename T_Parent>::MoveOver()
// 	{
// 		m_pNextPage = m_nMaxDistanceToNext ? NextPageWhileMove(m_nMaxDistanceToNext) : FindRightPage();
// 		Invalidate();
// 	}
// 		
// 	template<typename T_Parent>
// 	void CSlidePageUI<typename T_Parent>::DoPaint(HDC hDC, const RECT& rcPaint)
// 	{
// 		if (m_nDefautPage && static_cast<UINT>(m_nDefautPage) < m_vecPage.size())
// 		{
// 			T_Parent::DoPaint(hDC, rcPaint);
// 			m_pNextPage = m_vecPage[m_nDefautPage];
// 			m_nDefautPage = 0;
// 			AutoMove(-1);
// 		}
// 		if (m_pNextPage)
// 		{
// 			if (::GetTickCount() - m_dwProAutoMoveTime > 20)
// 				AutoMove(m_nSpeed);
// 			Invalidate();
// 		}
// 		T_Parent::DoPaint(hDC, rcPaint);
// 	}
// 	
// 	// X和Y轴上自动移动最合适的模块到可见区域
// 	template<typename T_Parent>
// 	void CSlidePageUI<typename T_Parent>::AutoMove(int nSpeed)
// 	{
// 		m_dwProAutoMoveTime = ::GetTickCount();
// 		if (!m_bAutoMove && !m_nDefautPage)
// 			m_pNextPage = NULL;
// 		if (m_pNextPage)
// 		{
// 			CDuiRect rcPage = m_pNextPage->GetPos();
// 			CDuiRect rcClient = GetPos();
// 			int nxMoveLen = 0;
// 			int nyMoveLen = 0;
// 			if (AlignPoint(rcPage, rcClient, nxMoveLen, nyMoveLen))
// 			{
// 				m_pThisPage = m_pNextPage;
// 				m_pNextPage = NULL;
// 				m_dwProAutoMoveTime = 0;
// 				return;
// 			}
// 			int nX = nxMoveLen;
// 			int nY = nyMoveLen;
// 			if (nSpeed > 0)
// 			{
// 				nX = abs(nxMoveLen) > nSpeed ? nxMoveLen > 0 ? nSpeed : -nSpeed : nxMoveLen;
// 				nY = abs(nyMoveLen) > nSpeed ? nyMoveLen > 0 ? nSpeed : -nSpeed : nyMoveLen;
// 			}
// 			Move(nX, nY);
// 		}
// 	}
// 		
// 	// 查找当前在可见区中占用最多的页面
// 	template<typename T_Parent>
// 	CControlUI* CSlidePageUI<typename T_Parent>::FindRightPage()
// 	{
// 		CControlUI* pRightPage = m_vecPage[0];
// 		for (UINT i = 1; i < m_vecPage.size(); ++i)
// 		{
// 			unsigned long ulRightPage = CalInClientSize(pRightPage);
// 			unsigned long ulThisPage = CalInClientSize(m_vecPage[i]);
// 			if (ulThisPage > ulRightPage)
// 				pRightPage = m_vecPage[i];
// 		}
// 		return pRightPage;
// 	}
// 
// 		// 如果页面移动超过多少距离则翻页
// 	template<typename T_Parent>
// 	CControlUI* CSlidePageUI<typename T_Parent>::NextPageWhileMove(int nMaxMoveDistance)
// 	{
// 		if (m_pThisPage)
// 		{
// 			VEC_PAGE_ITER iter = std::find(m_vecPage.begin(), m_vecPage.end(), m_pThisPage);
// 			CDuiRect rcClient = GetPos();
// 			CDuiRect rcPage= m_pThisPage->GetPos();
// 			int nX = 0, nY = 0;
// 			AlignPoint(rcPage, rcClient, nX, nY);
// 			if (abs(nX) > nMaxMoveDistance)
// 			{
// 				if (nX < 0)
// 					return iter == m_vecPage.begin() ? *iter : *(iter - 1);
// 				else if (nX > 0)
// 					return (iter + 1) == m_vecPage.end() ? *iter : *(iter + 1);
// 			}
// 			else if (abs(nY) > nMaxMoveDistance)
// 			{
// 				if (nY < 0)
// 					return iter == m_vecPage.begin() ? *iter : *(iter - 1);
// 				else if (nY > 0)
// 					return (iter + 1) == m_vecPage.end() ? *iter : *(iter + 1);
// 			}
// 		}
// 		return m_pThisPage;
// 	}
// 		
// 	// 计算当前模块在可见客户区中的面积
// 	template<typename T_Parent>
// 	unsigned long CSlidePageUI<typename T_Parent>::CalInClientSize(CControlUI* pPage) const
// 	{
// 		CSize size;
// 		CDuiRect rcPage = pPage->GetPos();
// 		CDuiRect rcClient = GetPos();
// 		CDuiRect rcIntersect;
// 		rcIntersect.IntersectRect(rcPage, rcClient);
// 
// 		return (unsigned long)rcIntersect.GetWidth() * (unsigned long) rcIntersect.GetHeight();
// 	}
// 
// 		// 对齐点
// 	template<typename T_Parent>
// 	bool CSlidePageUI<typename T_Parent>::AlignPoint(const CDuiRect& rcPage, const CDuiRect& rcClient, int& nxMoveLen, int& nyMoveLen)
// 	{
// 		switch (m_eAlignPoint)
// 		{
// 		case TOP_LEFT:
// 			if ( (rcPage.left == rcClient.left) && (rcPage.top == rcClient.top) )
// 				return true;
// 			nxMoveLen = rcClient.left - rcPage.left;
// 			nyMoveLen = rcClient.top - rcPage.top;
// 			return false;
// 		case TOP_RIGHT:
// 			if (rcPage.top == rcClient.top && rcPage.right == rcClient.right)
// 				return true;
// 			nxMoveLen = rcClient.right - rcPage.right;
// 			nyMoveLen = rcClient.top - rcPage.top;
// 			return false;
// 		case TOP_CENTER:
// 			if (rcPage.top == rcClient.top && rcPage.left + rcPage.GetWidth() / 2 == rcClient.left + rcClient.GetWidth() / 2)
// 				return true;
// 			nxMoveLen = (rcClient.left + rcClient.GetWidth() / 2) - (rcPage.left + rcPage.GetWidth() / 2);
// 			nyMoveLen = rcClient.top - rcPage.top;
// 			return false;
// 		case BOTTOM_LEFT:
// 			if (rcPage.bottom == rcClient.bottom && rcPage.left == rcClient.left)
// 				return true;
// 			nxMoveLen = rcClient.left - rcPage.left;
// 			nyMoveLen = rcClient.bottom - rcPage.bottom;
// 			return false;
// 		case BOTTOM_RIGHT:
// 			if ( (rcPage.bottom == rcClient.bottom) && (rcPage.right == rcClient.right) )
// 				return true;
// 			nxMoveLen = rcClient.right - rcPage.right;
// 			nyMoveLen = rcClient.bottom - rcPage.bottom;
// 			return false;
// 		case BOTTOM_CENTER:
// 			if (rcPage.bottom == rcClient.bottom && rcPage.left + rcPage.GetWidth() / 2 == rcClient.left + rcClient.GetWidth() / 2)
// 				return true;
// 			nxMoveLen = (rcClient.left + rcClient.GetWidth() / 2) - (rcPage.left + rcPage.GetWidth() / 2);
// 			nyMoveLen = rcClient.bottom - rcPage.bottom;
// 			return false;
// 		case LEFT_CENTER:
// 			if (rcPage.left == rcClient.left && rcPage.top + rcPage.GetHeight() / 2 == rcClient.top + rcClient.GetHeight() / 2)
// 				return true;
// 			nxMoveLen = rcClient.left - rcPage.left;
// 			nyMoveLen = (rcClient.top + rcClient.GetHeight() / 2) - (rcPage.top + rcPage.GetHeight() / 2);
// 			return false;
// 		case RIGHT_CENTER:
// 			if (rcPage.right == rcClient.right && rcPage.top + rcPage.GetHeight() / 2 == rcClient.top + rcClient.GetHeight() / 2)
// 				return true;
// 			nxMoveLen = rcClient.right - rcPage.right;
// 			nyMoveLen = (rcClient.top + rcClient.GetHeight() / 2) - (rcPage.top + rcPage.GetHeight() / 2);
// 			return false;
// 		case CENTER:
// 			if ( ((rcPage.left + rcPage.GetWidth()/2) == (rcClient.left + rcClient.GetWidth())) && ((rcPage.top + rcPage.GetHeight()) == (rcClient.top + rcClient.GetHeight())) )
// 				return true;
// 			nxMoveLen = (rcClient.left + rcClient.GetWidth() / 2) - (rcPage.left + rcPage.GetWidth() / 2);
// 			nyMoveLen = (rcClient.top + rcClient.GetHeight() / 2) - (rcPage.top + rcPage.GetHeight() / 2);
// 			return false;
// 		}
// 		return false;
// 	}
	
	LPCTSTR CHorSlidePageUI::GetClass() const
	{
		return DUI_CTR_HORSLIDEPAGE;
	}
	LPVOID CHorSlidePageUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_HORSLIDEPAGE) == 0 ) return static_cast<CHorSlidePageUI*>(this);
		return __super::GetInterface(pstrName);
	}
	

		// 如果页面移动超过多少距离则翻页
	CControlUI* CHorSlidePageUI::NextPageWhileMove(int nMaxMoveDistance)
	{
		if (m_pThisPage)
		{
			VEC_PAGE_ITER iter = std::find(m_vecPage.begin(), m_vecPage.end(), m_pThisPage);
			CDuiRect rcClient = GetPos();
			CDuiRect rcPage= m_pThisPage->GetPos();
			int nX = 0, nY = 0;
			AlignPoint(rcPage, rcClient, nX, nY);
			if (abs(nX) > nMaxMoveDistance)
			{
				if (nX < 0)
					return iter == m_vecPage.begin() ? *iter : *(iter - 1);
				else if (nX > 0)
					return (iter + 1) == m_vecPage.end() ? *iter : *(iter + 1);
			}
		}
		return m_pThisPage;
	}


	LPCTSTR CVerSlidePageUI::GetClass() const
	{
		return DUI_CTR_VERSLIDEPAGE;
	}
	LPVOID CVerSlidePageUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_VERSLIDEPAGE) == 0 ) return static_cast<CVerSlidePageUI*>(this);
		return __super::GetInterface(pstrName);
	}
	
		// 如果页面移动超过多少距离则翻页
	CControlUI* CVerSlidePageUI::NextPageWhileMove(int nMaxMoveDistance)
	{
		if (m_pThisPage)
		{
			VEC_PAGE_ITER iter = std::find(m_vecPage.begin(), m_vecPage.end(), m_pThisPage);
			CDuiRect rcClient = GetPos();
			CDuiRect rcPage= m_pThisPage->GetPos();
			int nX = 0, nY = 0;
			AlignPoint(rcPage, rcClient, nX, nY);
			if (abs(nY) > nMaxMoveDistance)
			{
				if (nY < 0)
					return iter == m_vecPage.begin() ? *iter : *(iter - 1);
				else if (nY > 0)
					return (iter + 1) == m_vecPage.end() ? *iter : *(iter + 1);
			}
		}
		return m_pThisPage;
	}

	CSlidePageItemUI::CSlidePageItemUI()
			: m_bDrag(false)
			, m_bLeftMove(true)
			, m_bRightMove(true)
			, m_bTopMove(true)
			, m_bBottomMove(true)
	{

	}
	CSlidePageItemUI::~CSlidePageItemUI()
	{

	}
	LPCTSTR CSlidePageItemUI::GetClass() const
	{
		return DUI_CTR_SLIDEPAGEITEM;
	}

	LPVOID CSlidePageItemUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_SLIDEPAGEITEM) == 0 ) return static_cast<CSlidePageItemUI*>(this);
		return ISlidePageItemUI::GetInterface(pstrName);
	}

	void CSlidePageItemUI::DoEvent(DuiLib::TEventUI& event)
	{
		switch (event.Type)
		{
		case UIEVENT_BUTTONDOWN:
			LBottonDown(event);
			return;
		case UIEVENT_BUTTONUP:
			LBottonUp(event);
			return;
		case UIEVENT_MOUSEMOVE:
			MouseMove(event);
			return;
		}
		CContainerUI::DoEvent(event);
	}

	void CSlidePageItemUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("leftmove")) == 0)
		{
			m_bLeftMove = _tcscmp(pstrValue, _T("true")) == 0;
		}
		else if (_tcscmp(pstrName, _T("rightmove")) == 0)
		{
			m_bRightMove = _tcscmp(pstrValue, _T("true")) == 0;
		}
		else if (_tcscmp(pstrName, _T("topmove")) == 0)
		{
			m_bTopMove = _tcscmp(pstrValue, _T("true")) == 0;
		}
		else if (_tcscmp(pstrName, _T("bottommove")) == 0)
		{
			m_bBottomMove = _tcscmp(pstrValue, _T("true")) == 0;
		}
		else if (_tcscmp(pstrName, _T("move")) == 0)
		{
			m_bLeftMove = _tcscmp(pstrValue, _T("true")) == 0;
			m_bRightMove = _tcscmp(pstrValue, _T("true")) == 0;
			m_bTopMove = _tcscmp(pstrValue, _T("true")) == 0;
			m_bBottomMove = _tcscmp(pstrValue, _T("true")) == 0;
		}
		else
			CContainerUI::SetAttribute(pstrName, pstrValue);
	}

	void CSlidePageItemUI::LBottonDown(TEventUI& event)
	{
		m_bDrag = true;
		m_ptOldMouse = event.ptMouse;
		CDuiString strPage = GetName();
		CDuiString strParent = GetParent()->GetName();
		LPCTSTR lpParentClass = GetParent()->GetClass();
		return;
	}

	void CSlidePageItemUI::LBottonUp(TEventUI& event)
	{
		if (m_bDrag)
		{
			m_bDrag = false;
			CControlUI* pContrl = (CControlUI*)GetParent();
			if (pContrl)
			{
				if (_tcscmp(pContrl->GetClass(), DUI_CTR_HORSLIDEPAGE) == 0)
					((CHorSlidePageUI*)pContrl)->MoveOver();
				else if (_tcscmp(pContrl->GetClass(), DUI_CTR_VERSLIDEPAGE) == 0)
					((CVerSlidePageUI*)pContrl)->MoveOver();
			}
		}
	}

	void CSlidePageItemUI::MouseMove(TEventUI& event)
	{
		if (m_bDrag)
		{
			int xDrag = event.ptMouse.x - m_ptOldMouse.x;
			int yDrag = event.ptMouse.y - m_ptOldMouse.y;
			if (!m_bLeftMove && xDrag < 0) xDrag = 0;
			if (!m_bRightMove && xDrag > 0) xDrag = 0;
			if (!m_bTopMove && yDrag < 0) yDrag = 0;
			if (!m_bBottomMove && yDrag > 0) yDrag = 0;
			// 如果鼠标按下的情况下移动，则为拖动
			if (m_bDrag)
			{
				CControlUI* pContrl = (CControlUI*)GetParent();
				LPCTSTR lpClass = pContrl->GetClass();
				if (pContrl)
				{
					if (_tcscmp(pContrl->GetClass(), DUI_CTR_HORSLIDEPAGE) == 0)
						((CHorSlidePageUI*)pContrl)->Move(xDrag, 0);
					else if (_tcscmp(pContrl->GetClass(), DUI_CTR_VERSLIDEPAGE) == 0)
						((CVerSlidePageUI*)pContrl)->Move(0, yDrag);
				}
			}
		}
		m_ptOldMouse = event.ptMouse;
	}

}