#pragma once
// CTilePage ��ȭ �����Դϴ�.

#include "Engine_Defines.h"

BEGIN(Engine)

class CGraphicDev;

END

class CTilePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTilePage)

public:
	CTilePage();
	virtual ~CTilePage();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TILEPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public: // Message Functions
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

public: // User Functions
	void SettingHorizontalScrollSize();

public:	// Control Variables
	CListBox m_ListBox;
	CStatic m_Picture;

public:
	int m_iDrawID;

	Engine::CGraphicDev* m_pGraphicDev;
};
