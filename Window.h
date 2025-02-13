#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pch.h"

class Window
{
public:
    Window() = default;
    ~Window();
    int GetWidth() const noexcept { return m_width; }
    int GetHeight() const noexcept { return m_height; }
    void SetWidth(int newWidth);
    void SetDimensions(int newHeight, int newWidth);
    void Maximize() const;
    void Minimize() const;
    void Restore() const;
    void Show();
    void Hide();


    int GetXPos() const noexcept { return m_x; }
    int GetYPos() const noexcept { return m_y; }
    void SetPos(int x, int y);
    void SetMenu(HMENU menu);
    void Enable();
    void Disable();


    void CloseWindow();
    HRESULT Create(std::wstring heading, HWND parent);

protected:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    virtual HRESULT HandleMessages(UINT msg, WPARAM wparam, LPARAM lParam) = 0;
    WNDCLASS wc;

    // dimensions
    int m_height{ CW_USEDEFAULT }, m_width{ CW_USEDEFAULT };


    // coordinates
    int m_x{ CW_USEDEFAULT }, m_y{ CW_USEDEFAULT };

    HWND m_hwnd{ nullptr };
    HMENU m_menu{ nullptr };
    DWORD m_style{ WS_OVERLAPPEDWINDOW };
    bool m_isEnabled{ true };
    LPCWSTR m_className{ nullptr };

    void UpdatePosOnMove(int x, int y) noexcept;
    void UpdateSizeOnReSize(int height, int width);
    bool m_isVisible{ true };
    bool m_isMinimized{ false };
    bool m_isMaximized{ false };
    bool m_isDefault{ false };
    bool m_Initalized{ false };
};

#endif // MAINWINDOW_H