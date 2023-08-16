#include "preferences.h"

/*
 *	Local includes
 */


/*
 *  System includes
 */

/*
 *	Qt includes
 */


/**
 * @brief Preferences.  Constructor.
 */
Preferences::Preferences( QObject *parent ) : QObject( parent )
{
    /*
     *  Initialize
     */
    m_version_major = QLatin1String( APP_VERSION_MAJOR );
    m_version_minor = QLatin1String( APP_VERSION_MINOR );
    m_version_patch = QLatin1String( APP_VERSION_PATCH );

    m_version_build = QLatin1String( APP_BUILD );
    m_version_hash = QLatin1String( APP_GITHASH );
    m_version_branch = QLatin1String( APP_GITBRANCH );

    /*
     * Get the settings
     */
    if( m_settings.value( "debug" ) == true )
    {
        m_debug = true;
    }
    else
    {
        m_debug = false;
    }

    if( m_settings.value( "theme" ) == PREF_THEME_DARK )
    {
        m_theme = PREF_THEME_DARK;
    }
    else
    {
        m_theme = PREF_THEME_LIGHT;
    }

    if( m_settings.value( "dbl_time" ) == PREF_DBL_SYSTEM )
    {
        m_time = PREF_DBL_SYSTEM;
    }
    else
    {
        m_time = PREF_DBL_CUSTOM;
    }

    if( m_settings.value( "dbl_custom_time" ).toInt() == 0 )
    {
        m_custom_time = PREF_DBL_CUSTOM_DEFAULT;
    }
    else
    {
        m_custom_time = m_settings.value( "dbl_custom_time" ).toInt();
    }
}


/*
 *  Get the theme pref.
 */
Preferences::Theme    Preferences::getTheme() const
{
    return m_theme;
}


/*
 *  Set the theme pref.
 */
void    Preferences::setTheme( Theme theme )
{
    if( m_theme != theme )
    {
        m_theme = theme;
        m_settings.setValue( "theme", m_theme );

        /*
         *  Tell the world the new preference
         */
        emit signalThemeChange();
    }
}


/*
 *  Get the double click time pref.
 */
Preferences::DoubleClickTime    Preferences::getDblClickTime() const
{
    return m_time;
}


/*
 *  Set the double click time pref.
 */
void    Preferences::setDblClickTime( DoubleClickTime time )
{
    if( m_time != time )
    {
        m_time = time;
        m_settings.setValue( "dbl_time", m_time );

        /*
         *  Tell the world the new preference
         */
        emit signalDblClickTimeChange();
    }
}


/*
 *  Get the custom double click time pref.
 */
int    Preferences::getCustomDblClickTime() const
{
    return m_custom_time;
}


/*
 *  Set the custom double click time pref.
 */
void    Preferences::setCustomDblClickTime( int time )
{
    if( m_custom_time != time )
    {
        m_custom_time = time;
        m_settings.setValue( "dbl_custom_time", m_custom_time );

        /*
         *  Tell the world the new preference
         */
        emit signalCustomDblClickTimeChange();
    }
}


/*
 *  Get the debug state.
 */
bool    Preferences::getDebug() const
{
    return m_debug;
}


/*
 *  Set the debug state.
 */
void    Preferences::setDebug( bool state )
{
    if( m_debug != state )
    {
        m_debug = state;
        m_settings.setValue( "debug", m_debug );

        /*
         *  Tell the world the new preference
         */
        emit signalDebugChange();
    }
}


/*
 *  Get the software version.
 */
QString    Preferences::getVersion() const
{
    return m_version_major + "." + m_version_minor + "." + m_version_patch;
}


/*
 *  Get the number of commits.
 */
QString    Preferences::getBuild() const
{
    return m_version_build;
}


/*
 *  Get the git hash.
 */
QString    Preferences::getHash() const
{
    return m_version_hash;
}


/*
 *  Get the software version.
 */
QString    Preferences::getBranch() const
{
    return m_version_branch;
}
