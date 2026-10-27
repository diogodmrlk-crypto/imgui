#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include <string>
#include <chrono>
#include <map>

#ifndef OBFUSCATE
#define OBFUSCATE(str) str
#endif

// --- Variáveis de Estado e Configuração ---
#define COLOR_BORDER_BLACK IM_COL32(0, 0, 0, 255)       
#define COLOR_BG_GRAY      IM_COL32(20, 20, 22, 255)    
#define COLOR_WIDGET_BG    IM_COL32(30, 30, 35, 255)    

bool init = true;
static bool isMenuOpen = false;        
static bool clickInProgress = false;
static float anim = 0.0f;
static int activeTab = 1;
static const char* tabNames[] = {"AIMBOT", "VISUAIS", "Perçonalizações", "Usuário & Criador", "RAGES/RISKS"};
static bool botao_tp_state = false;
static bool botaoTp_window = false;
static bool botao_silent_state = false;
static bool botao_speed_state = false;
static bool botaoSpeed_window = false;
static bool botaoSilent_window = false;
static bool botao_vp_state = false;
static bool botaoVp_window = false;

static bool lightMode = false; 
static double clickStartTime = 0.0;

// Variáveis que faltavam
ImVec4 panelColor = ImVec4(1.0f, 0.35f, 0.0f, 1.0f); 
ImVec4 espColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 fovColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
ImVec4 lineColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
ImVec4 ThemeColer = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 ThemeFov = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

static bool Active = false;
static bool AimbotRage = false;
static bool AimbotLegit = false;
static bool EspCircle = false;
static bool AimbotAuto = false;
static bool HidePanel = false;
static int aimPosition = 0;
static float Fov = 50.0f;
static float AimDistancia = 150.0f;

static bool EspLine = false;
static bool EspBox = false;
static bool EspVida = false;
static bool Nick = false;
static bool Inimigos = false;
static bool EspSkeleton = false;
static bool EspInfo = false;
static bool EsnapLine = false;

static bool SpeedHack = false;
static bool AimSilent = false;
static bool TPlayer = false;
static bool UpPlayer = false;

struct FakeTexture { ImTextureID textureId; };
static FakeTexture iconaim = { (ImTextureID)1 };
static FakeTexture iconesp = { (ImTextureID)1 };
static FakeTexture iconmisc = { (ImTextureID)1 };
static FakeTexture iconinfo = { (ImTextureID)1 };
static FakeTexture iconpada = { (ImTextureID)1 };
static FakeTexture logo = { (ImTextureID)1 };
typedef FakeTexture TextureInfo;

// =========================================================================
// WIDGETS CUSTOMIZADOS (MANTIDOS COMO NO TEU CÓDIGO)
// =========================================================================

void DrawModernSlider(const char* label, float* v, float v_min, float v_max) {
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImDrawList* draw_list = window->DrawList;
    
    ImGui::Text(label);
    char value_buf[64];
    snprintf(value_buf, sizeof(value_buf), "(%.0f)", *v);
   
    float slider_width = ImGui::GetContentRegionAvail().x - 20; 
    ImGui::SameLine(slider_width - ImGui::CalcTextSize(value_buf).x);
    ImGui::TextColored(panelColor, "%s", value_buf);

    ImVec2 pos = ImGui::GetCursorScreenPos();
    float height = 22.0f; 
    ImRect frame_bb(pos, ImVec2(pos.x + slider_width, pos.y + height));
    
    ImGui::ItemSize(ImRect(frame_bb.Min, ImVec2(frame_bb.Max.x, frame_bb.Max.y + 10)));
    if (!ImGui::ItemAdd(frame_bb, window->GetID(label))) return;

    bool hovered, held;
    ImGui::ButtonBehavior(frame_bb, window->GetID(label), &hovered, &held);
    if (held) {
        *v = ImClamp((g.IO.MousePos.x - frame_bb.Min.x) / slider_width * (v_max - v_min) + v_min, v_min, v_max);
    }

    draw_list->AddRectFilled(frame_bb.Min, frame_bb.Max, lightMode ? IM_COL32(220, 220, 225, 255) : COLOR_WIDGET_BG, 11.0f);
    
    ImVec2 line_s = ImVec2(frame_bb.Min.x + 15, frame_bb.Min.y + height/2);
    ImVec2 line_e = ImVec2(frame_bb.Max.x - 15, frame_bb.Min.y + height/2);
    draw_list->AddLine(line_s, line_e, lightMode ? IM_COL32(180, 180, 180, 255) : IM_COL32(50, 50, 60, 255), 3.0f);
    
    float t = (*v - v_min) / (v_max - v_min);
    ImVec2 grab_pos = ImVec2(line_s.x + (line_e.x - line_s.x) * t, line_s.y);
    draw_list->AddLine(line_s, grab_pos, ImColor(panelColor), 3.0f);
    draw_list->AddCircleFilled(grab_pos, 8.0f, ImColor(panelColor), 16);
}

bool CustomCheckboxList(const char* label, bool* v) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);
    
    ImVec2 pos = window->DC.CursorPos;
    float width = ImGui::GetContentRegionAvail().x - 10;
    float height = 45.0f; 
    ImRect bb(pos, ImVec2(pos.x + width, pos.y + height));
    
    ImGui::ItemSize(bb, g.Style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
    if (pressed) { *v = !(*v); }

    ImU32 col_bg = lightMode ? IM_COL32(255, 255, 255, 255) : COLOR_BG_GRAY;
    window->DrawList->AddRectFilled(bb.Min, bb.Max, col_bg, 22.0f);
    
    ImGui::RenderText(ImVec2(bb.Min.x + 20, bb.Min.y + (height - ImGui::CalcTextSize(label).y) * 0.5f), label);

    float radius = 18.0f;
    ImVec2 check_pos = ImVec2(bb.Max.x - 25, bb.Min.y + height * 0.5f);
    
    ImU32 col = *v ? ImGui::GetColorU32(panelColor) : (lightMode ? IM_COL32(210, 210, 215, 255) : IM_COL32(40, 40, 45, 255));
    window->DrawList->AddCircleFilled(check_pos, radius, col, 30);

    if (*v) {
        float sz = 9.5f;
        window->DrawList->AddLine(ImVec2(check_pos.x - sz*0.5f, check_pos.y), ImVec2(check_pos.x - sz*0.1f, check_pos.y + sz*0.4f), IM_COL32(255,255,255,255), 2.0f);
        window->DrawList->AddLine(ImVec2(check_pos.x - sz*0.1f, check_pos.y + sz*0.4f), ImVec2(check_pos.x + sz*0.5f, check_pos.y - sz*0.4f), IM_COL32(255,255,255,255), 2.0f);
    }
    return pressed;
}

void CustomCombo(const char* label, int* current_item, const char* items[], int items_count) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return;
    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);
    
    ImGui::Text(label);
    ImGui::Spacing();

    ImVec2 pos = window->DC.CursorPos;
    float width = ImGui::GetContentRegionAvail().x - 15;
    float height = 35.0f; 
    ImRect bb(pos, ImVec2(pos.x + width, pos.y + height));

    ImGui::ItemSize(bb);
    if (!ImGui::ItemAdd(bb, id)) return;

    bool hovered, held;
    if (ImGui::ButtonBehavior(bb, id, &hovered, &held)) ImGui::OpenPopup(label);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, lightMode ? IM_COL32(245, 245, 250, 255) : IM_COL32(15, 15, 25, 255), 18.0f);
    
    ImVec2 text_pos = ImVec2(bb.Min.x + 15, bb.Min.y + (height - ImGui::CalcTextSize(items[*current_item]).y) * 0.5f);
    window->DrawList->AddText(text_pos, lightMode ? IM_COL32(30, 30, 30, 255) : IM_COL32(230, 230, 230, 255), items[*current_item]);

    float arrow_sz = 10.0f;
    ImVec2 arrow_pos = ImVec2(bb.Max.x - 20, bb.Min.y + height * 0.5f);
    window->DrawList->AddTriangleFilled(
        ImVec2(arrow_pos.x - arrow_sz * 0.4f, arrow_pos.y - arrow_sz * 0.2f),
        ImVec2(arrow_pos.x + arrow_sz * 0.4f, arrow_pos.y - arrow_sz * 0.2f),
        ImVec2(arrow_pos.x, arrow_pos.y + arrow_sz * 0.3f),
        ImGui::ColorConvertFloat4ToU32(panelColor)
    );

    ImGui::SetNextWindowPos(ImVec2(bb.Min.x, bb.Max.y + 3));
    ImGui::SetNextWindowSize(ImVec2(width, 0));
    ImGui::PushStyleColor(ImGuiCol_PopupBg, lightMode ? ImVec4(1,1,1,1) : ImVec4(0.04f, 0.04f, 0.07f, 1.00f));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 12.0f); 

    if (ImGui::BeginPopup(label)) {
        for (int i = 0; i < items_count; i++) {
            bool is_selected = (*current_item == i);
            if (is_selected) ImGui::PushStyleColor(ImGuiCol_Text, panelColor);
            else ImGui::PushStyleColor(ImGuiCol_Text, lightMode ? ImVec4(0,0,0,1) : ImVec4(0.63f, 0.63f, 0.63f, 1.00f));
            if (ImGui::Selectable(items[i], is_selected)) *current_item = i;
            ImGui::PopStyleColor();
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

// =========================================================================
// MENU PRINCIPAL (O TEU DESIGN INTACTO)
// =========================================================================

void menu() {
    if (!init) return;

    if (!isMenuOpen) {
        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
        if (ImGui::Begin(OBFUSCATE("##TriggerIcon"), 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize)) {
            ImVec2 imageSize(120, 120);
            ImGui::Image(logo.textureId, imageSize);
            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                clickInProgress = true;
                clickStartTime = ImGui::GetTime();
            }
            if (clickInProgress && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                clickInProgress = false;
                if ((ImGui::GetTime() - clickStartTime) < 0.2f) isMenuOpen = true; 
            }
        }
        ImGui::End();
        anim = ImClamp(anim - (ImGui::GetIO().DeltaTime / 0.25f), 0.0f, 1.0f);
    } else {
        anim = ImClamp(anim + (ImGui::GetIO().DeltaTime / 0.25f), 0.0f, 1.0f);
    }

    if (anim <= 0.0f && !isMenuOpen) return;

    ImU32 bg_color = lightMode ? IM_COL32(255, 255, 255, 255) : IM_COL32(8, 8, 10, 255);
    ImU32 text_color = lightMode ? IM_COL32(0, 0, 0, 255) : IM_COL32(255, 255, 255, 255);

    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, IM_COL32(0, 0, 0, 0));             
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, lightMode ? IM_COL32(200, 200, 200, 255) : IM_COL32(45, 45, 45, 255));      
    ImGui::PushStyleColor(ImGuiCol_WindowBg, bg_color);
    ImGui::PushStyleColor(ImGuiCol_Text, text_color);

    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 30.0f);                                       
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 12.0f);                                   
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, anim);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 15.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); 

    ImGui::SetNextWindowSize(ImVec2(650, 500), ImGuiCond_Always);

    if (ImGui::Begin("phzwqy 01", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        
        ImGui::BeginChild("Side", ImVec2(80, -1), false, ImGuiWindowFlags_NoScrollbar); 
        TextureInfo* icons[] = {&iconaim, &iconesp, &iconmisc, &iconinfo, &iconpada};
        
        for (int i = 0; i < 5; i++) {
            ImGui::PushID(i);
            ImVec2 cp = ImGui::GetCursorScreenPos();
            if (ImGui::InvisibleButton("tab_btn", ImVec2(70, 70))) activeTab = i + 1;
            ImVec2 center = ImVec2(cp.x + 40, cp.y + 40);
            
            if (activeTab == i + 1) {
                draw_list->AddCircleFilled(center, 30.0f, lightMode ? IM_COL32(235, 235, 235, 255) : IM_COL32(25, 25, 30, 255), 30);
                draw_list->AddImage(icons[i]->textureId, ImVec2(center.x-15, center.y-15), ImVec2(center.x+15, center.y+15), ImVec2(0,0), ImVec2(1,1), ImColor(panelColor));
            } else {
                ImU32 icon_col = lightMode ? IM_COL32(0, 0, 0, 255) : IM_COL32(179, 179, 179, 255);
                draw_list->AddImage(icons[i]->textureId, ImVec2(center.x-15, center.y-15), ImVec2(center.x+15, center.y+15), ImVec2(0,0), ImVec2(1,1), icon_col);
            }
            ImGui::PopID();
        }
        ImGui::EndChild();

        ImGui::SameLine(80);
        ImGui::BeginChild("Content", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
        
        ImGui::SetCursorPos(ImVec2(10, 15));
        ImVec2 header_pos = ImGui::GetCursorScreenPos();
        float header_width = ImGui::GetContentRegionAvail().x - 15;
        float header_height = 60.0f;

        draw_list->AddRectFilled(header_pos, ImVec2(header_pos.x + header_width, header_pos.y + header_height), IM_COL32(20, 20, 22, 255), 30.0f); 

        float pill_w = header_width - 110.0f; 
        ImVec2 pill_m = ImVec2(header_pos.x + 30, header_pos.y + 12); 
        draw_list->AddRectFilled(pill_m, ImVec2(pill_m.x + pill_w, pill_m.y + 36), IM_COL32(22, 22, 25, 255), 18.0f); 
        
        float icon_size = 15.0f; 
        draw_list->AddImage(icons[activeTab-1]->textureId, ImVec2(pill_m.x + 15 - icon_size, pill_m.y + 3.0f), ImVec2(pill_m.x + 15 + icon_size, pill_m.y + 33.0f), ImVec2(0,0), ImVec2(1,1), ImColor(panelColor));
        
        ImVec2 text_size = ImGui::CalcTextSize(tabNames[activeTab-1]);
        float text_x = pill_m.x + 32.0f; 
        draw_list->AddText(ImVec2(text_x + 1.0f, pill_m.y + 18.0f - text_size.y * 0.5f), ImColor(panelColor), tabNames[activeTab-1]);
        draw_list->AddText(ImVec2(text_x, pill_m.y + 18.0f - text_size.y * 0.5f), ImColor(panelColor), tabNames[activeTab-1]);

        float icon_r = 22.0f;
        ImVec2 bola_p = ImVec2(header_pos.x + header_width - 80, header_pos.y + 30); 
        
        ImU32 bola_bg = lightMode ? IM_COL32(255, 255, 255, 255) : IM_COL32(40, 40, 45, 255);
        ImU32 bola_icon_col = lightMode ? IM_COL32(0, 0, 0, 255) : ImGui::ColorConvertFloat4ToU32(panelColor);
        draw_list->AddCircleFilled(bola_p, icon_r, bola_bg, 32);
        draw_list->AddCircle(bola_p, icon_r * 0.6f, bola_icon_col, 32, 2.0f); 
        draw_list->AddCircleFilled(ImVec2(bola_p.x + 5, bola_p.y - 5), icon_r * 0.5f, bola_bg, 32); 

        if (ImGui::IsMouseHoveringRect(ImVec2(bola_p.x-icon_r, bola_p.y-icon_r), ImVec2(bola_p.x+icon_r, bola_p.y+icon_r)) && ImGui::IsMouseClicked(0)) {
            lightMode = !lightMode;
        }

        ImVec2 close_p = ImVec2(header_pos.x + header_width - 30, header_pos.y + 30); 
        draw_list->AddCircleFilled(close_p, icon_r, IM_COL32(40, 40, 45, 255), 32); 
        
        float x_size = 10.0f;
        float x_thickness = 2.0f;
        ImU32 x_color = ImGui::ColorConvertFloat4ToU32(panelColor);
        draw_list->AddLine(ImVec2(close_p.x - x_size, close_p.y - x_size), ImVec2(close_p.x + x_size, close_p.y + x_size), x_color, x_thickness);
        draw_list->AddLine(ImVec2(close_p.x + x_size, close_p.y - x_size), ImVec2(close_p.x - x_size, close_p.y + x_size), x_color, x_thickness);

        if (ImGui::IsMouseHoveringRect(ImVec2(close_p.x-icon_r, close_p.y-icon_r), ImVec2(close_p.x+icon_r, close_p.y+icon_r)) && ImGui::IsMouseClicked(0)) {
            isMenuOpen = false;
        }

        ImGui::SetCursorPos(ImVec2(0, 80));
         
        ImGui::BeginChild("ScrollArea", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::Indent(20);

        if (activeTab == 1) { 
            CustomCheckboxList("Ativar Funções", &Active);
            CustomCheckboxList("Aimbot Rage", &AimbotRage);    
            CustomCheckboxList("Aimbot Legit", &AimbotLegit);        
            CustomCheckboxList("Exibir Fov", &EspCircle);    
            ImGui::Dummy(ImVec2(0, 10));
            const char* items[] = { "Cabeça", "Pescoço", "Peito", "Quadril" };
            CustomCombo("Local da Puxada:", &aimPosition, items, 4);
            DrawModernSlider("Fov Legit", &Fov, 0, 180);
            DrawModernSlider("Aim Distancia", &AimDistancia, 0, 500);
        }
        else if (activeTab == 2) { 
            CustomCheckboxList("Linha Target", &EsnapLine);
            CustomCheckboxList("Linha", &EspLine);
            CustomCheckboxList("Caixa", &EspBox);
            CustomCheckboxList("Vida", &EspVida);
            CustomCheckboxList("Nome", &Nick);
            CustomCheckboxList("Skeleton", &EspSkeleton);
            CustomCheckboxList("Inimigos", &Inimigos);
        }
        else if (activeTab == 3) { 
            ImGui::PushStyleColor(ImGuiCol_PopupBg, lightMode ? IM_COL32(255,255,255,255) : IM_COL32(0, 0, 0, 255));
            ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 10.0f);
            ImGui::PushItemWidth(150); 
            ImGui::ColorEdit4("Cor do Painel", (float*)&panelColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
            ImGui::ColorEdit3("Cor da Esp", (float*)&ThemeColer, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit3("Cor do Fov", (float*)&ThemeFov, ImGuiColorEditFlags_NoInputs);
            ImGui::PopItemWidth();
            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(1);
            ImGui::Text("AJUSTAR CORES"); ImGui::SameLine();
            ImGui::TextColored(panelColor, "Perçonalizações");
        }
        else if (activeTab == 4) {       
            ImGui::Text("Desenvolvido Por:"); ImGui::SameLine();
            ImGui::TextColored(panelColor, "@PURPODEV .wq");
            ImGui::Text("Versão do Pacote:"); ImGui::SameLine();
            ImGui::TextColored(panelColor, "com.dts.freefiremax");
            ImGui::Text("Versão do Jogo:"); ImGui::SameLine();
            ImGui::TextColored(panelColor, "Free Fire MAX 2.117.1 ApkMod");
            ImGui::Text("Key:"); ImGui::SameLine();
            ImGui::TextColored(panelColor, "FFH4X-1DIA-A7NO");
        }
        else if (activeTab == 5) { 
            CustomCheckboxList("Aim Silent", &botaoSilent_window);
            CustomCheckboxList("Up/Voar Player", &botaoVp_window);
            CustomCheckboxList("Tp Player", &botaoTp_window);
            CustomCheckboxList("SpeedHack", &botaoSpeed_window);
            DrawModernSlider("Fov Rage", &Fov, 0, 500);
        }

        ImGui::Unindent(20);
        ImGui::EndChild(); 
        ImGui::EndChild(); 
    }
    ImGui::End();
    ImGui::PopStyleColor(4); 
    ImGui::PopStyleVar(6); 
}

void RenderBotaoFlutuante() {
    if (botaoSpeed_window) {
        ImVec4 cor_fundo = botao_speed_state ? ImVec4(0.4f, 0.1f, 0.6f, 0.9f) : ImVec4(0.5f, 0.5f, 0.5f, 0.9f);
        ImVec4 cor_hover = botao_speed_state ? ImVec4(0.6f, 0.2f, 0.8f, 0.9f) : ImVec4(0.6f, 0.6f, 0.6f, 0.9f);
        ImVec4 cor_active = botao_speed_state ? ImVec4(0.7f, 0.3f, 0.9f, 0.9f) : ImVec4(0.7f, 0.7f, 0.7f, 0.9f);
        ImGui::PushStyleColor(ImGuiCol_Button, cor_fundo);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, cor_hover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, cor_active);
        ImGui::SetNextWindowPos(ImVec2(30, 30), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(180, 80)); 
        ImVec4 cor_borda = botao_speed_state ? ImVec4(0.8f, 0.0f, 1.0f, 1.0f) : ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, cor_borda);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::Begin("BotaoFlutuante", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
        const char* texto_botao = botao_speed_state ? "SPEED ON" : "SPEED OF";
        ImVec4 cor_texto = botao_speed_state ? ImVec4(0.8f, 0.0f, 1.0f, 1.0f) : ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, cor_texto);
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        if (ImGui::Button(texto_botao, ImVec2(160, 60))) {
            botao_speed_state = !botao_speed_state;
            SpeedHack = !SpeedHack;
        }
        ImGui::PopFont();
        ImGui::PopStyleColor();
        ImGui::End();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(5);
    }
}

void RenderBotaoSelint() {
    if (botaoSilent_window) {
        ImVec4 cor_fundo = botao_silent_state ? ImVec4(0.4f, 0.1f, 0.6f, 0.9f) : ImVec4(0.5f, 0.5f, 0.5f, 0.9f);
        ImVec4 cor_hover = botao_silent_state ? ImVec4(0.6f, 0.2f, 0.8f, 0.9f) : ImVec4(0.6f, 0.6f, 0.6f, 0.9f);
        ImVec4 cor_active = botao_silent_state ? ImVec4(0.7f, 0.3f, 0.9f, 0.9f) : ImVec4(0.7f, 0.7f, 0.7f, 0.9f);
        ImGui::PushStyleColor(ImGuiCol_Button, cor_fundo);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, cor_hover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, cor_active);
        ImGui::SetNextWindowPos(ImVec2(30, 30), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(180, 100));
        ImVec4 cor_borda = botao_silent_state ? ImVec4(0.8f, 0.0f, 1.0f, 1.0f) : ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, cor_borda);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::Begin("BotaoSelint", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
        const char* texto = botao_silent_state ? "SILENT ON" : "SILENT OF";
        ImVec4 cor_texto = botao_silent_state ? ImVec4(0.8f, 0.0f, 1.0f, 1.0f) : ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, cor_texto);
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        if (ImGui::Button(texto, ImVec2(160, 60))) {
            botao_silent_state = !botao_silent_state;
            AimSilent = !AimSilent;
        }
        ImGui::PopFont();
        ImGui::PopStyleColor();
        ImGui::End();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(5);
    }
}

void RenderBotaoTp() {
    if (botaoTp_window) {
        ImVec4 cor_fundo = botao_tp_state ? ImVec4(0.4f, 0.1f, 0.6f, 0.9f) : ImVec4(0.5f, 0.5f, 0.5f, 0.9f);
        ImVec4 cor_hover = botao_tp_state ? ImVec4(0.6f, 0.2f, 0.8f, 0.9f) : ImVec4(0.6f, 0.6f, 0.6f, 0.9f);
        ImVec4 cor_active = botao_tp_state ? ImVec4(0.7f, 0.3f, 0.9f, 0.9f) : ImVec4(0.7f, 0.7f, 0.7f, 0.9f);
        ImGui::PushStyleColor(ImGuiCol_Button, cor_fundo);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, cor_hover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, cor_active);
        ImGui::SetNextWindowPos(ImVec2(30, 30), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(180, 100));
        ImVec4 cor_borda = botao_tp_state ? ImVec4(0.8f, 0.0f, 1.0f, 1.0f) : ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, cor_borda);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::Begin("BotaoTp", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
        const char* texto = botao_tp_state ? "TP ON" : "TP OF";
        ImVec4 cor_texto = botao_tp_state ? ImVec4(0.8f, 0.0f, 1.0f, 1.0f) : ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, cor_texto);
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        if (ImGui::Button(texto, ImVec2(160, 60))) {
            botao_tp_state = !botao_tp_state;
            TPlayer = !TPlayer;
        }
        ImGui::PopFont();
        ImGui::PopStyleColor();
        ImGui::End();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(5);
    }
}

void RenderBotaoVp() {
    if (botaoVp_window) {
        ImVec4 cor_fundo = botao_vp_state ? ImVec4(0.4f, 0.1f, 0.6f, 0.9f) : ImVec4(0.5f, 0.5f, 0.5f, 0.9f);
        ImVec4 cor_hover = botao_vp_state ? ImVec4(0.6f, 0.2f, 0.8f, 0.9f) : ImVec4(0.6f, 0.6f, 0.6f, 0.9f);
        ImVec4 cor_active = botao_vp_state ? ImVec4(0.7f, 0.3f, 0.9f, 0.9f) : ImVec4(0.7f, 0.7f, 0.7f, 0.9f);
        ImGui::PushStyleColor(ImGuiCol_Button, cor_fundo);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, cor_hover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, cor_active);
        ImGui::SetNextWindowPos(ImVec2(30, 30), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(180, 100));
        ImVec4 cor_borda = botao_vp_state ? ImVec4(0.8f, 0.0f, 1.0f, 1.0f) : ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, cor_borda);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::Begin("BotaoVp", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
        const char* texto = botao_vp_state ? "VOAR ON" : "VOAR OF";
        ImVec4 cor_texto = botao_vp_state ? ImVec4(0.8f, 0.0f, 1.0f, 1.0f) : ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, cor_texto);
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        if (ImGui::Button(texto, ImVec2(160, 60))) {
            botao_vp_state = !botao_vp_state;
            UpPlayer = !UpPlayer;
        }
        ImGui::PopFont();
        ImGui::PopStyleColor();
        ImGui::End();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(5);
    }
}
