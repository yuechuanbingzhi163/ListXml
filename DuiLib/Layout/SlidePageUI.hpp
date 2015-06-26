#pragma once
#ifndef __SLIDEPAGEUI_HPP_2014_03_20
#define __SLIDEPAGEUI_HPP_2014_03_20

#include <memory>
#include <vector>
#include <algorithm>

namespace DuiLib
{
	template<typename T_Parent>
	interface ISlidePage : public T_Parent
	{
		virtual void MoveOver() = 0;
		virtual void Move(int x, int y) = 0;
		virtual bool IsEnableOverBoundary() = 0;
	};

	class CSlidePageItemUI : public CContainerUI
	{
	public:
		CSlidePageItemUI()
			: _m_bLeftMove(true)
			, _m_bRightMove(true)
			, _m_bTopMove(true)
			, _m_bBottomMove(true)
			, _m_bEnableOver(false)
			, _m_nX(0)
			, _m_nY(0)
		{}
		virtual ~CSlidePageItemUI(){}
		LPCTSTR GetClass() const
		{
			return DUI_CTR_SLIDEPAGEITEM;
		}
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			if (_tcscmp(pstrName, _T("leftmove")) == 0)
			{
				_m_bLeftMove = _tcscmp(pstrValue, "true") == 0;
			}
			else if (_tcscmp(pstrName, _T("rightmove")) == 0)
			{
				_m_bRightMove = _tcscmp(pstrValue, "true") == 0;
			}
			else if (_tcscmp(pstrName, _T("topmove")) == 0)
			{
				_m_bTopMove = _tcscmp(pstrValue, "true") == 0;
			}
			else if (_tcscmp(pstrName, _T("bottommove")) == 0)
			{
				_m_bBottomMove = _tcscmp(pstrValue, "true") == 0;
			}
			else if (_tcscmp(pstrName, _T("move")) == 0)
			{
				bool bMove = _tcscmp(pstrValue, "true") == 0;
				_m_bLeftMove = bMove;
				_m_bRightMove = bMove;
				_m_bTopMove = bMove;
				_m_bBottomMove = bMove;
			}
			else
				CContainerUI::SetAttribute(pstrName, pstrValue);
		}
		virtual void SetPos(RECT rc)
		{
			CRect rcNew(rc);
			rcNew.OffsetRect(_m_nX, _m_nY);
			__super::SetPos(rcNew);
		}
		virtual void Move(int _nX, int _nY)
		{
			_m_nX += _nX;
			_m_nY += _nY;
			CRect rcMove = GetPos();
			rcMove.OffsetRect(_nX, _nY);
			__super::SetPos(rcMove);
		}
		virtual void SetMoveInfo(int _nX, int _nY){_m_nX = _nX; _m_nY = _nY;}
		virtual bool IsLeftMove(){return _m_bLeftMove;}
		virtual bool IsRightMove(){return _m_bRightMove;}
		virtual bool IsTopMove(){return _m_bTopMove;}
		virtual bool IsBottomMove(){return _m_bBottomMove;}
		virtual void SetLeftMove(bool _bMove){_m_bLeftMove = _bMove;}
		virtual void SetRightMove(bool _bMove){_m_bRightMove = _bMove;}
		virtual void SetTopMove(bool _bMove){_m_bTopMove = _bMove;}
		virtual void SetBottomMove(bool _bMove){_m_bBottomMove = _bMove;}
	protected:
		bool		_m_bLeftMove;
		bool		_m_bRightMove;
		bool		_m_bTopMove;
		bool		_m_bBottomMove;
		bool		_m_bEnableOver;
		int			_m_nX;
		int			_m_nY;
	};

	class CSlidePageItem
	{
	public:
		CSlidePageItem(CControlUI* _pContrl)
			: _m_pContrl(_pContrl)
			, _m_pManager(nullptr)
			, _m_bDrag(false)
			, _m_bLeftMove(true)
			, _m_bRightMove(true)
			, _m_bTopMove(true)
			, _m_bBottomMove(true)
			, _m_nX(0)
			, _m_nY(0)
		{}
		virtual ~CSlidePageItem(){}
		virtual void DoInit()
		{
			if (!_m_pManager)
			{
				_m_pManager = _m_pContrl->GetManager();
				if (_tcscmp(_m_pContrl->GetClass(), DUI_CTR_SLIDEPAGEITEM) == 0)
				{
					_m_pContrl->OnEvent += MakeDelegate(this, &CSlidePageItem::DoEvent);
					CSlidePageItemUI* pItem = reinterpret_cast<CSlidePageItemUI*>(_m_pContrl);
					_m_bLeftMove = pItem->IsLeftMove();
					_m_bRightMove = pItem->IsRightMove();
					_m_bTopMove = pItem->IsTopMove();
					_m_bBottomMove = pItem->IsBottomMove();
				}
			}
		}
		virtual bool DoEvent(void* _pEvent)
		{
			ASSERT(_m_pContrl && _m_pManager);

			TEventUI* pEvent = reinterpret_cast<TEventUI*>(_pEvent);
			switch (pEvent->Type)
			{
			case UIEVENT_BUTTONDOWN:
				_LBottonDown(pEvent->ptMouse);
				break;
			case UIEVENT_BUTTONUP:
				_LBottonUp(pEvent->ptMouse);
				break;
			case UIEVENT_MOUSEMOVE:
				_MouseMove(pEvent->ptMouse);
				break;
			case UIEVENT_KILLFOCUS:
			case UIEVENT_RBUTTONDOWN:
				_LBottonUp(pEvent->ptMouse);
				break;
			}
			return true;
		}
		virtual void Move(int _nX, int _nY)
		{
			_m_nX += _nX;
			_m_nY += _nY;
			if (_tcscmp(_m_pContrl->GetClass(), DUI_CTR_SLIDEPAGEITEM) == 0)
			{
				((CSlidePageItemUI*)_m_pContrl)->Move(_nX, _nY);
			}
			else
			{
				CRect rcMove = _m_pContrl->GetPos();
				rcMove.OffsetRect(_nX, _nY);
				_m_pContrl->SetPos(rcMove);
			}
		}
		virtual void ResetMoveInfo()
		{
			if (_tcscmp(_m_pContrl->GetClass(), DUI_CTR_SLIDEPAGEITEM) == 0)
			{
				((CSlidePageItemUI*)_m_pContrl)->SetMoveInfo(0, 0);
			}
		}
	public:
		CControlUI* GetContrl(){return _m_pContrl;}
		CPaintManagerUI* GetManager(){return _m_pManager;}
	protected:
		virtual void _LBottonDown(::POINT& _ptMouse)
		{
			_m_bDrag = true;
			_m_ptOldMouse = _ptMouse;
			return;
		}
		virtual void _LBottonUp(::POINT& _ptMouse)
		{
			if (_m_bDrag)
			{
				_m_bDrag = false;
				CControlUI* pContrl = (CControlUI*)_m_pContrl->GetParent();
				if (pContrl)
				{
					if (_tcscmp(pContrl->GetClass(), DUI_CTR_HORSLIDEPAGE) == 0)
						((ISlidePage<CHorizontalLayoutUI>*)pContrl)->MoveOver();
					else if (_tcscmp(pContrl->GetClass(), DUI_CTR_VERSLIDEPAGE) == 0)
						((ISlidePage<CHorizontalLayoutUI>*)pContrl)->MoveOver();
				}
			}
		}
		virtual void _MouseMove(::POINT& _ptMouse)
		{
			if (_m_bDrag)
			{
				int xDrag = _ptMouse.x - _m_ptOldMouse.x;
				int yDrag = _ptMouse.y - _m_ptOldMouse.y;
				if (!_m_bLeftMove && xDrag < 0) 
					xDrag = 0;
				if (!_m_bRightMove && xDrag > 0) 
					xDrag = 0;
				if (!_m_bTopMove && yDrag < 0) 
					yDrag = 0;
				if (!_m_bBottomMove && yDrag > 0) 
					yDrag = 0;
				// 如果鼠标按下的情况下移动，则为拖动
				if (_m_bDrag)
				{
					CControlUI* pContrl = (CControlUI*)_m_pContrl->GetParent();
					LPCTSTR lpClass = pContrl->GetClass();
					if (pContrl)
					{
						if (_tcscmp(pContrl->GetClass(), DUI_CTR_HORSLIDEPAGE) == 0)
							((ISlidePage<CHorizontalLayoutUI>*)pContrl)->Move(xDrag, 0);
						else if (_tcscmp(pContrl->GetClass(), DUI_CTR_VERSLIDEPAGE) == 0)
							((ISlidePage<CVerticalLayoutUI>*)pContrl)->Move(0, yDrag);
					}
				}
			}
			_m_ptOldMouse = _ptMouse;
			_OverBoundary(_ptMouse);
		}
		virtual void _OverBoundary(::POINT& _ptMouse)
		{
			CControlUI* pParent = _m_pContrl->GetParent();
			ASSERT(pParent);
			::CRect rcParent(pParent->GetPos());
			if (!rcParent.PtInRect(_ptMouse))
			{
				if (	_tcscmp(pParent->GetClass(), DUI_CTR_HORSLIDEPAGE) == 0 && ((ISlidePage<CHorizontalLayoutUI>*)pParent)->IsEnableOverBoundary()
					||	_tcscmp(pParent->GetClass(), DUI_CTR_VERSLIDEPAGE) == 0 && ((ISlidePage<CVerticalLayoutUI>*)pParent)->IsEnableOverBoundary())
					return;
				else
					_LBottonUp(_ptMouse);
			}
			
		}
	protected:
		bool		_m_bDrag;
		bool		_m_bLeftMove;
		bool		_m_bRightMove;
		bool		_m_bTopMove;
		bool		_m_bBottomMove;
		::CPoint	_m_ptOldMouse;
		int			_m_nX;
		int			_m_nY;
	protected:
		CControlUI*			_m_pContrl;
		CPaintManagerUI*	_m_pManager;
	};

	template<typename T_Parent>
	class CSlidePageUI : public ISlidePage<T_Parent>
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
		typedef std::tr1::shared_ptr<CSlidePageItem>	SP_PAGE;
		typedef std::vector<SP_PAGE>					VEC_PAGE;
		typedef VEC_PAGE::iterator						VEC_PAGE_ITER;
	public:
		CSlidePageUI()
			: _m_nThisPage(-1)
			, _m_nNextPage(-1)
			, _m_nSpeed(100)
			, _m_nDefautPage(0)
			, _m_bAutoMove(true)
			, _m_nMaxDistanceToNext(0)
			, _m_eAlignPoint(TOP_LEFT)
			, _m_nSelectItemSpeed(0)
			, _m_bIsSelect(false)
			, _m_bSelfAdaption(false)
			, _m_unElapse(10)
			, _m_bEnableOver(false)
		{}
		virtual ~CSlidePageUI(){}
		virtual void DoInit()
		{
			__super::DoInit();
			if (!_m_vecPage.empty())
				_m_nThisPage = _m_nDefautPage < static_cast<int>(_m_vecPage.size()) ? _m_nDefautPage : 0;
			GetManager()->SetTimer(this, 1, _m_unElapse);
			_ReMoveToThisPage();
		}
		virtual bool Add(CControlUI* pControl)
		{
			_ResetMoveInfo();
			if (__super::Add(pControl))
			{
				SP_PAGE spPage(new CSlidePageItem(pControl));
				_m_vecPage.push_back(spPage);
				if (_m_nThisPage == -1)
					_m_nThisPage = 0;
				_ReMoveToThisPage();
				return true;
			}
			return false;
		}
		virtual bool AddAt(CControlUI* pControl, int iIndex)
		{
			_ResetMoveInfo();
			if (__super::AddAt(pControl, iIndex))
			{
				SP_PAGE spPage(new CSlidePageItem(pControl));
				if (static_cast<UINT>(iIndex) < _m_vecPage.size())
					_m_vecPage.insert(_m_vecPage.begin() + static_cast<UINT>(iIndex), spPage);
				else
					_m_vecPage.insert(_m_vecPage.end(), spPage);
				if (_m_nThisPage == -1)
					_m_nThisPage = 0;
				_ReMoveToThisPage();
				ASSERT(__super::m_items.GetSize() == static_cast<int>(_m_vecPage.size()));
				return true;
			}
			return false;
		}
		virtual bool Remove(CControlUI* pControl)
		{
			_ResetMoveInfo();
			if (__super::Remove(pControl))
			{
				VEC_PAGE_ITER iter = _m_vecPage.erase(std::find_if(_m_vecPage.begin(), _m_vecPage.end(), [&pControl](SP_PAGE& _spPage)->bool{return _spPage->GetContrl() == pControl;}));
				if (_m_nThisPage >= static_cast<int>(_m_vecPage.size()))
					_m_nThisPage = static_cast<int>(_m_vecPage.size()) - 1;
				if (_m_nNextPage >= static_cast<int>(_m_vecPage.size()))
					_m_nNextPage = static_cast<int>(_m_vecPage.size()) - 1;
				//_ReMoveToThisPage();
				MoveOver();
				return true;
			}
			return false;
		}
		virtual bool RemoveAt(int iIndex)
		{
			_ResetMoveInfo();
			if (__super::RemoveAt(iIndex))
			{
				if (static_cast<UINT>(iIndex) < _m_vecPage.size())
					_m_vecPage.erase(_m_vecPage.begin() + static_cast<UINT>(iIndex));
				else
					_m_vecPage.erase(_m_vecPage.begin() + _m_vecPage.size() - 1);
				if (_m_nThisPage >= static_cast<int>(_m_vecPage.size()))
					_m_nThisPage = static_cast<int>(_m_vecPage.size()) - 1;
				if (_m_nNextPage >= static_cast<int>(_m_vecPage.size()))
					_m_nNextPage = static_cast<int>(_m_vecPage.size()) - 1;
				//_ReMoveToThisPage();
				MoveOver();
				ASSERT(__super::m_items.GetSize() == static_cast<int>(_m_vecPage.size()));
				return true;
			}
			return false;
		}
		virtual void RemoveAll()
		{
			__super::RemoveAll();
			_m_vecPage.clear();
			_m_nThisPage = -1;
			_m_nNextPage = -1;
		}
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			if (_tcscmp(pstrName, _T("speed")) == 0)
			{
				LPTSTR pstr = NULL;
				_m_nSpeed = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			}
			else if (_tcscmp(pstrName, _T("selectid")) == 0)
			{
				LPTSTR pstr = NULL;
				_m_nDefautPage = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			}
			else if (_tcscmp(pstrName, _T("automove")) == 0)
			{
				_m_bAutoMove = _tcscmp(pstrValue, _T("true")) == 0;
			}
			else if (_tcscmp(pstrName, _T("distance")) == 0)
			{
				LPTSTR pstr = NULL;
				_m_nMaxDistanceToNext = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			}
			else if (_tcscmp(pstrName, _T("selectspeed")) == 0)
			{
				LPTSTR pstr = NULL;
				_m_nSelectItemSpeed = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			}
			else if (_tcscmp(pstrName, _T("alignpoint")) == 0)
			{
				if (_tcscmp(pstrValue, _T("topleft")) == 0)
					_m_eAlignPoint = TOP_LEFT;
				else if (_tcscmp(pstrValue, _T("topright")) == 0)
					_m_eAlignPoint = TOP_RIGHT;
				else if (_tcscmp(pstrValue, _T("topcenter")) == 0)
					_m_eAlignPoint = TOP_CENTER;
				else if (_tcscmp(pstrValue, _T("bottomleft")) == 0)
					_m_eAlignPoint = BOTTOM_LEFT;
				else if (_tcscmp(pstrValue, _T("bottomright")) == 0)
					_m_eAlignPoint = BOTTOM_RIGHT;
				else if (_tcscmp(pstrValue, _T("bottomcenter")) == 0)
					_m_eAlignPoint = BOTTOM_CENTER;
				else if (_tcscmp(pstrValue, _T("leftcenter")) == 0)
					_m_eAlignPoint = LEFT_CENTER;
				else if (_tcscmp(pstrValue, _T("rightcenter")) == 0)
					_m_eAlignPoint = RIGHT_CENTER;
				else if (_tcscmp(pstrValue, _T("center")) == 0)
					_m_eAlignPoint = CENTER;
			}
			else if (_tcscmp(pstrName, _T("selfadaption")) == 0)
				_m_bSelfAdaption = _tcscmp(pstrValue, _T("true")) == 0;
			else if (_tcscmp(pstrName, _T("enableover")) == 0)
			{
				_m_bEnableOver = _tcscmp(pstrValue, "true") == 0;
			}
			else
				__super::SetAttribute(pstrName, pstrValue);
		}
		// 移动到子页面
		virtual void SelectItem(int _nIndex)
		{
			if (_nIndex < m_items.GetSize())
			{
				_m_nNextPage = _nIndex;
				_m_bIsSelect = true;
			}
		}
		// 移动页面
		virtual void Move(int x, int y)
		{
			for (VEC_PAGE_ITER iter = _m_vecPage.begin(); iter != _m_vecPage.end(); ++iter)
				(*iter)->Move(x, y);
		}
		// 移动完毕
		virtual void MoveOver()
		{
			_m_nNextPage = _m_nMaxDistanceToNext ? _NextPageWhileMove(_m_nMaxDistanceToNext) : _FindRightPage();
			_m_nNextPage = _IsAppropriate(_m_nNextPage);
		}
		// 获取当前页面的序号
		virtual int	 GetCurSel()
		{
			return _m_nThisPage;
		}
		virtual void DoEvent(TEventUI& event)
		{
			switch (event.Type)
			{
			case UIEVENT_TIMER:
				{
					switch (event.wParam)
					{
					case 1:
						{
							std::for_each(_m_vecPage.begin(), _m_vecPage.end(), [](SP_PAGE& _spItem){_spItem->DoInit();});
							if (_m_nDefautPage && static_cast<UINT>(_m_nDefautPage) < _m_vecPage.size())
							{
								_m_nNextPage = _IsAppropriate(_m_nDefautPage);
								_m_nDefautPage = 0;
								_AutoMove(-1);
								Invalidate();
							}
							if (_m_nNextPage >= 0)
							{
								_AutoMove(_m_nSpeed);
								Invalidate();
							}
						}
						break;
					}
				}
				break;
			}
		}
	// 属性操作
	public:
		virtual void SetSpeed(int _nSpeed){_m_nSpeed = _nSpeed;}
		virtual int  GetSpeed(){return _m_nSpeed;}
		virtual void SetAutoMove(bool _bAutoMove){_m_bAutoMove = _bAutoMove;}
		virtual bool IsAutoMove(){return _m_bAutoMove;}
		virtual void SetSelfDaption(bool _bSelfDaption){_m_bSelfAdaption = _bSelfDaption;}
		virtual bool IsSelfDaption(){return _m_bSelfAdaption;}
		virtual void SetMaxDistanceToNext(int _nMaxDistanceToNext){_m_nMaxDistanceToNext = _nMaxDistanceToNext;}
		virtual int  GetMaxDistanceToNext(){return _m_nMaxDistanceToNext;}
		virtual void SetSelectItemSpeed(int nSelectItemSpeed){_m_nSelectItemSpeed = nSelectItemSpeed;}
		virtual int  GetSelectItemSpeed(){return _m_nSelectItemSpeed;}
		virtual void SetEnableOverBoundary(bool _bEnableOver){_m_bEnableOver = _bEnableOver;}
		virtual bool IsEnableOverBoundary(){return _m_bEnableOver;}
	protected:
		// 重置相对移动的位置信息
		virtual void _ResetMoveInfo()
		{
			std::for_each(_m_vecPage.begin(), _m_vecPage.end(), [](SP_PAGE& _spItem){_spItem->ResetMoveInfo();});
		}
		// 在添加删除这些操作时会重刷新回初始状态，添加一个内部移动页面到当前页的功能
		virtual void _ReMoveToThisPage()
		{
			_m_nDefautPage = _m_nThisPage;
		}
		// X和Y轴上自动移动最合适的模块到可见区域
		virtual void _AutoMove(int _nSpeed)
		{
			if (!_m_bAutoMove && !_m_nDefautPage)
				_m_nNextPage = -1;
			if (_m_nNextPage >= 0)
			{
				CRect rcPage = _m_vecPage[_m_nNextPage]->GetContrl()->GetPos();
				CRect rcClient = GetPos();
				int nxMoveLen = 0;
				int nyMoveLen = 0;
				if (_AlignPoint(rcPage, rcClient, nxMoveLen, nyMoveLen))
				{
					_m_nThisPage = _m_nNextPage;
					_m_nNextPage = -1;
					_m_bIsSelect = false;
					GetManager()->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED, _m_nThisPage);
					return;
				}
				int nX = nxMoveLen;
				int nY = nyMoveLen;
				// 如果是手动选择的页面，并且设置了手动选择页面的移动速度，则用设置的速度翻页
				if (_m_bIsSelect && _m_nSelectItemSpeed)
					_nSpeed = _m_nSelectItemSpeed;
				if (_nSpeed > 0)
				{
					nX = abs(nxMoveLen) > _nSpeed ? nxMoveLen > 0 ? _nSpeed : -_nSpeed : nxMoveLen;
					nY = abs(nyMoveLen) > _nSpeed ? nyMoveLen > 0 ? _nSpeed : -_nSpeed : nyMoveLen;
				}
				Move(nX, nY);
			}
		}
		// 查找当前在可见区中占用最多的页面
		virtual int _FindRightPage()
		{
			int nRightPage = 0;
			for (int i = 1; i < static_cast<int>(_m_vecPage.size()); ++i)
			{
				unsigned long ulRightPage = _CalInClientSize(_m_vecPage[nRightPage]->GetContrl());
				unsigned long ulThisPage = _CalInClientSize(_m_vecPage[i]->GetContrl());
				if (ulThisPage > ulRightPage)
					nRightPage = i;
			}
			return nRightPage;
		}
		// 如果页面移动超过多少距离则翻页
		virtual int _NextPageWhileMove(int _nMaxMoveDistance)
		{
			if (_m_nThisPage >= 0)
			{
				VEC_PAGE_ITER iter = _m_vecPage.begin() + _m_nThisPage;
				CRect rcClient = GetPos();
				CRect rcPage= (*iter)->GetContrl()->GetPos();
				int nX = 0, nY = 0;
				_AlignPoint(rcPage, rcClient, nX, nY);
				if (abs(nX) > _nMaxMoveDistance)
				{
					if (nX < 0)
						return iter == _m_vecPage.begin() ? _m_nThisPage : _m_nThisPage - 1;
					else if (nX > 0)
						return (iter + 1) == _m_vecPage.end() ? _m_nThisPage : _m_nThisPage + 1;
				}
				else if (abs(nY) > _nMaxMoveDistance)
				{
					if (nY < 0)
						return iter == _m_vecPage.begin() ? _m_nThisPage : _m_nThisPage - 1;
					else if (nY > 0)
						return (iter + 1) == _m_vecPage.end() ? _m_nThisPage : _m_nThisPage - 1;
				}
			}
			return _m_nThisPage;
		}
		// 计算当前模块在可见客户区中的面积
		virtual unsigned long	_CalInClientSize(CControlUI* _pPage) const
		{
			CSize size;
			CRect rcPage = _pPage->GetPos();
			CRect rcClient = GetPos();
			CRect rcIntersect;
			rcIntersect.IntersectRect(rcPage, rcClient);
			return (unsigned long)rcIntersect.Width() * (unsigned long) rcIntersect.Height();
		}
		// 对齐点
		virtual bool _AlignPoint(const CRect& _rcPage, const CRect& _rcClient, int& _nxMoveLen, int& _nyMoveLen)
		{
			switch (_m_eAlignPoint)
			{
			case TOP_LEFT:
				if (_rcPage.TopLeft() == _rcClient.TopLeft())
					return true;
				_nxMoveLen = _rcClient.left - _rcPage.left;
				_nyMoveLen = _rcClient.top - _rcPage.top;
				return false;
			case TOP_RIGHT:
				if (_rcPage.top == _rcClient.top && _rcPage.right == _rcClient.right)
					return true;
				_nxMoveLen = _rcClient.right - _rcPage.right;
				_nyMoveLen = _rcClient.top - _rcPage.top;
				return false;
			case TOP_CENTER:
				if (_rcPage.top == _rcClient.top && _rcPage.left + _rcPage.Width() / 2 == _rcClient.left + _rcClient.Width() / 2)
					return true;
				_nxMoveLen = (_rcClient.left + _rcClient.Width() / 2) - (_rcPage.left + _rcPage.Width() / 2);
				_nyMoveLen = _rcClient.top - _rcPage.top;
				return false;
			case BOTTOM_LEFT:
				if (_rcPage.bottom == _rcClient.bottom && _rcPage.left == _rcClient.left)
					return true;
				_nxMoveLen = _rcClient.left - _rcPage.left;
				_nyMoveLen = _rcClient.bottom - _rcPage.bottom;
				return false;
			case BOTTOM_RIGHT:
				if (_rcPage.BottomRight() == _rcClient.BottomRight())
					return true;
				_nxMoveLen = _rcClient.right - _rcPage.right;
				_nyMoveLen = _rcClient.bottom - _rcPage.bottom;
				return false;
			case BOTTOM_CENTER:
				if (_rcPage.bottom == _rcClient.bottom && _rcPage.left + _rcPage.Width() / 2 == _rcClient.left + _rcClient.Width() / 2)
					return true;
				_nxMoveLen = (_rcClient.left + _rcClient.Width() / 2) - (_rcPage.left + _rcPage.Width() / 2);
				_nyMoveLen = _rcClient.bottom - _rcPage.bottom;
				return false;
			case LEFT_CENTER:
				if (_rcPage.left == _rcClient.left && _rcPage.top + _rcPage.Height() / 2 == _rcClient.top + _rcClient.Height() / 2)
					return true;
				_nxMoveLen = _rcClient.left - _rcPage.left;
				_nyMoveLen = (_rcClient.top + _rcClient.Height() / 2) - (_rcPage.top + _rcPage.Height() / 2);
				return false;
			case RIGHT_CENTER:
				if (_rcPage.right == _rcClient.right && _rcPage.top + _rcPage.Height() / 2 == _rcClient.top + _rcClient.Height() / 2)
					return true;
				_nxMoveLen = _rcClient.right - _rcPage.right;
				_nyMoveLen = (_rcClient.top + _rcClient.Height() / 2) - (_rcPage.top + _rcPage.Height() / 2);
				return false;
			case CENTER:
				if (_rcPage.CenterPoint() == _rcClient.CenterPoint())
					return true;
				_nxMoveLen = (_rcClient.left + _rcClient.Width() / 2) - (_rcPage.left + _rcPage.Width() / 2);
				_nyMoveLen = (_rcClient.top + _rcClient.Height() / 2) - (_rcPage.top + _rcPage.Height() / 2);
				return false;
			}
			return false;
		}
		// 判断是否适合移动到当前页面，主要是防止多个不满屏的记录时，移动到当前记录可能会导致页面有空白
		// 具体实现由子类来实现
		virtual int _IsAppropriate(int _nIndex) = 0;
	protected:
		VEC_PAGE			_m_vecPage;				// 保存所有页面的指针
		int					_m_nThisPage;			// 保存当前显示的页面
		int					_m_nNextPage;			// 保存下一页面
		int					_m_nSpeed;				// 自动移动的速度
		int					_m_nDefautPage;			// 默认的页面
		bool				_m_bAutoMove;			// 是否自动移动到合适的位置
		bool				_m_bSelfAdaption;		// 启用自适应功能，也就是调用_IsAppropriate
		int					_m_nMaxDistanceToNext;	// 移动多少距离后翻页
		ALIGN_POINT			_m_eAlignPoint;			// 自动移动时对齐到哪一个点
		int					_m_nSelectItemSpeed;	// 手动选择页面的速度
		bool				_m_bIsSelect;			// 是否是手动选择页面
		const UINT			_m_unElapse;			// 定时器的时间间隔
		bool				_m_bEnableOver;			// 是否允许越过边界拖动
	};

	class CHorSlidePageUI : public CSlidePageUI<CHorizontalLayoutUI>
	{
	public:
		LPCTSTR GetClass() const
		{
			return DUI_CTR_HORSLIDEPAGE;
		}
	protected:
		// 如果页面移动超过多少距离则翻页
		virtual int _NextPageWhileMove(int _nMaxMoveDistance)
		{
			if (_m_nThisPage >= 0)
			{
				VEC_PAGE_ITER iter = _m_vecPage.begin() + _m_nThisPage;
				CRect rcClient = GetPos();
				CRect rcPage= (*iter)->GetContrl()->GetPos();
				int nX = 0, nY = 0;
				_AlignPoint(rcPage, rcClient, nX, nY);
				if (abs(nX) > _nMaxMoveDistance)
				{
					if (nX < 0)
						return iter == _m_vecPage.begin() ? _m_nThisPage : _m_nThisPage - 1;
					else if (nX > 0)
						return (iter + 1) == _m_vecPage.end() ? _m_nThisPage : _m_nThisPage + 1;
				}
			}
			return _m_nThisPage;
		}
		// 判断是否适合移动到当前页面，主要是防止多个不满屏的记录时，移动到当前记录可能会导致页面有空白
		virtual int _IsAppropriate(int _nIndex)
		{
			if (_m_vecPage.empty())
				return -1;
			if (!_m_bSelfAdaption)
				return _nIndex;
			::CRect rcItem;
			std::for_each(_m_vecPage.begin(), _m_vecPage.end(), [&rcItem](SP_PAGE& _spItem){rcItem.UnionRect(rcItem, &_spItem->GetContrl()->GetPos());});
			::CRect rcPage(GetPos());
			::CRect rcInter;
			rcInter.IntersectRect(rcItem, rcPage);
			int nCount = static_cast<int>(_m_vecPage.size());
			if (rcInter != rcPage)
			{
				switch (_m_eAlignPoint)
				{
				case TOP_LEFT:
				case BOTTOM_LEFT:
				case LEFT_CENTER:
					{
						for (int i = _nIndex, nWidth = 0; i < nCount; ++i)
						{
							nWidth += ::CRect(&_m_vecPage[i]->GetContrl()->GetPos()).Width();
							if (nWidth >= rcPage.Width())
								return _nIndex;
						}
						for (int i = nCount - 1, nWidth = 0; i >= 0; --i)
						{
							nWidth += ::CRect(&_m_vecPage[i]->GetContrl()->GetPos()).Width();
							if (nWidth >= rcPage.Width())
								return i;
						}
						return 0;
					}
				case TOP_RIGHT:
				case BOTTOM_RIGHT:
				case RIGHT_CENTER:
					{
						for (int i = _nIndex, nWidth = 0; i >= 0; --i)
						{
							nWidth += ::CRect(&_m_vecPage[i]->GetContrl()->GetPos()).Width();
							if (nWidth >= rcPage.Width())
								return _nIndex;
						}
						for (int i = 0, nWidth = 0; i < nCount; ++i)
						{
							nWidth += ::CRect(&_m_vecPage[i]->GetContrl()->GetPos()).Width();
							if (nWidth >= rcPage.Width())
								return i;
						}
						return nCount - 1;
					}
				}
			}
			return _nIndex;
		}
		// 查找当前在可见区中占用最多的页面
		virtual int _FindRightPage()
		{
			switch (_m_eAlignPoint)
			{
			case TOP_RIGHT:
			case BOTTOM_RIGHT:
			case RIGHT_CENTER:
				{
					int nRightPage = static_cast<int>(_m_vecPage.size() - 1);
					for (int i = static_cast<int>(_m_vecPage.size() - 1); i >= 0; --i)
					{
						unsigned long ulRightPage = _CalInClientSize(_m_vecPage[nRightPage]->GetContrl());
						unsigned long ulThisPage = _CalInClientSize(_m_vecPage[i]->GetContrl());
						if (ulThisPage > ulRightPage)
							nRightPage = i;
					}
					return nRightPage;
				}
			}
			return __super::_FindRightPage();
		}
	};

	class CVerSlidePageUI : public CSlidePageUI<CVerticalLayoutUI>
	{
	public:
		LPCTSTR GetClass() const
		{
			return DUI_CTR_VERSLIDEPAGE;
		}
	protected:
		// 如果页面移动超过多少距离则翻页
		virtual int _NextPageWhileMove(int _nMaxMoveDistance)
		{
			if (_m_nThisPage >= 0)
			{
				VEC_PAGE_ITER iter = _m_vecPage.begin() + _m_nThisPage;
				CRect rcClient = GetPos();
				CRect rcPage= (*iter)->GetContrl()->GetPos();
				int nX = 0, nY = 0;
				_AlignPoint(rcPage, rcClient, nX, nY);
				if (abs(nY) > _nMaxMoveDistance)
				{
					if (nY < 0)
						return iter == _m_vecPage.begin() ? _m_nThisPage : _m_nThisPage - 1;
					else if (nY > 0)
						return (iter + 1) == _m_vecPage.end() ? _m_nThisPage : _m_nThisPage + 1;
				}
			}
			return _m_nThisPage;
		}
		// 判断是否适合移动到当前页面，主要是防止多个不满屏的记录时，移动到当前记录可能会导致页面有空白
		virtual int _IsAppropriate(int _nIndex)
		{
			if (_m_vecPage.empty())
				return -1;
			if (!_m_bSelfAdaption)
				return _nIndex;
			::CRect rcItem;
			std::for_each(_m_vecPage.begin(), _m_vecPage.end(), [&rcItem](SP_PAGE& _spItem){rcItem.UnionRect(rcItem, &_spItem->GetContrl()->GetPos());});
			::CRect rcPage(GetPos());
			::CRect rcInter;
			rcInter.IntersectRect(rcItem, rcPage);
			int nCount = static_cast<int>(_m_vecPage.size());
			if (rcInter != rcPage)
			{
				switch (_m_eAlignPoint)
				{
				case TOP_LEFT:
				case TOP_RIGHT:
				case TOP_CENTER:
					{
						for (int i = _nIndex, nHeight = 0; i < nCount; ++i)
						{
							nHeight += ::CRect(&_m_vecPage[i]->GetContrl()->GetPos()).Height();
							if (nHeight >= rcPage.Height())
								return _nIndex;
						}
						for (int i = nCount - 1, nHeight = 0; i >= 0; --i)
						{
							nHeight += ::CRect(&_m_vecPage[i]->GetContrl()->GetPos()).Height();
							if (nHeight >= rcPage.Height())
								return i;
						}
						return 0;
					}
				case BOTTOM_LEFT:
				case BOTTOM_RIGHT:
				case BOTTOM_CENTER:
					{
						for (int i = _nIndex, nHeight = 0; i >= 0; --i)
						{
							nHeight += ::CRect(&_m_vecPage[i]->GetContrl()->GetPos()).Height();
							if (nHeight >= rcPage.Height())
								return _nIndex;
						}
						for (int i = 0, nHeight = 0; i < nCount; ++i)
						{
							nHeight += ::CRect(&_m_vecPage[i]->GetContrl()->GetPos()).Height();
							if (nHeight >= rcPage.Height())
								return i;
						}
						return nCount - 1;
					}
				}
			}
			return _nIndex;
		}
		// 查找当前在可见区中占用最多的页面
		virtual int _FindRightPage()
		{
			switch (_m_eAlignPoint)
			{
			case BOTTOM_LEFT:
			case BOTTOM_RIGHT:
			case BOTTOM_CENTER:
				{
					int nRightPage = static_cast<int>(_m_vecPage.size() - 1);
					for (int i = static_cast<int>(_m_vecPage.size() - 1); i >= 0; --i)
					{
						unsigned long ulRightPage = _CalInClientSize(_m_vecPage[nRightPage]->GetContrl());
						unsigned long ulThisPage = _CalInClientSize(_m_vecPage[i]->GetContrl());
						if (ulThisPage > ulRightPage)
							nRightPage = i;
					}
					return nRightPage;
				}
			}
			return __super::_FindRightPage();
		}
	};

}

#endif