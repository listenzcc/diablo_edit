#include "stdafx.h"
#include "DlgSummary.h"
#include "Diablo Edit2.h"
#include "Diablo Edit2View.h"


IMPLEMENT_DYNAMIC(CDlgSummary, CCharacterDialogBase)

CDlgSummary::CDlgSummary(CWnd* pParent /*=nullptr*/)
    : CCharacterDialogBase(CDlgSummary::IDD, pParent)
{
	a = 50;
}

void CDlgSummary::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    // TODO: Add DDX controls here
    DDX_Control(pDX, IDC_COMBO3, m_cbCharComboBox);
	DDX_Control(pDX, IDC_COMBO4, m_cbItemComboBox);
	DDX_Control(pDX, IDC_COMBO_Normal, m_cbNormalItemComboBox);
	DDX_Control(pDX, IDC_COMBO_Magic, m_cbMagicItemComboBox);
	DDX_Control(pDX, IDC_COMBO_Rare, m_cbRareItemComboBox);
	DDX_Control(pDX, IDC_COMBO_Set, m_cbSetItemComboBox);
	DDX_Control(pDX, IDC_COMBO_Unique, m_cbUniqueItemComboBox);
	DDX_Control(pDX, IDC_COMBO_RuneWord, m_cbRuneWordItemComboBox);
	DDX_Control(pDX, IDC_COMBO_Craft, m_cbCraftItemComboBox);
	DDX_Control(pDX, IDC_COMBO_SelectedItem, m_cbSelectedItemComboBox);

	DDX_Text(pDX, IDC_STATIC, m_s1);

}

BEGIN_MESSAGE_MAP(CDlgSummary, CDialog)
    // TODO: Add message handlers here
	ON_CBN_SELCHANGE(IDC_COMBO_Normal, &CDlgSummary::OnCbnSelchangeComboNormal)
	ON_CBN_SELCHANGE(IDC_COMBO_Magic, &CDlgSummary::OnCbnSelchangeComboMagic)
	ON_CBN_SELCHANGE(IDC_COMBO_Rare, &CDlgSummary::OnCbnSelchangeComboRare)
	ON_CBN_SELCHANGE(IDC_COMBO_Set, &CDlgSummary::OnCbnSelchangeComboSet)
	ON_CBN_SELCHANGE(IDC_COMBO_Unique, &CDlgSummary::OnCbnSelchangeComboUnique)
	ON_CBN_SELCHANGE(IDC_COMBO_RuneWord, &CDlgSummary::OnCbnSelchangeComboRuneWord)
	ON_CBN_SELCHANGE(IDC_COMBO_Craft, &CDlgSummary::OnCbnSelchangeComboCraft)
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
	// Only update after data is fully loaded

	if (::theApp.dataIsFullyLoaded) {
		LoadText();
		UpdateItemsCombobox();
	}
}

BOOL CDlgSummary::GatherData(CD2S_Struct& character)
{
    return TRUE;
}

void CDlgSummary::ResetAll()
{
}

static void UpdateItemComboboxWithHashMap(CComboBox& ccb, std::unordered_map<CString, std::vector<ItemWithCharacterName>, CStringHash> hashMap, std::vector<CString> & keys) {
	ccb.ResetContent();
	keys.clear();  // 清空传入的键向量

	// 遍历哈希表
	for (const auto& pair : hashMap) {
		// 处理键值对
		CString key = pair.first, content=_T("");
		const size_t count = pair.second.size();  // 获取向量的大小
		content.AppendFormat(_T("%s (%d)"), key, (int)count);  // 添加计数到键后面
		ccb.AddString(content);  // 添加键到组合框
		keys.push_back(key);  // 将键添加到传入的向量中
	}

	if (ccb.GetCount() > 0) {
		ccb.SetCurSel(0);
	}
}

void CDlgSummary::UpdateItemsCombobox()
{
	UpdateItemComboboxWithHashMap(m_cbMagicItemComboBox, ::theApp.g_hashMap_itemSelection_Magic, m_MagicItemKeys);
	UpdateItemComboboxWithHashMap(m_cbRareItemComboBox, ::theApp.g_hashMap_itemSelection_Rare, m_RareItemKeys);
	UpdateItemComboboxWithHashMap(m_cbSetItemComboBox, ::theApp.g_hashMap_itemSelection_Set, m_SetItemKeys);
	UpdateItemComboboxWithHashMap(m_cbUniqueItemComboBox, ::theApp.g_hashMap_itemSelection_Unique, m_UniqueItemKeys);
	UpdateItemComboboxWithHashMap(m_cbRuneWordItemComboBox, ::theApp.g_hashMap_itemSelection_RuneWord, m_RuneWordItemKeys);
	UpdateItemComboboxWithHashMap(m_cbCraftItemComboBox, ::theApp.g_hashMap_itemSelection_Craft, m_CraftItemKeys);
	UpdateItemComboboxWithHashMap(m_cbNormalItemComboBox, ::theApp.g_hashMap_itemSelection_Normal, m_NormalItemKeys);
}

void CDlgSummary::LoadText(void) {
	m_s1.SetString((LPCTSTR)_T("This string is set to \"s1\""));

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

void CDlgSummary::UpdateSelectedItemCombobox(std::vector<ItemWithCharacterName> & items)
{
	m_cbSelectedItemComboBox.ResetContent();
	for (const auto& item : items) {
		CString itemName = item.item.ItemName().GetString(),
			content = _T("");
		content.AppendFormat(_T("%s (%s)"), itemName, item.characterName.GetString());
		m_cbSelectedItemComboBox.AddString(content);
	}
	if (m_cbSelectedItemComboBox.GetCount() > 0) {
		m_cbSelectedItemComboBox.SetCurSel(0);
	}
}

void CDlgSummary::OnCbnSelchangeComboNormal()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_Normal);
	int nIndex = pCombo->GetCurSel();
	CString key = m_NormalItemKeys[nIndex], msg=_T("");
	std::vector<ItemWithCharacterName> items = ::theApp.g_hashMap_itemSelection_Normal[key];

	msg.AppendFormat(_T("Selected Normal item: %d | %s | %d"), nIndex, key, items.size());
	m_s1.SetString(msg);

	UpdateSelectedItemCombobox(items);

	UpdateData(FALSE);
}

void CDlgSummary::OnCbnSelchangeComboMagic()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_Magic);
	int nIndex = pCombo->GetCurSel();
	CString key = m_MagicItemKeys[nIndex], msg = _T("");
	std::vector<ItemWithCharacterName> items = ::theApp.g_hashMap_itemSelection_Magic[key];

	msg.AppendFormat(_T("Selected Magic item: %d | %s | %d"), nIndex, key, items.size());
	m_s1.SetString(msg);

	UpdateSelectedItemCombobox(items);

	UpdateData(FALSE);
}

void CDlgSummary::OnCbnSelchangeComboRare()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_Rare);
	int nIndex = pCombo->GetCurSel();
	CString key = m_RareItemKeys[nIndex], msg = _T("");
	std::vector<ItemWithCharacterName> items = ::theApp.g_hashMap_itemSelection_Rare[key];

	msg.AppendFormat(_T("Selected Rare item: %d | %s | %d"), nIndex, key, items.size());
	m_s1.SetString(msg);

	UpdateSelectedItemCombobox(items);

	UpdateData(FALSE);
}

void CDlgSummary::OnCbnSelchangeComboSet()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_Set);
	int nIndex = pCombo->GetCurSel();
	CString key = m_SetItemKeys[nIndex], msg = _T("");
	std::vector<ItemWithCharacterName> items = ::theApp.g_hashMap_itemSelection_Set[key];

	msg.AppendFormat(_T("Selected Set item: %d | %s | %d"), nIndex, key, items.size());
	m_s1.SetString(msg);
	
	UpdateSelectedItemCombobox(items);

	UpdateData(FALSE);
}

void CDlgSummary::OnCbnSelchangeComboUnique()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_Unique);
	int nIndex = pCombo->GetCurSel();
	CString key = m_UniqueItemKeys[nIndex], msg = _T("");
	std::vector<ItemWithCharacterName> items = ::theApp.g_hashMap_itemSelection_Unique[key];

	msg.AppendFormat(_T("Selected Unique item: %d | %s | %d"), nIndex, key, items.size());
	m_s1.SetString(msg);

	UpdateSelectedItemCombobox(items);

	UpdateData(FALSE);
}

void CDlgSummary::OnCbnSelchangeComboRuneWord()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_RuneWord);
	int nIndex = pCombo->GetCurSel();
	CString key = m_RuneWordItemKeys[nIndex], msg = _T("");
	std::vector<ItemWithCharacterName> items = ::theApp.g_hashMap_itemSelection_RuneWord[key];

	msg.AppendFormat(_T("Selected RuneWord item: %d | %s | %d"), nIndex, key, items.size());
	m_s1.SetString(msg);

	UpdateSelectedItemCombobox(items);

	UpdateData(FALSE);
}

void CDlgSummary::OnCbnSelchangeComboCraft()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_Craft);
	int nIndex = pCombo->GetCurSel();
	CString key = m_CraftItemKeys[nIndex], msg = _T("");
	std::vector<ItemWithCharacterName> items = ::theApp.g_hashMap_itemSelection_Craft[key];

	msg.AppendFormat(_T("Selected Craft item: %d | %s | %d"), nIndex, key, items.size());
	m_s1.SetString(msg);

	UpdateSelectedItemCombobox(items);

	UpdateData(FALSE);
}