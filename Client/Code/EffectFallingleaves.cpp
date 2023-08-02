#include "stdafx.h"
#include "..\Header\EffectFallingleaves.h"
#include "PoolManager.h"
#include "Effectleaf.h"

CEffectFallingleaves::CEffectFallingleaves(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectFallingleaves::~CEffectFallingleaves()
{
	Free();
}

HRESULT CEffectFallingleaves::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_LEAVES;

	_vec3 NewDir;
	CTempEffect::Get_RandomVector(&NewDir, &_vec3(-0.3f, -1.f, -0.3f), &_vec3(0.3f, 0.f, 0.3f));
	_float fMoveSpeed = CTempEffect::Get_RandomFloat(10.f, 20.f);

	for (_uint i = 0; i < 70; ++i)
	{
		CGameObject* pGameObject = CEffectleaf::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<CEffectleaf*>(pGameObject)->Change_Move(NewDir, fMoveSpeed);
		m_vecLeaf.push_back(pGameObject);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	}

	m_fChangeTime = CTempEffect::Get_RandomFloat(5.f, 10.f);

	return S_OK;
}

Engine::_int CEffectFallingleaves::Update_Object(const _float& fTimeDelta)
{
	if (m_bChange) {
		m_bChange = false;

		for (auto& iter : m_vecLeaf) {
			dynamic_cast<CEffectleaf*>(iter)->Set_ChangeMode(true);
		}
	}

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	m_fMoveTime += 1.f * fTimeDelta;

	if (m_fMoveTime > m_fChangeTime)
	{
		m_fMoveTime = 0.f;
		m_fChangeTime = CTempEffect::Get_RandomFloat(5.f, 10.f);

		_vec3 NewDir;
		CTempEffect::Get_RandomVector(&NewDir, &_vec3(-0.3f, -1.f, -0.3f), &_vec3(0.3f, 0.f, 0.3f));
		_float fMoveSpeed = CTempEffect::Get_RandomFloat(10.f, 20.f);
		for (auto& iter : m_vecLeaf)
		{
			dynamic_cast<CEffectleaf*>(iter)->Change_Move(NewDir, fMoveSpeed);
		}
	}

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectFallingleaves::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();

	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
}

void CEffectFallingleaves::Render_Object(void)
{

}

HRESULT CEffectFallingleaves::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CEffectFallingleaves::Free()
{
	CTempEffect::Free();
}

CEffectFallingleaves* CEffectFallingleaves::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectFallingleaves*	pInstance = new CEffectFallingleaves(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectFallingleaves Create Failed");
		return nullptr;
	}

	return pInstance;
}
