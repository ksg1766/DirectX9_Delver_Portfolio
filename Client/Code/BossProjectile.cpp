#include "stdafx.h"
#include "..\Header\BossProjectile.h"
#include "Export_Function.h"
#include "Player.h"
#include "SoundManager.h"
CBossProjectile::CBossProjectile(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev), m_fFrame(0.f)
{
}

CBossProjectile::CBossProjectile(const CBossProjectile& rhs)
	:	Engine::CMonster(rhs), m_fFrame(rhs.m_fFrame)
{
}

CBossProjectile::~CBossProjectile()
{
	Free();
}

HRESULT CBossProjectile::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::MONSTERBULLET;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pBasicStat->Get_Stat()->fAttack = 2.f;
	m_fTime = 0.f;
	m_fSpeed = 20.f;
	m_bHit = false;
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale()*0.8f);
	CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WARRIOR);
	CSoundManager::GetInstance()->PlaySound(L"Boss_FireBall1.wav", CHANNELID::SOUND_WARRIOR, 1.f);
	return S_OK;
}

_int CBossProjectile::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_int iExit = __super::Update_Object(fTimeDelta);
	m_fTime += fTimeDelta;
		if ((!m_bHit)&&(5.f < m_fTime))
		{
			m_fTime = 0.f;
			m_IsDead = true;
		}
		m_pTransform->Translate(m_vDir* m_fSpeed * fTimeDelta);
		m_fFrame += 8.f * fTimeDelta * 2;
		if (8.f < m_fFrame)
				m_fFrame = 0.f;

	return iExit;
}

void CBossProjectile::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CBossProjectile::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pTexture->Render_Texture((_uint)m_fFrame);
	m_pBuffer->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CBossProjectile::Init_Stat()
{
}

void CBossProjectile::Set_Target(_vec3 _vPos)
{
	m_pTransform->m_vInfo[INFO_POS] = _vPos;
	m_pTransform->Translate(_vec3(1.f, 3.f, 0.f));
	m_vTargetPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
	m_vDir = m_vTargetPos - m_pTransform->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&m_vDir, &m_vDir);
}

void CBossProjectile::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CBossProjectile::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (m_bHit) { return; }
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_DOOR);
		CSoundManager::GetInstance()->PlaySound(L"Fire_Hit1.wav", CHANNELID::SOUND_DOOR, 1.f);

		CPlayerStat& PlayerState = *(dynamic_cast<CPlayer*>(_pOther->Get_Host())->Get_Stat());
		PlayerState.Take_Damage(this->Get_BasicStat()->Get_Stat()->fAttack);
		this->Set_AttackTick(true);
		m_bHit = false;
		Engine::EventManager()->DeleteObject(this);
	}
}

void CBossProjectile::OnCollisionExit(CCollider* _pOther)
{

}

HRESULT CBossProjectile::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossFire"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CBossProjectile* CBossProjectile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossProjectile* pInstance = new CBossProjectile(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BossProjectitle Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBossProjectile::Free()
{
	Safe_Release(m_pEffect);

	__super::Free();
}
