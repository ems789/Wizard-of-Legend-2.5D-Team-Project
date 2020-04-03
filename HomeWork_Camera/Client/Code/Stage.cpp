#include "stdafx.h"
#include "Stage.h"

#include "Export_Function.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{

}

CStage::~CStage(void)
{

}

HRESULT CStage::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev, RESOURCE_END), E_FAIL);
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);

	D3DXMatrixLookAtLH(&m_matView, &_vec3(0.f, 5.f, -5.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(60.f), _float(WINCX) / WINCY, 1.f, 1000.f);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pCamera = CCamera::Create(m_pGraphicDev);

	return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);

	m_pCamera->Update_GameObject(fTimeDelta);

	return S_OK;
}

void CStage::Render_Scene(void)
{
	
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage*	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStage::Free(void)
{
	Engine::Safe_Release(m_pCamera);
	Engine::CScene::Free();
}

HRESULT CStage::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	// Player
	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);
	
	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax)
{
	// 메모리 예약
	Engine::Reserve_ContainerSize(eMax);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
											RESOURCE_STATIC,
											L"Buffer_RcTex",
											Engine::BUFFER_RCTEX),
											E_FAIL);
		
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
											RESOURCE_STATIC,
											L"Buffer_TerrainTex",
											Engine::BUFFER_TERRAINTEX,
											VTXCNTX,
											VTXCNTZ,
											VTXITV),
											E_FAIL);

	// 텍스쳐

	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
											RESOURCE_STAGE,
											L"Texture_Terrain",
											Engine::TEX_NORMAL,
											L"../Bin/Resource/Texture/Terrain/Terrain%d.png"),
											E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
											RESOURCE_STAGE,
											L"Texture_Player", 
											Engine::TEX_NORMAL, 
											L"../Bin/Resource/Texture/Player%d.png"), 
											E_FAIL);

	return S_OK;
}
