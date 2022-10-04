#include "titlebar.h"

/*
 *  System includes
 */

/*
 *	Local includes
 */
#include "windowctrl-win.h"
#include "titlebarwidget.h"

/*
 * Qt includes
 */


/*
 * Constructor
 */
Titlebar::Titlebar( WindowCtrlWin* base, QRect geometry, char* caption )
    : QMainWindow()
{
    /*
     * Store the base
     */
    m_base = base;

    /*
     * Set window parameter for the titlebar holder (invisible windows, kills taskbar icon)
     */
    setWindowFlags( Qt::Window |
                    Qt::WindowTitleHint |
                    Qt::CustomizeWindowHint |
                    Qt::MSWindowsFixedSizeDialogHint );
    setFixedSize( 512, 1 );
    hide();

    /*
     * Setup the titlebar which is shown and handled
     */
    TitlebarWidget* m_titlebar = new TitlebarWidget( this, geometry, caption );

    connect( m_titlebar, &TitlebarWidget::signalMaximized, m_base, &WindowCtrlWin::slotMaximizedTitlebar );
    connect( m_titlebar, &TitlebarWidget::signalMinimized, m_base, &WindowCtrlWin::slotMinimizedTitlebar );
    connect( m_titlebar, &TitlebarWidget::signalClose, m_base, &WindowCtrlWin::slotCloseTitlebarAndShade );

    m_winid = m_titlebar->winId();
}


/*
 * Get the winid of the titlebar
 */
WId Titlebar::getWinId()
{
    return m_winid;
}
