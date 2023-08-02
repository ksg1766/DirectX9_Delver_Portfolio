
#include "stdafx.h"
#include "SoundManager.h"
#include "Boss_MeteorCube3Ph.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Player.h"
#include "BossExplosion.h"
#include "FlyingCamera.h"
#include "CameraManager.h"
CBoss_MeteorCube3Ph::CBoss_MeteorCube3Ph(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
{
}

CBoss_MeteorCube3Ph::CBoss_MeteorCube3Ph(const CBoss_MeteorCube3Ph& rhs)
	: CMonster(rhs)
{
}

CBoss_MeteorCube3Ph::~CBoss_MeteorCube3Ph()
{
	Free();
}

HRESULT CBoss_MeteorCube3Ph::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::MONSTERBULLET;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_bChanneling_Start = false;
	m_bChanneling_End = false;
	m_bTargetSet = false;
	m_bHit = false;
	m_bSound = false;
	m_bMaxHeight = 20.f;
	m_fMeteorExplosionTime = 0.f;
	m_fScale = 0;
	m_fEndTime = 0.f;
	m_fAttack = 15.f;
	m_pTransform->Scale(_vec3(0.6f, 0.6f, 0.6f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	return S_OK;
}

_int CBoss_MeteorCube3Ph::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_PRIORITY, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_uint iExit = __super::Update_Object(fTimeDelta);

	if (STATE::BOSS_STURN == dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_StateMachine()->Get_State())
	{
		m_bChanneling_Start = false;
		m_bChanneling_End = false;
		m_fEndTime = 0.f;
		m_fAttack = 15.0;
		m_pTransform->Scale(_vec3(0.6f, 0.6f, 0.6f));
		Engine::EventManager()->DeleteObject(this);
	}
	m_fScale += fTimeDelta / 1.5f;
	m_fEndTime += fTimeDelta;
	if ((m_bChanneling_Start) && (20.f <= m_fEndTime))
	{
		Channeling_End(fTimeDelta);
		return iExit;
	}
	if (m_bChanneling_Start)
	{
		if (!m_bSound)
		{
			CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_ALIEN);
			CSoundManager::GetInstance()->PlaySound(L"Boss_MeteorCharging.wav", CHANNELID::SOUND_ALIEN, 0.4f);
			m_bSound = true;
		}
		Channeling_Now(fTimeDelta);
		m_pTransform->Translate(_vec3(-fTimeDelta / 4.f, 0.f, 0.f));
	}
	else
		m_bChanneling_Start = true;

	if (15.f > m_fScale)
	{
		m_pTransform->Scale(_vec3(m_fScale, m_fScale, m_fScale));
	}
	return iExit;
}

void CBoss_MeteorCube3Ph::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CBoss_MeteorCube3Ph::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(40);
	m_pCubeBf->Render_Buffer();//큐브버퍼

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CBoss_MeteorCube3Ph::Channeling_Begin()
{
	m_vCenter = Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front()->m_pTransform->m_vInfo[INFO_POS];
	m_pTransform->m_vInfo[INFO_POS] = Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front()->m_pTransform->m_vInfo[INFO_POS];
	m_pTransform->Translate(_vec3(8.f, 0.f, 0.f));
}

void CBoss_MeteorCube3Ph::Channeling_Now(const _float& fTimeDelta)
{
	CFlyingCamera* pCamera = dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam());
	pCamera->Set_ShakeForce(0.f, 0.1f, 0.3f, 2.f);
	pCamera->Shake_Camera();

	m_pTransform->Translate(_vec3(0.f, 3.f * fTimeDelta, 0.f));
	m_pTransform->Rotate(_vec3(0.f, 0.f, 3.f));
	m_pTransform->RotateAround(m_vCenter, _vec3(0.f, 3.f, 0.f), 3.f * fTimeDelta / 2.f);
	m_fAttack *= 1.5;
}

void CBoss_MeteorCube3Ph::Channeling_End(const _float& fTimeDelta)
{
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	m_vTargetPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
	m_vDir = m_vTargetPos - m_pTransform->m_vInfo[INFO_POS];
	m_pTransform->Translate(m_vDir * fTimeDelta);
}

void CBoss_MeteorCube3Ph::Set_PlayerPos()
{
	m_vTargetPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
	m_vDir = m_vTargetPos - m_pTransform->m_vInfo[INFO_POS];
	m_bTargetSet = true;
}

void CBoss_MeteorCube3Ph::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (OBJECTTAG::PLAYER == _pOther->Get_ObjectTag())
	{
		CFlyingCamera* pCamera = dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam());
		pCamera->Set_ShakeForce(0.f, 0.5f, 0.5f, 2.f);
		pCamera->Shake_Camera();
		m_bSound = false;

			m_fMeteorExplosionTime = 0.f;
			m_bHit = true;
			CPlayerStat& PlayerState = *dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer())->Get_Stat();
			PlayerState.Take_Damage(m_fAttack);
			this->Set_AttackTick(true);

			m_bChanneling_Start = false;
			m_bChanneling_End = false;
			m_fEndTime = 0.f;
			m_pBasicStat->Get_Stat()->fAttack = 15.0;
			m_pTransform->Scale(_vec3(0.6f, 0.6f, 0.6f));
			Engine::CGameObject* pGameObject = nullptr;
			pGameObject = CBossExplosion::Create(m_pGraphicDev);
			m_vTargetPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
			_vec3 m_vDis = (dynamic_cast<CPlayer*>(_pOther->Get_Host())->m_pTransform->m_vInfo[INFO_LOOK] * 0.2f);
			dynamic_cast<CBossExplosion*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_vTargetPos.x + m_vDis.x, m_pTransform->m_vInfo[INFO_POS].y, m_vTargetPos.z + m_vDis.z);
			dynamic_cast<CBossExplosion*>(pGameObject)->Set_Scale(300.f);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			Engine::EventManager()->DeleteObject(this);
	}
}

void CBoss_MeteorCube3Ph::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

}

void CBoss_MeteorCube3Ph::OnCollisionExit(CCollider* _pOther)
{
	m_pBasicStat->Get_Stat()->fAttack = 15.f;
}

void CBoss_MeteorCube3Ph::Init_Stat()
{
}

void CBoss_MeteorCube3Ph::Set_Center(_vec3 _CenterPos)
{
	m_pTransform->m_vInfo[INFO_POS] = _CenterPos;
}

HRESULT CBoss_MeteorCube3Ph::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pCubeBf = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Cube"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CBoss_MeteorCube3Ph* CBoss_MeteorCube3Ph::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss_MeteorCube3Ph* pInstance = new CBoss_MeteorCube3Ph(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Meteor Cube Failed");
		return nullptr;
	}
	return pInstance;
}

void CBoss_MeteorCube3Ph::Free()
{
	__super::Free();
}