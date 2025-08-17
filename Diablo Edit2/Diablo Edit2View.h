
// Diablo Edit2View.h: CDiabloEdit2View 类的接口
//

#pragma once

#include "afxcmn.h"
#include "CharacterDialogBase.h"
#include "Diablo Edit2Doc.h"

class CDiabloEdit2View : public CFormView
{
protected: // 仅从序列化创建
	CDiabloEdit2View() noexcept;
	DECLARE_DYNCREATE(CDiabloEdit2View)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_DIABLOEDIT2_FORM };
#endif

// 特性
public:
	CDiabloEdit2Doc* GetDocument() const;

// 操作
public:

// 重写
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CDiabloEdit2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
// 自定义成员
private:
	CString	m_sTitle;
	CString	m_sPathName;
	CCharacterDialogBase	** m_dlgTabPage;
	CTabCtrl m_tcTab;
	int	m_nTabCurSel,m_nTabPageCount;
	CD2S_Struct	m_Character;

// 自定义函数
	void InitUI(void);
	void RefreshUI(void);
	BOOL GatherAllData(void);
    void LoadText();
	BOOL ReadD2sFile(const CString & path);
	BOOL WriteD2sFile(const CString & path);
public:
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnCloseFile();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg void OnUpdateLanguage1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLanguage2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLanguage3(CCmdUI *pCmdUI);
	afx_msg void OnLanguage1();
	afx_msg void OnLanguage2();
	afx_msg void OnLanguage3();
};

#ifndef _DEBUG  // Diablo Edit2View.cpp 中的调试版本
inline CDiabloEdit2Doc* CDiabloEdit2View::GetDocument() const
   { return reinterpret_cast<CDiabloEdit2Doc*>(m_pDocument); }
#endif

