#ifndef Scene_h__
#define Scene_h__

#include "Layer.h"

BEGIN(Engine)

class CCollisionMgr;
class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	const CComponent*	Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID) const;
	const CGameObject*	Get_GameObjcet(const _tchar* pLayerTag, const _tchar* pObjTag) const;
	const CLayer*		Get_Layer(const _tchar* pLayerTag) const;

public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene();

public:
	HRESULT		Add_Layer(const _tchar* pLayerTag, CLayer* pLayer);
	HRESULT		Add_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag, CGameObject* pObj);

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;
	map<const _tchar*, CLayer*> m_mapLayer;
	CCollisionMgr*				m_pCollisionMgr = nullptr;
	
protected:
	virtual void Free();
};

END

#endif // Scene_h__
