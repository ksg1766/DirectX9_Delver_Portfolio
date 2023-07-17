#include "..\..\Header\Animation.h"
#include "Texture.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), 
	m_pCurAnimation(nullptr), 
	m_fFrameSpeed(.0f), 
	m_fCurFrame(0.f),
	m_bIsEnd(false),
	m_bIsLoop(false)
{
	m_pGraphicDev->AddRef();
}

CAnimation::CAnimation(const CAnimation& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev),
	m_pOwner(rhs.m_pOwner),
	m_pCurAnimation(rhs.m_pCurAnimation),
	m_fFrameSpeed(rhs.m_fFrameSpeed),
	m_fCurFrame(rhs.m_fCurFrame),
	m_bIsEnd(rhs.m_bIsEnd),
	m_bIsLoop(rhs.m_bIsLoop),
	m_pPrevAnmation(rhs.m_pPrevAnmation),
	m_pGhostingRenderTarget(rhs.m_pGhostingRenderTarget),
	m_pOriginalRenderTarget(rhs.m_pOriginalRenderTarget)
{
}

CAnimation::~CAnimation()
{
	Free();
}

HRESULT CAnimation::Ready_Animation(CTexture* pTexture,
	STATE _eState, _float _fFrame, _bool Loop)
{

	if (pTexture == nullptr)
		return E_FAIL;

	m_pCurAnimation = pTexture;
	// 현재 텍스쳐를 받아온다
	m_eState = _eState;
	// 스테이트 머신과 함께 사용하기 위해 State로 키 값 받음.
	m_fFrameSpeed = _fFrame;
	// 스프라이트 돌릴 프레임 누적시간
	m_bIsLoop = Loop;
	// Loop를 돌릴건지 False로 바꾸면 스프라이트를 멈출 수 있게 만듦. End 불 값이 있다.


	return S_OK;
}

STATE CAnimation::Update_Animation(const _float& fTimeDelta)
{
	if (!m_bIsEnd)
	m_fCurFrame += m_fFrameSpeed * fTimeDelta;
	// 여기서 현재 프레임이 누적되고

	if (m_fCurFrame >= m_pCurAnimation->Get_TextureSize() && !m_bIsEnd) // 벡터 사이즈보다 프레임 사이즈가 커야 조건문 안으로 들어옴.
	{
		if (m_bIsLoop)
			m_fCurFrame = 0.f;
		// 들어왔는데 Loop 상태라는건 다시 스프라이트 이미지 돌리겠다는 뜻임.

		else
		{
			m_fCurFrame = static_cast<float>(m_pCurAnimation->Get_TextureSize() - 1);
			m_bIsEnd = true;
			// Loop가 아니면 여기로 들어와서 마지막 End에 고정 됨.
		}
	}

	if (m_pCurAnimation != nullptr)
		m_pPrevAnmation = m_pCurAnimation;

	return STATE::STATE_END; // 신경쓰지말 것 반환 아무도 안 받음.
}

void CAnimation::LateUpdate_Animation()
{
}

void CAnimation::Render_Animation()
{
	m_pCurAnimation->Render_Texture((_uint)m_fCurFrame);
	// 최종적으로 렌더가 여기서 완료 됨.
}

CAnimation* CAnimation::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTexture* pTexture, STATE _eState, _float _fFrame, _bool Loop)
{
	CAnimation* pAnimation = new CAnimation(pGraphicDev);

	if (FAILED(pAnimation->Ready_Animation(pTexture, _eState, _fFrame, Loop)))
	{
		Safe_Release<CAnimation*>(pAnimation);

		MSG_BOX("Create Animation Failed");
		return nullptr;
	}

	// 애니메이션을 생성할 때, 
	// 1. 디바이스 정보 -> 디바이스 정보는 생성자에 넘겨주고 나머진 Ready에 넘겨주세요
	
	// 2. 현재 텍스쳐정보(Add Component 단계에서 텍스쳐 배열을 STATE 상태로 생성하면 됨.)
	// 3. 현재 STATE 변수 
	// 4. 프레임을 돌릴 속도
	// 5. Loop 시킬건지, 안할건지 여부 

	return pAnimation;
}

void CAnimation::Free()
{
	Safe_Release<LPDIRECT3DDEVICE9>(m_pGraphicDev);
}
