#include "Window.h"

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Window* pThis = nullptr;

    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lparam;
        pThis = (Window*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

        pThis->m_hwnd = hwnd;
    }
    else
    {
        pThis = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis)
    {
        switch (msg)
        {
        case WM_SIZE:
            pThis->UpdateSizeOnReSize(LOWORD(lparam), HIWORD(lparam));

            InvalidateRect(hwnd, nullptr, TRUE);

            break;
        case WM_MOVE:
            pThis->UpdatePosOnMove(LOWORD(lparam), HIWORD(lparam));
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        }
        return pThis->HandleMessages(msg, wparam, lparam);
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

Window::~Window()
{
    CloseWindow();
}

void Window::SetWidth(int newWidth)
{
    if (newWidth < 0)
    {
        throw std::invalid_argument("Width cannot be negative");
    }
    else if (!m_hwnd) {
        throw std::runtime_error("Window not created");
    }
    else {
        m_width = newWidth;
        SetWindowPos(m_hwnd, nullptr, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);
    }
}

void Window::SetDimensions(int newHeight, int newWidth)
{
    if (newHeight < 0 || newWidth < 0)
    {
        throw std::invalid_argument("Height and Width cannot be negative");
    }
    else if (!m_hwnd) {
        throw std::runtime_error("Window not created");
    }
    else
    {
        m_height = newHeight;
        m_width = newWidth;
        SetWindowPos(m_hwnd, nullptr, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);
    }
}

void Window::Maximize() const
{
    if (m_isMaximized)
    {
        return;
    }
    else if (!m_hwnd) {
        throw std::runtime_error("Window not created");
    }
    else {
        ShowWindow(m_hwnd, SW_MAXIMIZE);
    }
}

void Window::Minimize() const
{
    if (m_isMinimized)
    {
        return;
    }
    else if (!m_hwnd) {
        throw std::runtime_error("Window not created");
    }
    else {
        ShowWindow(m_hwnd, SW_MINIMIZE);
    }
}

void Window::Restore() const
{
    if (!m_isMinimized && !m_isMaximized) {
        return;
    }
    else if (!m_hwnd) {
        throw std::runtime_error("Window not created");
    }
    else {
        ShowWindow(m_hwnd, SW_RESTORE);
    }
}

void Window::Show()
{
    if (m_isVisible) {
        return;
    }
    else if (!m_hwnd) {
        throw std::runtime_error("Window not created");
    }
    else {
        ShowWindow(m_hwnd, SW_SHOW);
        m_isVisible = true;
    }
}

void Window::Hide()
{
    if (!m_isVisible) {
        return;
    }
    else if (!m_hwnd) {
        throw std::runtime_error("Window not created");
    }
    else {
        ShowWindow(m_hwnd, SW_HIDE);
        m_isVisible = false;
    }
}

void Window::SetPos(int x, int y)
{
    m_x = x;
    m_y = y;
    if (m_hwnd) {
        SetWindowPos(m_hwnd, nullptr, m_x, m_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
}

void Window::SetMenu(HMENU menu)
{
    if (!m_hwnd) {
        throw std::runtime_error("Window not created");
    }
    ::SetMenu(m_hwnd, menu);
    m_menu = menu;
}

void Window::Enable()
{
    if (!m_hwnd) {
        throw std::runtime_error("Window not created");
    }
    else if (m_isEnabled) {
        return;
    }
    else {
        EnableWindow(m_hwnd, TRUE);
        m_isEnabled = true;
    }

}

void Window::Disable()
{
    if (!m_hwnd) {
        throw std::runtime_error("Window not created");
    }
    else if (!m_isEnabled) {
        return;
    }
    else {
        EnableWindow(m_hwnd, FALSE);
        m_isEnabled = false;
    }
}

void Window::UpdatePosOnMove(int x, int y) noexcept
{
    m_x = x;
    m_y = y;
}

void Window::UpdateSizeOnReSize(int height, int width)
{
    m_height = height;
    m_width = width;
}

void Window::CloseWindow()
{
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
    else {
        throw std::runtime_error("Window not created");
    }
}

HRESULT Window::Create(std::wstring heading, HWND parent = nullptr)
{
    if (!m_Initalized)
    {
        wc = { 0 };
        wc.lpfnWndProc = WndProc;
        wc.hInstance = hInstance();
        wc.lpszClassName = m_className;
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        RegisterClass(&wc);
        m_Initalized = true;
    }

    if (m_hwnd != nullptr)
    {
        throw std::runtime_error("Window already created");
        return E_FAIL;
    }
    m_hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        wc.lpszClassName,               // Window class
        heading.c_str(),                // Window text
        m_style,                        // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        parent,                         // Parent window    
        nullptr,                        // Menu
        GetModuleHandle(nullptr),       // Instance handle
        this                            // Additional application data
    );

    if (m_hwnd == nullptr)
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }
    return S_OK;
}