#include "Projectile_Trap_Body.h"
#include "Export_Function.h"
#include "Projectile_Trap_Arrow.h"

CProjectile_Trap::CProjectile_Trap(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CProjectile_Trap::CProjectile_Trap(const CProjectile_Trap& rhs)
	:Engine::CGameObject(rhs)
{
}

CProjectile_Trap::~CProjectile_Trap()
{
}

HRESULT CProjectile_Trap::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::MONSTERBULLET; //미정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(0.5f, 0.5f, 0.5f));

	return S_OK;
}

_int CProjectile_Trap::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_PRIORITY, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_uint iExit = __super::Update_Object(fTimeDelta);

	if (Engine::InputDev()->Key_Down(DIK_Z))
		Create_Projectile();
	if (Engine::InputDev()->Key_Down(DIK_X))
	{
		m_pTransform->Rotate(_vec3(0.f, 45.f, 0.f));
	}
	return iExit;
}

void CProjectile_Trap::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CProjectile_Trap::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture();
	m_pCubeBf->Render_Buffer();
}

void CProjectile_Trap::Create_Projectile()
{
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CProjectile_Trap_Arrow::Create(m_pGraphicDev);
	dynamic_cast<CProjectile_Trap_Arrow*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_vInfo[INFO_POS];
	//dynamic_cast<CProjectile_Trap_Arrow*>(pGameObject)->Set_Dir());//뭘 넣어야 바라보는 방향으로 나갈까
	Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
}

HRESULT CProjectile_Trap::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_pCubeBf = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_ProjectileTrapBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CProjectile_Trap* CProjectile_Trap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CProjectile_Trap* pInstance = new CProjectile_Trap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Projectile_Trap_Body Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CProjectile_Trap::Free()
{
	__super::Free();
}
