#include "stdafx.h"
#include "..\Header\SkeletonKing_Clone.h"
#include "Export_Function.h"
#include "BossProjectile.h"
#include "BossExplosion.h"

CSkeletonKing_Clone::CSkeletonKing_Clone(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bSkill(false)
{

}

CSkeletonKing_Clone::CSkeletonKing_Clone(const CSkeletonKing_Clone& rhs)
	: Engine::CGameObject(rhs), m_bSkill(rhs.m_bSkill)
{
}

CSkeletonKing_Clone::~CSkeletonKing_Clone()
{
	Free();
}

HRESULT CSkeletonKing_Clone::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BOSS;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Scale(_vec3(1.f, 1.f, 1.f));
	return S_OK;
}

_int CSkeletonKing_Clone::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);
	
	m_fFrame += 8.f * fTimeDelta;
	m_fSkillCool += fTimeDelta;
	if (8.f < m_fFrame)
		m_fFrame = 0.f;

	if (!m_bSkill)
		Clone_Pattern();

	return iExit;
}

void CSkeletonKing_Clone::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	m_pBillBoard->LateUpdate_Component();
	__super::LateUpdate_Object();
	__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CSkeletonKing_Clone::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pTexture->Render_Texture((_uint)m_fFrame);
	m_pBuffer->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CSkeletonKing_Clone::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Boss"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pBillBoard = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

//테스트 용 코드입니다.
void CSkeletonKing_Clone::Clone_Pattern()
{
	if (3.f < m_fSkillCool)
	{
		m_fSkillCool = 0.f;
		Engine::CGameObject* pGameObject = nullptr;
		srand(unsigned(time(NULL)));
		int _iTemp = rand() % 2;
		switch (_iTemp)
		{
		case 0:
			m_bSkill = true;
			pGameObject = CBossProjectile::Create(m_pGraphicDev);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			dynamic_cast<CBossProjectile*>(pGameObject)->Set_Target(m_pTransform->m_vInfo[INFO_POS]);
			Engine::EventManager()->DeleteObject(this);
			break;
		case 1:
			m_bSkill = true;
			pGameObject = CBossExplosion::Create(m_pGraphicDev);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPos(m_pTransform->m_vInfo[INFO_POS]);
			dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPosY(-2.f);
			Engine::EventManager()->DeleteObject(this);
			break;
		}
	}
}
//테스트 용 코드입니다.


CSkeletonKing_Clone* CSkeletonKing_Clone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkeletonKing_Clone* pInstance = new CSkeletonKing_Clone(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SkeletonKing Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSkeletonKing_Clone::Free()
{
	__super::Free();
}
