#include "Blade_Trap_Body.h"
#include "Export_Function.h"
#include "Blade_Trap_Blade.h"

CBlade_Trap::CBlade_Trap(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBlade_Trap::CBlade_Trap(const CBlade_Trap& rhs)
	:Engine::CGameObject(rhs)
{
}

CBlade_Trap::~CBlade_Trap()
{
}

HRESULT CBlade_Trap::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::MONSTER;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(1.f, 0.2f, 1.f));

	m_vBladePos[0] = _vec3(0.6f,	0.f,	0.6f);//우측상단
	m_vBladePos[1] = _vec3(0.6f,	0.f,	-0.6f);//우측하단
	m_vBladePos[2] = _vec3(-0.6f,	0.f,	-0.6f);//좌측하단
	m_vBladePos[3] = _vec3(-0.6f,	0.f,	0.6f);//좌측상단
	m_vBladePos[4] = _vec3(0.7f,	0.f,	0.f);//오른쪽
	m_vBladePos[5] = _vec3(-0.7f,	0.f,	0.f);//왼쪽
	m_vBladePos[6] = _vec3(0.f,		0.f,	0.f);//가운데
	m_vBladePos[7] = _vec3(0.f,		0.f,	0.8f);//위
	m_vBladePos[8] = _vec3(0.f,		0.f,	-0.8f);//아래

	m_vBladeDir[0] = _vec3(0.f, -10.f, 0.f);//우측상단
	m_vBladeDir[1] = _vec3(0.f, 10.f, 0.f);//우측하단
	m_vBladeDir[2] = _vec3(0.f, -10.f, 0.f);//좌측하단
	m_vBladeDir[3] = _vec3(0.f, 10.f, 0.f);//좌측상단
	m_vBladeDir[4] = _vec3(0.f, 30.f, 0.f);//오른쪽
	m_vBladeDir[5] = _vec3(0.f, 30.f, 0.f);//왼쪽
	m_vBladeDir[6] = _vec3(0.f, 30.f, 0.f);//가운데

	m_bSpawnBlade = false;

	return S_OK;
}

_int CBlade_Trap::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_uint iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CBlade_Trap::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CBlade_Trap::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture();
	m_pCubeBf->Render_Buffer();
}

void CBlade_Trap::Create_Blade()
{
	Engine::CGameObject* pGameObject = nullptr;
	for (int i = 0; i < 9; ++i)
	{
		pGameObject = m_pTrapBlade = CBlade_Trap_Blade::Create(m_pGraphicDev, m_pTransform->m_vInfo[INFO_POS]);
		dynamic_cast<CBlade_Trap_Blade*>(m_pTrapBlade)->m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_vInfo[INFO_POS];
		dynamic_cast<CBlade_Trap_Blade*>(m_pTrapBlade)->m_pTransform->Translate(m_vBladePos[i]);
		if (i == 6)
			dynamic_cast<CBlade_Trap_Blade*>(m_pTrapBlade)->Set_Collider();
		if(i < 7)
			dynamic_cast<CBlade_Trap_Blade*>(m_pTrapBlade)->m_pTransform->Rotate(m_vBladeDir[i]);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	}
}

HRESULT CBlade_Trap::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_pCubeBf = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BladeTrapBody"));
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

CBlade_Trap* CBlade_Trap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBlade_Trap* pInstance = new CBlade_Trap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Blade_Trap_Body Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBlade_Trap::Free()
{
	__super::Free();
}
