#include "Export_Utility.h"
#include "Trap.h"

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
	Remder_Effect(pGraphicDev);
	Render_Priority(pGraphicDev);

	/*D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = D3DCOLORVALUE{ 0.f, 1.f, 1.f, 1.f };
	light.Ambient = D3DXCOLOR(0.f, 1.f, 1.f, 1.f);
	_vec3 vDir(1.f, -1.f, -1.f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vDir);

	pGraphicDev->SetLight(0, &light);
	pGraphicDev->LightEnable(0, TRUE);
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	pGraphicDev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_ARGB(125, 255, 255, 255));
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

	mtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	mtrl.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	mtrl.Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	mtrl.Emissive = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	pGraphicDev->SetMaterial(&mtrl);*/

	Render_Nonalpha(pGraphicDev);
	Render_Alpha(pGraphicDev);
	//pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
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
	SCENETAG CurrentScene = SceneManager()->Get_Scene()->Get_SceneTag();

	if (SCENETAG::VILLAGE == CurrentScene)
	{
		_float fNear = 1.f;
		_float fFar  = 130.0f;

		pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
		pGraphicDev->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
		
		pGraphicDev->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(100, 60, 10, 55));

		pGraphicDev->SetRenderState(D3DRS_FOGSTART, *(DWORD*)&fNear);
		pGraphicDev->SetRenderState(D3DRS_FOGEND, *(DWORD*)&fFar);
	}
	else if (SCENETAG::STAGE == CurrentScene)
	{
		_float fNear = 1.f;
		_float fFar = 110.0f;

		//pGraphicDev->SetRenderState(D3DRS_FOGENABLE, FALSE);

		pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
		pGraphicDev->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
		// 안개 색상 설정
		//pGraphicDev->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(1, 100, 155, 180));
		pGraphicDev->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(1, 10, 25, 50));
		//_float fFar = 140.0f;
		pGraphicDev->SetRenderState(D3DRS_FOGSTART, *(DWORD*)&fNear);
		pGraphicDev->SetRenderState(D3DRS_FOGEND, *(DWORD*)&fFar);
	}

	for (auto iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_Object();
}
	
void CRenderer::Render_Nonalpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	for (auto iter : m_RenderGroup[RENDER_NONALPHA])
	{
		/*if (iter->Get_ObjectTag() == OBJECTTAG::TRAP)
			if (TRAPTAG::STRIKEDOWN != static_cast<CTrap*>(iter)->Get_TrapTag())
				continue;*/

		iter->Render_Object();
	}
	pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	//pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // 내가 그리려는 색 
	//pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // 백 버퍼에 이미 그려져있던 색
	//pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// 
	//pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000001);
	//m_RenderGroup[RENDER_ALPHA].sort([](CGameObject* pDst, CGameObject* pSrc)
	//	{ return pDst->Get_ViewZ() > pSrc->Get_ViewZ(); });

	for (auto iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render_Object();

	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	// 
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// 
	//pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Remder_Effect(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto iter : m_RenderGroup[RENDER_EFFECT])
		iter->Render_Object();
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9& pGraphicDev)
{
	UIManager()->Render_UI(pGraphicDev);
}

void CRenderer::Free()
{

	Clear_RenderGroup();
}
