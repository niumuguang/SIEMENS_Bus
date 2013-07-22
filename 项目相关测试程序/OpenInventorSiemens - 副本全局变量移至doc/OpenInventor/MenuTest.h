#pragma once


// CMenuTest

class CMenuTest : public CMFCMenuButton
{
	DECLARE_DYNAMIC(CMenuTest)

public:
	CMenuTest();
	virtual ~CMenuTest();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


