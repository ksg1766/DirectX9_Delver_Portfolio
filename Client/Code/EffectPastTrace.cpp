#include "stdafx.h"
#include "..\Header\EffectPastTrace.h"
#include "Player.h"
#include "PoolManager.h"

CEffectPastTrace::CEffectPastTrace(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectPastTrace::~CEffectPastTrace()
{
	Free();
}

HRESULT CEffectPastTrace::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_PASTTRACE;

	m_fFrame = 0.f;

	m_fLife = 3.f;
	m_fSpeed = 20.f;

	m_fEffectScale = 1.f;
	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));

	return S_OK;
}

Engine::_int CEffectPastTrace::Update_Object(const _float& fTimeDelta)
{
	if (!m_bCreateSet) {
		m_bCreateSet = true;

		CPlayer* pPlayer = static_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());
		if (pPlayer != nullptr) {
			m_pPlayerInfo = *pPlayer->m_pTransform->m_vInfo;

			m_pTransform->Copy_RUL(&m_pPlayerInfo);
			m_pTransform->Rotate(ROT_Y, D3DXToRadian(90.f));
			m_pTransform->Rotate(ROT_X, D3DXToRadian(90.f));

			m_vecDir = pPlayer->m_pTransform->m_vInfo[INFO_LOOK];
			D3DXVec3Normalize(&m_vecDir, &m_vecDir);
		}

		m_bParent = true;
	}

	m_fCreatTime += 1.f * fTimeDelta;

	if (m_RandomSet) {
		if (m_fCreatTime > .05f) {
			m_fCreatTime = 0.f;

			for (_uint i = 0; i < 2; ++i)
			{
				CGameObject* pGameObject = CEffectPastTrace::Create(m_pGraphicDev);
				static_cast<CTempEffect*>(pGameObject)->Set_CreateSet(true);
				static_cast<CTempEffect*>(pGameObject)->Set_RandomSet(false);
				pGameObject->m_pTransform->Copy_RUL(&m_pPlayerInfo);
				static_cast<CEffectPastTrace*>(pGameObject)->Set_EffectDir(m_vecDir);

				if (i == 0) {
					pGameObject->m_pTransform->Translate(m_pTransform->m_vInfo[INFO_POS]);
					pGameObject->m_pTransform->Rotate(ROT_Y, D3DXToRadian(90.f));
				}
				else if (i == 1){
					pGameObject->m_pTransform->Translate(_vec3(m_pTransform->m_vInfo[INFO_POS].x - .1f, m_pTransform->m_vInfo[INFO_POS].y - .1f, m_pTransform->m_vInfo[INFO_POS].z));
					pGameObject->m_pTransform->Rotate(ROT_Y, D3DXToRadian(90.f));
					pGameObject->m_pTransform->Rotate(ROT_X, D3DXToRadian(90.f));
				}
				Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			}
		}

		m_pTransform->m_vInfo[INFO_POS] += m_vecDir * fTimeDelta * m_fSpeed;
	}

	m_fXScale += .05f * fTimeDelta * m_fSpeed;
	if (m_fXScale > m_fEffectScale)
		m_fXScale = m_fEffectScale;
	m_pTransform->Scale(_vec3(m_fEffectScale - m_fXScale, m_fEffectScale, m_fEffectScale));

	if (m_fTime > m_fLife || m_fFrame == m_fFinal && m_bAnimation && !m_bLoop)
	{
		//CPoolManager::GetInstance()->Delete_Object(this);
	}

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CEffectPastTrace::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();

}

void CEffectPastTrace::Render_Object(void)
{
	if (m_bParent)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CEffectPastTrace::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectPastTrace"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pBillBoardCom = static_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CEffectPastTrace::Free()
{
	CTempEffect::Free();
}

CEffectPastTrace* CEffectPastTrace::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectPastTrace*	pInstance = new CEffectPastTrace(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectPastTrace Create Failed");
		return nullptr;
	}

	return pInstance;
}
