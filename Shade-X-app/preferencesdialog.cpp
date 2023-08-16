#include "preferencesdialog.h"
#include "ui_preferences.h"

/*
 *	Local includes
 */

/*
 *	Qt includes
 */
#include <QEvent>
#include <QKeyEvent>

/*
 *  Constructor
 */
PreferencesDialog::PreferencesDialog( Preferences *pref, QWidget *parent ) : QDialog( parent ), m_ui( new Ui::PreferencesDialog )
{
    m_ui->setupUi( this );

    /*
     *  Store link to preferences
     */
    m_pref = pref;

    /*
     *  Always start with the first tab
     */
    m_ui->tabWidget->setCurrentIndex(0);

    /*
     *  Signals and slots
     */
    connect( m_ui->savePushButton, &QPushButton::clicked, this, &PreferencesDialog::slotAccept);
    connect( m_ui->cancelPushButton, &QPushButton::clicked, this, &PreferencesDialog::slotReject);

    /*
     *  Set theme button Ids
     */
    m_ui->themeButtonGroup->setId( m_ui->lightRadioButton, Preferences::PREF_THEME_LIGHT);
    m_ui->themeButtonGroup->setId( m_ui->darkRadioButton, Preferences::PREF_THEME_DARK );

    /*
     *  Set double click time button Ids
     */
    m_ui->doubleClickTimeButtonGroup->setId( m_ui->systemTimeRadioButton, Preferences::PREF_DBL_SYSTEM);
    m_ui->doubleClickTimeButtonGroup->setId( m_ui->darkRadioButton, Preferences::PREF_DBL_CUSTOM );

    /*
     *  Dialog on top
     */
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowStaysOnTopHint;
    setWindowFlags( flags );

    /*
     * Setup the stored values
     */
    setDebug( m_pref->getDebug() );
    setTheme( m_pref->getTheme() );
    setDblClickTime( m_pref->getDblClickTime() );
    setCustomDblClickTime( m_pref->getCustomDblClickTime() );
}


/*
 *  Handle the language change event
 */
void PreferencesDialog::changeEvent( QEvent *event )
{
    QDialog::changeEvent( event );

    if( QEvent::LanguageChange == event->type() )
    {
        m_ui->retranslateUi( this );
    }
}


/*
 *  Handle key event
 */
void PreferencesDialog::keyPressEvent( QKeyEvent *event )
{
    if( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
    {
        slotAccept();

        return;
    }

    if( event->key() == Qt::Key_Escape )
    {
        slotReject();

        return;
    }

    QDialog::keyPressEvent( event );
}



/*
 *  Set the debug state
 */
void    PreferencesDialog::setDebug( bool state )
{
   m_ui->debugWindowCheckBox->setChecked( state );
}


/*
 *  Set the theme
 */
void    PreferencesDialog::setTheme( Preferences::Theme theme )
{
   ( m_ui->themeButtonGroup->button( theme ) )->setChecked( true );
}


/*
 *  Set the double click time
 */
void    PreferencesDialog::setDblClickTime( Preferences::DoubleClickTime time )
{
   ( m_ui->doubleClickTimeButtonGroup->button( time ) )->setChecked( true );
}


/*
 *  Set the custom double click time spinner
 */
void    PreferencesDialog::setCustomDblClickTime( int time )
{
    m_ui->doubleClickSpinBox->setValue( time );
}


/*
 *  Handle the accept signal
 */
void    PreferencesDialog::slotAccept()
{
    /*
     *  Get all the selected values and store them in the preferences
     */
    Preferences::Theme theme = static_cast< Preferences::Theme >( m_ui->themeButtonGroup->checkedId() );
    m_pref->setTheme( theme );

    Preferences::DoubleClickTime time = static_cast< Preferences::DoubleClickTime >( m_ui->doubleClickTimeButtonGroup->checkedId() );
    m_pref->setDblClickTime( time );

    int custom_time = m_ui->doubleClickSpinBox->value();
    m_pref->setCustomDblClickTime( custom_time );

    m_pref->setDebug( m_ui->debugWindowCheckBox->isChecked() );

    /*
     *  Close it
     */
//    QDialog::accept();
    hide();
}


/*
 *  Handle the accept signal
 */
void    PreferencesDialog::slotReject()
{
    /*
     *  Close it
     */
//    QDialog::reject();
    hide();

    /*
     *  Reset all parameters
     */
    setTheme( m_pref->getTheme() );
    setDblClickTime( m_pref->getDblClickTime() );
    setCustomDblClickTime( m_pref->getCustomDblClickTime() );
    setDebug( m_pref->getDebug());
}


/*
 *  Handle the debug change signal
 */
void    PreferencesDialog::slotDebugChange()
{
    setDebug( m_pref->getDebug() );
}


/*
 *  Handle the theme change signal
 */
void    PreferencesDialog::slotThemeChange()
{
    setTheme( m_pref->getTheme() );
}


/*
 *  Handle the double click time change signal
 */
void    PreferencesDialog::slotDblClickTimeChange()
{
    setDblClickTime( m_pref->getDblClickTime() );
}


/*
 *  Handle the customdouble click time change signal
 */
void    PreferencesDialog::slotCustomDblClickTimeChange()
{
    setCustomDblClickTime( m_pref->getCustomDblClickTime() );
}
