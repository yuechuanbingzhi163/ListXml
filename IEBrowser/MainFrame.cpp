/*******************************
**FileName	： MainFrame.h
**Creator	： zwl	
**Date		： 2014-01-14	
**Modifier	： 
**ModifyDate： 
**Comment	： 主窗口类的实现
**Version	： 1.0.0
/*******************************/
#include <StdAfx.h>
#include "MainFrame.h"
#include "resource.h"
#include <io.h>
#include "MenuWnd.h"
#include "PList/ParsePlistModel.h"

std::vector<char> base64Decode(const char* encodedData)
{
	using namespace std;

	vector<char> data;
	insert_iterator<vector<char> > ii(data, data.begin());
	base64<char> b64;
	int state = 0;
	b64.get(encodedData, encodedData + strlen(encodedData), ii, state); 

	return data;
}

void base64Encode(std::string& dataEncoded, const std::vector<char>& data)
{
	using namespace std;
	dataEncoded.clear();
	insert_iterator<string> ii(dataEncoded, dataEncoded.begin());
	base64<char> b64;
	int state = 0;

#if defined(_WIN32) || defined(_WIN64)
	b64.put(data.begin(), data.end(), ii, state , base64<>::crlf());
#else
	b64.put(data.begin(), data.end(), ii, state , base64<>::lf());
#endif

}


#define FADE_IN_TIMER 1
#define FADE_OUT_TIMER 2

#define FADE_STEP 10
#define TIMER_FREQUENCY 25


DUI_BEGIN_MESSAGE_MAP(CMainFrame, WindowImplBase)
// DUI_ON_MSGTYPE(DUI_MSGTYPE_TIMER, OnTimer)
DUI_END_MESSAGE_MAP()

CMainFrame::CMainFrame(LPCTSTR v_pStrSkinPath):CXMLWnd(v_pStrSkinPath)
{
	m_pStrSkinPath = v_pStrSkinPath;

	m_pBtnClose		= NULL;
	m_pBtnMin		= NULL;
	m_pBtnMax		= NULL;
	m_pBtnRestore	= NULL;


	m_pRootNode		= NULL;
	m_pTreeListXml	= NULL;
	m_pBtnAddChild	= NULL;
	m_pBtnAdd		= NULL;
	m_pBtnDelete	= NULL;
	m_pBtnOpen		= NULL;
	m_pBtnSave		= NULL;
	m_pBtnExit		= NULL;

	m_pTextTitle	= NULL;



	m_bExit = false;

}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CMainFrame::InitWindow(void)
{
	m_pBtnClose		= static_cast<CFadeButtonUI*>(m_PaintManager.FindControl(_T("BtnClose")));
	if(m_pBtnClose) m_pBtnClose->OnNotify += MakeDelegate(this, &CMainFrame::OnBtnClose);

	m_pBtnMin		= static_cast<CFadeButtonUI*>(m_PaintManager.FindControl(_T("BtnMin")));
	if(m_pBtnMin)   m_pBtnMin->OnNotify += MakeDelegate(this, &CMainFrame::OnBtnMin);

	m_pBtnMax		= static_cast<CFadeButtonUI*>(m_PaintManager.FindControl(_T("BtnMax")));
	if(m_pBtnMax)   m_pBtnMax->OnNotify += MakeDelegate(this, &CMainFrame::OnBtnMax);

	m_pBtnRestore   = static_cast<CFadeButtonUI*>(m_PaintManager.FindControl(_T("BtnRestore")));
	if(m_pBtnRestore)   m_pBtnRestore->OnNotify += MakeDelegate(this, &CMainFrame::OnBtnRestore);


	SetIcon(IDI_ICON1);

	// Add code
	m_pTreeListXml	= static_cast<CTreeViewUI*>(m_PaintManager.FindControl(_T("ListXml")));
	m_pRootNode		= static_cast<CTreeNodeUI*>(m_PaintManager.FindControl(_T("RootNode")));
	if(m_pRootNode) 
	{
		CComboUI* pComboChild = m_pRootNode->GetComboChild();
		if(pComboChild)
		{
			for (int i = pComboChild->GetCount()-1; i>1; i--)
			{
				pComboChild->RemoveAt(i);
			}
		}
		m_pRootNode->GetCheckBox()->SetVisible(true);
		m_pRootNode->SetItemText(_T("Dictionary"));
		m_pRootNode->SetComboItemIndex(1);
		m_pRootNode->SetItemText(_T("(0 Item)"), 2);
		m_pRootNode->OnNotify += MakeDelegate(this, &CMainFrame::OnTreeNodeNotify);
	}
	else
	{
		m_pRootNode = new CTreeNodeUI(m_pTreeListXml, NULL);
		if(m_pRootNode)
		{
			CComboUI* pComboChild = m_pRootNode->GetComboChild();
			if(pComboChild)
			{
				for (int i = pComboChild->GetCount()-1; i>1; i--)
				{
					pComboChild->RemoveAt(i);
				}
			}
			m_pRootNode->SetReadOnly(true);
			m_pRootNode->SetItemText(_T("Root"), 0);
			m_pRootNode->SetItemText(_T("Dictionary"));
			m_pRootNode->SetComboItemIndex(1);
			m_pRootNode->SetItemText(_T("(0 Item)"), 2);
			m_pRootNode->GetFolderButton()->ApplyAttributeList(_T("padding=\"0,1,0,0\" width=\"16\" height=\"16\" visible=\"true\" normalimage=\"file='Icon/treeview_b.png' source='0,0,16,16' \" hotimage=\"file='Icon/treeview_b.png' source='16,0,32,16' \" selectedimage=\"file='Icon/treeview_a.png' source='0,0,16,16' \" selectedhotimage=\"file='Icon/treeview_a.png' source='16,0,32,16' \""));
			m_pRootNode->GetCheckBox()->ApplyAttributeList(_T("width=\"16\" height=\"16\" visible=\"true\" normalimage=\"file='Icon/unchecked.png' \" selectedimage=\"file='Icon/checked.png' \""));
			m_pRootNode->GetComboChild()->SetMouseWheelEnable(false);
			m_pRootNode->OnNotify += MakeDelegate(this, &CMainFrame::OnTreeNodeNotify);
			m_pTreeListXml->Add(m_pRootNode);
		}
	}

	m_pBtnAddChild   =	static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("BtnAddChild")));
	if(m_pBtnAddChild)  m_pBtnAddChild->OnNotify += MakeDelegate(this, &CMainFrame::OnBtnAddChild);

	m_pBtnAdd		=	static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("BtnAdd")));
	if(m_pBtnAdd)		m_pBtnAdd->OnNotify += MakeDelegate(this, &CMainFrame::OnBtnAdd);

	m_pBtnDelete	=	static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("BtnDelete")));
	if(m_pBtnDelete)	m_pBtnDelete->OnNotify += MakeDelegate(this, &CMainFrame::OnBtnDelete);

	m_pBtnOpen		=	static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("BtnOpen")));
	if(m_pBtnOpen)		m_pBtnOpen->OnNotify += MakeDelegate(this, &CMainFrame::OnBtnOpen);
	m_pBtnSave		=	static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("BtnSave")));
	if(m_pBtnSave)		m_pBtnSave->OnNotify += MakeDelegate(this, &CMainFrame::OnBtnSave);
	m_pBtnExit		=	static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("BtnExit")));
	if(m_pBtnExit)		m_pBtnExit->OnNotify += MakeDelegate(this, &CMainFrame::OnBtnExit);

	m_pTextTitle	=	static_cast<CTextUI*>(m_PaintManager.FindControl(_T("TextTitle")));

	DragAcceptFiles(m_hWnd,TRUE);

// 	LoadPList(m_pRootNode, _T("MeituMV.xml"), m_mapDict);

	//阴影效果
	CWndShadow::Initialize(AfxGetInstanceHandle());
	m_wndShadow.Create(m_hWnd);
	m_wndShadow.SetSize(5);
	m_wndShadow.SetPosition(0,0);


	CenterWindow();

}

bool CMainFrame::Add( CTreeNodeUI* v_pCurNode )
{
	CTreeNodeUI* pChildRoot = new CTreeNodeUI(v_pCurNode->GetTreeParent(), v_pCurNode);
	if(pChildRoot && v_pCurNode && v_pCurNode->GetParentNode())
	{
		CTreeNodeUI* pParentNode = v_pCurNode->GetParentNode();
		CString strItemText = pParentNode->GetComboItemText(pParentNode->GetComboItemIndex());
		if(0 == strItemText.CompareNoCase(_T("Array")))
			strItemText.Format(_T("Item %d"), pParentNode->GetCountChild());
		else
			strItemText.Format(_T("New Item - %d"), pParentNode->GetCountChild());


		pChildRoot->SetItemText(strItemText, 0);
		pChildRoot->SetItemText(_T("String"), 1);
		pChildRoot->SetComboItemIndex(_T("String"));
		pChildRoot->SetItemText(_T(""), 2);

		pChildRoot->GetFolderButton()->ApplyAttributeList(_T("padding=\"0,1,0,0\" width=\"16\" height=\"16\" visible=\"false\" normalimage=\"file='Icon/treeview_b.png' source='0,0,16,16' \" hotimage=\"file='Icon/treeview_b.png' source='16,0,32,16' \" selectedimage=\"file='Icon/treeview_a.png' source='0,0,16,16' \" selectedhotimage=\"file='Icon/treeview_a.png' source='16,0,32,16' \""));
		pChildRoot->GetCheckBox()->ApplyAttributeList(_T("width=\"16\" height=\"16\" visible=\"true\" normalimage=\"file='Icon/unchecked.png' \" selectedimage=\"file='Icon/checked.png' \""));
		pChildRoot->GetItemButton()->ApplyAttributeList(_T("visible=\"false\""));
		pChildRoot->OnNotify += MakeDelegate(this, &CMainFrame::OnTreeNodeNotify);
		
		int nIndex = v_pCurNode->GetNodeIndex();
		int nCount = pParentNode->GetCountChild();
		bool bResult = false;
		if(nIndex < nCount-1)
			bResult = pParentNode->AddAt(pChildRoot, nIndex+1);
		else
			bResult = pParentNode->Add(pChildRoot);
		if( !bResult )
		{
			delete pChildRoot;
			return false;
		}
		else
		{
			CDuiString strText;
			strText.Format(_T("(%d Item)"), pParentNode->GetCountChild()>0?pParentNode->GetCountChild():0);
			pParentNode->SetItemText(strText, 2);
			pParentNode->GetFolderButton()->SetVisible(true);
		}

		return true;
	}

	return false;
}
bool CMainFrame::AddChild( CTreeNodeUI* v_pCurNode )
{
	if(v_pCurNode == NULL)
		return false;

	CTreeNodeUI* pChildRoot = new CTreeNodeUI(v_pCurNode->GetTreeParent(), v_pCurNode);
	CString strItemText = v_pCurNode->GetComboItemText(v_pCurNode->GetComboItemIndex());
	if(pChildRoot && ( (0 == strItemText.CompareNoCase(_T("Array"))) || (0 == strItemText.CompareNoCase(_T("Dictionary")))) )
	{
		if(0 == strItemText.CompareNoCase(_T("Array")))
			strItemText.Format(_T("Item %d"), v_pCurNode->GetCountChild());
		else
			strItemText.Format(_T("New Item - %d"), v_pCurNode->GetCountChild());
		pChildRoot->SetItemText(strItemText, 0);
		pChildRoot->SetItemText(_T("String"), 1);
		pChildRoot->SetComboItemIndex(_T("String"));
		pChildRoot->SetItemText(_T(""), 2);

		pChildRoot->GetFolderButton()->ApplyAttributeList(_T("padding=\"0,1,0,0\" width=\"16\" height=\"16\" visible=\"false\" normalimage=\"file='Icon/treeview_b.png' source='0,0,16,16' \" hotimage=\"file='Icon/treeview_b.png' source='16,0,32,16' \" selectedimage=\"file='Icon/treeview_a.png' source='0,0,16,16' \" selectedhotimage=\"file='Icon/treeview_a.png' source='16,0,32,16' \""));
		pChildRoot->GetCheckBox()->ApplyAttributeList(_T("width=\"16\" height=\"16\" visible=\"true\" normalimage=\"file='Icon/unchecked.png' \" selectedimage=\"file='Icon/checked.png' \""));
		pChildRoot->GetItemButton()->ApplyAttributeList(_T("visible=\"false\""));
		pChildRoot->OnNotify += MakeDelegate(this, &CMainFrame::OnTreeNodeNotify);
// 		if(!v_pCurNode->AddAt(pChildRoot, 0))
		if(!v_pCurNode->Add(pChildRoot))
		{
			delete pChildRoot;
			return false;
		}
		else
		{
			CDuiString strText;
			strText.Format(_T("(%d Item)"), v_pCurNode->GetCountChild()>0?v_pCurNode->GetCountChild():0);
			v_pCurNode->SetItemText(strText, 2);
			v_pCurNode->GetFolderButton()->SetVisible(true);
		}

		return true;
	}

	return false;
}
bool CMainFrame::Delete( CTreeViewUI* v_pTreeParent, CTreeNodeUI* v_pCurNode )
{
	if(!v_pTreeParent || !v_pCurNode || !v_pCurNode->GetParentNode() ) //Root节点不能删除
		return false;


	CTreeNodeUI* pParentNode = v_pCurNode->GetParentNode();
	int nCount = pParentNode->GetCountChild();
	if(pParentNode->Remove(v_pCurNode))
	{
		CDuiString strText;
		strText.Format(_T("(%d Item)"), (nCount-1)>0?(nCount-1):0);
		pParentNode->SetItemText(strText, 2);
		if(pParentNode->GetCountChild() == 0)
			pParentNode->GetFolderButton()->SetVisible(false);
		return true;
	}
	else
	{
		return false;
	}
}

bool CMainFrame::RemoveAllChild(CTreeNodeUI* v_pParentNode)
{
	if(!v_pParentNode)
		return false;

	int nCount = v_pParentNode->GetCountChild();
	for (int i = nCount-1; i>=0; i--)
	{
		CTreeNodeUI* pChildNode = v_pParentNode->GetChildNode(i);
		if(pChildNode)
			v_pParentNode->RemoveAt(pChildNode);
	}
	
	return true;
}

bool CMainFrame::Open(CTreeNodeUI* v_pRootNode, vector<pair<string, boost::any>> v_mapDict)
{
	if(!v_pRootNode)
		return false;

	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,_T("Plist Files(*.plist)|*.plist|XML Files(*.xml)|*.xml|All File(*.*)|*.*||"),CWnd::FromHandle(m_hWnd));
	dlg.m_ofn.lpstrTitle = _T("打开文件");
	if(dlg.DoModal() == IDOK)
	{
		RemoveAllChild(v_pRootNode);
		return LoadPList(m_pRootNode, dlg.GetPathName(), v_mapDict);
	}
	else
	{
		return false;
	}
}
bool CMainFrame::Save()
{
	CFileDialog dlg(FALSE, _T("*.plist"), _T("test"), OFN_HIDEREADONLY, _T("Plist Files(*.plist)|*.plist|XML Files(*.xml)|*.xml|All Files(*.*)|*.*||"), CWnd::FromHandle(m_hWnd));
	dlg.m_ofn.lpstrTitle = _T("打开文件");
	if(dlg.DoModal() == IDOK)
	{
		CString strFilaName = dlg.GetPathName();
		return SavePList(m_pRootNode, strFilaName);
	}
	else
	{
		return false;
	}
}
bool CMainFrame::Exit()
{
	if(m_pTextTitle)
		m_pTextTitle->SetText(_T("ListXml编辑器"));

	RemoveAllChild(m_pRootNode);

	return false;
}

bool CMainFrame::OnBtnAddChild(void *lParam)
{
	TNotifyUI* pNotify = (TNotifyUI*)lParam;
	if(pNotify->sType == DUI_MSGTYPE_CLICK)
	{
		if(m_pTreeListXml)
		{
			CTreeNodeUI* pCurNode = (CTreeNodeUI*)m_pTreeListXml->GetItemAt(m_pTreeListXml->GetCurSel());
			if(pCurNode)
			{
				AddChild(pCurNode);
			}
		}
	}

	return true;
}
bool CMainFrame::OnBtnAdd(void *lParam)
{
	TNotifyUI* pNotify = (TNotifyUI*)lParam;
	if(pNotify->sType == DUI_MSGTYPE_CLICK)
	{
		if(m_pTreeListXml)
		{
			CTreeNodeUI* pCurNode = (CTreeNodeUI*)m_pTreeListXml->GetItemAt(m_pTreeListXml->GetCurSel());
			if(pCurNode)
			{
				Add(pCurNode);
			}
		}
	}

	return true;
}
bool CMainFrame::OnBtnDelete(void *lParam)
{
	TNotifyUI* pNotify = (TNotifyUI*)lParam;
	if(pNotify->sType == DUI_MSGTYPE_CLICK)
	{

		if(m_pTreeListXml)
		{
			CTreeNodeUI* pCurNode = (CTreeNodeUI*)m_pTreeListXml->GetItemAt(m_pTreeListXml->GetCurSel());
			if(pCurNode)
			{
				Delete(m_pTreeListXml, pCurNode);
			}
		}
	}

	return true;
}

bool CMainFrame::OnBtnOpen(void *lParam)
{
	TNotifyUI* pNotify = (TNotifyUI*)lParam;
	if(pNotify->sType == DUI_MSGTYPE_CLICK)
	{
		Open(m_pRootNode, m_mapDict);
	}

	return true;
}
bool CMainFrame::OnBtnSave(void *lParam)
{
	TNotifyUI* pNotify = (TNotifyUI*)lParam;
	if(pNotify->sType == DUI_MSGTYPE_CLICK)
	{
		if(m_pRootNode && m_pRootNode->GetCountChild()>0)
			Save();
	}

	return true;
}
bool CMainFrame::OnBtnExit(void *lParam)
{
	TNotifyUI* pNotify = (TNotifyUI*)lParam;
	if(pNotify->sType == DUI_MSGTYPE_CLICK)
	{
		Exit();
	}

	return true;
}

bool CMainFrame::OnTreeNodeNotify(void* lParam)
{
	TNotifyUI* pNotify = (TNotifyUI*)lParam;
	if(pNotify->sType == _T("nodeitemselect"))// 树节点里面的Combo有选项选中
	{
		CTreeNodeUI* pCurNode = (CTreeNodeUI*)pNotify->pSender;
		if(pCurNode)
		{
			CDuiString strName = pCurNode->GetComboItemText(pCurNode->GetComboItemIndex());
			if( (0 != strName.Compare(_T("Array"))) && (0 != strName.Compare(_T("Dictionary"))) )
			{
				if(pCurNode->GetParentNode())
				{
					int nCount = pCurNode->GetCountChild();
					for (int i = nCount-1; i>=0; i--)
					{
						pCurNode->Remove(pCurNode->GetChildNode(i));
					}
				}
				else
				{
					pCurNode->SetComboItemIndex(strName);
				}
				pCurNode->SetReadOnly(false);
			}
			else
			{
				pCurNode->SetReadOnly(true);
			}
			if( 0 == strName.CompareNoCase(_T("Array")) )
			{
				CDuiString strText;
				strText.Format(_T("(%d Item)"), pCurNode->GetCountChild()>0?pCurNode->GetCountChild():0);
				pCurNode->SetItemText(strText, 2);
			}
			if( 0 == strName.CompareNoCase(_T("Dictionary")) )
			{
				CDuiString strText;
				strText.Format(_T("(%d Item)"), pCurNode->GetCountChild()>0?pCurNode->GetCountChild():0);
				pCurNode->SetItemText(strText, 2);
			}
			if( 0 == strName.CompareNoCase(_T("Boolean")) )
			{
				pCurNode->SetItemText(_T("NO"), 2);
			}
			if( 0 == strName.CompareNoCase(_T("Data")) )
			{
				pCurNode->SetItemText(_T("Data"), 2);
			}
			if( 0 == strName.CompareNoCase(_T("Date")) )
			{	
				time_t curtime=time(0); 
				tm *tim =localtime(&curtime); 
				CString strCurTime;
				strCurTime.Format(_T("%04d-%02d-%02dT%02d:%02d:%02dZ"), tim->tm_year+1900, tim->tm_mon+1, tim->tm_mday, tim->tm_hour, tim->tm_min, tim->tm_sec);

				pCurNode->SetItemText(strCurTime, 2);
			}
			if( 0 == strName.CompareNoCase(_T("Number")) )
			{
				pCurNode->SetItemText(_T("0"), 2);
			}
			if( 0 == strName.CompareNoCase(_T("String")) )
			{
				pCurNode->SetItemText(_T(""), 2);
			}
		}
	}
	else if(pNotify->sType == DUI_MSGTYPE_ITEMCLICK)
	{
		CTreeNodeUI* pCurNode = (CTreeNodeUI*)m_pTreeListXml->GetItemAt(m_pTreeListXml->GetCurSel());
		if(pCurNode)
		{
			CString strText = pCurNode->GetComboItemText(pCurNode->GetComboItemIndex());
			if( (0 != strText.Compare(_T("Array"))) && (0 != strText.Compare(_T("Dictionary"))) )
			{
				m_pBtnAddChild->SetEnabled(false);
				m_pBtnAdd->SetEnabled(true);
			}
			else
			{
				m_pBtnAddChild->SetEnabled(true);
				m_pBtnAdd->SetEnabled(true);
			}
		}
		else
		{
			m_pBtnAddChild->SetEnabled(false);
			m_pBtnAdd->SetEnabled(false);
		}
	}

	return true;
}

bool CMainFrame::LoadPList(CTreeNodeUI* v_pRootNode, CString v_strFileName, vector<pair<string, boost::any>> v_mapDict)
{
	if(!v_pRootNode || v_strFileName.IsEmpty())
		return false;

	char szFileName[MAX_PATH] = {0};

	USES_CONVERSION;
	strcpy(szFileName, W2A(v_strFileName.GetBuffer(0)));
	v_strFileName.ReleaseBuffer();
	m_mapDict.clear();
	m_arrayRoot.clear();
	Plist::readPlist(szFileName, m_mapDict, m_arrayRoot);

	if(m_mapDict.size() > 0)
	{
		v_pRootNode->SetItemText(_T("Dictionary"), 1);
		v_pRootNode->SetComboItemIndex(_T("Dictionary"));
		CString cstrValue;
		cstrValue.Format(_T("(%d Item)"), m_mapDict.size());
		v_pRootNode->SetItemText(cstrValue, 2);

		AddDict(v_pRootNode, m_mapDict);
	}
	else if(m_arrayRoot.size() > 0)
	{
		v_pRootNode->SetItemText(_T("Array"), 1);
		v_pRootNode->SetComboItemIndex(_T("Array"));
		CString cstrValue;
		cstrValue.Format(_T("(%d Item)"), m_arrayRoot.size());
		v_pRootNode->SetItemText(cstrValue, 2);

		AddArray(v_pRootNode, m_arrayRoot);
	}
	else
	{
		return false;
	}


	if(m_pTextTitle)
		m_pTextTitle->SetText(v_strFileName);

	return true;
}

bool CMainFrame::SavePList(CTreeNodeUI* v_pRootNode, CString v_strSavePath)
{
	if(!v_pRootNode || v_strSavePath.IsEmpty())
		return false;

	PListModel::ObjectType otype = PListModel::UNKNOW;
	CString strType = v_pRootNode->GetComboItemText(v_pRootNode->GetComboItemIndex());
	if( 0 == strType.CompareNoCase(_T("Dictionary")) )
	{
		m_saveDict.clear();
		SaveDict(v_pRootNode, m_saveDict);
		otype = PListModel::PLS_MAP;
	}
	else if( 0 == strType.CompareNoCase(_T("Array")) )
	{
		m_aSaveRoot.clear();
		SaveArray(v_pRootNode, m_aSaveRoot);
		otype = PListModel::PLS_ARRAY;
	}
	else
	{
		return false;
	}


	char szFileName[MAX_PATH] = {0};
	USES_CONVERSION;
	strcpy(szFileName, W2A(v_strSavePath.GetBuffer(0)));
	v_strSavePath.ReleaseBuffer();
	if(otype == PListModel::PLS_MAP)
		Plist::writePlistXML(szFileName, m_saveDict);
	else if( otype == PListModel::PLS_ARRAY )
		Plist::writePlistXML(szFileName, m_aSaveRoot);
	if(m_pTextTitle)
		m_pTextTitle->SetText(v_strSavePath);

	return true;
}

bool CMainFrame::AddArray(CTreeNodeUI* v_pParentNode, boost::any v_arrayNode)
{
	static boost::any aString = string();
	static boost::any aDouble = double(0);
	static boost::any aFloat = float(0);
	static boost::any anInt32 = int32_t(0);
	static boost::any anInt64 = int64_t(0);
	static boost::any aLong = long(0);
	static boost::any aShort = short(0);
	static boost::any anArray = vector<boost::any>();
// 	static boost::any aMap = map<string, boost::any>();
	static boost::any aMap = vector<pair<string, boost::any>>();
	static boost::any aByteArray = vector<char>();
	static boost::any aPlistDate = PlistDate();
	static boost::any aPlistBool = bool(true);

	if(v_arrayNode.type() != anArray.type())
		return false;


	PlistDate dateValue;
	string	strValue;
	bool bValue;
	int  nValue;
	short uValue;
	float fValue;
	double dValue;

	vector<boost::any> vArray = (boost::any_cast<vector<boost::any>>)(v_arrayNode);
	int nCount = vArray.size();
	for(int i = 0; i < nCount; i++)
	{
		boost::any obj = (boost::any)vArray[i];

		CTreeNodeUI* pChildRoot = new CTreeNodeUI(v_pParentNode->GetTreeParent(), v_pParentNode);
		if(!pChildRoot)
			continue;

		PListModel::ObjectType otype = PListModel::UNKNOW;
		CString cstrKey(_T(""));
		CString cstrType(_T(""));
		CString cstrValue(_T(""));

		cstrKey.Format(_T("Item %d"), i);
		pChildRoot->SetItemText(cstrKey, 0);
		
		if(obj.type() == aMap.type())
		{
			cstrType.Format(_T("Dictionary"));
// 			const map<string, boost::any>& pDictChild = (boost::any_cast<const map<string, boost::any>& >)(obj);
			const vector<pair<string, boost::any>>& pDictChild = (boost::any_cast<const vector<pair<string, boost::any>>& >)(obj);
			cstrValue.Format(_T("(%d Item)"), pDictChild.size());

			otype = PListModel::PLS_MAP;
		}
		else if(obj.type() == anArray.type())
		{
			cstrType.Format(_T("Array"));
			vector<boost::any> vArray = (boost::any_cast<vector<boost::any>>)(obj);
			cstrValue.Format(_T("(%d Item)"), vArray.size());

			otype = PListModel::PLS_ARRAY;
		}
		else
		{
			if(obj.type() == aPlistDate.type())		// Date
			{
				cstrType.Format(_T("Date"));
				dateValue = (boost::any_cast<const PlistDate&>)(obj);
				USES_CONVERSION;
				cstrValue.Format(_T("%s"), A2W(dateValue.timeAsXMLConvention().c_str()));
			}
			else if(obj.type() == aString.type())	// String
			{
				cstrType.Format(_T("String"));
				strValue = (boost::any_cast<const string&>)(obj);
				wchar_t* pResult = G_Utf8ToUnicode(strValue.c_str());
				cstrValue.Format(_T("%s"), pResult);
				if(pResult)
				{
					delete []pResult;
					pResult = NULL;
				}
			}
			else if(obj.type() == aPlistBool.type()) // bool
			{
				cstrType.Format(_T("Boolean"));
				bValue = (boost::any_cast<const bool&>)(obj);
				cstrValue.Format(_T("%s"), bValue?_T("YES"):_T("NO"));
			}
			else if(obj.type() == aByteArray.type())	// Data
			{
				cstrType.Format(_T("Data"));
				vector<char> pDataValue = (boost::any_cast<const vector<char>&>)(obj);
				string strTmp;
				base64Encode(strTmp, pDataValue);
				wchar_t *strUnicode = G_AnsiToUnicode((char*)strTmp.c_str());
				cstrValue.Format(_T("%s"), strUnicode);
				if(strUnicode) 
				{
					delete []strUnicode;
					strUnicode = NULL;
				}
			}
			else
			{
				cstrType.Format(_T("Number"));
				if(obj.type() == aFloat.type())
				{
					cstrValue.Format(_T("%f"), (boost::any_cast<const double&>)(obj) );
				}
				else if(obj.type() == aDouble.type())
				{
					cstrValue.Format(_T("%lf"), (boost::any_cast<const double&>)(obj) );
				}
				else if(obj.type() == anInt32.type())
				{
					cstrValue.Format(_T("%d"), (boost::any_cast<const int32_t&>)(obj) );
				}
				else if(obj.type() == anInt64.type())
				{
					cstrValue.Format(_T("%d"), (boost::any_cast<const int64_t&>)(obj) );
				}
				else if(obj.type() == aLong.type())
				{
					cstrValue.Format(_T("%ld"), (boost::any_cast<const long&>)(obj) );
				}
				else if(obj.type() == aShort.type())
				{
					cstrValue.Format(_T("%d"), (boost::any_cast<const short&>)(obj) );
				}
			}
		}
		pChildRoot->SetItemText(cstrType, 1);
		pChildRoot->SetComboItemIndex(cstrType);
		pChildRoot->SetItemText(cstrValue, 2);

		pChildRoot->GetFolderButton()->ApplyAttributeList(_T("padding=\"0,1,0,0\" width=\"16\" height=\"16\" visible=\"true\" normalimage=\"file='Icon/treeview_b.png' source='0,0,16,16' \" hotimage=\"file='Icon/treeview_b.png' source='16,0,32,16' \" selectedimage=\"file='Icon/treeview_a.png' source='0,0,16,16' \" selectedhotimage=\"file='Icon/treeview_a.png' source='16,0,32,16' \""));
		pChildRoot->GetCheckBox()->ApplyAttributeList(_T("width=\"16\" height=\"16\" visible=\"true\" normalimage=\"file='Icon/unchecked.png' \" selectedimage=\"file='Icon/checked.png' \""));
		pChildRoot->GetComboChild()->SetMouseWheelEnable(false);
// 				pChildRoot->GetItemButton()->ApplyAttributeList(_T("visible=\"false\""));
		pChildRoot->OnNotify += MakeDelegate(this, &CMainFrame::OnTreeNodeNotify);
// 		if(!v_pParentNode->AddAt(pChildRoot, i))
// 			delete pChildRoot;
		if(!v_pParentNode->AddChildNode(pChildRoot))
			delete pChildRoot;

		if(otype == PListModel::PLS_MAP)
		{
			AddDict(pChildRoot, obj);
		}
		else if(otype == PListModel::PLS_ARRAY)
		{
			AddArray(pChildRoot, obj);
		}
		else
		{
			pChildRoot->GetFolderButton()->SetVisible(false);
		}
	}

	return true;
}
bool CMainFrame::AddDict(CTreeNodeUI* v_pParentNode, boost::any v_dictNode)
{
	static boost::any aString = string();
	static boost::any aDouble = double(0);
	static boost::any aFloat = float(0);
	static boost::any anInt32 = int32_t(0);
	static boost::any anInt64 = int64_t(0);
	static boost::any aLong = long(0);
	static boost::any aShort = short(0);
	static boost::any anArray = vector<boost::any>();
// 	static boost::any aMap = map<string, boost::any>();
	static boost::any aMap = vector<pair<string, boost::any>>();
	static boost::any aByteArray = vector<char>();
	static boost::any aPlistDate = PlistDate();
	static boost::any aPlistBool = bool(true);

	if(v_dictNode.type() != aMap.type())
		return false;


	PlistDate dateValue;
	string	strValue;
	bool bValue;
	int  nValue;
	short uValue;
	float fValue;
	double dValue;

// 	map<string, boost::any>& dict = (boost::any_cast<map<string, boost::any>& >)(v_dictNode);
// 	map<string,boost::any>::iterator it = dict.begin();
	vector<pair<string, boost::any>>& dict = (boost::any_cast<vector<pair<string, boost::any>>& >)(v_dictNode);

// 	int i = 0;
// 	for(it;it!=dict.end();++it)
	int nCount = dict.size();
	for (int i = 0; i<nCount; i++)
	{
// 		boost::any obj = (boost::any)it->second;
		pair<string, boost::any>& pairTmp = (pair<string, boost::any>)dict[i];
		boost::any obj = (boost::any)pairTmp.second;

		CTreeNodeUI* pChildRoot = new CTreeNodeUI(v_pParentNode->GetTreeParent(), v_pParentNode);
		if(!pChildRoot)
			continue;

		PListModel::ObjectType otype = PListModel::UNKNOW;
		CString cstrKey(_T(""));
		CString cstrType(_T(""));
		CString cstrValue(_T(""));

		USES_CONVERSION;
// 		cstrKey = A2W(it->first.c_str());
		cstrKey = A2W(pairTmp.first.c_str());
		pChildRoot->SetItemText(cstrKey, 0);

		if(obj.type() == aMap.type())
		{
			cstrType.Format(_T("Dictionary"));
// 			const map<string, boost::any>& pDictChild = (boost::any_cast<const map<string, boost::any>& >)(obj);
			const vector<pair<string, boost::any>>& pDictChild = (boost::any_cast<const vector<pair<string, boost::any>>& >)(obj);
			cstrValue.Format(_T("(%d Item)"), pDictChild.size());

			otype = PListModel::PLS_MAP;
		}
		else if(obj.type() == anArray.type())
		{
			cstrType.Format(_T("Array"));
			vector<boost::any> vArray = (boost::any_cast<vector<boost::any>>)(obj);
			cstrValue.Format(_T("(%d Item)"), vArray.size());

			otype = PListModel::PLS_ARRAY;
		}
		else
		{
			if(obj.type() == aPlistDate.type())		// Date
			{
				cstrType.Format(_T("Date"));
				dateValue = (boost::any_cast<const PlistDate&>)(obj);
				USES_CONVERSION;
				cstrValue.Format(_T("%s"), A2W(dateValue.timeAsXMLConvention().c_str()));
			}
			else if(obj.type() == aString.type())	// String
			{
				cstrType.Format(_T("String"));
				strValue = (boost::any_cast<const string&>)(obj);
				wchar_t* pResult = G_Utf8ToUnicode(strValue.c_str());
				cstrValue.Format(_T("%s"), pResult);
				if(pResult)
				{
					delete []pResult;
					pResult = NULL;
				}
			}
			else if(obj.type() == aPlistBool.type()) // bool
			{
				cstrType.Format(_T("Boolean"));
				bValue = (boost::any_cast<const bool&>)(obj);
				cstrValue.Format(_T("%s"), bValue?_T("YES"):_T("NO"));
			}
			else if(obj.type() == aByteArray.type())	// Data
			{
				cstrType.Format(_T("Data"));
				vector<char> pDataValue = (boost::any_cast<const vector<char>&>)(obj);
				string strTmp;
				base64Encode(strTmp, pDataValue);
				wchar_t *strUnicode = G_AnsiToUnicode((char*)strTmp.c_str());
				cstrValue.Format(_T("%s"), strUnicode);
				if(strUnicode) 
				{
					delete []strUnicode;
					strUnicode = NULL;
				}
			}
			else
			{
				cstrType.Format(_T("Number"));
				if(obj.type() == aFloat.type())
				{
					cstrValue.Format(_T("%f"), (boost::any_cast<const double&>)(obj) );
				}
				else if(obj.type() == aDouble.type())
				{
					cstrValue.Format(_T("%lf"), (boost::any_cast<const double&>)(obj) );
				}
				else if(obj.type() == anInt32.type())
				{
					cstrValue.Format(_T("%d"), (boost::any_cast<const int32_t&>)(obj) );
				}
				else if(obj.type() == anInt64.type())
				{
					cstrValue.Format(_T("%d"), (boost::any_cast<const int64_t&>)(obj) );
				}
				else if(obj.type() == aLong.type())
				{
					cstrValue.Format(_T("%ld"), (boost::any_cast<const long&>)(obj) );
				}
				else if(obj.type() == aShort.type())
				{
					cstrValue.Format(_T("%d"), (boost::any_cast<const short&>)(obj) );
				}
			}
		}
		pChildRoot->SetItemText(cstrType, 1);
		pChildRoot->SetComboItemIndex(cstrType);
		pChildRoot->SetItemText(cstrValue, 2);

		pChildRoot->GetFolderButton()->ApplyAttributeList(_T("padding=\"0,1,0,0\" width=\"16\" height=\"16\" visible=\"true\" normalimage=\"file='Icon/treeview_b.png' source='0,0,16,16' \" hotimage=\"file='Icon/treeview_b.png' source='16,0,32,16' \" selectedimage=\"file='Icon/treeview_a.png' source='0,0,16,16' \" selectedhotimage=\"file='Icon/treeview_a.png' source='16,0,32,16' \""));
		pChildRoot->GetCheckBox()->ApplyAttributeList(_T("width=\"16\" height=\"16\" visible=\"true\" normalimage=\"file='Icon/unchecked.png' \" selectedimage=\"file='Icon/checked.png' \""));
		pChildRoot->GetComboChild()->SetMouseWheelEnable(false);
// 				pChildRoot->GetItemButton()->ApplyAttributeList(_T("visible=\"false\""));
		pChildRoot->OnNotify += MakeDelegate(this, &CMainFrame::OnTreeNodeNotify);
// 		if(!v_pParentNode->AddAt(pChildRoot, i++))
// 			delete pChildRoot;
		if(!v_pParentNode->AddChildNode(pChildRoot))
			delete pChildRoot;

		if(otype == PListModel::PLS_MAP)
		{
// 			AddDict(pChildRoot, it->second);
			AddDict(pChildRoot, obj);
		}
		else if(otype == PListModel::PLS_ARRAY)
		{
// 			AddArray(pChildRoot, it->second);
			AddArray(pChildRoot, obj);
		}
		else
		{
			pChildRoot->GetFolderButton()->SetVisible(false);
		}
	}

	return true;
}

bool CMainFrame::SaveArray(CTreeNodeUI* v_pParentNode, vector<boost::any>& v_arrayNode)
{
	if(!v_pParentNode )
		return false;

	v_arrayNode.clear();

	int nCount = v_pParentNode->GetCountChild();
	for (int i = 0; i<nCount; i++)
	{
		CTreeNodeUI* pChildNode = v_pParentNode->GetChildNode(i);
		if(pChildNode)
		{
			CString strText = pChildNode->GetComboItemText(pChildNode->GetComboItemIndex());
			if(0 == strText.CompareNoCase(_T("Array")))
			{
				vector<boost::any> anArray;
				SaveArray(pChildNode, anArray);
				v_arrayNode.push_back(anArray);
			}
			else if(0 == strText.CompareNoCase(_T("Dictionary")))
			{
				vector<pair<string, boost::any>> aMap;
				SaveDict(pChildNode, aMap);
				v_arrayNode.push_back(aMap);
			}
			else
			{
				if(0 == strText.CompareNoCase(_T("String")))
				{
					char *pszText = G_UnicodeToUtf8(pChildNode->GetItemText(2).GetData());
					boost::any aString = string(pszText);
					if(pszText)
					{
						delete []pszText;
						pszText = NULL;
					}
					v_arrayNode.push_back(aString);
				}
				else if(0 == strText.CompareNoCase(_T("Date")))
				{
					PlistDate aDate;
					char *pszText = G_UnicodeToAnsi(pChildNode->GetItemText(2).GetData());
					aDate.setTimeFromXMLConvention(pszText);
					if(pszText)
					{
						delete []pszText;
						pszText = NULL;
					}
					v_arrayNode.push_back(aDate);

				}
				else if(0 == strText.CompareNoCase(_T("Data")))
				{
					char *pszText = G_UnicodeToAnsi(pChildNode->GetItemText(2).GetData());
					vector<char> aByteArray = base64Decode(pszText);
					if(pszText)
					{
						delete []pszText;
						pszText = NULL;
					}
					v_arrayNode.push_back(aByteArray);
				}
				else if(0 == strText.CompareNoCase(_T("Boolean")))
				{
					CString strValue = pChildNode->GetItemText(2);
					boost::any aPlistBool = (0 == strValue.CompareNoCase(_T("YES")))?bool(true):bool(false);
					v_arrayNode.push_back(aPlistBool);
				}
				else if(0 == strText.CompareNoCase(_T("Number")))
				{
					CString strValue = pChildNode->GetItemText(2);
					if(strValue == strValue.SpanIncluding(_T("-0123456789")) )
					{
						boost::any aInt = _wtoi(strValue);
						v_arrayNode.push_back(aInt);
					}
					else if(strValue == strValue.SpanIncluding(_T("-0123456789.")))
					{
						boost::any aDouble = _wtof(strValue);
						v_arrayNode.push_back(aDouble);
					}
					else
					{
						boost::any aInt = (int)0;
						v_arrayNode.push_back(aInt);
					}
				}
			}
		}
	}
	

	return true;
}
bool CMainFrame::SaveDict(CTreeNodeUI* v_pParentNode, vector<pair<string, boost::any>>& v_dictNode)
{
	if(!v_pParentNode)
		return false;

	v_dictNode.clear();

	int nCount = v_pParentNode->GetCountChild();
	for (int i = 0; i<nCount; i++)
	{
		CTreeNodeUI* pChildNode = v_pParentNode->GetChildNode(i);
		if(pChildNode)
		{
			CString strText = pChildNode->GetComboItemText(pChildNode->GetComboItemIndex());
			if(0 == strText.CompareNoCase(_T("Array")))
			{
				vector<boost::any> anArray;
				SaveArray(pChildNode, anArray);
				char *pszText = G_UnicodeToAnsi(pChildNode->GetItemText(0).GetData());
// 				v_dictNode[string(pszText)] = anArray;
				pair<string, boost::any> pairTmp(string(pszText), anArray);
				v_dictNode.push_back(pairTmp);
				if(pszText)
				{
					delete []pszText;
					pszText = NULL;
				}
			}
			else if(0 == strText.CompareNoCase(_T("Dictionary")))
			{
				vector<pair<string, boost::any>> aMap;
				SaveDict(pChildNode, aMap);

				char *pszText = G_UnicodeToAnsi(pChildNode->GetItemText(0).GetData());
// 				v_dictNode[string(pszText)] = aMap;
				pair<string, boost::any> pairTmp(string(pszText), aMap);
				v_dictNode.push_back(pairTmp);

				if(pszText)
				{
					delete []pszText;
					pszText = NULL;
				}
			}
			else
			{
				if(0 == strText.CompareNoCase(_T("String")))
				{
					char *pszText = G_UnicodeToUtf8(pChildNode->GetItemText(2).GetData());
					boost::any aString = string(pszText);

					if(pszText)
					{
						delete []pszText;
					}
					pszText = G_UnicodeToAnsi(pChildNode->GetItemText(0).GetData());
// 					v_dictNode[string(pszText)] = aString;
					pair<string, boost::any> pairTmp(string(pszText), aString);
					v_dictNode.push_back(pairTmp);

					if(pszText)
					{
						delete []pszText;
						pszText = NULL;
					}
				}
				else if(0 == strText.CompareNoCase(_T("Date")))
				{
					PlistDate aDate;
					char *pszText = G_UnicodeToAnsi(pChildNode->GetItemText(2).GetData());
					aDate.setTimeFromXMLConvention(pszText);
					if(pszText)
					{
						delete []pszText;
					}

					pszText = G_UnicodeToAnsi(pChildNode->GetItemText(0).GetData());
// 					v_dictNode[string(pszText)] = aDate;
					pair<string, boost::any> pairTmp(string(pszText), aDate);
					v_dictNode.push_back(pairTmp);

					if(pszText)
					{
						delete []pszText;
						pszText = NULL;
					}

				}
				else if(0 == strText.CompareNoCase(_T("Data")))
				{
					char *pszText = G_UnicodeToAnsi(pChildNode->GetItemText(2).GetData());
					vector<char> aByteArray = base64Decode(pszText);
					if(pszText)
						delete []pszText;

					pszText = G_UnicodeToAnsi(pChildNode->GetItemText(0).GetData());
// 					v_dictNode[string(pszText)] = aByteArray;
					pair<string, boost::any> pairTmp(string(pszText), aByteArray);
					v_dictNode.push_back(pairTmp);
					if(pszText)
					{
						delete []pszText;
						pszText = NULL;
					}
				}
				else if(0 == strText.CompareNoCase(_T("Boolean")))
				{
					CString strValue = pChildNode->GetItemText(2);
					boost::any aPlistBool = (0 == strValue.CompareNoCase(_T("YES")))?bool(true):bool(false);

					char *pszText = G_UnicodeToAnsi(pChildNode->GetItemText(0).GetData());
// 					v_dictNode[string(pszText)] = aPlistBool;
					pair<string, boost::any> pairTmp(string(pszText), aPlistBool);
					v_dictNode.push_back(pairTmp);

					if(pszText)
					{
						delete []pszText;
						pszText = NULL;
					}
				}
				else if(0 == strText.CompareNoCase(_T("Number")))
				{
					CString strValue = pChildNode->GetItemText(2);
					if(strValue == strValue.SpanIncluding(_T("-0123456789")) )
					{
						boost::any aInt = _wtoi(strValue);

						char *pszText = G_UnicodeToAnsi(pChildNode->GetItemText(0).GetData());
// 						v_dictNode[string(pszText)] = aInt;
						pair<string, boost::any> pairTmp(string(pszText), aInt);
						v_dictNode.push_back(pairTmp);

						if(pszText)
						{
							delete []pszText;
							pszText = NULL;
						}
					}
					else if(strValue == strValue.SpanIncluding(_T("-0123456789.")))
					{
						boost::any aDouble = _wtof(strValue);

						char *pszText = G_UnicodeToAnsi(pChildNode->GetItemText(0).GetData());
// 						v_dictNode[string(pszText)] = aDouble;
						pair<string, boost::any> pairTmp(string(pszText), aDouble);
						v_dictNode.push_back(pairTmp);

						if(pszText)
						{
							delete []pszText;
							pszText = NULL;
						}
					}
					else
					{
						boost::any aInt = (int)0;

						char *pszText = G_UnicodeToAnsi(pChildNode->GetItemText(0).GetData());
// 						v_dictNode[string(pszText)] = aInt;
						pair<string, boost::any> pairTmp(string(pszText), aInt);
						v_dictNode.push_back(pairTmp);

						if(pszText)
						{
							delete []pszText;
							pszText = NULL;
						}
					}
				}
			}
		}
	}

	return true;
}

void CMainFrame::OnDropFiles( HDROP hDropInfo )
{
	//查询拖入文件个数;
	int nCount = DragQueryFile(hDropInfo,0xFFFFFFFF,NULL,0);

	//查询每个文件的名称;
	TCHAR szFileName[MAX_PATH];
	CString strFilePath;
	DWORD dwSize;
	for(int i = 0; i < nCount; i++)
	{
		dwSize = DragQueryFile(hDropInfo,i,NULL,0);
		DragQueryFile(hDropInfo,i,szFileName,dwSize + 1);
		strFilePath = szFileName;

		if(PathFileExists(strFilePath))
		{
			RemoveAllChild(m_pRootNode);
		}
		if( !LoadPList(m_pRootNode, strFilePath, m_mapDict) )
		{
			DragFinish(hDropInfo);
			return;
		}
		break;

	}
	DragFinish(hDropInfo);
}

void CMainFrame::OnPrepare()
{
	//窗口透明
	m_dwStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	m_dwStyle |= WS_EX_LAYERED;
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, m_dwStyle);
	// 	COLORREF rgbMask(RGB(0,0,0));
	// 	::SetLayeredWindowAttributes(*this, rgbMask, 255, LWA_COLORKEY | LWA_ALPHA);

	CDuiRect rcWnd;
	::GetWindowRect(*this, &rcWnd);
	rcWnd.Offset(-rcWnd.left, -rcWnd.top);
	rcWnd.right++; rcWnd.bottom++;
	HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, 5, 5);
	::SetWindowRgn(*this, hRgn, TRUE);
	::DeleteObject(hRgn);

	// 	FadeIn();
}

/*******************************
** FunctionName：Notify
** @Param uMsg: 消息类型
** Comment:     处理事件通知
** return：
** Creator：    zwl
** Date：		2014-01-15
** Modifier：	 
** ModifyDate：  
** Version：    1.0.0
*******************************/
void CMainFrame::Notify(TNotifyUI & msg)
{
	if(msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		OnPrepare();
	}
	else if(msg.sType == DUI_MSGTYPE_CLICK)
	{		
	}
	else if(msg.sType == DUI_MSGTYPE_MENU)
	{
		if(msg.pSender == m_pTreeListXml)
		{
			CMenuWnd* pMenu = new CMenuWnd(_T("menu.xml"));
			if( pMenu == NULL ) { return; }

			POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
			::ClientToScreen(*this, &pt);
			pMenu->Init(msg.pSender, pt);
		}
	}
	else if(msg.sType == _T("MenuNotify"))
	{
		CDuiString strName = msg.pSender->GetName();
		
		if(m_pTreeListXml)
		{
			CTreeNodeUI* pCurNode = (CTreeNodeUI*)m_pTreeListXml->GetItemAt(m_pTreeListXml->GetCurSel());
			if(pCurNode)
			{
				if(strName == _T("menu_newchild"))
					AddChild(pCurNode);
				else if(strName == _T("menu_newsibling"))
					Add(pCurNode);
				else if(strName == _T("menu_delete"))
					Delete(m_pTreeListXml, pCurNode);
				else if(strName == _T("menu_expand"))
				{
					if(pCurNode->GetFolderButton()->IsSelected())
					{
						pCurNode->GetFolderButton()->Selected(false);
						m_pTreeListXml->SetItemExpand(true, pCurNode, true);
					}
				}
				else if(strName == _T("menu_collapse"))
				{
					if(!pCurNode->GetFolderButton()->IsSelected())
					{
						pCurNode->GetFolderButton()->Selected(true);
						m_pTreeListXml->SetItemExpand(false, pCurNode, true);
					}
				}				
			}
		}
	}
	else if(msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if(msg.pSender == m_pTreeListXml)
		{
			if(m_pTreeListXml->GetCurSel() == -1)
			{
				m_pBtnAdd->SetEnabled(false);
				m_pBtnAddChild->SetEnabled(false);
			}
		}
	}
}

// /*******************************
// ** FunctionName：OnCreate
// ** @Param uMsg:   消息类型
// ** @Param wParam：附加消息
// ** @Param lParam：附加消息
// ** @Param bHandled：
// ** Comment:     窗口创建消息响应
// ** return：     true :返回状态
// ** Creator：    zwl
// ** Date：		2014-01-15
// ** Modifier：	 
// ** ModifyDate：  
// ** Version：    1.0.0
// *******************************/

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = 0;

	if(uMsg == WM_TIMER)
	{
		if (0 == OnTimer(uMsg, wParam, lParam, bHandled))
			return 0;
	}
	else if(uMsg == WM_NCLBUTTONDOWN)
	{
		m_PaintManager.SetFocus(NULL);
	}
	else if(uMsg == WM_NCLBUTTONDBLCLK)
	{
		BOOL bZoomed = ::IsZoomed(*this);
		if(bZoomed)
		{
			PostMessage(WM_SYSCOMMAND, SC_RESTORE);
		}
		else
		{
			PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
		}
	}
	else if(uMsg == WM_DROPFILES)
	{
		OnDropFiles((HDROP)wParam);
	}

	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	//处理键盘消息
	switch( uMsg ) 
	{
		case WM_KEYDOWN:
			{
				char sz = wParam;
				if (wParam == VK_ESCAPE)
				{
					PostMessage(WM_CLOSE);
					/*MessageBox(m_hWnd, _T("esc"), _T("cc"), MB_OK);*/
					return true;
				}
				else if(wParam == 'O')
				{
// 					if(GetKeyState(VK_CONTROL) < 0)
// 					{
// 						CFileDialog dlg(TRUE, NULL, NULL, OFN_NOTESTFILECREATE );
// 						dlg.DoModal();
// 					}
				}
				else if(wParam == VK_RETURN)
				{

				}
			}
			break;
		case WM_SYSCHAR:
			{
				char sz = wParam;
			}
			break;
		case WM_SYSKEYDOWN:
			{
			}
			break;
	}
	return true;
}

LRESULT CMainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed )
	{
		// 切换最大化按钮和还原按钮的状态
		if (m_pBtnMax && m_pBtnRestore)
		{
			m_pBtnMax->SetVisible(TRUE == bZoomed);       // 此处用表达式是为了避免编译器BOOL转换的警告
			m_pBtnRestore->SetVisible(FALSE == bZoomed);
		}
	}
#else
	LRESULT lRes = WindowImplBase::HandleMessage(uMsg, wParam, lParam);
#endif
	return lRes;
}

bool CMainFrame::OnBtnClose(void *lParam)
{
	TNotifyUI* pNotify = (TNotifyUI*)lParam;
	if(pNotify->sType == DUI_MSGTYPE_CLICK)
	{
// 		if(m_bExit)
// 			return 0;
// 
// 		FadeOut();
		PostMessage(WM_CLOSE);
	}

	return true;
}

bool CMainFrame::OnBtnMin(void *lParam)
{
	TNotifyUI* pNotify = (TNotifyUI*)lParam;
	if(pNotify->sType == DUI_MSGTYPE_CLICK)
	{
		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	}

	return true;
}

bool CMainFrame::OnBtnMax(void *lParam)
{
	TNotifyUI* pNotify = (TNotifyUI*)lParam;
	if(pNotify->sType == DUI_MSGTYPE_CLICK)
	{
		PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	}

	return true;
}

bool CMainFrame::OnBtnRestore(void *lParam)
{
	TNotifyUI* pNotify = (TNotifyUI*)lParam;
	if(pNotify->sType == DUI_MSGTYPE_CLICK)
	{
		PostMessage(WM_SYSCOMMAND, SC_RESTORE);
	}

	return true;
}

LRESULT CMainFrame::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ( wParam == FADE_IN_TIMER )
	{
		m_nAlpha += FADE_STEP ;
	}
	else if ( wParam == FADE_OUT_TIMER )
	{
		m_bExit = true;
		m_nAlpha -= FADE_STEP ;

		if ( m_nAlpha <= 0 )
		{
			PostMessage(WM_CLOSE);

			return 0;
		}

	}

	if( (wParam == FADE_IN_TIMER) || (wParam == FADE_OUT_TIMER) )
	{
		if ( m_nAlpha >= 0 && m_nAlpha < 255 )
		{
			::SetLayeredWindowAttributes(*this, 0, m_nAlpha, LWA_ALPHA);
		}
		else
		{
			m_nAlpha = 255;
			::SetLayeredWindowAttributes(*this, 0, m_nAlpha, LWA_ALPHA);
			m_dwStyle &= (~WS_EX_LAYERED);
			::SetWindowLong(m_hWnd, GWL_EXSTYLE, m_dwStyle);

			KillTimer(m_hWnd, m_nFadeTimer);
			m_nFadeTimer = 0 ;
		}

		return 0;
	}

	return 1;
}

void CMainFrame::FadeIn()
{
	if ( m_nFadeTimer != 0 )
	{
		KillTimer(m_hWnd, m_nFadeTimer) ;
	}

	m_nAlpha = 0 ;

	::SetLayeredWindowAttributes(*this, 0, m_nAlpha, LWA_ALPHA);

	m_nFadeTimer = SetTimer(m_hWnd, FADE_IN_TIMER, TIMER_FREQUENCY, NULL) ;


}

void CMainFrame::FadeOut()
{
	if ( m_nFadeTimer != 0 )
	{
		KillTimer(m_hWnd, m_nFadeTimer) ;
	}

	m_dwStyle |= WS_EX_LAYERED;
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, m_dwStyle);

	m_nAlpha = 255 ;
	::SetLayeredWindowAttributes(*this, 0, m_nAlpha, LWA_ALPHA);

	m_nFadeTimer = SetTimer(m_hWnd, FADE_OUT_TIMER, TIMER_FREQUENCY, NULL) ;

}