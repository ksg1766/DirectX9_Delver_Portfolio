#include "stdafx.h"
#include "SoundManager.h"
#include "..\Header\Wizard_Attack.h"
#include "Export_Function.h"
#include "Player.h"
#include "Magic_Ball.h"
#include "Wizard.h"

CWizard_Attack::CWizard_Attack()
{
}

CWizard_Attack::CWizard_Attack(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CWizard_Attack::~CWizard_Attack()
{
}

HRESULT CWizard_Attack::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fChase = 0.f;
	m_vPrevPos = _vec3(0.f, 0.f, 0.f);
	m_vRandomPos = _vec3(0.f, 0.f, 0.f);

	m_bIsAttack = false;
	m_bAttackTick = false;


	return S_OK;
}

STATE CWizard_Attack::Update_State(const _float& fTimeDelta)
{


	// 어떤 객체를 소환.
	// 본인 위치값 -> 아마도 오브젝트 생성할 위치겠지
	CPlayer& pPlayer =
		*dynamic_cast<CPlayer*>
		(SceneManager()->GetInstance()->Get_ObjectList
		(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());


	// 4번 이미지에서 5번 이미지 넘어갈 때 생성하고 쏴야함.
	if(!m_bIsAttack)
		if (m_pOwner->Get_Animator()->Get_Animation()->Get_Frame() > 5.f)
		{

			Attack_Sound();

			_float fSpeed = dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_Speed();

			m_pOwner->Get_Animator()->Get_Animation()->Set_Frame(0.f);

			CGameObject* pGameObject = nullptr;
			pGameObject = CMagic_Ball::Create(m_pGraphicDev, m_pOwner->Get_Transform(), fSpeed);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

			m_bIsAttack = true;
			return STATE::ROMIMG;
		}

	m_bIsAttack = false;

	dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_State(STATE::ATTACK);
}

void CWizard_Attack::LateUpdate_State()
{
}

void CWizard_Attack::Render_State()
{
}

void CWizard_Attack::Attack_Sound()
{

	MONSTERTAG _eMonsterTag = dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_MonsterTag();

	switch (_eMonsterTag)
	{
	case MONSTERTAG::WIZARD:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WIZARD);
		CSoundManager::GetInstance()->PlaySound(L"en_mage_attack_01.mp3", CHANNELID::SOUND_WIZARD, 1.f);
		break;
	case MONSTERTAG::ALIEN:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_ALIEN);
		CSoundManager::GetInstance()->PlaySound(L"en_beholder_attack_01.mp3", CHANNELID::SOUND_ALIEN, 1.f);
		break;
	}
}

CWizard_Attack* CWizard_Attack::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CWizard_Attack* pState = new CWizard_Attack(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CWizard_Attack*>(pState);

		MSG_BOX("Create Wizard Attack Failed");
		return nullptr;
	}

	return pState;
}

void CWizard_Attack::Free()
{
	__super::Free();
}
