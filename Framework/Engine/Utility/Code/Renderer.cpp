#include "Renderer.h"
#include "CameraMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

Engine::CRenderer::CRenderer()
{

}

Engine::CRenderer::~CRenderer()
{
	Free();
}

void Engine::CRenderer::Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject)
{
	if (RENDER_END <= eGroup)
		return;

	m_RenderGroup[eGroup].push_back(pGameObject);

	pGameObject->AddRef();
}

void Engine::CRenderer::Render_GameObject()
{
	for (_uint i = 0; i < RENDER_ALPHA; ++i)
	{
		for (auto& pObj : m_RenderGroup[i])
		{
			pObj->Render_GameObject();
			Safe_Release(pObj);
		}

		m_RenderGroup[i].clear();
	}

	CCameraMgr::GetInstance()->SetUp_Alpha();

	for (auto& pObj : m_RenderGroup[RENDER_ALPHA])
	{
		pObj->Render_GameObject();
		Safe_Release(pObj);
	}

	m_RenderGroup[RENDER_ALPHA].clear();

	CCameraMgr::GetInstance()->Finish_Alpha();

	CCameraMgr::GetInstance()->SetUp_RenderUI();

	for (auto& pObj : m_RenderGroup[RENDER_UI])
	{
		pObj->Render_GameObject();
		Safe_Release(pObj);
	}

	m_RenderGroup[RENDER_UI].clear();

	CCameraMgr::GetInstance()->Finish_RenderUI();
}

void Engine::CRenderer::Clear_RenderGroup()
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void Engine::CRenderer::Free(void)
{
	Clear_RenderGroup();
}

