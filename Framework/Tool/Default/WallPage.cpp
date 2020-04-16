// WallPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "WallPage.h"
#include "afxdialogex.h"


// CWallPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CWallPage, CPropertyPage)

CWallPage::CWallPage()
	: CPropertyPage(IDD_WALLPAGE)
{

}

CWallPage::~CWallPage()
{
}

void CWallPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWallPage, CPropertyPage)
END_MESSAGE_MAP()


// CWallPage 메시지 처리기입니다.
