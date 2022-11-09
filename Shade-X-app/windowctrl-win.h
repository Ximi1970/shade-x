#ifndef WINDOWCTRLWIN_H
#define WINDOWCTRLWIN_H

/*
 *	Local includes
 */
#include "debug.h"
#include "preferences.h"

/*
 *  System includes
 */
#include <Windows.h>

/*
 *  Standard library includes
 */

/*
 *  Qt includes
 */
#include <QObject>
#include <QWindow>
#include <QTimer>

/*
 *  Forward declarations
 */
class QWidget;
class WindowCtrlWin;
class Titlebar;
class TitlebarWidget;
class DebugWidget;

/*
 * Constants
 */
#define    SPLASH_TIMEOUT   1000

/**
 * @brief The WindowCtrlWin class
 */
class WindowCtrlWin : public QObject
{
    Q_OBJECT

    private:

        /**
         * @brief The EnumWindowsProcData struct
         */
        struct EnumWindowsProcData
        {
            WindowCtrlWin&  window_ctrl;
        };

    public:

        /**
         * @brief WindowCtrlWin. Constructor.
         *
         * @param parent    My parent.
         */
        explicit WindowCtrlWin( QWidget* parent = nullptr );

        /**
         * @brief ~WindowCtrlWin. Destructor.
         */
        ~WindowCtrlWin();

        /**
         * @brief setDebug
         *
         *  @param debug
         */
        void    setDebug( DebugWidget* debug );

        /**
         * @brief unshadeAll
         */
        void    unshadeAll();

#ifdef  TEST_SENDMESSAGE
        void    dblClickParent2();
#endif

    private:

        /**
         * @brief splashTimeout
         */
        void    splashTimeout();

        /**
         * @brief dblClickParent
         */
        void    dblClickParent();

        /**
         * @brief click. Simulate a mouse click on pos.
         *
         *  @param pos      Position to be clicked.
         */
        void    click( POINT pos );

        /**
         * @brief interceptMouseEvent. Intercept a mouse event.
         */
        void    interceptMouseEvent();

        /**
         * @brief stopInterceptMouseEvent. Remove the intercept hook.
         */
        void    stopInterceptMouseEvent();

        /**
         * @brief mouseEventHook
         *
         *  @param nCode
         *  @param wParam
         *  @param lParam
         *
         *  @return
         */
        static LRESULT CALLBACK    mouseEventHook( int nCode, WPARAM wParam, LPARAM lParam );

        /**
         * @brief mouseEventHookAction
         *
         *  @param pt.  Current mouse position.
         */
        bool    mouseEventHookAction( POINT pt );

        void    displayBorders();

        /**
         * @brief displayGeometricsWin
         *
         *  @param hwnd
         */
        void    displayGeometricsWin( HWND hwnd );

        /**
         * @brief getGeometricsWin. Get the window geometrics.
         *
         *  @param hwnd.    The handle of the window.
         *
         *  @return The geometrics of the window.
         */
        QRect   getGeometricsWin( HWND hwnd, bool valid_size );

        /**
         * @brief getGeometricsWin2. Get the window geometrics for a window with invisible titlebar.
         *
         *  @param hwnd.    The handle of the window.
         *
         *  @return The geometrics of the window.
         */
        QRect   getGeometricsWin2( HWND hwnd );

        /**
         * @brief getRestorePos. Get the restore position.
         *
         *  @param hwnd.    The handle of the window.
         *
         *  @return The top left position of the window.
         */
        QPoint  getRestorePos( HWND hwnd );

        /**
         * @brief doubleClickTimeout
         */
        void    doubleClickTimeout();

        /**
         * @brief interceptWinEvent. Intercept a win event.
         */
        void    interceptWinEvent();

        /**
         * @brief stopInterceptWinEvent. Remove the intercept hook.
         */
        void    stopInterceptWinEvent();

        /**
         * @brief winEventHook. Callback for the hook.
         *
         *  @param hook
         *  @param event
         *  @param hwnd
         *  @param idObject
         *  @param idChild
         *  @param dwEventThread
         *  @param dwmsEventTime
         */
        static void CALLBACK    winEventHook( HWINEVENTHOOK hook, DWORD event, HWND hwnd,
                                                LONG idObject, LONG idChild,
                                                DWORD dwEventThread, DWORD dwmsEventTime);

        /**
         * @brief winEventHookAction. Non-static function to be used by the hook callback.
         */
        void    winEventHookActionNormalMaximize( HWND hWnd );

        /**
         * @brief winDestroyEventHook
         *
         *  @param hook
         *  @param event
         *  @param hWnd
         *  @param idObject
         *  @param idChild
         *  @param dwEventThread
         *  @param dwmsEventTime
         */
        static void CALLBACK   winDestroyEventHook( HWINEVENTHOOK hook, DWORD event, HWND hWnd,
                                                LONG idObject, LONG idChild,
                                                DWORD dwEventThread, DWORD dwmsEventTime );

        /**
         * @brief winDestroyEventHookAction
         *
         *  @param hWnd
         */
        void    winDestroyEventHookAction( HWND hWnd );

        /**
         * @brief initLists. Initialize the lists.
         */
        void    initLists();

        /**
         * @brief clean. Remove index from the lists.
         *
         *  @param index    Index to be removed from the lists.
         */
        void    clean( int index );

        /**
         * @brief getDesktopWindows. Get all desktop windows.
         */
        void    getDesktopWindows();

        /**
         * @brief enumWindowsProc
         *
         *  @param hwnd     Windows handle found.
         *  @param lParam   Pointer to the data.
         *
         *  @return
         */
        static BOOL CALLBACK   enumWindowsProc( HWND hwnd, LPARAM lParam );

        /**
         * @brief isMainWindow. Check for a main window.
         *
         *  @param hwnd     Window handle to be checked.
         *
         *  @return     Window state.
         */
        static BOOL    isMainWindow( HWND hwnd );

        /**
         * @brief IsInvisibleWin10BackgroundAppWindow. Check for special win10 windows.
         *
         *  @param hwnd     Window handle to be checked.
         *
         *  @return     Window state.
         */
        static BOOL    IsInvisibleWin10BackgroundAppWindow( HWND hwnd );

    signals:

        /**
         * @brief signalConsole. Send a console message.
         *
         *  @param message      The message.
         */
        void    signalConsole( QString message );

    public slots:

        /**
         * @brief slotMaximizedTitlebar
         *
         *  @param hwnd
         */
        void    slotMaximizedTitlebar( WId hwnd );

        /**
         * @brief slotMinimizedTitlebar
         *
         *  @param hwnd
         */
        void    slotMinimizedTitlebar( WId hwnd );

        /**
         * @brief slotCloseTitlebarAndShade
         *
         *  @param hwnd
         */
        void    slotCloseTitlebarAndShade( WId hwnd );

    protected:

        /**
         * @brief m_intercept
         */
        static WindowCtrlWin*  m_ctrl_parent;

    private:

        /**
         * @brief m_parent. Pointer to the parent.
         */
        QWidget*    m_parent;

        /**
         * @brief m_debug. Pointer to the debug widget.
         */
        DebugWidget*    m_debug;

        /**
         * @brief m_shade_hwnds
         */
        QList<HWND> m_shade_hwnds;

        /**
         * @brief m_shade_states
         */
        QList<int>  m_shade_states;

        /**
         * @brief m_shade_correction
         */
        QList<QPoint>  m_shade_correction;

        /**
         * @brief m_titlebar_hwnds
         */
        QList<HWND> m_titlebar_hwnds;

#ifdef  TITLEBAR_WIDGET
        /**
         * @brief m_titlebars
         */
        QList<TitlebarWidget*>  m_titlebars;
#else
        /**
         * @brief m_titlebars
         */
        QList<Titlebar*>  m_titlebars;
#endif



        /**
         * @brief m_caption_height
         */
        int m_caption_height;

        /**
         * @brief m_decoration_width
         */
        int m_decoration_width;

        /**
         * @brief m_border_width
         */
        int m_border_width;

        /**
         * @brief m_edge_width
         */
        int m_edge_width;

        /**
         * @brief m_frame_width
         */
        int m_frame_width;

        /**
         * @brief m_mouse_event_hook
         */
        HHOOK   m_mouse_event_hook;

        /**
         * @brief m_win_event_hook
         */
        HWINEVENTHOOK m_win_event_hook;

        /**
         * @brief m_win_destroy_event_hook
         */
        HWINEVENTHOOK m_win_destroy_event_hook;

        /**
         * @brief m_click_timer
         */
        QTimer  m_click_timer;

        /**
         * @brief m_mouse_clicks
         */
        int m_mouse_clicks;

        /**
         * @brief m_splash_timer
         */
        QTimer  m_splash_timer;

        /**
         * @brief m_desktop_windows
         */
        QList< HWND >   m_desktop_windows;
};

#endif // WINDOWCTRLWIN_H
