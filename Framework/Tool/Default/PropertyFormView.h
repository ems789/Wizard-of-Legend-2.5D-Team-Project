#pragma once

// CPropertyFormView 폼 뷰입니다.
#include "MySheet.h"

#include "PathTool.h"

class CPropertyFormView : public CFormView
{
	DECLARE_DYNCREATE(CPropertyFormView)

protected:
	CPropertyFormView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CPropertyFormView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPERTYFORMVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedPathTool();

public:
	CMySheet* m_MySheet;
	CPathTool m_PathTool;

	int	 m_iTerrainX;
	int	 m_iTerrainZ;
	int	 m_iTerrainItv;

};


