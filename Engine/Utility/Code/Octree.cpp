#include "Export_Utility.h"
#include "OctreeNode.h"
#include "Frustum.h"
#include "Trap.h"

IMPLEMENT_SINGLETON(COctree)

COctree::COctree()
{
}

COctree::~COctree()
{
	Free();
}

HRESULT COctree::Ready_Octree()
{
    m_pOctreeRoot = BuildOctree(_vec3(0.f, Width * 0.5f, 0.f), Width * 0.5f, Depth_Limit);

    if (!m_pOctreeRoot)
        return E_FAIL;

    vector<CGameObject*>& vecStaticObject = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BLOCK);
    for(auto& iter : vecStaticObject)
        FindCurrentPosNode(iter->m_pTransform, m_pOctreeRoot);

    vector<CGameObject*>& vecTrapObject = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::TRAP);
    for (auto& iter : vecTrapObject)
        if(TRAPTAG::STRIKEDOWN != static_cast<CTrap*>(iter)->Get_TrapTag())
            FindCurrentPosNode(iter->m_pTransform, m_pOctreeRoot);

    vector<CGameObject*>& vecImmortalObjectEnv = SceneManager()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::IMMORTAL);
    for (auto& iter : vecImmortalObjectEnv)
        FindCurrentPosNode(iter->m_pTransform, m_pOctreeRoot);

    vector<CGameObject*>& vecImmortalObject = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::IMMORTAL);
    for (auto& iter : vecImmortalObject)
        FindCurrentPosNode(iter->m_pTransform, m_pOctreeRoot);

    m_pFrustum = new CFrustum;

	return S_OK;
}

_int COctree::Update_Octree()
{
    if (m_pFrustum)
        //if (m_pFrustum->Update_Frustum())
        FrustumCull(m_pOctreeRoot);

    return _int();
}

void COctree::Render_Octree(LPDIRECT3DDEVICE9 pGraphicDev)
{
    pGraphicDev->AddRef();
#ifdef _DEBUG
    /*pGraphicDev->SetFVF(VTXCOL::format);
    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pOctreeRoot->Render_OctreeNode(pGraphicDev);

   pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
   pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
   pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);*/
#endif

   m_pOctreeRoot->Render_OctreeNode(pGraphicDev);
}

COctreeNode* COctree::BuildOctree(_vec3 vCenter, _float fHalfWidth, _int depthLimit)
{
    if (depthLimit < 0)
        return nullptr;

    COctreeNode*    pOctNode = new COctreeNode();
    BoundingBox*    pBBox = pOctNode->GetBoundingBox();

    _vec3           vCorner[8];

    for (_uint i = 0; i < 8; ++i)
    {
        *((_float*)(&vCorner[i]) + 0) = ((i & 1) ? fHalfWidth : -fHalfWidth) + *((_float*)(&vCenter) + 0);
        *((_float*)(&vCorner[i]) + 1) = ((i & 4) ? fHalfWidth : -fHalfWidth) + *((_float*)(&vCenter) + 1);
        *((_float*)(&vCorner[i]) + 2) = ((i & 2) ? fHalfWidth : -fHalfWidth) + *((_float*)(&vCenter) + 2);
    }

    pOctNode->SetCorners(vCorner);
    pOctNode->SetPosition(vCenter);
    pBBox->SetRadius(fHalfWidth);

    _vec3           vOffset;
    _vec3           vChildCenter;
    _float          fStep = fHalfWidth * 0.5f;

    for (_uint iTree = 0; iTree < COctreeNode::m_iChild_Node_Count; ++iTree)
    {
        *((_float*)(&vOffset) + 0) = ((iTree & 1) ? fStep : -fStep);
        *((_float*)(&vOffset) + 1) = ((iTree & 4) ? fStep : -fStep);
        *((_float*)(&vOffset) + 2) = ((iTree & 2) ? fStep : -fStep);

        *((_float*)(&vChildCenter) + 0) = *((_float*)(&vOffset) + 0) + *((_float*)(&vCenter) + 0);
        *((_float*)(&vChildCenter) + 1) = *((_float*)(&vOffset) + 1) + *((_float*)(&vCenter) + 1);
        *((_float*)(&vChildCenter) + 2) = *((_float*)(&vOffset) + 2) + *((_float*)(&vCenter) + 2);

        pOctNode->AddChildNode(BuildOctree(vChildCenter, fStep, depthLimit - 1));
    }

#ifdef _DEBUG
    //pOctNode->InitBoundingBoxVisible();
#endif

    return pOctNode;
}

void COctree::FindCurrentPosNode(CTransform* pTransform, COctreeNode* const pNode) // static object Ãß°¡
{
    _vec3 vObjectPos = pTransform->m_vInfo[INFO_POS];
    _vec3 vNodePos = pNode->GetPosition();

    _float fRadius = pNode->GetBoundingBox()->GetRadius();
    //fRadius *= 2.f; // Loose Factor = 2.f

    _float fMin, fMax;

    for (_uint index = 0; index < 3; ++index)
    {
        fMin = *((_float*)(&vNodePos) + index) - fRadius;
        fMax = *((_float*)(&vNodePos) + index) + fRadius;

        if (*((_float*)(&vObjectPos) + index) < fMin || *((_float*)(&vObjectPos) + index) > fMax)
            return;
    }

    pNode->AddObject(pTransform->Get_Host());

    if (pNode->GetChildren().empty())
        return;

    for(_uint index = 0; index < COctreeNode::m_iChild_Node_Count; ++index)
        FindCurrentPosNode(pTransform, pNode->GetChildNode(index));
}

_int COctree::IsInFrustum(COctreeNode* pNode)
{
    _int    iSum = 0;

    if (!m_pFrustum->IsInSphere(pNode->GetPosition(), pNode->GetBoundingBox()->GetRadius()))
        return FRUSTUM_OUT;

    for (_int i = 0; i < 8; ++i)
        iSum += m_pFrustum->IsIn(pNode->GetCorner()[i]);

    if (8 == iSum)
        return FRUSTUM_COMPLETELY_IN;

    return FRUSTUM_PRIMARILLY_IN;
}

void COctree::FrustumCull(COctreeNode* pNode)
{
    switch (IsInFrustum(pNode))
    {
    case FRUSTUM_COMPLETELY_IN:
        pNode->CullNode(FRUSTUM_COMPLETELY_IN);
        return;
    case FRUSTUM_PRIMARILLY_IN:
        pNode->CullNode(FRUSTUM_PRIMARILLY_IN);
        break;
    case FRUSTUM_OUT:
        pNode->CullNode(FRUSTUM_OUT);
        return;
    }

    vector<COctreeNode*>& vecChildren = pNode->GetChildren();

    if (!vecChildren.empty())
        for (_int i = 0; i < 8; ++i)
            FrustumCull(vecChildren[i]);
}

COctreeNode* COctree::GetParentNodeByPos(_vec3 vPos, COctreeNode* const pNode)
{
    // DFS
    if (pNode->IsInNode(vPos))
    {
        if (pNode->GetChildren().empty())
            return pNode->GetParent();
        else
        {
            for (_uint iIndex = 0; iIndex < COctreeNode::m_iChild_Node_Count; ++iIndex)
            {
                COctreeNode* pParent = GetParentNodeByPos(vPos, pNode->GetChildNode(iIndex));
                if (pParent)
                    return pParent;
            }

            return pNode->GetParent();
        }
    }
    else
        return nullptr;
}

void COctree::Free()
{
    Safe_Release(m_pOctreeRoot);
    Safe_Release(m_pFrustum);
}
