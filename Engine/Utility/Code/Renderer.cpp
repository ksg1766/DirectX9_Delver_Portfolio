#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
{
	
}

CRenderer::~CRenderer()
{
	Free();
}

void CRenderer::Add_RenderGroup(RENDERID eType, CGameObject * pGameObject)
{
	if (RENDER_END <= eType || nullptr == pGameObject)
		return;

	m_RenderGroup[eType].push_back(pGameObject);
	pGameObject->AddRef();
}

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	Render_Priority(pGraphicDev);
	Render_Nonalpha(pGraphicDev);
	Render_Alpha(pGraphicDev);
	Render_UI(pGraphicDev);

	Clear_RenderGroup();
}

void CRenderer::Clear_RenderGroup()
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_Object();
}
	
void CRenderer::Render_Nonalpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto iter : m_RenderGroup[RENDER_NONALPHA])
			iter->Render_Object();
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	//pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // 내가 그리려는 색
	//pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // 백 버퍼에 이미 그려져있던 색
	//pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	//m_RenderGroup[RENDER_ALPHA].sort([](CGameObject* pDst, CGameObject* pSrc)
	//	{ return pDst->Get_ViewZ() > pSrc->Get_ViewZ(); });

	for (auto iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render_Object();

	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9& pGraphicDev)
{
	//for (auto iter : m_RenderGroup[RENDER_UI])
	//	iter->Render_Object();

	UIManager()->Render_UI(pGraphicDev);
}

void CRenderer::Free()
{

	Clear_RenderGroup();
}
