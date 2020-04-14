#include "Skill.h"

#include "Export_Function.h"

USING(Engine)

Engine::CSkill::CSkill(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CSkill::~CSkill()
{

}

void Engine::CSkill::Free()
{
	Safe_Release(m_pGraphicDev);
}
