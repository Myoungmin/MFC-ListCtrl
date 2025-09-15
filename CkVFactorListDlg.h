// CkVFactorListDlg.h : header file (kV Factor List Test Dialog)
//
//

#pragma once

// AEC_KV_FACTOR_DATA structure definition
struct AEC_KV_FACTOR_DATA
{
	int kV;
	float Factor;

	AEC_KV_FACTOR_DATA()
	{
		memset(this, 0, sizeof(AEC_KV_FACTOR_DATA));
	}
};

// AEC_KV_FACTOR_LIST structure definition
struct AEC_KV_FACTOR_LIST
{
	int nCount;
	AEC_KV_FACTOR_DATA* pFactorList;

	AEC_KV_FACTOR_LIST()
	{
		nCount = 0;
		pFactorList = NULL;
	}

	~AEC_KV_FACTOR_LIST()
	{
		if(pFactorList)
			delete[] pFactorList;

		pFactorList = NULL;
	}
};

// External SDK API function declarations
void AecKvFactorListGet(AEC_KV_FACTOR_LIST* pList);
void AecKvFactorListSet(AEC_KV_FACTOR_LIST* pList);

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