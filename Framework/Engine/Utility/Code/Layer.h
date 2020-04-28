#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:	//Get Functions
	const CComponent*	Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID) const;
	const CGameObject*	Get_GameObjcet(const _tchar* pObjTag) const;
	const multimap<const _tchar*, CGameObject*>* Get_LayerMap() const { return &m_mapObject; }
	_uint				Get_ObjectMapSize() const { return m_mapObject.size(); }

public:
	CGameObject*	Get_GameObject_NC(const _tchar* pObjTag);
	void			All_Die();

public:
	HRESULT		Ready_Layer();
	_int		Update_Layer(const _float& fTimeDelta);
	void		Render_Layer();

public:
	HRESULT		Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);

private:
	multimap<const _tchar*, CGameObject*>	m_mapObject;

public:
	static CLayer*	Create();

private:
	virtual void Free();


};

END

#endif // Layer_h__
