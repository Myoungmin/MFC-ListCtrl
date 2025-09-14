// CkVFactorListDlg.cpp : implementation file (kV Factor List Test Dialog)
//

#include "pch.h"
#include "framework.h"
#include "MFC-ListCtrl.h"
#include "CkVFactorListDlg.h"
#include "afxdialogex.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Global variable for external SDK API simulation
static std::vector<ST_KV_RATIO> g_DataStorage;

// External SDK API implementation (simulation)
void kVFactorListSet(const ST_KV_RATIO* pstItems, int nCount)
{
	g_DataStorage.clear();
	for (int i = 0; i < nCount; i++)
	{
		g_DataStorage.push_back(pstItems[i]);
	}
}

void kVFactorListGet(const ST_KV_RATIO** ppstItems, int* pnCount)
{
	*ppstItems = g_DataStorage.data();
	*pnCount = static_cast<int>(g_DataStorage.size());
}

// CkVFactorListDlg dialog

CkVFactorListDlg::CkVFactorListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KV_FACTOR_LIST_TEST_DIALOG, pParent)
	, m_pListCtrl(nullptr)
	, m_pEditInput(nullptr)
{
}

void CkVFactorListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CkVFactorListDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CkVFactorListDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CkVFactorListDlg::OnBnClickedBtnRefresh)
END_MESSAGE_MAP()

// CkVFactorListDlg message handlers

BOOL CkVFactorListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Initialize control pointers
	m_pListCtrl = static_cast<CListCtrl*>(GetDlgItem(IDC_LIST_DATA));
	m_pEditInput = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_INPUT));

	// Initialize ListControl
	InitializeListControl();

	// Initialize sample data if no data exists
	const ST_KV_RATIO* pstItems = nullptr;
	int nCount = 0;
	kVFactorListGet(&pstItems, &nCount);
	
	if (nCount == 0)
	{
		InitializeSampleData();
	}

	// Load initial data
	OnBnClickedBtnRefresh();

	return TRUE;
}

void CkVFactorListDlg::InitializeListControl()
{
	if (m_pListCtrl == nullptr) return;

	// Set Full Row Select style
	m_pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Add columns with compact width
	m_pListCtrl->InsertColumn(0, _T("kV"), LVCFMT_LEFT, 120);
	m_pListCtrl->InsertColumn(1, _T("Ratio"), LVCFMT_LEFT, 120);
}

void CkVFactorListDlg::InitializeSampleData()
{
	// Prepare sample data
	ST_KV_RATIO astSampleData[] = {
		{80.0f, 1.0f},
		{90.0f, 1.2f},
		{100.0f, 1.4f},
		{110.0f, 1.6f},
		{120.0f, 1.8f}
	};

	// Save sample data
	kVFactorListSet(astSampleData, sizeof(astSampleData) / sizeof(ST_KV_RATIO));
}

void CkVFactorListDlg::OnBnClickedBtnSave()
{
	if (m_pEditInput == nullptr) return;

	CString strInput;
	m_pEditInput->GetWindowText(strInput);

	std::vector<ST_KV_RATIO> vecData;
	CString strLine;
	int nStartPos = 0;

	// Parse line by line
	while (true)
	{
		strLine = strInput.Tokenize(_T("\r\n"), nStartPos);
		if (strLine.IsEmpty()) break;

		// CSV parsing (kV,Ratio)
		int nCommaPos = strLine.Find(_T(','));
		if (nCommaPos > 0)
		{
			CString strKV = strLine.Left(nCommaPos);
			CString strRatio = strLine.Mid(nCommaPos + 1);

			ST_KV_RATIO stData;
			stData.fKV = static_cast<float>(_tstof(strKV));
			stData.fRatio = static_cast<float>(_tstof(strRatio));

			vecData.push_back(stData);
		}
	}

	// Call kVFactorListSet to save to firmware
	if (!vecData.empty())
	{
		kVFactorListSet(vecData.data(), static_cast<int>(vecData.size()));
	}

	// Auto refresh after save to update ListControl
	OnBnClickedBtnRefresh();
}

void CkVFactorListDlg::OnBnClickedBtnRefresh()
{
	// Load data from firmware and update EditBox and ListControl
	UpdateEditBox();
	UpdateListControl();
}

void CkVFactorListDlg::UpdateListControl()
{
	if (m_pListCtrl == nullptr) return;

	// Delete existing items
	m_pListCtrl->DeleteAllItems();

	// Get data using kVFactorListGet
	const ST_KV_RATIO* pstItems = nullptr;
	int nCount = 0;
	kVFactorListGet(&pstItems, &nCount);

	// Add data to list
	for (int i = 0; i < nCount; i++)
	{
		CString strKV, strRatio;
		strKV.Format(_T("%.1f"), pstItems[i].fKV);
		strRatio.Format(_T("%.2f"), pstItems[i].fRatio);

		int nItem = m_pListCtrl->InsertItem(i, strKV);
		m_pListCtrl->SetItemText(nItem, 1, strRatio);
	}
}

void CkVFactorListDlg::UpdateEditBox()
{
	if (m_pEditInput == nullptr) return;

	// Get data using kVFactorListGet
	const ST_KV_RATIO* pstItems = nullptr;
	int nCount = 0;
	kVFactorListGet(&pstItems, &nCount);

	// Generate string in CSV format
	CString strOutput;
	for (int i = 0; i < nCount; i++)
	{
		CString strLine;
		strLine.Format(_T("%.1f,%.2f"), pstItems[i].fKV, pstItems[i].fRatio);
		
		if (i > 0)
			strOutput += _T("\r\n");
		strOutput += strLine;
	}

	m_pEditInput->SetWindowText(strOutput);
}