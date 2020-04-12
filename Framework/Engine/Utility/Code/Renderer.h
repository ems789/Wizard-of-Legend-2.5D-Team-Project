#ifndef Renderer_h__
#define Renderer_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer();
	virtual ~CRenderer();

public:
	void Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject);
	void Render_GameObject();
	void Clear_RenderGroup();

private:
	list<CGameObject*>	m_RenderGroup[RENDER_END];

private:
	virtual void Free(void);
};

END

#endif // Renderer_h__
