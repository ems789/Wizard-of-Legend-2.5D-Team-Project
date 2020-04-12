#ifndef Management_h__
#define Management_h__

#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:	//	Get Functions
	const CComponent*	Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID) const;
	const CGameObject*	Get_GameObjcet(const _tchar* pLayerTag, const _tchar* pObjTag) const;

	CLayer*	Get_StaticLayer() const { return m_pStaticLayer; }
	
	const CComponent*	Get_Component_From_StaticLayer(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID) const;
	const CGameObject*	Get_GameObject_From_StaticLayer(const _tchar* pObjTag) const;

	const CComponent*	Get_Component_of_Player(const _tchar* pComponentTag, COMPONENTID eID) const;
	const CGameObject*	Get_Player() const;

public:
	HRESULT		SetUp_Scene(CScene* pScene);
	_int		Update_Scene(const _float& fTimeDelta);
	void		Render_Scene();

public:
	HRESULT		Add_GameObjectToStaticLayer(const _tchar* pObjTag, CGameObject* pObj);

private:
	CScene*		m_pScene = nullptr;
	CLayer*		m_pStaticLayer = nullptr;

public:
	virtual void Free(void);
};

END



#endif // Management_h__
