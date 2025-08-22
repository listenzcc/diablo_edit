#include "stdafx.h"
#include "DlgSummary.h"
#include "Diablo Edit2.h"
#include "Diablo Edit2View.h"
#include <numeric> // 包含 accumulate




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
	DDX_Control(pDX, IDC_INV_CUBE2, m_stInfo);  // 绑定 Group-box
	DDX_Control(pDX, IDC_EDIT1, m_edItemDetails);
	DDX_Control(pDX, IDC_TREE2, m_treeItems);

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
	ON_CBN_SELCHANGE(IDC_COMBO_SelectedItem, &CDlgSummary::OnCbnSelchangeComboSelectedItem)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, &CDlgSummary::OnTvnSelchangedTreeItems)
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

// 选择改变事件
void CDlgSummary::OnTvnSelchangedTreeItems(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hSelected = m_treeItems.GetSelectedItem();
	if (hSelected)
	{
		CString strFullPath;
		HTREEITEM hCurrent = hSelected;
		std::vector<CString> pathParts;

		// 构建路径
		while (hCurrent != NULL)
		{
			CString strName = m_treeItems.GetItemText(hCurrent);
			pathParts.push_back(strName);
			hCurrent = m_treeItems.GetParentItem(hCurrent);
		}

		CString joinedPath = std::accumulate(
			std::next(pathParts.begin()),
			pathParts.end(),
			pathParts[0],
			[](const CString& a, const CString& b) {
				return a + _T(" < ") + b;
			}
		);

		int lvl = pathParts.size();
		joinedPath.AppendFormat(_T(" (level: %d)"), lvl);

		if (lvl == 3) {
			//MessageBox(joinedPath, _T("选中的节点路径"));

			CString key = pathParts[0];
			std::vector<ItemWithCharacterName> iwcns;

			if (pathParts[1] == _T("Normal")) {
				// 处理普通物品
				iwcns = ::theApp.g_hashMap_itemSelection_Normal[key];
			}
			else if (pathParts[1] == _T("Magic")) {
				// 处理魔法物品
				iwcns = ::theApp.g_hashMap_itemSelection_Magic[key];
			}
			else if (pathParts[1] == _T("Rare")) {
				// 处理稀有物品
				iwcns = ::theApp.g_hashMap_itemSelection_Rare[key];
			}
			else if (pathParts[1] == _T("Set")) {
				// 处理套装物品
				iwcns = ::theApp.g_hashMap_itemSelection_Set[key];
			}
			else if (pathParts[1] == _T("Unique")) {
				// 处理独特物品
				iwcns = ::theApp.g_hashMap_itemSelection_Unique[key];
			}
			else if (pathParts[1] == _T("RuneWord")) {
				// 处理符文之语
				iwcns = ::theApp.g_hashMap_itemSelection_RuneWord[key];
			}
			else if (pathParts[1] == _T("Craft")) {
				// 处理制作物品
				iwcns = ::theApp.g_hashMap_itemSelection_Craft[key];
			}
			UpdateSelectedItemCombobox(iwcns);

		}
	}

	*pResult = 0;
}

void CDlgSummary::UpdateUI(const CD2S_Struct& character)
{
	// Only update after data is fully loaded

	if (::theApp.dataIsFullyLoaded) {
		LoadText();
		UpdateItemsCombobox();

		m_treeItems.DeleteAllItems(); // Clear the tree control before updating
		// 添加根节点

		HTREEITEM hRoot = m_treeItems.InsertItem(_T("Items"), 0, 1, TVI_ROOT),
			hNorm = m_treeItems.InsertItem(_T("Normal"), 0, 1, hRoot),
			hMagic = m_treeItems.InsertItem(_T("Magic"), 0, 1, hRoot),
			hRare = m_treeItems.InsertItem(_T("Rare"), 0, 1, hRoot),
			hUnique = m_treeItems.InsertItem(_T("Unique"), 0, 1, hRoot),
			hSet = m_treeItems.InsertItem(_T("Set"), 0, 1, hRoot),
			hRuneWord = m_treeItems.InsertItem(_T("RuneWord"), 0, 1, hRoot),
			hCraft = m_treeItems.InsertItem(_T("Craft"), 0, 1, hRoot);

		// 添加子节点
		for (auto& name : m_NormalItemKeys) {
			// 在普通物品下添加子节点
			m_treeItems.InsertItem(name, 2, 3, hNorm);
		}

		for (auto& name : m_MagicItemKeys) {
			// 在魔法物品下添加子节点
			m_treeItems.InsertItem(name, 2, 3, hMagic);
		}

		for (auto& name : m_RareItemKeys) {
			// 在稀有物品下添加子节点
			m_treeItems.InsertItem(name, 2, 3, hRare);
		}

		for (auto& name : m_UniqueItemKeys) {
			// 在独特物品下添加子节点
			m_treeItems.InsertItem(name, 2, 3, hUnique);
		}

		for (auto& name : m_SetItemKeys) {
			// 在套装物品下添加子节点
			m_treeItems.InsertItem(name, 2, 3, hSet);
		}

		for (auto& name : m_RuneWordItemKeys) {
			// 在符文之语下添加子节点
			m_treeItems.InsertItem(name, 2, 3, hRuneWord);
		}

		for (auto& name : m_CraftItemKeys) {
			// 在制作物品下添加子节点
			m_treeItems.InsertItem(name, 2, 3, hCraft);
		}

		// 展开根节点
		m_treeItems.Expand(hRoot, TVE_EXPAND);
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

	std::vector<CString> sortedKeys;
	for (const auto& pair : hashMap) {
		sortedKeys.push_back(pair.first);
	}
	std::qsort(&sortedKeys[0], sortedKeys.size(), sizeof(CString),
		[](const void* a, const void* b) -> int {
			return ((CString*)a)->Compare(*(CString*)b);
		});

	for (const auto& key : sortedKeys) {
		const auto& pair = hashMap.find(key);
		if (pair != hashMap.end()) {
			// 处理键值对
			CString content = _T("");
			const size_t count = pair->second.size();  // 获取向量的大小
			content.AppendFormat(_T("%s (%d)"), key, (int)count);  // 添加计数到键后面
			ccb.AddString(content);  // 添加键到组合框
			keys.push_back(key);  // 将键添加到传入的向量中
		}
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
}

void CDlgSummary::UpdateSelectedItemCombobox(std::vector<ItemWithCharacterName> & iwcns)
{
	m_cbSelectedItemComboBox.ResetContent();
	m_selectedItemVector.clear(); // Clear the vector to store new items
	for (const auto& iwcn : iwcns) {
		CString itemName = iwcn.item.ItemName().GetString(),
			content = _T("");
		content.AppendFormat(_T("%s (%s)"), itemName, iwcn.characterName.GetString());
		m_cbSelectedItemComboBox.AddString(content);
		m_selectedItemVector.push_back(iwcn.item); // Store the item in the vector for later use
	}
	if (m_cbSelectedItemComboBox.GetCount() > 0) {
		m_cbSelectedItemComboBox.SetCurSel(0);
	}

	//const CD2Item* item = 0;
	//auto iwcn = iwcns[0];
	//item = &iwcn.item;
	//int x = 77, gems = 0; // Default values for x and gems

	if (m_pDlgItemInfo) m_pDlgItemInfo.reset();

	//ShowItemInfoDlg(item, x, 0); // Show item info dialog with the first item in the list

	OnCbnSelchangeComboSelectedItem(); // Trigger the selection change to update the display

	UpdateData(FALSE);
}

void CDlgSummary::OnCbnSelchangeComboSelectedItem()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SelectedItem);
	int nIndex = pCombo->GetCurSel();
	if (nIndex < 0 || nIndex >= m_selectedItemVector.size()) return;
	const CD2Item* pItem = &m_selectedItemVector[nIndex];
	CString msg = _T("");
	msg.AppendFormat(_T("Selected item: %d | %s"), nIndex, pItem->ItemName().GetString());
	m_s1.SetString(msg);
	
	if (m_pDlgItemInfo) m_pDlgItemInfo.reset();
	ShowItemInfoDlg(pItem, 77, 0); // Show item info dialog with the selected item
	UpdateData(FALSE);
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

void CDlgSummary::ShowItemInfoDlg(const CD2Item* pItem, int x, int gems) {
	m_bNotShowItemInfoDlg = FALSE; // This should be set based on user preference or application state
	m_scTrasparent.SetRange(0, 255);
	m_scTrasparent.SetPos(200); // Set default transparency, can be adjusted based on user preference

	if (!m_bNotShowItemInfoDlg && pItem && (!m_pDlgItemInfo || pItem != m_pDlgItemInfo->GetItemPtr())) {
		if (!m_pDlgItemInfo) {
			m_pDlgItemInfo = std::make_unique<CDlgSuspend>(this, 200);
			m_pDlgItemInfo->Create(CDlgSuspend::IDD, NULL);
			m_pDlgItemInfo->m_bAllowMouseHoverHide = FALSE; // Disable auto-hide on mouse hover
		}
		m_pDlgItemInfo->GetItemInfo(pItem, gems);

		CString details = _T("");
		for (std::pair<BYTE, CString> & pair : m_pDlgItemInfo->m_sItemMsg) {
			details.AppendFormat(_T("%s\r\n"), pair.second);
		}

		m_edItemDetails.SetWindowText(details); // Display item details in the edit control

		CRect rect, rect1, rect2;
		m_pDlgItemInfo->GetWindowRect(&rect);
		GetWindowRect(&rect1);
		m_stInfo.GetWindowRect(&rect2);

		m_pDlgItemInfo->MoveWindow(rect2.left, rect2.top, rect.Width(), rect.Height(), TRUE);
		m_pDlgItemInfo->ShowWindow(SW_SHOWNOACTIVATE); //显示对话框
		m_pDlgItemInfo->Invalidate();
	}
	else if (!pItem && m_pDlgItemInfo) {
		m_pDlgItemInfo.reset();
	}

}