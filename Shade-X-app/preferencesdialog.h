#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

/*
 *	Local includes
 */
#include "preferences.h"

/*
 *	Qt includes
 */
#include <QDialog>

/*
 *	Predefines
 */
class QEvent;
class QKeyEvent;

/*
 *  Namespace
 */
namespace Ui {
    class PreferencesDialog;
}


/**
 * @brief The PreferencesDialog class. Handles the preferences.
 */
class PreferencesDialog : public QDialog
{
    Q_OBJECT

    public:

        /**
         * @brief PreferencesDialog
         */
        PreferencesDialog( Preferences *pref, QWidget *parent = nullptr );

        /**
         * @brief changeEvent. Catch the event.
         *
         *  @param event    The event
         */
        void    changeEvent( QEvent* event );

        /**
         * @brief keyPressEvent. Catch the event.
         *
         *  @param event    The event
         */
        void    keyPressEvent( QKeyEvent *event );

    private:

        /**
         * @brief setDebug. Set the debug state.
         *
         *  @param state    The state.
         */
        void    setDebug( bool state );

        /**
         * @brief setTheme. Set the theme.
         *
         * @param theme     The theme.
         */
        void    setTheme( Preferences::Theme theme );

        /**
         * @brief setDblClickTime. Set the double click time.
         *
         * @param time     The time.
         */
        void    setDblClickTime( Preferences::DoubleClickTime time );

        /**
         * @brief setCustomDblClcikTime. Set the custom double click time.
         *
         * @param time     The time.
         */
        void    setCustomDblClickTime( int time );

    signals:

        /**
         * @brief signalConsole. Send a console message.
         *
         *  @param message      The message.
         */
        void    signalConsole( QString message );

    public slots:

        /**
         * @brief slotDebugChange. Slot for handling debug change signals.
         */
        void    slotDebugChange();

        /**
         * @brief slotThemeChange. Slot for handling theme change.
         */
        void    slotThemeChange();

        /**
         * @brief slotDblClickTimeChange. Slot for handling double click time change.
         */
        void    slotDblClickTimeChange();

        /**
         * @brief slotCustomDblClickTimeChange. Slot for handling custom double click time change.
         */
        void    slotCustomDblClickTimeChange();

    private slots:

        /**
         * @brief slotAccept. Store the preferences on the accept signal.
         */
        void    slotAccept();

        /**
         * @brief slotReject. Store the preferences on the accept signal.
         */
        void    slotReject();

    private:

        /**
         * @brief m_ui. Pointer to the dialog.
         */
        Ui::PreferencesDialog *m_ui;

        /**
         * @brief m_pref. Pointer to the preferences storage.
         */
        Preferences *m_pref;
};

#endif // PREFERENCESDIALOG_H
