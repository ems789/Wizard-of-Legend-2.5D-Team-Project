// MySheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MySheet.h"


// CMySheet

IMPLEMENT_DYNAMIC(CMySheet, CPropertySheet)

//CMySheet::CMySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
//	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
//{
//
//}
//
//CMySheet::CMySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
//	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
//{
//	CPropertySheet::AddPage(&m_TilePage);
//}

CMySheet::CMySheet(CWnd* pParentWnd)
{
	CPropertySheet::AddPage(&m_TilePage);
	CPropertySheet::AddPage(&m_WallPage);
}

CMySheet::~CMySheet()
{
}


BEGIN_MESSAGE_MAP(CMySheet, CPropertySheet)
END_MESSAGE_MAP()


// CMySheet 메시지 처리기입니다.
