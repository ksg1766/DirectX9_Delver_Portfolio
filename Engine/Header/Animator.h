#pragma once
#include "Component.h"

BEGIN(Engine)

class CTexture;
class CAnimation;

class ENGINE_DLL CAnimator :
    public CComponent
{
private:
	explicit CAnimator();
	explicit CAnimator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimator(const CAnimator& rhs);
	virtual ~CAnimator();

public:
	HRESULT			Ready_Animator();
	virtual _int	Update_Animator(const _float& fTimeDelta);
	virtual void	LateUpdate_Component()	override;
	virtual void	Render_Animator();

public:
	void	Set_Animation(STATE	eState);

public:
	CAnimation* Get_Animation() { return m_pCurAnimation; }

public:
	HRESULT	Add_Animation(STATE _eState, CAnimation* pAnimation);


private:
	map<STATE, CAnimation*> m_mapAnimation;
	// 애니메이션을 맵에 STATE의 키값으로 저장한다.

	_float	m_fFrame;
	// 사용하지 않음(애니메이션이 들고 있음)
	STATE	m_eCurState;
	// 현재 애니메이터의 스테이트

	CAnimation* m_pCurAnimation;
	// 현재 애니메이션의 포인트.
	
	

public:
	static	CAnimator* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
private:
	virtual void			Free();
};

END