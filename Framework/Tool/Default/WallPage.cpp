// WallPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "WallPage.h"
#include "afxdialogex.h"


// CWallPage ��ȭ �����Դϴ�.

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


// CWallPage �޽��� ó�����Դϴ�.
