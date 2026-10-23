#include "ui.h"

static Font fonteUI;
static bool fonteCarregada = false;

void carregarFonteUI(const char *caminho){
    int cod[224];
    for (int i = 0; i < 224; i++) cod[i] = 32 + i; // 32..255: ASCII + Latin-1 (acentos)

    fonteUI = LoadFontEx(caminho, 48, cod, 224);
    if (IsFontValid(fonteUI)){
        SetTextureFilter(fonteUI.texture, TEXTURE_FILTER_BILINEAR); // texto suave ao reduzir
        fonteCarregada = true;
    }
}

void descarregarFonteUI(void){
    if (fonteCarregada) UnloadFont(fonteUI);
}

void desenharTexto(const char *texto, int x, int y, int tamanho, Color cor){
    if (fonteCarregada) DrawTextEx(fonteUI, texto, (Vector2){ x, y }, (float)tamanho, tamanho / 10.0f, cor);
    else DrawText(texto, x, y, tamanho, cor); // fallback: sem crash se a fonte faltar
}

int medirTexto(const char *texto, int tamanho){
    if (fonteCarregada) return (int)MeasureTextEx(fonteUI, texto, (float)tamanho, tamanho / 10.0f).x;
    return MeasureText(texto, tamanho);
}