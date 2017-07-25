
// MainFrm.h : CMainFrame ��Ľӿ�
//
#include "ScreenshotDlg.h"
#include "ManualInputDlg.h"
#include "MyTTrace.h"
#include "SettingDlg.h"
#include "CBaseReadIDCard.h"

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	virtual void OnSysCommand( UINT nID, LPARAM lParam);
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar        m_wndStatusBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);  //���������Ϣ��Ӧ����������
	afx_msg LRESULT OnScreenshot(WPARAM wParam, LPARAM lParam);  //������Ϣ
	afx_msg LRESULT OnIocpRead(WPARAM wParam, LPARAM lParam);
	afx_msg void OnQuit();
	afx_msg void StartKeyBoardHook();
	afx_msg void OnManualInput();
	afx_msg void ShowManualInput(char *tip);
	afx_msg void OnSettingWin();
	afx_msg void OnMyClose();
	afx_msg void OnGaoPaiYiCeShi();
	afx_msg LRESULT OnScreenDlgMessage(WPARAM wParam, LPARAM iParam);
	afx_msg LRESULT OnContinueInput(WPARAM wParam, LPARAM iParam);
	DECLARE_MESSAGE_MAP()
public:
	NOTIFYICONDATA m_nid; //�������̵���ؽṹ��
	virtual void ActivateFrame(int nCmdShow = -1);
	BOOL SaveFrmPosToFile();
	void InitDevice();
public:
	CMenu m_menu;
	BOOL m_hookFlag;
	CScreenshotDlg *screenshotDlg;
	CManualInput *input;
	CSetting *settingDlg;
	CBaseReadIDCardInfo *pBaseReadIDCardInfo;
	CBaseSaveDeskPic *pBaseSaveDeskPic;
	CBaseSaveEnvPic *pBaseSaveEnvPic;
};


