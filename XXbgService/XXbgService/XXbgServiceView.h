
// XXbgServiceView.h : CXXbgServiceView ��Ľӿ�
//

#pragma once


class CXXbgServiceView : public CHtmlView
{
protected: // �������л�����
	CXXbgServiceView();
	DECLARE_DYNCREATE(CXXbgServiceView)

// ����
public:
	CXXbgServiceDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CXXbgServiceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM iParam);
	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // XXbgServiceView.cpp �еĵ��԰汾
inline CXXbgServiceDoc* CXXbgServiceView::GetDocument() const
   { return reinterpret_cast<CXXbgServiceDoc*>(m_pDocument); }
#endif

