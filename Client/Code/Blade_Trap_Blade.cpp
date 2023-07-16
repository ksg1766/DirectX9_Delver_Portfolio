#include "Blade_Trap_Blade.h"
#include "Export_Function.h"
#include "Blade_Trap_Idle.h"
#include "Blade_Trap_Attack.h"
CBlade_Trap_Blade::CBlade_Trap_Blade(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBlade_Trap_Blade::CBlade_Trap_Blade(const CBlade_Trap_Blade& rhs)
	: Engine::CGameObject(rhs)
{
}

CBlade_Trap_Blade::~CBlade_Trap_Blade()
{
}

HRESULT CBlade_Trap_Blade::Ready_Object(void)
{
	//m_eObjectTag = OBJECTTAG::MONSTERBULLET; //¹ÌÁ¤
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	CState* pState = CBlade_Trap_Idle::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::IDLE, pState);

	pState = CBlade_Trap_Attack::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ATTACK, pState);
	
	m_pStateMachine->Set_State(STATE::IDLE);
	return S_OK;
}

_int CBlade_Trap_Blade::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_PRIORITY, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_uint iExit = __super::Update_Object(fTimeDelta);
	m_pStateMachine->Update_StateMachine(fTimeDelta);
	return iExit;
}

void CBlade_Trap_Blade::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CBlade_Trap_Blade::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();
}

HRESULT CBlade_Trap_Blade::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	//pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossFire"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pStateMachine = dynamic_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_State"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CBlade_Trap_Blade* CBlade_Trap_Blade::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBlade_Trap_Blade* pInstance = new CBlade_Trap_Blade(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Blade_Trap_Blade Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBlade_Trap_Blade::Free()
{
	__super::Free();
}
