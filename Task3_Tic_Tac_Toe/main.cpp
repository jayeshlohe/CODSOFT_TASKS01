
#include <windows.h>
#include <string>

using namespace std;

HWND cells[9];
HWND statusText;
HFONT cellFont = NULL;
HFONT statusFont = NULL;

char board[9];
char currentPlayer = 'X';
bool gameOver = false;

void updateStatus() {
    string message = "Player ";
    message += currentPlayer;
    message += "'s Turn";

    SetWindowTextA(statusText, message.c_str());
}

bool checkWinner() {
    const int wins[8][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8},
        {2, 4, 6}
    };

    for (int i = 0; i < 8; i++) {
        int a = wins[i][0];
        int b = wins[i][1];
        int c = wins[i][2];

        if (board[a] != ' ' &&
            board[a] == board[b] &&
            board[b] == board[c]) {
            return true;
        }
    }

    return false;
}

bool boardIsFull() {
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ')
            return false;
    }

    return true;
}

void resetGame() {
    for (int i = 0; i < 9; i++) {
        board[i] = ' ';
        SetWindowTextA(cells[i], "");
        EnableWindow(cells[i], TRUE);
    }

    currentPlayer = 'X';
    gameOver = false;
    updateStatus();
}

LRESULT CALLBACK WindowProcedure(
    HWND window, UINT message,
    WPARAM wParam, LPARAM lParam) {

    switch (message) {

    case WM_CREATE: {
        statusText = CreateWindowA(
            "STATIC", "Player X's Turn",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            20, 15, 320, 40,
            window, NULL, NULL, NULL
        );

        statusFont = CreateFontA(
            24, 0, 0, 0, FW_BOLD,
            FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE,
            "Segoe UI"
        );

        SendMessageA(
            statusText, WM_SETFONT,
            (WPARAM)statusFont, TRUE
        );

        cellFont = CreateFontA(
            34, 0, 0, 0, FW_BOLD,
            FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE,
            "Segoe UI"
        );

        for (int i = 0; i < 9; i++) {
            int row = i / 3;
            int col = i % 3;

            board[i] = ' ';

            cells[i] = CreateWindowA(
                "BUTTON", "",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                30 + col * 100,
                70 + row * 100,
                90, 90,
                window,
                (HMENU)(INT_PTR)(i + 1),
                NULL, NULL
            );

            SendMessageA(
                cells[i], WM_SETFONT,
                (WPARAM)cellFont, TRUE
            );
        }

        HWND restartButton = CreateWindowA(
            "BUTTON", "New Game",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            110, 385, 140, 40,
            window,
            (HMENU)(INT_PTR)10,
            NULL, NULL
        );

        SendMessageA(
            restartButton, WM_SETFONT,
            (WPARAM)GetStockObject(DEFAULT_GUI_FONT),
            TRUE
        );

        return 0;
    }

    case WM_COMMAND: {
        int id = LOWORD(wParam);

        if (id == 10) {
            resetGame();
            return 0;
        }

        if (id < 1 || id > 9 || gameOver)
            return 0;

        int index = id - 1;

        if (board[index] != ' ')
            return 0;

        board[index] = currentPlayer;

        string mark(1, currentPlayer);
        SetWindowTextA(cells[index], mark.c_str());

        if (checkWinner()) {
            string message = "Player ";
            message += currentPlayer;
            message += " wins!";

            SetWindowTextA(
                statusText, message.c_str()
            );

            gameOver = true;

            for (int i = 0; i < 9; i++)
                EnableWindow(cells[i], FALSE);

            return 0;
        }

        if (boardIsFull()) {
            SetWindowTextA(
                statusText, "It's a Draw!"
            );

            gameOver = true;

            for (int i = 0; i < 9; i++)
                EnableWindow(cells[i], FALSE);

            return 0;
        }

        currentPlayer =
            (currentPlayer == 'X') ? 'O' : 'X';

        updateStatus();
        return 0;
    }

    case WM_DESTROY:
        if (cellFont)
            DeleteObject(cellFont);

        if (statusFont)
            DeleteObject(statusFont);

        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcA(
        window, message, wParam, lParam
    );
}

int WINAPI WinMain(
    HINSTANCE instance, HINSTANCE,
    LPSTR, int showCommand) {

    const char CLASS_NAME[] = "TicTacToeWindow";

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = instance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassA(&wc))
        return 1;

    HWND window = CreateWindowA(
        CLASS_NAME, "Tic-Tac-Toe",
        WS_OVERLAPPED | WS_CAPTION |
        WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        380, 480,
        NULL, NULL, instance, NULL
    );

    if (!window)
        return 1;

    ShowWindow(window, showCommand);
    UpdateWindow(window);

    MSG message = {};

    while (GetMessageA(
        &message, NULL, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return 0;
}
// use g++ main.cpp -o TicTacToe.exe -lgdi32 -mwindows
// .\TicTacToe.exe
