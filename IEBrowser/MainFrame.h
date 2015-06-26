/*******************************
**FileName	�� MainFrame.h
**Creator	�� zwl	
**Date		�� 2014-01-14	
**Modifier	�� 
**ModifyDate�� 
**Comment	�� �������������
**Version	�� 1.0.0
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
	**@brief: ��ʼ��
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
	**@brief�������¼�֪ͨ
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
	* @brief : ��Ϣ��Ӧ���� 
	*/

	DUI_DECLARE_MESSAGE_MAP()

	bool OnBtnClose(void *lParam);
	bool OnBtnMin(void *lParam);
	bool OnBtnMax(void *lParam);
	bool OnBtnRestore(void *lParam);


	bool OnBtnAddChild(void *lParam);	// ����ӽڵ���Ϣ��Ӧ
	bool OnBtnAdd(void *lParam);		// ��ӽڵ���Ϣ��Ӧ
	bool OnBtnDelete(void *lParam);		// ɾ���ڵ���Ϣ��Ӧ

	bool OnBtnOpen(void *lParam);		// ���ļ���Ϣ��Ӧ
	bool OnBtnSave(void *lParam);		// �����ļ���Ϣ��Ӧ
	bool OnBtnExit(void *lParam);		// �ر��ļ���Ϣ��Ӧ

	bool Add(CTreeNodeUI* v_pCurNode);		// ��ӽڵ㷽��
	bool AddChild(CTreeNodeUI* v_pCurNode);	// ����ӽڵ㷽��
	bool Delete(CTreeViewUI* v_pTreeParent, CTreeNodeUI* v_pCurNode);	// ɾ���ڵ㷽��
	bool RemoveAllChild(CTreeNodeUI* v_pParentNode);	//�Ƴ����ڵ�֮������нڵ�
	bool Open(CTreeNodeUI* v_pRootNode, vector<pair<string, boost::any>> v_mapDict);//���ļ�����
	bool Save();	// �����ļ�����
	bool Exit();	// �ر��ļ����� �������ļ�

	bool OnTreeNodeNotify(void* lParam);	// TreeNode�ڵ���Ϣ��Ӧ

	void FadeIn() ;	// ������ʾʱ���Է���
	void FadeOut() ;// �����˳�ʱ��������

	bool LoadPList(CTreeNodeUI* v_pRootNode, CString v_strFileName, vector<pair<string, boost::any>> v_mapDict);//����Plist��xml�ļ�����
	bool SavePList(CTreeNodeUI* v_pRootNode, CString v_strSavePath);				// �����ļ�����
	bool AddArray(CTreeNodeUI* v_pParentNode, boost::any v_arrayNode);				//���array�ڵ㷽��
	bool AddDict(CTreeNodeUI* v_pParentNode, boost::any v_dictNode);				// ���dictionary�ڵ㷽��
	bool SaveArray(CTreeNodeUI* v_pParentNode, vector<boost::any>& v_arrayNode);	//����array�ڵ㷽��
	bool SaveDict(CTreeNodeUI* v_pParentNode, vector<pair<string, boost::any>>& v_dictNode);	// ����dictionary�ڵ㷽��

	//�ļ��Ϸ���Ϣ��Ӧ
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

	CTreeNodeUI*	m_pRootNode;//Root�ڵ�
	CTreeViewUI*	m_pTreeListXml;// ���οؼ�

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
