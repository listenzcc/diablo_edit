#include "stdafx.h"
#include "DlgSummary.h"
#include "Diablo Edit2.h"
#include "Diablo Edit2View.h"


IMPLEMENT_DYNAMIC(CDlgSummary, CCharacterDialogBase)

CDlgSummary::CDlgSummary(CWnd* pParent /*=nullptr*/)
    : CCharacterDialogBase(CDlgSummary::IDD, pParent)
{
}

void CDlgSummary::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    // TODO: Add DDX controls here
    DDX_Control(pDX, IDC_COMBO3, m_cbCharComboBox);
	DDX_Control(pDX, IDC_COMBO4, m_cbItemComboBox);
	DDX_Text(pDX, IDC_STATIC, m_s1);
}

BEGIN_MESSAGE_MAP(CDlgSummary, CDialog)
    // TODO: Add message handlers here
END_MESSAGE_MAP()

//BOOL CDlgSummary::OnInitDialog()
//{
//    CDialog::OnInitDialog();
//    // TODO: Add initialization code here
//    return TRUE;
//}

CDlgSummary::~CDlgSummary()
{
}

void CDlgSummary::UpdateUI(const CD2S_Struct& character)
{
	LoadText();
}

BOOL CDlgSummary::GatherData(CD2S_Struct& character)
{
    return TRUE;
}

void CDlgSummary::ResetAll()
{
}

void CDlgSummary::LoadText(void) {
	m_s1.SetString((LPCTSTR)_T("This string is set to \"s1\""));

	// Only update after data is fully loaded
	if (::theApp.dataIsFullyLoaded) {

	// Update only once
	if (m_cbCharComboBox.GetCount() == 0) {
		m_cbCharComboBox.ResetContent();
		for (const auto& info : ::theApp.g_allCharacterNames) {
			m_cbCharComboBox.AddString(info);
		}
	}

	// Update only once
	if (m_cbItemComboBox.GetCount() == 0) {
		m_cbItemComboBox.ResetContent();
		for (const auto& info : ::theApp.g_allItemNames) {
			m_cbItemComboBox.AddString(info);
		}
	}
	}



    UpdateData(FALSE);
}

BOOL CDlgSummary::OnInitDialog()
{
    CCharacterDialogBase::OnInitDialog();
    InitUI();
    return TRUE;
}

void CDlgSummary::OnPaint()
{
    RefreshUI();
    CCharacterDialogBase::OnPaint();
}

void CDlgSummary::InitUI(void) {
	RefreshUI();
}

void CDlgSummary::RefreshUI(void)
{
	//// In case of resize, the tab control needs to resize
	//CRect rect;
	//GetClientRect(&rect);

	//CRect rectTab;
	//m_tcBasicInfo.GetWindowRect(&rectTab);
	//ScreenToClient(&rectTab);
	//rect.top = rectTab.top;

	//m_tcBasicInfo.MoveWindow(rect);
	//m_tcBasicInfo.GetClientRect(&rect);
	//m_tcBasicInfo.AdjustRect(FALSE, &rect);

	//// resize views
	//if (m_dlgTabPage)
	//	for (int i = 0; i < m_nTabPageCount; ++i)
	//		m_dlgTabPage[i]->MoveWindow(rect);
}