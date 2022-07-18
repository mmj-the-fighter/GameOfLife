
// GOLView.cpp : implementation of the CGOLView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GOL.h"
#endif

#include "GOLDoc.h"
#include "GOLView.h"
#include "OptionsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGOLView

IMPLEMENT_DYNCREATE(CGOLView, CScrollView)

BEGIN_MESSAGE_MAP(CGOLView, CScrollView)
	ON_COMMAND(ID_RANDOMIZE, &CGOLView::OnRandomize)
	ON_COMMAND(ID_RUN, &CGOLView::OnRun)
	ON_COMMAND(ID_PAUSE, &CGOLView::OnPause)
	ON_COMMAND(ID_CLEAR, &CGOLView::OnClear)
	ON_COMMAND(ID_NEXT, &CGOLView::OnNext)
	ON_COMMAND(ID_OPTIONS, &CGOLView::OnOptions)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CGOLView construction/destruction
CGOLView::CGOLView()
	: m_cxOffset(50)
	, m_cyOffset(50)
	, m_bRunSimulation(false)
	, m_pSimulationThread(NULL)
	, m_bLeftButtonDown(false)
	, m_bRightButtonDown(false)
{
	// TODO: add construction code here

}

CGOLView::~CGOLView()
{
}

BOOL CGOLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CGOLView drawing

void CGOLView::OnDraw(CDC* pDC)
{
	CGOLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	pDC->SetMapMode(MM_LOENGLISH);
	int nRows = pDoc->GetNumOfRows();
	int nCols = pDoc->GetNumOfCols();
	int cx = pDoc->GetHorizontalCellSize();
	int cy = pDoc->GetVerticalCellSize();
	for (int i = 0; i<nRows; i++) {
		for (int j = 0; j<nCols; j++) {
			COLORREF color = pDoc->GetCell(i, j);
			CBrush brush(color);
			int x1 = (j * cx) + m_cxOffset;
			int y1 = (i * -cy) - m_cyOffset;
			int x2 = x1 + cx;
			int y2 = y1 - cy;
			CRect rect(x1, y1, x2, y2);
			pDC->FillRect(rect, &brush);
		}
	}

	//
	// Then the draw the grid lines surrounding them.
	//
	int maxX = m_cxOffset + nCols*cx;
	int minY = -(m_cyOffset + nRows*cy);
	for (int x = m_cxOffset; x <= maxX; x += cx) {
		pDC->MoveTo(x, -m_cyOffset);
		pDC->LineTo(x, minY);
	}

	for (int y = -m_cyOffset; y >= minY; y -= cy) {
		pDC->MoveTo(m_cxOffset, y);
		pDC->LineTo(maxX, y);
	}

	pDC->SetMapMode(MM_TEXT);
	CString str;
	str.Format(_T("Generation: %d"), pDoc->GetGeneration());
	pDC->TextOutW(50, 15, str);
	//if (m_bRunSimulation){
	//	pDoc->CalculateNeighbours();
	//	pDoc->MakeNextGeneration();
	//	Invalidate();
	//}
}

void CGOLView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CGOLView diagnostics

#ifdef _DEBUG
void CGOLView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGOLView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CGOLDoc* CGOLView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGOLDoc)));
	return (CGOLDoc*)m_pDocument;
}
#endif //_DEBUG


// CGOLView message handlers


void CGOLView::OnRandomize()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CGOLDoc* pDoc = GetDocument();
	pDoc->Randomize();
	Invalidate();
}


void CGOLView::OnRun()
{
	// TODO: Add your command handler code here
	if (m_pSimulationThread == NULL){
		m_pThreadParams.pWnd = AfxGetMainWnd();
		m_pThreadParams.pDoc = GetDocument();
		m_pThreadParams.pView = this;
		m_pSimulationThread = AfxBeginThread(
			SimulationThreadFunc, &m_pThreadParams,
			THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	}
	if (m_pSimulationThread != NULL)
		m_pSimulationThread->ResumeThread();
}


void CGOLView::OnPause()
{
	// TODO: Add your command handler code here
	if (m_pSimulationThread != NULL){
		m_pSimulationThread->SuspendThread();
	}
}



void CGOLView::OnClear()
{
	// TODO: Add your command handler code here
	CGOLDoc* pDoc = GetDocument();
	pDoc->Clear();
	Invalidate();
}


void CGOLView::OnNext()
{
	// TODO: Add your command handler code here
	CGOLDoc* pDoc = GetDocument();
	pDoc->CalculateNeighbours();
	pDoc->MakeNextGeneration();
	Invalidate();
}


void CGOLView::OnOptions()
{
	// TODO: Add your command handler code here
	COptionsDialog dlg(this);
	CGOLDoc* pDoc = GetDocument();
	int prevW = pDoc->GetHorizontalCellSize();
	int prevH = pDoc->GetVerticalCellSize();
	int prevR = pDoc->GetNumOfRows();
	int prevC = pDoc->GetNumOfCols();
	if (dlg.DoModal() == IDOK) {
		int w = dlg.m_nWidth;
		int h = dlg.m_nHeight;
		int r = dlg.m_nRows;
		int c = dlg.m_nColumns;
		BOOL fModified = FALSE;
		if ((w != prevW || h != prevH)) {
			if (w > 0 && w <= 100 && h>0 && h <= 100) {
				pDoc->SetCellSize(w, h);
				fModified = TRUE;
			}
			else{
				MessageBox(L"Enter a value between 0 and 100 for cell width and cell height");
			}
		}

		if ((r != prevR || c != prevC)) {
			if (r > 0 && r <= 200 && c>0 && c <= 200) {
				pDoc->SetGridSize(r, c);
				fModified = TRUE;
			}
			else{
				MessageBox(L"Enter a value between 0 and 200 for cell rows and columns");
			}
		}

		if (fModified) {
			Invalidate();
			CSize sizeTotal = pDoc->GetDocumentSize();
			SetScrollSizes(MM_TEXT, sizeTotal);
		}
	}
}


void CGOLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	point = point + GetScrollPosition();
	CScrollView::OnLButtonDown(nFlags, point);
	m_bLeftButtonDown = true;


	//
	// Convert to click coordinates to MM_LOENGLISH units.
	//
	CClientDC dc(this);
	dc.SetMapMode(MM_LOENGLISH);
	CPoint pos = point;
	dc.DPtoLP(&pos);

	//
	// If a square was clicked, set its color to the current color.
	//
	CGOLDoc* pDoc = GetDocument();
	int nRows = pDoc->GetNumOfRows();
	int nCols = pDoc->GetNumOfCols();
	int cx = pDoc->GetHorizontalCellSize();
	int cy = pDoc->GetVerticalCellSize();
	int maxX = m_cxOffset + nCols*cx;
	int minY = -(m_cyOffset + nRows*cy);
	if (pos.x >= cx && pos.x <= maxX && pos.y <= -m_cyOffset && pos.y >= minY) {
		int i = (-pos.y - m_cyOffset) / cy;
		int j = (pos.x - m_cxOffset) / cx;

		//TODO: fix negative bug here
		if (i >= 0 && j >= 0){
			pDoc->SetCell(i, j, pDoc->GetCurrentPrimaryColor());
		}
	}

	//CScrollView::OnLButtonDown(nFlags, point);
}


void CGOLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	//point = point + GetScrollPosition();
	// TODO: Add your message handler code here and/or call default
	m_bLeftButtonDown = false;
	//CScrollView::OnLButtonUp(nFlags, point);
}


void CGOLView::OnRButtonDown(UINT nFlags, CPoint point)
{
	point = point + GetScrollPosition();
	// TODO: Add your message handler code here and/or call default

	///CScrollView::OnRButtonDown(nFlags, point);
	m_bRightButtonDown = true;
	//
	// Convert to click coordinates to MM_LOENGLISH units.
	//
	CClientDC dc(this);
	dc.SetMapMode(MM_LOENGLISH);
	CPoint pos = point;
	dc.DPtoLP(&pos);

	//
	// If a square was clicked, set its color to the secondary color.
	//
	CGOLDoc* pDoc = GetDocument();
	int nRows = pDoc->GetNumOfRows();
	int nCols = pDoc->GetNumOfCols();
	int cx = pDoc->GetHorizontalCellSize();
	int cy = pDoc->GetVerticalCellSize();
	int maxX = m_cxOffset + nCols*cx;
	int minY = -(m_cyOffset + nRows*cy);
	if (pos.x >= cx && pos.x <= maxX && pos.y <= -m_cyOffset && pos.y >= minY) {
		int i = (-pos.y - m_cyOffset) / cy;
		int j = (pos.x - m_cxOffset) / cx;
		pDoc->SetCell(i, j, pDoc->GetCurrentSecondaryColor());
	}
	//Invalidate();
}


void CGOLView::OnRButtonUp(UINT nFlags, CPoint point)
{
	//point = point + GetScrollPosition();
	// TODO: Add your message handler code here and/or call default
	m_bRightButtonDown = false;
	//CScrollView::OnLButtonUp(nFlags, point);
}


void CGOLView::OnMouseMove(UINT nFlags, CPoint point)
{
	point = point + GetScrollPosition();
	// TODO: Add your message handler code here and/or call default
	if (!m_bLeftButtonDown && !m_bRightButtonDown) {
		//CScrollView::OnMouseMove(nFlags, point);
		return;
	}
	if (m_bLeftButtonDown && m_bRightButtonDown) {
		//CScrollView::OnMouseMove(nFlags, point);
		return;
	}
	//
	// Convert to click coordinates to MM_LOENGLISH units.
	//
	CClientDC dc(this);
	dc.SetMapMode(MM_LOENGLISH);
	CPoint pos = point;
	dc.DPtoLP(&pos);

	//
	// If a square was clicked, set its color to the secondary color.
	//
	CGOLDoc* pDoc = GetDocument();
	int nRows = pDoc->GetNumOfRows();
	int nCols = pDoc->GetNumOfCols();
	int cx = pDoc->GetHorizontalCellSize();
	int cy = pDoc->GetVerticalCellSize();
	int maxX = m_cxOffset + nCols*cx;
	int minY = -(m_cyOffset + nRows*cy);
	if (pos.x >= cx && pos.x <= maxX && pos.y <= -m_cyOffset && pos.y >= minY) {
		int i = (-pos.y - m_cyOffset) / cy;
		int j = (pos.x - m_cxOffset) / cx;
		if (m_bLeftButtonDown)
			pDoc->SetCell(i, j, pDoc->GetCurrentPrimaryColor());
		else if (m_bRightButtonDown)
			pDoc->SetCell(i, j, pDoc->GetCurrentSecondaryColor());
	}
	//Invalidate();
}

UINT SimulationThreadFunc(LPVOID pParam)
{
	THREADPARMS* ptp = (THREADPARMS*)pParam;
	while (1){
		ptp->pDoc->CalculateNeighbours();
		ptp->pDoc->MakeNextGeneration();
		ptp->pView->Invalidate();
		Sleep(100);
	}
	return 0;
}