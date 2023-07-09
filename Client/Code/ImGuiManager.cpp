#include "..\Header\ImGuiManager.h"
#include "stdafx.h"

#include "CameraManager.h"
#include "FlyingCamera.h"
#include "Terrain.h"

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
        vOut = Picking();
}

_vec3 CImGuiManager::Picking()
{
    // 일단 임시로 front()로 하나만 가져와 보자.
    CTerrain* pTerrain = dynamic_cast<CTerrain*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::TERRAIN).front());
    NULL_CHECK_RETURN(pTerrain, _vec3(0.f, 0.f, 0.f));

    //CTerrainTex* pTerrainBuffer = dynamic_cast<CTerrainTex*>(pTerrain->Get_Component(COMPONENTTAG::BUFFER, ID_STATIC));
    //NULL_CHECK_RETURN(pTerrainBuffer, _vec3(0.f, 0.f, 0.f));

    CTransform* pTerrainTransform = dynamic_cast<CTransform*>(pTerrain->m_pTransform);
    NULL_CHECK_RETURN(pTerrainTransform, _vec3(0.f, 0.f, 0.f));

    //

    POINT		ptMouse{};
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    _vec3		vMousePos;

    D3DVIEWPORT9		ViewPort;
    ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
    CGraphicDev::GetInstance()->Get_GraphicDev()->GetViewport(&ViewPort);

    //0,0      -> -1, 1
    //400, 300 ->  0, 0
    //800, 600 ->  1, -1

    // 뷰포트 -> 투영
    vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
    vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
    vMousePos.z = 0.f;

    // 투영 -> 뷰 스페이스
    _matrix		matProj;
    //CGraphicDev::GetInstance()->Get_GraphicDev()->GetTransform(D3DTS_PROJECTION, &matProj);
    D3DXMatrixInverse(&matProj, 0, &dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()
        ->Get_CurrentCam())->Get_Camera()->Get_ProjMatrix());
    D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

    _vec3	vRayPos, vRayDir;

    vRayPos = _vec3(0.f, 0.f, 0.f);
    vRayDir = vMousePos - vRayPos;

    // 뷰 스페이스 -> 월드 스페이스
    _matrix		matView;
   // CGraphicDev::GetInstance()->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &matView);
    //dynamic_cast<CCamera*>(CCameraManager::GetInstance()->Get_CurrentCam())->Get_ViewMatrix();

    D3DXMatrixInverse(&matView, 0, &dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()
        ->Get_CurrentCam())->Get_Camera()->Get_ViewMatrix());
    D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
    D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

    // 월드 스페이스 -> 로컬 스페이스
    /*_matrix		matWorld;
    pTerrainTransform->Get_WorldMatrix(&matWorld);
    D3DXMatrixInverse(&matWorld, 0, &matWorld);
    D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
    D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);*/

    const vector<_vec3>& pTerrainVtxPos = pTerrain->LoadTerrainVertex();

    _ulong		dwVtxIdx[3]{};

    _float	fU = 0.f, fV = 0.f, fDist = 0.f;

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
                // V1 + U(V2 - V1) + V(V3 - V1)

                return _vec3(pTerrainVtxPos[dwVtxIdx[1]].x + fU * (pTerrainVtxPos[dwVtxIdx[0]].x - pTerrainVtxPos[dwVtxIdx[1]].x),
                    0.f,
                    pTerrainVtxPos[dwVtxIdx[1]].z + fV * (pTerrainVtxPos[dwVtxIdx[2]].z - pTerrainVtxPos[dwVtxIdx[1]].z));
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
                // V1 + U(V2 - V1) + V(V3 - V1)

                return _vec3(pTerrainVtxPos[dwVtxIdx[1]].x + fU * (pTerrainVtxPos[dwVtxIdx[0]].x - pTerrainVtxPos[dwVtxIdx[1]].x),
                    0.f,
                    pTerrainVtxPos[dwVtxIdx[1]].z + fV * (pTerrainVtxPos[dwVtxIdx[2]].z - pTerrainVtxPos[dwVtxIdx[1]].z));
            }
        }
    }

    return _vec3(0.f, 0.f, 0.f);
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



    _bool test_window_0 = true;

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    if (test_window_0)
    {
        ImGui::Begin("test window 0", &test_window_0);
        
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

            int pressed_count = 0;
            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    _int iIndex = 4 * i + j;
                    if (iIndex > m_pTerainTexture.size())
                        break;

                    ImGui::PushID(iIndex);

                    if (ImGui::ImageButton("", m_pTerainTexture[iIndex], size))
                        selected_texture = m_pTerainTexture[iIndex];

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

void CImGuiManager::Free()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
