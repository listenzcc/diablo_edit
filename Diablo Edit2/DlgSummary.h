#pragma once

#include "CharacterDialogBase.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"

class CDlgSummary : public CCharacterDialogBase
{
    DECLARE_DYNAMIC(CDlgSummary)
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
	CComboBox m_cbCharComboBox;
	CComboBox m_cbItemComboBox;
	CComboBox m_cbNormalItemComboBox;
	CComboBox m_cbMagicItemComboBox;
	CComboBox m_cbRareItemComboBox;
	CComboBox m_cbSetItemComboBox;
	CComboBox m_cbUniqueItemComboBox;
	CComboBox m_cbRuneWordItemComboBox;
	CComboBox m_cbCraftItemComboBox;

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
public:
	int b;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
