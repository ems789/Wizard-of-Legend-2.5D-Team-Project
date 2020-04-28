#ifndef FireStage_h__
#define FireStage_h__

#include "Defines.h"
#include "Scene.h"
#include "UI.h"

class CRoomBlock;
class CBlobRoller;
class CScaffold;
class CFireStage : public Engine::CScene
{
	enum	ROOM_STATE	{ ROOM_IDLE, ROOM_CLOSE, ROOM_OPEN, ROOM_END };
	enum	ROOM_NUMBER { ROOM_NUM_1, ROOM_NUM_2, ROOM_NUM_3, ROOM_NUM_4, ROOM_NUM_5, ROOM_NUM_6, ROOM_NUM_7, ROOM_NUM_8, ROOM_NUM_9, ROOM_NUM_END };
	enum	ROOM_PHASE	{ RP_1, RP_2, RP_3, RP_4, RP_5, RP_6, RP_7, RP_8, RP_9, RP_END };

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
	void		SixthRoomStart(const _vec3* pPlayerPos);
	void		SeventhRoomStart(const _vec3* pPlayerPos);
	void		EighthRoomStart(const _vec3* pPlayerPos);
	void		EngindRoomStart();


private:
	_int		Room_State_Update(const _float& fTimeDelta);
	_int		Room_Idle_Update(const _float& fTimeDelta);
	_int		Room_Close_Update(const _float& fTimeDelta);

private:
	_int		FirstRoom_Update(const _float& fTimeDelta);
	_int		SecondRoom_Update(const _float& fTimeDelta);
	_int		ThirdRoom_Update(const _float& fTimeDelta);

private:
	//	폭탄 피하기		-> 완료
	_int		FourthRoom_Update(const _float& fTimeDelta);
	_int		FourthRoom_Gen(const _float& fTimeDelta);
	_int		FourthRoom_Go(const _float& fTimeDelta);
	_int		FourthRoom_Gen2(const _float& fTimeDelta);
	_int		FourthRoom_Go2(const _float& fTimeDelta);

	//	상자 찾기		-> 완료
	_int		FifthRoom_Update(const _float& fTimeDelta);

	//	장판 피하기	->	완료
	_int		SixthRoom_Update(const _float& fTimeDelta);

	//	발판 누르기	->	완료
	_int		SeventhRoom_Update(const _float& fTimeDelta);

	//	보스방
	_int		EighthRoom_Update(const _float& fTimeDelta);

	//	 다음 방
	_int		NinthRoom_Update(const _float& fTimeDelta);


private:
	_bool		Check_Monster();
	void		RoomBlock_Close();
	void		RoomBlock_Open();
	HRESULT		Store_Setting();

private:
	_bool		m_b1stMonsterGen = false;
	_bool		m_b2ndMonsterGen = false;
	_bool		m_b3rdMonsterGen = false;
	_bool		m_b4thMonsterGen = false;
	_bool		m_b5thMonsterGen = false;
	_bool		m_b6thRoomStart = false;
	_bool		m_b7thRoomStart = false;
	_bool		m_b8thRoomStart = false;
	_bool		m_b9thRoomStart = false;

	_bool		m_bMonsterGen = false;

	ROOM_STATE			m_eCurState		= ROOM_IDLE;
	ROOM_NUMBER			m_eRoomNumber	= ROOM_NUM_END;
	ROOM_PHASE			m_eRoomPhase	= RP_END;
	list<CRoomBlock*>	m_RoomBlockList;

	_vec3		m_vRoomPos = {};
	_float		m_fRoomMinX = 0.f;
	_float		m_fRoomMaxX = 0.f;
	_float		m_fRoomMinZ = 0.f;
	_float		m_fRoomMaxZ = 0.f;

	_float		m_fTimer = 0.f;
	_float		m_fTimer2 = 0.f;
	
	_float		m_f4RoomX = 0.f;
	_float		m_f4RoomZ = 0.f;

	_vec3		m_v4RoomPos = {};
	_float 		m_f4RoomRadius = 0.f;
	_uint		m_ui4RoomPattern = 0;

	_bool				m_bR4Go = false;
	list<CBlobRoller*>	m_BlobRollerList;

	vector<CScaffold*>	m_vecScaffold;
	_bool				m_bScaffoldAnswer[16];

public:
	static CFireStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

#endif // FireStage_h__
