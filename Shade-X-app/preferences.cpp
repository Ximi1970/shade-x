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
