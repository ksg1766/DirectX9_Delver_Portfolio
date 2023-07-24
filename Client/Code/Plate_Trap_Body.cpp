#include "Plate_Trap_Body.h"
#include "Export_Function.h"
#include "Player.h"
#include "BossExplosion.h"
#include "Item.h"

CPlate_Trap::CPlate_Trap(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CTrap(pGraphicDev)
{
}

CPlate_Trap::CPlate_Trap(const CPlate_Trap& rhs)
	:Engine::CTrap(rhs)
{
}

CPlate_Trap::~CPlate_Trap()
{
}

HRESULT CPlate_Trap::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::TRAP;
	//m_eTrapTag = TRAPTAG::PLATE;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Scale(_vec3(0.4f, 0.15f, 0.4f));
	m_fTime = 0.f;
	m_bAttack = false;
	m_fInitialHeight = false;

	__super::Ready_Object();
	//m_pTransform->Translate(_vec3(0.0f, -0.85f, 0.0f));

	return S_OK;
}

_int CPlate_Trap::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	_uint iExit = 0;
	if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
		return iExit;

	if (SceneManager()->Get_GameStop()) { return 0; }
	iExit = __super::Update_Object(fTimeDelta);
	m_fTime += fTimeDelta;
	if (m_bAttack)
		Trap_On();
	return iExit;
}

void CPlate_Trap::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CPlate_Trap::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture();
	m_pCubeBf->Render_Buffer();
}

void CPlate_Trap::Trap_On()
{
	//이펙트 종류보고 추가할 예정
}

void CPlate_Trap::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CPlate_Trap::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (m_bAttack) { return; }
	m_pOtherObj = _pOther->GetHost();
	if (OBJECTTAG::PLAYER == m_pOtherObj->Get_ObjectTag())
	{
		//독함정
		/*dynamic_cast<CPlayer*>(m_pOtherObj)->Set_Poisoning(true);
		m_pTransform->Scale(_vec3(0.4f, 0.05f, 0.4f));
		m_bAttack = true;*/

		//폭발함정
		CGameObject* pGameObject = CBossExplosion::Create(m_pGraphicDev);
		m_pTransform->Scale(_vec3(0.4f, 0.05f, 0.4f));
		pGameObject->m_pTransform->Translate(_vec3(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y+1.f, m_pTransform->m_vInfo[INFO_POS].z));
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

		m_bAttack = true;
	}
	if (OBJECTTAG::ITEM == m_pOtherObj->Get_ObjectTag())
	{
		if (dynamic_cast<CItem*>(m_pOtherObj)->Get_WorldItem())
		{
			CGameObject* pGameObject = CBossExplosion::Create(m_pGraphicDev);
			m_pTransform->Scale(_vec3(0.4f, 0.05f, 0.4f));
			pGameObject->m_pTransform->Translate(_vec3(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y + 1.f, m_pTransform->m_vInfo[INFO_POS].z));
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		m_bAttack = true;
	}
}

void CPlate_Trap::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CPlate_Trap::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_pCubeBf = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_PlateTrapBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CPlate_Trap* CPlate_Trap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlate_Trap* pInstance = new CPlate_Trap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Plate_Trap_Body Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPlate_Trap::Free()
{
	__super::Free();
}
