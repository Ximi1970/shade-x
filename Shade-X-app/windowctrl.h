#ifndef WINDOWCTRL_H
#define WINDOWCTRL_H

#include "windowctrl-win.h"

/*
 *  Predefines
 */
class Preferences;

/**
 * @brief The WindowCtrl class.
 */
class WindowCtrl : public WindowCtrlWin
{
    Q_OBJECT

    public:

        /**
         * @brief WindowCtrlUnix. Constructor.
         *
         * @param parent    My parent.
         */
        explicit WindowCtrl( Preferences* pref, QWidget* parent = nullptr );

    signals:

    public slots:

        /**
         * @brief slotWindowTest1. Start a test.
         */
        void    slotWindowTest1();

        /**
         * @brief slotWindowTest2. Start a test.
         */
        void    slotWindowTest2();

        /**
         * @brief slotWindowTest3. Start a test.
         */
        void    slotWindowTest3();

        /**
         * @brief slotWindowTest4. Start a test.
         */
        void    slotWindowTest4();

        /**
         * @brief slotUnshadeAll. Slot for handling of unshading all windows signal.
         */
        void    slotUnshadeAll();

    private:

        /**
         * @brief m_pref. Pointer to the preferences storage.
         */
        Preferences*    m_pref;

        /**
         * @brief m_show_hide_active
         */
        bool    m_show_hide_active;
};


#endif // WINDOWCTRL_H
