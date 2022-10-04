#
#	Git version getters
#
win32: {
    contains(QMAKE_HOST.os, Windows): {
        #
        #	Host is Windows
        #
        GIT = ""C:\\Progra~1\\Git\\bin\\git.exe""
    }
    else {
        #
        #	Host is Linux, cross compiling with mingw
        #
        GIT = git
    }
}

#
#	Define the app version strings
#
BUILD_NUMBER = $$system($$GIT rev-list --count HEAD)
GIT_HASH = $$system($$GIT rev-parse HEAD)
GIT_BRANCH = $$system($$GIT rev-parse --abbrev-ref HEAD)

GIT_VERSION_LONG = $$system($$GIT describe --long)
GIT_VERSION = $$section(GIT_VERSION_LONG, -, 0, 0)

VERSION_MAJOR = $$section(GIT_VERSION, ., 0, 0)
VERSION_MINOR = $$section(GIT_VERSION, ., 1, 1)
VERSION_PATCH = $$section(GIT_VERSION, ., 2, 2)

!contains(DEFINES,EXT_VERSION) {
    DEFINES += APP_VERSION_MAJOR=\\\"$$VERSION_MAJOR\\\"
    DEFINES += APP_VERSION_MINOR=\\\"$$VERSION_MINOR\\\"
    DEFINES += APP_VERSION_PATCH=\\\"$$VERSION_PATCH\\\"
    DEFINES += APP_BUILD=\\\"$$BUILD_NUMBER\\\"
    DEFINES += APP_GITHASH=\\\"$$GIT_HASH\\\"
    DEFINES += APP_GITBRANCH=\\\"$$GIT_BRANCH\\\"

    message("Buildnumber: "$$BUILD_NUMBER)
    message("Git hash: "$$GIT_HASH)
    message("Git branch: "$$GIT_BRANCH)
    message("Version: "$$VERSION_MAJOR"."$$VERSION_MINOR"."$$VERSION_PATCH)
    #message($$QMAKESPEC)
}

#
# Set build dirs
#
DISTDIR = $$PWD/dist
Release:DESTDIR = $$PWD/build/release
Release:OBJECTS_DIR = $$PWD/build/release/.obj
Release:MOC_DIR = $$PWD/build/release/.moc
Release:RCC_DIR = $$PWD/build/release/.rcc
Release:UI_DIR = $$PWD/build/release/.ui

Debug:DESTDIR = $$PWD/build/debug
Debug:OBJECTS_DIR = $$PWD/build/debug/.obj
Debug:MOC_DIR = $$PWD/build/debug/.moc
Debug:RCC_DIR = $$PWD/build/debug/.rcc
Debug:UI_DIR = $$PWD/build/debug/.ui


#
# Copies the given files to the destination directory
#
defineTest(copyToDestdir) {
    FILES = $$1
    DDIR = $$2

    for(FILE, FILES) {

        # Replace slashes in paths with backslashes for Windows
        win32: {
            FILE ~= s,/,\\,g
            DDIR ~= s,/,\\,g

            QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
        }
    }

    export(QMAKE_POST_LINK)
}


#
# Delete directory
#
defineTest(delDir) {
    TARGETDIR = $$1

    win32: {

        exists($$TARGETDIR) {
            TARGETDIR ~= s,/,\\,g

            QMAKE_POST_LINK += rmdir /S /Q $$quote($$TARGETDIR) $$escape_expand(\\n\\t)
        }
    }

    export(QMAKE_POST_LINK)
}


#
# Create directory
#
defineTest(mkDir) {
    TARGETDIR = $$1

    win32: {
        TARGETDIR ~= s,/,\\,g

        QMAKE_POST_LINK += mkdir $$quote($$TARGETDIR) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}
