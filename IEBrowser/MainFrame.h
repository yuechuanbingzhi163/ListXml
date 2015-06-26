/*******************************
**FileName	： MainFrame.h
**Creator	： zwl	
**Date		： 2014-01-14	
**Modifier	： 
**ModifyDate： 
**Comment	： 主窗口类的申明
**Version	： 1.0.0
/*******************************/

#pragma once
#include "Duilib/duilib.h"
#include "WndShadow.h"

#include "PList/Plist.hpp"


class CMainFrame : public CXMLWnd
{
public:

	CMainFrame(LPCTSTR v_pStrSkinPath);
	~CMainFrame();

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
	void Notify(TNotifyUI & msg);
	virtual void InitWindow(void);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void OnPrepare();
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	/**
	* @brief : 消息响应函数 
	*/

	DUI_DECLARE_MESSAGE_MAP()

	bool OnBtnClose(void *lParam);
	bool OnBtnMin(void *lParam);
	bool OnBtnMax(void *lParam);
	bool OnBtnRestore(void *lParam);


	bool OnBtnAddChild(void *lParam);	// 添加子节点消息响应
	bool OnBtnAdd(void *lParam);		// 添加节点消息响应
	bool OnBtnDelete(void *lParam);		// 删除节点消息响应

	bool OnBtnOpen(void *lParam);		// 打开文件消息响应
	bool OnBtnSave(void *lParam);		// 保存文件消息响应
	bool OnBtnExit(void *lParam);		// 关闭文件消息响应

	bool Add(CTreeNodeUI* v_pCurNode);		// 添加节点方法
	bool AddChild(CTreeNodeUI* v_pCurNode);	// 添加子节点方法
	bool Delete(CTreeViewUI* v_pTreeParent, CTreeNodeUI* v_pCurNode);	// 删除节点方法
	bool RemoveAllChild(CTreeNodeUI* v_pParentNode);	//移除根节点之外的所有节点
	bool Open(CTreeNodeUI* v_pRootNode, vector<pair<string, boost::any>> v_mapDict);//打开文件方法
	bool Save();	// 保存文件方法
	bool Exit();	// 关闭文件方法 不保存文件

	bool OnTreeNodeNotify(void* lParam);	// TreeNode节点消息响应

	void FadeIn() ;	// 窗口显示时渐显方法
	void FadeOut() ;// 窗口退出时渐隐方法

	bool LoadPList(CTreeNodeUI* v_pRootNode, CString v_strFileName, vector<pair<string, boost::any>> v_mapDict);//加载Plist和xml文件方法
	bool SavePList(CTreeNodeUI* v_pRootNode, CString v_strSavePath);				// 保存文件方法
	bool AddArray(CTreeNodeUI* v_pParentNode, boost::any v_arrayNode);				//添加array节点方法
	bool AddDict(CTreeNodeUI* v_pParentNode, boost::any v_dictNode);				// 添加dictionary节点方法
	bool SaveArray(CTreeNodeUI* v_pParentNode, vector<boost::any>& v_arrayNode);	//保存array节点方法
	bool SaveDict(CTreeNodeUI* v_pParentNode, vector<pair<string, boost::any>>& v_dictNode);	// 保存dictionary节点方法

	//文件拖放消息响应
	void OnDropFiles(HDROP hDropInfo);
private:
	CButtonUI*	m_pBtnAddChild;
	CButtonUI*	m_pBtnAdd;
	CButtonUI*	m_pBtnDelete;
	CButtonUI*	m_pBtnOpen;
	CButtonUI*	m_pBtnSave;
	CButtonUI*	m_pBtnExit;
	CTextUI*	m_pTextTitle;

	bool m_bExit;
	int m_nAlpha ;
	int m_nFadeTimer ;
	DWORD m_dwStyle;

	CTreeNodeUI*	m_pRootNode;//Root节点
	CTreeViewUI*	m_pTreeListXml;// 树形控件

	vector<boost::any> m_arrayRoot;
	vector<boost::any> m_aSaveRoot;
// 	map<string, boost::any, DisableCompare<string> > m_mapDict;
// 	map<string, boost::any, DisableCompare<string> > m_saveDict;
	vector<pair<string, boost::any>> m_mapDict;
	vector<pair<string, boost::any>> m_saveDict;


	CFadeButtonUI	*m_pBtnClose;
	CFadeButtonUI	*m_pBtnMin;
	CFadeButtonUI	*m_pBtnMax;
	CFadeButtonUI	*m_pBtnRestore;

	CDuiString m_pStrSkinPath;
	CWndShadow m_wndShadow;
};
