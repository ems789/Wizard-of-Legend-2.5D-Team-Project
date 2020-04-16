#pragma once

// CMySheet
#include "TilePage.h"
#include "WallPage.h"

class CMySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMySheet)

public:
	CMySheet(CWnd *pParentWnd = NULL);
	//CMySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	//CMySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMySheet();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CTilePage m_TilePage;
	CWallPage m_WallPage;
};


