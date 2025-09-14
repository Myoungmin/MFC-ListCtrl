// CkVFactorListDlg.h : header file (kV Factor List Test Dialog)
//
//

#pragma once

// ST_KV_RATIO structure definition
typedef struct _ST_KV_RATIO
{
	float fKV;
	float fRatio;
} ST_KV_RATIO;

// External SDK API function declarations
void kVFactorListSet(const ST_KV_RATIO* pstItems, int nCount);
void kVFactorListGet(const ST_KV_RATIO** ppstItems, int* pnCount);

// CkVFactorListDlg dialog
class CkVFactorListDlg : public CDialogEx
{
// Construction
public:
	CkVFactorListDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KV_FACTOR_LIST_TEST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnRefresh();
	DECLARE_MESSAGE_MAP()

private:
	CListCtrl* m_pListCtrl;
	CEdit* m_pEditInput;
	
	void InitializeListControl();
	void UpdateListControl();
	void UpdateEditBox();
	void InitializeSampleData();
};