
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
	m_pGraphicDev(Engine::CGraphicDev::GetInstance()),
	m_pTerrainGuidLine(nullptr),
	m_pDynamicCamera(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
	Engine::Safe_Release(m_pGraphicDev);

	Engine::Release_Resources();
	Engine::CInputDev::GetInstance()->DestroyInstance();
	//CGraphicDev::GetInstance()->DestroyInstance();
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
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	m_pGraphicDev->Render_Begin(D3DCOLOR_ARGB(255, 153, 217, 234));

	m_pTerrainGuidLine->Render_GameObject();
	m_pTerrain->Update_GameObject(1.f);
	m_pTerrain->Render_GameObject();

	m_pGraphicDev->Render_End();
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
		&m_pGraphicDev));
	m_pGraphicDev->AddRef();

	LPDIRECT3DDEVICE9 pGraphicDev = m_pGraphicDev->GetDevice();
	pGraphicDev->AddRef();
	FAILED_CHECK_VOID(pGraphicDev);

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 메모리 예약
	Engine::Reserve_ContainerSize(::RESOURCE_END);

	CPropertyFormView* pPropertyFormView = dynamic_cast<CPropertyFormView*>(GetParentSplitter(this, FALSE)->GetPane(0, 0));

	// 버퍼 초기화
	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TerrainTex",
		Engine::BUFFER_TERRAINTEX,
		L"",
		pPropertyFormView->m_iTerrainX + 1, // 정점의 개수이므로 1을 더함
		pPropertyFormView->m_iTerrainZ + 1,
		pPropertyFormView->m_iTerrainItv),
		E_FAIL);

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TileTex",
		Engine::BUFFER_TILETEX,
		L""),
		E_FAIL);

	// 텍스쳐 로드
	/*FAILED_CHECK_RETURN_VOID(Engine::Ready_Texture(pGraphicDev,
		::RESOURCE_STAGE,
		L"Texture_Terrain",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/Texture/Terrain/Grass_%d.tga", 2),
		E_FAIL);*/

	// 타일
	FAILED_CHECK_RETURN_VOID(Engine::Ready_Texture(pGraphicDev,
		::RESOURCE_STAGE,
		L"Texture_Tile",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/Texture/Tile/ForestTile/ForestTile%d.png", 321),
		E_FAIL);



	m_pTerrainGuidLine = CTerrainGuidLine::Create(pGraphicDev);
	NULL_CHECK_MSG(m_pTerrainGuidLine, L"TerrainGuidLine Create Failed");

	// 터레인 만들기(현재 지정되어 있는 x * y 크기만큼)
	m_pTerrain = CTerrain::Create(pGraphicDev, pPropertyFormView->m_iTerrainX, 
												pPropertyFormView->m_iTerrainZ,
												pPropertyFormView->m_iTerrainItv);
	NULL_CHECK_MSG(m_pTerrain, L"Terrain Create Failed");


	// dynamicCamera
	m_pDynamicCamera = CDynamicCamera::Create(pGraphicDev,
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

	pGraphicDev->Release();
}

void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	Engine::Set_InputDev();

	m_pDynamicCamera->Update_KeyInput(0.25f);

	Invalidate(FALSE);
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (Engine::Get_DIMouseState(Engine::DIM_LB))
	{
		const Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<const Engine::CTerrainTex*>(m_pTerrainGuidLine->Get_Component(L"Com_Buffer", Engine::ID_STATIC));
		NULL_CHECK_RETURN_VOID(pTerrainBufferCom);

		const Engine::CTransform*	pTerrainTransformCom = dynamic_cast<const Engine::CTransform*>(m_pTerrainGuidLine->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN_VOID(pTerrainTransformCom);

		::_vec3 vPickedTile = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
		/// TODO :: 바꿀 텍스쳐의 태그를 넣어야함
		m_pTerrain->TileChange(vPickedTile, L"Texture_Tile", true);
	}
	else if (Engine::Get_DIMouseState(Engine::DIM_RB))
	{
		const Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<const Engine::CTerrainTex*>(m_pTerrainGuidLine->Get_Component(L"Com_Buffer", Engine::ID_STATIC));
		NULL_CHECK_RETURN_VOID(pTerrainBufferCom);

		const Engine::CTransform*	pTerrainTransformCom = dynamic_cast<const Engine::CTransform*>(m_pTerrainGuidLine->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN_VOID(pTerrainTransformCom);

		::_vec3 vPickedTile = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
		/// TODO :: 바꿀 텍스쳐의 태그를 넣어야함
		m_pTerrain->TileChange(vPickedTile, L"Texture_Tile", false);
	}

	Engine::Set_InputDev();

	m_pDynamicCamera->Update_MouseMove(0.1f);

	Invalidate(FALSE);
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);

	const Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<const Engine::CTerrainTex*>(m_pTerrainGuidLine->Get_Component(L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK_RETURN_VOID(pTerrainBufferCom);

	const Engine::CTransform*	pTerrainTransformCom = dynamic_cast<const Engine::CTransform*>(m_pTerrainGuidLine->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN_VOID(pTerrainTransformCom);

	::_vec3 vPickedTile = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
	/// TODO :: 바꿀 텍스쳐의 태그를 넣어야함
	m_pTerrain->TileChange(vPickedTile, L"Test", true);

	Invalidate(FALSE);
}

void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CView::OnRButtonDown(nFlags, point);

	const Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<const Engine::CTerrainTex*>(m_pTerrainGuidLine->Get_Component(L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK_RETURN_VOID(pTerrainBufferCom);

	const Engine::CTransform*	pTerrainTransformCom = dynamic_cast<const Engine::CTransform*>(m_pTerrainGuidLine->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN_VOID(pTerrainTransformCom);

	::_vec3 vPickedTilePos = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
	/// TODO :: 바꿀 텍스쳐의 태그를 넣어야함
	m_pTerrain->TileChange(vPickedTilePos, L"Test", false);

	Invalidate(FALSE);
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
	m_pGraphicDev->GetDevice()->GetViewport(&ViewPort);

	::_vec3	vMousePos;

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = -ptMouse.y / (ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영에서 뷰 스페이스로
	::_matrix		matProj;

	m_pGraphicDev->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj); /// TODO:: GetDevice를 매번 호출하지 말고 한번만 얻어오게 변경
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	::_vec3	vRayPos, vRayDir;

	vRayPos = ::_vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	// 뷰 스페이스에서 월드 스페이스로
	::_matrix		matView;

	m_pGraphicDev->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
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

	const ::_vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();

	::_ulong			dwVtxIdx[3];
	float				fU, fV, fDist;

	for (::_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (::_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			::_ulong	dwIndex = i * dwVtxCntX + j;

			// 오른쪽 위
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

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
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
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