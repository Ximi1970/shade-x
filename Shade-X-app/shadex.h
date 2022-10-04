#ifndef SHADEX_H
#define SHADEX_H

/*
 *	Local includes
 */
#include "ui_debugwidget.h"
#include "ui_aboutdialog.h"
#include "preferences.h"

/*
 *	Qt includes
 */
#include <QMainWindow>
#include <QTranslator>
#include <QSystemTrayIcon>

/*
 *	Predefines
 */
class QAction;

class DebugWidget;
class PreferencesDialog;
class ShadeXIcon;
class WindowCtrl;

/**
 * @brief The ShadeX class
 */
class ShadeX : public QMainWindow
{
    Q_OBJECT

    public:

        /**
         * @brief ShadeX. Constructor.
         *
         *  @param parent   My parent.
         */
        explicit ShadeX( QWidget* parent = nullptr );

        /**
         * @brief ~ShadeX. Destructor.
         */
        ~ShadeX();

        void    splashTimeout();

    private:

        /**
         * @brief createActions. Create the menu actions.
         */
        void    createMenu();

        /**
         * @brief destroyMenu. Destroy the menu.
         */
        void    destroyMenu();

        /**
         * @brief showTrayIcon. Create and show the icon.
         */
        void    showTrayIcon();

        /**
         * @brief hideTrayIcon. Hide and destroy the icon.
         */
        void    hideTrayIcon();

    signals:

        /**
         * @brief signalConsole. Send a console message.
         *
         *  @param message      The message.
         */
        void    signalConsole( QString message );

    public slots:

        /**
         * @brief slotLoadLanguage. Handle locale change signal.
         *
         *  @param locale   The requested locale.
         */
        void    slotLoadLanguage( QString locale );

    private slots:

        /**
         * @brief slotShutdown. Handle shutdown request from the menu.
         */
        void    slotShutdown();

        /**
         * @brief slotAbout. Handle about request from the menu.
         */
        void    slotAbout();

#ifdef DEBUG_TEST_ACTIONS

        /**
         * @brief slotTest. Handle test action from the menu.
         */
        void    slotTest();

#endif

    private:

        /**
         * @brief m_preferences. Pointer to the preferences storage.
         */
        Preferences*    m_preferences;

        /**
         * @brief m_debug
         */
        DebugWidget*    m_debug;

        /**
         * @brief m_win_ctrl. Pointer to the window control.
         */
        WindowCtrl*     m_win_ctrl;

        /**
         * @brief m_pref_dialog. Pointer to the preferences dialog.
         */
        PreferencesDialog*  m_pref_dialog;

        /**
         * @brief m_tray_icon. Pointer to the system tray icon.
         */
        ShadeXIcon*   m_tray_icon;

        /**
         * @brief m_tray_icon_menu. Pointer to the tray icon menu.
         */
        QMenu*  m_tray_icon_menu;

        /**
         * @brief m_xxxx_action. Pointer to the menu actions.
         */
        QAction*    m_unshade_action;
#ifdef DEBUG_TEST_ACTIONS
        QAction*    m_test_action;
#endif
        QAction*    m_pref_action;
        QAction*    m_about_action;
        QAction*    m_quit_action;

        /**
         * @brief m_translator. The translator object.
         */
        QTranslator m_translator;

        /**
         * @brief m_locale. Storage for the current locale.
         */
        QString m_locale;
};

#endif // SHADEX_H
