#include "titlebarwidget.h"

/*
 *  System includes
 */

/*
 *	Local includes
 */

/*
 * Qt includes
 */
#include <QCloseEvent>


/*
 * Constructor
 */
TitlebarWidget::TitlebarWidget( QWidget* parent, QRect geometry, char* caption )
    : QWidget( parent )
{
    setWindowFlags( Qt::Window |
                    Qt::WindowTitleHint |
                    Qt::CustomizeWindowHint |
                    Qt::WindowMinMaxButtonsHint |
                    Qt::WindowCloseButtonHint );
    setGeometry( geometry );
    setFixedSize( geometry.width(), geometry.height() );
    setWindowTitle( QString( caption ) );
    show();
}


/*
 * Intercept event
 */
bool    TitlebarWidget::event( QEvent* event )
{
    if( event->type() == QEvent::WindowStateChange )
    {
        if( windowState().testFlag( Qt::WindowMaximized ) )
        {
            QRect rect = geometry();
            setGeometry( rect.x(), rect.y(), rect.width(), 1 );

            //  Maximized
            emit signalMaximized( winId() );
            return true;
        }

        if( windowState().testFlag( Qt::WindowMinimized ) )
        {
            //  Minimized
            emit signalMinimized( winId() );
            return true;
        }
    }

    return QWidget::event( event );
}


/*
 * Handle the close event
 */
void    TitlebarWidget::closeEvent( QCloseEvent* event )
{
    Q_UNUSED( event )

    /*
     * Signal the close action
     */
    emit signalClose( winId() );

    /*
     * Let the parent close the shade and titlebar
     */
    event->ignore();
}
