#ifndef TITLEBAR_H
#define TITLEBAR_H

/*
 *	Local includes
 */
#include "preferences.h"

/*
 *	Qt includes
 */
#include <QMainWindow>

/*
 *	Predefines
 */
class QWidget;
class WindowCtrlWin;

/**
 * @brief The Dummy class
 */
class Titlebar : public QMainWindow
{
    Q_OBJECT

    public:

        /**
         * @brief Titlebar. Holder for the titlebar.
         *
         *  @param geometry     The geometry.
         *  @param caption.     The text in the titlebar.
         */
        explicit Titlebar( WindowCtrlWin* base, QRect geometry, char* caption );

        /**
         * @brief getWinId. Get the winid of the titlebar.
         *
         *   @return
         */
        WId getWinId();

    private:

        /**
         * @brief m_base. Pointer to the base class.
         */
        WindowCtrlWin*  m_base;

        /**
         * @brief m_titlebar. Pointer to the titlebar.
         */
        QWidget*   m_titlebar;

        /**
         * @brief m_winid. The winid of the titlebar.
         */
        WId m_winid;
};

#endif // TITLEBAR_H
