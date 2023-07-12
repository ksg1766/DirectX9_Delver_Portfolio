#include "stdafx.h"
#include "Box_Cube.h"
#include "Export_Function.h"
#include "Player.h"
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
	m_eObjectTag = OBJECTTAG::ENVIRONMENT_OBJECT;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fHitCool = 0.f;
	m_fShakeCool = 0.f;
	m_bHit = false;
	m_bShake = false;
	m_pTransform->Scale(_vec3 (0.5f, 0.5f, 0.5f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	m_pTransform->Translate(_vec3(-20.f, 0.5f, 20.f));
	m_pBasicStat->Get_Stat()->fHealth = 3.f;
	return S_OK;
}

_int CBox_Cube::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	m_fHitCool += fTimeDelta;
	m_fShakeCool += fTimeDelta;
	//Shake_Box(fTimeDelta);

	if ((1.5f < m_fHitCool)&&(m_bHit))
	{
		m_fHitCool = 0.f;
		m_pBasicStat->Get_Stat()->fHealth -= 1;
		m_bHit = false;
	}

	if (0 >= m_pBasicStat->Get_Stat()->fHealth)
		Engine::EventManager()->DeleteObject(this);
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
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	m_pTexture->Render_Texture(m_iTextureNumber);
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBox_Cube::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (m_bHit)
		return;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->GetInstance()
		->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
	if ((pPlayer->Get_Attack())&&(_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::ITEM))
	{
		m_bShake = true;
		m_bHit = true;
	}

}

void CBox_Cube::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

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

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Cube"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CBox_Cube::Shake_Box(const _float& fTimeDelta)
{
	if ((!m_bHit) || (!m_bShake))
		return;
	if (0.02 > m_fShakeCool)
		m_pTransform->Rotate(_vec3 (0.f, 0.05f, 0.f));
	else if (0.02 < m_fShakeCool)
	{
		m_pTransform->Rotate(_vec3(0.f, 0.05f, 0.f));
		m_fShakeCool = 0.f;
		m_bShake = false;
	}
}

CBox_Cube* CBox_Cube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBox_Cube* pInstance = new CBox_Cube(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CubeBlock Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBox_Cube::Free()
{
	__super::Free();
}
