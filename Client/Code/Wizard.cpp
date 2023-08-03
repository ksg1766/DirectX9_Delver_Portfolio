#include "stdafx.h"
#include "..\Header\Wizard.h"
#include "Export_Function.h"

#include "Monster_Move.h"
#include "Wizard_Attack.h"
#include "Monster_Hit.h"
#include "Monster_Dead.h"
#include "EffectBlood.h"
#include "Player.h"
#include "SoundManager.h"
#include "PoolManager.h"

CWizard::CWizard(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CWizard::CWizard(const CWizard& rhs)
	: CMonster(rhs)
{
}

CWizard::~CWizard()
{
}

HRESULT CWizard::Ready_Object()
{
	Set_ObjectTag(OBJECTTAG::MONSTER);
	Set_MonsterState(MONSTERTAG::WIZARD);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	m_pTransform->Translate(_vec3(1.f, 3.f, 20.f));

	CState* pState = CMonster_Move::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ROMIMG, pState);
	pState = CWizard_Attack::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ATTACK, pState);
	pState = CMonster_Hit::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::HIT, pState);
	pState = CMonster_Dead::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::DEAD, pState);
	

	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ROMIMG], STATE::ROMIMG, 5.f, TRUE);
	m_pAnimator->Add_Animation(STATE::ROMIMG, pAnimation);
	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ATTACK], STATE::ATTACK, 5.f, TRUE);
	m_pAnimator->Add_Animation(STATE::ATTACK, pAnimation);
	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::HIT], STATE::HIT, 5.f, TRUE);
	m_pAnimator->Add_Animation(STATE::HIT, pAnimation);
	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::DEAD], STATE::DEAD, 3.f, TRUE);
	m_pAnimator->Add_Animation(STATE::DEAD, pAnimation);
	
	m_pStateMachine->Set_Animator(m_pAnimator);
	m_pStateMachine->Set_State(STATE::ROMIMG);
	
	Set_Speed(20.f);

#pragma region WizardStat

	Init_Stat();

#pragma endregion

	return S_OK;
}

_int CWizard::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);


	if (IsKnockBack())
	{
		m_pStateMachine->Set_State(STATE::HIT);
		Set_KnockBack(false);
	}


	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	_float fDistance = D3DXVec3Length(&(rPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS]));

	if (fDistance < 15.f)
	{
		if (!m_bSearch)
		{
			m_bSearch = true;
			CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WIZARD);
			CSoundManager::GetInstance()->PlaySound(L"en_mage_alert_03.mp3", CHANNELID::SOUND_WIZARD, 1.f);
		}
	}


	if (m_pBasicStat->Get_Stat()->fHP <= 0)
	{
			m_pStateMachine->Set_State(STATE::DEAD);

		if (m_pAnimator->Get_Animation()->Get_Frame() >= 1)
			m_pAnimator->Get_Animation()->Set_Loop(FALSE);
		{

			//////////////////////////////////////////////////////////////////////////////// ÀÌÆåÆ® 
			if (m_bDieEffect)
			{
				if (!m_bDeadCheck)
				{
					CGameObject* pGameObject = CEffectBlood::Create(m_pGraphicDev);
					pGameObject->m_pTransform->Translate(_vec3(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y - .95f, m_pTransform->m_vInfo[INFO_POS].z));
					dynamic_cast<CTempEffect*>(pGameObject)->Set_EffectColor(ECOLOR_RED);
					Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

					m_bDeadCheck = true;
				}
				//m_bDieEffect = false;
			}
			//////////////////////////////////////////////////////////////////////////////// ÀÌÆåÆ® 
			
			m_fDeadCoolTime += fTimeDelta;

			if (m_fDeadCoolTime > 3.f)
			CPoolManager::GetInstance()->Delete_Object(this);
		}
	}

	m_pStateMachine->Update_StateMachine(fTimeDelta);
	//ForceHeight(m_pTransform->m_vInfo[INFO_POS]);

	return iExit;
}

void CWizard::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();

	m_pStateMachine->LateUpdate_StateMachine();
	//__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);


}

void CWizard::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();
}

void CWizard::Init_Stat()
{
	m_pBasicStat->Get_Stat()->fSpeed = 4.f;
	m_pBasicStat->Get_Stat()->fAgility = 4.f;
	m_pBasicStat->Get_Stat()->fDeffense = 4.f;
	m_pBasicStat->Get_Stat()->fMagic = 4.f;
	m_pBasicStat->Get_Stat()->fAttack = 4.f;
	m_pBasicStat->Get_Stat()->fHealth = 4.f;
	m_pBasicStat->Get_Stat()->iExp = 6.f;
}

void CWizard::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::ITEM &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYERBULLET)
		__super::OnCollisionEnter(_pOther);
}

void CWizard::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::ITEM &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYERBULLET)
		__super::OnCollisionStay(_pOther);
}

void CWizard::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }


}

HRESULT CWizard::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pRigidBody = dynamic_cast<CRigidBody*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	pComponent = m_pTexture[(_uint)STATE::ROMIMG] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Wizard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::ATTACK] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_WizardAttack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::HIT] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_WizardHit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::DEAD] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_WizardDead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

	pComponent = m_pStateMachine = dynamic_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_State"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CWizard* CWizard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWizard* pInstance = new CWizard(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CWizard*>(pInstance);

		MSG_BOX("Wizard Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CWizard::Free()
{
	__super::Free();

}
