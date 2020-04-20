#pragma once
// CWallPage ��ȭ �����Դϴ�.

#include "Engine_Defines.h"
#include "afxwin.h"

BEGIN(Engine)

class CGraphicDev;

END

class CWallPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CWallPage)

public:
	CWallPage();
	virtual ~CWallPage();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WALLPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public: // Message Functions
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();

public: // User Functions
	void SettingHorizontalScrollSize();

public:	// Control Variables
	CListBox m_ListBox;
	CStatic m_Picture;

public:
	Engine::CGraphicDev* m_pGraphicDev;

	int m_iDrawID;
	CButton m_Check[WALL_END];
	bool	m_bIsCheck[WALL_END];
};
