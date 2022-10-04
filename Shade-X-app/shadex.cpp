#include "shadex.h"

/*
 *	Local includes
 */
#include "debugwidget.h"
#include "preferencesdialog.h"
#include "shadexicon.h"
#include "windowctrl.h"

/*
 *	Qt includes
 */
#include <QCoreApplication>
#include <QMenu>
#include <QIcon>
#include <QTimer>


/*
 *  Constants
 */


/*
 *  Constructor
 */
ShadeX::ShadeX( QWidget* parent ) : QMainWindow( parent )
{
    /*
     * Set window parameter
     */
    setWindowFlags( Qt::Window |
                    Qt::WindowTitleHint |
                    Qt::CustomizeWindowHint |
                    Qt::MSWindowsFixedSizeDialogHint );
    setFixedSize( 512, 1 );

    /*
     *  Initialize
     */
    m_tray_icon = nullptr;
    m_tray_icon_menu = nullptr;
    m_locale = QString("");

    /*
     *  Setup preferences storage
     */
    m_preferences = new Preferences( this );

    /*
     *  Setup window control
     */
    m_win_ctrl = new WindowCtrl( m_preferences, this );
    m_win_ctrl->setDebug( m_debug );

    /*
     *  Setup preferences dialog
     */
    m_pref_dialog = new PreferencesDialog( m_preferences, this );

    /*
     *  Setup debug window
     */
    m_debug = new DebugWidget( m_preferences );
    if( m_preferences->getDebug() ) {
        m_debug->show();
    }

    /*
     *  Connect debug link signals
     */
    connect( this, &ShadeX::signalConsole, m_debug, &DebugWidget::slotConsole );
    connect( m_preferences, &Preferences::signalConsole, m_debug, &DebugWidget::slotConsole );
    connect( m_pref_dialog, &PreferencesDialog::signalConsole, m_debug, &DebugWidget::slotConsole );
    connect( m_win_ctrl, &WindowCtrl::signalConsole, m_debug, &DebugWidget::slotConsole );

    connect( m_debug, &DebugWidget::signalTest1ButtonClicked, m_win_ctrl, &WindowCtrl::slotWindowTest1 );
    connect( m_debug, &DebugWidget::signalTest2ButtonClicked, m_win_ctrl, &WindowCtrl::slotWindowTest2 );
    connect( m_debug, &DebugWidget::signalTest3ButtonClicked, m_win_ctrl, &WindowCtrl::slotWindowTest3 );
    connect( m_debug, &DebugWidget::signalTest4ButtonClicked, m_win_ctrl, &WindowCtrl::slotWindowTest4 );

    connect( this, &ShadeX::signalConsole, m_debug, &DebugWidget::slotConsole );

    /*
     *  Connect preferences signals
     */
    connect( m_preferences, &Preferences::signalThemeChange, m_pref_dialog, &PreferencesDialog::slotThemeChange );
    connect( m_preferences, &Preferences::signalDebugChange, m_pref_dialog, &PreferencesDialog::slotDebugChange );

    connect( m_preferences, &Preferences::signalDebugChange, m_debug, &DebugWidget::slotDebugChange );

    /*
     * Show the icon
     */
    showTrayIcon();

//    SetWindowLong( (HWND)winId(), GWL_EXSTYLE,
//        GetWindowLong( (HWND)winId(), GWL_EXSTYLE) & ~WS_EX_APPWINDOW & ~WS_EX_TOOLWINDOW & WS_EX_NOACTIVATE );

//    QTimer::singleShot( 2000, this, &ShadeX::splashTimeout );
}


/*
 * Destructor
 */
ShadeX::~ShadeX()
{
    /*
     * Hide the icon
     */
    hideTrayIcon();
}


void    ShadeX::splashTimeout()
{
    SetWindowLong( (HWND)winId(), GWL_EXSTYLE,
        GetWindowLong( (HWND)winId(), GWL_EXSTYLE) & ~WS_EX_APPWINDOW & ~WS_EX_TOOLWINDOW & WS_EX_NOACTIVATE );
}

/*
 *  Create the menu for the system tray icon
 */
void    ShadeX::createMenu()
{
    m_unshade_action = new QAction(tr("&Unshade all"), this);
    m_unshade_action->setIcon( QIcon( ":/files/icons/window-restore.png" ) );
    connect( m_unshade_action, &QAction::triggered, m_win_ctrl, &WindowCtrl::slotUnshadeAll );

#ifdef DEBUG_TEST_ACTIONS
    m_test_action = new QAction(tr("&Test"), this);
    m_test_action->setIcon( QIcon( ":/files/icons/Shade-X.png" ) );
    connect( m_test_action, &QAction::triggered, this, &ShadeX::slotTest );
#endif

    m_pref_action = new QAction(tr("&Preferences"), this);
    m_pref_action->setIcon( QIcon( ":/files/icons/gtk-preferences.png" ) );
    connect( m_pref_action, &QAction::triggered, m_pref_dialog, &PreferencesDialog::showNormal );

    m_about_action = new QAction(tr("&About"), this);
    m_about_action->setIcon( QIcon( ":/files/icons/help-about.png" ) );
    connect( m_about_action, &QAction::triggered, this, &ShadeX::slotAbout );

    m_quit_action = new QAction( tr("&Quit"), this );
    m_quit_action->setIcon( QIcon( ":/files/icons/window-close.png" ) );
    connect( m_quit_action, &QAction::triggered, this, &ShadeX::slotShutdown );

    /*
     *  Setup menu
     */
    m_tray_icon_menu = new QMenu();

    m_tray_icon_menu->addAction( m_unshade_action );
    m_tray_icon_menu->addSeparator();
#ifdef DEBUG_TEST_ACTIONS
    m_tray_icon_menu->addAction( m_test_action );
    m_tray_icon_menu->addSeparator();
#endif
    m_tray_icon_menu->addAction( m_pref_action );
    m_tray_icon_menu->addAction( m_about_action );
    m_tray_icon_menu->addSeparator();
    m_tray_icon_menu->addAction( m_quit_action );
}


/*
 *  Destroy the icon menu
 */
void    ShadeX::destroyMenu()
{
    if( m_tray_icon_menu )
    {
        delete m_tray_icon_menu;
        m_tray_icon_menu = nullptr;
    }
}


/*
 *  Show / create tray icon
 */
void    ShadeX::showTrayIcon()
{
    if( !m_tray_icon )
    {        
        /*
         *  Setup tray menu
         */
        createMenu();

        /*
         *  Create system tray icon
         */
        m_tray_icon = new ShadeXIcon( m_preferences );
        m_tray_icon->setContextMenu( m_tray_icon_menu );

        /*
         *  Connect the world
         */
        connect( m_tray_icon, &ShadeXIcon::signalUnshadeAll, m_win_ctrl, &WindowCtrl::slotUnshadeAll );
        connect( m_preferences, &Preferences::signalThemeChange, m_tray_icon, &ShadeXIcon::slotThemeChange );

        /*
         *  Show it
         */
        m_tray_icon->show();
    }
}


/*
 *  Hide / remove tray icon
 */
void    ShadeX::hideTrayIcon()
{
    if( m_tray_icon )
    {
        /*
         *  Disconnect all signals
         */
        disconnect( m_tray_icon, &ShadeXIcon::signalUnshadeAll, m_win_ctrl, &WindowCtrl::slotUnshadeAll );
        disconnect( m_preferences, &Preferences::signalThemeChange, m_tray_icon, &ShadeXIcon::slotThemeChange );

        /*
         *  Hide the icon  first to prevent "ghosts"
         */
//        m_tray_icon->hide();

        /*
         *  Remove the icon
         */
        delete m_tray_icon;
        m_tray_icon = nullptr;

        /*
         *  Destroy the mennu
         */
        destroyMenu();
    }
}


/*
 *  Quit the app by quit menu
 */
void    ShadeX::slotShutdown()
{
    /*
     *  Hide systray icon to prevent ghost systray icon in Windows
     */
    if( m_tray_icon )
    {
        m_tray_icon->hide();
    }

    /*
     *  Let's quit
     */
    QCoreApplication::quit();
}


/*
 *  Show the about dialog
 */
void    ShadeX::slotAbout()
{
    Ui::AboutDialog ui;
    QDialog dialog;

    ui.setupUi( &dialog );

    ui.version->setText( m_preferences->getVersion() );
    ui.build->setText( m_preferences->getBuild() );
    ui.hash->setText( m_preferences->getHash() );
    ui.branch->setText( m_preferences->getBranch() );

    /*
     *  Set the translation for the button box
     */
    ui.buttonBox->button( QDialogButtonBox::Close )->setText( tr( "Close" ) );

    dialog.exec();
}


#ifdef DEBUG_TEST_ACTIONS

/*
 *  Test action slot
 */
void    ShadeX::slotTest()
{
    m_win_ctrl->slotDblClickMyself();
}

#endif


/*
 *  Handle a change in locale
 */
void    ShadeX::slotLoadLanguage( QString locale )
{
    if( m_locale != locale)
    {
        m_locale = locale;

        if (!m_translator.isEmpty())
        {
            qApp->removeTranslator( &m_translator );
        }

        QString locale_path = "Shade-X."+ locale;
        m_translator.load( locale_path, ":/languages/" );
//        bool status = m_translator.load( locale_path, ":/languages/" );
//        emit signalConsole( QString( "Language loaded %1").arg(status));
        qApp->installTranslator( &m_translator );
    }
}
