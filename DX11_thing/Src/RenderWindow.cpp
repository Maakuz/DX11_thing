#include "WindowContainer.h"

bool RenderWindow::initialize(WindowContainer* pWinContainer, HINSTANCE hInstance, std::string title, std::string windowClass, int width, int height)
{
    m_hInstance = hInstance;
    m_width = width;
    m_height = height;
    m_winTitle = title;
    m_winTitleW = StringConverter::stringToWide(title);

    m_windowClass = windowClass;
    m_windowClassW = StringConverter::stringToWide(windowClass);

    this->registerWindowClass();

    m_handle = CreateWindowEx(0,
        m_windowClassW.c_str(),
        m_winTitleW.c_str(),
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        0,
        0,
        m_width,
        m_height,
        NULL,
        NULL,
        m_hInstance,
        pWinContainer);

    if (m_handle == NULL)
    {
        ErrorLogger::log(GetLastError(), "CreateWindowEx failed for " + m_winTitle);
        return false;
    }

    ShowWindow(m_handle, SW_SHOW);
    SetForegroundWindow(m_handle);
    SetFocus(m_handle);

    return true;
}

bool RenderWindow::processMessage()
{
    MSG l_msg;
    ZeroMemory(&l_msg, sizeof(MSG));

    while (PeekMessage(&l_msg,
        m_handle,
        0,
        0,
        PM_REMOVE))
    {
        TranslateMessage(&l_msg);
        DispatchMessage(&l_msg);
    }

    if (l_msg.message == WM_NULL)
    {
        if (!IsWindow(m_handle))
        {
            m_handle = NULL;
            UnregisterClass(m_windowClassW.c_str(), m_hInstance);
            return false;
        }
    }

    return true;
}

RenderWindow::~RenderWindow()
{
    if (m_handle != NULL)
    {
        UnregisterClass(m_windowClassW.c_str(), m_hInstance);
        DestroyWindow(m_handle);
    }
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    default:
        WindowContainer* const l_pWindowContainer = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        return l_pWindowContainer->WindowProc(hwnd, uMsg, wParam, lParam);
    }
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_NCCREATE:
    {
        CREATESTRUCTW* const l_pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        WindowContainer* l_pWindow = reinterpret_cast<WindowContainer*>(l_pCreate->lpCreateParams);
        if (l_pWindow == nullptr)
        {
            ErrorLogger::log("Error: Pointer to window container is null during WM_NCCREATE");
            exit(-1);
        }

        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(l_pWindow));
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
        return l_pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void RenderWindow::registerWindowClass()
{
    WNDCLASSEX l_wc = {0};
    l_wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    l_wc.lpfnWndProc = HandleMessageSetup;
    l_wc.cbClsExtra = 0;
    l_wc.cbWndExtra = 0;
    l_wc.hInstance = m_hInstance;
    l_wc.hIcon = NULL;
    l_wc.hIconSm = NULL;
    l_wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    l_wc.hbrBackground = NULL;
    l_wc.lpszMenuName = NULL;
    l_wc.lpszClassName = m_windowClassW.c_str();
    l_wc.cbSize = sizeof(WNDCLASSEX);
    RegisterClassEx(&l_wc);
}
