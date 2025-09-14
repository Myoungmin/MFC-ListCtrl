// CListCtrlDlg.h : header file
//

#pragma once

// ST_KV_PIXEL 구조체 정의
typedef struct _ST_KV_PIXEL
{
	float fKV;
	int nPixelValue;
} ST_KV_PIXEL;

// 외부 SDK API 함수 선언
void SetAll(const ST_KV_PIXEL* pstItems, int nCount);
void GetAll(const ST_KV_PIXEL** ppstItems, int* pnCount);

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
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnLoad();
	DECLARE_MESSAGE_MAP()

private:
	CListCtrl* m_pListCtrl;
	CEdit* m_pEditInput;
	
	void InitializeListControl();
	void UpdateListControl();
	void UpdateEditBox();
};
