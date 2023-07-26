#include "Pot.h"
#include "Export_Function.h"
#include "Player.h"
CPot::CPot(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CPot::CPot(const CPot& rhs)
	: Engine::CGameObject(rhs)
{
}

CPot::~CPot()
{
	
}

HRESULT CPot::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::FRAGILE;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_bCrash = false;
	m_bHit = false;
	m_fHitCool = 0.f;
	m_pTransform->Scale(_vec3 (0.5f, 0.5f, 0.5f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale() * 1.1f);
	return S_OK;
}

_int CPot::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_uint iExit = __super::Update_Object(fTimeDelta);

	if (m_bCrash)
		Engine::EventManager()->DeleteObject(this);//Drop_RandomItem();

	return iExit;
}

void CPot::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CPot::Render_Object()
{
	m_pTransform->Scale(_vec3(0.5f, 0.5f, 0.5f));
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(0);
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CPot::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CPot::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (m_bHit) { return; }
	m_pOtherObj = _pOther->Get_Host();
	if ((OBJECTTAG::ITEM == m_pOtherObj->Get_ObjectTag()) || (OBJECTTAG::PLAYERBULLET == m_pOtherObj->Get_ObjectTag()))
	{
		if (!dynamic_cast<CPlayer*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front())->Get_Attack())
			return;
		//깨지고 난 후 아이템 생성 후 삭제시킬 예정
		//cout << "항아리 공격받음" << endl;
		m_bHit = true;
		m_bCrash = true;
		return;
	}
}

void CPot::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CPot::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Pot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);
	return S_OK;
}

void CPot::Drop_RandomItem()
{


}

CPot* CPot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPot* pInstance = new CPot(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CPot*>(pInstance);

		MSG_BOX("Trader Npc Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPot::Free()
{
	__super::Free();
}
