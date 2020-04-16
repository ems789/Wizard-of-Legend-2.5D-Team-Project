#pragma once


// CWallPage 대화 상자입니다.

class CWallPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CWallPage)

public:
	CWallPage();
	virtual ~CWallPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WALLPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
