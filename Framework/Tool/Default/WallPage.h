#pragma once


// CWallPage ��ȭ �����Դϴ�.

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
};
