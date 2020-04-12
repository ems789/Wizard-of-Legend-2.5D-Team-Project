#include "stdafx.h"
#include "Logo.h"
#include "LoadingScene.h"

#include "Export_Function.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev, RESOURCE_END), E_FAIL);
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_UI_Layer(L"UI"), E_FAIL);
	

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = CScene::Update_Scene(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	//	Temporary
	if (Engine::KeyDown(DIK_RETURN))
	{
		Engine::CScene* pScene = CLoadingScene::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pScene, -1);

		FAILED_CHECK_RETURN(Engine::SetUp_Scene(pScene), -1);

		return 1;
	}

	return iExit;
}

void CLogo::Render_Scene()
{

}

HRESULT CLogo::Ready_GameLogic_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*	pGameObject = nullptr;

	//	BackGround
	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN_MSG(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL, L"Failed to add BackGround");

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CLogo::Ready_UI_Layer(const _tchar* pLayerTag)
{

	return S_OK;
}

HRESULT CLogo::Ready_Resource(LPDIRECT3DDEVICE9 & pGraphicDev, RESOURCEID eMax)
{
	Engine::Reserve_ContainerSize(eMax);

	//	Buffer
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev, RESOURCE_STATIC, L"Buffer_RcCol", Engine::BUFFER_RCCOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev, RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev, RESOURCE_STATIC, L"Buffer_TriCol", Engine::BUFFER_TRICOL), E_FAIL);

	//	Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_LOGO, L"Texture_Logo", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/BG/Background%d.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_STATIC, L"Texture_CastingCircle", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Effect/CastingCircle/CastingCircle_%d.png", 27), E_FAIL);

	return S_OK;
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo* pInst = new CLogo(pGraphicDev);

	if (FAILED(pInst->Ready_Scene()))
		Engine::Safe_Release(pInst);

	return pInst;
}


void CLogo::Free()
{
	CScene::Free();
}

