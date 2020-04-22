
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
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

// CToolView ����/�Ҹ�

CToolView::CToolView() :
	m_pDeviceClass(Engine::CGraphicDev::GetInstance()),
	m_pTerrainGuidLine(nullptr),
	m_pDynamicCamera(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
#ifdef _DEBUG
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
#endif // _DEBUG

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	m_pDeviceClass->Render_Begin(D3DCOLOR_ARGB(255, 153, 217, 234));

	m_pTerrainGuidLine->Render_GameObject();
	m_pTerrain->Update_GameObject(1.f);
	m_pTerrain->Render_GameObject();

	m_pDeviceClass->Render_End();
}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();

	// View ũ�⸦ �����ϱ� ���ؼ��� ������ �������� ũ�⸦ �ǵ帰��.
	// ������ �������� ������ ������ ���.
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK_MSG(pFrameWnd, L"pFrameWnd is null");

	RECT rcFrame = {};

	// �������� ũ��(�׵θ� + Ŭ���̾�Ʈ ����) ������ �Լ�.(��ũ�� ��ǥ ����)
	pFrameWnd->GetWindowRect(&rcFrame);
	::SetRect(&rcFrame, 0, 0, rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);

	RECT rcView = {};

	// �������� Ŭ���̾�Ʈ ����(�׵θ��� �������� ����) ũ�� ������ �Լ�. (Ŭ���̾�Ʈ ��ǥ ����)
	GetClientRect(&rcView);

	// ���� ������������� View ������ ������ ���Ѵ�.
	int iGapX = rcFrame.right - rcView.right;
	int iGapY = rcFrame.bottom - rcView.bottom;

	// �������������� ũ�⸦ ���� ����.
	pFrameWnd->SetWindowPos(nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER | SWP_NOMOVE);


	// ��ġ �ʱ�ȭ
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

	// �޸� ����
	Engine::Reserve_ContainerSize(::RESOURCE_END);

	CPropertyFormView* pPropertyFormView = dynamic_cast<CPropertyFormView*>(GetParentSplitter(this, FALSE)->GetPane(0, 0));

	// ���� �ʱ�ȭ
	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(m_pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TerrainTex",
		Engine::BUFFER_TERRAINTEX, 
		Engine::WALL_OUTER,
		D3DXCOLOR(1.f, 1.f, 1.f, 1.f),
		L"",
		pPropertyFormView->m_iTerrainX + 1, // ������ �����̹Ƿ� 1�� ����
		pPropertyFormView->m_iTerrainZ + 1,
		pPropertyFormView->m_iTerrainItv),
		E_FAIL);

	FAILED_CHECK_RETURN_VOID(Engine::Ready_Buffer(m_pGraphicDev,
		::RESOURCE_STATIC,
		L"Buffer_TileTex",
		Engine::BUFFER_TILETEX),
		E_FAIL);

	// �ð� ���� �� �ؽ���
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

	// �ݽð� ���� �� �ؽ���
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

	// Ÿ�� �ؽ��� �ε�
	//FAILED_CHECK_RETURN_VOID(Engine::Ready_Texture(m_pGraphicDev, ::RESOURCE_STAGE, L"Texture_Tile", Engine::TEX_NORMAL, L"../../Client/Bin/Resource/Texture/ForestStage/ForestStageTile/ForestTile%d.png", 321), E_FAIL);
	FAILED_CHECK_RETURN_VOID(Engine::Ready_Texture(m_pGraphicDev, ::RESOURCE_STAGE, L"Texture_Tile", Engine::TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Tile/Tile%d.png", 449), E_FAIL);

	// �� �ؽ��� �ε�
	FAILED_CHECK_RETURN_VOID(Engine::Ready_Texture(m_pGraphicDev, ::RESOURCE_STAGE, L"Texture_Wall", Engine::TEX_NORMAL, L"../../Client/Bin/Resource/Texture/Wall/Wall%d.png", 267), E_FAIL);

	m_pTerrainGuidLine = CTerrainGuidLine::Create(m_pGraphicDev);
	NULL_CHECK_MSG(m_pTerrainGuidLine, L"TerrainGuidLine Create Failed");

	// �ͷ��� �����(���� �����Ǿ� �ִ� x * y ũ�⸸ŭ)
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

	// Ű���� �Է����� üũ �ڽ� üũ
	if (pPropertyFormView->m_MySheet->GetActiveIndex() == 1) // �� �϶��� �۵�
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


	// ������Ʈ �Ϸķ� �ϰ� ����
	const Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<const Engine::CTerrainTex*>(m_pTerrainGuidLine->Get_Component(L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK_RETURN_VOID(pTerrainBufferCom);

	const Engine::CTransform*	pTerrainTransformCom = dynamic_cast<const Engine::CTransform*>(m_pTerrainGuidLine->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN_VOID(pTerrainTransformCom);

	::_vec3 vPickedTile = { 0, 0, 0 };
	int iDrawID = 0;
	// z ����
	if ((Engine::Get_DIKeyState(DIK_N) & 0x80))
	{		
		vPickedTile = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
		int iSrcZ = (int)m_vLastPickTile.z;
		int iDestZ = (int)vPickedTile.z;
		
		switch (pPropertyFormView->m_MySheet->GetActiveIndex())
		{
		// Ÿ��
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
		// �� ������Ʈ
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
			// �� �ɼ��� ���� ���� ������ ���� �������� �ʰ� ����
			else
				break;
		}
		}
	}

	// x ����
	if ((Engine::Get_DIKeyState(DIK_M) & 0x80))
	{
		vPickedTile = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);	
		int iSrcX = (int)m_vLastPickTile.x;
		int iDestX = (int)vPickedTile.x;

		switch (pPropertyFormView->m_MySheet->GetActiveIndex())
		{
			// Ÿ��
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
		// ��
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
			// �� �ɼ��� ���� ���� ������ ���� �������� �ʰ� ����
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

	// GetActive�� 0�̸� TilePage�� Ȱ��ȭ �� �� �̰ɷ� �������� ����
	const int iTilePage = 0;
	if (pPropertyFormView->m_MySheet->GetActiveIndex() == iTilePage)
	{
		// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// Ÿ���� Draw ID ����
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CPropertyFormView* pPropertyFormView = dynamic_cast<CPropertyFormView*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
	NULL_CHECK(pPropertyFormView);

	const Engine::CTerrainTex* pTerrainBufferCom = dynamic_cast<const Engine::CTerrainTex*>(m_pTerrainGuidLine->Get_Component(L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK_RETURN_VOID(pTerrainBufferCom);

	const Engine::CTransform*	pTerrainTransformCom = dynamic_cast<const Engine::CTransform*>(m_pTerrainGuidLine->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN_VOID(pTerrainTransformCom);

	// ��ġ�� ������Ʈ ������, � �ؽ��İ� ���õǾ�����
	int iDrawID = 0;
	::_vec3 vPickedTile = { 0, 0, 0 };
	switch (pPropertyFormView->m_MySheet->GetActiveIndex())
	{
	case 0:
		iDrawID = pPropertyFormView->m_MySheet->m_TilePage.m_iDrawID;
		vPickedTile = Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
		m_vLastPickTile = vPickedTile; // ���������� ��ŷ�� Ÿ�� ��ġ ����
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
		// �� �ɼ��� ���� ���� ������ ���� �������� �ʰ� ����
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

	// ������ ���� == ����Ʈ ����
	GetCursorPos(&ptMouse);
	::ScreenToClient(hWnd, &ptMouse);

	// �� ��Ʈ���� ��������
	D3DVIEWPORT9			ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pDeviceClass->GetDevice()->GetViewport(&ViewPort);

	::_vec3	vMousePos;

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = -ptMouse.y / (ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// �������� �� �����̽���
	::_matrix		matProj;

	m_pDeviceClass->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj); /// TODO:: GetDevice�� �Ź� ȣ������ ���� �ѹ��� ������ ����
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	::_vec3	vRayPos, vRayDir;

	vRayPos = ::_vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	// �� �����̽����� ���� �����̽���
	::_matrix		matView;

	m_pDeviceClass->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// ���忡�� ���÷�
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

			// ������ ��
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


			// ���� �Ʒ�
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