#include "stdafx.h"
#include "Logo.h"

#include "Export_Function.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{

}

CLogo::~CLogo(void)
{

}

HRESULT CLogo::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev, RESOURCE_END), E_FAIL);
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);

	/*D3DXMatrixLookAtLH(&m_matView, &_vec3(0.f, 0.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f));
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(60.f), _float(WINCX) / WINCY, 1.f, 1000.f);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);*/

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);

	return S_OK;
}

void CLogo::Render_Scene(void)
{
	// DEBUG 용

}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLogo::Free(void)
{
	Engine::CScene::Free();
}

HRESULT CLogo::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	// BackGround
	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	// TestPlayer
	//pGameObject = CTestPlayer::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestPlayer", pGameObject), E_FAIL);

	//// TestMonster
	//pGameObject = CTestMonster::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestMonster", pGameObject), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;


//	pGameObject = CBackGround::Create(m_pGraphicDev);
//	if (nullptr == pGameObject)
//		goto except;
//
//	if (FAILED(pLayer->Add_GameObject(L"BackGround", pGameObject)))
//		goto except;
//
//	m_mapLayer.emplace(pLayerTag, pLayer);
//	return S_OK;
//
//except:
//	Engine::Safe_Release(pLayer);
//	return E_FAIL;
}

HRESULT CLogo::Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax)
{
	// 메모리 예약
	Engine::Reserve_ContainerSize(eMax);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_TriCol",
		Engine::BUFFER_TRICOL),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcCol",
		Engine::BUFFER_RCCOL),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcTex",
		Engine::BUFFER_RCTEX),
		E_FAIL);

	// 텍스쳐
	/*FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
											RESOURCE_LOGO,
											L"Texture_Logo",
											Engine::TEX_NORMAL,
											L"../Bin/Resource/Texture/Logo/Logo.jpg"),
											E_FAIL);*/

	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
											RESOURCE_LOGO,
											L"Texture_Logo",
											Engine::TEX_NORMAL,
											L"../Bin/Resource/Texture/LogoBack/LogoBack_%d.png", 38),
											E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
											RESOURCE_LOGO,
											L"Texture_Player", 
											Engine::TEX_NORMAL, 
											L"../Bin/Resource/Texture/Player/Ma.jpg"), 
											E_FAIL);

	return S_OK;
}
