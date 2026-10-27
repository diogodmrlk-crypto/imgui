#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

static bool init = true;
static int activeTab = 1; // 1=AIM, 2=ESP, 3=FAKHER
static ImVec4 panelColor = ImVec4(0.9f, 0.1f, 0.1f, 1.0f); 

void ApplyCustomTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.02f, 0.02f, 0.95f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.05f, 0.02f, 0.02f, 0.95f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.02f, 0.02f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.1f, 0.02f, 0.02f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.1f, 0.02f, 0.02f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.2f, 0.05f, 0.05f, 1.0f);
    colors[ImGuiCol_CheckMark] = panelColor;
    colors[ImGuiCol_FrameBg] = ImVec4(0.1f, 0.02f, 0.02f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2f, 0.05f, 0.05f, 1.0f);
    colors[ImGuiCol_Text] = ImVec4(0.9f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_Border] = panelColor;
    style.WindowRounding = 0.0f;
    style.FrameRounding = 2.0f;
    style.WindowBorderSize = 1.5f;
    style.WindowPadding = ImVec2(10, 10);
    style.ItemSpacing = ImVec2(8, 8);
}

bool ToggleButton(const char* label, bool* v) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    
    const float height = 30.0f;
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect bb(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + height));
    
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;
    
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
    if (pressed) { *v = !(*v); ImGui::MarkItemEdited(id); }
    
    ImGui::RenderText(ImVec2(bb.Min.x + 10, bb.Min.y + 7), label);
    
    ImU32 col_bg = *v ? ImGui::GetColorU32(panelColor) : ImGui::GetColorU32(ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImVec2 box_pos = ImVec2(bb.Max.x - 40, bb.Min.y + 5);
    window->DrawList->AddRectFilled(box_pos, ImVec2(box_pos.x + 35, box_pos.y + 20), col_bg, 3.0f);
    
    float circle_x = *v ? box_pos.x + 25 : box_pos.x + 10;
    window->DrawList->AddCircleFilled(ImVec2(circle_x, box_pos.y + 10), 6.0f, IM_COL32(255,255,255,255));
    
    return pressed;
}

void menu() {
    ApplyCustomTheme();
    
    ImGui::SetNextWindowSize(ImVec2(320, 260), ImGuiCond_Always);
    if (ImGui::Begin("PURPOU DEV", &init, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar)) {
        
        if (ImGui::Button("ESP", ImVec2(95, 30))) activeTab = 2;
        ImGui::SameLine();
        if (ImGui::Button("AIM", ImVec2(95, 30))) activeTab = 1;
        ImGui::SameLine();
        if (ImGui::Button("FAKHER", ImVec2(95, 30))) activeTab = 3;
        
        ImGui::Separator();
        
        if (activeTab == 1) { // AIM
            static bool silent = false, aimsys = false, fovcircle = false;
            ToggleButton("SILENT AIM", &silent);
            ToggleButton("AIM SYSTEM", &aimsys);
            ImGui::Text("Target: MIXED");
            ImGui::Text("Headshot rate: 75%%");
            ToggleButton("FOV CIRCLE", &fovcircle);
            
            ImGui::Text("FOV radius: 196");
            static float fov = 196.0f;
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.9f, 0.1f, 0.5f, 1.0f));
            ImGui::SliderFloat("##fov", &fov, 0.0f, 360.0f, "");
            ImGui::PopStyleColor();
        }
        else if (activeTab == 2) { // ESP
            static bool master = false, box = false, tracer = false, health = false, name = false, dist = false, skel = false;
            ToggleButton("ESP MASTER", &master);
            ToggleButton("PLAYER BOX", &box);
            ToggleButton("TOP TRACER", &tracer);
            ToggleButton("HEALTH BAR", &health);
            ToggleButton("PLAYER NAME", &name);
            ToggleButton("DISTANCE", &dist);
            ToggleButton("SKELETON", &skel);
        }
        else if (activeTab == 3) {
            ImGui::Text("EM BREVE...");
        }
    }
    ImGui::End();
}
