#include "stdafx.h"
#include "..\Header\Player.h"

#include "Export_Function.h"
#include "Terrain.h"

// 임시 아이템
#include "TempItem.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}
CPlayer::CPlayer(const CPlayer& rhs)
	: Engine::CGameObject(rhs)
{

}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::PLAYER;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Translate (_vec3(0.f, 1.f, 0.f));
	//m_pTransform->Scale(_vec3( 1.f, 2.f, 1.f ));
	//m_fSpeed = 10.f;

	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);
	Key_Input(fTimeDelta);
	ForceHeight(m_pTransform->m_vInfo[INFO_POS]);

	return iExit;
}

void CPlayer::LateUpdate_Object(void)
{

	__super::LateUpdate_Object();
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pBuffer->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPlayer::Set_Terrain(CTerrain* _pCurrentTerrain)
{
	m_pTerrain = _pCurrentTerrain;
}

HRESULT CPlayer::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CPlayerBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_PlayerBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);
	
	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);
	
	for(int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	if (Engine::InputDev()->Key_Pressing(DIK_W))
		m_pTransform->Translate(m_fSpeed * fTimeDelta * m_pTransform->m_vInfo[INFO_LOOK]);

	if (Engine::InputDev()->Key_Pressing(DIK_S))
		m_pTransform->Translate(m_fSpeed * fTimeDelta * -m_pTransform->m_vInfo[INFO_LOOK]);

	if (Engine::InputDev()->Key_Pressing(DIK_A))
		m_pTransform->Translate(m_fSpeed * fTimeDelta * -m_pTransform->m_vInfo[INFO_RIGHT]);

	if (Engine::InputDev()->Key_Pressing(DIK_D))
		m_pTransform->Translate(m_fSpeed * fTimeDelta * m_pTransform->m_vInfo[INFO_RIGHT]);
	
	_long	dwMouseMove = 0;

	if(dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_X))
		m_pTransform->Rotate(ROT_Y, D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);

	if (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_Y))
		m_pTransform->Rotate(ROT_X, D3DXToRadian(dwMouseMove)* fTimeDelta * 3.f);

	if (Engine::InputDev()->Key_Down(DIK_1))
	{
		Engine::CGameObject* pGameObject = nullptr;
		pGameObject = CTempItem::Create(m_pGraphicDev);
		pGameObject->m_pTransform->Translate(m_pTransform->m_vInfo[INFO_POS] + _vec3(0.55f, 0.1f, 1.8f));
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	}

	// UI 단축키 추가
	if (Engine::InputDev()->Key_Down(DIK_I))
	{
		if (Engine::UIManager()->Set_InvenUse())
		{
			// 반환하는 값이 true, 즉 인벤을 열겠다는 의미이기에
			// 마우스를 따라 회전하던 카메라 멈추고 / 화면 가운데에 고정되어있는 fix를 해제해주시면 됩니다!
		}
		else
		{
			//ShowCursor(false);
			// 반대로 다시 카메라를 마우스에 따라 회전시키고 / 마우스 화면 가운데에 고정(fix = true)시키면 됩니다!
		}
	}
	else if (Engine::InputDev()->Key_Down(DIK_C))
	{
		if (Engine::UIManager()->Set_StatUse())
		{
			// I단축키에 써있는 내용이랑 전부 같습니다 똑같이 채워주시면 됩니다!
		}
		else
		{

		}
	}
	else if (Engine::InputDev()->Key_Down(DIK_M))
	{
		if (Engine::UIManager()->Set_MapUse())
		{

		}
		else
		{

		}
	}
	else if (Engine::InputDev()->Key_Down(DIK_ESCAPE))
	{
		if (Engine::UIManager()->Set_EscUse())
		{

		}
		else
		{

		}
	}
}

void CPlayer::ForceHeight(_vec3 _vPos)
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

void CPlayer::Free()
{
	__super::Free();
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pInstance;
}
