
// XXbgServiceView.cpp : CXXbgServiceView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "XXbgService.h"
#endif

#include "XXbgServiceDoc.h"
#include "XXbgServiceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "utils.h"

// CXXbgServiceView

IMPLEMENT_DYNCREATE(CXXbgServiceView, CHtmlView)

BEGIN_MESSAGE_MAP(CXXbgServiceView, CHtmlView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_MESSAGE(WM_HTML_SHOW,OnMyMessage)
END_MESSAGE_MAP()

// CXXbgServiceView ����/����

CXXbgServiceView::CXXbgServiceView()
{
	// TODO: �ڴ˴���ӹ������
	//this->SetActiveWindow();
}

CXXbgServiceView::~CXXbgServiceView()
{
	
}

BOOL CXXbgServiceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	
	return CHtmlView::PreCreateWindow(cs);
}

void CXXbgServiceView::OnInitialUpdate()
{
	
	CHtmlView::OnInitialUpdate();
	
//	Navigate2(_T("http://www.msdn.microsoft.com/visualc/"),NULL,NULL);
	
}

LRESULT CXXbgServiceView::OnMyMessage(WPARAM wParam, LPARAM iParam)
{
	//char s_string[1000+1] = {0};
	//sprintf(s_string,"��Ӧ��%s,�����鿴",(char *)wParam);
	//::MessageBox(NULL,s_string,"��ʾ",MB_OK);
	CHtmlView::OnInitialUpdate();
	
	Navigate2(_T((char *)wParam),NULL,NULL);

	if (AfxGetApp()->m_pMainWnd->IsZoomed())
	{
		// ��󻯣��ָ����
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	}
	else if (AfxGetApp()->m_pMainWnd->IsIconic())
	{
		// ��С�����ָ�ԭ��״̬
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOWNOACTIVATE);
	}
	else
	{
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
	}

	return 0;
}
// CXXbgServiceView ��ӡ



// CXXbgServiceView ���

#ifdef _DEBUG
void CXXbgServiceView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CXXbgServiceView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CXXbgServiceDoc* CXXbgServiceView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXXbgServiceDoc)));
	return (CXXbgServiceDoc*)m_pDocument;
}
#endif //_DEBUG


// CXXbgServiceView ��Ϣ�������


