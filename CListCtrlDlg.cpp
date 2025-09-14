// CListCtrlDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC-ListCtrl.h"
#include "CListCtrlDlg.h"
#include "CkVFactorListDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CListCtrlDlg dialog

CListCtrlDlg::CListCtrlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCLISTCTRL_DIALOG, pParent)
{
}

void CListCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CListCtrlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_KV_FACTOR_TEST, &CListCtrlDlg::OnBnClickedBtnkVFactorTest)
END_MESSAGE_MAP()

// CListCtrlDlg message handlers

BOOL CListCtrlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here

	return TRUE;
}

void CListCtrlDlg::OnBnClickedBtnkVFactorTest()
{
	// Create and display kV Factor List Test dialog
	CkVFactorListDlg dlgkVFactorTest(this);
	dlgkVFactorTest.DoModal();
}

