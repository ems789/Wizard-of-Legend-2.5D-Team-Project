
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "MainFrm.h"
#include "ToolDoc.h"
#include "ToolView.h"
#include "PropertyFormView.h"

#include "Terrain.h"
#include "TerrainGuidLine.h"
#include "DynamicCamera.h"
#include "Macro.h"
#include "Export_Function.h"

USING(Engine)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CToolView
IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ACTIVATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

HWND	  g_hWnd;
HINSTANCE g_hInst;

// CToolView 생성/소멸

CToolView::CToolView() :
	m_pDeviceClass(Engine::CGraphicDev::GetInstance()),
	m_pTerrainGuidLine(nullptr),
	m_pDynamicCamera(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
	m_pTerrain->Release();
	m_pTerrainGuidLine->Release();
	m_pDynamicCamera->Release();

	Engine::Release_Resources();
	Engine::CInputDev::GetInstance()->DestroyInstance();
	Engine::Safe_Release(m_pDeviceClass);
	Engine::CGraphicDev::GetInstance()->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
#ifdef _DEBUG
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
#endif // _DEBUG

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	m_pDeviceClass->Render_Begin(D3DCOLOR_ARGB(255, 153, 217, 234));

	m_pTerrainGuidLine->Render_GameObject();
	m_pTerrain->Update_GameObject(1.f);
	m_pTerrain->Render_GameObject();

	m_pDeviceClass->Render_End();
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();

	// View 크기를 조정하기 위해서는 프레임 원도우의 크기를 건드린다.
	// 프레임 윈도우의 포인터 얻어오는 방법.
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK_MSG(pFrameWnd, L"pFrameWnd is null");

	RECT rcFrame = {};

	// 윈도우의 크기(테두리 + 클라이언트 영역) 얻어오는 함수.(스크린 좌표 기준)
	pFrameWnd->GetWindowRect(&rcFrame);
	::SetRect(&rcFrame, 0, 0, rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);

	RECT rcView = {};

	// 윈도우의 클라이언트 영역(테두리를 포함하지 않은) 크기 얻어오는 함수. (클라이언트 좌표 기준)
	GetClientRect(&rcView);

	// 현재 프레임윈도우와 View 사이의 갭들을 구한다.
	int iGapX = rcFrame.right - rcView.right;
	int iGapY = rcFrame.bottom - rcView.bottom;

	// 프레임윈도우의 크기를 새로 설정.
	pFrameWnd->SetWindowPos(nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER | SWP_NOMOVE);


	// 장치 초기화
	FAILED_CHECK_VOID(Engine::Ready_GraphicDev(
		g_hWnd,
		Engine::MODE_WIN,
		WINCX,
		WINCY,
		&m_pDeviceClass));
	m_pDeviceClass->AddRef();

	m_pGraphicDev = m_pDeviceClass->GetDevice();
	m_pGraphicDev->AddRef();
	FAILED_CHECK_VOID(m_pGraphicDev);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 메모리 예약
	Engine::Reserve_ContainerSize(::RESOURCE_END);

	CPropertyFormView* pPropertyFormView = dynamic_cast<CPropertyFormView*>(GetParentSplitter(this, FALSE)->GetPane(0, 0));

	// 버퍼 초기화
	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(m_pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TerrainTex",
		Engine::BUFFER_TERRAINTEX, 
		Engine::WALL_OUTER,
		D3DXCOLOR(1.f, 1.f, 1.f, 1.f),
		L"",
		pPropertyFormView->m_iTerrainX + 1, // 정점의 개수이므로 1을 더함
		pPropertyFormView->m_iTerrainZ + 1,
		pPropertyFormView->m_iTerrainItv),
		E_FAIL);

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(m_pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TileTex",
		Engine::BUFFER_TILETEX),
		E_FAIL);

	// 시계 방향 벽 텍스쳐
	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(m_pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_LeftWallTex_Outer",
		Engine::BUFFER_LEFTWALLTEX,
		WALL_OUTER),
		E_FAIL);

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(m_pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TopWallTex_Outer",
		Engine::BUFFER_TOPWALLTEX,
		WALL_OUTER),
		E_FAIL);

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(m_pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_RightWallTex_Outer",
		Engine::BUFFER_RIGHTWALLTEX,
		WALL_OUTER),
		E_FAIL);

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(m_pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_BottomWallTex_Outer",
		Engine::BUFFER_BOTTOMWALLTEX,
		Engine::WALL_OUTER),
		E_FAIL);


	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(m_pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_CeilingWallTex",
		Engine::BUFFER_CEILINGWALLTEX),
		E_FAIL);

	// 반시계 방향 벽 텍스쳐
	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(m_pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_LeftWallTex_Inner",
		Engine::BUFFER_LEFTWALLTEX,
		Engine::WALL_INNER),
		E_FAIL);

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(m_pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TopWallTex_Inner",
		Engine::BUFFER_TOPWALLTEX,
		Engine::WALL_INNER),
		E_FAIL);

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(m_pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_RightWallTex_Inner",
		Engine::BUFFER_RIGHTWALLTEX,
		Engine::WALL_INNER),
		E_FAIL);

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(m_pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_BottomWallTex_Inner",
		Engine::BUFFER_BOTTOMWALLTEX,
		Engine::WALL_INNER),
		E_FAIL);

	// 타일 텍스쳐 로드
	//FAILED_CHECK_RETURN_VOID(Engine::Ready_Texture(m_pGraphicDev, ::RESOURCE_STAGE, L"Texture_Tile", Engine::TEX_NORMAL, L"../../Client/Bin/Resource/Texture/ForestStage/ForestStageTile/ForestTile%d.png", 321), E_FAIL);
	FAILED_CHECK_RETURN_VOID(Engine::Ready_Texture(m_pGraphicDev, ::RESOURCE_STAGE, L"Texture_Tile", Engine::TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Tile/Tile%d.png", 449), E_FAIL);

	// 벽 텍스쳐 로드
	FAILED_CHECK_RETURN_VOID(Engine::Ready_Texture(m_pGraphicDev, ::RESOURCE_STAGE, L"Texture_Wall", Engine::TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Wall/Wall%d.png", 267), E_FAIL);

	m_pTerrainGuidLine = CTerrainGuidLine::Create(m_pGraphicDev);
	NULL_CHECK_MSG(m_pTerrainGuidLine, L"TerrainGuidLine Create Failed");

	// 터레인 만들기(현재 지정되어 있는 x * y 크기만큼)
	m_pTerrain = CTerrain::Create(m_pGraphicDev, pPropertyFormView->m_iTerrainX, 
												pPropertyFormView->m_iTerrainZ,
												pPropertyFormView->m_iTerrainItv);
	NULL_CHECK_MSG(m_pTerrain, L"Terrain Create Failed");


	// dynamicCamera
	m_pDynamicCamera = CDynamicCamera::Create(m_pGraphicDev,
		&::_vec3(0.f, 5.f, -5.f),
		&::_vec3(0.f, 0.f, 0.f),
		&::_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f),
		::_float(WINCX) / WINCY,
		1.f,
		1000.f);
	NULL_CHECK_RETURN_VOID(m_pDynamicCamera);

	// INPUTDEV
	FAILED_CHECK_RETURN_VOID(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

}

void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	Engine::Set_InputDev();


	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CPropertyFormView* pPropertyFormView = dynamic_cast<CPropertyFormView*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
	NULL_CHECK(pPropertyFormView);

	// 키보드 입력으로 체크 박스 체크
	if (pPropertyFormView->m_MySheet->GetActiveIndex() == 1) // 툴 일때만 작동
	{
		if (Engine::Get_DIKeyState(DIK_Z) & 0x80)
		{
			pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_CEILING].SetCheck(!pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_CEILING].GetCheck());
			pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_CEILING] = pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_CEILING].GetCheck();
		}
		else if (Engine::Get_DIKeyState(DIK_X) & 0x80)
		{
			pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_LEFT].SetCheck(!pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_LEFT].GetCheck());
			pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_LEFT] = pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_LEFT].GetCheck();
		}
		else if (Engine::Get_DIKeyState(DIK_C) & 0x80)
		{
			pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_TOP].SetCheck(!pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_TOP].GetCheck());
			pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_TOP] = pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_TOP].GetCheck();
		}
		else if (Engine::Get_DIKeyState(DIK_V) & 0x80)
		{
			pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_RIGHT].SetCheck(!pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_RIGHT].GetCheck());
			pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_RIGHT] = pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_RIGHT].GetCheck();

		}
		else if (Engine::Get_DIKeyState(DIK_B) & 0x80)
		{
			pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_BOTTOM].SetCheck(!pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_BOTTOM].GetCheck());
			pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_BOTTOM] = pPropertyFormView->m_MySheet->m_WallPage.m_Check[WALL_BOTTOM].GetCheck();
		}
	}


	// 오브젝트 일렬로 일괄 번경
	const Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<const Engine::CTerrainTex*>(m_pTerrainGuidLine->Get_Component(L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK_RETURN_VOID(pTerrainBufferCom);

	const Engine::CTransform*	pTerrainTransformCom = dynamic_cast<const Engine::CTransform*>(m_pTerrainGuidLine->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN_VOID(pTerrainTransformCom);

	::_vec3 vPickedTile = { 0, 0, 0 };
	int iDrawID = 0;
	// z 방향
	if ((Engine::Get_DIKeyState(DIK_N) & 0x80))
	{		
		vPickedTile = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
		int iSrcZ = (int)m_vLastPickTile.z;
		int iDestZ = (int)vPickedTile.z;
		
		switch (pPropertyFormView->m_MySheet->GetActiveIndex())
		{
		// 타일
		case 0:
		{
			iDrawID = pPropertyFormView->m_MySheet->m_TilePage.m_iDrawID;
			if (iSrcZ < iDestZ)
			{
				for (iSrcZ; iSrcZ <= iDestZ; ++iSrcZ)
					m_pTerrain->TileChange(::_vec3(m_vLastPickTile.x, m_vLastPickTile.y, (float)iSrcZ), L"Texture_Tile", iDrawID, m_bRender);
			}
			else if (iSrcZ > iDestZ)
			{
				for (iSrcZ; iSrcZ >= iDestZ; --iSrcZ)
					m_pTerrain->TileChange(::_vec3(m_vLastPickTile.x, m_vLastPickTile.y, (float)iSrcZ), L"Texture_Tile", iDrawID, m_bRender);
			}
			break;
		}
		// 벽 오브젝트
		case 1:
		{
			bool bHasleftWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_LEFT];
			bool bHasTopWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_TOP];
			bool bHasRighttWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_RIGHT];
			bool bHasBottomWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_BOTTOM];
			bool bHasCeilingWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_CEILING];
			Engine::WALLTYPE eWallType = pPropertyFormView->m_MySheet->m_WallPage.m_eWallType;
			int  iWallY = pPropertyFormView->m_MySheet->m_WallPage.m_iWallY;

			if (bHasleftWall || bHasTopWall || bHasRighttWall || bHasBottomWall)
			{
				iDrawID = pPropertyFormView->m_MySheet->m_WallPage.m_iDrawID;
				if (iSrcZ < iDestZ)
				{
					for (iSrcZ; iSrcZ <= iDestZ; ++iSrcZ)
					{
						m_pTerrain->WallChange(::_vec3(m_vLastPickTile.x, m_vLastPickTile.y, (float)iSrcZ), L"Texture_Wall", iDrawID, m_bRender, bHasleftWall, bHasTopWall, bHasRighttWall, bHasBottomWall, bHasCeilingWall, eWallType, iWallY);
					}
				}
				else if (iSrcZ > iDestZ)
				{
					for (iSrcZ; iSrcZ >= iDestZ; --iSrcZ)
						m_pTerrain->WallChange(::_vec3(m_vLastPickTile.x, m_vLastPickTile.y, (float)iSrcZ), L"Texture_Wall", iDrawID, m_bRender, bHasleftWall, bHasTopWall, bHasRighttWall, bHasBottomWall, bHasCeilingWall, eWallType, iWallY);
				}			
			}
			// 벽 옵션이 전부 꺼져 있으면 벽을 생성하지 않고 리턴
			else
				break;
		}
		}
	}

	// x 방향
	if ((Engine::Get_DIKeyState(DIK_M) & 0x80))
	{
		vPickedTile = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);	
		int iSrcX = (int)m_vLastPickTile.x;
		int iDestX = (int)vPickedTile.x;

		switch (pPropertyFormView->m_MySheet->GetActiveIndex())
		{
			// 타일
		case 0:
		{
			iDrawID = pPropertyFormView->m_MySheet->m_TilePage.m_iDrawID;
			if (iSrcX < iDestX)
			{
				for (iSrcX; iSrcX <= iDestX; ++iSrcX)
					m_pTerrain->TileChange(::_vec3((float)iSrcX, m_vLastPickTile.y, m_vLastPickTile.z), L"Texture_Tile", iDrawID, m_bRender);
			}
			else if (iSrcX > iDestX)
			{
				for (iSrcX; iSrcX >= iDestX; --iSrcX)
					m_pTerrain->TileChange(::_vec3((float)iSrcX, m_vLastPickTile.y, m_vLastPickTile.z), L"Texture_Tile", iDrawID, m_bRender);
			}
		}
		// 벽
		case 1:
		{
			bool bHasleftWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_LEFT];
			bool bHasTopWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_TOP];
			bool bHasRighttWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_RIGHT];
			bool bHasBottomWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_BOTTOM];
			bool bHasCeilingWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_CEILING];
			Engine::WALLTYPE eWallType = pPropertyFormView->m_MySheet->m_WallPage.m_eWallType;
			int  iWallY = pPropertyFormView->m_MySheet->m_WallPage.m_iWallY;

			if (bHasleftWall || bHasTopWall || bHasRighttWall || bHasBottomWall)
			{
				iDrawID = pPropertyFormView->m_MySheet->m_WallPage.m_iDrawID;
				if (iSrcX < iDestX)
				{
					for (iSrcX; iSrcX <= iDestX; ++iSrcX)
						m_pTerrain->WallChange(::_vec3((float)iSrcX, m_vLastPickTile.y, m_vLastPickTile.z), L"Texture_Wall", iDrawID, m_bRender, bHasleftWall, bHasTopWall, bHasRighttWall, bHasBottomWall, bHasCeilingWall, eWallType, iWallY);
				}
				else if (iSrcX > iDestX)
				{
					for (iSrcX; iSrcX >= iDestX; --iSrcX)
						m_pTerrain->WallChange(::_vec3((float)iSrcX, m_vLastPickTile.y, m_vLastPickTile.z), L"Texture_Wall", iDrawID, m_bRender, bHasleftWall, bHasTopWall, bHasRighttWall, bHasBottomWall, bHasCeilingWall, eWallType, iWallY);
				}
			}
			// 벽 옵션이 전부 꺼져 있으면 벽을 생성하지 않고 리턴
			else
				break;
		}
		}
	}
	m_pDynamicCamera->Update_KeyInput(0.25f);

	if (Engine::Get_DIKeyState(DIK_9) & 0x80)
	{
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	if (Engine::Get_DIKeyState(DIK_0) & 0x80)
	{
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	Invalidate(FALSE);
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CPropertyFormView* pPropertyFormView = dynamic_cast<CPropertyFormView*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
	NULL_CHECK(pPropertyFormView);

	// GetActive가 0이면 TilePage가 활성화 된 것 이걸로 페이지를 구분
	const int iTilePage = 0;
	if (pPropertyFormView->m_MySheet->GetActiveIndex() == iTilePage)
	{
		// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
		if (Engine::Get_DIMouseState(Engine::DIM_LB))
			CreateObject(true);
		else if (Engine::Get_DIMouseState(Engine::DIM_RB))
			CreateObject(false);
	}

	Engine::Set_InputDev();

	m_pDynamicCamera->Update_MouseMove(0.1f);

	Invalidate(FALSE);
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);
	m_bRender = true;

	if(!(Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80))
		CreateObject(m_bRender);

	Invalidate(FALSE);
}

void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CView::OnRButtonDown(nFlags, point);
	m_bRender = false;

	if (!(Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80))
		CreateObject(m_bRender);

	Invalidate(FALSE);
}


void CToolView::CreateObject(bool bIsRender)
{
	// 타일의 Draw ID 설정
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CPropertyFormView* pPropertyFormView = dynamic_cast<CPropertyFormView*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
	NULL_CHECK(pPropertyFormView);

	const Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<const Engine::CTerrainTex*>(m_pTerrainGuidLine->Get_Component(L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK_RETURN_VOID(pTerrainBufferCom);

	const Engine::CTransform*	pTerrainTransformCom = dynamic_cast<const Engine::CTransform*>(m_pTerrainGuidLine->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN_VOID(pTerrainTransformCom);

	// 설치할 오브젝트 종류와, 어떤 텍스쳐가 선택되었는지
	int iDrawID = 0;
	::_vec3 vPickedTile = { 0, 0, 0 };
	switch (pPropertyFormView->m_MySheet->GetActiveIndex())
	{
	case 0:
		iDrawID = pPropertyFormView->m_MySheet->m_TilePage.m_iDrawID;
		vPickedTile = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
		m_vLastPickTile = vPickedTile; // 마지막으로 피킹한 타일 위치 저장
		m_pTerrain->TileChange(vPickedTile, L"Texture_Tile", iDrawID, m_bRender);
		break;
	case 1:
	{
		bool bHasleftWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_LEFT];
		bool bHasTopWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_TOP];
		bool bHasRighttWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_RIGHT];
		bool bHasBottomWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_BOTTOM];
		bool bHasCeilingWall = pPropertyFormView->m_MySheet->m_WallPage.m_bIsCheck[WALL_CEILING];
		Engine::WALLTYPE eWallType = pPropertyFormView->m_MySheet->m_WallPage.m_eWallType;
		int  iWallY = pPropertyFormView->m_MySheet->m_WallPage.m_iWallY;

		if (bHasleftWall || bHasTopWall || bHasRighttWall || bHasBottomWall || bHasCeilingWall)
		{
			iDrawID = pPropertyFormView->m_MySheet->m_WallPage.m_iDrawID;
			vPickedTile = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
			m_vLastPickTile = vPickedTile;
			m_pTerrain->WallChange(vPickedTile, L"Texture_Wall", iDrawID, m_bRender, bHasleftWall, bHasTopWall, bHasRighttWall, bHasBottomWall, bHasCeilingWall, eWallType, iWallY);

		}
		// 벽 옵션이 전부 꺼져 있으면 벽을 생성하지 않고 리턴
		else
			break;

		break;
	}
	default:
		break;
	}
}

::_vec3 CToolView::Picking_OnTerrain(HWND hWnd, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransformCom)
{
	POINT			ptMouse{};

	// 윈도우 영역 == 뷰포트 영역
	GetCursorPos(&ptMouse);
	::ScreenToClient(hWnd, &ptMouse);

	// 뷰 포트에서 투영으로
	D3DVIEWPORT9			ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pDeviceClass->GetDevice()->GetViewport(&ViewPort);

	::_vec3	vMousePos;

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = -ptMouse.y / (ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영에서 뷰 스페이스로
	::_matrix		matProj;

	m_pDeviceClass->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj); /// TODO:: GetDevice를 매번 호출하지 말고 한번만 얻어오게 변경
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	::_vec3	vRayPos, vRayDir;

	vRayPos = ::_vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	// 뷰 스페이스에서 월드 스페이스로
	::_matrix		matView;

	m_pDeviceClass->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드에서 로컬로
	::_matrix		matWorld;
  	pTerrainTransformCom->GetWorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	::_ulong	dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	::_ulong	dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();
	::_ulong	dwItv = pTerrainBufferCom->Get_Itv();

	const ::_vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();

	::_ulong			dwVtxIdx[3];
	float				fU, fV, fDist;

	for (::_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (::_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			::_ulong	dwIndex = i * dwVtxCntX + j;

			// 오른쪽 위
			/*dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;*/
			dwVtxIdx[0] = dwIndex + (dwVtxCntX * dwItv);
			dwVtxIdx[1] = dwIndex + (dwVtxCntX * dwItv)+ dwItv;
			dwVtxIdx[2] = dwIndex + dwItv; 

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[0]],
				&vRayPos,
				&vRayDir, &fU, &fV, &fDist))
			{
				return ::_vec3(pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z) * fV);
			}


			// 왼쪽 아래
			/*dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;*/
			dwVtxIdx[0] = dwIndex + (dwVtxCntX * dwItv);
			dwVtxIdx[1] = dwIndex + dwItv;
			dwVtxIdx[2] = dwIndex; 

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[0]],
				&pTerrainVtx[dwVtxIdx[1]],
				&vRayPos,
				&vRayDir, &fU, &fV, &fDist))
			{
				return ::_vec3(pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);
			}
		}

	}

	return ::_vec3(0.f, 0.f, 0.f);
}