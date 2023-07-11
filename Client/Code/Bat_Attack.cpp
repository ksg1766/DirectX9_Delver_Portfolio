#include "..\Header\Bat_Attack.h"
#include "Export_Function.h"
#include "Player.h"

CBat_Attack::CBat_Attack()
{
}

CBat_Attack::CBat_Attack(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CBat_Attack::~CBat_Attack()
{
}

HRESULT CBat_Attack::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fChase = 0.f;
	m_vPrevPos = _vec3(0.f, 0.f, 0.f);
	m_bIsAttack = false;
	m_fSpeed = 13.f;

	return S_OK;
}

STATE CBat_Attack::Update_State(const _float& fTimeDelta)
{

 	CTransform* pPlayerTransform = SceneManager()->
		Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

	_vec3 vPlayerPos = pPlayerTransform->m_vInfo[INFO_POS];
	_vec3& vMonsterPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

	if (!m_bIsAttack)
	{
		m_vPrevPos = vPlayerPos;
		m_bIsAttack = true;
	}

	_vec3 vDir = m_vPrevPos - vMonsterPos;
	D3DXVec3Normalize(&vDir, &vDir);


	_float fAttackDistance = m_fSpeed * fTimeDelta;
	_vec3 vAttackPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + vDir * fAttackDistance;

	vMonsterPos = vAttackPos;

	_float fRange = D3DXVec3LengthSq(&(vMonsterPos - m_vPrevPos));


	if (fRange <= 5.f)	
	{
		m_bIsAttack = false;
		m_pOwner->Set_State(STATE::ROMIMG);
		return STATE::ROMIMG;
	}

	m_pOwner->Set_State(STATE::ATTACK);
	return STATE::ATTACK;
}

void CBat_Attack::LateUpdate_State()
{
}

void CBat_Attack::Render_State()
{
}

CBat_Attack* CBat_Attack::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CBat_Attack* pState = new CBat_Attack(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CBat_Attack*>(pState);

		MSG_BOX("Create Bat Attack Failed");
		return nullptr;
	}

	return pState;
}

void CBat_Attack::Free()
{
	__super::Free();
}
