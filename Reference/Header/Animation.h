#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CAnimator;
class CTexture;

class ENGINE_DLL CAnimation : public CBase 
{
protected:
	explicit CAnimation();
	explicit CAnimation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimation(const CAnimation& rhs);
	virtual	~CAnimation();

public:
	virtual	HRESULT	Ready_Animation(CTexture* pTexture,
		STATE _eState, _float _fFrame, _bool Loop);
	virtual STATE	Update_Animation(const _float& fTimeDelta);
	virtual void	LateUpdate_Animation();
	virtual void	Render_Animation();

	virtual _float	Get_Frame() { return m_fCurFrame; }
	void	Set_Frame(_float _fFrame) { m_fCurFrame = _fFrame; }

public:


public:
	void	Set_Loop(_bool _loop)		{ m_bIsLoop = _loop; }
	void	Set_End(_bool _end)			{ m_bIsEnd = _end; }
	_bool	Get_Loop()					{ return m_bIsLoop; }
	_bool	Get_End()					{ return m_bIsEnd; }

	STATE	  Get_State()					   { return m_eState; }
	CTexture* Get_CurTexture()				   { return m_pCurAnimation; }
	CTexture* Get_PrevTexture()				   { return m_pPrevAnmation; }
	LPDIRECT3DTEXTURE9* Get_GhostiongTexture() { return m_pGhostingRenderTarget; }
	IDirect3DSurface9* Get_OriginalTarget()	   { return m_pOriginalRenderTarget; }

protected:
	// 애니메이션 관련 함수
	CAnimator* m_pOwner = nullptr;
	// 누가 갖고 있는지, (현재 사용 안하는 중)
	CTexture* m_pCurAnimation = nullptr;
	CTexture* m_pPrevAnmation = nullptr;
	LPDIRECT3DTEXTURE9* m_pGhostingRenderTarget = nullptr;
	IDirect3DSurface9* m_pOriginalRenderTarget = nullptr;
	// 현재 어떤 텍스쳐를 쥐고 있는 지
	_float	m_fFrameSpeed;
	// 프레임 누적시간 속도.
	_float	m_fCurFrame;
	// 현재 프레임.

	_bool	m_bIsLoop;
	// 애니메이션을 루프할건지
	_bool	m_bIsEnd;
	// 애니메이션을 종료할건지

	STATE	m_eState = STATE::STATE_END;

	LPDIRECT3DDEVICE9	m_pGraphicDev;
	// CBase를 상속 받고 있어서 그래픽 디바이스 정보를 따로 받아야 함.

public:
	static	CAnimation* Create(LPDIRECT3DDEVICE9 pGraphicDev, CTexture* pTexture,
		STATE _eState, _float _fFrame, _bool Loop);
	// 애니메이션을 생성할 때, 
	// 1. 디바이스 정보
	// 2. 현재 텍스쳐정보(Add Component 단계에서 텍스쳐 배열을 STATE 상태로 생성하면 됨.)
	// 3. 프레임을 돌릴 속도
	// 4. Loop 시킬건지, 안할건지 여부 

protected:
	virtual void Free();
};


END
