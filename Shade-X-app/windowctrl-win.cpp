#include "windowctrl-win.h"

/*
 *  System includes
 */
#include <dwmapi.h>

/*
 *	Local includes
 */
#include "titlebar.h"
#include "debugwidget.h"

/*
 * Qt includes
 */
#include <QString>


/*
 *  Initialize statics
 */
WindowCtrlWin*  WindowCtrlWin::m_ctrl_parent = nullptr;


/*
 *  Constructor
 */
WindowCtrlWin::WindowCtrlWin( QWidget* parent) : QObject( parent )
{
    /*
     *  Store the parent
     */
    m_parent = parent;

    /*
     *  Initialize
     */
    m_mouse_clicks = 0;
    m_click_timer.stop();

    m_caption_height = GetSystemMetrics( SM_CYCAPTION );
    m_border_width = GetSystemMetrics( SM_CXBORDER );
    m_edge_width = GetSystemMetrics( SM_CXEDGE );
    m_frame_width = GetSystemMetrics( SM_CXFRAME );

    m_decoration_width = m_border_width + m_edge_width + m_frame_width;

    initLists();

    /*
     * Setup the minimize intercept
     */
    m_ctrl_parent = this;

    interceptMouseEvent();
    interceptWinEvent();

    /*
     * Automaticly hide the window with a double click
     * Solves also the first double click maximize bug?
     */
    m_splash_timer.singleShot( SPLASH_TIMEOUT, this, &WindowCtrlWin::splashTimeout );
}


/*
 * Destructor
 */
WindowCtrlWin::~WindowCtrlWin()
{
    /*
     * Remove the intercept hook
     */
    stopInterceptMouseEvent();
    stopInterceptWinEvent();
}


/*
 * Set the debug object
 */
void    WindowCtrlWin::setDebug( DebugWidget* debug )
{
    m_debug = debug;
}


/*
 * Splash timeout
 */
void    WindowCtrlWin::splashTimeout()
{
    if( m_parent->isVisible() )
    {
//        emit signalConsole("Splash visible, clicking");

        dblClickParent();

        m_splash_timer.singleShot( SPLASH_TIMEOUT, this, &WindowCtrlWin::splashTimeout );
    }
}


/*
 * Simulate mouse click
 */
void    WindowCtrlWin::dblClickParent()
{
#ifdef  DISPLAY_DBLCLKMYSELF
    emit signalConsole("Double click parent");
#endif

    /*
     * Get the handle
     */
    HWND hwnd = (HWND)m_parent->winId();

    /*
     * Get my titlebar geometrics
     */
    TITLEBARINFOEX* ptinfo = (TITLEBARINFOEX*)malloc( sizeof( TITLEBARINFOEX ) );
    ptinfo->cbSize = sizeof( TITLEBARINFOEX );
    SendMessage( hwnd, WM_GETTITLEBARINFOEX, 0, (LPARAM)ptinfo );

    POINT point;
    point.x = ptinfo->rcTitleBar.left + (ptinfo->rcTitleBar.right - ptinfo->rcTitleBar.left ) / 2;
    point.y = ptinfo->rcTitleBar.top + m_caption_height / 2;

#ifdef  DISPLAY_DBLCLKMYSELF
    emit signalConsole( QString( "Titlebar : %1, %2, %3, %4" )
            .arg( ptinfo->rcTitleBar.left )
            .arg( ptinfo->rcTitleBar.top )
            .arg( ptinfo->rcTitleBar.right )
            .arg( ptinfo->rcTitleBar.bottom ) );

    emit signalConsole( QString( "Click point : %1, %2" )
            .arg( point.x )
            .arg( point.y ) );
#endif

    free( ptinfo );

    SetForegroundWindow( hwnd );
    click( point );
    SetForegroundWindow( hwnd );
    click( point );
}

/*
 * Simulate mouse click
 */
void    WindowCtrlWin::click( POINT pos )
{
    //  https://stackoverflow.com/questions/28386029/how-to-simulate-mouse-click-using-c

    INPUT Inputs[ 2 ] = {0};

    POINT old_pos;
    GetCursorPos(&old_pos);

    SetCursorPos( pos.x, pos.y );

    Inputs[ 0 ].type = INPUT_MOUSE;
    Inputs[ 0 ].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    Inputs[ 1 ].type = INPUT_MOUSE;
    Inputs[ 1 ].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput( 2, Inputs, sizeof(INPUT) );

    SetCursorPos( old_pos.x, old_pos.y );
}


#ifdef  TEST_SENDMESSAGE
void    WindowCtrlWin::dblClickParent2()
{
    emit signalConsole("Double click myself");

    HWND hwnd = (HWND)m_parent->winId();

    /*
     * Get my titlebar geometrics
     */
    TITLEBARINFOEX* ptinfo = (TITLEBARINFOEX*)malloc( sizeof( TITLEBARINFOEX ) );
    ptinfo->cbSize = sizeof( TITLEBARINFOEX );
    SendMessage( hwnd, WM_GETTITLEBARINFOEX, 0, (LPARAM)ptinfo );

    POINT pos;
    pos.x = ptinfo->rcTitleBar.left + (ptinfo->rcTitleBar.right - ptinfo->rcTitleBar.left ) / 2;
    pos.y = ptinfo->rcTitleBar.top + m_caption_height / 2;

    free( ptinfo );

    LPARAM lParam = MAKELPARAM( pos.x, pos.y);
    SendMessage( hwnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam );
    SendMessage( hwnd, WM_LBUTTONUP, 0, lParam );

    SendMessage( hwnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam );
    SendMessage( hwnd, WM_LBUTTONUP, 0, lParam );
}
#endif


/*
 * Intercept the mouse event
 */
void    WindowCtrlWin::interceptMouseEvent()
{
    m_mouse_event_hook = SetWindowsHookExA( WH_MOUSE_LL, mouseEventHook, 0, 0 );
}


/*
 * Remove the intercept hook
 */
void    WindowCtrlWin::stopInterceptMouseEvent()
{
    UnhookWindowsHookEx( m_mouse_event_hook );
}


/*
 * Hook callback
 */
LRESULT CALLBACK    WindowCtrlWin::mouseEventHook( int nCode, WPARAM wParam, LPARAM lParam )
{
    bool block = false;

    MOUSEHOOKSTRUCT* pMouseStruct = (MOUSEHOOKSTRUCT*)lParam;
    if( pMouseStruct != NULL )
    {
        if( wParam == WM_LBUTTONDOWN )
        {
            /*
             * Left mouse clicked
             */
            if( m_ctrl_parent )
            {
                block = m_ctrl_parent->mouseEventHookAction( pMouseStruct->pt );
            }
        }
    }

    if( block )
    {
        return 1;
    }
    else
    {
        return CallNextHookEx( NULL, nCode, wParam, lParam );
    }
}


/*
 * Non-static method to use by the hook callback
 */
bool    WindowCtrlWin::mouseEventHookAction( POINT pt )
{
    /*
     * Count the clicks
     */
    m_mouse_clicks++;

    if( m_mouse_clicks == 1 )
    {
        /*
         * Start double click timer
         */
        m_click_timer.singleShot( GetDoubleClickTime(), this, &WindowCtrlWin::doubleClickTimeout );
    }
    else
    {
        if( m_mouse_clicks == 2 )
        {
#ifdef  DISPLAY_DBLCLICKDETECT
            emit signalConsole( QString( "Double click detected: %1, %2" )
                                .arg( pt.x )
                                .arg( pt.y ) );
#endif

            /*
             * Reset click counter
             */
            m_mouse_clicks = 0;
            m_click_timer.stop();

            /*
             * Get window clicked
             */
            HWND hwnd = WindowFromPoint( pt );

            /*
             * Is it a window?
             */
            if( !IsWindow( hwnd ) )
            {
               return false;
            }

            /*
             * Clicked the splash?
             */
            if( hwnd == (HWND)m_parent->winId() )
            {
                m_parent->hide();
                return true;
            }

#ifdef  DISPLAY_DBLCLICKDETECT
            displayGeometricsWin( hwnd );
#endif

            /*
             * Already shaded?
             */
            if( !m_titlebar_hwnds.contains( hwnd ) )
            {
                /*
                 * Get pointer position within the window
                 */
                POINT rel = pt;
                ScreenToClient( hwnd, &rel );

                /*
                 * Get window rect
                 */
                RECT window_rect;
                GetWindowRect( hwnd, &window_rect );

                RECT lean_rect;
                DwmGetWindowAttribute( hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &lean_rect, sizeof( lean_rect ) );

                int left_decoration = lean_rect.left - window_rect.left;

                /*
                 * Get titlebar geometrics
                 */
                TITLEBARINFOEX* ptinfo = (TITLEBARINFOEX*)malloc( sizeof( TITLEBARINFOEX ) );
                ptinfo->cbSize = sizeof( TITLEBARINFOEX );
                SendMessage( hwnd, WM_GETTITLEBARINFOEX, 0, (LPARAM)ptinfo );

                bool valid_visible = ptinfo->rgstate[ 0 ] == STATE_SYSTEM_FOCUSABLE;
                bool valid_invisible = ptinfo->rgstate[ 0 ] == ( STATE_SYSTEM_FOCUSABLE | STATE_SYSTEM_INVISIBLE );
                bool valid_size = ( ptinfo->rcTitleBar.right - ptinfo->rcTitleBar.left > 0 ) && ( ptinfo->rcTitleBar.bottom - ptinfo->rcTitleBar.top > 0 );

#ifdef  DISPLAY_DBLCLICKDETECT
                emit signalConsole( QString( "State: %1" ).arg( ptinfo->rgstate[ 0 ], 0, 16 ) );
                emit signalConsole( QString( "Titlebar valid visible: %1" ).arg( valid_visible ) );
                emit signalConsole( QString( "Titlebar valid invisible: %1" ).arg( valid_invisible ) );

                displayBorders();
#endif

                bool parent_active = false;
                bool valid_visible_parent = false;
                bool valid_invisible_parent = false;
                bool valid_size_parent = false;

                TITLEBARINFOEX* ptinfo_parent = nullptr;
                HWND parent_hwnd;

                if( valid_invisible )
                {
                    /*
                     * Get parent handle
                     */
                    parent_hwnd = GetParent( hwnd );

                    /*
                     * Get window rect
                     */
                    RECT window_rect_parent;
                    GetWindowRect( parent_hwnd, &window_rect_parent );

                    RECT lean_rect_parent;
                    DwmGetWindowAttribute( parent_hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &lean_rect_parent, sizeof( lean_rect ) );

                    left_decoration = lean_rect_parent.left - window_rect_parent.left;

                    ptinfo_parent = (TITLEBARINFOEX*)malloc( sizeof( TITLEBARINFOEX ) );
                    ptinfo_parent->cbSize = sizeof( TITLEBARINFOEX );
                    SendMessage( parent_hwnd, WM_GETTITLEBARINFOEX, 0, (LPARAM)ptinfo_parent );

#ifdef  DISPLAY_DBLCLICKDETECT
                    emit signalConsole( QString( "Parent" ) );

                    displayGeometricsWin( parent_hwnd );
#endif

                    valid_visible_parent = ptinfo_parent->rgstate[ 0 ] == STATE_SYSTEM_FOCUSABLE;
                    valid_invisible_parent = ptinfo_parent->rgstate[ 0 ] == ( STATE_SYSTEM_FOCUSABLE | STATE_SYSTEM_INVISIBLE );
                    valid_size_parent = ( ptinfo_parent->rcTitleBar.right - ptinfo_parent->rcTitleBar.left > 0 ) && ( ptinfo_parent->rcTitleBar.bottom - ptinfo_parent->rcTitleBar.top > 0 );

                    /*
                     * Still invisible, exit
                     */
                    if( !valid_visible_parent && valid_invisible_parent )
                    {
                        free( ptinfo );
                        free( ptinfo_parent );

                        return false;
                    }

                    parent_active = true;
                }

                if( ( valid_visible &&
                      pt.y > ptinfo->rcTitleBar.top &&
                      pt.y < ptinfo->rcTitleBar.top + m_caption_height ) ||
                    ( valid_invisible &&
                      pt.y > window_rect.top &&
                      pt.y < window_rect.top + m_caption_height ) )
                {
#ifdef  DISPLAY_DBLCLICKDETECT
                    emit signalConsole("Titlebar clicked");
#endif

                    /*
                     * Only shade when the titlebar is double clicked
                     */

                    /*
                     * Correction for restoring a window withoud titlebar (Firefox)
                     */
                    QPoint correction( 0, 0 );
                    if( valid_visible && !valid_size )
                    {
                        if( left_decoration < m_decoration_width )
                        {
                            correction.setX( m_decoration_width - left_decoration );
                        }
                    }
                    if( valid_invisible )
                    {
//                        correction.setX( m_decoration_width );
                        correction.setX( m_decoration_width - left_decoration );
                    }

                    /*
                     * Get the geometry
                     */
                    QRect geometry;
                    if( valid_visible )
                    {
                        geometry = getGeometricsWin( hwnd, valid_size );
                    }
                    else
                    {
                        geometry = getGeometricsWin2( hwnd );
                    }

                    if( parent_active )
                    {
                        hwnd = parent_hwnd;
                    }

                    /*
                     * Get caption text
                     */
                    char caption[ 1024 ];
                    GetWindowTextA( hwnd, caption, sizeof( caption ) );

                    /*
                     * Create the replacement shaded titlebar
                     */
                    Titlebar* titlebar = new Titlebar( this, geometry, caption );

                    /*
                     * Get the window state
                     */
                    WINDOWPLACEMENT wp;
                    wp.length = sizeof( WINDOWPLACEMENT );
                    GetWindowPlacement( hwnd, &wp );

                    /*
                     * Store the data
                     */
                    m_shade_hwnds.append( hwnd );
                    m_shade_states.append( wp.showCmd );
                    m_shade_correction.append( correction );
                    m_titlebar_hwnds.append( (HWND)titlebar->getWinId() );
                    m_titlebars.append( titlebar );

                    /*
                     * Force Minimize the window without effects
                     */
                    bool transition = false;
                    DwmSetWindowAttribute( hwnd,
                        DWMWA_TRANSITIONS_FORCEDISABLED,
                        &transition,
                        sizeof(transition));

                    ShowWindow( hwnd, SW_FORCEMINIMIZE);

                    /*
                     * Cleanup
                     */
                    free( ptinfo );
                    if( parent_active ) free( ptinfo_parent );

                    return true;
                }

                /*
                 * Cleanup
                 */
                free( ptinfo );
                if( parent_active ) free( ptinfo_parent );
            }
            else
            {
#ifdef  DISPLAY_DBLCLICKDETECT
                emit signalConsole("Restore by double clicking shade");
#endif

                /*
                 * Get the index
                 */
                int index = m_titlebar_hwnds.indexOf( hwnd );

                /*
                 * Get the window position
                 */
                QPoint pos = getRestorePos( hwnd );
                pos.setX( pos.x() +  m_shade_correction[ index ].x() );
                pos.setY( pos.y() +  m_shade_correction[ index ].y() );

                /*
                 *  Unshade window
                 */
                ShowWindow( m_shade_hwnds[ index ], m_shade_states[ index ] );
                SetWindowPos( m_shade_hwnds[ index ], NULL,
                              pos.x(),
                              pos.y(),
                              0, 0, SWP_NOZORDER | SWP_NOSIZE );

                /*
                 * Cleanup the lists
                 */
                clean( index );
            }
        }
    }

    return false;
}


/*
 * Display all windows decoration sizes
 */
void   WindowCtrlWin::displayBorders()
{
    emit signalConsole( QString( "SM_CXBORDER: %1" ).arg( GetSystemMetrics( SM_CXBORDER ) ) );
    emit signalConsole( QString( "SM_CYBORDER: %1" ).arg( GetSystemMetrics( SM_CYBORDER ) ) );
    emit signalConsole( QString( "SM_CXEDGE: %1" ).arg( GetSystemMetrics( SM_CXEDGE ) ) );
    emit signalConsole( QString( "SM_CYEDGE: %1" ).arg( GetSystemMetrics( SM_CYEDGE ) ) );
    emit signalConsole( QString( "SM_CXFRAME: %1" ).arg( GetSystemMetrics( SM_CXFRAME ) ) );
    emit signalConsole( QString( "SM_CYFRAME: %1" ).arg( GetSystemMetrics( SM_CYFRAME ) ) );
    emit signalConsole( QString( "SM_CXFIXEDFRAME: %1" ).arg( GetSystemMetrics( SM_CXFIXEDFRAME ) ) );
    emit signalConsole( QString( "SM_CYFIXEDFRAME: %1" ).arg( GetSystemMetrics( SM_CYFIXEDFRAME ) ) );
    emit signalConsole( QString( "SM_CXSIZEFRAME: %1" ).arg( GetSystemMetrics( SM_CXSIZEFRAME ) ) );
    emit signalConsole( QString( "SM_CYSIZEFRAME: %1" ).arg( GetSystemMetrics( SM_CYSIZEFRAME ) ) );

    emit signalConsole( QString( "SM_CXPADDEDBORDER: %1" ).arg( GetSystemMetrics( SM_CXPADDEDBORDER ) ) );
}


/*
 * Get the geometry for the window
 */
void   WindowCtrlWin::displayGeometricsWin( HWND hwnd )
{
    RECT window_rect;
    GetWindowRect( hwnd, &window_rect );

    RECT client_rect;
    GetClientRect( hwnd, &client_rect );

    RECT lean_rect;
    DwmGetWindowAttribute( hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &lean_rect, sizeof( lean_rect ) );

    int left_decoration = lean_rect.left - window_rect.left;
    int top_decoration = lean_rect.top - window_rect.top;
    int right_decoration = window_rect.right - lean_rect.right;
    int bottom_decoration = window_rect.bottom - lean_rect.bottom;

    emit signalConsole( QString( "Window rect: %1, %2, %3, %4" )
                        .arg( window_rect.left )
                        .arg( window_rect.top )
                        .arg( window_rect.right )
                        .arg( window_rect.bottom ) );

    emit signalConsole( QString( "Client rect: %1, %2, %3, %4" )
                        .arg( client_rect.left )
                        .arg( client_rect.top )
                        .arg( client_rect.right )
                        .arg( client_rect.bottom ) );

    emit signalConsole( QString( "Lean rect: %1, %2, %3, %4" )
                        .arg( lean_rect.left )
                        .arg( lean_rect.top )
                        .arg( lean_rect.right )
                        .arg( lean_rect.bottom ) );

    emit signalConsole( QString( "Dec: %1, %2, %3, %4" )
                        .arg( left_decoration )
                        .arg( top_decoration )
                        .arg( right_decoration )
                        .arg( bottom_decoration ) );

    TITLEBARINFOEX* ptinfo = (TITLEBARINFOEX*)malloc( sizeof( TITLEBARINFOEX ) );
    ptinfo->cbSize = sizeof( TITLEBARINFOEX );
    SendMessage( hwnd, WM_GETTITLEBARINFOEX, 0, (LPARAM)ptinfo);

    emit signalConsole( QString( "TInfo: %1, %2, %3, %4" )
                        .arg( ptinfo->rcTitleBar.left )
                        .arg( ptinfo->rcTitleBar.top )
                        .arg( ptinfo->rcTitleBar.right )
                        .arg( ptinfo->rcTitleBar.bottom ) );

    emit signalConsole( QString( "TInfo rgstate: %1" )
                        .arg( ptinfo->rgstate[ 0 ], 0, 16 ) );

    free( ptinfo );
}


/*
 * Get the geometry for the window
 */
QRect   WindowCtrlWin::getGeometricsWin( HWND hwnd, bool valid_size )
{
    RECT window_rect;
    GetWindowRect( hwnd, &window_rect );

    RECT lean_rect;
    DwmGetWindowAttribute( hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &lean_rect, sizeof( lean_rect ) );

    int left_decoration = lean_rect.left - window_rect.left;

    int top_correction = m_border_width;
    if( !valid_size && left_decoration < m_decoration_width )
    {
        top_correction = m_border_width + m_edge_width;
    }

    QRect geometry( lean_rect.left + m_border_width,
                    lean_rect.top + m_caption_height + left_decoration + top_correction,
                    lean_rect.right - lean_rect.left - 2 * m_border_width,
                    1 );

    return geometry;
}


/*
 * Get the geometry for the window
 */
QRect   WindowCtrlWin::getGeometricsWin2( HWND hwnd )
{
    RECT window_rect;
    GetWindowRect( hwnd, &window_rect );

    QRect geometry( window_rect.left + m_border_width,
                    window_rect.top + m_caption_height + 2 * m_frame_width,
                    window_rect.right - window_rect.left - 2 * m_border_width,
                    1 );

    return geometry;
}


/*
 * Get the restore position
 */
QPoint  WindowCtrlWin::getRestorePos( HWND hwnd )
{
    RECT win_rect;
    GetWindowRect( hwnd, &win_rect );

    QPoint pos( win_rect.left, win_rect.top );

    return pos;
}


/*
 * Handle double click timeout
 */
void    WindowCtrlWin::doubleClickTimeout()
{
    m_mouse_clicks = 0;
}


/*
 * Intercept the win event
 */
void    WindowCtrlWin::interceptWinEvent()
{
    m_win_event_hook = SetWinEventHook( EVENT_OBJECT_LOCATIONCHANGE, EVENT_OBJECT_LOCATIONCHANGE,
                                            NULL,
                                            winEventHook,
                                            0, 0,
                                            WINEVENT_OUTOFCONTEXT );

    m_win_destroy_event_hook = SetWinEventHook( EVENT_OBJECT_DESTROY, EVENT_OBJECT_DESTROY,
                                                    NULL,
                                                    winDestroyEventHook,
                                                    0, 0,
                                                    WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
}


/*
 * Remove the intercept hook
 */
void    WindowCtrlWin::stopInterceptWinEvent()
{
    UnhookWinEvent( m_win_event_hook );
    UnhookWinEvent( m_win_destroy_event_hook );
}


/*
 * Hook callback
 */
void CALLBACK   WindowCtrlWin::winEventHook( HWINEVENTHOOK hook, DWORD event, HWND hWnd,
                                             LONG idObject, LONG idChild,
                                             DWORD dwEventThread, DWORD dwmsEventTime )
{
    Q_UNUSED( hook );
    Q_UNUSED( idObject );
    Q_UNUSED( idChild );
    Q_UNUSED( dwEventThread );
    Q_UNUSED( dwmsEventTime );

    if( EVENT_OBJECT_LOCATIONCHANGE == event )
    {
        WINDOWPLACEMENT wp;
        wp.length = sizeof( WINDOWPLACEMENT );
        GetWindowPlacement( hWnd, &wp );

        if( SW_NORMAL == wp.showCmd || SW_MAXIMIZE == wp.showCmd )
        {
            /*
             * Window is shown
             */
            if( m_ctrl_parent )
            {
                m_ctrl_parent->winEventHookActionNormalMaximize( hWnd );
            }
        }
    }
}


/*
 * Non-static method to use by the hook callback
 */
void    WindowCtrlWin::winEventHookActionNormalMaximize( HWND hwnd )
{
    if( m_shade_hwnds.contains( hwnd ) )
    {
        /*
         * Get the index
         */
        int index = m_shade_hwnds.indexOf( hwnd );

        /*
         * Get the window position
         */
        QPoint pos = getRestorePos( m_titlebar_hwnds[ index ] );

        /*
         *  Unshade window
         */
        SetWindowPos( hwnd, NULL,
                      pos.x(),
                      pos.y(),
                      0, 0, SWP_NOZORDER | SWP_NOSIZE );

        /*
         * Cleanup the lists
         */
        clean( index );
    }
}


/*
 * Hook callback
 */
void CALLBACK   WindowCtrlWin::winDestroyEventHook( HWINEVENTHOOK hook, DWORD event, HWND hWnd,
                                                    LONG idObject, LONG idChild,
                                                    DWORD dwEventThread, DWORD dwmsEventTime )
{
    Q_UNUSED( hook );
    Q_UNUSED( event );
    Q_UNUSED( idObject );
    Q_UNUSED( idChild );
    Q_UNUSED( dwEventThread );
    Q_UNUSED( dwmsEventTime );

    if( m_ctrl_parent )
    {
        m_ctrl_parent->winDestroyEventHookAction( hWnd );
    }
}


/*
 * Non-static method to use by the hook callback
 */
void    WindowCtrlWin::winDestroyEventHookAction( HWND hwnd )
{
    if( m_shade_hwnds.contains( hwnd ) )
    {
//        emit signalConsole( QString( "Shaded app closed" ) );

        /*
         * Get the index
         */
        int index = m_shade_hwnds.indexOf( hwnd );

        /*
         * Cleanup the lists
         */
        clean( index );
    }
}


/*
 * Unshade and maximize
 */
void    WindowCtrlWin::slotMaximizedTitlebar( WId hwnd )
{
//    emit signalConsole( QString( "Maximized" ) );

    /*
     * Get the index
     */
    int index = m_titlebar_hwnds.indexOf( (HWND)hwnd );

    /*
     *  Maximize window
     */
    ShowWindow( m_shade_hwnds[ index ], SW_MAXIMIZE );

    /*
     * Cleanup the lists
     */
    clean( index );
}


/*
 * Unshade and minimize (already minimized)
 */
void    WindowCtrlWin::slotMinimizedTitlebar( WId hwnd )
{
//    emit signalConsole( QString( "Minimized" ) );

    /*
     * Get the index
     */
    int index = m_titlebar_hwnds.indexOf( (HWND)hwnd );

    /*
     * Cleanup the lists
     */
    clean( index );
}


/*
 * Handle the close signal from a titlebar
 */
void    WindowCtrlWin::slotCloseTitlebarAndShade( WId hwnd )
{
    /*
     * Stop click timer
     */
    m_click_timer.stop();
    m_mouse_clicks = 0;

    /*
     * Get the index
     */
    int index = m_titlebar_hwnds.indexOf( (HWND)hwnd );

    /*
     * Get the window position
     */
    QPoint pos = getRestorePos( (HWND)hwnd );

    /*
     *  Unshade window
     */
    ShowWindow( m_shade_hwnds[ index ], m_shade_states[ index ] );
    SetWindowPos( m_shade_hwnds[ index ], NULL,
                  pos.x(),
                  pos.y(),
                  0, 0, SWP_NOZORDER | SWP_NOSIZE );

    /*
     * Close the shaded app
     */
    SendMessage( m_shade_hwnds[ index ], WM_CLOSE, 0, 0 );

    /*
     * Cleanup the lists
     */
    clean( index );
}


void    WindowCtrlWin::unshadeAll()
{
    while( !m_shade_hwnds.isEmpty() )
    {
        HWND shade_hwnd =  m_shade_hwnds[ 0 ];
        int shade_state =  m_shade_states[ 0 ];
        HWND titlebar_hwnd =  m_titlebar_hwnds[ 0 ];

        /*
         * Get the window position
         */
        QPoint pos = getRestorePos( titlebar_hwnd );

        /*
         * Cleanup the lists
         */
        clean( 0 );

        /*
         *  Unshade window
         */
        ShowWindow( shade_hwnd, shade_state );
        SetWindowPos( shade_hwnd, NULL,
                      pos.x(),
                      pos.y(),
                      0, 0, SWP_NOZORDER | SWP_NOSIZE );
    }

    /*
     * Make sure the lists are empty
     */
   initLists();
}


/*
 * Initialize the lists
 */
void    WindowCtrlWin::initLists()
{
    m_shade_hwnds = QList<HWND>();
    m_shade_states = QList<int>();
    m_shade_correction = QList<QPoint>();
    m_titlebar_hwnds = QList<HWND>();
    m_titlebars = QList<Titlebar*>();
}


/*
 * Remove index from the lists
 */
void    WindowCtrlWin::clean( int index )
{
    /*
     * Cleanup the lists
     */
    m_shade_hwnds.removeAt( index );
    m_shade_states.removeAt( index );
    m_shade_correction.removeAt( index );
    m_titlebar_hwnds.removeAt( index );

    m_titlebars[ index ]->close();
    delete m_titlebars[ index ];
    m_titlebars.removeAt( index );
}
