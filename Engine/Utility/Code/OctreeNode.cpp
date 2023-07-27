#include "..\..\Header\OctreeNode.h"
#include "Export_Function.h"

COctreeNode::COctreeNode()
{
    m_pBoundBox = new BoundingBox;
}

COctreeNode::COctreeNode(LPDIRECT3DDEVICE9 pGraphicDev)
{
}

COctreeNode::COctreeNode(const CGameObject& rhs)
{
}

COctreeNode::~COctreeNode()
{
}

HRESULT COctreeNode::InitBoundingBoxVisible()
{
#ifdef _DEBUG
	FAILED_CHECK_RETURN(CGraphicDev::GetInstance()->Get_GraphicDev()->CreateVertexBuffer(8 * sizeof(VTXCOL), // 생성할 버퍼의 크기
		0, // 생성하고자 버텍스 버퍼의 종류(0인 경우 정적버퍼, D3DUSAGE_DYNAMIC 지정 시 동적 버퍼)
		VTXCOL::format,		// 버텍스의 속성 옵션
		D3DPOOL_MANAGED,	// 메모리 저장 방식(정적 버퍼인 경우 MANAGED)
		&m_pVB,				// 결과 값 컴객체
		NULL),
		E_FAIL);

	FAILED_CHECK_RETURN(CGraphicDev::GetInstance()->Get_GraphicDev()->CreateIndexBuffer(12 * sizeof(INDEX32), // 생성할 버퍼의 크기
		0, // 생성하고자 버텍스 버퍼의 종류(0인 경우 정적버퍼, D3DUSAGE_DYNAMIC 지정 시 동적 버퍼)
		D3DFMT_INDEX32,		// 버텍스의 속성 옵션
		D3DPOOL_MANAGED, // 메모리 저장 방식(정적 버퍼인 경우 MANAGED)
		&m_pIB,			// 결과 값 컴객체
		NULL),
		E_FAIL);

	m_pVB->AddRef();
	m_pIB->AddRef();

	VTXCOL* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_float fRadius = m_pBoundBox->GetRadius();
	// 전면
	pVertex[0].vPosition = { -m_fLooseFactor * fRadius, m_fLooseFactor * fRadius, -m_fLooseFactor * fRadius };
	pVertex[0].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	pVertex[1].vPosition = { m_fLooseFactor * fRadius, m_fLooseFactor * fRadius, -m_fLooseFactor * fRadius };
	pVertex[1].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	pVertex[2].vPosition = { m_fLooseFactor * fRadius, - m_fLooseFactor * fRadius, -m_fLooseFactor * fRadius };
	pVertex[2].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	pVertex[3].vPosition = { -m_fLooseFactor * fRadius, - m_fLooseFactor * fRadius, -m_fLooseFactor * fRadius };
	pVertex[3].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	// 후면
	pVertex[4].vPosition = { -m_fLooseFactor * fRadius, m_fLooseFactor * fRadius, m_fLooseFactor * fRadius };
	pVertex[4].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	pVertex[5].vPosition = { m_fLooseFactor * fRadius, m_fLooseFactor * fRadius, m_fLooseFactor * fRadius };
	pVertex[5].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	pVertex[6].vPosition = { m_fLooseFactor * fRadius, -m_fLooseFactor * fRadius, m_fLooseFactor * fRadius };
	pVertex[6].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	pVertex[7].vPosition = { -m_fLooseFactor * fRadius, -m_fLooseFactor * fRadius, m_fLooseFactor * fRadius };
	pVertex[7].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// X+
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// X-
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	// Y+
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// Y-
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// Z+
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// Z-
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();
#endif
	return S_OK;
}

void COctreeNode::AddChildNode(COctreeNode* pChild)
{
    if (pChild)
    {
        m_vecChildren.push_back(pChild);
        pChild->SetParent(this);
    }
}

_bool COctreeNode::IsInNode(const _vec3 _vPos)
{
    _float fRadius = m_pBoundBox->GetRadius();
    //fRadius *= m_fLooseFactor; // Loose Factor = 2.f

    _float fMin, fMax;

    for (_uint index = 0; index < 3; ++index)
    {
        fMin = *((_float*)(&m_vPosition) + index) - fRadius;
        fMax = *((_float*)(&m_vPosition) + index) + fRadius;

        if (*((_float*)(&_vPos) + index) < fMin || *((_float*)(&_vPos) + index) > fMax)
            return FALSE;
    }

	return TRUE;
}

void COctreeNode::Render_OctreeNode(LPDIRECT3DDEVICE9 pGraphicDev)
{
	pGraphicDev->AddRef();
#ifdef _DEBUG
	//pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCOL));

	//pGraphicDev->SetIndices(m_pIB);

	//_matrix matOctreeNode;
	//D3DXMatrixIdentity(&matOctreeNode);
	//// Fixing Bounding Box
	//::CopyMemory(&matOctreeNode.m[3], &m_vPosition, sizeof(_vec3));

	//pGraphicDev->SetTransform(D3DTS_WORLD, &matOctreeNode);
	//pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	//if (!m_vecChildren.empty())
	//{
	//	for (auto& iter : m_vecChildren)
	//		iter->Render_OctreeNode(pGraphicDev);
	//}
#endif

	if (!m_vecChildren.empty())
	{
		for (auto& iter : m_vecChildren)
		{
			if (0 == iter->IsCulled())
				continue;
			else if (1 == iter->IsCulled())
				iter->Render_OctreeNode(pGraphicDev);
			else
			{
				for (auto& _iter : iter->GetObjectList())
				{
					_iter->Update_Object(0.02f);
				}
				//continue;
			}
		}
	}
}

void COctreeNode::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Delete(m_pBoundBox);

	if (!m_vecChildren.empty())
	{
		for (auto& iter : m_vecChildren)
		{
			iter->Free();
			Safe_Delete(iter);
		}
		m_vecObjects.clear();
		m_vecChildren.clear();
	}
}	