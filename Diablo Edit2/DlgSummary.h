#pragma once

#include "CharacterDialogBase.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"
#include "Diablo Edit2.h"
#include "DlgSuspend.h"

#include <tuple>
#include <memory>

const int GRID_WIDTH = 30;	//每个网格的边长(像素)

// SCALING to take into account the possiblity of different dialog base units
static int GRID_WIDTH_SCALED = GRID_WIDTH;

class CDlgSummary : public CCharacterDialogBase
{
    DECLARE_DYNAMIC(CDlgSummary)

	//悬浮窗
	static const int INFO_WINDOW_LEFT = 50;		//左边悬浮窗的位置X
	static const int INFO_WINDOW_RIGHT = 550;	//右边悬浮窗的位置X
	std::unique_ptr<CDlgSuspend> m_pDlgItemInfo;//显示物品信息的悬浮窗口
	BOOL m_bNotShowItemInfoDlg;					//是否隐藏物品信息悬浮窗
	CSliderCtrl m_scTrasparent;					//属性悬浮窗的透明度
	void ShowItemInfoDlg(const CD2Item* pItem, int x, int gems);	//显示/隐藏(pItem = 0)物品信息悬浮窗口。x用来选择窗口位置，gems为镶嵌宝石数量

public:
    CDlgSummary(CWnd* pParent = nullptr);
    virtual ~CDlgSummary();

    enum { IDD = IDD_DIALOG_Summary };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    //virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()

// 自定义成员
private:
	int a;
	// IDC_COMBOBOX1
	CComboBox m_cbCharComboBox,
		m_cbItemComboBox,
		m_cbNormalItemComboBox,
		m_cbMagicItemComboBox,
		m_cbRareItemComboBox,
		m_cbSetItemComboBox,
		m_cbUniqueItemComboBox,
		m_cbRuneWordItemComboBox,
		m_cbCraftItemComboBox,
		m_cbSelectedItemComboBox;

	CStatic m_stInfo;

	std::vector<CString> m_NormalItemKeys,
		m_MagicItemKeys,
		m_RareItemKeys,
		m_SetItemKeys,
		m_UniqueItemKeys,
		m_RuneWordItemKeys,
		m_CraftItemKeys;

	std::vector<CD2Item> m_selectedItemVector;

	CString m_s1;

// 自定义函数
public:
	void UpdateUI(const CD2S_Struct& character);
	void UpdateItemsCombobox();
	BOOL GatherData(CD2S_Struct& character);
	void ResetAll();
	void LoadText(void);	//加载控件的字符串内容
private:
	void InitUI(void);
	void RefreshUI(void);
	void UpdateSelectedItemCombobox(std::vector<ItemWithCharacterName> & iwcn);
public:
	int b;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeComboNormal();
	afx_msg void OnCbnSelchangeComboMagic();
	afx_msg void OnCbnSelchangeComboRare();
	afx_msg void OnCbnSelchangeComboSet();
	afx_msg void OnCbnSelchangeComboUnique();
	afx_msg void OnCbnSelchangeComboRuneWord();
	afx_msg void OnCbnSelchangeComboCraft();
	afx_msg void OnCbnSelchangeComboSelectedItem();
};
