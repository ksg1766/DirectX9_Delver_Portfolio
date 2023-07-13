#include "..\Header\Arrow.h"
#include "Export_Function.h"
#include "Player.h"

CArrow::CArrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev)
{
}

CArrow::CArrow(const CArrow& rhs)
	: CItem(rhs)
{

}

CArrow::~CArrow()
{
	Free();
}

HRESULT CArrow::Ready_Object(CTransform* Weapon, CTransform* pOwner, _float _fSpeed)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_eObjectTag = OBJECTTAG::PLAYERBULLET;
	m_fSpeed = _fSpeed;

	//m_pTransform->Set_Parent(pOwner);
	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		m_pTransform->LocalScale() * 0.5f);
	
	if (Weapon != nullptr && pOwner != nullptr)
	{
		m_pTransform->m_vInfo[INFO_POS] = Weapon->m_vInfo[INFO_POS];
		m_pTransform->Copy_RUL(pOwner->Get_Transform()->m_vInfo);
	}

	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
	m_pBasicStat->Get_Stat()->fAttack = 1.f;

	return S_OK;
}

_int CArrow::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	
	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->GetInstance()->
		Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	_vec3 vDir = m_pTransform->m_vInfo[INFO_LOOK];
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_vInfo[INFO_POS] + vDir * m_fSpeed * fTimeDelta;

	return iExit;
}

void CArrow::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CArrow::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();


#if _DEBUG
	m_pCollider->Render_Collider();
#endif

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CArrow::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Arrow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CArrow::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CArrow::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CArrow::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

CArrow* CArrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* Weapon,
	CTransform* pOwner, _float _fSpeed)
{
	CArrow* pInstance = new CArrow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(Weapon, pOwner, _fSpeed)))
	{
		Safe_Release<CArrow*>(pInstance);

		MSG_BOX("Create Arrow Failed");
		return nullptr;
	}

	return pInstance;
}

void CArrow::Free()
{
	__super::Free();
}
