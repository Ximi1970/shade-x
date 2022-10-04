#ifndef SHADEXICON_H
#define SHADEXICON_H

/*
 *	Local includes
 */
#include "preferences.h"

/*
 *	Qt includes
 */
#include <QSystemTrayIcon>

/*
 *	Predefines
 */


/**
 * @brief The ShadeXIcon class. The system tray icon.
 */
class ShadeXIcon : public QSystemTrayIcon
{
    Q_OBJECT

    public:

        /**
         * @brief ShadeXIcon. Constructor.
         *
         *  @param parent   My parent.
         */
        ShadeXIcon( Preferences* pref, QObject* parent = nullptr );

    private:

        /**
         * @brief setIcon. Set a new rendered icon.
         */
        void    renderIcon();

    signals:

        /**
         * @brief signalUnshadeAll. Signal unshade all windows.
         */
        void    signalUnshadeAll();

   public slots:

        /**
         * @brief slotThemeChange. Slot for handling theme change signals.
         */
        void    slotThemeChange();

    private slots:

        /**
         * @brief slotIconActivated. Hnadle activation signal of the tray icon.
         *
         *  @param reason   Activation reason.
         */
        void    slotIconActivated( QSystemTrayIcon::ActivationReason reason );

    private:

        /**
         * @brief m_pref    Pointer to the preferences storage.
         */
        Preferences*    m_pref;
};

#endif // SHADEXICON_H
