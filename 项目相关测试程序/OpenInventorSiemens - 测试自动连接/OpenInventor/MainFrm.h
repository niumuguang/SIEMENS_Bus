
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "MainFrm.h"

class CStyleBar : public CToolBar
{
public:
	CComboBox   m_comboBox;
	CFont       m_font;
};

class CMainFrame : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	CMFCToolBar m_ViewToolBar;

	bool m_bTimer;
	UINT m_nTimer;

public:
	afx_msg void OnTimerOn(void);
	afx_msg void OnTimerOff(void);
	void OnUpdateTimerOn(CCmdUI *pCmdUI);
	void OnUpdateTimerOff(CCmdUI *pCmdUI);

	afx_msg void OnCreateIBar();
	afx_msg void OnDllDlg();
	afx_msg void OnFileSaveTest();
	afx_msg void OnAutoConn();
	afx_msg void OnCreatZbar();
	afx_msg void OnModelMove();
	afx_msg void OnCreatHead();
	afx_msg void OnAutoConnect();
};


