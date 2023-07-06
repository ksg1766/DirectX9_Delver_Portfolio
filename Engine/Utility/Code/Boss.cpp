#include "../../Header/Boss.h"

CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev), m_IsDead(false), m_eObjectTag(OBJECTTAG::MONSTER)
	,m_pBossState(nullptr)
{
}

CBoss::CBoss(const CBoss& rhs)
	: CMonster(rhs), m_IsDead(rhs.m_IsDead), m_eObjectTag(OBJECTTAG::MONSTER)
	, m_pBossState(nullptr)
{
}

CBoss::~CBoss()
{
}

HRESULT CBoss::Ready_Object()
{



	return S_OK;
}

_int CBoss::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CBoss::Free(void)
{
	__super::Free();
}
