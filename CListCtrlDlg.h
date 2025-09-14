// CListCtrlDlg.h : header file
//

#pragma once


// CListCtrlDlg dialog
class CListCtrlDlg : public CDialogEx
{
// Construction
public:
	CListCtrlDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCLISTCTRL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
