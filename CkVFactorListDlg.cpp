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

// 외부 SDK API 시뮬레이션을 위한 전역 변수
static std::vector<ST_KV_RATIO> g_DataStorage;

// 외부 SDK API 구현 (시뮬레이션)
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

	// 컨트롤 포인터 초기화
	m_pListCtrl = static_cast<CListCtrl*>(GetDlgItem(IDC_LIST_DATA));
	m_pEditInput = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_INPUT));

	// ListControl 초기화
	InitializeListControl();

	// 샘플 데이터가 없으면 초기화
	const ST_KV_RATIO* pstItems = nullptr;
	int nCount = 0;
	kVFactorListGet(&pstItems, &nCount);
	
	if (nCount == 0)
	{
		InitializeSampleData();
	}

	// 초기 데이터 로드
	OnBnClickedBtnRefresh();

	return TRUE;
}

void CkVFactorListDlg::InitializeListControl()
{
	if (m_pListCtrl == nullptr) return;

	// Full Row Select 스타일 설정
	m_pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 컬럼 추가
	m_pListCtrl->InsertColumn(0, _T("kV"), LVCFMT_LEFT, 100);
	m_pListCtrl->InsertColumn(1, _T("Ratio"), LVCFMT_LEFT, 150);
}

void CkVFactorListDlg::InitializeSampleData()
{
	// 샘플 데이터 준비
	ST_KV_RATIO astSampleData[] = {
		{80.0f, 1.0f},
		{90.0f, 1.2f},
		{100.0f, 1.4f},
		{110.0f, 1.6f},
		{120.0f, 1.8f}
	};

	// 샘플 데이터 저장
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

	// 줄 단위로 파싱
	while (true)
	{
		strLine = strInput.Tokenize(_T("\r\n"), nStartPos);
		if (strLine.IsEmpty()) break;

		// CSV 파싱 (kV,Ratio)
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

	// kVFactorListSet 호출하여 펌웨어에 저장
	if (!vecData.empty())
	{
		kVFactorListSet(vecData.data(), static_cast<int>(vecData.size()));
	}

	// 저장 후 자동으로 Refresh하여 ListControl 업데이트
	OnBnClickedBtnRefresh();
}

void CkVFactorListDlg::OnBnClickedBtnRefresh()
{
	// 펌웨어에서 데이터를 불러와서 ListControl만 업데이트
	UpdateListControl();
}

void CkVFactorListDlg::UpdateListControl()
{
	if (m_pListCtrl == nullptr) return;

	// 기존 아이템 삭제
	m_pListCtrl->DeleteAllItems();

	// kVFactorListGet로 데이터 가져오기
	const ST_KV_RATIO* pstItems = nullptr;
	int nCount = 0;
	kVFactorListGet(&pstItems, &nCount);

	// 리스트에 데이터 추가
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

	// kVFactorListGet로 데이터 가져오기
	const ST_KV_RATIO* pstItems = nullptr;
	int nCount = 0;
	kVFactorListGet(&pstItems, &nCount);

	// CSV 형식으로 문자열 생성
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