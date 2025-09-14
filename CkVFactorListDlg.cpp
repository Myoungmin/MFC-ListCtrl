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

// �ܺ� SDK API �ùķ��̼��� ���� ���� ����
static std::vector<ST_KV_RATIO> g_DataStorage;

// �ܺ� SDK API ���� (�ùķ��̼�)
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

	// ��Ʈ�� ������ �ʱ�ȭ
	m_pListCtrl = static_cast<CListCtrl*>(GetDlgItem(IDC_LIST_DATA));
	m_pEditInput = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_INPUT));

	// ListControl �ʱ�ȭ
	InitializeListControl();

	// ���� �����Ͱ� ������ �ʱ�ȭ
	const ST_KV_RATIO* pstItems = nullptr;
	int nCount = 0;
	kVFactorListGet(&pstItems, &nCount);
	
	if (nCount == 0)
	{
		InitializeSampleData();
	}

	// �ʱ� ������ �ε�
	OnBnClickedBtnRefresh();

	return TRUE;
}

void CkVFactorListDlg::InitializeListControl()
{
	if (m_pListCtrl == nullptr) return;

	// Full Row Select ��Ÿ�� ����
	m_pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// �÷� �߰�
	m_pListCtrl->InsertColumn(0, _T("kV"), LVCFMT_LEFT, 100);
	m_pListCtrl->InsertColumn(1, _T("Ratio"), LVCFMT_LEFT, 150);
}

void CkVFactorListDlg::InitializeSampleData()
{
	// ���� ������ �غ�
	ST_KV_RATIO astSampleData[] = {
		{80.0f, 1.0f},
		{90.0f, 1.2f},
		{100.0f, 1.4f},
		{110.0f, 1.6f},
		{120.0f, 1.8f}
	};

	// ���� ������ ����
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

	// �� ������ �Ľ�
	while (true)
	{
		strLine = strInput.Tokenize(_T("\r\n"), nStartPos);
		if (strLine.IsEmpty()) break;

		// CSV �Ľ� (kV,Ratio)
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

	// kVFactorListSet ȣ���Ͽ� �߿�� ����
	if (!vecData.empty())
	{
		kVFactorListSet(vecData.data(), static_cast<int>(vecData.size()));
	}

	// ���� �� �ڵ����� Refresh�Ͽ� ListControl ������Ʈ
	OnBnClickedBtnRefresh();
}

void CkVFactorListDlg::OnBnClickedBtnRefresh()
{
	// �߿���� �����͸� �ҷ��ͼ� ListControl�� ������Ʈ
	UpdateListControl();
}

void CkVFactorListDlg::UpdateListControl()
{
	if (m_pListCtrl == nullptr) return;

	// ���� ������ ����
	m_pListCtrl->DeleteAllItems();

	// kVFactorListGet�� ������ ��������
	const ST_KV_RATIO* pstItems = nullptr;
	int nCount = 0;
	kVFactorListGet(&pstItems, &nCount);

	// ����Ʈ�� ������ �߰�
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

	// kVFactorListGet�� ������ ��������
	const ST_KV_RATIO* pstItems = nullptr;
	int nCount = 0;
	kVFactorListGet(&pstItems, &nCount);

	// CSV �������� ���ڿ� ����
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