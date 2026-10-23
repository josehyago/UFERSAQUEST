// ui.c
#include "ui.h"

static Font fonteUI;                 // fonte usada em todos os textos do jogo
static bool fonteCarregada = false;  // false = usa a fonte padrão da Raylib (sem acentos)

// FUNÇÃO: carregarFonteUI
// PARÂMETROS: caminho - arquivo .ttf da fonte (ex: "../assets/fonts/open-sans.bold.ttf").
// POR QUE: a fonte padrão da Raylib só tem ASCII, então "ê", "ç", "ã" etc. não apareceriam.
// PARA QUE SERVE: carrega a fonte com os caracteres de 32 a 255 (ASCII + Latin-1), que cobrem o português. Deve ser chamada DEPOIS do InitWindow.
void carregarFonteUI(const char *caminho){
    // Monta a lista de caracteres (codepoints) que serão carregados da fonte.
    int cod[224];
    for (int i = 0; i < 224; i++) cod[i] = 32 + i;

    fonteUI = LoadFontEx(caminho, 48, cod, 224);
    if (IsFontValid(fonteUI)){
        SetTextureFilter(fonteUI.texture, TEXTURE_FILTER_BILINEAR); // texto suave ao reduzir o tamanho
        fonteCarregada = true;
    }
}

// FUNÇÃO: descarregarFonteUI
// PARÂMETROS: nenhum.
// POR QUE: a fonte ocupa memória da placa de vídeo e precisa ser liberada.
// PARA QUE SERVE: libera a fonte carregada. Deve ser chamada antes do CloseWindow.
void descarregarFonteUI(void){
    if (fonteCarregada) UnloadFont(fonteUI);
    fonteCarregada = false;
}

// FUNÇÃO: desenharTexto
// PARÂMETROS: texto - o que escrever; x e y - posição do canto superior esquerdo; tamanho - altura da letra em pixels; cor - cor do texto.
// POR QUE: centraliza a escolha da fonte num só lugar, em vez de espalhar DrawText pelo código.
// PARA QUE SERVE: escreve o texto com a fonte do jogo (com acentos). Se a fonte não carregou, usa a padrão para o jogo continuar funcionando.
void desenharTexto(const char *texto, int x, int y, int tamanho, Color cor){
    if (fonteCarregada) DrawTextEx(fonteUI, texto, (Vector2){ x, y }, (float)tamanho, tamanho / 10.0f, cor);
    else DrawText(texto, x, y, tamanho, cor);
}

// FUNÇÃO: medirTexto
// PARÂMETROS: texto e tamanho - os mesmos usados em desenharTexto.
// POR QUE: para centralizar ou alinhar à direita é preciso saber a largura que o texto vai ocupar.
// PARA QUE SERVE: RETORNA a largura, em pixels, do texto desenhado com a fonte do jogo.
int medirTexto(const char *texto, int tamanho){
    if (fonteCarregada) return (int)MeasureTextEx(fonteUI, texto, (float)tamanho, tamanho / 10.0f).x;
    return MeasureText(texto, tamanho);
}

// FUNÇÃO: desenharTextoCentralizado
// PARÂMETROS: texto, y (altura), tamanho e cor.
// POR QUE: a conta "(largura da tela - largura do texto) / 2" estava repetida em vários lugares.
// PARA QUE SERVE: escreve o texto centralizado na horizontal, na altura y.
void desenharTextoCentralizado(const char *texto, int y, int tamanho, Color cor){
    int x = (GetScreenWidth() - medirTexto(texto, tamanho)) / 2;
    desenharTexto(texto, x, y, tamanho, cor);
}