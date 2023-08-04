#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpiderPoison.h"
#include "Export_Function.h"
#include "Player.h"
#include "EffectSquare.h"
CKingSpiderPoison::CKingSpiderPoison(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev)
{
}

CKingSpiderPoison::CKingSpiderPoison(const CKingSpiderPoison& rhs)
	: Engine::CMonster(rhs)
{
}

CKingSpiderPoison::~CKingSpiderPoison()
{
	Free();
}

HRESULT CKingSpiderPoison::Ready_Object(void)
{
	Set_ObjectTag(OBJECTTAG::MONSTERBULLET);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_bHit = false;
	m_bStraight = false;
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	return S_OK;
}

_int CKingSpiderPoison::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_int iExit = __super::Update_Object(fTimeDelta);
	if (m_bStraight)
		m_pTransform->Translate(m_vDir);



	return iExit;
}

void CKingSpiderPoison::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CKingSpiderPoison::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(2);
	m_pBuffer->Render_Buffer();
}

void CKingSpiderPoison::Init_Stat()
{
}

void CKingSpiderPoison::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

}

void CKingSpiderPoison::OnCollisionStay(CCollider* _pOther)
{

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BLOCK)
	{
		_matrix      matMonsterWorld = m_pTransform->WorldMatrix();
		_vec3        vecMonsterPos = _vec3(matMonsterWorld._41, matMonsterWorld._42 + .5f, matMonsterWorld._43);
		CGameObject* pGameObject = CEffectSquare::Create(m_pGraphicDev, vecMonsterPos, 50, EFFECTCOLOR::ECOLOR_GREEN);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		m_bStraight = false;

		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_KINGSPIDER);
		CSoundManager::GetInstance()->PlaySound(L"KingSpider_Poison.wav", CHANNELID::SOUND_KINGSPIDER, 1.f);

		EventManager()->DeleteObject(this);
	}

	if ((_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)&&(!m_bHit))
	{
		_matrix      matMonsterWorld = _pOther->Get_Host()->m_pTransform->WorldMatrix();
		_vec3        vecMonsterPos = _vec3(matMonsterWorld._41, matMonsterWorld._42 + .5f, matMonsterWorld._43);
		CGameObject* pGameObject = CEffectSquare::Create(m_pGraphicDev, vecMonsterPos, 50, EFFECTCOLOR::ECOLOR_GREEN);

		static_cast<CPlayer*>(_pOther->Get_Host())->Set_Poisoning(true);
		m_bHit = true;
		m_bStraight = false;

		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_KINGSPIDER);
		CSoundManager::GetInstance()->PlaySound(L"KingSpider_Poison.wav", CHANNELID::SOUND_KINGSPIDER, 1.f);

		EventManager()->DeleteObject(this);
	}
}

void CKingSpiderPoison::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CKingSpiderPoison::Set_DirParabola(_vec3 _vDir)
{
	m_vDir = _vDir - m_pTransform->m_vInfo[INFO_POS];
	//D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_pRigidBody->Add_Force(_vec3(m_vDir.x, 8.f, m_vDir.z));
}

void CKingSpiderPoison::Set_DirStraight(_vec3 _vDir)
{
	m_vDir = _vDir - m_pTransform->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_bStraight = true;
}


HRESULT CKingSpiderPoison::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectBlood"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = static_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = static_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	pComponent = m_pRigidBody = static_cast<CRigidBody*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CKingSpiderPoison* CKingSpiderPoison::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CKingSpiderPoison* pInstance = new CKingSpiderPoison(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("KingSpider Poision Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CKingSpiderPoison::Free()
{
	__super::Free();
}
