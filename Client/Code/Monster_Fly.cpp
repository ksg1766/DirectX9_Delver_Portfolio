#include "..\Header\Monster_Fly.h"
#include "Export_Function.h"

CMonster_Fly::CMonster_Fly()
{
}

CMonster_Fly::CMonster_Fly(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev), m_vSavePos(_vec3(0.f,0.f,0.f))
{
}

CMonster_Fly::~CMonster_Fly()
{
}

HRESULT CMonster_Fly::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;

	return S_OK;
}

STATE CMonster_Fly::Update_State(const _float& fTimeDelta)
{
	
	Fly(fTimeDelta);

	return STATE::ROMIMG;
}

void CMonster_Fly::LateUpdate_State()
{
}

void CMonster_Fly::Render_State()
{
	cout << "Fly" << endl;
}

void CMonster_Fly::Fly(const _float& fTimeDelta)
{
	//srand((unsigned)time(NULL));

	const _float fMinDistance = 2.f;
	const _float fMaxDistance = 8.f;
	const _float fAmplitude = 8.f;

	_float X = m_vSavePos.x + (fAmplitude * cosf((_float)rand() / RAND_MAX * 2.f * D3DX_PI)) / 50.f;
	_float Y = m_vSavePos.y + (fAmplitude * -sinf((_float)rand() / RAND_MAX * 2.f * D3DX_PI)) / 50.f;
	_float Z = m_vSavePos.z + (fAmplitude * sinf((_float)rand() / RAND_MAX * 2.f * D3DX_PI)) / 100.f;

	_vec3 vDir = _vec3(X, Y, Z);

	D3DXVec3Normalize(&vDir, &vDir);

	m_vSavePos = vDir;

	CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;
	_vec3 vPlayerPos = pPlayerTransform->m_vInfo[INFO_POS];

	_vec3 vTargetPos = vPlayerPos + vDir * 40.f;

	_vec3 vDir2 = vTargetPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&vDir2, &vDir2);

	_float fMoveSpeed = 5.f;
	_float fMoveDistance = fMoveSpeed * fTimeDelta;

	_vec3 vNewPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + vDir2 * fMoveDistance;

	if (vNewPos.y < fMinDistance)
		vNewPos.y = fMinDistance;

	// 하늘의 최대 높이를 넘지 않도록 제한
	if (vNewPos.y > fMaxDistance)
		vNewPos.y = fMaxDistance;

	m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = vNewPos;

}

CMonster_Fly* CMonster_Fly::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CMonster_Fly* pState = new CMonster_Fly(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CMonster_Fly*>(pState);

		MSG_BOX("Create Fly Failed");
		return nullptr;
	}

	return pState;
}

void CMonster_Fly::Free()
{
	__super::Free();
}
