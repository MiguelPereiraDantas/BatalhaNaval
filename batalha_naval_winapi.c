#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ID_BTN_BASE 1000
#define MAX_TAM 15
#define IDM_NOVO 2000
#define IDM_FACIL 2001
#define IDM_MEDIO 2002
#define IDM_DIFICIL 2003
#define IDM_TAM5 2004
#define IDM_TAM10 2005
#define IDM_TAM15 2006
#define IDM_SAIR 2007

int tam = 10;
int dificuldade = 1; // 1=fácil, 2=médio, 3=difícil
HWND botoes[MAX_TAM][MAX_TAM] = {{0}};
int tabuleiro[MAX_TAM][MAX_TAM] = {{0}};     // tabuleiro jogador (onde IA atira)
int tabuleiroIA[MAX_TAM][MAX_TAM] = {{0}};  // tabuleiro IA (onde jogador atira)
int vezJogador = 1; // 1 = jogador, 0 = IA

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void AjustarTamanhoJanela(HWND hwnd) {
    int largura = 40 * tam + 60;  // botões + margem
    int altura = 40 * tam + 100;
    RECT rc;
    GetWindowRect(hwnd, &rc);
    SetWindowPos(hwnd, NULL, 0, 0, largura, altura, SWP_NOMOVE | SWP_NOZORDER);
}

void RemoverBotoes() {
    for (int i = 0; i < MAX_TAM; i++) {
        for (int j = 0; j < MAX_TAM; j++) {
            if (botoes[i][j]) {
                DestroyWindow(botoes[i][j]);
                botoes[i][j] = NULL;
            }
        }
    }
}

void CriarTabuleiro(HWND hwnd) {
    RemoverBotoes();
    AjustarTamanhoJanela(hwnd);
    int cellSize = 40;
    int startX = 20, startY = 50;

    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            botoes[i][j] = CreateWindow(
                "BUTTON", "",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
                startX + j * cellSize, startY + i * cellSize, cellSize, cellSize,
                hwnd, (HMENU)(ID_BTN_BASE + i * tam + j),
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
            );
            tabuleiro[i][j] = 0;
            tabuleiroIA[i][j] = 0;
        }
    }
}

void PosicionarNavios(int matriz[MAX_TAM][MAX_TAM]) {
    for (int i = 0; i < tam; i++)
        for (int j = 0; j < tam; j++)
            matriz[i][j] = 0;

    int navios = tam;
    while (navios > 0) {
        int x = rand() % tam;
        int y = rand() % tam;
        if (matriz[x][y] == 0) {
            matriz[x][y] = 1; // navio
            navios--;
        }
    }
}

void AtualizarTabuleiro(HWND hwnd) {
    InvalidateRect(hwnd, NULL, TRUE);
}

int VerificarVitoria(int matriz[MAX_TAM][MAX_TAM]) {
    for (int i = 0; i < tam; i++)
        for (int j = 0; j < tam; j++)
            if (matriz[i][j] == 1) // navio não atingido
                return 0;
    return 1;
}

void MostrarMensagem(HWND hwnd, const char *msg) {
    MessageBox(hwnd, msg, "Fim de Jogo", MB_OK | MB_ICONINFORMATION);
}

void JogadaIA(HWND hwnd);

void JogadaJogador(HWND hwnd, int i, int j) {
    if (vezJogador == 0) return; // não é a vez do jogador

    if (tabuleiroIA[i][j] <= 1) {
        if (tabuleiroIA[i][j] == 1)
            tabuleiroIA[i][j] = 3; // acerto
        else
            tabuleiroIA[i][j] = 2; // erro

        AtualizarTabuleiro(hwnd);

        if (VerificarVitoria(tabuleiroIA)) {
            MostrarMensagem(hwnd, "Parabéns, você venceu!");
            vezJogador = 0;
            return;
        }

        vezJogador = 0;
        JogadaIA(hwnd);
    }
}

void JogadaIA(HWND hwnd) {
    int tentativas = dificuldade * 5; // mais dificuldade, mais tentativas para acertar
    while (tentativas--) {
        int x = rand() % tam;
        int y = rand() % tam;
        if (tabuleiro[x][y] <= 1) {
            if (tabuleiro[x][y] == 1)
                tabuleiro[x][y] = 3; // acerto
            else
                tabuleiro[x][y] = 2; // erro

            AtualizarTabuleiro(hwnd);

            if (VerificarVitoria(tabuleiro)) {
                MostrarMensagem(hwnd, "Você perdeu, a IA venceu.");
                vezJogador = 0;
                return;
            }

            break;
        }
    }
    vezJogador = 1; // volta pro jogador
}

void CriarMenu(HWND hwnd) {
    HMENU hMenu = CreateMenu();
    HMENU hJogo = CreatePopupMenu();
    HMENU hDif = CreatePopupMenu();
    HMENU hTam = CreatePopupMenu();

    AppendMenu(hJogo, MF_STRING, IDM_NOVO, "Nova Partida");
    AppendMenu(hJogo, MF_STRING, IDM_SAIR, "Sair");

    AppendMenu(hDif, MF_STRING, IDM_FACIL, "Fácil");
    AppendMenu(hDif, MF_STRING, IDM_MEDIO, "Médio");
    AppendMenu(hDif, MF_STRING, IDM_DIFICIL, "Difícil");

    AppendMenu(hTam, MF_STRING, IDM_TAM5, "5x5");
    AppendMenu(hTam, MF_STRING, IDM_TAM10, "10x10");
    AppendMenu(hTam, MF_STRING, IDM_TAM15, "15x15");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hJogo, "Jogo");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hDif, "Dificuldade");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hTam, "Tamanho");

    SetMenu(hwnd, hMenu);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        CriarMenu(hwnd);
        CriarTabuleiro(hwnd);
        PosicionarNavios(tabuleiro);
        PosicionarNavios(tabuleiroIA);
        vezJogador = 1;
        break;

    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id >= ID_BTN_BASE && id < ID_BTN_BASE + tam * tam) {
            if (vezJogador == 1) {
                int i = (id - ID_BTN_BASE) / tam;
                int j = (id - ID_BTN_BASE) % tam;
                JogadaJogador(hwnd, i, j);
            }
        }
        else if (id == IDM_NOVO) {
            CriarTabuleiro(hwnd);
            PosicionarNavios(tabuleiro);
            PosicionarNavios(tabuleiroIA);
            vezJogador = 1;
            AtualizarTabuleiro(hwnd);
        }
        else if (id == IDM_FACIL) dificuldade = 1;
        else if (id == IDM_MEDIO) dificuldade = 2;
        else if (id == IDM_DIFICIL) dificuldade = 3;
        else if (id == IDM_TAM5) {
            tam = 5;
            CriarTabuleiro(hwnd);
            PosicionarNavios(tabuleiro);
            PosicionarNavios(tabuleiroIA);
            vezJogador = 1;
            AtualizarTabuleiro(hwnd);
        }
        else if (id == IDM_TAM10) {
            tam = 10;
            CriarTabuleiro(hwnd);
            PosicionarNavios(tabuleiro);
            PosicionarNavios(tabuleiroIA);
            vezJogador = 1;
            AtualizarTabuleiro(hwnd);
        }
        else if (id == IDM_TAM15) {
            tam = 15;
            CriarTabuleiro(hwnd);
            PosicionarNavios(tabuleiro);
            PosicionarNavios(tabuleiroIA);
            vezJogador = 1;
            AtualizarTabuleiro(hwnd);
        }
        else if (id == IDM_SAIR) PostQuitMessage(0);
        break;
    }

    case WM_DRAWITEM: {
        LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
        int id = pdis->CtlID;
        if (id >= ID_BTN_BASE && id < ID_BTN_BASE + tam * tam) {
            int i = (id - ID_BTN_BASE) / tam;
            int j = (id - ID_BTN_BASE) % tam;

            HBRUSH brush;

            // Mostrar os navios IA ocultos (1) como água (0) para o jogador
            int val = tabuleiroIA[i][j];
            if (val == 1) val = 0;

            if (val == 0) brush = CreateSolidBrush(RGB(0, 150, 255));  // água
            else if (val == 2) brush = CreateSolidBrush(RGB(200, 200, 200)); // erro
            else if (val == 3) brush = CreateSolidBrush(RGB(255, 0, 0)); // acerto
            else brush = CreateSolidBrush(RGB(0, 0, 0)); // fallback

            FillRect(pdis->hDC, &pdis->rcItem, brush);
            DeleteObject(brush);
        }
        return TRUE;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    srand((unsigned)time(NULL));
    const char CLASS_NAME[] = "BatalhaNaval";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Batalha Naval - WinAPI",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 650,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
