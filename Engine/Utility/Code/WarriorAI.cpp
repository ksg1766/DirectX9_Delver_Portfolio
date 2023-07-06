#include "..\..\Header\WarriorAI.h"
#include "Transform.h"

CWarriorAI::CWarriorAI()
{
}

CWarriorAI::CWarriorAI(LPDIRECT3DDEVICE9 pGrahicDev)
	: Engine::CMonsterAI(pGrahicDev)
{
}

CWarriorAI::CWarriorAI(const CWarriorAI& rhs)
	: Engine::CMonsterAI(rhs)
{
}

CWarriorAI::~CWarriorAI()
{
}

HRESULT CWarriorAI::Ready_WarriorAI()
{

	return S_OK;
}

_int CWarriorAI::Update_Component(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Component(fTimeDelta);
	
	

	return iExit;
}

void CWarriorAI::Set_NewTargetPosition()
{
	srand((unsigned)time(NULL));

	m_vTargetPos.x = 15 * cosf((_float)rand() / 30 * (D3DX_PI * 2));
	m_vTargetPos.y = 1.f;
	m_vTargetPos.z = 15 * sinf((_float)rand() / 20 * (D3DX_PI * 2));
}

void CWarriorAI::Move_To_TargetPosition(const _float& fTimeDelta)
{
	_vec3 vCurrentPos = m_pTransform->m_vInfo[INFO_POS];
	_vec3 vDir = m_vTargetPos - vCurrentPos;

	D3DXVec3Normalize(&vDir, &vDir);

	_float fMoveSpeed = 5.f;

	_float fMoveDistance = fMoveSpeed * fTimeDelta;

	vCurrentPos += vDir * fMoveDistance;

	m_pTransform->m_vInfo[INFO_POS] = vCurrentPos;
}

_bool CWarriorAI::Reached_TargetPosition()
{
	_vec3 vCurrentPos = m_pTransform->m_vInfo[INFO_POS];
	_float fDistance = D3DXVec3Length(&(vCurrentPos - m_vTargetPos));

	return fDistance <= 4.0f;
}

CWarriorAI* CWarriorAI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWarriorAI* pAI = new CWarriorAI(pGraphicDev);

	if (FAILED(pAI->Ready_WarriorAI()))
	{
		Safe_Release<CWarriorAI*>(pAI);

		MSG_BOX("WarriorAI Create Failed");
		return nullptr;
	}

	return pAI;
}

CComponent* CWarriorAI::Clone()
{
	return new CWarriorAI(*this);
}

void CWarriorAI::Free()
{
	__super::Free();
}
