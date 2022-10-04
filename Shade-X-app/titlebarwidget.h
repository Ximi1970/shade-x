#ifndef TITLEBARWIDGET_H
#define TITLEBARWIDGET_H

/*
 *	Local includes
 */
#include "preferences.h"

/*
 *	Qt includes
 */
#include <QWidget>


/**
 * @brief The TitlebarWidget class
 */
class TitlebarWidget : public QWidget
{
    Q_OBJECT

    public:

        /**
         * @brief TitlebarWidget
         *
         *  @param parent.      Pointer to the parent.
         *  @param geometry     The geometry.
         *  @param caption.     The text in the titlebar.
         */
        explicit TitlebarWidget( QWidget* parent, QRect geometry, char* caption );

    protected:

        /**
         * @brief event
         *
         *  @param event
         *  @return
         */
        bool    event( QEvent* event );

        /**
         * @brief closeEvent. Handle the close event.
         *
         *  @param event.   The event.
         */
        void    closeEvent( QCloseEvent* event );

    signals:

        /**
         * @brief signalMaximized. Signal the maximized state.
         *
         *  @param hwnd
         */
        void    signalMaximized( WId hwnd );

        /**
         * @brief signalMinimized. Signal the minimized state.
         *
         *  @param hwnd
         */
        void    signalMinimized( WId hwnd );

        /**
         * @brief signalClose. Signal the close.
         *
         *  @param hwnd
         */
        void    signalClose( WId hwnd );
};

#endif // TITLEBARWIDGET_H
