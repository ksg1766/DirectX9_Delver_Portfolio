#include "Boss_BatSwarm.h"
#include "Export_Function.h"


CBoss_BatSwarm::CBoss_BatSwarm(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CBoss_BatSwarm::CBoss_BatSwarm(const CBoss_BatSwarm& rhs)
	: Engine::CGameObject(rhs)
{
}

CBoss_BatSwarm::~CBoss_BatSwarm()
{
	Free();
}

HRESULT CBoss_BatSwarm::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::PROJECTTILE;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fFrame = 0.f;
	m_fRallyTime = 0.f;
	m_fAngle = 0.f;
	m_bTest = false;
	return S_OK;
}

_int CBoss_BatSwarm::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_uint iExit = __super::Update_Object(fTimeDelta);
	m_fFrame += 2.f * fTimeDelta * 2;
	m_fRallyTime += fTimeDelta;
	if (2.f < m_fFrame)
		m_fFrame = 0.f;
	if(2.5f < m_fRallyTime)
		Engine::EventManager()->DeleteObject(this);
	if (2.f < m_fRallyTime)
	{
		m_vDir = Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front()->m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&m_vDir,&m_vDir);
		m_pTransform->Translate((m_vDir*25.F)* fTimeDelta);
	}
	else if(2.f > m_fRallyTime)
		Move_to_Random(fTimeDelta);

		

	return iExit;
}

void CBoss_BatSwarm::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	m_pBillBoard->LateUpdate_Component();
}

void CBoss_BatSwarm::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pTexture->Render_Texture(_uint(m_fFrame));
	m_pBuffer->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBoss_BatSwarm::Move_to_NewPos(_vec3 _vPos,const _float& fTimeDelta)
{
	m_vDir = _vPos - m_pTransform->m_vInfo[INFO_POS];
}

void CBoss_BatSwarm::Move_to_Random(const _float& fTimeDelta)
{
	_float X = (m_fAngle * cosf((_float)rand() / RAND_MAX * 2.f * D3DX_PI));
	_float Y = (m_fAngle * sinf((_float)rand() / RAND_MAX * 2.f * D3DX_PI));
	_float Z = (m_fAngle * sinf((_float)rand() / RAND_MAX * 2.f * D3DX_PI));
	_vec3 vDir = _vec3(X, Y, Z);
	m_pTransform->m_vInfo[INFO_LOOK] = vDir - m_pTransform->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&vDir, &vDir);
	m_pTransform->Translate((vDir* m_fAngle)* fTimeDelta);
}

void CBoss_BatSwarm::Set_StartPos(_vec3 vStartPos)
{
	m_pTransform->m_vInfo[INFO_POS] = vStartPos;
}

void CBoss_BatSwarm::Set_MovePos(_vec3 vMovetPos)
{
	m_vMovePos = vMovetPos;
}

HRESULT CBoss_BatSwarm::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;
	
	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossBatSwarm"));
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

CBoss_BatSwarm* CBoss_BatSwarm::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss_BatSwarm* pInstance = new CBoss_BatSwarm(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Boss_BatSwarm Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBoss_BatSwarm::Free()
{
	__super::Free();
}
