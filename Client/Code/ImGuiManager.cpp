#include "..\Header\ImGuiManager.h"
#include "stdafx.h"

#include "CameraManager.h"
#include "FlyingCamera.h"
#include "CubeBlock.h"
#include "SpawningPool.h"
#include "StrikeDown_Trap_Body.h"
#include "Blade_Trap_Body.h"
#include "Jump_Plate.h"

#include "Tree.h"
#include "Rock.h"
#include "Grass.h"
#include "Mushroom.h"
#include "Pumpkin.h"
#include "ImmortalSprite.h"

#include "BoxCube.h"
#include "EquipBox.h"

IMPLEMENT_SINGLETON(CImGuiManager)

CImGuiManager::CImGuiManager()
	:bUI_OnOff(true)
{

}

CImGuiManager::~CImGuiManager()
{
    
}

void CImGuiManager::Key_Input(const _float& fTimeDelta)
{
    _vec3 vOut;
    if (Engine::InputDev()->Mouse_Down(DIM_LB))
    {
        if (!ImGui::IsMousePosValid())
            return;

        if (!ImGui::GetIO().WantCaptureMouse)
        {
            if (MAP == m_eToolMode)
                vOut = PickingBlock();
            else if (SPAWNER == m_eToolMode)
            {
                if (1 == m_iPickingMode)
                    vOut = PickingBlock();
                else if (2 == m_iPickingMode)
                    vOut = PickingSpawner();
            }
            else if (TRAP == m_eToolMode)
            {
                if (1 == m_iPickingMode)
                    vOut = PickingBlock();
                else if (2 == m_iPickingMode)
                    vOut = PickingTrap();
            }
            else if (ENVIRONMENT == m_eToolMode)
            {
                if (1 == m_iPickingMode)
                    vOut = PickingBlock();
                else if (2 == m_iPickingMode)
                    vOut = PickingEnvironment();
            }
            else if (FRAGILE == m_eToolMode)
            {
                if (1 == m_iPickingMode)
                    vOut = PickingBlock();
                else if (2 == m_iPickingMode)
                    return;
            }

            if (_vec3(0.f, -10.f, 0.f) == vOut)
                return;
        }
        else
            return;

        Engine::CGameObject* pGameObject = nullptr;

        if (MAP == m_eToolMode)
        {
            if (selected_texture)
            {
                pGameObject = CCubeBlock::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                NULL_CHECK_RETURN(pGameObject);
                dynamic_cast<CCubeBlock*>(pGameObject)->Set_TextureNumber(selected_texture_index);
                pGameObject->m_pTransform->Translate(vOut);
                EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
            }
        }
        else if (SPAWNER == m_eToolMode)
        {
            pGameObject = CSpawningPool::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
            NULL_CHECK_RETURN(pGameObject);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_LifeCount(m_iSpawnerLife);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_SpawnTime(m_fSpawnTime);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_MonsterTag(m_eSpawnerTag);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_PoolCapacity(m_iSpawnCapacity);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_SpawnRadius(m_fSpawnRadius);
            pGameObject->m_pTransform->Translate(vOut);
            EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        }
        else if (TRAP == m_eToolMode)
        {
            switch (m_iSelected_index)
            {
            case 0:
                pGameObject = CBlade_Trap::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                dynamic_cast<CBlade_Trap*>(pGameObject)->Create_Blade();
                pGameObject->m_pTransform->Translate(_vec3(0.f, -0.8f, 0.f));
                break;

            case 1:
                pGameObject = CStrikeDown_Trap::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                pGameObject->m_pTransform->Translate(_vec3(0.f, 11.f, 0.f));
                break;

            case 2:
                pGameObject = CJump_Plate::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                pGameObject->m_pTransform->Translate(_vec3(0.0f, -0.85f, 0.0f));
                break;
            }

            NULL_CHECK_RETURN(pGameObject);
            pGameObject->m_pTransform->Translate(vOut);
            EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        }
        else if (ENVIRONMENT == m_eToolMode)
        {
            switch (m_iSelected_index)
            {
            case 0:
                pGameObject = CTree::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                dynamic_cast<CTree*>(pGameObject)->Set_TreeType(m_iType, m_iHeight);
                pGameObject->m_pTransform->Scale(_vec3(m_fScaleX, m_fScaleY, 9.f));
                pGameObject->m_pTransform->Translate(_vec3(0.f,m_fScaleY - 1.f, 0.f));
                break;

            case 1:
                pGameObject = CRock::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                dynamic_cast<CRock*>(pGameObject)->Set_RockNumber(m_iType);
                pGameObject->m_pTransform->Scale(_vec3(m_fScaleX, m_fScaleY, 9.f));
                pGameObject->m_pTransform->Translate(_vec3(0.f, m_fScaleY - 1.f, 0.f));
                break;

            case 2:
                pGameObject = CGrass::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                dynamic_cast<CGrass*>(pGameObject)->Set_GrassNumber(m_iType);
                pGameObject->m_pTransform->Scale(_vec3(m_fScaleX, m_fScaleY, 9.f));
                pGameObject->m_pTransform->Translate(_vec3(0.f, m_fScaleY - 1.f, 0.f));
                break;

            case 3:
                pGameObject = CMushroom::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                dynamic_cast<CMushroom*>(pGameObject)->Set_MushroomNumber(m_iType);
                pGameObject->m_pTransform->Scale(_vec3(m_fScaleX, m_fScaleY, 9.f));
                pGameObject->m_pTransform->Translate(_vec3(0.f, m_fScaleY - 1.f, 0.f));
                break;

            case 4:
                pGameObject = CPumpkin::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                dynamic_cast<CPumpkin*>(pGameObject)->Set_PumpkinNumber(m_iType);
                pGameObject->m_pTransform->Scale(_vec3(m_fScaleX, m_fScaleY, 9.f));
                pGameObject->m_pTransform->Translate(_vec3(0.f, m_fScaleY - 1.f, 0.f));
                break;
            case 5:
                pGameObject = CImmortalSprite::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                dynamic_cast<CImmortalSprite*>(pGameObject)->Set_SpriteNumber(m_iType);
                pGameObject->m_pTransform->Scale(_vec3(m_fScaleX, m_fScaleY, 9.f));
                pGameObject->m_pTransform->Translate(_vec3(0.f, m_fScaleY - 1.f, 0.f));
                break;
            }

            if (pGameObject)
            {
                pGameObject->m_pTransform->Translate(vOut);
                EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
            }
        }
        else if (FRAGILE == m_eToolMode)
        {
            switch (m_iSelected_index)
            {
            case 0:
                pGameObject = CBoxCube::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                break;

            case 1:
                pGameObject = CEquipBox::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                break;
            }

            if (pGameObject)
            {
                pGameObject->m_pTransform->Translate(vOut - _vec3(0.f, 0.5f, 0.f));
                EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
            }
        }
    }

    if (Engine::InputDev()->Key_Pressing(DIK_LCONTROL) && Engine::InputDev()->Mouse_Pressing(DIM_LB))
    {
        if (!ImGui::IsMousePosValid())
            return;

        if (!ImGui::GetIO().WantCaptureMouse)
        {
            if (MAP == m_eToolMode)
                vOut = PickingBlock();

            if (_vec3(0.f, -10.f, 0.f) == vOut)
                return;
        }
        else
            return;

        Engine::CGameObject* pGameObject = nullptr;

        if (MAP == m_eToolMode)
        {
            if (selected_texture)
            {
                pGameObject = CCubeBlock::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                NULL_CHECK_RETURN(pGameObject);
                dynamic_cast<CCubeBlock*>(pGameObject)->Set_TextureNumber(selected_texture_index);
                pGameObject->m_pTransform->Translate(vOut);
                EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
            }
        }
    }

    if (Engine::InputDev()->Key_Pressing(DIK_LCONTROL) && Engine::InputDev()->Key_Down(DIK_S))
        OnSaveData();

    if (Engine::InputDev()->Key_Pressing(DIK_LCONTROL) && Engine::InputDev()->Key_Down(DIK_L))
        OnLoadData();
}

_vec3 CImGuiManager::PickingBlock()
{
    if (0 == m_iPickingMode)
        return _vec3(0.f, -10.f, 0.f);

#pragma region Cube Picking
    const vector<CGameObject*>& vecBlock = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BLOCK);
   
    priority_queue<pair<_float, CCubeBlock*>, vector<pair<_float, CCubeBlock*>>, greater<pair<_float, CCubeBlock*>>> pq;

    POINT		ptMouse{};
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    _vec3		vMousePos;

    D3DVIEWPORT9		ViewPort;
    ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
    CGraphicDev::GetInstance()->Get_GraphicDev()->GetViewport(&ViewPort);

    // 뷰포트 -> 투영
    vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
    vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
    vMousePos.z = 0.f;

    // 투영 -> 뷰 스페이스
    _matrix		matProj;
    D3DXMatrixInverse(&matProj, 0, &dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()
        ->Get_CurrentCam())->Get_Camera()->Get_ProjMatrix());
    D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

    _vec3	vRayPos, vRayDir;

    vRayPos = _vec3(0.f, 0.f, 0.f);
    vRayDir = vMousePos - vRayPos;

    // 뷰 스페이스 -> 월드 스페이스
    _matrix		matView;
    D3DXMatrixInverse(&matView, 0, &dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()
        ->Get_CurrentCam())->Get_Camera()->Get_ViewMatrix());
    D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
    D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

    _bool IsPicked = false;

    for (auto& iter : vecBlock)
    {
        CCubeBlock* pCubeBlock = dynamic_cast<CCubeBlock*>(iter);

        _vec3 vRayPosWorld = vRayPos;
        _vec3 vRayDirWorld = vRayDir;

        // 월드 스페이스 -> 로컬 스페이스
        _matrix		matWorld;
        matWorld = pCubeBlock->m_pTransform->WorldMatrix();
        D3DXMatrixInverse(&matWorld, 0, &matWorld);
        D3DXVec3TransformCoord(&vRayPosWorld, &vRayPosWorld, &matWorld);
        D3DXVec3TransformNormal(&vRayDirWorld, &vRayDirWorld, &matWorld);

        const vector<_vec3>& pCubeVtxPos = pCubeBlock->LoadCubeVertex();
        const vector<INDEX32>& pCubeIdxPos = pCubeBlock->LoadCubeIndex();

        _float	fU = 0.f, fV = 0.f, fDist = 0.f;

        for (_ulong i = 0; i < pCubeIdxPos.size(); ++i)
        {
            if (D3DXIntersectTri(&pCubeVtxPos[pCubeIdxPos[i]._2],
                &pCubeVtxPos[pCubeIdxPos[i]._0],
                &pCubeVtxPos[pCubeIdxPos[i]._1],
                &vRayPosWorld, &vRayDirWorld, &fU, &fV, &fDist))
            {
                // V1 + U(V2 - V1) + V(V3 - V1)

                pq.push(make_pair(fDist, pCubeBlock));

                IsPicked = true;
                //break;

               /* return pCubeVtxPos[pCubeIdxPos[i]._1] + fU * (pCubeVtxPos[pCubeIdxPos[i]._0] - pCubeVtxPos[pCubeIdxPos[i]._1])
                    + fV * (pCubeVtxPos[pCubeIdxPos[i]._2] - pCubeVtxPos[pCubeIdxPos[i]._1]);*/
            }
        }
    }
#pragma endregion Cube Picking

    // 한 번 더 최종 큐브 피킹 평면 검출하는 구간인데, 비효율적으로 됐지만 우선 놔둠...

    if (pq.empty() && !IsPicked) // 피킹된 큐브가 없다면
        return _vec3(0.f, -10.f, 0.f);

    CCubeBlock* pFinalCube = pq.top().second;

    if (2 == m_iPickingMode)
    {
        EventManager()->DeleteObject(pFinalCube);
        return _vec3(0.f, -10.f, 0.f);
    }

    const vector<_vec3>&    pFinalCubeVtxPos = pFinalCube->LoadCubeVertex();
    const vector<INDEX32>&  pFinalCubeIdxPos = pFinalCube->LoadCubeIndex();
    vector <pair<_float, INDEX32>> vecFinalPlane;

    // 월드 스페이스 -> 로컬 스페이스
    _matrix		matWorld;
    matWorld = pFinalCube->m_pTransform->WorldMatrix();
    D3DXMatrixInverse(&matWorld, 0, &matWorld);
    D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
    D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

    _float	fU = 0.f, fV = 0.f, fDist = 0.f;

    for (_ulong i = 0; i < pFinalCubeIdxPos.size(); ++i)
    {
        if (D3DXIntersectTri(&pFinalCubeVtxPos[pFinalCubeIdxPos[i]._2],
            &pFinalCubeVtxPos[pFinalCubeIdxPos[i]._0],
            &pFinalCubeVtxPos[pFinalCubeIdxPos[i]._1],
            &vRayPos, &vRayDir, &fU, &fV, &fDist))
        {
            vecFinalPlane.push_back(make_pair(fDist, pFinalCubeIdxPos[i]));
        }
    }

    if (vecFinalPlane.empty() && !IsPicked)  // 한 번 더 피킹 검출
        return _vec3(0.f, -10.f, 0.f);

    INDEX32 finalIndex = (vecFinalPlane[0].first < vecFinalPlane[1].first) ? vecFinalPlane[0].second : vecFinalPlane[1].second;

    const _vec3& v0 = pFinalCubeVtxPos[finalIndex._0];  // Base
    const _vec3& v1 = pFinalCubeVtxPos[finalIndex._1];  // first
    const _vec3& v2 = pFinalCubeVtxPos[finalIndex._2];  // second

    _vec3 pOut;
    D3DXVec3Cross(&pOut, &(v1 - v0), &(v2 - v0));
    D3DXVec3Normalize(&pOut, &pOut);

    const _float epsilon = 0.001f;
    _vec3   vCenterPos = pFinalCube->Get_Collider()->GetCenterPos();
    _vec3*  vAxisDir = pFinalCube->Get_Collider()->GetAxisDir();
    _float* fAxisLen = pFinalCube->Get_Collider()->GetAxisLen();

    for (_uint i = 0; i < 3; ++i)
    {
        if (epsilon < fabs(D3DXVec3Dot(&vAxisDir[i], &pOut)))
            return vCenterPos + pOut * 2.f * fAxisLen[i];
    }

    return _vec3(0.f, -10.f, 0.f);
}

_vec3 CImGuiManager::PickingSpawner()
{
    _vec3   vFinalPos(0.f, -10.f, 0.f);

    _vec3	vRayPos, vRayDir;

    if (0 == m_iPickingMode)
        return vFinalPos;
    else
    {
        POINT		ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        _vec3		vMousePos;

        D3DVIEWPORT9		ViewPort;
        ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
        CGraphicDev::GetInstance()->Get_GraphicDev()->GetViewport(&ViewPort);

        // 뷰포트 -> 투영
        vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
        vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
        vMousePos.z = 0.f;

        // 투영 -> 뷰 스페이스
        _matrix		matProj;
        D3DXMatrixInverse(&matProj, 0, &dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()
            ->Get_CurrentCam())->Get_Camera()->Get_ProjMatrix());
        D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

        vRayPos = _vec3(0.f, 0.f, 0.f);
        vRayDir = vMousePos - vRayPos;

        // 뷰 스페이스 -> 월드 스페이스
        _matrix		matView;
        D3DXMatrixInverse(&matView, 0, &dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()
            ->Get_CurrentCam())->Get_Camera()->Get_ViewMatrix());
        D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
        D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

        _vec3 vRayPosWorld = vRayPos;
        _vec3 vRayDirWorld = vRayDir;

        if (1 == m_iPickingMode)
        {

        }
        else if (2 == m_iPickingMode)
        {
            vector<CGameObject*> vecSpawningPool = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::SPAWNINGPOOL);
            
            for (auto& iter : vecSpawningPool)
            {
                _vec3 vRayPosWorld = vRayPos;
                _vec3 vRayDirWorld = vRayDir;

                _matrix		matWorld;
                matWorld = iter->m_pTransform->WorldMatrix();
                D3DXMatrixInverse(&matWorld, 0, &matWorld);
                D3DXVec3TransformCoord(&vRayPosWorld, &vRayPosWorld, &matWorld);
                D3DXVec3TransformNormal(&vRayDirWorld, &vRayDirWorld, &matWorld);

                const vector<_vec3>& pSpawnerVtxPos = dynamic_cast<CSpawningPool*>(iter)->LoadSpawnerVertex();
                const vector<INDEX32>& pSpawnerIdxPos = dynamic_cast<CSpawningPool*>(iter)->LoadSpawnerIndex();

                _float	fU = 0.f, fV = 0.f, fDist = 0.f;

                for (_ulong i = 0; i < pSpawnerIdxPos.size(); ++i)
                {
                    if (D3DXIntersectTri(&pSpawnerVtxPos[pSpawnerIdxPos[i]._2],
                        &pSpawnerVtxPos[pSpawnerIdxPos[i]._0],
                        &pSpawnerVtxPos[pSpawnerIdxPos[i]._1],
                        &vRayPosWorld, &vRayDirWorld, &fU, &fV, &fDist))
                    {
                        EventManager()->DeleteObject(iter);
                        return _vec3(0.f, -10.f, 0.f);
                    }
                }
            }
            return _vec3(0.f, -10.f, 0.f);
        }
    }

    return vFinalPos;
}

_vec3 CImGuiManager::PickingTrap()
{
    _vec3   vFinalPos(0.f, -10.f, 0.f);

    _vec3	vRayPos, vRayDir;

    if (0 == m_iPickingMode)
        return vFinalPos;
    else
    {
        POINT		ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        _vec3		vMousePos;

        D3DVIEWPORT9		ViewPort;
        ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
        CGraphicDev::GetInstance()->Get_GraphicDev()->GetViewport(&ViewPort);

        // 뷰포트 -> 투영
        vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
        vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
        vMousePos.z = 0.f;

        // 투영 -> 뷰 스페이스
        _matrix		matProj;
        D3DXMatrixInverse(&matProj, 0, &dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()
            ->Get_CurrentCam())->Get_Camera()->Get_ProjMatrix());
        D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

        vRayPos = _vec3(0.f, 0.f, 0.f);
        vRayDir = vMousePos - vRayPos;

        // 뷰 스페이스 -> 월드 스페이스
        _matrix		matView;
        D3DXMatrixInverse(&matView, 0, &dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()
            ->Get_CurrentCam())->Get_Camera()->Get_ViewMatrix());
        D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
        D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

        _vec3 vRayPosWorld = vRayPos;
        _vec3 vRayDirWorld = vRayDir;

        if (1 == m_iPickingMode)
        {

        }
        else if (2 == m_iPickingMode)
        {
            vector<CGameObject*> vecTrap = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::TRAP);

            for (auto& iter : vecTrap)
            {
                _vec3 vRayPosWorld = vRayPos;
                _vec3 vRayDirWorld = vRayDir;

                _matrix		matWorld;
                matWorld = iter->m_pTransform->WorldMatrix();
                D3DXMatrixInverse(&matWorld, 0, &matWorld);
                D3DXVec3TransformCoord(&vRayPosWorld, &vRayPosWorld, &matWorld);
                D3DXVec3TransformNormal(&vRayDirWorld, &vRayDirWorld, &matWorld);

                const vector<_vec3>& pTrapVtxPos = dynamic_cast<CTrap*>(iter)->LoadTrapVertex();
                const vector<INDEX32>& pTrapIdxPos = dynamic_cast<CTrap*>(iter)->LoadTrapIndex();

                _float	fU = 0.f, fV = 0.f, fDist = 0.f;

                for (_ulong i = 0; i < pTrapIdxPos.size(); ++i)
                {
                    if (D3DXIntersectTri(&pTrapVtxPos[pTrapIdxPos[i]._2],
                        &pTrapVtxPos[pTrapIdxPos[i]._0],
                        &pTrapVtxPos[pTrapIdxPos[i]._1],
                        &vRayPosWorld, &vRayDirWorld, &fU, &fV, &fDist))
                    {
                        EventManager()->DeleteObject(iter);
                        return _vec3(0.f, -10.f, 0.f);
                    }
                }
            }
            return _vec3(0.f, -10.f, 0.f);
        }
    }

    return vFinalPos;
}

_vec3 CImGuiManager::PickingEnvironment()
{
    _vec3   vFinalPos(0.f, -10.f, 0.f);

    _vec3	vRayPos, vRayDir;

    if (0 == m_iPickingMode)
        return vFinalPos;
    else
    {
        POINT		ptMouse{};
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);

        _vec3		vMousePos;

        D3DVIEWPORT9		ViewPort;
        ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
        CGraphicDev::GetInstance()->Get_GraphicDev()->GetViewport(&ViewPort);

        // 뷰포트 -> 투영
        vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
        vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
        vMousePos.z = 0.f;

        // 투영 -> 뷰 스페이스
        _matrix		matProj;
        D3DXMatrixInverse(&matProj, 0, &dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()
            ->Get_CurrentCam())->Get_Camera()->Get_ProjMatrix());
        D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

        vRayPos = _vec3(0.f, 0.f, 0.f);
        vRayDir = vMousePos - vRayPos;

        // 뷰 스페이스 -> 월드 스페이스
        _matrix		matView;
        D3DXMatrixInverse(&matView, 0, &dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()
            ->Get_CurrentCam())->Get_Camera()->Get_ViewMatrix());
        D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
        D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

        _vec3 vRayPosWorld = vRayPos;
        _vec3 vRayDirWorld = vRayDir;

        if (1 == m_iPickingMode)
        {

        }
        else if (2 == m_iPickingMode)
        {
            vector<CGameObject*> vecTrap = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::IMMORTAL);

            for (auto& iter : vecTrap)
            {
                _vec3 vRayPosWorld = vRayPos;
                _vec3 vRayDirWorld = vRayDir;

                _matrix		matWorld;
                matWorld = iter->m_pTransform->WorldMatrix();
                D3DXMatrixInverse(&matWorld, 0, &matWorld);
                D3DXVec3TransformCoord(&vRayPosWorld, &vRayPosWorld, &matWorld);
                D3DXVec3TransformNormal(&vRayDirWorld, &vRayDirWorld, &matWorld);

                const vector<_vec3>& pEnvVtxPos = dynamic_cast<CEnvironment*>(iter)->LoadEnvVertex();
                const vector<INDEX32>& pEnvIdxPos = dynamic_cast<CEnvironment*>(iter)->LoadEnvIndex();

                _float	fU = 0.f, fV = 0.f, fDist = 0.f;

                for (_ulong i = 0; i < pEnvIdxPos.size(); ++i)
                {
                    if (D3DXIntersectTri(&pEnvVtxPos[pEnvIdxPos[i]._2],
                        &pEnvVtxPos[pEnvIdxPos[i]._0],
                        &pEnvVtxPos[pEnvIdxPos[i]._1],
                        &vRayPosWorld, &vRayDirWorld, &fU, &fV, &fDist))
                    {
                        EventManager()->DeleteObject(iter);
                        return _vec3(0.f, -10.f, 0.f);
                    }
                }
            }
            return _vec3(0.f, -10.f, 0.f);
        }
    }

    return vFinalPos;
}

HRESULT CImGuiManager::SetUp_ImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	//ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(Engine::CGraphicDev::GetInstance()->Get_GraphicDev());

    // resources
    CTexture* pCubeTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Tile"));
    m_pCubeTexture = pCubeTexture->Get_TextureList();

    m_iPickingMode = 0;
	
    return S_OK;
}

_int CImGuiManager::Update_ImGui(const _float& fTimeDelta)
{
    Key_Input(fTimeDelta);

    return 0;
}

void CImGuiManager::LateUpdate_ImGui()
{
    if (!bUI_OnOff) return;

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // demo
//#pragma region demo
//
//    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//    if (show_demo_window)
//        ImGui::ShowDemoWindow(&show_demo_window);
//
//    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
//    {
//        static float f = 0.0f;
//        static int counter = 0;
//
//        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//
//        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//        ImGui::Checkbox("Another Window", &show_another_window);
//
//        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//            counter++;
//        ImGui::SameLine();
//        ImGui::Text("counter = %d", counter);
//
//        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//        ImGui::End();
//    }
//
//    // 3. Show another simple window.
//    if (show_another_window)
//    {
//        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
//        ImGui::Text("Hello from another window!");
//        if (ImGui::Button("Close Me"))
//            show_another_window = false;
//        ImGui::End();
//    }
//    
//#pragma endregion demo

    _bool map_tool_window = true;

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    if (map_tool_window)
    {
        ImGui::Begin("Tool", &map_tool_window);
        
        _vec3 vCameraPos = CCameraManager::GetInstance()->Get_CurrentCam()->m_pTransform->m_vInfo[INFO_POS];

        ImGui::Text("Camera Pos");
        ImGui::Text("x : %.2f y : %.2f z : %.2f", vCameraPos.x, vCameraPos.y, vCameraPos.z);

        if (ImGui::TreeNode("Tool"))
        {
            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
            if (ImGui::BeginTabBar("Tool", tab_bar_flags))
            {
                if (ImGui::BeginTabItem("Block"))
                {
                    m_eToolMode = MAP;
                    //m_iPickingMode = 0;
                    ImGuiIO& io = ImGui::GetIO();

                    //ImTextureID         TerrainTextureID = m_pTerainTexture[0];
                    //LPDIRECT3DTEXTURE9  TerrainTexture = nullptr;
                    //ImTextureID selected_texture = nullptr;
                    ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
                    ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
                    ImVec2 uv1 = ImVec2(1.0f, 1.0f);                          // UV coordinates for (32,32) in our texture
                    ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
                    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint

                    ImGui::Image(selected_texture, ImVec2(96.0f, 96.0f), uv0, uv1, tint_col, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                    ImGui::SameLine();

                    if (ImGui::Button("Mode"))
                        ++m_iPickingMode %= 3;

                    ImGui::SameLine();
                    if (0 == m_iPickingMode)
                        ImGui::Text("None");
                    else if (1 == m_iPickingMode)
                        ImGui::Text("Draw");
                    else
                        ImGui::Text("Erase");

                    for (int i = 0; i < 10; i++)
                    {
                        for (int j = 0; j < 7; j++)
                        {
                            _int iIndex = 7 * i + j;
                            if (iIndex >= m_pCubeTexture.size())
                                break;

                            ImGui::PushID(iIndex);

                            if (ImGui::ImageButton("", m_pCubeTexture[iIndex], size))
                            {
                                selected_texture = m_pCubeTexture[iIndex];
                                selected_texture_index = iIndex;
                            }
                            ImGui::PopID();
                            ImGui::SameLine();
                        }
                        ImGui::NewLine();
                    }

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Spawner"))
                {
                    m_eToolMode = SPAWNER;
                    //m_iPickingMode = 0;
                    ImGuiIO& io = ImGui::GetIO();

                    const char* items[] = { "Spider", "Warrior", "Bat", "Wizard", "Alien", "Slime", "Skeleton", "SkullGhost", "Worm", "Monk" };
                    static _int item_current = 1;
                    ImGui::ListBox("MonsterList", &item_current, items, IM_ARRAYSIZE(items), 10);

                    m_eSpawnerTag = (MONSTERTAG)item_current;

                    ImGui::SliderFloat("Spawner Timer", &m_fSpawnTime, 1.f, 40.f, "%0.1f");
                    ImGui::SliderFloat("Spawner Radius", &m_fSpawnRadius, 1.f, 50.f, "%0.1f");
                    ImGui::SliderInt("Spawner LifeCount", &m_iSpawnerLife, 1, 20, "%d", ImGuiSliderFlags_Logarithmic);
                    ImGui::SliderInt("Spawner Capacity", &m_iSpawnCapacity, 1, 20, "%d", ImGuiSliderFlags_Logarithmic);

                    ImGui::NewLine();

                    if (ImGui::Button("Mode"))
                        ++m_iPickingMode %= 3;

                    if (0 == m_iPickingMode)
                        ImGui::Text("None");
                    else if (1 == m_iPickingMode)
                        ImGui::Text("Place");
                    else
                        ImGui::Text("Erase");

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Trap"))
                {
                    m_eToolMode = TRAP;
                    //m_iPickingMode = 0;
                    ImGuiIO& io = ImGui::GetIO();

                    const char* items[] = { "Blade", "StrikeDown", "Jump"};
                    static _int item_current = 1;
                    ImGui::ListBox("TrapList", &item_current, items, IM_ARRAYSIZE(items), 3);

                    m_iSelected_index = item_current;

                    ImGui::NewLine();

                    if (ImGui::Button("Mode"))
                        ++m_iPickingMode %= 3;

                    if (0 == m_iPickingMode)
                        ImGui::Text("None");
                    else if (1 == m_iPickingMode)
                        ImGui::Text("Place");
                    else
                        ImGui::Text("Erase");

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Environment"))
                {
                    m_eToolMode = ENVIRONMENT;
                    //m_iPickingMode = 0;
                    ImGuiIO& io = ImGui::GetIO();

                    const char* items[] = { "Tree", "Rock", "Grass", "Mushroom", "Pumpkin", "Etc"};
                    static _int item_current = 1;
                    ImGui::ListBox("EnvironmentList", &item_current, items, IM_ARRAYSIZE(items), 3);

                    m_iSelected_index = item_current;

                    ImGui::SliderInt("Type", &m_iType, 0, 19, "%d");
                    ImGui::SliderInt("Height", &m_iHeight, 0, 3, "%d");
                    ImGui::SliderFloat("ScaleX", &m_fScaleX, 1.f, 20.f, "%0.1f");
                    ImGui::SliderFloat("ScaleY", &m_fScaleY, 1.f, 20.f, "%0.1f");

                    ImGui::NewLine();

                    if (ImGui::Button("Mode"))
                        ++m_iPickingMode %= 3;

                    if (0 == m_iPickingMode)
                        ImGui::Text("None");
                    else if (1 == m_iPickingMode)
                        ImGui::Text("Place");
                    else
                        ImGui::Text("Erase");

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Fragile"))
                {
                    m_eToolMode = FRAGILE;
                    //m_iPickingMode = 0;
                    ImGuiIO& io = ImGui::GetIO();

                    const char* items[] = { "RandomBox", "EquipBox" };
                    static _int item_current = 1;
                    ImGui::ListBox("FragileList", &item_current, items, IM_ARRAYSIZE(items), 3);

                    m_iSelected_index = item_current;

                    ImGui::NewLine();

                    if (ImGui::Button("Mode"))
                        ++m_iPickingMode %= 3;

                    if (0 == m_iPickingMode)
                        ImGui::Text("None");
                    else if (1 == m_iPickingMode)
                        ImGui::Text("Place");
                    else
                        ImGui::Text("Erase");

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
            ImGui::Separator();
            ImGui::TreePop();
        }

        ImGui::End();
    }
}

void CImGuiManager::Render_ImGui(LPDIRECT3DDEVICE9 pGraphicDev)
{
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

HRESULT CImGuiManager::OnSaveData()
{
    CScene* pScene = SceneManager()->Get_Scene();

    HANDLE hFile = CreateFile(L"../Bin/Data/Sewer_TrapTest_v1.0.dat", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    //HANDLE hFile = CreateFile(L"../Bin/Data/TerrainGiantTree12.dat", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    if (INVALID_HANDLE_VALUE == hFile)
        return E_FAIL;

    OBJECTTAG eTag = OBJECTTAG::OBJECT_END;

    DWORD	dwByte = 0;
    _float  fX, fY, fZ;
    _ubyte  byTextureNumber = 0;

    for (int i = 0; i < (UINT)OBJECTTAG::OBJECT_END; ++i)
    {
        if (OBJECTTAG::BLOCK == (OBJECTTAG)i)
        {
            vector<CGameObject*>& vecObjList = pScene->Get_ObjectList(LAYERTAG::GAMELOGIC, (OBJECTTAG)i);
            for (auto& iter : vecObjList)
            {
                if (iter->m_pTransform->m_vInfo[INFO_POS].y < 0.9f)
                    continue;

                //dwStrByte = sizeof(CHAR) * (strlen(typeid(*iter).name()) + 1);
                eTag = iter->Get_ObjectTag();
                //CHAR* pName = nullptr;
                //strcpy(pName, typeid(iter).name());

                WriteFile(hFile, &eTag, sizeof(OBJECTTAG), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].x), sizeof(_float), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].y), sizeof(_float), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].z), sizeof(_float), &dwByte, nullptr);

                if (OBJECTTAG::BLOCK == (OBJECTTAG)i)
                {
                    byTextureNumber = dynamic_cast<CCubeBlock*>(iter)->Get_TextureNumber();
                    WriteFile(hFile, &byTextureNumber, sizeof(_ubyte), &dwByte, nullptr);
                }
            }
        }
        else if (OBJECTTAG::SPAWNINGPOOL == (OBJECTTAG)i)
        {
            MONSTERTAG      eMonsterTag = MONSTERTAG::MONSTER_END;  //
            _int            iSpawnerLife = 10;
            _int            iPoolCapacity = 5;
            _float          fSpawnRadius = 10.0f;
            _float          fSpawnTime = 10.0f;

            vector<CGameObject*>& vecObjList = pScene->Get_ObjectList(LAYERTAG::GAMELOGIC, (OBJECTTAG)i);
            for (auto& iter : vecObjList)
            {
                if (iter->m_pTransform->m_vInfo[INFO_POS].y < 0.f)
                    continue;

                eTag = iter->Get_ObjectTag();
                iSpawnerLife = dynamic_cast<CSpawningPool*>(iter)->Get_LifeCount();
                eMonsterTag = dynamic_cast<CSpawningPool*>(iter)->Get_MonsterTag();
                iPoolCapacity = dynamic_cast<CSpawningPool*>(iter)->Get_PoolCapacity();
                fSpawnRadius = dynamic_cast<CSpawningPool*>(iter)->Get_SpawnRadius();
                fSpawnTime = dynamic_cast<CSpawningPool*>(iter)->Get_SpawnTime();

                WriteFile(hFile, &eTag, sizeof(OBJECTTAG), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].x), sizeof(_float), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].y), sizeof(_float), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].z), sizeof(_float), &dwByte, nullptr);


                WriteFile(hFile, &eMonsterTag, sizeof(MONSTERTAG), &dwByte, nullptr);
                WriteFile(hFile, &iSpawnerLife, sizeof(_int), &dwByte, nullptr);
                WriteFile(hFile, &iPoolCapacity, sizeof(_int), &dwByte, nullptr);
                WriteFile(hFile, &fSpawnRadius, sizeof(_float), &dwByte, nullptr);
                WriteFile(hFile, &fSpawnTime, sizeof(_float), &dwByte, nullptr);
            }
        }
        else if (OBJECTTAG::TRAP == (OBJECTTAG)i)
        {
            vector<CGameObject*>& vecObjList = pScene->Get_ObjectList(LAYERTAG::GAMELOGIC, (OBJECTTAG)i);
            for (auto& iter : vecObjList)
            {
                if (iter->m_pTransform->m_vInfo[INFO_POS].y < -10000.f)
                    continue;

                eTag = iter->Get_ObjectTag();
                TRAPTAG eTrapTag = dynamic_cast<CTrap*>(iter)->Get_TrapTag();
                if (TRAPTAG::TRAP_END == eTrapTag)
                    continue;

                WriteFile(hFile, &eTag, sizeof(OBJECTTAG), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].x), sizeof(_float), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].y), sizeof(_float), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].z), sizeof(_float), &dwByte, nullptr);

                WriteFile(hFile, &eTrapTag, sizeof(TRAPTAG), &dwByte, nullptr);
            }
        }
        else if (OBJECTTAG::IMMORTAL == (OBJECTTAG)i)
        {
            vector<CGameObject*>& vecObjList = pScene->Get_ObjectList(LAYERTAG::GAMELOGIC, (OBJECTTAG)i);
            for (auto& iter : vecObjList)
            {
                if (iter->m_pTransform->m_vInfo[INFO_POS].y < -10000.f)
                    continue;

                eTag = iter->Get_ObjectTag();
                ENVIRONMENTTAG eEnvTag = dynamic_cast<CEnvironment*>(iter)->Get_EnvTag();
                if (ENVIRONMENTTAG::ENVIRONMENT_END == eEnvTag)
                    continue;

                WriteFile(hFile, &eTag, sizeof(OBJECTTAG), &dwByte, nullptr);

                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].x), sizeof(_uint), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].y), sizeof(_uint), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].z), sizeof(_uint), &dwByte, nullptr);

                _vec3 vLocalScale = iter->m_pTransform->LocalScale();
                WriteFile(hFile, &(vLocalScale.x), sizeof(_float), &dwByte, nullptr);
                WriteFile(hFile, &(vLocalScale.y), sizeof(_float), &dwByte, nullptr);
                WriteFile(hFile, &(vLocalScale.z), sizeof(_float), &dwByte, nullptr);

                WriteFile(hFile, &eEnvTag, sizeof(ENVIRONMENTTAG), &dwByte, nullptr);

                if (eEnvTag == ENVIRONMENTTAG::TREE)
                {
                    _uint iTreeNumber = dynamic_cast<CTree*>(iter)->Get_TreeNumber();
                    WriteFile(hFile, &iTreeNumber, sizeof(OBJECTTAG), &dwByte, nullptr);
                }
                else if (eEnvTag == ENVIRONMENTTAG::ROCK)
                {
                    _uint iRockNumber = dynamic_cast<CRock*>(iter)->Get_RockNumber();
                    WriteFile(hFile, &iRockNumber, sizeof(OBJECTTAG), &dwByte, nullptr);
                }
                else if (eEnvTag == ENVIRONMENTTAG::GRASS)
                {
                    _uint iGrassNumber = dynamic_cast<CGrass*>(iter)->Get_GrassNumber();
                    WriteFile(hFile, &iGrassNumber, sizeof(OBJECTTAG), &dwByte, nullptr);
                }
                else if (eEnvTag == ENVIRONMENTTAG::MUSHROOM)
                {
                    _uint iMushroomNumber = dynamic_cast<CMushroom*>(iter)->Get_MushroomNumber();
                    WriteFile(hFile, &iMushroomNumber, sizeof(OBJECTTAG), &dwByte, nullptr);
                }
                else if (eEnvTag == ENVIRONMENTTAG::PUMPKIN)
                {
                    _uint iPumpkinNumber = dynamic_cast<CPumpkin*>(iter)->Get_PumpkinNumber();
                    WriteFile(hFile, &iPumpkinNumber, sizeof(OBJECTTAG), &dwByte, nullptr);
                }
                else if (eEnvTag == ENVIRONMENTTAG::ETC)
                {
                    _uint iSpriteNumber = dynamic_cast<CImmortalSprite*>(iter)->Get_SpriteNumber();
                    WriteFile(hFile, &iSpriteNumber, sizeof(OBJECTTAG), &dwByte, nullptr);
                }
            }
        }
        else if (OBJECTTAG::FRAGILE == (OBJECTTAG)i)
        {
            vector<CGameObject*>& vecObjList = pScene->Get_ObjectList(LAYERTAG::GAMELOGIC, (OBJECTTAG)i);
            for (auto& iter : vecObjList)
            {
                if (iter->m_pTransform->m_vInfo[INFO_POS].y < -10000.f)
                    continue;

                eTag = iter->Get_ObjectTag();
                FRAGILETAG eFragileTag = dynamic_cast<CFragile*>(iter)->Get_FragileTag();
                if (FRAGILETAG::FRAGILE_END == eFragileTag)
                    continue;

                WriteFile(hFile, &eTag, sizeof(OBJECTTAG), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].x), sizeof(_float), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].y), sizeof(_float), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].z), sizeof(_float), &dwByte, nullptr);

                WriteFile(hFile, &eFragileTag, sizeof(FRAGILETAG), &dwByte, nullptr);
            }
        }
    }

    CloseHandle(hFile);

    return S_OK;
}

HRESULT CImGuiManager::OnLoadData()
{
    CScene* pScene = SceneManager()->Get_Scene();
    CLayer* pLayer = pScene->Get_Layer(LAYERTAG::GAMELOGIC);
    for (int i = 0; i < (UINT)OBJECTTAG::OBJECT_END; ++i)
    {
        vector<CGameObject*>& refObjectList = pLayer->Get_ObjectList((OBJECTTAG)i);
        for_each(refObjectList.begin(), refObjectList.end(), [&](CGameObject* pObj) { EventManager()->DeleteObject(pObj); });
        refObjectList.clear();
    }

    //HANDLE hFile = CreateFile(L"../Bin/Data/TerrainGiantTree12.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    HANDLE hFile = CreateFile(L"../Bin/Data/Sewer_TrapTest_v1.0.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (INVALID_HANDLE_VALUE == hFile)
        return E_FAIL;

    OBJECTTAG eTag = OBJECTTAG::OBJECT_END;

    DWORD	dwByte = 0;
    _float  fX = 0.f, fY = 0.f, fZ = 0.f;
    _ubyte  byTextureNumber = 0;

    MONSTERTAG      eSpawnerTag = MONSTERTAG::MONSTER_END;  //
    _int            iSpawnerLife = 10;
    _int            iPoolCapacity = 5;
    _float          fSpawnRadius = 10.0f;
    _float          fSpawnTime = 10.0f;

    while (true)
    {
        // key 값 저장
        ReadFile(hFile, &eTag, sizeof(OBJECTTAG), &dwByte, nullptr);

        if (0 == dwByte)
            break;

        // if문 추가
        if (OBJECTTAG::BLOCK == eTag)
        {
            ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fY, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

            ReadFile(hFile, &byTextureNumber, sizeof(_ubyte), &dwByte, nullptr);
            
            if (0 == dwByte)
                break;

            if (fY < 0.f)
                continue;
            // value값 저장

            CGameObject* pGameObject = CCubeBlock::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
            NULL_CHECK_RETURN(pGameObject, E_FAIL);
            dynamic_cast<CCubeBlock*>(pGameObject)->Set_TextureNumber(byTextureNumber);
            pGameObject->m_pTransform->Translate(_vec3(fX, fY, fZ));
            pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
            //EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        }
        else if (OBJECTTAG::SPAWNINGPOOL == eTag)
        {
            // value값 저장
            ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fY, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

            ReadFile(hFile, &eSpawnerTag, sizeof(MONSTERTAG), &dwByte, nullptr);
            ReadFile(hFile, &iSpawnerLife, sizeof(_int), &dwByte, nullptr);
            ReadFile(hFile, &iPoolCapacity, sizeof(_int), &dwByte, nullptr);
            ReadFile(hFile, &fSpawnRadius, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fSpawnTime, sizeof(_float), &dwByte, nullptr);

            if (0 == dwByte)
                break;

            CGameObject* pGameObject = CSpawningPool::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
            NULL_CHECK_RETURN(pGameObject, E_FAIL);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_MonsterTag(eSpawnerTag);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_LifeCount(iSpawnerLife);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_PoolCapacity(iPoolCapacity);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_SpawnRadius(fSpawnRadius);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_SpawnTime(fSpawnTime);
            pGameObject->m_pTransform->Translate(_vec3(fX, fY, fZ));
            pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
            //EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        }
        else if (OBJECTTAG::TRAP == eTag)
        {
            // value값 저장
            ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fY, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

            TRAPTAG eTrapTag;
            ReadFile(hFile, &eTrapTag, sizeof(TRAPTAG), &dwByte, nullptr);

            if (0 == dwByte)
                break;

            CGameObject* pGameObject = nullptr;

            switch(eTrapTag)
            {
            case TRAPTAG::BLADE:
                pGameObject = CBlade_Trap::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                dynamic_cast<CBlade_Trap*>(pGameObject)->Create_Blade();
                break;

            case TRAPTAG::STRIKEDOWN:
                pGameObject = CStrikeDown_Trap::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                break;

            case TRAPTAG::JUMP:
                pGameObject = CJump_Plate::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                break;
            }
            //pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3(fX, fY, fZ);
            NULL_CHECK_RETURN(pGameObject, E_FAIL);
            pGameObject->m_pTransform->Translate(_vec3(fX, fY, fZ));
            pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
            //EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        }
        else if (OBJECTTAG::IMMORTAL == eTag)
        {
            // value값 저장
            ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fY, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

            _float  fCX = 0.f, fCY = 0.f, fCZ = 0.f;
            ReadFile(hFile, &fCX, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fCY, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fCZ, sizeof(_float), &dwByte, nullptr);

            ENVIRONMENTTAG eEnvTag;
            ReadFile(hFile, &eEnvTag, sizeof(ENVIRONMENTTAG), &dwByte, nullptr);

            if (0 == dwByte)
                break;

            CGameObject* pGameObject = nullptr;

            switch (eEnvTag)
            {
            case ENVIRONMENTTAG::TREE:
            {
                _uint iTreeNumber = 0;

                ReadFile(hFile, &iTreeNumber, sizeof(_uint), &dwByte, nullptr);

                pGameObject = CTree::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                dynamic_cast<CTree*>(pGameObject)->Set_TreeNumber(iTreeNumber);
                break;
            }

            case ENVIRONMENTTAG::ROCK:
            {
                _uint iRockNumber = 0;

                ReadFile(hFile, &iRockNumber, sizeof(_uint), &dwByte, nullptr);

                pGameObject = CRock::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                dynamic_cast<CRock*>(pGameObject)->Set_RockNumber(iRockNumber);
                break;
            }   

            case ENVIRONMENTTAG::GRASS:
            {
                _uint iGrassNumber = 0;

                ReadFile(hFile, &iGrassNumber, sizeof(_uint), &dwByte, nullptr);

                pGameObject = CGrass::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                dynamic_cast<CGrass*>(pGameObject)->Set_GrassNumber(iGrassNumber);
                break;
            }

            case ENVIRONMENTTAG::MUSHROOM:
            {
                _uint iMushroomNumber = 0;

                ReadFile(hFile, &iMushroomNumber, sizeof(_uint), &dwByte, nullptr);

                pGameObject = CMushroom::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                dynamic_cast<CMushroom*>(pGameObject)->Set_MushroomNumber(iMushroomNumber);
                break;
            }

            case ENVIRONMENTTAG::PUMPKIN:
            {
                _uint iPumpkinNumber = 0;

                ReadFile(hFile, &iPumpkinNumber, sizeof(_uint), &dwByte, nullptr);

                pGameObject = CPumpkin::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                dynamic_cast<CPumpkin*>(pGameObject)->Set_PumpkinNumber(iPumpkinNumber);
                break;
            }

            case ENVIRONMENTTAG::ETC:
            {
                _uint iSpriteNumber = 0;

                ReadFile(hFile, &iSpriteNumber, sizeof(_uint), &dwByte, nullptr);

                pGameObject = CImmortalSprite::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                dynamic_cast<CImmortalSprite*>(pGameObject)->Set_SpriteNumber(iSpriteNumber);
                break;
            }
            }
            //pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3(fX, fY, fZ);
            NULL_CHECK_RETURN(pGameObject, E_FAIL);
            pGameObject->m_pTransform->Scale(_vec3(fCX, fCY, fCZ));
            pGameObject->m_pTransform->Translate(_vec3(fX, fY, fZ));
            pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
            //EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        }
        else if (OBJECTTAG::FRAGILE == eTag)
        {
            // value값 저장
            ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fY, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

            FRAGILETAG eFragileTag;
            ReadFile(hFile, &eFragileTag, sizeof(FRAGILETAG), &dwByte, nullptr);

            if (0 == dwByte)
                break;

            CGameObject* pGameObject = nullptr;

            switch (eFragileTag)
            {
            case FRAGILETAG::RANDOMBOX:
            {
                pGameObject = CBoxCube::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                break;
            }
            case FRAGILETAG::EQUIPBOX:
            {
                pGameObject = CEquipBox::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                break;
            }
            }
            NULL_CHECK_RETURN(pGameObject, E_FAIL);

            pGameObject->m_pTransform->Translate(_vec3(fX, fY, fZ));
            pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
            //EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        }
    }

    CloseHandle(hFile);
    return S_OK;
}

void CImGuiManager::Free()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}