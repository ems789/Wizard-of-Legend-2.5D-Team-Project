#ifndef LightningStage_h__
#define LightningStage_h__

#include "Defines.h"
#include "Scene.h"
#include "UI.h"

class CRoomBlock;
class CLightningStage : public Engine::CScene
{
	enum	ROOM_STATE { ROOM_IDLE, ROOM_CLOSE, ROOM_OPEN, ROOM_END };
	enum	ROOM_NUMBER { ROOM_NUM_1, ROOM_NUM_2, ROOM_NUM_3, ROOM_NUM_4, ROOM_NUM_5, ROOM_NUM_6, ROOM_NUM_7, ROOM_NUM_8, ROOM_NUM_9, ROOM_NUM_END };
	enum	ROOM_PHASE { RP_1, RP_2, RP_3, RP_4, RP_5, RP_6, RP_7, RP_8, RP_9, RP_END };

private:
	explicit CLightningStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightningStage();

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
	void		SixMonsterGen(const _vec3* pPlayerPos);

private:
	_int		Room_State_Update(const _float& fTimeDelta);
	_int		Room_Idle_Update(const _float& fTimeDelta);
	_int		Room_Close_Update(const _float& fTimeDelta);

private:
	_int		FirstRoom_Update(const _float& fTimeDelta);
	_int		SecondRoom_Update(const _float& fTimeDelta);
	_int		ThirdRoom_Update(const _float& fTimeDelta);
	_int		FifthRoom_Update(const _float& fTimeDelta);
	_int		SixRoom_Update(const _float& fTimeDelta);

private:
	_bool		Check_Monster();
	void		RoomBlock_Close();
	void		RoomBlock_Open();
	HRESULT		Store_Setting();

private:
	ROOM_STATE			m_eCurState = ROOM_IDLE;
	ROOM_NUMBER			m_eRoomNumber = ROOM_NUM_END;
	ROOM_PHASE			m_eRoomPhase = RP_END;
	list<CRoomBlock*>	m_RoomBlockList;

	_bool		m_b1stMonsterGen = false;
	_bool		m_b2ndMonsterGen = false;
	_bool		m_b3rdMonsterGen = false;
	_bool		m_b4thMonsterGen = false;
	_bool		m_b5thMonsterGen = false;
	_bool		m_b6thMonsterGen = false;

	_bool		m_bMonsterGen = false;
	
	_bool		m_bSceneChange = false;
	_float		m_fSceneChangeTime = 0.f;


public:
	static CLightningStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

#endif // LightningStage_h__
