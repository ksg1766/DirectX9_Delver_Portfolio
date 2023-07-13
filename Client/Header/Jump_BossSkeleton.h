#pragma once
#include "State.h"
class CJump_BossSkeleton :
    public CState
{
    private:
        explicit CJump_BossSkeleton();
        explicit CJump_BossSkeleton(LPDIRECT3DDEVICE9 pGraphicDev);
        virtual ~CJump_BossSkeleton();


    public:
        virtual HRESULT    Ready_State(CStateMachine * pOwner);
        virtual STATE        Update_State(const _float & fTimeDelta);
        virtual  void        LateUpdate_State();
        virtual  void        Render_State();

    public:
        virtual STATE    Key_Input(const _float & fTimeDelta) { return STATE(); }
    private:
        void    Jump(const _float & fTimeDelta);
    private:
        _vec3   m_vTargetPos;
        _vec3   m_vDir;

        _float  m_fSpeed;
        _float  m_fCool;
        _float  m_fJumpVelocity;

        _bool   m_bJump;
    public:
        static CJump_BossSkeleton* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine * pOwner);

    private:
        virtual void Free();
};

