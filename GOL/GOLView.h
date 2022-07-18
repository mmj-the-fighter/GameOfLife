
// GOLView.h : interface of the CGOLView class
//

#pragma once
#include "GOLDoc.H"


UINT SimulationThreadFunc(LPVOID pParam);

class CGOLView;

typedef struct tagTHREADPARMS {
	CWnd* pWnd;
	CGOLDoc* pDoc;
	CGOLView* pView;
}THREADPARMS;

class CGOLView : public CScrollView
{
protected: // create from serialization only
	CGOLView();
	DECLARE_DYNCREATE(CGOLView)

// Attributes
public:
	CGOLDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CGOLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
private:
	int m_cxOffset;
	int m_cyOffset;
	THREADPARMS m_pThreadParams;
	CWinThread* m_pSimulationThread;
	bool m_bRunSimulation;
	bool m_bLeftButtonDown;
	bool m_bRightButtonDown;
public:
	afx_msg void OnRandomize();
	afx_msg void OnRun();
	afx_msg void OnPause();
	afx_msg void OnClear();
	afx_msg void OnNext();
	afx_msg void OnOptions();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in GOLView.cpp
inline CGOLDoc* CGOLView::GetDocument() const
   { return reinterpret_cast<CGOLDoc*>(m_pDocument); }
#endif

