/*
 *	Local includes
 */
#include "shadex.h"


/*
 *	Qt includes
 */
#include <QApplication>

int main( int argc, char *argv[] )
{
    QApplication a( argc, argv );
    a.setOrganizationName("Ximi1970");
    a.setApplicationName("Shade-X");
    a.setQuitOnLastWindowClosed( false );

    ShadeX shadex;
    shadex.show();

    return a.exec();
}
