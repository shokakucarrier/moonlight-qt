#include "gui/mainwindow.h"
#include <QApplication>

#include "backend/nvhttp.h"

// Don't let SDL hook our main function, since Qt is already
// doing the same thing
#define SDL_MAIN_HANDLED
#include <SDL.h>

int main(int argc, char *argv[])
{
    // This avoids using the default keychain for SSL, which may cause
    // password prompts on macOS.
    qputenv("QT_SSL_USE_TEMPORARY_KEYCHAIN", QByteArray("1"));

    // Set these here to allow us to use the default QSettings constructor
    QCoreApplication::setOrganizationName("Moonlight Game Streaming Project");
    QCoreApplication::setOrganizationDomain("moonlight-stream.com");
    QCoreApplication::setApplicationName("Moonlight");

    // Register custom metatypes for use in signals
    qRegisterMetaType<NvApp>("NvApp");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // Ensure that SDL is always initialized since we may need to use it
    // for non-streaming purposes (like checking on audio devices)
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO |
                 SDL_INIT_AUDIO |
                 SDL_INIT_GAMECONTROLLER) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "SDL_Init() failed: %s",
                     SDL_GetError());
    }

    int err = a.exec();

    SDL_Quit();

    return err;
}