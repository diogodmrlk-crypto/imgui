#pragma once
// #define IMGUI_DEFINE_MATH_OPERATORS // (Removido, o Makefile já define)
#include "imgui.h"
#include "imgui_internal.h"
#include <string>
#include <chrono>
#include <map>

#ifndef OBFUSCATE
#define OBFUSCATE(str) str
#endif

int targetIndex = 0;
int radioOption = 2;

enum class BoxStyle;
enum class LinePosition;

extern ImVec4 ThemeFov;

static bool botao_vp = false;
static bool botao_tp = false;
static bool botao_selint = false;
static bool botao_ligado = false;
static bool botaospeedhack = false;
static bool botaoselint = false;
static bool botaoTp = false;
static bool botaovp = false;

int nickStyles = 0;

bool FastParachute = false;
bool RevealEnemies = false; 
bool FreeMovement = false;

bool init = true;

bool showMenu = false;
static int tapCount = 0;
static double lastTapTime = 0;
static float anim = 0.0f;
static bool isOpening = true;
static ImFont* sansFont = nullptr;

ImVec4 espColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 fovColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
static ImVec4 panelColor = ImVec4(0.85f, 0.05f, 0.05f, 1.0f);
static ImVec4 lineColor  = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

static bool openColorPicker = false;

const float BORDER_ROUNDING = 26.0f;
const float BORDER_SIZE = 1.5f;

static float Fov = 50.0f; 
static int aimPosition = 0; 
static bool Active = false;
static bool AimbotRage = false;
static bool AimbotLegit = false;
static bool EspCircle = false;
static bool AimbotAuto = false;
static bool HidePanel = false;

static bool EspLine = false;
static bool EspBox = false;
static bool EspVida = false;
static bool Nick = false;
static bool Inimigos = false;
static bool EspSkeleton = false;
static bool EspInfo = false;

static ImVec4 ThemeColer = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
static bool temaEscuro = false;

struct FakeTexture {
    ImTextureID textureId;
};
static FakeTexture iconaim = { (ImTextureID)1 };
static FakeTexture iconesp = { (ImTextureID)1 };
static FakeTexture iconmisc = { (ImTextureID)1 };
static FakeTexture iconinfo = { (ImTextureID)1 };

void ApplyCustomTheme(const ImVec4& mainColor, bool darkTheme = false) {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    ImVec4 light(fmin(mainColor.x + 0.05f, 1.0f), fmin(mainColor.y + 0.05f, 1.0f), fmin(mainColor.z + 0.05f, 1.0f), 1.0f);    
    ImVec4 dark(mainColor.x * 0.85f, mainColor.y * 0.85f, mainColor.z * 0.85f, 1.0f);    

    if (darkTheme) {
        ImVec4 netunoBg = ImVec4(0.10f, 0.12f, 0.16f, 1.00f);
        ImVec4 netunoCard = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
        ImVec4 netunoLight = ImVec4(0.20f, 0.23f, 0.28f, 1.00f);
        ImVec4 netunoText = ImVec4(0.80f, 0.83f, 0.87f, 1.00f);
        ImVec4 netunoBorder = ImVec4(0.25f, 0.28f, 0.35f, 1.00f);
        
        colors[ImGuiCol_WindowBg]         = netunoBg;    
        colors[ImGuiCol_ChildBg]          = netunoBg;    
        colors[ImGuiCol_TitleBg]          = netunoCard;    
        colors[ImGuiCol_TitleBgActive]    = netunoCard;    
        colors[ImGuiCol_TitleBgCollapsed] = netunoCard;    
        colors[ImGuiCol_Button]           = netunoCard;    
        colors[ImGuiCol_ButtonHovered]    = netunoLight;    
        colors[ImGuiCol_ButtonActive]     = netunoLight;    
        colors[ImGuiCol_Header]           = netunoCard;    
        colors[ImGuiCol_HeaderHovered]    = netunoLight;    
        colors[ImGuiCol_HeaderActive]     = netunoLight;    
        colors[ImGuiCol_CheckMark]        = mainColor;  
        colors[ImGuiCol_FrameBg]          = netunoCard;    
        colors[ImGuiCol_FrameBgHovered]   = netunoLight;    
        colors[ImGuiCol_FrameBgActive]    = mainColor;    
        colors[ImGuiCol_Border]           = netunoBorder;
        colors[ImGuiCol_BorderShadow]     = ImVec4(0, 0, 0, 0);    
        colors[ImGuiCol_ScrollbarBg]      = ImVec4(0.08f, 0.10f, 0.14f, 0.30f);
        colors[ImGuiCol_ScrollbarGrab]    = ImVec4(0.30f, 0.33f, 0.40f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.38f, 0.45f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.40f, 0.43f, 0.50f, 0.80f);
        colors[ImGuiCol_PopupBg]          = netunoCard;
        colors[ImGuiCol_Text]             = netunoText;
        colors[ImGuiCol_TextDisabled]     = ImVec4(0.50f, 0.53f, 0.58f, 1.00f);
    } else {
        colors[ImGuiCol_WindowBg]         = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_ChildBg]          = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TitleBg]          = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TitleBgActive]    = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_Button]           = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_ButtonHovered]    = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
        colors[ImGuiCol_ButtonActive]     = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_Header]           = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_HeaderHovered]    = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
        colors[ImGuiCol_HeaderActive]     = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_CheckMark]        = mainColor;  
        colors[ImGuiCol_FrameBg]          = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]   = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
        colors[ImGuiCol_FrameBgActive]    = mainColor;    
        colors[ImGuiCol_Border]           = ImVec4(0.80f, 0.80f, 0.80f, 0.50f);
        colors[ImGuiCol_BorderShadow]     = ImVec4(0, 0, 0, 0);    
        colors[ImGuiCol_ScrollbarBg]      = ImVec4(0.96f, 0.96f, 0.96f, 0.30f);
        colors[ImGuiCol_ScrollbarGrab]    = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.60f, 0.60f, 0.60f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.50f, 0.50f, 0.50f, 0.80f);
        colors[ImGuiCol_PopupBg]          = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_Text]             = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_TextDisabled]     = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    }

    style.WindowBorderSize = 0.0f;    
    style.FrameBorderSize  = 0.0f;
    style.TabBorderSize    = 0.0f;    
    style.FrameRounding    = 20.0f;
    style.WindowRounding   = 20.0f;    
    style.GrabRounding     = 20.0f;
    style.TabRounding      = 20.0f;
    style.ScrollbarRounding = 12.0f;
    style.WindowPadding    = ImVec2(12, 10);    
    style.FramePadding     = ImVec2(10, 10);
    style.ItemSpacing      = ImVec2(10, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);    
    style.IndentSpacing    = 25.0f;    
    style.WindowTitleAlign = ImVec2(0.1f, 0.5f);    
    style.ScrollbarSize    = 20.0f;
    style.GrabMinSize      = 25.0f;
}

void DrawHorizontalLine(float extraTopSpace = 6.0f, float extraBottomSpace = 10.0f) {
    ImGui::Dummy(ImVec2(0, extraTopSpace));
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddLine(ImVec2(p.x, p.y), ImVec2(p.x + ImGui::GetContentRegionAvail().x, p.y), ImColor(lineColor), 1.0f);
    ImGui::Dummy(ImVec2(0, extraBottomSpace));
}

void CabecalhoTabComClose(ImTextureID iconTexture, const char* titulo, bool* fechar = nullptr) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    const float altura = 46.0f;
    const float iconSize = 28.0f;
    const float closeButtonSize = 38.0f;
    const float themeButtonSize = 38.0f;
    ImVec2 textSize = ImGui::CalcTextSize(titulo);
    float largura_painel = ImGui::GetContentRegionAvail().x;
    float cabecalho_largura = largura_painel * 0.5f;
    const ImVec2 pos = ImVec2(window->DC.CursorPos.x, window->DC.CursorPos.y + 8.0f);
    const ImRect cabecalho_bb(pos, ImVec2(pos.x + cabecalho_largura, pos.y + altura));
    float close_x = ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - 60.0f;
    float theme_x = close_x - themeButtonSize - 15.0f;
    float buttons_y = pos.y + (altura - closeButtonSize) * 0.5f;
    ImRect theme_bb(ImVec2(theme_x, buttons_y), ImVec2(theme_x + themeButtonSize, buttons_y + themeButtonSize));
    ImRect close_bb(ImVec2(close_x, buttons_y), ImVec2(close_x + closeButtonSize, buttons_y + closeButtonSize));
    ImU32 cor_fundo = temaEscuro ? ImGui::GetColorU32(ImVec4(0.15f, 0.18f, 0.22f, 1.00f)) : ImGui::GetColorU32(ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
    window->DrawList->AddRectFilled(cabecalho_bb.Min, cabecalho_bb.Max, cor_fundo, BORDER_ROUNDING);
    ImU32 cor_borda = temaEscuro ? ImGui::GetColorU32(ImVec4(0.25f, 0.28f, 0.35f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
    window->DrawList->AddRect(cabecalho_bb.Min, cabecalho_bb.Max, cor_borda, BORDER_ROUNDING, 0, BORDER_SIZE);
    ImVec2 iconPos = ImVec2(pos.x + 15.0f, pos.y + (altura - iconSize) * 0.5f);
    window->DrawList->AddImage(iconTexture, iconPos, ImVec2(iconPos.x + iconSize, iconPos.y + iconSize), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(panelColor));
    ImVec2 texto_pos = ImVec2(iconPos.x + iconSize + 10.0f, pos.y + (altura - textSize.y) * 0.5f);
    ImU32 cor_texto = ImGui::GetColorU32(panelColor);
    window->DrawList->AddText(texto_pos, cor_texto, titulo);
    bool theme_hovered = ImGui::IsMouseHoveringRect(theme_bb.Min, theme_bb.Max);
    ImU32 theme_color = theme_hovered ? (temaEscuro ? ImGui::GetColorU32(ImVec4(0.20f, 0.23f, 0.28f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.95f, 0.95f, 0.95f, 1.0f))) : (temaEscuro ? ImGui::GetColorU32(ImVec4(0.15f, 0.18f, 0.22f, 1.0f)) : ImGui::GetColorU32(ImVec4(1.00f, 1.00f, 1.00f, 1.0f)));
    ImVec2 theme_center = ImVec2(theme_bb.Min.x + themeButtonSize * 0.5f, theme_bb.Min.y + themeButtonSize * 0.5f);
    float theme_radius = themeButtonSize * 0.5f;
    ImU32 theme_border_color = temaEscuro ? ImGui::GetColorU32(ImVec4(0.25f, 0.28f, 0.35f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
    window->DrawList->AddCircle(theme_center, theme_radius, theme_border_color, 0, 1.0f);
    float moon_size = themeButtonSize * 0.35f;
    ImU32 moon_color = ImGui::GetColorU32(panelColor);
    window->DrawList->AddCircle(theme_center, moon_size, moon_color, 0, 1.5f);
    float ear_size = moon_size * 0.8f;
    ImVec2 ear_center = ImVec2(theme_center.x + moon_size * 0.25f, theme_center.y);
    window->DrawList->AddCircleFilled(ear_center, ear_size, theme_color);
    if (theme_hovered && ImGui::IsMouseClicked(0)) {
        temaEscuro = !temaEscuro;
        panelColor = ImVec4(0.85f, 0.05f, 0.05f, 1.0f);
        lineColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    bool close_hovered = ImGui::IsMouseHoveringRect(close_bb.Min, close_bb.Max);
    ImU32 close_color = close_hovered ? (temaEscuro ? ImGui::GetColorU32(ImVec4(0.20f, 0.23f, 0.28f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.95f, 0.95f, 0.95f, 1.0f))) : (temaEscuro ? ImGui::GetColorU32(ImVec4(0.15f, 0.18f, 0.22f, 1.0f)) : ImGui::GetColorU32(ImVec4(1.00f, 1.00f, 1.00f, 1.0f)));
    ImVec2 close_center = ImVec2(close_bb.Min.x + closeButtonSize * 0.5f, close_bb.Min.y + closeButtonSize * 0.5f);
    float close_radius = closeButtonSize * 0.5f;
    window->DrawList->AddCircleFilled(close_center, close_radius, close_color);
    ImU32 close_border_color = temaEscuro ? ImGui::GetColorU32(ImVec4(0.25f, 0.28f, 0.35f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
    window->DrawList->AddCircle(close_center, close_radius, close_border_color, 0, 1.0f);
    float cross_size = closeButtonSize * 0.4f;
    ImVec2 cross_start = ImVec2(close_center.x - cross_size * 0.5f, close_center.y - cross_size * 0.5f);
    ImVec2 cross_end = ImVec2(close_center.x + cross_size * 0.5f, close_center.y + cross_size * 0.5f);
    ImU32 cross_color = temaEscuro ? ImGui::GetColorU32(ImVec4(0.90f, 0.30f, 0.30f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.90f, 0.20f, 0.20f, 1.0f));
    window->DrawList->AddLine(cross_start, cross_end, cross_color, 2.0f);
    window->DrawList->AddLine(ImVec2(cross_start.x, cross_end.y), ImVec2(cross_end.x, cross_start.y), cross_color, 2.0f);
    ImGui::ItemSize(cabecalho_bb, 0);
    if (close_hovered && ImGui::IsMouseClicked(0) && fechar != nullptr) {
        *fechar = false;
    }
}

bool NovoBotaoEstilo(const char* label, bool* v, const char* tag = nullptr) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float altura = 52.0f;
    const float largura = ImGui::GetContentRegionAvail().x - 5.0f;
    const float toggle_diametro = 32.0f;
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, ImVec2(pos.x + largura, pos.y + altura));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id)) return false;
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) { *v = !(*v); ImGui::MarkItemEdited(id); }
    ImU32 cor_fundo = hovered ? (temaEscuro ? ImGui::GetColorU32(ImVec4(0.20f, 0.23f, 0.28f, 1.00f)) : ImGui::GetColorU32(ImVec4(0.95f, 0.95f, 0.95f, 1.00f))) : (temaEscuro ? ImGui::GetColorU32(ImVec4(0.15f, 0.18f, 0.22f, 1.00f)) : ImGui::GetColorU32(ImVec4(1.00f, 1.00f, 1.00f, 1.00f)));
    ImU32 cor_texto = temaEscuro ? ImGui::GetColorU32(ImVec4(0.80f, 0.83f, 0.87f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
    ImU32 cor_botao_redondo = *v ? ImGui::GetColorU32(panelColor) : (temaEscuro ? ImGui::GetColorU32(ImVec4(0.30f, 0.33f, 0.40f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.80f, 0.80f, 0.80f, 1.0f)));
    window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, cor_fundo, BORDER_ROUNDING);
    ImU32 cor_borda = temaEscuro ? ImGui::GetColorU32(ImVec4(0.25f, 0.28f, 0.35f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
    window->DrawList->AddRect(total_bb.Min, total_bb.Max, cor_borda, BORDER_ROUNDING, 0, BORDER_SIZE);
    ImVec2 texto_pos = ImVec2(pos.x + 18.0f, pos.y + (altura - ImGui::GetFontSize()) * 0.5f);
    window->DrawList->AddText(texto_pos, cor_texto, label);
    if (tag) {
        ImVec2 tag_tamanho = ImGui::CalcTextSize(tag);
        ImVec2 tag_pos = ImVec2(texto_pos.x + ImGui::CalcTextSize(label).x + 12.0f, pos.y + (altura - tag_tamanho.y) * 0.5f);
        ImU32 tag_fundo = ImGui::GetColorU32(ImVec4(0.90f, 0.20f, 0.20f, 1.0f));
        ImVec2 tag_padding = ImVec2(10.0f, 5.0f);
        ImRect tag_bb = ImRect(ImVec2(tag_pos.x - tag_padding.x, tag_pos.y - tag_padding.y), ImVec2(tag_pos.x + tag_tamanho.x + tag_padding.x, tag_pos.y + tag_tamanho.y + tag_padding.y));
        float tag_rounding = 14.0f;
        window->DrawList->AddRectFilled(tag_bb.Min, tag_bb.Max, tag_fundo, tag_rounding);
        window->DrawList->AddRect(tag_bb.Min, tag_bb.Max, ImGui::GetColorU32(ImVec4(1.0f, 0.3f, 0.3f, 1.0f)), tag_rounding, 0, 2.0f);
        window->DrawList->AddText(tag_pos, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), tag);
    }
    float botao_x = total_bb.Max.x - toggle_diametro - 18.0f;
    float botao_y = pos.y + (altura - toggle_diametro) * 0.5f;
    ImVec2 centro_botao = ImVec2(botao_x + toggle_diametro * 0.5f, botao_y + toggle_diametro * 0.5f);
    float raio = toggle_diametro * 0.5f;
    window->DrawList->AddCircleFilled(centro_botao, raio, cor_botao_redondo);
    ImU32 cor_borda_botao = *v ? ImGui::GetColorU32(ImVec4(panelColor.x * 0.8f, panelColor.y * 0.8f, panelColor.z * 0.8f, 1.0f)) : (temaEscuro ? ImGui::GetColorU32(ImVec4(0.20f, 0.23f, 0.28f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.70f, 0.70f, 0.70f, 1.0f)));
    window->DrawList->AddCircle(centro_botao, raio, cor_borda_botao, 0, BORDER_SIZE);
    return pressed;
}

void TituloSecao(const char* titulo) {
    ImGui::Dummy(ImVec2(0, 12));
    if (temaEscuro) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.83f, 0.87f, 0.9f));
    else ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.10f, 0.10f, 0.10f, 0.9f));
    ImGui::Text("%s", titulo);
    ImGui::PopStyleColor();
    ImGui::Dummy(ImVec2(0, 6));
}

bool CustomCheckbox12(const char* label, bool* v) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const float square_sz = ImGui::GetFrameHeight() * 1.2f;
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, ImVec2(pos.x + square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), pos.y + label_size.y + style.FramePadding.y * 2.0f));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id)) return false;
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (hovered && g.IO.MouseClicked[0]) { *v = !(*v); ImGui::MarkItemEdited(id); pressed = true; }
    const ImRect check_bb(pos, ImVec2(pos.x + square_sz, pos.y + square_sz));
    static std::map<ImGuiID, float> animTimes;
    float& animTime = animTimes[id];
    if (pressed) animTime = 1.0f;
    else if (animTime > 0.0f) { animTime -= g.IO.DeltaTime * 8.0f; if (animTime < 0.0f) animTime = 0.0f; }
    float scale = 1.0f - (animTime * 0.2f);
    ImVec2 center = ImVec2((check_bb.Min.x + check_bb.Max.x) * 0.5f, (check_bb.Min.y + check_bb.Max.y) * 0.5f);
    ImVec2 size = ImVec2((check_bb.Max.x - check_bb.Min.x) * scale, (check_bb.Max.y - check_bb.Min.y) * scale);
    ImRect anim_bb = ImRect(center.x - size.x * 0.5f, center.y - size.y * 0.5f, center.x + size.x * 0.5f, center.y + size.y * 0.5f);
    ImU32 bg_color = *v ? ImGui::GetColorU32(panelColor) : (temaEscuro ? ImGui::GetColorU32(ImVec4(0.15f, 0.18f, 0.22f, 1.0f)) : ImGui::GetColorU32(ImVec4(1.00f, 1.00f, 1.00f, 1.0f)));
    window->DrawList->AddRectFilled(anim_bb.Min, anim_bb.Max, bg_color, BORDER_ROUNDING);
    if (*v) window->DrawList->AddRect(anim_bb.Min, anim_bb.Max, ImGui::GetColorU32(ImVec4(panelColor.x * 1.1f, panelColor.y * 1.1f, panelColor.z * 1.1f, 1.0f)), BORDER_ROUNDING, 0, BORDER_SIZE);
    else window->DrawList->AddRect(anim_bb.Min, anim_bb.Max, temaEscuro ? ImGui::GetColorU32(ImVec4(0.25f, 0.28f, 0.35f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), BORDER_ROUNDING, 0, BORDER_SIZE);
    if (label_size.x > 0.0f) {
        if (temaEscuro) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.83f, 0.87f, 1.0f));
        else ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
        ImGui::RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x + 5.0f, pos.y + style.FramePadding.y), label);
        ImGui::PopStyleColor();
    }
    return pressed;
}

// SLIDER FOV NO ESTILO DA SEGUNDA IMAGEM
bool CustomSliderFloatFOV(const char* label, float* v, float v_min, float v_max, const char* format = "%.1f") {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = ImGui::CalcItemWidth();
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, ImVec2(window->DC.CursorPos.x + w, window->DC.CursorPos.y + 36.0f));
    const ImRect total_bb(frame_bb.Min, ImVec2(frame_bb.Max.x + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), frame_bb.Max.y));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id, &frame_bb)) return false;
    const bool hovered = ImGui::IsMouseHoveringRect(frame_bb.Min, frame_bb.Max);
    ImU32 frame_col = temaEscuro ? ImGui::GetColorU32(ImVec4(0.15f, 0.18f, 0.22f, 1.00f)) : ImGui::GetColorU32(ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
    window->DrawList->AddRectFilled(frame_bb.Min, frame_bb.Max, frame_col, BORDER_ROUNDING);
    ImU32 border_col = temaEscuro ? ImGui::GetColorU32(ImVec4(0.25f, 0.28f, 0.35f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
    window->DrawList->AddRect(frame_bb.Min, frame_bb.Max, border_col, BORDER_ROUNDING, 0, BORDER_SIZE);
    float t = (*v - v_min) / (v_max - v_min);
    ImRect fill_bb = frame_bb;
    fill_bb.Max.x = ImLerp(fill_bb.Min.x, fill_bb.Max.x, t);
    if (t > 0.0f) window->DrawList->AddRectFilled(fill_bb.Min, fill_bb.Max, ImGui::GetColorU32(panelColor), BORDER_ROUNDING);
    char value_buf[64];
    ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), format, *v);
    ImU32 text_col = temaEscuro ? ImGui::GetColorU32(ImVec4(0.80f, 0.83f, 0.87f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
    ImVec2 text_size = ImGui::CalcTextSize(value_buf);
    ImVec2 text_pos = ImVec2(frame_bb.Min.x + (frame_bb.Max.x - frame_bb.Min.x) * 0.5f - text_size.x * 0.5f, frame_bb.Min.y + (frame_bb.Max.y - frame_bb.Min.y) * 0.5f - text_size.y * 0.5f);
    window->DrawList->AddText(text_pos, text_col, value_buf);
    if (label_size.x > 0.0f) {
        if (temaEscuro) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.83f, 0.87f, 1.0f));
        else ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
        ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x + 5.0f, frame_bb.Min.y + (frame_bb.Max.y - frame_bb.Min.y) * 0.5f - label_size.y * 0.5f), label);
        ImGui::PopStyleColor();
    }
    if (hovered && g.IO.MouseClicked[0]) { ImGui::SetActiveID(id, window); ImGui::SetFocusID(id, window); ImGui::FocusWindow(window); }
    if (g.ActiveId == id) {
        if (g.IO.MouseDown[0]) {
            const float mouse_pos_x = g.IO.MousePos.x;
            const float normalized_pos = ImClamp((mouse_pos_x - frame_bb.Min.x) / (frame_bb.Max.x - frame_bb.Min.x), 0.0f, 1.0f);
            *v = v_min + normalized_pos * (v_max - v_min);
            return true;
        } else { ImGui::ClearActiveID(); }
    }
    return false;
}

bool CustomSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.1f") {
    return CustomSliderFloatFOV(label, v, v_min, v_max, format);
}

void menu() {
    ImVec2 imageSize(28, 28);
    ApplyCustomTheme(panelColor, temaEscuro);
    static int activeTab = 1;
    anim = ImClamp(anim + (ImGui::GetIO().DeltaTime / 0.2f) * (isOpening ? 1.0f : -1.0f), 0.0f, 1.0f);    
    ImVec2 winSize = ImVec2(630 * anim, 520 * anim);
    if (anim > 0.0f) {    
        ImGui::SetNextWindowSize(winSize, ImGuiCond_Always);    
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, anim);    
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 20.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    }    
    if (init) {    
        ImGui::PushFont(sansFont);    
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 12.0f));
        if (ImGui::Begin("##MenuFloating", &init, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar)) {    
            ImGui::BeginChild("LeftBar", ImVec2(100, 0), false);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 999.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
            ImU32 iconColorInactive = temaEscuro ? ImGui::GetColorU32(ImVec4(0.50f, 0.53f, 0.60f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.81f, 0.81f, 0.81f, 1.0f));
            ImU32 iconColorActive = ImGui::GetColorU32(panelColor);
            ImU32 borderColor = temaEscuro ? ImGui::GetColorU32(ImVec4(0.30f, 0.33f, 0.40f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.81f, 0.81f, 0.81f, 1.0f));
            if (ImGui::Button("##tab1", ImVec2(75, 75))) activeTab = 1;    
            {  
                bool isActive = (activeTab == 1);
                ImVec2 p = ImGui::GetItemRectMin();   
                ImVec2 s = ImGui::GetItemRectSize();    
                ImVec2 imgPos(p.x + (s.x - imageSize.x) * 0.5f, p.y + (s.y - imageSize.y) * 0.5f);    
                ImU32 iconColor = isActive ? iconColorActive : iconColorInactive;
                ImGui::GetWindowDrawList()->AddImage(iconaim.textureId, imgPos, ImVec2(imgPos.x + imageSize.x, imgPos.y + imageSize.y), ImVec2(0,0), ImVec2(1,1), iconColor);
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                ImVec2 p_min = ImGui::GetItemRectMin();
                ImVec2 p_max = ImGui::GetItemRectMax();
                ImVec2 center = ImVec2((p_min.x + p_max.x) * 0.5f, (p_min.y + p_max.y) * 0.5f);
                float radius = (p_max.x - p_min.x) * 0.5f;
                draw_list->AddCircle(center, radius - 2.0f, borderColor, 0, 0.8f);
            }    
            if (ImGui::Button("##tab2", ImVec2(75, 75))) activeTab = 2;    
            {  
                bool isActive = (activeTab == 2);
                ImVec2 p = ImGui::GetItemRectMin();   
                ImVec2 s = ImGui::GetItemRectSize();    
                ImVec2 imgPos(p.x + (s.x - imageSize.x) * 0.5f, p.y + (s.y - imageSize.y) * 0.5f);    
                ImU32 iconColor = isActive ? iconColorActive : iconColorInactive;
                ImGui::GetWindowDrawList()->AddImage(iconesp.textureId, imgPos, ImVec2(imgPos.x + imageSize.x, imgPos.y + imageSize.y), ImVec2(0,0), ImVec2(1,1), iconColor);
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                ImVec2 p_min = ImGui::GetItemRectMin();
                ImVec2 p_max = ImGui::GetItemRectMax();
                ImVec2 center = ImVec2((p_min.x + p_max.x) * 0.5f, (p_min.y + p_max.y) * 0.5f);
                float radius = (p_max.x - p_min.x) * 0.5f;
                draw_list->AddCircle(center, radius - 2.0f, borderColor, 0, 0.8f);
            }    
            if (ImGui::Button("##tab3", ImVec2(75, 75))) activeTab = 3;    
            {  
                bool isActive = (activeTab == 3);
                ImVec2 p = ImGui::GetItemRectMin();   
                ImVec2 s = ImGui::GetItemRectSize();    
                ImVec2 imgPos(p.x + (s.x - imageSize.x) * 0.5f, p.y + (s.y - imageSize.y) * 0.5f);    
                ImU32 iconColor = isActive ? iconColorActive : iconColorInactive;
                ImGui::GetWindowDrawList()->AddImage(iconmisc.textureId, imgPos, ImVec2(imgPos.x + imageSize.x, imgPos.y + imageSize.y), ImVec2(0,0), ImVec2(1,1), iconColor);
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                ImVec2 p_min = ImGui::GetItemRectMin();
                ImVec2 p_max = ImGui::GetItemRectMax();
                ImVec2 center = ImVec2((p_min.x + p_max.x) * 0.5f, (p_min.y + p_max.y) * 0.5f);
                float radius = (p_max.x - p_min.x) * 0.5f;
                draw_list->AddCircle(center, radius - 2.0f, borderColor, 0, 0.8f);
            }    
            if (ImGui::Button("##tab4", ImVec2(75, 75))) activeTab = 4;    
            {  
                bool isActive = (activeTab == 4);
                ImVec2 p = ImGui::GetItemRectMin();   
                ImVec2 s = ImGui::GetItemRectSize();    
                ImVec2 imgPos(p.x + (s.x - imageSize.x) * 0.5f, p.y + (s.y - imageSize.y) * 0.5f);    
                ImU32 iconColor = isActive ? iconColorActive : iconColorInactive;
                ImGui::GetWindowDrawList()->AddImage(iconinfo.textureId, imgPos, ImVec2(imgPos.x + imageSize.x, imgPos.y + imageSize.y), ImVec2(0,0), ImVec2(1,1), iconColor);
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                ImVec2 p_min = ImGui::GetItemRectMin();
                ImVec2 p_max = ImGui::GetItemRectMax();
                ImVec2 center = ImVec2((p_min.x + p_max.x) * 0.5f, (p_min.y + p_max.y) * 0.5f);
                float radius = (p_max.x - p_min.x) * 0.5f;
                draw_list->AddCircle(center, radius - 2.0f, borderColor, 0, 0.8f);
            }    
            ImGui::PopStyleVar(2);
            ImGui::EndChild();    
            ImGui::SameLine(0, 10);    
            ImGui::BeginChild("ContentArea", ImVec2(0, 0), false);    
            if (activeTab == 1) { CabecalhoTabComClose(iconaim.textureId, "AIMBOT", &init); } 
            else if (activeTab == 2) { CabecalhoTabComClose(iconesp.textureId, "ESP", &init); } 
            else if (activeTab == 3) { CabecalhoTabComClose(iconmisc.textureId, "DIVERSOS", &init); } 
            else if (activeTab == 4) { CabecalhoTabComClose(iconinfo.textureId, "INFORMAÇÕES", &init); }
            ImGui::Dummy(ImVec2(0, 18));
            if (activeTab == 1) {
                ImGui::BeginChild("AimbotScroll", ImVec2(0, ImGui::GetContentRegionAvail().y - 10), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
                NovoBotaoEstilo("Habilitar Aimblt", &Active);
                NovoBotaoEstilo("Aimbot Legit", &AimbotRage);
                NovoBotaoEstilo("Exibir Fov", &AimbotLegit);
                NovoBotaoEstilo("Exibir Linha", &EspCircle);
                ImGui::Dummy(ImVec2(0, 15));
                ImGuiWindow* window = ImGui::GetCurrentWindow();
                ImVec2 text_pos = window->DC.CursorPos;
                float available_width = ImGui::GetContentRegionAvail().x;
                if (temaEscuro) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.83f, 0.87f, 1.0f));
                else ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
                ImGui::Text("FOV:");
                ImGui::PopStyleColor();
                char fov_text[32];
                snprintf(fov_text, sizeof(fov_text), "[%.0f°]", Fov);
                ImVec2 fov_value_size = ImGui::CalcTextSize(fov_text);
                float right_padding = 5.0f;
                float fov_value_x = text_pos.x + available_width - fov_value_size.x - right_padding;
                float fov_text_width = ImGui::CalcTextSize("FOV:").x + 10.0f;
                if (fov_value_x < text_pos.x + fov_text_width) fov_value_x = text_pos.x + fov_text_width;
                ImVec2 fov_value_pos = ImVec2(fov_value_x, text_pos.y);
                window->DrawList->AddText(fov_value_pos, ImGui::GetColorU32(panelColor), fov_text);
                ImGui::Dummy(ImVec2(0, 8));
                ImVec2 card_pos = window->DC.CursorPos;
                float card_width = ImGui::GetContentRegionAvail().x - 5.0f;
                float card_height = 60.0f;
                const float FOV_ROUNDING = 38.0f;
                const float FOV_BORDER_SIZE = 0.8f;
                ImU32 card_bg = temaEscuro ? ImGui::GetColorU32(ImVec4(0.15f, 0.18f, 0.22f, 1.00f)) : ImGui::GetColorU32(ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
                ImU32 card_border = temaEscuro ? ImGui::GetColorU32(ImVec4(0.25f, 0.28f, 0.35f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
                window->DrawList->AddRectFilled(card_pos, ImVec2(card_pos.x + card_width, card_pos.y + card_height), card_bg, FOV_ROUNDING);
                window->DrawList->AddRect(card_pos, ImVec2(card_pos.x + card_width, card_pos.y + card_height), card_border, FOV_ROUNDING, 0, FOV_BORDER_SIZE);
                float slider_y = card_pos.y + card_height/2;
                float slider_width = card_width - 40.0f;
                ImRect slider_bb = ImRect(ImVec2(card_pos.x + 20.0f, slider_y - 4.0f), ImVec2(card_pos.x + 20.0f + slider_width, slider_y + 4.0f));
                ImU32 slider_bg = temaEscuro ? ImGui::GetColorU32(ImVec4(0.10f, 0.12f, 0.16f, 1.00f)) : ImGui::GetColorU32(ImVec4(0.90f, 0.90f, 0.90f, 1.00f));
                window->DrawList->AddRectFilled(ImVec2(slider_bb.Min.x, slider_y - 2.0f), ImVec2(slider_bb.Max.x, slider_y + 2.0f), slider_bg, 2.0f);
                float t = (Fov - 0.0f) / (760.0f - 0.0f);
                float filled_width = slider_width * t;
                if (filled_width > 0) {
                    ImRect filled_bb = ImRect(ImVec2(slider_bb.Min.x, slider_y - 2.0f), ImVec2(slider_bb.Min.x + filled_width, slider_y + 2.0f));
                    ImU32 filled_color = ImGui::GetColorU32(panelColor);
                    window->DrawList->AddRectFilled(filled_bb.Min, filled_bb.Max, filled_color, 2.0f);
                }
                float marker_x = slider_bb.Min.x + slider_width * t;
                ImVec2 marker_center = ImVec2(marker_x, slider_y);
                ImU32 marker_color = ImGui::GetColorU32(panelColor);
                window->DrawList->AddCircleFilled(marker_center, 8.0f, marker_color);
                ImU32 marker_border_color = temaEscuro ? ImGui::GetColorU32(ImVec4(0.25f, 0.28f, 0.35f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
                window->DrawList->AddCircle(marker_center, 8.0f, marker_border_color, 0, 0.6f);
                static bool slider_active = false;
                static ImGuiID slider_id = 0;
                bool slider_hovered = ImGui::IsMouseHoveringRect(ImVec2(slider_bb.Min.x, slider_bb.Min.y - 15.0f), ImVec2(slider_bb.Max.x, slider_bb.Max.y + 15.0f));
                if (slider_hovered && ImGui::IsMouseClicked(0)) { slider_active = true; slider_id = ImGui::GetID("##FovSlider"); ImGui::SetActiveID(slider_id, window); ImGui::SetFocusID(slider_id, window); ImGui::FocusWindow(window); }
                if (slider_active && ImGui::IsMouseDown(0)) {
                    float mouse_x = ImGui::GetMousePos().x;
                    float normalized = (mouse_x - slider_bb.Min.x) / slider_width;
                    normalized = ImClamp(normalized, 0.0f, 1.0f);
                    Fov = normalized * 760.0f;
                } else if (slider_active && !ImGui::IsMouseDown(0)) {
                    slider_active = false;
                    if (ImGui::GetActiveID() == slider_id) ImGui::ClearActiveID();
                }
                ImGui::Dummy(ImVec2(0, card_height + 8));
                ImGui::Spacing();
                ImGui::Spacing();
                const char* items[] = { "Cabeça", "Pescoço", "Peito", "Quadril" };
                const char* current_item = items[aimPosition];
                if (temaEscuro) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.83f, 0.87f, 1.0f));
                else ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
                ImGui::Text("Puxada:");
                ImGui::PopStyleColor();
                ImGui::SameLine();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5.0f);
                if (temaEscuro) {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.18f, 0.22f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.20f, 0.23f, 0.28f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, panelColor);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.18f, 0.22f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.20f, 0.23f, 0.28f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, panelColor);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.83f, 0.87f, 1.0f));
                } else {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95f, 0.95f, 0.95f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, panelColor);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.95f, 0.95f, 0.95f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, panelColor);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
                }
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, BORDER_ROUNDING);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 8.0f));
                if (ImGui::BeginCombo("##PuxadaCombo", current_item, ImGuiComboFlags_NoArrowButton)) {
                    for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
                        bool is_selected = (aimPosition == n);
                        if (ImGui::Selectable(items[n], is_selected)) aimPosition = n;
                        if (is_selected) ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor(7);
                ImGui::Spacing();
                ImGui::Spacing();
                if (temaEscuro) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.83f, 0.87f, 1.0f));
                else ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
                ImGui::Text("Tipo de Aimbot:");
                ImGui::PopStyleColor();
                ImGui::Spacing();
                if (temaEscuro) {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.18f, 0.22f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, panelColor);
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, panelColor);
                    ImGui::PushStyleColor(ImGuiCol_CheckMark, panelColor);
                } else {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, panelColor);
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, panelColor);
                    ImGui::PushStyleColor(ImGuiCol_CheckMark, panelColor);
                }
                if (temaEscuro) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.83f, 0.87f, 1.0f));
                else ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
                if (ImGui::RadioButton("Ao Olhar", AimbotAuto)) { AimbotAuto = true; HidePanel = false; }
                ImGui::SameLine();
                if (ImGui::RadioButton("Ao Atirar", !AimbotAuto)) { AimbotAuto = false; HidePanel = true; }
                ImGui::PopStyleColor(temaEscuro ? 5 : 5);
                ImGui::EndChild();
            }
            if (activeTab == 2) {
                ImGui::BeginChild("ESPScroll", ImVec2(0, 0), false);
                NovoBotaoEstilo("ESP Linha", &EspLine);
                NovoBotaoEstilo("ESP Caixa", &EspBox);
                NovoBotaoEstilo("ESP Vida", &EspVida);
                NovoBotaoEstilo("ESP Nome", &Nick);
                NovoBotaoEstilo("ESP Inimigos", &Inimigos);
                NovoBotaoEstilo("ESP Skeleton", &EspSkeleton);
                NovoBotaoEstilo("ESP Info", &EspInfo);
                ImGui::EndChild();
            } 
            if (activeTab == 3) {
                ImGui::BeginChild("MiscScroll", ImVec2(0, 0), false);
                NovoBotaoEstilo("Aim Silent", &botaoselint);
                NovoBotaoEstilo("Voar Player", &botaovp);
                NovoBotaoEstilo("Speed Hack ", &botaospeedhack);
                NovoBotaoEstilo("Tp Player", &botaoTp);
                ImGui::PushItemWidth(250);
                if (temaEscuro) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.83f, 0.87f, 1.0f));
                if (ImGui::ColorEdit3("##CorPainelLinhas", (float*)&panelColor, ImGuiColorEditFlags_NoInputs)) lineColor = panelColor;  
                ImGui::SameLine();
                ImGui::ColorEdit3("Cor ESP", (float*)&ThemeColer, ImGuiColorEditFlags_NoInputs);
                if (temaEscuro) ImGui::PopStyleColor();
                ImGui::Dummy(ImVec2(0, 10));
                ImVec2 savePos = ImGui::GetCursorScreenPos();
                ImVec2 saveSize = ImVec2(155, 48);
                ImRect saveRect(savePos, ImVec2(savePos.x + saveSize.x, savePos.y + saveSize.y));
                bool saveHovered = ImGui::IsMouseHoveringRect(saveRect.Min, saveRect.Max);
                ImU32 saveColor = saveHovered ? (temaEscuro ? ImGui::GetColorU32(ImVec4(0.20f, 0.23f, 0.28f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.95f, 0.95f, 0.95f, 1.0f))) : (temaEscuro ? ImGui::GetColorU32(ImVec4(0.15f, 0.18f, 0.22f, 1.0f)) : ImGui::GetColorU32(ImVec4(1.00f, 1.00f, 1.00f, 1.0f)));
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                draw_list->AddRectFilled(saveRect.Min, saveRect.Max, saveColor, BORDER_ROUNDING);
                ImU32 border_color = temaEscuro ? ImGui::GetColorU32(ImVec4(0.25f, 0.28f, 0.35f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
                draw_list->AddRect(saveRect.Min, saveRect.Max, border_color, BORDER_ROUNDING, 0, BORDER_SIZE);
                ImVec2 textSize = ImGui::CalcTextSize("Salvar Config");
                ImVec2 textPos = ImVec2(savePos.x + (saveSize.x - textSize.x) * 0.5f, savePos.y + (saveSize.y - textSize.y) * 0.5f);
                ImU32 textColor = temaEscuro ? ImGui::GetColorU32(ImVec4(0.80f, 0.83f, 0.87f, 1.0f)) : ImGui::GetColorU32(ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
                draw_list->AddText(textPos, textColor, "Salvar Config");
                if (ImGui::InvisibleButton("##SalvarConfig", saveSize)) {}
                ImGui::EndChild();
            }    
            if (activeTab == 4) {
                ImGui::BeginChild("InfoScroll", ImVec2(0, 0), false);
                if (temaEscuro) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.83f, 0.87f, 1.0f));
                else ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
                ImGui::Text("Desenvolvido Por:"); ImGui::SameLine();
                ImGui::TextColored(panelColor, "@PURPOU.wq");
                ImGui::Text("Versão do Jogo:"); ImGui::SameLine();
                ImGui::TextColored(panelColor, "Free Fire MAX");
                ImGui::Text("Key de Acesso:"); ImGui::SameLine();
                ImGui::TextColored(panelColor, "Souzaxiters-hour-00");
                ImGui::PopStyleColor();
                ImGui::EndChild();
            }    
            ImGui::EndChild();    
        }    
        ImGui::End();    
        ImGui::PopStyleVar(); 
        if (sansFont) ImGui::PopFont();    
    }    
    if (anim > 0.0f) ImGui::PopStyleVar(3);
}

void RenderWithTripleTap() {
    ImGuiIO& io = ImGui::GetIO();
    double currentTime = ImGui::GetTime();
    int fingersDown = 0;    
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) {    
        if (io.MouseDown[i]) fingersDown++;    
    }    
    if (fingersDown >= 3) {    
        tapCount++;    
        if (tapCount == 1) lastTapTime = currentTime;    
        if (tapCount == 3 && (currentTime - lastTapTime) < 0.8) {    
            showMenu = !showMenu;    
            isOpening = showMenu;    
            tapCount = 0;    
        } else if ((currentTime - lastTapTime) > 0.8) {    
            tapCount = 1;    
            lastTapTime = currentTime;    
        }    
    } else if ((currentTime - lastTapTime) > 0.8 && tapCount > 0) {    
        tapCount = 0;    
    }    
    if (showMenu) menu();
}
