#include "..\Header\SpiderWalk.h"
#include "Export_Function.h"

CSpiderWalk::CSpiderWalk()
{
}

CSpiderWalk::CSpiderWalk(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CState(pGraphicDev)
{
}

CSpiderWalk::~CSpiderWalk()
{
}

HRESULT CSpiderWalk::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_fChase = 0.f;
    return S_OK;
}

STATE CSpiderWalk::Update_State(const _float& fTimeDelta)
{
    _float fSpeed = 5.f;
    m_fChase += fTimeDelta;

    if (m_fChase >= 3.f)
    {
        m_fChase = 0.f;
        //return STATE::ATTACK;
    }

    _vec3 vLook = m_pOwner->Get_Transform()->m_vInfo[INFO_LOOK];
    _vec3 vRight = m_pOwner->Get_Transform()->m_vInfo[INFO_RIGHT];

    m_pOwner->Get_Transform()->Translate(10.f * fTimeDelta * vLook);

    return STATE::ROMIMG;
}

void CSpiderWalk::LateUpdate_State()
{
}

void CSpiderWalk::Render_State()
{
    cout << "Spider Walk" << endl;
}

CSpiderWalk* CSpiderWalk::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CSpiderWalk* pState = new CSpiderWalk(pGraphicDev);

    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CSpiderWalk*>(pState);

        MSG_BOX("Create SpiderWalk Failed");
        return nullptr;
    }

    return pState;
}

void CSpiderWalk::Free()
{
    __super::Free();
}
