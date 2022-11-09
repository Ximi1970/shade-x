#include "windowctrl.h"

/*
 *  System includes
 */

/*
 *  Local includes
 */
#include "debug.h"
#include "preferences.h"

/*
 *  Qt includes
 */


/*
 *  Constructor
 */
WindowCtrl::WindowCtrl( Preferences* pref, QWidget* parent )
    : WindowCtrlWin( parent )
{
    /*
     *  Store preferences
     */
    m_pref = pref;

    /*
     *  Initialize
     */
    m_show_hide_active = false;
}


/*
 *  Test func 1
 */
void    WindowCtrl::slotWindowTest1()
{
//    emit signalConsole("Test 1 started");

    // Do something.
    emit signalConsole( "" );


//    emit signalConsole("Test 1 done");
}


/*
 *  Test func 2
 */
void    WindowCtrl::slotWindowTest2()
{
    emit signalConsole("Test 2 started");

    // Do something.

    emit signalConsole("Test 2 done");
}


/*
 *  Test func 3
 */
void    WindowCtrl::slotWindowTest3()
{
    emit signalConsole("Test 3 started");

    // Do something.

    emit signalConsole("Test 3 done");
}


/*
 *  Test func 4
 */
void    WindowCtrl::slotWindowTest4()
{
    emit signalConsole("Test 4 started");

    // Do something.

    emit signalConsole("Test 4 done");
}



/*
 *  Handle show / hide signal
 */
void    WindowCtrl::slotUnshadeAll()
{
    unshadeAll();
}
