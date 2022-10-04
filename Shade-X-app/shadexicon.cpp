#include "shadexicon.h"

/*
 *	Local includes
 */
#include "preferences.h"

/*
 *	Qt includes
 */


/*
 *	Constructor
 */
ShadeXIcon::ShadeXIcon( Preferences* pref, QObject* parent )
    : QSystemTrayIcon( QIcon( ":/files/icons/Shade-X.png" ), parent )
{
    /*
     *  Initialize
     */
    m_pref = pref;

    connect( this, &QSystemTrayIcon::activated, this, &ShadeXIcon::slotIconActivated );
}


/*
 *  Set and render the icon in the system tray
 */
void    ShadeXIcon::renderIcon()
{
    QPixmap pixmap( ":/files/icons/Shade-X.png" );

    /*
     *  Set the tray icon
     */
    QSystemTrayIcon::setIcon( QIcon( pixmap ) );
}


/*
 *  Handle the theme change signal
 */
void    ShadeXIcon::slotThemeChange()
{
    renderIcon();
}


/*
 *  Handle activation of the tray icon
 */
void    ShadeXIcon::slotIconActivated( QSystemTrayIcon::ActivationReason reason )
{
    switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::MiddleClick:
        {
            //  Clicked
            emit signalUnshadeAll();
            break;
        }

        case QSystemTrayIcon::DoubleClick:
        {
            break;
        }

        default:
        {
            break;
        }
    }
}
