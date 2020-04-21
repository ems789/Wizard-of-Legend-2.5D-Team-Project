// PropertyFormView.cpp : 구현 파일입니다.

#include "stdafx.h"
#include "Tool.h"
#include "ToolView.h"
#include "PropertyFormView.h"

#include "GraphicDev.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "TerrainGuidLine.h"

// CPropertyFormView

IMPLEMENT_DYNCREATE(CPropertyFormView, CFormView)

CPropertyFormView::CPropertyFormView()
	: CFormView(IDD_PROPERTYFORMVIEW)	
	, m_iTerrainX(10)
	, m_iTerrainZ(10)
	, m_iTerrainItv(1)
{

}

CPropertyFormView::~CPropertyFormView()
{
	Safe_Delete(m_MySheet);
}

void CPropertyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iTerrainX);
	DDX_Text(pDX, IDC_EDIT2, m_iTerrainZ);
	DDX_Text(pDX, IDC_EDIT3, m_iTerrainItv);
}

BEGIN_MESSAGE_MAP(CPropertyFormView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CPropertyFormView::OnBnClickedRefresh)
	ON_BN_CLICKED(IDC_BUTTON2, &CPropertyFormView::OnBnClickedPathTool)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CPropertyFormView 진단입니다.

#ifdef _DEBUG
void CPropertyFormView::AssertValid() const 
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPropertyFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPropertyFormView 메시지 처리기입니다.


void CPropertyFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// 다이얼로그 생성
	m_PathTool.Create(IDD_PATHTOOL);


	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CWnd* pPlaceHolder = GetDlgItem(IDC_PLACEHOLDER);
	m_MySheet = new CMySheet(pPlaceHolder);


	if (!m_MySheet->Create(pPlaceHolder, WS_CHILD | WS_VISIBLE, 0))
	{
		Safe_Delete(m_MySheet);
		return;
	}

	CRect rcSheet;
	pPlaceHolder->GetWindowRect(&rcSheet);
	ScreenToClient(&rcSheet);


	CTabCtrl *TabCtrl = m_MySheet->GetTabControl();
	TabCtrl->MoveWindow(0, 0, rcSheet.Width(), rcSheet.Height());
	m_MySheet->SetWindowPos(NULL, 0, 0, rcSheet.Width(), rcSheet.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	m_MySheet->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	m_MySheet->ModifyStyle(0, WS_TABSTOP);
	m_MySheet->SetActivePage(0); // Force the right & bottom borders to appear
}

void CPropertyFormView::OnBnClickedRefresh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CToolView* pToolView = dynamic_cast<CToolView*>(GetParentSplitter(this, FALSE)->GetPane(0, 1));

	// 지형을 재생성
	LPDIRECT3DDEVICE9 pGraphicDev = Engine::CGraphicDev::GetInstance()->GetDevice();
	pGraphicDev->AddRef();
	FAILED_CHECK_VOID(pGraphicDev);

	Engine::CResourcesMgr::GetInstance()->Remove_Resource(::RESOURCE_STATIC, L"Buffer_TerrainTex");
	Engine::CResourcesMgr::GetInstance()->Remove_Resource(::RESOURCE_STATIC, L"Buffer_TileTex");
	pToolView->m_pTerrain->Release();
	pToolView->m_pTerrainGuidLine->Release();

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TerrainTex",
		Engine::BUFFER_TERRAINTEX, 
		Engine::WALL_OUTER,
		D3DXCOLOR(1.f, 1.f, 1.f, 1.f),
		L"",
		m_iTerrainX + 1,
		m_iTerrainZ + 1,
		m_iTerrainItv),
		E_FAIL);

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TileTex",
		Engine::BUFFER_TILETEX,
		Engine::WALL_OUTER,
		D3DXCOLOR(1.f, 1.f, 1.f, 1.f),
		L"",
		0,
		0,
		m_iTerrainItv),
		E_FAIL);

	pToolView->m_pTerrainGuidLine = CTerrainGuidLine::Create(pGraphicDev);
	pToolView->m_pTerrain = CTerrain::Create(pGraphicDev, m_iTerrainX, m_iTerrainZ, m_iTerrainItv);
	NULL_CHECK_MSG(pToolView->m_pTerrainGuidLine, L"Terrain Create Failed");

	pGraphicDev->Release();

	pToolView->Invalidate(FALSE);
}

void CPropertyFormView::OnBnClickedPathTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NULL_CHECK_MSG(m_PathTool.GetSafeHwnd(), L"PathTool is null");
	m_PathTool.ShowWindow(SW_SHOW); // SW_SHOW: 창모양으로 출력.
}
