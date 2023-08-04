#include "stdafx.h"
#include "SoundManager.h"
#include "..\Header\Ghost_Attack.h"
#include "Export_Function.h"
#include "Player.h"
#include "Ghost_Bullet.h"

CGhost_Attack::CGhost_Attack()
{
}

CGhost_Attack::CGhost_Attack(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CGhost_Attack::~CGhost_Attack()
{
}

HRESULT CGhost_Attack::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fChase = 0.f;
	m_iCount = 0;
	m_bIsAttack = false;
	
	
	return S_OK;
}

STATE CGhost_Attack::Update_State(const _float& fTimeDelta)
{
	CPlayer& pPlayer =
		*static_cast<CPlayer*>
		(SceneManager()->GetInstance()->Get_ObjectList
		(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());



	for (_uint i = 0; i < 36; ++i)
	{
		_float fAngle = i * 30;

		_float fSpeed = static_cast<CMonster*>(m_pOwner->Get_Host())->Get_Speed();


		_vec3 vDir = _vec3(cos(D3DXToRadian(fAngle)), 0.f, sinf(D3DXToRadian(fAngle)));
		_vec3 vOffset = vDir * fSpeed;

		CGameObject* pGameObject = nullptr;
		pGameObject = CGhost_Bullet::Create(m_pGraphicDev, m_pOwner->Get_Transform(), fSpeed, vOffset);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	}


	CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SKULL);
	CSoundManager::GetInstance()->PlaySound(L"en_skull_atk_01.mp3", CHANNELID::SOUND_SKULL, 1.f);
	m_bIsAttack = false;
	


	return STATE::ROMIMG;
}

void CGhost_Attack::LateUpdate_State()
{
}

void CGhost_Attack::Render_State()
{
}

CGhost_Attack* CGhost_Attack::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CGhost_Attack* pState = new CGhost_Attack(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CGhost_Attack*>(pState);

		MSG_BOX("Create GhostAttack Failed");
		return nullptr;
	}

	return pState;
}

void CGhost_Attack::Free()
{
	__super::Free();
}
