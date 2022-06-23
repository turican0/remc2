# Protect from building on incompatible Qt version
lessThan(QT_MAJOR_VERSION, 6):{
    lessThan(QT_MAJOR_VERSION, 5): error("You are using an old version of Qt. To build project need a Qt 5.4 and greater!")
    lessThan(QT_MINOR_VERSION, 4): error("You are using an old version of Qt. To build project need a Qt 5.4 and greater!")
} else {
    warning("Building under Qt 6 and greater may be incompatible with current project Please use Qt 5.7 before project will be ported to Qt6 or greater!")
}

# Resolve architecture type
android:{
    ARCH=android_arm
} else : unix: {
    linux-g++-32: ARCH=x32
    linux-g++-64: ARCH=x64
    isEmpty(ARCH) {
        UNAMEARCH=$$QMAKE_HOST.arch
        equals(UNAMEARCH, "x86_64") {
            ARCH=x64
        } else {
            ARCH=x32
        }
    }
} else {
    !contains(QMAKE_TARGET.arch, x86_64) {
        ARCH=x32
    } else {
        ARCH=x64
    }
}

static: {
    LINKTYPE=static
} else {
    LINKTYPE=dynamic
}

CONFIG(release, debug|release): {
    CONFIG -= debug
    CONFIG += pge_release
    BUILDTP=release
}
CONFIG(debug, debug|release): {
    CONFIG -= release
    CONFIG += pge_debug
    BUILDTP = debug
    DEFINES += DEBUG_BUILD=1
}

BUILD_OBJ_DIR=$$PWD/OBJ

GIT_VERSION = $$system(git --git-dir $$PWD/../.git --work-tree $$PWD/../ describe --always)
DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"

OBJECTS_DIR = $$BUILD_OBJ_DIR/_build_$$ARCH/$$TARGET/_$$BUILDTP/.obj
MOC_DIR     = $$BUILD_OBJ_DIR/_build_$$ARCH/$$TARGET/_$$BUILDTP/.moc
RCC_DIR     = $$BUILD_OBJ_DIR/_build_$$ARCH/$$TARGET/_$$BUILDTP/.rcc
UI_DIR      = $$BUILD_OBJ_DIR/_build_$$ARCH/$$TARGET/_$$BUILDTP/.ui
message("$$TARGET will be built as $$BUILDTP $$ARCH $$TARGETOS ($$QMAKE_TARGET.arch) $${LINKTYPE}ally in $$OBJECTS_DIR, GIT version $$GIT_VERSION")

# Compiler info
*-g++:      message("GCC compiler will be used")
*-clang:    message("CLANG compiler will be used")
win*-msvc*: message("MSVC compiler will be used")

# Global optimization flags
!win*-msvc*:    QMAKE_CFLAGS_RELEASE += -fno-omit-frame-pointer
!win*-msvc*:    QMAKE_CXXFLAGS_RELEASE += -fno-omit-frame-pointer
# set minimal version 10.10
macx:           QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10

