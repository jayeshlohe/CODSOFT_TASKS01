#include <windows.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

HWND display;
HFONT displayFont;
double firstNumber = 0;
char operation = '\0';
bool startNewNumber = true;

void setDisplay(string text) {
    SetWindowTextA(display, text.c_str());
}

string getDisplay() {
    char text[256] = {};
    GetWindowTextA(display, text, sizeof(text));
    return text;
}

void calculate() {
    double secondNumber = stod(getDisplay());
    double result = 0;

    switch (operation) {
        case '+':
            result = firstNumber + secondNumber;
            break;

        case '-':
            result = firstNumber - secondNumber;
            break;

        case '*':
            result = firstNumber * secondNumber;
            break;

        case '/':
            if (secondNumber == 0) {
                setDisplay("Cannot divide by zero");
                operation = '\0';
                startNewNumber = true;
                return;
            }
            result = firstNumber / secondNumber;
            break;
    }

    if (!isfinite(result)) {
        setDisplay("Error");
        operation = '\0';
        startNewNumber = true;
        return;
    }

    ostringstream output;
    output << setprecision(12) << defaultfloat << result;
    setDisplay(output.str());

    operation = '\0';
    startNewNumber = true;
}

LRESULT CALLBACK WindowProcedure(
    HWND window, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message) {

        case WM_CREATE: {
            display = CreateWindowA(
                "EDIT", "0",
                WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
                15, 20, 290, 65,
                window, NULL, NULL, NULL
            );

            displayFont = CreateFontA(
                36, 0, 0, 0, FW_NORMAL,
                FALSE, FALSE, FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE,
                "Segoe UI"
            );

            SendMessageA(
                display, WM_SETFONT,
                (WPARAM)displayFont, TRUE
            );

            const char* labels[16] = {
                "C", "/", "*", "-",
                "7", "8", "9", "+",
                "4", "5", "6", "=",
                "1", "2", "3", "0"
            };

            for (int i = 0; i < 16; i++) {
                int row = i / 4;
                int col = i % 4;

                int x = 15 + col * 73;
                int y = 100 + row * 65;

                HWND button = CreateWindowA(
                    "BUTTON", labels[i],
                    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    x, y, 67, 57,
                    window,
                    (HMENU)(INT_PTR)(i + 1),
                    NULL, NULL
                );

                SendMessageA(
                    button, WM_SETFONT,
                    (WPARAM)GetStockObject(DEFAULT_GUI_FONT),
                    TRUE
                );
            }

            return 0;
        }

        case WM_COMMAND: {
            int id = LOWORD(wParam);

            if (id < 1 || id > 16)
                break;

            const char* labels[16] = {
                "C", "/", "*", "-",
                "7", "8", "9", "+",
                "4", "5", "6", "=",
                "1", "2", "3", "0"
            };

            string key = labels[id - 1];
            string current = getDisplay();

            if (key == "C") {
                setDisplay("0");
                firstNumber = 0;
                operation = '\0';
                startNewNumber = true;
            }
            else if (
                key == "+" || key == "-" ||
                key == "*" || key == "/"
            ) {
                try {
                    firstNumber = stod(current);
                    operation = key[0];
                    startNewNumber = true;
                }
                catch (...) {
                    setDisplay("Error");
                    operation = '\0';
                    startNewNumber = true;
                }
            }
            else if (key == "=") {
                if (operation != '\0') {
                    try {
                        calculate();
                    }
                    catch (...) {
                        setDisplay("Error");
                        operation = '\0';
                        startNewNumber = true;
                    }
                }
            }
            else {
                if (startNewNumber) {
                    current = key;
                    startNewNumber = false;
                }
                else if (current.size() < 18) {
                    if (current == "0")
                        current = key;
                    else
                        current += key;
                }

                setDisplay(current);
            }

            return 0;
        }

        case WM_DESTROY:
            if (displayFont)
                DeleteObject(displayFont);

            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcA(
        window, message, wParam, lParam
    );
}

int WINAPI WinMain(
    HINSTANCE instance,
    HINSTANCE,
    LPSTR,
    int showCommand
) {
    const char CLASS_NAME[] = "SimpleCalculatorWindow";

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = instance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassA(&wc))
        return 1;

    HWND window = CreateWindowA(
        CLASS_NAME,
        "Simple Calculator",
        WS_OVERLAPPED | WS_CAPTION |
        WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        340, 460,
        NULL, NULL, instance, NULL
    );

    if (!window)
        return 1;

    ShowWindow(window, showCommand);
    UpdateWindow(window);

    MSG message = {};

    while (GetMessageA(&message, NULL, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return 0;
}

// use g++ main.cpp -o SimpleCalculator.exe -lgdi32 -mwindows &
// use .\SimpleCalculator.exe
