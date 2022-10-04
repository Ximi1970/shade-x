#ifndef PREFERENCES_H
#define PREFERENCES_H

/*
 *	Local includes
 */

/*
 *	Qt includes
 */
#include <QObject>
#include <QString>
#include <QSettings>


/**
 * @brief The Preferences class. Class to hold the preferences.
 */
class Preferences : public QObject
{
    Q_OBJECT

    public:

        enum Theme {
            PREF_THEME_LIGHT = 0,
            PREF_THEME_DARK
        };

    public:

        /**
         * @brief Preferences.  Constructor. Destructor.
         */
        Preferences( QObject *parent = nullptr );

        /**
         * @brief getTheme. Get the theme state.
         *
         * @return      The state.
         */
        Theme getTheme() const;

        /**
         * @brief setTheme. Set the theme state.
         *
         * @param theme     The state.
         */
        void setTheme( Theme theme );

        /**
         * @brief getDebug. Get the debug windows state.
         *
         * @return      The state.
         */
        bool getDebug() const;

        /**
         * @brief setDebug. Set the debug windows state.
         *
         * @param state     The state.
         */
        void setDebug( bool state );

        /**
         * @brief getVersion. Get the software version.
         *
         * @return      The version.
         */
        QString getVersion() const;

        /**
         * @brief getBuild. Get the git number of commits.
         *
         * @return      The number of commits.
         */
        QString getBuild() const;

        /**
         * @brief getHash. Get the git hash.
         *
         * @return      The hash.
         */
        QString getHash() const;

        /**
         * @brief getBuild. Get the git branch.
         *
         * @return      The branch.
         */
        QString getBranch() const;

    signals:

        /**
         * @brief signalConsole. Send a console message.
         *
         *  @param message      The message.
         */
        void    signalConsole( QString message );

        /**
         * @brief signalDebugChange. Signal a debug state change.
         */
        void signalDebugChange();

        /**
         * @brief signalThemeChange. Signal a theme state change.
         */
        void signalThemeChange();

    private:

        /**
         * @brief m_settings
         */
        QSettings    m_settings;

        /**
         * @brief m_debug. Display debug window.
         */
        bool m_debug;

        /**
         * @brief m_version_major. Major version number.
         */
        QString m_version_major;

        /**
         * @brief m_version_minor. Minor version number.
         */
        QString m_version_minor;

        /**
         * @brief m_version_patch. patch version number.
         */
        QString m_version_patch;

        /**
         * @brief m_version_build. Git commits count.
         */
        QString m_version_build;

        /**
         * @brief m_version_build. Git hash.
         */
        QString m_version_hash;

        /**
         * @brief m_version_build. Git branch.
         */
        QString m_version_branch;

        /**
         * @brief m_theme. The theme.
         */
        Theme m_theme;
};

#endif // PREFERENCES_H
