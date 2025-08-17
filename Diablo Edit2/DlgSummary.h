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

// �Զ����Ա
private:
	int a;
	// IDC_COMBOBOX1
	CComboBox m_cbCharComboBox;
	CComboBox m_cbItemComboBox;
	CString m_s1;

// �Զ��庯��
public:
	void UpdateUI(const CD2S_Struct& character);
	BOOL GatherData(CD2S_Struct& character);
	void ResetAll();
	void LoadText(void);	//���ؿؼ����ַ�������
private:
	void InitUI(void);
	void RefreshUI(void);
public:
	int b;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
