// CListCtrlDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC-ListCtrl.h"
#include "CListCtrlDlg.h"
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
END_MESSAGE_MAP()


// CListCtrlDlg message handlers

BOOL CListCtrlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

