#include "Export_Utility.h"
#include "Trap.h"

IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer() : m_bFogUse(false)
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
	Remder_Effect(pGraphicDev);
	Render_Priority(pGraphicDev);

	Render_Nonalpha(pGraphicDev);
	Render_Alpha(pGraphicDev);

	Render_Item(pGraphicDev);
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

void CRenderer::Remder_Effect(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto iter : m_RenderGroup[RENDER_EFFECT])
		iter->Render_Object();
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev)
{
	if (m_bFogUse)
	{
		pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
		pGraphicDev->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

		pGraphicDev->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(m_iFogColor[0], m_iFogColor[1], m_iFogColor[2], m_iFogColor[3]));
		pGraphicDev->SetRenderState(D3DRS_FOGSTART, *(DWORD*)&m_fFogNear);
		pGraphicDev->SetRenderState(D3DRS_FOGEND,   *(DWORD*)&m_fFogFar);
	}
	else
		pGraphicDev->SetRenderState(D3DRS_FOGENABLE, FALSE);

	for (auto iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_Object();
}
	
void CRenderer::Render_Nonalpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	if(SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
		pGraphicDev->SetRenderState(D3DRS_FOGENABLE, FALSE);

	pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	for (auto iter : m_RenderGroup[RENDER_NONALPHA])
	{
		iter->Render_Object();
	}
	pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000001);
	
	for (auto iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render_Object();

	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CRenderer::Render_Item(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000001);

	for (auto iter : m_RenderGroup[RENDER_ITEM])
		iter->Render_Object();

	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9& pGraphicDev)
{
	UIManager()->Render_UI(pGraphicDev);
}

void CRenderer::Free()
{
	Clear_RenderGroup();
}
