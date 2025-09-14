// CListCtrlDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC-ListCtrl.h"
#include "CListCtrlDlg.h"
#include "afxdialogex.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// �ܺ� SDK API �ùķ��̼��� ���� ���� ����
static std::vector<ST_KV_PIXEL> g_DataStorage;

// �ܺ� SDK API ���� (�ùķ��̼�)
void SetAll(const ST_KV_PIXEL* pstItems, int nCount)
{
	g_DataStorage.clear();
	for (int i = 0; i < nCount; i++)
	{
		g_DataStorage.push_back(pstItems[i]);
	}
}

void GetAll(const ST_KV_PIXEL** ppstItems, int* pnCount)
{
	*ppstItems = g_DataStorage.data();
	*pnCount = static_cast<int>(g_DataStorage.size());
}

// CListCtrlDlg dialog

CListCtrlDlg::CListCtrlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCLISTCTRL_DIALOG, pParent)
	, m_pListCtrl(nullptr)
	, m_pEditInput(nullptr)
{
}

void CListCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CListCtrlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CListCtrlDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CListCtrlDlg::OnBnClickedBtnLoad)
END_MESSAGE_MAP()

// CListCtrlDlg message handlers

BOOL CListCtrlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��Ʈ�� ������ �ʱ�ȭ
	m_pListCtrl = static_cast<CListCtrl*>(GetDlgItem(IDC_LIST_DATA));
	m_pEditInput = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_INPUT));

	// ListControl �ʱ�ȭ
	InitializeListControl();

	// ���� ������ �غ�
	ST_KV_PIXEL astSampleData[] = {
		{80.0f, 1000},
		{90.0f, 1200},
		{100.0f, 1400},
		{110.0f, 1600},
		{120.0f, 1800}
	};

	// ���� ������ ����
	SetAll(astSampleData, sizeof(astSampleData) / sizeof(ST_KV_PIXEL));

	// �ʱ� ������ �ε�
	OnBnClickedBtnLoad();

	return TRUE;
}

void CListCtrlDlg::InitializeListControl()
{
	if (m_pListCtrl == nullptr) return;

	// Full Row Select ��Ÿ�� ����
	m_pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// �÷� �߰�
	m_pListCtrl->InsertColumn(0, _T("kV"), LVCFMT_LEFT, 100);
	m_pListCtrl->InsertColumn(1, _T("PixelValue"), LVCFMT_LEFT, 150);
}

void CListCtrlDlg::OnBnClickedBtnSave()
{
	if (m_pEditInput == nullptr) return;

	CString strInput;
	m_pEditInput->GetWindowText(strInput);

	std::vector<ST_KV_PIXEL> vecData;
	CString strLine;
	int nStartPos = 0;

	// �� ������ �Ľ�
	while (true)
	{
		strLine = strInput.Tokenize(_T("\r\n"), nStartPos);
		if (strLine.IsEmpty()) break;

		// CSV �Ľ� (kV,PixelValue)
		int nCommaPos = strLine.Find(_T(','));
		if (nCommaPos > 0)
		{
			CString strKV = strLine.Left(nCommaPos);
			CString strPixel = strLine.Mid(nCommaPos + 1);

			ST_KV_PIXEL stData;
			stData.fKV = static_cast<float>(_tstof(strKV));
			stData.nPixelValue = _tstoi(strPixel);

			vecData.push_back(stData);
		}
	}

	// SetAll ȣ��
	if (!vecData.empty())
	{
		SetAll(vecData.data(), static_cast<int>(vecData.size()));
	}

	// ����Ʈ ��Ʈ�� ������Ʈ
	UpdateListControl();
}

void CListCtrlDlg::OnBnClickedBtnLoad()
{
	// GetAll ȣ��
	const ST_KV_PIXEL* pstItems = nullptr;
	int nCount = 0;
	GetAll(&pstItems, &nCount);

	// EditBox ������Ʈ
	UpdateEditBox();

	// ListControl ������Ʈ
	UpdateListControl();
}

void CListCtrlDlg::UpdateListControl()
{
	if (m_pListCtrl == nullptr) return;

	// ���� ������ ����
	m_pListCtrl->DeleteAllItems();

	// GetAll�� ������ ��������
	const ST_KV_PIXEL* pstItems = nullptr;
	int nCount = 0;
	GetAll(&pstItems, &nCount);

	// ����Ʈ�� ������ �߰�
	for (int i = 0; i < nCount; i++)
	{
		CString strKV, strPixel;
		strKV.Format(_T("%.1f"), pstItems[i].fKV);
		strPixel.Format(_T("%d"), pstItems[i].nPixelValue);

		int nItem = m_pListCtrl->InsertItem(i, strKV);
		m_pListCtrl->SetItemText(nItem, 1, strPixel);
	}
}

void CListCtrlDlg::UpdateEditBox()
{
	if (m_pEditInput == nullptr) return;

	// GetAll�� ������ ��������
	const ST_KV_PIXEL* pstItems = nullptr;
	int nCount = 0;
	GetAll(&pstItems, &nCount);

	// CSV �������� ���ڿ� ����
	CString strOutput;
	for (int i = 0; i < nCount; i++)
	{
		CString strLine;
		strLine.Format(_T("%.1f,%d"), pstItems[i].fKV, pstItems[i].nPixelValue);
		
		if (i > 0)
			strOutput += _T("\r\n");
		strOutput += strLine;
	}

	m_pEditInput->SetWindowText(strOutput);
}

