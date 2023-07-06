#include "..\Header\DungeonSpider.h"
#include "Export_Function.h"
#include "Terrain.h"

CDungeonSpider::CDungeonSpider(LPDIRECT3DDEVICE9 pGrapicDev)
	: Engine::CMonster(pGrapicDev), m_fFrame(0.f)
{
}

CDungeonSpider::CDungeonSpider(const CDungeonSpider& rhs)
	: Engine::CMonster(rhs), m_fFrame(rhs.m_fFrame)
{
}

CDungeonSpider::~CDungeonSpider()
{
}

HRESULT CDungeonSpider::Ready_Object()
{
	Set_ObjectTag(OBJECTTAG::MONSTER);
	
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Translate(_vec3(2.f, 1.f, 5.f));

	m_bIsJumping = true;
	m_fJumpInitializeVelocity = 15.f;
	m_pAI->Set_Transform(m_pTransform);
	m_eState = STATE::Roming;
	m_fChase = 0.f;

	return S_OK;
}

_int CDungeonSpider::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	_float m_fSpeed = 0.f;

	m_fFrame += 5.f * fTimeDelta;

	if (12.f < m_fFrame)
		m_fFrame = 0.f;


	CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;
	NULL_CHECK_RETURN(pPlayerTransform, -1);

	_vec3	vPlayerPos;
	vPlayerPos = pPlayerTransform->m_vInfo[INFO_POS];



	if (m_eState == STATE::Roming && m_fChase < 10.f)
	{
		m_fSpeed = 5.f;
		m_fChase += fTimeDelta;
		ForceHeight(m_pTransform->m_vInfo[INFO_POS]);

		if (m_fChase >= 5.f)
		{
			m_eState = STATE::Jump;
			m_fChase = 0.f;
		}
	}
	
	if (m_eState == STATE::Jump)
	{
		m_fSpeed = 13.f;
		Jump(fTimeDelta);
		

		if (!m_bIsJumping)
		{
			m_eState = STATE::Roming;
			m_bIsJumping = true;
		}
	}

	

	m_pAI->Chase_Target(&vPlayerPos, fTimeDelta, m_fSpeed);


	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CDungeonSpider::LateUpdate_Object()
{
	__super::LateUpdate_Object();
}

void CDungeonSpider::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTexture->Render_Texture((_uint)m_fFrame);
	m_pBuffer->Render_Buffer();
		
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDungeonSpider::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Spider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pAI = dynamic_cast<CMonsterAI*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Monster_AI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::MONSTERAI, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CDungeonSpider::Jump(const _float& fTimeDelta)
{
	if (!m_bIsJumping)
		return;

	m_pTransform->m_vInfo[INFO_POS].y += m_fJumpInitializeVelocity * fTimeDelta;

	m_fJumpInitializeVelocity -= 0.5f * fTimeDelta * fTimeDelta * 3000.f;

	if (m_pTransform->m_vInfo[INFO_POS].y < 1.f)
	{
		m_bIsJumping = false;
		m_pTransform->m_vInfo[INFO_POS].y = 1.f;
		m_fJumpInitializeVelocity = 20.f;
	}
	
}

void CDungeonSpider::ForceHeight(_vec3 _vPos)
{
	_float x = (VTXCNTX * VTXITV / 2.f) + _vPos.x;
	_float z = (VTXCNTZ * VTXITV / 2.f) + _vPos.z;

	x /= (_float)VTXITV;
	z /= (_float)VTXITV;

	_int col = ::floorf(x);
	_int row = ::floorf(z);

	_vec3 A = m_pTerrain->LoadTerrainVertex()[row * VTXCNTX + col];
	_vec3 B = m_pTerrain->LoadTerrainVertex()[row * VTXCNTX + col + 1];
	_vec3 C = m_pTerrain->LoadTerrainVertex()[(row + 1) * VTXCNTX + col];
	_vec3 D = m_pTerrain->LoadTerrainVertex()[(row + 1) * VTXCNTX + col + 1];

	_float dx = x - col;
	_float dz = z - row;

	_float height;
	//c-d b-d cdb 
	if (dz < 1.0f - dx)
	{
		/*
		Lerp(_float _a, _float _b, _float _c)
		{
			return a - (a * t) + (b * t);
		}
		*/

		_vec3 uy = B - A;
		_vec3 vy = C - A;

		height = A.y + (uy.y * dx) + (vy.y * dz) + 1.f;
		m_pTransform->m_vInfo[INFO_POS].y = height;
	}// c-a b-a cba
	else
	{
		_vec3 uy = C - D;
		_vec3 vy = B - D;

		height = D.y + (uy.y * (1.f - dx)) + (vy.y * (1.f - dz)) + 1.f;
		m_pTransform->m_vInfo[INFO_POS].y = height;
	}
}

CDungeonSpider* CDungeonSpider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDungeonSpider* pInstance = new CDungeonSpider(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CDungeonSpider*>(pInstance);

		MSG_BOX("Spiedt Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CDungeonSpider::Free()
{
	__super::Free();
}
