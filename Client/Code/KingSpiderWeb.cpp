#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpiderWeb.h"
#include "Export_Function.h"
#include "Player.h"

CKingSpiderWeb::CKingSpiderWeb(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev)
{
}

CKingSpiderWeb::CKingSpiderWeb(const CKingSpiderWeb& rhs)
	: Engine::CMonster(rhs)
{
}

CKingSpiderWeb::~CKingSpiderWeb()
{
	Free();
}

HRESULT CKingSpiderWeb::Ready_Object(void)
{
	Set_ObjectTag(OBJECTTAG::MONSTERBULLET);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	/*CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WARRIOR);
	CSoundManager::GetInstance()->PlaySound(L"Boss_FireBall1.wav", CHANNELID::SOUND_WARRIOR, 1.f);*/
	m_bDebuff = false;
	m_bPlayerHit = false;
	m_bHit = false;
	m_bFloor = false;
	m_fDelay = 0.f;
	m_iHp = 2;
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS],&m_pTransform->m_vInfo[INFO_RIGHT],m_pTransform->LocalScale());
	return S_OK;
}

_int CKingSpiderWeb::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_int iExit = __super::Update_Object(fTimeDelta);

	m_fDelay += fTimeDelta;
	//if((!m_bPlayerHit)&&(!m_bFloor))
		//m_pTransform->Translate(m_vDir);
	if (m_bDebuff)
	{
		m_vPlayerLook = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer())->m_pTransform->m_vInfo[INFO_LOOK];
		m_vPlayerRight = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer())->m_pTransform->m_vInfo[INFO_RIGHT];
		m_pTransform->m_vInfo[INFO_POS] = _vec3(m_vPlayerLook.x * 2.f, m_vPlayerLook.y*2.f, m_vPlayerLook.z * 2.f);
		if (STATE::ROMIMG == dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer())->Get_StateMachine()->Get_State())
		{
			if(Engine::InputDev()->Key_Pressing(DIK_W))
				dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer())->m_pTransform->Translate(-m_vPlayerLook * 4.f * fTimeDelta);
			if (Engine::InputDev()->Key_Pressing(DIK_S))
				dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer())->m_pTransform->Translate(m_vPlayerLook * 4.f * fTimeDelta);
			if (Engine::InputDev()->Key_Pressing(DIK_D))
				dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer())->m_pTransform->Translate(-m_vPlayerRight * 4.f * fTimeDelta);
			if (Engine::InputDev()->Key_Pressing(DIK_A))
				dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer())->m_pTransform->Translate(m_vPlayerRight * 4.f * fTimeDelta);
		}
		if(dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer())->IsJump())
			dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer())->Get_RigidBody()->Add_Force(_vec3(0.f, -0.5f, 0.f));
		
	}
	if ((1.f < m_fDelay) && (m_bHit))
	{
		m_fDelay = 0.f;
		m_bHit = false;
	}

	return iExit;
}

void CKingSpiderWeb::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
	if (0 >= m_iHp)
	{
		m_IsDead = true;
		m_bDebuff = false;
		m_bPlayerHit = false;
		m_bHit = false;
		m_bFloor = false;
		m_fDelay = 0.f;
	}
}

void CKingSpiderWeb::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(11);
	m_pBuffer->Render_Buffer();
	m_pTransform->Rotate(ROT_Y, D3DXToRadian(90.f));
	_matrix matWorld = m_pTransform->WorldMatrix();
	m_pTransform->Rotate(ROT_Y, D3DXToRadian(-90.f));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pTexture->Render_Texture(11);
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CKingSpiderWeb::Init_Stat()
{
}

void CKingSpiderWeb::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER)
		__super::OnCollisionEnter(_pOther);
}

void CKingSpiderWeb::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if ((_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)&&(!m_bPlayerHit))
		{
			m_bDebuff = true;
			m_bPlayerHit = true;
		}
	if (OBJECTTAG::ITEM == _pOther->Get_Host()->Get_ObjectTag() && (!m_bHit))
	{
		if (dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer())->Get_Attack())
		{
			--m_iHp;
			m_bHit = 0.f;
			m_bHit = true;
		}
	}
}

void CKingSpiderWeb::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CKingSpiderWeb::Set_Dir(_vec3 _vDir)
{
	m_vDir = _vDir - m_pTransform->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&m_vDir ,&m_vDir);
}

HRESULT CKingSpiderWeb::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_ImmortalSprite"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	/*pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);*/

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

CKingSpiderWeb* CKingSpiderWeb::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CKingSpiderWeb* pInstance = new CKingSpiderWeb(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("KingSpider Web Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CKingSpiderWeb::Free()
{
	__super::Free();
}
