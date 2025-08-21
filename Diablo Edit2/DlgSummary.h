#pragma once

#include "CharacterDialogBase.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"
#include "Diablo Edit2.h"
#include "DlgSuspend.h"

#include <tuple>
#include <memory>

const int GRID_WIDTH = 30;	//ÿ������ı߳�(����)

// SCALING to take into account the possiblity of different dialog base units
static int GRID_WIDTH_SCALED = GRID_WIDTH;

class CDlgSummary : public CCharacterDialogBase
{
    DECLARE_DYNAMIC(CDlgSummary)

	//������
	static const int INFO_WINDOW_LEFT = 50;		//�����������λ��X
	static const int INFO_WINDOW_RIGHT = 550;	//�ұ���������λ��X
	std::unique_ptr<CDlgSuspend> m_pDlgItemInfo;//��ʾ��Ʒ��Ϣ����������
	BOOL m_bNotShowItemInfoDlg;					//�Ƿ�������Ʒ��Ϣ������
	CSliderCtrl m_scTrasparent;					//������������͸����
	void ShowItemInfoDlg(const CD2Item* pItem, int x, int gems);	//��ʾ/����(pItem = 0)��Ʒ��Ϣ�������ڡ�x����ѡ�񴰿�λ�ã�gemsΪ��Ƕ��ʯ����

public:
    CDlgSummary(CWnd* pParent = nullptr);
    virtual ~CDlgSummary();

    enum { IDD = IDD_DIALOG_Summary };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    //virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()

// �Զ����Ա
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

// �Զ��庯��
public:
	void UpdateUI(const CD2S_Struct& character);
	void UpdateItemsCombobox();
	BOOL GatherData(CD2S_Struct& character);
	void ResetAll();
	void LoadText(void);	//���ؿؼ����ַ�������
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
