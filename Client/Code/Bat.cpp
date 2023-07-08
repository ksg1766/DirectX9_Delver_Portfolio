#include "..\Header\Bat.h"
#include "Export_Function.h"

CBat::CBat(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev), m_fFrame(0.f)
{
}

CBat::CBat(const CBat& rhs)
	: CMonster(rhs), m_fFrame(rhs.m_fFrame)
{
}

CBat::~CBat()
{
}

HRESULT CBat::Ready_Object()
{
	Set_ObjectTag(OBJECTTAG::MONSTER);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	m_pTransform->Translate(_vec3(2.f, 3.f, 10.f));

	return S_OK;
}

_int CBat::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	m_fFrame += 5.f * fTimeDelta;

	if (2 < m_fFrame)
		m_fFrame = 0.F;


	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CBat::LateUpdate_Object()
{
	__super::LateUpdate_Object();
}

void CBat::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTexture->Render_Texture((_uint)m_fFrame);
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBat::Add_Component(void)
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

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pStateMachine = dynamic_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_State"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Bat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CBat* CBat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBat* pInstance = new CBat(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CBat*>(pInstance);

		MSG_BOX("Bat Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBat::Free()
{
	__super::Free();
}
