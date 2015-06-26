/**********************************************************************************************************************
/*	<?xml version="1.0" encoding="utf-8"?>
/*	<Controls>
/*		<VerSlidePage parent="VerticalLayout" notifies="setfocus killfocus timer menu windowinit(root)">
/*			<!--VerSlidePage 私有属性-->
/*			<Attribute name="automove" default="true" type="BOOL" comment="是否当停止移动时，页面自动移动到正确的位置,如(true)"/>
/*			<Attribute name="speed" default="100" type="INT" comment="自动移动时每次刷新移动的步长,最小间隔20MS,如(50)"/>
/*			<Attribute name="selectid" default="0" type="INT" comment="默认选择的页面,如(1)"/>
/*			<Attribute name="distance" default="0" type="INT" comment="子页移动多少距离后翻页，为0时自动选择最当前在屏幕最多的子页面,如(100)"/>
/*			<Attribute name="alignpoint" default="topleft" type="STRING" comment="子页面自动对齐的点,值有：topleft,topright,topcenter,bottomleft,bottomright,bottomcenter,leftcenter,rightcenter,center"/>
/*		</VerSlidePage>
/*		<HorSlidePage parent="HorizontalLayout" notifies="setfocus killfocus timer menu windowinit(root)">
/*			<!--HorSlidePage 私有属性-->
/*			<Attribute name="automove" default="true" type="BOOL" comment="是否当停止移动时，页面自动移动到正确的位置,如(true)"/>
/*			<Attribute name="speed" default="100" type="INT" comment="自动移动时每次刷新移动的步长,最小间隔20MS,如(50)"/>
/*			<Attribute name="selectid" default="0" type="INT" comment="默认选择的页面,如(1)"/>
/*			<Attribute name="distance" default="0" type="INT" comment="子页移动多少距离后翻页，为0时自动选择最当前在屏幕最多的子页面,如(100)"/>
/*			<Attribute name="alignpoint" default="topleft" type="STRING" comment="子页面自动对齐的点,值有：topleft,topright,topcenter,bottomleft,bottomright,bottomcenter,leftcenter,rightcenter,center"/>
/*		</HorSlidePage>
/*		<SlidePageItem parent="Container" notifies="setfocus killfocus timer menu windowinit(root)">
/*			<!--SlidePageItem 私有属性-->
/*			<Attribute name="leftmove" default="true" type="BOOL" comment="允许向左拖动,如(false)"/>
/*			<Attribute name="rightmove" default="true" type="BOOL" comment="允许向右拖动,如(false)"/>
/*			<Attribute name="topmove" default="true" type="BOOL" comment="允许向上拖动,如(false)"/>
/*			<Attribute name="bottommove" default="true" type="BOOL" comment="允许向下拖动,如(false)"/>
/*			<Attribute name="move" default="true" type="BOOL" comment="允许拖动,如(false)"/>
/*		</SlidePageItem>
/*	</Controls>
**********************************************************************************************************************/


#ifndef __UISLIDEPAGE_H__
#define __UISLIDEPAGE_H__

#pragma once


namespace DuiLib
{
	class ISlidePageItemUI : public CContainerUI
	{
	public:
		ISlidePageItemUI();
		virtual ~ISlidePageItemUI();
		virtual void SetPos(RECT rc);
		
		virtual void Move(int nX, int nY);
	protected:
		int			m_nX;
		int			m_nY;
	};

	template<typename T_Parent>
	class CSlidePageUI : public T_Parent
	{
	protected:
		enum ALIGN_POINT
		{
			TOP_LEFT,
			TOP_RIGHT,
			BOTTOM_LEFT,
			BOTTOM_RIGHT,
			TOP_CENTER,
			BOTTOM_CENTER,
			LEFT_CENTER,
			RIGHT_CENTER,
			CENTER,
		};
		typedef std::vector<DuiLib::ISlidePageItemUI*>	VEC_PAGE;
		typedef VEC_PAGE::iterator						VEC_PAGE_ITER;
	public:
		CSlidePageUI() : m_pThisPage(NULL)//默认属性
			, m_pNextPage(NULL)
			, m_nSpeed(100)
			, m_nDefautPage(0)
			, m_bAutoMove(true)
			, m_bMoveNextPage( false )
			, m_nMaxDistanceToNext(0)
			, m_eAlignPoint(TOP_LEFT)
			, m_dwProAutoMoveTime(0)
		{

		}
		~CSlidePageUI()
		{

		}

		LPCTSTR GetClass() const
		{
			return _T("CSlidePageUI");
		}
		LPVOID GetInterface(LPCTSTR pstrName)
		{
			if( _tcscmp(pstrName, _T("CSlidePageUI")) == 0 ) return static_cast<CSlidePageUI*>(this);
			return __super::GetInterface(pstrName);
		}

		void DoInit()
		{
			T_Parent::DoInit();
			for (int i = 0; i < GetCount(); ++i)
			{
				ISlidePageItemUI* pPage = (ISlidePageItemUI*)GetItemAt(i);
				m_vecPage.push_back(pPage);
			}
			m_pThisPage = m_vecPage[static_cast<UINT>(m_nDefautPage) < m_vecPage.size() ? m_nDefautPage : 0];
		}

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			if (_tcscmp(pstrName, _T("speed")) == 0)
			{
				LPTSTR pstr = NULL;
				m_nSpeed = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			}
			else if (_tcscmp(pstrName, _T("selectid")) == 0)
			{
				LPTSTR pstr = NULL;
				m_nDefautPage = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			}
			else if (_tcscmp(pstrName, _T("automove")) == 0)
			{
				m_bAutoMove = _tcscmp(pstrValue, _T("true")) == 0;
			}
			else if (_tcscmp(pstrName, _T("automovenextpage")) == 0)
			{
				m_bMoveNextPage = _tcscmp(pstrValue, _T("true")) == 0;
			}
			else if (_tcscmp(pstrName, _T("distance")) == 0)
			{
				LPTSTR pstr = NULL;
				m_nMaxDistanceToNext = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			}
			else if (_tcscmp(pstrName, _T("alignpoint")) == 0)
			{
				if (_tcscmp(pstrValue, _T("topleft")) == 0)
					m_eAlignPoint = TOP_LEFT;
				else if (_tcscmp(pstrValue, _T("topright")) == 0)
					m_eAlignPoint = TOP_RIGHT;
				else if (_tcscmp(pstrValue, _T("topcenter")) == 0)
					m_eAlignPoint = TOP_CENTER;
				else if (_tcscmp(pstrValue, _T("bottomleft")) == 0)
					m_eAlignPoint = BOTTOM_LEFT;
				else if (_tcscmp(pstrValue, _T("bottomright")) == 0)
					m_eAlignPoint = BOTTOM_RIGHT;
				else if (_tcscmp(pstrValue, _T("bottomcenter")) == 0)
					m_eAlignPoint = BOTTOM_CENTER;
				else if (_tcscmp(pstrValue, _T("leftcenter")) == 0)
					m_eAlignPoint = LEFT_CENTER;
				else if (_tcscmp(pstrValue, _T("rightcenter")) == 0)
					m_eAlignPoint = RIGHT_CENTER;
				else if (_tcscmp(pstrValue, _T("center")) == 0)
					m_eAlignPoint = CENTER;
			}
			else
				T_Parent::SetAttribute(pstrName, pstrValue);
		}

		// 移动到子页面
		void SelectItem(UINT unIndex)
		{
			if (unIndex < m_vecPage.size())
			{
				m_pNextPage = m_vecPage[unIndex];
				if(m_bMoveNextPage)
				{
					MoveNextPage();
				}
				Invalidate();
			}
		}
		// 移动到子页面
		int GetItemIndex()
		{
			return std::find(m_vecPage.begin(), m_vecPage.end(), m_pThisPage)-m_vecPage.begin();
		}

		// 移动页面
		void Move(int x, int y)
		{
			for (VEC_PAGE_ITER iter = m_vecPage.begin(); iter != m_vecPage.end(); ++iter)
			{
				ISlidePageItemUI* pPage = *iter;
				pPage->Move(x, y);
			}
		}

		// 移动完毕
		void MoveOver()
		{
			m_pNextPage = m_nMaxDistanceToNext ? NextPageWhileMove(m_nMaxDistanceToNext) : FindRightPage();
			Invalidate();
		}

		void SetMoveNextPage(bool v_bMoveNextPage)
		{
			m_bMoveNextPage = v_bMoveNextPage;
		}
		bool GetIsMoveNextPage()
		{
			return m_bMoveNextPage;
		}
		// 直接移动到下一页之前，跳过中间页
		void MoveNextPage()
		{
			if (m_pNextPage)
			{
				CDuiRect rcPage = m_pNextPage->GetPos();
				CDuiRect rcClient = GetPos();
				int nxMoveLen = 0;
				int nyMoveLen = 0;
				if (AlignPoint(rcPage, rcClient, nxMoveLen, nyMoveLen))
				{
					m_pThisPage = m_pNextPage;
					m_pNextPage = NULL;
					m_dwProAutoMoveTime = 0;
					return;
				}
				int nX = (nxMoveLen == 0) ? 0 : (abs(nxMoveLen) - rcClient.GetWidth());
				int nY = (nyMoveLen == 0) ? 0 : (abs(nyMoveLen) - rcClient.GetHeight());
				if(nX > 0)
				{
					nX = nxMoveLen > 0 ? nX : -nX;
				}
				if(nY > 0)
				{
					nY = nyMoveLen > 0 ? nY : -nY;
				}
				Move(nX, nY);
			}
		}

		void DoPaint(HDC hDC, const RECT& rcPaint)
		{
			if (m_nDefautPage && static_cast<UINT>(m_nDefautPage) < m_vecPage.size())
			{
				T_Parent::DoPaint(hDC, rcPaint);
				m_pNextPage = m_vecPage[m_nDefautPage];
				m_nDefautPage = 0;
				AutoMove(-1);
			}
			if (m_pNextPage)
			{
				if (::GetTickCount() - m_dwProAutoMoveTime > 20)
					AutoMove(m_nSpeed);
				Invalidate();
			}
			T_Parent::DoPaint(hDC, rcPaint);
		}

protected:
		// X和Y轴上自动移动最合适的模块到可见区域
		void AutoMove(int nSpeed)
		{
			m_dwProAutoMoveTime = ::GetTickCount();
			if (!m_bAutoMove && !m_nDefautPage)
				m_pNextPage = NULL;
			if (m_pNextPage)
			{
				CDuiRect rcPage = m_pNextPage->GetPos();
				CDuiRect rcClient = GetPos();
				int nxMoveLen = 0;
				int nyMoveLen = 0;
				if (AlignPoint(rcPage, rcClient, nxMoveLen, nyMoveLen))
				{
					m_pThisPage = m_pNextPage;
					m_pNextPage = NULL;
					m_dwProAutoMoveTime = 0;
					return;
				}
				int nX = nxMoveLen;
				int nY = nyMoveLen;
				if (nSpeed > 0)
				{
					nX = abs(nxMoveLen) > nSpeed ? nxMoveLen > 0 ? nSpeed : -nSpeed : nxMoveLen;
					nY = abs(nyMoveLen) > nSpeed ? nyMoveLen > 0 ? nSpeed : -nSpeed : nyMoveLen;
				}
				Move(nX, nY);
			}
		}

		// 查找当前在可见区中占用最多的页面
		CControlUI* FindRightPage()
		{
			CControlUI* pRightPage = m_vecPage[0];
			for (UINT i = 1; i < m_vecPage.size(); ++i)
			{
				unsigned long ulRightPage = CalInClientSize(pRightPage);
				unsigned long ulThisPage = CalInClientSize(m_vecPage[i]);
				if (ulThisPage > ulRightPage)
					pRightPage = m_vecPage[i];
			}
			return pRightPage;
		}

		// 如果页面移动超过多少距离则翻页
		CControlUI* NextPageWhileMove(int nMaxMoveDistance)
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
				else if (abs(nY) > nMaxMoveDistance)
				{
					if (nY < 0)
						return iter == m_vecPage.begin() ? *iter : *(iter - 1);
					else if (nY > 0)
						return (iter + 1) == m_vecPage.end() ? *iter : *(iter + 1);
				}
			}
			return m_pThisPage;
		}

		// 计算当前模块在可见客户区中的面积
		unsigned long CalInClientSize(CControlUI* pPage) const
		{
			CSize size;
			CDuiRect rcPage = pPage->GetPos();
			CDuiRect rcClient = GetPos();
			CDuiRect rcIntersect;
			rcIntersect.IntersectRect(rcPage, rcClient);

			return (unsigned long)rcIntersect.GetWidth() * (unsigned long) rcIntersect.GetHeight();
		}

		// 对齐点
		bool AlignPoint(const CDuiRect& rcPage, const CDuiRect& rcClient, int& nxMoveLen, int& nyMoveLen)
		{
			switch (m_eAlignPoint)
			{
			case TOP_LEFT:
				if ( (rcPage.left == rcClient.left) && (rcPage.top == rcClient.top) )
					return true;
				nxMoveLen = rcClient.left - rcPage.left;
				nyMoveLen = rcClient.top - rcPage.top;
				return false;
			case TOP_RIGHT:
				if (rcPage.top == rcClient.top && rcPage.right == rcClient.right)
					return true;
				nxMoveLen = rcClient.right - rcPage.right;
				nyMoveLen = rcClient.top - rcPage.top;
				return false;
			case TOP_CENTER:
				if (rcPage.top == rcClient.top && rcPage.left + rcPage.GetWidth() / 2 == rcClient.left + rcClient.GetWidth() / 2)
					return true;
				nxMoveLen = (rcClient.left + rcClient.GetWidth() / 2) - (rcPage.left + rcPage.GetWidth() / 2);
				nyMoveLen = rcClient.top - rcPage.top;
				return false;
			case BOTTOM_LEFT:
				if (rcPage.bottom == rcClient.bottom && rcPage.left == rcClient.left)
					return true;
				nxMoveLen = rcClient.left - rcPage.left;
				nyMoveLen = rcClient.bottom - rcPage.bottom;
				return false;
			case BOTTOM_RIGHT:
				if ( (rcPage.bottom == rcClient.bottom) && (rcPage.right == rcClient.right) )
					return true;
				nxMoveLen = rcClient.right - rcPage.right;
				nyMoveLen = rcClient.bottom - rcPage.bottom;
				return false;
			case BOTTOM_CENTER:
				if (rcPage.bottom == rcClient.bottom && rcPage.left + rcPage.GetWidth() / 2 == rcClient.left + rcClient.GetWidth() / 2)
					return true;
				nxMoveLen = (rcClient.left + rcClient.GetWidth() / 2) - (rcPage.left + rcPage.GetWidth() / 2);
				nyMoveLen = rcClient.bottom - rcPage.bottom;
				return false;
			case LEFT_CENTER:
				if (rcPage.left == rcClient.left && rcPage.top + rcPage.GetHeight() / 2 == rcClient.top + rcClient.GetHeight() / 2)
					return true;
				nxMoveLen = rcClient.left - rcPage.left;
				nyMoveLen = (rcClient.top + rcClient.GetHeight() / 2) - (rcPage.top + rcPage.GetHeight() / 2);
				return false;
			case RIGHT_CENTER:
				if (rcPage.right == rcClient.right && rcPage.top + rcPage.GetHeight() / 2 == rcClient.top + rcClient.GetHeight() / 2)
					return true;
				nxMoveLen = rcClient.right - rcPage.right;
				nyMoveLen = (rcClient.top + rcClient.GetHeight() / 2) - (rcPage.top + rcPage.GetHeight() / 2);
				return false;
			case CENTER:
				if ( ((rcPage.left + rcPage.GetWidth()/2) == (rcClient.left + rcClient.GetWidth())) && ((rcPage.top + rcPage.GetHeight()) == (rcClient.top + rcClient.GetHeight())) )
					return true;
				nxMoveLen = (rcClient.left + rcClient.GetWidth() / 2) - (rcPage.left + rcPage.GetWidth() / 2);
				nyMoveLen = (rcClient.top + rcClient.GetHeight() / 2) - (rcPage.top + rcPage.GetHeight() / 2);
				return false;
			}
			return false;
		}

// 		CSlidePageUI();
// 		virtual ~CSlidePageUI();
// 		virtual void DoInit();
// 		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
// 		// 移动到子页面
// 		virtual void SelectItem(UINT unIndex);
// 		// 移动页面
// 		virtual void Move(int x, int y);
// 		// 移动完毕
// 		virtual void MoveOver();
// 		virtual void DoPaint(HDC hDC, const RECT& rcPaint);
// 	protected:
// 		// X和Y轴上自动移动最合适的模块到可见区域
// 		virtual void AutoMove(int nSpeed);
// 		// 查找当前在可见区中占用最多的页面
// 		virtual CControlUI* FindRightPage();
// 		// 如果页面移动超过多少距离则翻页
// 		virtual CControlUI* NextPageWhileMove(int nMaxMoveDistance);
// 		// 计算当前模块在可见客户区中的面积
// 		virtual unsigned long CalInClientSize(CControlUI* pPage) const;
// 		// 对齐点
// 		virtual bool AlignPoint(const CDuiRect& rcPage, const CDuiRect& rcClient, int& nxMoveLen, int& nyMoveLen);
	protected:
		VEC_PAGE			m_vecPage;				// 保存所有页面的指针
		CControlUI*			m_pThisPage;			// 保存当前显示的页面
		CControlUI*			m_pNextPage;			// 保存下一页面
		int					m_nSpeed;				// 自动移动的速度
		int					m_nDefautPage;			// 默认的页面
		bool				m_bAutoMove;			// 是否自动移动到合适的位置
		bool				m_bMoveNextPage;		// 是否直接跳到下一页之前
		int					m_nMaxDistanceToNext;	// 移动多少距离后翻页
		ALIGN_POINT			m_eAlignPoint;			// 自动移动时对齐到哪一个点
		DWORD				m_dwProAutoMoveTime;	// 上一次移动的时间
	};

	class CHorSlidePageUI : public CSlidePageUI<CHorizontalLayoutUI>
	{
	public:
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
	protected:
		// 如果页面移动超过多少距离则翻页
		virtual CControlUI* NextPageWhileMove(int nMaxMoveDistance);
	};

	class CVerSlidePageUI : public CSlidePageUI<CVerticalLayoutUI>
	{
	public:
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
	protected:
		// 如果页面移动超过多少距离则翻页
		virtual CControlUI* NextPageWhileMove(int nMaxMoveDistance);
	};

	class CSlidePageItemUI : public ISlidePageItemUI
	{
	public:
		CSlidePageItemUI();
		virtual ~CSlidePageItemUI();
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void DoEvent(DuiLib::TEventUI& event);
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	protected:
		virtual void LBottonDown(TEventUI& event);
		virtual void LBottonUp(TEventUI& event);
		virtual void MouseMove(TEventUI& event);
	protected:
		bool		m_bDrag;
		bool		m_bLeftMove;
		bool		m_bRightMove;
		bool		m_bTopMove;
		bool		m_bBottomMove;
		CPoint		m_ptOldMouse;
	};
}

#endif //__UISLIDEPAGE_H__