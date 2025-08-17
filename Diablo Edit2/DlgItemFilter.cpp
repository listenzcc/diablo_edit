#include "stdafx.h"
#include "DlgItemFilter.h"
#include <filesystem>
#include <fstream>
#include "D2Item.h"
#include "D2Struct.h"

using namespace std;
namespace fs = std::filesystem;

IMPLEMENT_DYNAMIC(CDlgItemFilter, CDialog)

CDlgItemFilter::CDlgItemFilter(CWnd* pParent)
    : CDialog(CDlgItemFilter::IDD, pParent)
{
}

void CDlgItemFilter::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_ITEMS, m_lstItems);
    DDX_Control(pDX, IDC_EDIT_PLAYERNAME, m_edPlayerName);
    DDX_Control(pDX, IDC_EDIT_ITEMNAME, m_edItemName);
    DDX_Control(pDX, IDC_COMBO_QUALITY, m_cbQuality);
    DDX_Control(pDX, IDC_BUTTON_FILTER, m_btnFilter);
}

BEGIN_MESSAGE_MAP(CDlgItemFilter, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_FILTER, &CDlgItemFilter::OnBnClickedFilter)
END_MESSAGE_MAP()

BOOL CDlgItemFilter::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_lstItems.InsertColumn(0, _T("角色名"), LVCFMT_LEFT, 120);
    m_lstItems.InsertColumn(1, _T("物品名"), LVCFMT_LEFT, 180);
    m_lstItems.InsertColumn(2, _T("品质"), LVCFMT_LEFT, 60);
    // 初始化品质下拉框
    m_cbQuality.AddString(_T("全部"));
    m_cbQuality.AddString(_T("普通"));
    m_cbQuality.AddString(_T("魔法"));
    m_cbQuality.AddString(_T("稀有"));
    m_cbQuality.AddString(_T("套装"));
    m_cbQuality.AddString(_T("唯一"));
    m_cbQuality.SetCurSel(0);
    CollectAllItems();
    FilterItems(_T(""), _T(""), -1);
    return TRUE;
}

void CDlgItemFilter::CollectAllItems()
{
    m_allItems.clear();
    CString profilePath = ::theApp.GetUserProfilePath();
    vector<fs::path> files;
    for (const auto& entry : fs::recursive_directory_iterator((LPCTSTR)profilePath)) {
        if (entry.is_regular_file() && entry.path().extension() == L".d2s") {
            files.push_back(entry.path());
        }
    }
    for (const auto& file : files) {
        CD2S_Struct character;
        std::ifstream in(file, std::ios::binary);
        if (!in) continue;
        // 这里假设有 LoadFromStream 方法
        if (!character.LoadFromStream(in)) continue;
        CString playerName = character.name();
        for (auto& item : character.ItemList.vItems) {
            ItemInfo info;
            info.playerName = playerName;
            info.itemName = item.ItemName();
            info.quality = item.Quality();
            info.item = item;
            m_allItems.push_back(info);
        }
    }
}

void CDlgItemFilter::FilterItems(const CString& playerName, const CString& itemName, int quality)
{
    m_filteredItems.clear();
    for (const auto& info : m_allItems) {
        if (!playerName.IsEmpty() && info.playerName.Find(playerName) == -1) continue;
        if (!itemName.IsEmpty() && info.itemName.Find(itemName) == -1) continue;
        if (quality >= 0 && info.quality != quality) continue;
        m_filteredItems.push_back(info);
    }
    m_lstItems.DeleteAllItems();
    for (size_t i = 0; i < m_filteredItems.size(); ++i) {
        const auto& info = m_filteredItems[i];
        int idx = m_lstItems.InsertItem(i, info.playerName);
        m_lstItems.SetItemText(idx, 1, info.itemName);
        m_lstItems.SetItemText(idx, 2, CString(std::to_wstring(info.quality).c_str()));
    }
}

void CDlgItemFilter::OnBnClickedFilter()
{
    CString playerName, itemName;
    m_edPlayerName.GetWindowText(playerName);
    m_edItemName.GetWindowText(itemName);
    int quality = m_cbQuality.GetCurSel() - 1;
    FilterItems(playerName, itemName, quality);
}
