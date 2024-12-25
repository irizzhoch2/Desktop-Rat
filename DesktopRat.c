#include <windows.h>

const char CLASS_NAME[] = "MeineFensterklasse";
static int yPosBottom = 0;
static POINT mouseOffset;
static BOOL isDragging = FALSE;
static int width = 500;
static int height = 200;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);  
        return 0;

    case WM_LBUTTONDOWN:
        isDragging = TRUE;
        POINT mousePos;
        GetCursorPos(&mousePos);

        // Hole Fensterposition
        RECT rect;
        GetWindowRect(hwnd, &rect);

        // Berechne Offset zwischen Klickpunkt und Fensterecke
        mouseOffset.x = mousePos.x - rect.left;
        mouseOffset.y = mousePos.y - rect.top;

        return 0;

    case WM_LBUTTONUP:
        isDragging = FALSE;
        GetWindowRect(hwnd, &rect);
        int xPos = rect.left;
        int yPos = rect.top;

        // Setze die Fensterposition direkt auf die aktuelle Position
        SetWindowPos(
            hwnd,
            NULL,
            xPos,
            yPosBottom,
            rect.right - rect.left,
            rect.bottom - rect.top,
            SWP_NOZORDER | SWP_NOACTIVATE
        );
        return 0;
    
    case WM_MOUSEMOVE:
        if (isDragging){
            POINT mousePos;
            GetCursorPos(&mousePos);

            RECT rect;
            GetWindowRect(hwnd, &rect);
            /*int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;*/

            SetWindowPos(hwnd, NULL, mousePos.x - mouseOffset.x, mousePos.y - mouseOffset.y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// WinMain: Einstiegspunkt
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   
    WNDCLASSEXA wc = { 0 };
    wc.cbSize        = sizeof(wc);                    // Größe der Struktur
    wc.style         = CS_HREDRAW | CS_VREDRAW;        // Neuzeichnen bei Größenänderung
    wc.lpfnWndProc   = WindowProc;                     // Prozedur, die die Fenster-Nachrichten verarbeitet
    wc.hInstance     = hInstance;                      // Handle zur Anwendung
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);    // Standard-Pfeil als Mauszeiger
    wc.lpszClassName = CLASS_NAME;                     // Name der Fensterklasse

    RegisterClassExA(&wc);

    /*int width = 500;
    int height = 200;*/

    RECT rcWorkArea;
    SystemParametersInfoA(SPI_GETWORKAREA, 0, &rcWorkArea, 0);

    yPosBottom = (rcWorkArea.bottom - height) + 5;
    int xPosBottom = (rcWorkArea.right - rcWorkArea.left - width) / 2;

    // 2) Fenster erstellen
    HWND hwnd = CreateWindowExA(
        0,                          // Erweiterter Fensterstil (0 = kein spezieller Stil)
        CLASS_NAME,                 // Fensterklasse
        "Wer das liest ist ein gooner", // Text in der Titelleiste
        WS_OVERLAPPEDWINDOW,        // Standard-Fenster-Stil (mit Titelleiste, Rahmen etc.)
        xPosBottom,                 // X-Position
        yPosBottom,                 // Y-Position
        width,                      // Breite
        height,                     // Höhe
        NULL,                       // Kein übergeordnetes Fenster
        NULL,                       // Kein Menü
        hInstance,                  // Handle zur Anwendung
        NULL                        // Keine zusätzlichen Parameter
    );

    // 3) Fenster anzeigen
    ShowWindow(hwnd, nCmdShow);

    // 4) Nachrichten-Schleife
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);     // Übersetzt Tastatur-Ereignisse (z. B. in Zeichen)
        DispatchMessage(&msg);      // Schickt die Nachricht an WindowProc
    }

    return 0;
}
