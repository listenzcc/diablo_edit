#pragma once
#include <vector>
#include "D2Item.h"
#include "D2Struct.h"
#include <afxcmn.h>
#include <afxwin.h>

// ��Ʒɸѡ Tab �Ի���
class CDlgItemFilter : public CDialog
{
public:
    enum { IDD = IDD_DIALOG_ITEM_FILTER };
    CDlgItemFilter(CWnd* pParent = nullptr);

    // �ռ����н�ɫ��������Ʒ
    void CollectAllItems();
    // ִ��ɸѡ
    void FilterItems(const CString& playerName, const CString& itemName, int quality);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

    // UI �ؼ�
    CListCtrl m_lstItems;
    CEdit m_edPlayerName;
    CEdit m_edItemName;
    CComboBox m_cbQuality;
    CButton m_btnFilter;

    // ������Ʒ����
    struct ItemInfo {
        CString playerName;
        CString itemName;
        int quality;
        CD2Item item;
    };
    std::vector<ItemInfo> m_allItems;
    std::vector<ItemInfo> m_filteredItems;

    afx_msg void OnBnClickedFilter();
    DECLARE_MESSAGE_MAP()
};
