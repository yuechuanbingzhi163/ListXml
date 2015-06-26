
// IEBrowser.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ListXml.h"
#include "MainFrame.h"
#include "WndShadow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIEBrowserApp

BEGIN_MESSAGE_MAP(CIEBrowserApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CIEBrowserApp 构造

CIEBrowserApp::CIEBrowserApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CIEBrowserApp 对象

CIEBrowserApp theApp;



LONG ExceptionFilterProc(EXCEPTION_POINTERS *pException)
{
	// 这里弹出一个错误对话框并退出程序
	//
	// 	FatalAppExit(-1,  _T("*** Unhandled Exception! ***"));
	G_DbgWrite(_T("[Time:%d][File:ListXml.cpp][Fun:ExceptionFilterProc][Line:%d]\r\n"), __TIME__, __LINE__);
	return EXCEPTION_EXECUTE_HANDLER;
}  

// CIEBrowserApp 初始化

BOOL CIEBrowserApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

// 	CIEBrowserDlg dlg;
// 	m_pMainWnd = &dlg;
// 	INT_PTR nResponse = dlg.DoModal();
// 	CWndShadow::Initialize(m_hInstance);

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) 
		return 0;

	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ExceptionFilterProc);  


	CPaintManagerUI::SetInstance(m_hInstance);
#ifdef __STATIC_LIB__
	CPaintManagerUI::SetResourceZip(CPaintManagerUI::GetInstancePath()+_T("Skins"));
#else
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+_T("Skins"));
#endif

	INT_PTR nResponse = 0;


	g_pMainFrame = new CMainFrame(_T("MainFrame.xml"));
	g_pMainFrame->Create(NULL, _T("ListXml"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	nResponse = g_pMainFrame->ShowModal();
	::CoUninitialize();

// 	g_pMainFrame->ShowWindow(true);
// 	CPaintManagerUI::MessageLoop();
// 	CPaintManagerUI::Term();

	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
