#include "..\Header\ImGuiManager.h"
#include "stdafx.h"

#include "CameraManager.h"
#include "FlyingCamera.h"
#include "Terrain.h"
#include "CubeBlock.h"

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
        vOut = Picking();
        if (_vec3(0.f, -10.f, 0.f) == vOut)
            return;

        if (selected_texture)
        {
            Engine::CGameObject* pGameObject = nullptr;

            pGameObject = CCubeBlock::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
            NULL_CHECK_RETURN(pGameObject);
            dynamic_cast<CCubeBlock*>(pGameObject)->Set_TextureNumber(selected_texture_index);
            pGameObject->m_pTransform->Translate(vOut);
            EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        }
    }

}

_vec3 CImGuiManager::Picking()
{
#pragma region Cube Picking 구현 중
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
                //break;

               /* return pCubeVtxPos[pCubeIdxPos[i]._1] + fU * (pCubeVtxPos[pCubeIdxPos[i]._0] - pCubeVtxPos[pCubeIdxPos[i]._1])
                    + fV * (pCubeVtxPos[pCubeIdxPos[i]._2] - pCubeVtxPos[pCubeIdxPos[i]._1]);*/
            }
        }
    }

    // 한 번 더 피킹을 검출하는 구간인데, 비효율적으로 됐지만 우선 놔둠...

    if (pq.empty()) // 피킹된 큐브가 없다면
        return _vec3(0.f, -10.f, 0.f);

    CCubeBlock* pFinalCube = pq.top().second;

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

    if (vecFinalPlane.empty())  // 한 번 더 피킹 검출
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
        ImGui::Begin("test window 0", &map_tool_window);
        
        ImGui::Text("Camera Pos");
        ImGui::Text("Mouse Pos");
        ImGui::Text("Texture Info");

        if(ImGui::TreeNode("Images"))
        {
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
            ImGui::NewLine();
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

void CImGuiManager::OnSaveData()
{
//    HANDLE hFile = CreateFile(L"../Data/TempData.dat", GENERIC_WRITE, 0, 0,
//        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
//
//    if (INVALID_HANDLE_VALUE == hFile)
//        return;
//
//    DWORD	dwByte = 0;
//    DWORD	dwStrByte = 0;
//
//
//    for (int i = 0; i < (UINT)OBJECTTAG::OBJECT_END; ++i)
//    {
//        /*if ((UINT)OBJID::OBJ_MOUSE == i)
//            continue;
//*/
//        vector<CGameObject*>& vecObjList = CObjectMgr::Get_Instance()->GetObjList((OBJECTTAG)i);
//        for (auto& iter : vecObjList)
//        {
//            /*if (OBJID::OBJ_MOUSE == iter->GetType())
//                continue;*/
//
//            dwStrByte = sizeof(TCHAR) * (iter->GetData().strName.GetLength() + 1);
//
//            WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
//            WriteFile(hFile, iter->GetData().strName, dwStrByte, &dwByte, nullptr);
//
//            WriteFile(hFile, &(iter->GetTransform()->Position().x), sizeof(float), &dwByte, nullptr);
//            WriteFile(hFile, &(iter->GetTransform()->Position().y), sizeof(float), &dwByte, nullptr);
//        }
//    }
//
//
//    CloseHandle(hFile);
}

void CImGuiManager::OnLoadData()
{
    //CObjectMgr::Get_Instance()->Release();

    //CString		strTemp = Dlg.GetPathName().GetString();
    //const TCHAR* pGetPath = strTemp.GetString();

    //HANDLE hFile = CreateFile(pGetPath, GENERIC_READ,
    //    0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    //if (INVALID_HANDLE_VALUE == hFile)
    //    return;

    //DWORD	dwByte = 0;
    //DWORD	dwStrByte = 0;
    //float fX, fY;

    //while (true)
    //{
    //    // key 값 저장
    //    ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

    //    TCHAR* pName = new TCHAR[dwStrByte];

    //    ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);


    //    // value값 저장
    //    ReadFile(hFile, &fX, sizeof(float), &dwByte, nullptr);
    //    ReadFile(hFile, &fY, sizeof(float), &dwByte, nullptr);


    //    if (0 == dwByte)
    //    {
    //        delete[]pName;
    //        pName = nullptr;
    //        break;
    //    }

    //    //if문추가
    //    CObjectMgr::Get_Instance()->CreateObject(pName, _vec3(fX, fY, 0.f));

    //    CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
    //    dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1))->Invalidate(FALSE);

    //    delete[]pName;
    //    pName = nullptr;
    //}

    //CloseHandle(hFile);
}

void CImGuiManager::Free()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
