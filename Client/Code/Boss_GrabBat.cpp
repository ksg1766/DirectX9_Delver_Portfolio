#include "Boss_GrabBat.h"
#include "Export_Function.h"
#include "Player.h"
#include "SkeletonKing.h"
#include "SkeletonKing_Clone.h"
#include "DynamicCamera.h"
#include "CameraManager.h"
CBoss_GrabBat::CBoss_GrabBat(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CMonster(pGraphicDev)
{
}

CBoss_GrabBat::CBoss_GrabBat(const CBoss_GrabBat& rhs)
	: Engine::CMonster(rhs)
{
}

CBoss_GrabBat::~CBoss_GrabBat()
{
	Free();
}

HRESULT CBoss_GrabBat::Ready_Object(void)
{
	//m_eObjectTag = OBJECTTAG::MONSTERBULLET;
	m_eObjectTag = OBJECTTAG::MONSTER;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fFrame = 0.f;
	m_bGrab = false;
	m_bHit = false;
	m_iHp = 2;
	m_fHitCool = 0.f;
	m_fSpeed = 3.f;
	return S_OK;
}

_int CBoss_GrabBat::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_uint iExit = __super::Update_Object(fTimeDelta);
	m_fFrame += 2.f * fTimeDelta * 2;
	m_fHitCool += fTimeDelta;
	if ((m_bHit)&&(0.5f < m_fHitCool))
	{
		m_bHit = false;
		m_fHitCool = 0.f;
	}
	if (2.f < m_fFrame)
		m_fFrame = 0.f;
	if (m_bGrab)
	{
		m_vDir = Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front()->m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransform->Translate(m_vDir * m_fSpeed* fTimeDelta);
		m_vOffset = m_pTransform->m_vInfo[INFO_LOOK] * 2.f;
		//Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS] = m_vOffset;//m_pTransform->m_vInfo[INFO_POS];//플레이어 강제로 끌고가게하기
		Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->Translate(_vec3(m_vOffset.x, 0.f, m_vOffset.z));//m_pTransform->m_vInfo[INFO_POS];//플레이어 강제로 끌고가게하기
	}
	else if (!m_bGrab)
	{
		m_vDir = Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransform->Translate(m_vDir * (m_fSpeed*5.f)* fTimeDelta);
	}
	if (0 < m_iHp)
		Engine::EventManager()->DeleteObject(this);

	return iExit;
}

void CBoss_GrabBat::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CBoss_GrabBat::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(_uint(m_fFrame));
	m_pBuffer->Render_Buffer();
}

void CBoss_GrabBat::Init_Stat()
{
}

void CBoss_GrabBat::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CBoss_GrabBat::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (OBJECTTAG::PLAYER == _pOther->GetHost()->Get_ObjectTag())
	{
		if (m_bGrab) { return; }
		m_bGrab = true;
	}
	if (m_bHit) { return; }
	if ((OBJECTTAG::ITEM == _pOther->GetHost()->Get_ObjectTag()) || (OBJECTTAG::PLAYERBULLET == _pOther->GetHost()->Get_ObjectTag()))
	{
		if (!dynamic_cast<CPlayer*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front())->Get_Attack())
			return;
		m_bHit = true;
		if(m_bGrab)
			--m_iHp;
		else 
			----m_iHp;
	}
}

void CBoss_GrabBat::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CBoss_GrabBat::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossBatSwarm"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CBoss_GrabBat::PickingBat()
{
	
}

CBoss_GrabBat* CBoss_GrabBat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss_GrabBat* pInstance = new CBoss_GrabBat(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Boss_GrabBat Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBoss_GrabBat::Free()
{
	__super::Free();
}
