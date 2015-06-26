/**********************************************************************************************************************
/*	<?xml version="1.0" encoding="utf-8"?>
/*	<Controls>
/*		<VerSlidePage parent="VerticalLayout" notifies="setfocus killfocus timer menu windowinit(root)">
/*			<!--VerSlidePage ˽������-->
/*			<Attribute name="automove" default="true" type="BOOL" comment="�Ƿ�ֹͣ�ƶ�ʱ��ҳ���Զ��ƶ�����ȷ��λ��,��(true)"/>
/*			<Attribute name="speed" default="100" type="INT" comment="�Զ��ƶ�ʱÿ��ˢ���ƶ��Ĳ���,��С���20MS,��(50)"/>
/*			<Attribute name="selectid" default="0" type="INT" comment="Ĭ��ѡ���ҳ��,��(1)"/>
/*			<Attribute name="distance" default="0" type="INT" comment="��ҳ�ƶ����پ����ҳ��Ϊ0ʱ�Զ�ѡ���ǰ����Ļ������ҳ��,��(100)"/>
/*			<Attribute name="alignpoint" default="topleft" type="STRING" comment="��ҳ���Զ�����ĵ�,ֵ�У�topleft,topright,topcenter,bottomleft,bottomright,bottomcenter,leftcenter,rightcenter,center"/>
/*		</VerSlidePage>
/*		<HorSlidePage parent="HorizontalLayout" notifies="setfocus killfocus timer menu windowinit(root)">
/*			<!--HorSlidePage ˽������-->
/*			<Attribute name="automove" default="true" type="BOOL" comment="�Ƿ�ֹͣ�ƶ�ʱ��ҳ���Զ��ƶ�����ȷ��λ��,��(true)"/>
/*			<Attribute name="speed" default="100" type="INT" comment="�Զ��ƶ�ʱÿ��ˢ���ƶ��Ĳ���,��С���20MS,��(50)"/>
/*			<Attribute name="selectid" default="0" type="INT" comment="Ĭ��ѡ���ҳ��,��(1)"/>
/*			<Attribute name="distance" default="0" type="INT" comment="��ҳ�ƶ����پ����ҳ��Ϊ0ʱ�Զ�ѡ���ǰ����Ļ������ҳ��,��(100)"/>
/*			<Attribute name="alignpoint" default="topleft" type="STRING" comment="��ҳ���Զ�����ĵ�,ֵ�У�topleft,topright,topcenter,bottomleft,bottomright,bottomcenter,leftcenter,rightcenter,center"/>
/*		</HorSlidePage>
/*		<SlidePageItem parent="Container" notifies="setfocus killfocus timer menu windowinit(root)">
/*			<!--SlidePageItem ˽������-->
/*			<Attribute name="leftmove" default="true" type="BOOL" comment="���������϶�,��(false)"/>
/*			<Attribute name="rightmove" default="true" type="BOOL" comment="���������϶�,��(false)"/>
/*			<Attribute name="topmove" default="true" type="BOOL" comment="���������϶�,��(false)"/>
/*			<Attribute name="bottommove" default="true" type="BOOL" comment="���������϶�,��(false)"/>
/*			<Attribute name="move" default="true" type="BOOL" comment="�����϶�,��(false)"/>
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
		CSlidePageUI() : m_pThisPage(NULL)//Ĭ������
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

		// �ƶ�����ҳ��
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
		// �ƶ�����ҳ��
		int GetItemIndex()
		{
			return std::find(m_vecPage.begin(), m_vecPage.end(), m_pThisPage)-m_vecPage.begin();
		}

		// �ƶ�ҳ��
		void Move(int x, int y)
		{
			for (VEC_PAGE_ITER iter = m_vecPage.begin(); iter != m_vecPage.end(); ++iter)
			{
				ISlidePageItemUI* pPage = *iter;
				pPage->Move(x, y);
			}
		}

		// �ƶ����
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
		// ֱ���ƶ�����һҳ֮ǰ�������м�ҳ
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
		// X��Y�����Զ��ƶ�����ʵ�ģ�鵽�ɼ�����
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

		// ���ҵ�ǰ�ڿɼ�����ռ������ҳ��
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

		// ���ҳ���ƶ��������پ�����ҳ
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

		// ���㵱ǰģ���ڿɼ��ͻ����е����
		unsigned long CalInClientSize(CControlUI* pPage) const
		{
			CSize size;
			CDuiRect rcPage = pPage->GetPos();
			CDuiRect rcClient = GetPos();
			CDuiRect rcIntersect;
			rcIntersect.IntersectRect(rcPage, rcClient);

			return (unsigned long)rcIntersect.GetWidth() * (unsigned long) rcIntersect.GetHeight();
		}

		// �����
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
// 		// �ƶ�����ҳ��
// 		virtual void SelectItem(UINT unIndex);
// 		// �ƶ�ҳ��
// 		virtual void Move(int x, int y);
// 		// �ƶ����
// 		virtual void MoveOver();
// 		virtual void DoPaint(HDC hDC, const RECT& rcPaint);
// 	protected:
// 		// X��Y�����Զ��ƶ�����ʵ�ģ�鵽�ɼ�����
// 		virtual void AutoMove(int nSpeed);
// 		// ���ҵ�ǰ�ڿɼ�����ռ������ҳ��
// 		virtual CControlUI* FindRightPage();
// 		// ���ҳ���ƶ��������پ�����ҳ
// 		virtual CControlUI* NextPageWhileMove(int nMaxMoveDistance);
// 		// ���㵱ǰģ���ڿɼ��ͻ����е����
// 		virtual unsigned long CalInClientSize(CControlUI* pPage) const;
// 		// �����
// 		virtual bool AlignPoint(const CDuiRect& rcPage, const CDuiRect& rcClient, int& nxMoveLen, int& nyMoveLen);
	protected:
		VEC_PAGE			m_vecPage;				// ��������ҳ���ָ��
		CControlUI*			m_pThisPage;			// ���浱ǰ��ʾ��ҳ��
		CControlUI*			m_pNextPage;			// ������һҳ��
		int					m_nSpeed;				// �Զ��ƶ����ٶ�
		int					m_nDefautPage;			// Ĭ�ϵ�ҳ��
		bool				m_bAutoMove;			// �Ƿ��Զ��ƶ������ʵ�λ��
		bool				m_bMoveNextPage;		// �Ƿ�ֱ��������һҳ֮ǰ
		int					m_nMaxDistanceToNext;	// �ƶ����پ����ҳ
		ALIGN_POINT			m_eAlignPoint;			// �Զ��ƶ�ʱ���뵽��һ����
		DWORD				m_dwProAutoMoveTime;	// ��һ���ƶ���ʱ��
	};

	class CHorSlidePageUI : public CSlidePageUI<CHorizontalLayoutUI>
	{
	public:
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
	protected:
		// ���ҳ���ƶ��������پ�����ҳ
		virtual CControlUI* NextPageWhileMove(int nMaxMoveDistance);
	};

	class CVerSlidePageUI : public CSlidePageUI<CVerticalLayoutUI>
	{
	public:
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
	protected:
		// ���ҳ���ƶ��������پ�����ҳ
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