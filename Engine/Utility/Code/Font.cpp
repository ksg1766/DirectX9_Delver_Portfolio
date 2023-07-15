#include "Font.h"

CFont::CFont()
{
}

CFont::CFont(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CFont::CFont(const CComponent& rhs)
	: CComponent(rhs)
{
}

CFont::~CFont()
{

}


HRESULT CFont::Ready_Font()
{
	m_pFont = 0;
	m_dwAnchor = 0;
	m_rect = {0,0, 0, 0};
	m_FontColor = 0xffffffff;
	return S_OK;
}



void CFont::DrawText(const TCHAR* outputString, LONG x, LONG y)
{
	m_rect;
#ifdef UNICODE
	m_pFont->DrawTextW(0, outputString, -1, &m_rect, m_dwAnchor, m_FontColor);//출력문자열, null문자열, 텍스트 사각형, 사각형위치, 글꼴색
#else
	m_pFont->DrawTextA(0, outputString, -1, &m_rect, m_dwAnchor, m_FontColor);
#endif
}

CFont* CFont::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFont* pInstance = new CFont(pGraphicDev);

	if (FAILED(pInstance->Ready_Font()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Font_Component Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CFont::Clone(void)
{
	return new CFont(*this);
}

void CFont::Free()
{
	CComponent::Free();
}
