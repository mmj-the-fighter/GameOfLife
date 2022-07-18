
// GOLDoc.h : interface of the CGOLDoc class
//


#pragma once


class CGOLDoc : public CDocument
{
protected: // create from serialization only
	CGOLDoc();
	DECLARE_DYNCREATE(CGOLDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	void SetCell(int i, int j, COLORREF color);
	COLORREF GetCell(int i, int j);
	COLORREF GetCurrentPrimaryColor();
	COLORREF GetCurrentSecondaryColor();
	int GetNumOfRows();
	int GetNumOfCols();
	int GetGeneration();
	CSize GetDocumentSize();
	int GetHorizontalCellSize();
	int GetVerticalCellSize();
	void SetCellSize(int cx, int cy);
	void SetGridSize(int rows, int cols);
	void Clear();
	void Randomize();
	void CalculateNeighbours();
	void MakeNextGeneration();
	virtual ~CGOLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
private:
	COLORREF* m_colorGrid;
	int* m_neighboursCountGrid;
	int m_nRows;
	int m_nCols;
	int m_cxCellSize;
	int m_cyCellSize;
	int m_nGeneration;
	COLORREF m_colorPrimary;
	COLORREF m_colorSecondary;
};
