#pragma once
#include "afxwin.h"


// PathTool 대화 상자입니다.

class CPathTool : public CDialog
{
	DECLARE_DYNAMIC(CPathTool)

public:
	CPathTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnDropFiles(HDROP hDropInfo);

public: // User Functions
	void SettingHorizontalScrollSize();

public: // User Variables
	list<PATH_INFO*>	m_PathInfoList;
	CListBox m_ListBox;
};
