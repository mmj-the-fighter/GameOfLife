
// GOLDoc.cpp : implementation of the CGOLDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GOL.h"
#endif

#include "GOLDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGOLDoc

IMPLEMENT_DYNCREATE(CGOLDoc, CDocument)

BEGIN_MESSAGE_MAP(CGOLDoc, CDocument)
END_MESSAGE_MAP()


// CGOLDoc construction/destruction
CGOLDoc::CGOLDoc()
	: m_nRows(16)
	, m_nCols(16)
	, m_nGeneration(0)
	, m_cxCellSize(20)
	, m_cyCellSize(20)
	, m_colorPrimary(RGB(0, 0, 128))
	, m_colorSecondary(RGB(255, 255, 255))
{
	// TODO: add one-time construction code here
	m_colorGrid = new COLORREF[m_nRows * m_nCols];
	m_neighboursCountGrid = new int[m_nRows * m_nCols];
	Clear();
}


CGOLDoc::~CGOLDoc()
{
	if (m_colorGrid != NULL)
		delete[] m_colorGrid;
	if (m_neighboursCountGrid != NULL)
		delete[] m_neighboursCountGrid;
}

BOOL CGOLDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CGOLDoc serialization

void CGOLDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CGOLDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CGOLDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGOLDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGOLDoc diagnostics

#ifdef _DEBUG
void CGOLDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGOLDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGOLDoc commands
void CGOLDoc::SetCell(int i, int j, COLORREF color)
{
	if (i >= 0 && i < m_nRows && j >= 0 && j < m_nCols)
	{
		*(m_colorGrid + i * m_nCols + j) = color;
		//SetModifiedFlag(TRUE);
		UpdateAllViews(NULL);
	}
}
COLORREF CGOLDoc::GetCell(int i, int j)
{
	ASSERT(i >= 0 && i < m_nRows && j >= 0 && j < m_nCols);
	return *(m_colorGrid + i * m_nCols + j);//m_colorGrid[i][j];
}

COLORREF CGOLDoc::GetCurrentPrimaryColor()
{
	return m_colorPrimary;
}

COLORREF CGOLDoc::GetCurrentSecondaryColor()
{
	return m_colorSecondary;
}

int CGOLDoc::GetNumOfRows()
{
	return m_nRows;
}
int CGOLDoc::GetNumOfCols()
{
	return m_nCols;
}
int CGOLDoc::GetGeneration()
{
	return m_nGeneration;
}
CSize CGOLDoc::GetDocumentSize()
{
	CSize s;
	s.cx = m_nCols * m_cxCellSize + 50;
	s.cy = m_nRows * m_cyCellSize + 50;
	return s;
}
int CGOLDoc::GetHorizontalCellSize()
{
	return m_cxCellSize;
}
int CGOLDoc::GetVerticalCellSize()
{
	return m_cyCellSize;
}
void CGOLDoc::SetCellSize(int cx, int cy)
{
	m_cxCellSize = cx;
	m_cyCellSize = cy;
}
void CGOLDoc::SetGridSize(int rows, int cols)
{
	if (m_colorGrid != NULL)
		delete[] m_colorGrid;
	if (m_neighboursCountGrid != NULL)
		delete[] m_neighboursCountGrid;
	m_nRows = rows;
	m_nCols = cols;
	m_colorGrid = new COLORREF[m_nRows * m_nCols];
	m_neighboursCountGrid = new int[m_nRows * m_nCols];
	Clear();
}
void CGOLDoc::Clear()
{
	int nRows = GetNumOfRows();
	int nCols = GetNumOfCols();
	m_nGeneration = 0;
	for (int i = 0; i<nRows; i++)
		for (int j = 0; j < nCols; j++) {
		*(m_colorGrid + i * m_nCols + j) = GetCurrentSecondaryColor();
		*(m_neighboursCountGrid + i * m_nCols + j) = 0;
		}
}
void CGOLDoc::Randomize()
{
	int nRows = GetNumOfRows();
	int nCols = GetNumOfCols();
	m_nGeneration = 0;
	for (int i = 0; i<nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			if (rand() % 4 == 0)
			{
				*(m_colorGrid + i * m_nCols + j) = m_colorPrimary;
			}
			else {
				*(m_colorGrid + i * m_nCols + j) = m_colorSecondary;
			}
		}
	}
}
void CGOLDoc::CalculateNeighbours()
{
	int nRows = GetNumOfRows();
	int nCols = GetNumOfCols();

	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
			*(m_neighboursCountGrid + i * m_nCols + j) = 0;


	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			int ii;
			int jj;
			//WEST
			ii = i;
			jj = j - 1;
			if ((ii >= 0 && ii < nRows) && (jj >= 0 && jj < nCols) && (*(m_colorGrid + ii * m_nCols + jj) == m_colorPrimary))
				++*(m_neighboursCountGrid + i * m_nCols + j);

			//NORTH-WEST
			ii = i - 1;
			jj = j - 1;
			if ((ii >= 0 && ii < nRows) && (jj >= 0 && jj < nCols) && (*(m_colorGrid + ii * m_nCols + jj) == m_colorPrimary))
				++*(m_neighboursCountGrid + i * m_nCols + j);

			//NORTH
			ii = i - 1;
			jj = j;
			if ((ii >= 0 && ii < nRows) && (jj >= 0 && jj < nCols) && (*(m_colorGrid + ii * m_nCols + jj) == m_colorPrimary))
				++*(m_neighboursCountGrid + i * m_nCols + j);

			//NORTH-EAST
			ii = i - 1;
			jj = j + 1;
			if ((ii >= 0 && ii < nRows) && (jj >= 0 && jj < nCols) && (*(m_colorGrid + ii * m_nCols + jj) == m_colorPrimary))
				++*(m_neighboursCountGrid + i * m_nCols + j);

			//EAST
			ii = i;
			jj = j + 1;
			if ((ii >= 0 && ii < nRows) && (jj >= 0 && jj < nCols) && (*(m_colorGrid + ii * m_nCols + jj) == m_colorPrimary))
				++*(m_neighboursCountGrid + i * m_nCols + j);

			//SOUTH-EAST
			ii = i + 1;
			jj = j + 1;
			if ((ii >= 0 && ii < nRows) && (jj >= 0 && jj < nCols) && (*(m_colorGrid + ii * m_nCols + jj) == m_colorPrimary))
				++*(m_neighboursCountGrid + i * m_nCols + j);

			//SOUTH
			ii = i + 1;
			jj = j;
			if ((ii >= 0 && ii < nRows) && (jj >= 0 && jj < nCols) && (*(m_colorGrid + ii * m_nCols + jj) == m_colorPrimary))
				++*(m_neighboursCountGrid + i * m_nCols + j);

			//SOUTH-WEST
			ii = i + 1;
			jj = j - 1;
			if ((ii >= 0 && ii < nRows) && (jj >= 0 && jj < nCols) && (*(m_colorGrid + ii * m_nCols + jj) == m_colorPrimary))
				++*(m_neighboursCountGrid + i * m_nCols + j);
		}
	}
}
void CGOLDoc::MakeNextGeneration()
{
	int nRows = GetNumOfRows();
	int nCols = GetNumOfCols();
	++m_nGeneration;
	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			int n = *(m_neighboursCountGrid + i * m_nCols + j);
			if (n == 0 || n == 1) {
				*(m_colorGrid + i * m_nCols + j) = m_colorSecondary;
			}
			else if (n >= 4) {
				*(m_colorGrid + i * m_nCols + j) = m_colorSecondary;
			}
			else if (n == 3) {
				if (*(m_colorGrid + i * m_nCols + j) == m_colorSecondary) {
					*(m_colorGrid + i * m_nCols + j) = m_colorPrimary;
				}
			}
		}
	}
}