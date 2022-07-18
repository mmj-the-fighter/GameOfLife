// OptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GOL.h"
#include "OptionsDialog.h"
#include "afxdialogex.h"


// COptionsDialog dialog

IMPLEMENT_DYNAMIC(COptionsDialog, CDialog)

COptionsDialog::COptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDialog::IDD, pParent)
	, m_nWidth(10)
	, m_nHeight(10)
	, m_nColumns(8)
	, m_nRows(8)
{

}

COptionsDialog::~COptionsDialog()
{
}

void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nWidth);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_EDIT_ROWS, m_nRows);
	DDX_Text(pDX, IDC_EDIT_COLUMNS, m_nColumns);
}


BEGIN_MESSAGE_MAP(COptionsDialog, CDialog)
	ON_BN_CLICKED(ID_OK, &COptionsDialog::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &COptionsDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// COptionsDialog message handlers


BOOL COptionsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	CGOLView *pView = dynamic_cast<CGOLView*>(m_pParentWnd);
	if (pView != NULL)
	{
		CGOLDoc* pDoc = pView->GetDocument();
		if (pDoc) {
			m_nWidth = pDoc->GetHorizontalCellSize();
			m_nHeight = pDoc->GetVerticalCellSize();
			m_nRows = pDoc->GetNumOfRows();
			m_nColumns = pDoc->GetNumOfCols();
		}
	}

	CString strW;
	strW.Format(_T("%d"), m_nWidth);
	SetDlgItemText(IDC_EDIT_WIDTH, strW);
	CString strH;
	strH.Format(_T("%d"), m_nHeight);
	SetDlgItemText(IDC_EDIT_HEIGHT, strH);

	CString strR;
	strR.Format(_T("%d"), m_nRows);
	SetDlgItemText(IDC_EDIT_ROWS, strR);

	CString strC;
	strC.Format(_T("%d"), m_nColumns);
	SetDlgItemText(IDC_EDIT_COLUMNS, strC);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void COptionsDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}


void COptionsDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
}
