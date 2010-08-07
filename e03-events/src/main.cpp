
#include <windows/eeWindowSystemFactory.h>
#include <engines/eeGLEngine.h>
#include <cameras/eeOrthoCamera.h>
#include <interface/eeGUIBuilder.h>

#include <iostream>

using namespace EE;

int main(void) {
    int winHeight = 240;
    int winWidth = 320;
    WindowSystem *winsys = WindowSystemFactory::createWindowSystem();

    EE::Window *win = winsys->createWindow("enfent-01", 0, 0, winWidth, winHeight);
    winsys->show(win);

    GLEngine *engine = new GLEngine();
    engine->initialize(win);

    GPUStateManager *smgr = engine->getStateManager();

    smgr->setDepthTestState(DepthTestState(LEQUAL));
    smgr->setClearDepthValue(1.0);
    smgr->setClearColorValue(0.0, 0.0, 0.0, 0.0);

    OrthoCamera *cam2d = new OrthoCamera(engine, win->getClientViewport(), false, true);
    cam2d->apply();

    Sprite *spr = GUIBuilder::createSpriteFromFile(engine, std::string("data/butterfly.png"));
    CHECK_POINTER(spr);

    spr->setPosition(Vector3f(winWidth / 4, winHeight / 4, 0.5));
    spr->setSize(winWidth / 2, winHeight / 2);

    bool finish = false;

    while (!finish)
    {
        while (win->hasAvailableEvent())
        {
            SystemEvent *evt = winsys->getNextEvent(win);

            if (evt)
            {
                switch (evt->getType())
                {
                    case KEY_PRESS:
                        std::cerr << "Tastendruck" << std::endl;
                        break;
                    case MOUSE_PRESS:
                        std::cerr << "Mausdruck" << std::endl;
                        break;
                    case WINDOW_CLOSE:
                        finish = true;
                        break;
                    default:
                        break;
                }
            }
        }

        engine->clear(ClearFlag(COLOR | DEPTH));
        spr->render();
        win->swap();
    }

}
