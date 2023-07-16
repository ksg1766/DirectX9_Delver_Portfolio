#include "..\Header\ImGuiManager.h"
#include "stdafx.h"

#include "CameraManager.h"
#include "FlyingCamera.h"
#include "Terrain.h"
#include "CubeBlock.h"
#include "SpawningPool.h"

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
                vOut = PickingSpawner();

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
            pGameObject->m_pTransform->Translate(vOut);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_SpawnTime(m_fSpawnTime);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_MonsterTag(m_eSpawnerTag);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_PoolCapacity(m_iSpawnCapacity);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_SpawnRadius(m_fSpawnRadius);
            EventManager()->CreateObject(pGameObject, LAYERTAG::ENVIRONMENT);

            // 옮기자
            /*LPD3DXMESH pShpere;
            LPD3DXBUFFER pShpereBuffer;
            D3DXCreateSphere(Engine::CGraphicDev::GetInstance()->Get_GraphicDev(), m_fSpawnRadius, 10, 10, &pShpere, &pShpereBuffer);
            m_vecShpere.push_back(make_pair(pShpere, pShpereBuffer));*/
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
    CTerrain* pTerrain = dynamic_cast<CTerrain*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::TERRAIN).front());

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

#pragma region Terrain Picking

    if(!IsPicked)
    {
        if (2 == m_iPickingMode)
            return _vec3(0.f, -10.f, 0.f);

        _vec3 vRayPosWorld = vRayPos;
        _vec3 vRayDirWorld = vRayDir;

        // 월드 스페이스 -> 로컬 스페이스
        _matrix		matWorld;
        matWorld = pTerrain->m_pTransform->WorldMatrix();
        D3DXMatrixInverse(&matWorld, 0, &matWorld);
        D3DXVec3TransformCoord(&vRayPosWorld, &vRayPosWorld, &matWorld);
        D3DXVec3TransformNormal(&vRayDirWorld, &vRayDirWorld, &matWorld);

        const vector<_vec3>& pTerrainVtxPos = pTerrain->LoadTerrainVertex();
        
        _float	fU = 0.f, fV = 0.f, fDist = 0.f;
        //
        _ulong		dwVtxIdx[3]{};

        for (_ulong i = 0; i < VTXCNTZ - 1; ++i)
        {
            for (_ulong j = 0; j < VTXCNTX - 1; ++j)
            {
                _ulong	dwIndex = i * VTXCNTX + j;

                // 오른쪽 위
                dwVtxIdx[0] = dwIndex + VTXCNTX;
                dwVtxIdx[1] = dwIndex + VTXCNTX + 1;
                dwVtxIdx[2] = dwIndex + 1;

                if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[1]],
                    &pTerrainVtxPos[dwVtxIdx[0]],
                    &pTerrainVtxPos[dwVtxIdx[2]],
                    &vRayPos, &vRayDir, &fU, &fV, &fDist))
                {
                    // V0 + U(V1 - V0) + V(V2 - V0)
                    _vec3 vFinalPos = _vec3(pTerrainVtxPos[dwVtxIdx[1]].x,
                        1.f,    // 1.f 는 Cube Radius
                        pTerrainVtxPos[dwVtxIdx[1]].z);

                    //if ((int)pTerrainVtxPos[dwVtxIdx[1]].x % 2)
                     //   vFinalPos.x += 1.f;

                    //if ((int)pTerrainVtxPos[dwVtxIdx[1]].z % 2)
                     //   vFinalPos.z += 1.f;

                    return vFinalPos;
                }

                // 왼쪽 아래
                dwVtxIdx[0] = dwIndex + VTXCNTX;
                dwVtxIdx[1] = dwIndex + 1;
                dwVtxIdx[2] = dwIndex;

                if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[1]],
                    &pTerrainVtxPos[dwVtxIdx[0]],
                    &pTerrainVtxPos[dwVtxIdx[2]],
                    &vRayPos, &vRayDir, &fU, &fV, &fDist))
                {
                    _vec3 vFinalPos = _vec3(pTerrainVtxPos[dwVtxIdx[1]].x,
                        1.f,    // 1.f 는 Cube Radius
                        pTerrainVtxPos[dwVtxIdx[1]].z);

                    //if ((int)pTerrainVtxPos[dwVtxIdx[1]].x % 2)
                    //    vFinalPos.x += 1.f;

                    //if ((int)pTerrainVtxPos[dwVtxIdx[1]].z % 2)
                     //   vFinalPos.z += 1.f;

                    return vFinalPos;
                }
            }
        }
    }

#pragma endregion Terrain Picking

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
        CTerrain* pTerrain = dynamic_cast<CTerrain*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::TERRAIN).front());

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
            // 월드 스페이스 -> 로컬 스페이스
            _matrix		matWorld;
            matWorld = pTerrain->m_pTransform->WorldMatrix();
            D3DXMatrixInverse(&matWorld, 0, &matWorld);
            D3DXVec3TransformCoord(&vRayPosWorld, &vRayPosWorld, &matWorld);
            D3DXVec3TransformNormal(&vRayDirWorld, &vRayDirWorld, &matWorld);

            const vector<_vec3>& pTerrainVtxPos = pTerrain->LoadTerrainVertex();

            _float	fU = 0.f, fV = 0.f, fDist = 0.f;
            //
            _ulong	dwVtxIdx[3]{};

            for (_ulong i = 0; i < VTXCNTZ - 1; ++i)
            {
                for (_ulong j = 0; j < VTXCNTX - 1; ++j)
                {
                    _ulong	dwIndex = i * VTXCNTX + j;

                    // 오른쪽 위
                    dwVtxIdx[0] = dwIndex + VTXCNTX;
                    dwVtxIdx[1] = dwIndex + VTXCNTX + 1;
                    dwVtxIdx[2] = dwIndex + 1;

                    if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[1]],
                        &pTerrainVtxPos[dwVtxIdx[0]],
                        &pTerrainVtxPos[dwVtxIdx[2]],
                        &vRayPos, &vRayDir, &fU, &fV, &fDist))
                    {
                        // V0 + U(V1 - V0) + V(V2 - V0)
                        vFinalPos = _vec3(pTerrainVtxPos[dwVtxIdx[1]].x,
                            1.f,    // 1.f 는 Cube Radius
                            pTerrainVtxPos[dwVtxIdx[1]].z);
                    }

                    // 왼쪽 아래
                    dwVtxIdx[0] = dwIndex + VTXCNTX;
                    dwVtxIdx[1] = dwIndex + 1;
                    dwVtxIdx[2] = dwIndex;

                    if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[1]],
                        &pTerrainVtxPos[dwVtxIdx[0]],
                        &pTerrainVtxPos[dwVtxIdx[2]],
                        &vRayPos, &vRayDir, &fU, &fV, &fDist))
                    {
                        vFinalPos = _vec3(pTerrainVtxPos[dwVtxIdx[1]].x,
                            1.f,    // 1.f 는 Cube Radius
                            pTerrainVtxPos[dwVtxIdx[1]].z);
                    }
                }
            }
        }
        else if (2 == m_iPickingMode)
        {
            vector<CGameObject*> vecSpawningPool = SceneManager()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::SPAWNINGPOOL);
            
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
    CTexture* pTerainTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Terrain"));
    m_pTerainTexture = pTerainTexture->Get_TextureList();
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
#pragma region demo

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
    
#pragma endregion demo

    _bool map_tool_window = true;

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    if (map_tool_window)
    {
        ImGui::Begin("Tool", &map_tool_window);
        
        ImGui::Text("Camera Pos");
        ImGui::Text("Mouse Pos");
        ImGui::Text("Texture Info");

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

                    for (int i = 0; i < 6; i++)
                    {
                        for (int j = 0; j < 5; j++)
                        {
                            _int iIndex = 5 * i + j;
                            if (iIndex >= m_pTerainTexture.size())
                                break;

                            ImGui::PushID(iIndex);

                            if (ImGui::ImageButton("", m_pTerainTexture[iIndex], size))
                            {
                                selected_texture = m_pTerainTexture[iIndex];
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

                    const char* items[] = { "Spider", "Warrior", "Bat", "Wizard", "Alien", "Slime", "Skeleton" };
                    static _int item_current = 1;
                    ImGui::ListBox("MonsterList", &item_current, items, IM_ARRAYSIZE(items), 7);

                    m_eSpawnerTag = (MONSTERTAG)item_current;

                    ImGui::SliderFloat("Spawner Timer", &m_fSpawnTime, 3.f, 30.f, "%0.1f");
                    ImGui::SliderFloat("Spawner Radius", &m_fSpawnRadius, 10.f, 50.f, "%0.1f");
                    ImGui::SliderInt("Spawner Capacity", &m_iSpawnCapacity, 4.0f, 20.0f, "%d", ImGuiSliderFlags_Logarithmic);

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

    HANDLE hFile = CreateFile(L"../Bin/Data/TempData.dat", GENERIC_WRITE, 0, 0,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    if (INVALID_HANDLE_VALUE == hFile)
        return E_FAIL;

    OBJECTTAG eTag = OBJECTTAG::OBJECT_END;

    DWORD	dwByte = 0;
    _float  fX, fY, fZ;
    _ubyte  byTextureNumber = 0;

    for (int i = 0; i < (UINT)OBJECTTAG::OBJECT_END; ++i)
    {
        if (OBJECTTAG::TERRAIN == (OBJECTTAG)i)
            continue;

        if (OBJECTTAG::BLOCK == (OBJECTTAG)i)
        {
            vector<CGameObject*>& vecObjList = pScene->Get_ObjectList(LAYERTAG::GAMELOGIC, (OBJECTTAG)i);
            for (auto& iter : vecObjList)
            {
                if (_vec3(0.00000000f, 0.00000000f, -1.99999988f) == iter->m_pTransform->m_vInfo[INFO_POS])
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
        if (OBJECTTAG::SPAWNINGPOOL == (OBJECTTAG)i)
        {
            MONSTERTAG      m_eMonsterTag = MONSTERTAG::MONSTER_END;  //
            _int            m_iPoolCapacity = 5;
            _float          m_fSpawnRadius = 10.0f;
            _float          m_fSpawnTime = 10.0f;

            vector<CGameObject*>& vecObjList = pScene->Get_ObjectList(LAYERTAG::ENVIRONMENT, (OBJECTTAG)i);
            for (auto& iter : vecObjList)
            {
                eTag = iter->Get_ObjectTag();
                m_eMonsterTag = dynamic_cast<CSpawningPool*>(iter)->Get_MonsterTag();
                m_iPoolCapacity = dynamic_cast<CSpawningPool*>(iter)->Get_PoolCapacity();
                m_fSpawnRadius = dynamic_cast<CSpawningPool*>(iter)->Get_SpawnRadius();
                m_fSpawnTime = dynamic_cast<CSpawningPool*>(iter)->Get_SpawnTime();

                WriteFile(hFile, &eTag, sizeof(OBJECTTAG), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].x), sizeof(_float), &dwByte, nullptr);
                WriteFile(hFile, &(iter->m_pTransform->m_vInfo[INFO_POS].z), sizeof(_float), &dwByte, nullptr);

                WriteFile(hFile, &m_eMonsterTag, sizeof(MONSTERTAG), &dwByte, nullptr);
                WriteFile(hFile, &m_iPoolCapacity, sizeof(_int), &dwByte, nullptr);
                WriteFile(hFile, &m_fSpawnRadius, sizeof(_float), &dwByte, nullptr);
                WriteFile(hFile, &m_fSpawnTime, sizeof(_float), &dwByte, nullptr);
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
        if (OBJECTTAG::TERRAIN == (OBJECTTAG)i)
            continue;

        vector<CGameObject*>& refObjectList = pLayer->Get_ObjectList((OBJECTTAG)i);
        for_each(refObjectList.begin(), refObjectList.end(), [&](CGameObject* pObj) { EventManager()->DeleteObject(pObj); });
        //refObjectList.clear();
    }
    HANDLE hFile = CreateFile(L"../Bin/Data/TempData.dat", GENERIC_READ,
        0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (INVALID_HANDLE_VALUE == hFile)
        return E_FAIL;

    OBJECTTAG eTag = OBJECTTAG::OBJECT_END;

    DWORD	dwByte = 0;
    _float  fX, fY, fZ;
    _ubyte  byTextureNumber = 0;

    MONSTERTAG      eSpawnerTag = MONSTERTAG::MONSTER_END;  //
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
            
            if (fY < 0.f)
                continue;
            // value값 저장
            

            if (0 == dwByte)
                break;

            CGameObject* pGameObject = CCubeBlock::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
            NULL_CHECK_RETURN(pGameObject, E_FAIL);
            dynamic_cast<CCubeBlock*>(pGameObject)->Set_TextureNumber(byTextureNumber);
            pGameObject->m_pTransform->Translate(_vec3(fX, fY, fZ));
            EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
            //pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
        }
        else if (OBJECTTAG::SPAWNINGPOOL == eTag)
        {
            // value값 저장
            ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

            ReadFile(hFile, &eSpawnerTag, sizeof(MONSTERTAG), &dwByte, nullptr);
            ReadFile(hFile, &iPoolCapacity, sizeof(_int), &dwByte, nullptr);
            ReadFile(hFile, &fSpawnRadius, sizeof(_float), &dwByte, nullptr);
            ReadFile(hFile, &fSpawnTime, sizeof(_float), &dwByte, nullptr);

            if (0 == dwByte)
                break;

            CGameObject* pGameObject = CSpawningPool::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
            NULL_CHECK_RETURN(pGameObject, E_FAIL);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_MonsterTag(eSpawnerTag);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_PoolCapacity(iPoolCapacity);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_SpawnRadius(fSpawnRadius);
            dynamic_cast<CSpawningPool*>(pGameObject)->Set_SpawnTime(fSpawnTime);
            pGameObject->m_pTransform->Translate(_vec3(fX, 0.f, fZ));
            EventManager()->CreateObject(pGameObject, LAYERTAG::ENVIRONMENT);
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