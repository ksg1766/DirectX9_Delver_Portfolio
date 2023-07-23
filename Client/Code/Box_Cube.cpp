#include "stdafx.h"
#include "Box_Cube.h"
#include "Export_Function.h"
#include "Player.h"
#include "Bow.h"
#include "TempItem.h"
#include "EffectBrokenbox.h"
#include "EffectSquare.h"
CBox_Cube::CBox_Cube(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
}

CBox_Cube::CBox_Cube(const CBox_Cube& rhs)
    : CGameObject(rhs)
{
}

CBox_Cube::~CBox_Cube()
{
	Free();
}

HRESULT CBox_Cube::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::FRAGILE;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fHitCool = 0.f;
	m_fShakeDelay = 0.f;
	m_bHit = false;
	m_bShake = false;
	m_iHP = 2;
	m_pTransform->Scale(_vec3 (0.5f, 0.5f, 0.5f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale() * 1.2f);
	return S_OK;
}

_int CBox_Cube::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);
	m_fHitCool += fTimeDelta;
	if (1.5f < m_fHitCool)
		m_bHit = false;
	//if(m_bShake)
	//	Shake_Box(fTimeDelta);
	if (0 >= m_iHP)
		Drop_RandomItem();
	return iExit;
}

void CBox_Cube::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CBox_Cube::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	
	m_pTexture->Render_Texture(m_iTextureNumber);
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CBox_Cube::Drop_RandomItem()
{
	CItem* pItem = CTempItem::Create(m_pGraphicDev, true);
	pItem->m_pTransform->Translate(m_pTransform->m_vInfo[INFO_POS]);
	dynamic_cast<CTempItem*>(pItem)->Set_WorldItem(true);
	Engine::EventManager()->CreateObject(pItem, LAYERTAG::GAMELOGIC);

	// 이펙트 생성
	CGameObject* pGameObject = CEffectBrokenbox::Create(m_pGraphicDev);
	dynamic_cast<CEffectBrokenbox*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_vInfo[INFO_POS];
	Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

	Engine::EventManager()->DeleteObject(this);
}

void CBox_Cube::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CBox_Cube::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (m_bHit) { return; }
	m_pOtherObj = _pOther->GetHost();
	if ((OBJECTTAG::ITEM == m_pOtherObj->Get_ObjectTag()) || (OBJECTTAG::PLAYERBULLET == m_pOtherObj->Get_ObjectTag()))
	{
		if (!dynamic_cast<CPlayer*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front())->Get_Attack())
			return;
		//깨지고 난 후 아이템 생성 후 삭제시킬 예정
		m_fHitCool = 0.f;
		m_bHit = true;
		m_bShake = true;
		--m_iHP;
		return;
	}
}

void CBox_Cube::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CBox_Cube::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_WoodBox"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CBox_Cube::Shake_Box(const _float& fTimeDelta)
{
	m_fShakeDelay += fTimeDelta;
	if(0.01 > m_fShakeDelay)
		m_pTransform->Rotate(ROT_Y, 1.f);
	else if (0.03 > m_fShakeDelay)
	{
		m_pTransform->Rotate(ROT_Y, -1.f);
		m_bShake = false;
	}

}	

CBox_Cube* CBox_Cube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBox_Cube* pInstance = new CBox_Cube(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CubeBox Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBox_Cube::Free()
{
	__super::Free();
}
