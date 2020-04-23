#ifndef FireStage_h__
#define FireStage_h__

#include "Defines.h"
#include "Scene.h"
#include "UI.h"

class CFireStage : public Engine::CScene
{
private:
	explicit CFireStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireStage();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene() override;

private:
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag) { return S_OK; }
	HRESULT		Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_Monster_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_Effect_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_UI_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_StaticLayer();
	HRESULT		Ready_Camera();
	HRESULT		UI_Setting();

private:
	void		FirstMonsterGen(const _vec3* pPlayerPos);
	void		SecondMonsterGen(const _vec3* pPlayerPos);
	void		ThirdMonsterGen(const _vec3* pPlayerPos);
	void		FourthMonsterGen(const _vec3* pPlayerPos);
	void		FifthMonsterGen(const _vec3* pPlayerPos);

private:
	_bool		m_b1stMonsterGen = false;
	_bool		m_b2ndMonsterGen = false;
	_bool		m_b3rdMonsterGen = false;
	_bool		m_b4thMonsterGen = false;
	_bool		m_b5thMonsterGen = false;


public:
	static CFireStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

#endif // FireStage_h__
