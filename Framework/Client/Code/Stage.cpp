#include "stdafx.h"
#include "Stage.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CStage::~CStage()
{

}

HRESULT CStage::Ready_Scene()
{

	return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
	return 0;
}

void CStage::Render_Scene()
{
	
}

void CStage::Free()
{
	CScene::Free();
}

