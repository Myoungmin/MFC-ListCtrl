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
static AEC_KV_FACTOR_LIST g_DataStorage;

// External SDK API implementation (simulation)
void AecKvFactorListSet(AEC_KV_FACTOR_LIST* pList)
{
	if (pList == NULL) return;

	// Clear existing data
	if (g_DataStorage.pFactorList)
	{
		delete[] g_DataStorage.pFactorList;
		g_DataStorage.pFactorList = NULL;
	}
	g_DataStorage.nCount = 0;

	// Copy new data
	if (pList->nCount > 0 && pList->pFactorList != NULL)
	{
		g_DataStorage.nCount = pList->nCount;
		g_DataStorage.pFactorList = new AEC_KV_FACTOR_DATA[pList->nCount];
		
		for (int i = 0; i < pList->nCount; i++)
		{
			g_DataStorage.pFactorList[i] = pList->pFactorList[i];
		}
	}
}

void AecKvFactorListGet(AEC_KV_FACTOR_LIST* pList)
{
	if (pList == NULL) return;

	// Clear existing data in output structure
	if (pList->pFactorList)
	{
		delete[] pList->pFactorList;
		pList->pFactorList = NULL;
	}
	pList->nCount = 0;

	// Copy data from global storage
	if (g_DataStorage.nCount > 0 && g_DataStorage.pFactorList != NULL)
	{
		pList->nCount = g_DataStorage.nCount;
		pList->pFactorList = new AEC_KV_FACTOR_DATA[g_DataStorage.nCount];
		
		for (int i = 0; i < g_DataStorage.nCount; i++)
		{
			pList->pFactorList[i] = g_DataStorage.pFactorList[i];
		}
	}
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
	AEC_KV_FACTOR_LIST stList;
	AecKvFactorListGet(&stList);
	
	if (stList.nCount == 0)
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
	m_pListCtrl->InsertColumn(1, _T("Factor"), LVCFMT_LEFT, 120);
}

void CkVFactorListDlg::InitializeSampleData()
{
	// Prepare sample data
	AEC_KV_FACTOR_LIST stList;
	stList.nCount = 5;
	stList.pFactorList = new AEC_KV_FACTOR_DATA[5];
	
	stList.pFactorList[0].kV = 80;
	stList.pFactorList[0].Factor = 1.0f;
	
	stList.pFactorList[1].kV = 90;
	stList.pFactorList[1].Factor = 1.2f;
	
	stList.pFactorList[2].kV = 100;
	stList.pFactorList[2].Factor = 1.4f;
	
	stList.pFactorList[3].kV = 110;
	stList.pFactorList[3].Factor = 1.6f;
	
	stList.pFactorList[4].kV = 120;
	stList.pFactorList[4].Factor = 1.8f;

	// Save sample data
	AecKvFactorListSet(&stList);
}

void CkVFactorListDlg::OnBnClickedBtnSave()
{
	if (m_pEditInput == nullptr) return;

	CString strInput;
	m_pEditInput->GetWindowText(strInput);

	std::vector<AEC_KV_FACTOR_DATA> vecData;
	CString strLine;
	int nStartPos = 0;

	// Parse line by line
	while (true)
	{
		strLine = strInput.Tokenize(_T("\r\n"), nStartPos);
		if (strLine.IsEmpty()) break;

		// CSV parsing (kV,Factor)
		int nCommaPos = strLine.Find(_T(','));
		if (nCommaPos > 0)
		{
			CString strKV = strLine.Left(nCommaPos);
			CString strFactor = strLine.Mid(nCommaPos + 1);

			AEC_KV_FACTOR_DATA stData;
			stData.kV = _ttoi(strKV);
			stData.Factor = static_cast<float>(_tstof(strFactor));

			vecData.push_back(stData);
		}
	}

	// Call AecKvFactorListSet to save to firmware
	if (!vecData.empty())
	{
		AEC_KV_FACTOR_LIST stList;
		stList.nCount = static_cast<int>(vecData.size());
		stList.pFactorList = new AEC_KV_FACTOR_DATA[stList.nCount];
		
		for (int i = 0; i < stList.nCount; i++)
		{
			stList.pFactorList[i] = vecData[i];
		}
		
		AecKvFactorListSet(&stList);
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

	// Get data using AecKvFactorListGet
	AEC_KV_FACTOR_LIST stList;
	AecKvFactorListGet(&stList);

	// Add data to list
	for (int i = 0; i < stList.nCount; i++)
	{
		CString strKV, strFactor;
		strKV.Format(_T("%d"), stList.pFactorList[i].kV);
		strFactor.Format(_T("%.2f"), stList.pFactorList[i].Factor);

		int nItem = m_pListCtrl->InsertItem(i, strKV);
		m_pListCtrl->SetItemText(nItem, 1, strFactor);
	}
}

void CkVFactorListDlg::UpdateEditBox()
{
	if (m_pEditInput == nullptr) return;

	// Get data using AecKvFactorListGet
	AEC_KV_FACTOR_LIST stList;
	AecKvFactorListGet(&stList);

	// Generate string in CSV format
	CString strOutput;
	for (int i = 0; i < stList.nCount; i++)
	{
		CString strLine;
		strLine.Format(_T("%d,%.2f"), stList.pFactorList[i].kV, stList.pFactorList[i].Factor);
		
		if (i > 0)
			strOutput += _T("\r\n");
		strOutput += strLine;
	}

	m_pEditInput->SetWindowText(strOutput);
}