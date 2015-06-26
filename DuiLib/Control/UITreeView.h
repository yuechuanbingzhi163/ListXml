#ifndef UITreeView_h__
#define UITreeView_h__

#include <vector>
using namespace std;

#pragma once

namespace DuiLib
{
	class CTreeViewUI;
	class CCheckBoxUI;
	class CLabelUI;
	class COptionUI;
	class CTextUI;
	class CEditUI;
	class CComboBoxUI;

	class UILIB_API CTreeNodeUI : public CListContainerElementUI
	{
	public:
		CTreeNodeUI(CTreeNodeUI* _ParentNode = NULL);
		CTreeNodeUI( CTreeViewUI* _ParentTreeView, CTreeNodeUI* _ParentNode );
		~CTreeNodeUI(void);

	public:
		LPCTSTR GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);
		void	DoEvent(TEventUI& event);
		void	Invalidate();
		bool	Select(bool bSelect = true);

		bool	Add(CControlUI* _pTreeNodeUI);
		bool	AddAt(CControlUI* pControl, int iIndex);
		bool	Remove(CControlUI* pControl);

		void	SetVisibleTag(bool _IsVisible);
		bool	GetVisibleTag();
		void	SetItemText(LPCTSTR pstrValue);
		CDuiString	GetItemText();

		void	SetItemText( LPCTSTR pstrValue, int nIndex );
		CDuiString	GetItemText( int nIndex );

		void	CheckBoxSelected(bool _Selected);
		bool	IsCheckBoxSelected() const;
		bool	IsHasChild() const;
		long	GetTreeLevel() const;
		bool	AddChildNode(CTreeNodeUI* _pTreeNodeUI);
		bool	RemoveAt(CTreeNodeUI* _pTreeNodeUI);
		void	SetParentNode(CTreeNodeUI* _pParentTreeNode);
		CTreeNodeUI* GetParentNode();
		long	GetCountChild();
		void	SetTreeView(CTreeViewUI* _CTreeViewUI);
		CTreeViewUI* GetTreeView();
		CTreeNodeUI* GetChildNode(int _nIndex);
		void	SetVisibleFolderBtn(bool _IsVisibled);
		bool	GetVisibleFolderBtn();
		void	SetVisibleCheckBtn(bool _IsVisibled);
		bool	GetVisibleCheckBtn();
		void	SetItemTextColor(DWORD _dwItemTextColor);
		DWORD	GetItemTextColor() const;
		void	SetItemHotTextColor(DWORD _dwItemHotTextColor);
		DWORD	GetItemHotTextColor() const;
		void	SetSelItemTextColor(DWORD _dwSelItemTextColor);
		DWORD	GetSelItemTextColor() const;
		void	SetSelItemHotTextColor(DWORD _dwSelHotItemTextColor);
		DWORD	GetSelItemHotTextColor() const;

		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		CStdPtrArray GetTreeNodes();

		int		GetTreeIndex();
		int		GetNodeIndex();

		CTreeViewUI* GetTreeParent();
		void	SetPos(RECT rc);
		void	SetReadOnly(bool bReadOnly);
		bool	IsReadOnly() const;
		virtual bool OnEditEvent(void* param);	// 编辑框消息
		virtual bool OnComboEvent(void* param);	// 组合框控件消息
		int		GetComboItemIndex();// 获取组合框当前选中项
		CDuiString GetComboItemText(int v_nIndex);// 获取组合框下标项文本内容
		void	SetComboItemIndex(int v_nIndex);//
		void	SetComboItemIndex(LPCTSTR pstrValue);
		vector<CHorizontalLayoutUI*>& GetHorizChild()
		{
			return m_verHorizChild;
		}
		vector<CEditUI*>& GetEditChild()
		{
			return m_verEditChild;
		}
		vector<CTextUI*>& GetTextChild()
		{
			return m_verTextChild;
		}
		CComboUI* GetComboChild()
		{
			return m_pComboChild;
		}


	private:
		CTreeNodeUI* GetLastNode();
		CTreeNodeUI* CalLocation(CTreeNodeUI* _pTreeNodeUI);
	public:
		CHorizontalLayoutUI*	GetTreeNodeHoriznotal() const {return pHoriz;};
		CCheckBoxUI*			GetFolderButton() const {return pFolderButton;};
		CLabelUI*				GetDottedLine() const {return pDottedLine;};
		CCheckBoxUI*			GetCheckBox() const {return pCheckBox;};
		COptionUI*				GetItemButton() const {return pItemButton;};

	private:
		long	m_iTreeLavel;
		bool	m_bIsVisable;
		bool	m_bIsCheckBox;
		DWORD	m_dwItemTextColor;
		DWORD	m_dwItemHotTextColor;
		DWORD	m_dwSelItemTextColor;
		DWORD	m_dwSelItemHotTextColor;

		CTreeViewUI*			pTreeView;
		CHorizontalLayoutUI*	pHoriz;
		CCheckBoxUI*			pFolderButton;// 控制节点收缩的按钮
		CLabelUI*				pDottedLine;// 填充前面空白部分位置
		CCheckBoxUI*			pCheckBox;	// 可以打钩的复选框
		COptionUI*				pItemButton;// 节点

		CHorizontalLayoutUI*	m_pHorizTool;// 放置pDottedLine、pFolderButton、pCheckBox、pItemButton节点
		CHorizontalLayoutUI*	m_pHorizItem;// 放置第一列的文本编辑框
		CTreeNodeUI*			pParentTreeNode;
		int						m_nColumns;	// 列数
		bool					m_bReadOnly;// 节点是否是只读
		int						m_nComboItemIndex;//组合框当前选中项

		CTreeViewUI*					pTreeParent;
		CComboUI*						m_pComboChild;
		vector<CTextUI*>				m_verTextChild;
		vector<CEditUI*>				m_verEditChild;
		vector<CHorizontalLayoutUI*>	m_verHorizChild;

		CStdPtrArray			mTreeNodes;
	};

	class UILIB_API CTreeViewUI : public CListUI,public INotifyUI
	{
	public:
		CTreeViewUI(void);
		~CTreeViewUI(void);

	public:
		virtual LPCTSTR GetClass() const;
		virtual LPVOID	GetInterface(LPCTSTR pstrName);
		virtual bool Add(CTreeNodeUI* pControl );
		virtual long AddAt(CTreeNodeUI* pControl, int iIndex );
		virtual bool AddAt(CTreeNodeUI* pControl,CTreeNodeUI* _IndexNode);
		virtual bool Remove(CTreeNodeUI* pControl);
		virtual bool RemoveAt(int iIndex);
		virtual void RemoveAll();
		virtual bool OnCheckBoxChanged(void* param);
		virtual bool OnFolderChanged(void* param);
		virtual bool OnDBClickItem(void* param);
		virtual bool SetItemCheckBox(bool _Selected,CTreeNodeUI* _TreeNode = NULL, bool v_IsCheckParent = false );
		virtual void SetItemExpand(bool _Expanded,CTreeNodeUI* _TreeNode = NULL, bool v_bExpandedAll = false );
		virtual void Notify(TNotifyUI& msg);
		virtual void SetVisibleFolderBtn(bool _IsVisibled);
		virtual bool GetVisibleFolderBtn();
		virtual void SetVisibleCheckBtn(bool _IsVisibled);
		virtual bool GetVisibleCheckBtn();
		virtual void SetItemMinWidth(UINT _ItemMinWidth);
		virtual UINT GetItemMinWidth();
		virtual void SetItemTextColor(DWORD _dwItemTextColor);
		virtual void SetItemHotTextColor(DWORD _dwItemHotTextColor);
		virtual void SetSelItemTextColor(DWORD _dwSelItemTextColor);
		virtual void SetSelItemHotTextColor(DWORD _dwSelHotItemTextColor);
		
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		virtual void DoEvent(TEventUI& event);
		virtual void SetEnableItemDbClick(bool v_bEnable);
		virtual bool IsEnableItemDbClick();
	private:
		UINT m_uItemMinWidth;
		bool m_bVisibleFolderBtn;
		bool m_bVisibleCheckBtn;

		bool m_bEnableItemDbClick;
	};
}


#endif // UITreeView_h__
