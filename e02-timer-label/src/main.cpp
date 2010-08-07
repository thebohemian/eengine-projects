
#include <windows/eeWindowSystemFactory.h>
#include <engines/eeGLEngine.h>
#include <cameras/eeOrthoCamera.h>
#include <interface/eeGUIBuilder.h>
#include <tools/eeTimerFactory.h>

using namespace EE;

int main(void) {
    int winHeight = 240;
    int winWidth = 320;
    WindowSystem *winsys = WindowSystemFactory::createWindowSystem();

    Timer *timer = TimerFactory::createTimer();

    EE::Window *win = winsys->createWindow("enfent-01", 0, 0, winWidth, winHeight);
    winsys->show(win);

    GLEngine *engine = new GLEngine();
    engine->initialize(win);

    GPUStateManager *smgr = engine->getStateManager();

    smgr->setDepthTestState(DepthTestState(LEQUAL));
    smgr->setClearDepthValue(1.0);
    smgr->setClearColorValue(0.0, 0.0, 0.0, 0.0);

    EE::Font *font = winsys->createFont(engine,
     new FontDescriptor(COURIER_FONT_FAMILY, SLANT_ROMAN, SETWIDTH_ANY, 14, SPACING_MONOSPACED, 100, 100));
    ASSERT(font->initialize(256));

    OrthoCamera *cam2d = new OrthoCamera(engine, win->getClientViewport(), false, true);
    cam2d->apply();

    Sprite *spr = GUIBuilder::createSpriteFromFile(engine, std::string("data/butterfly.png"));
    CHECK_POINTER(spr);

    spr->setPosition(Vector3f(winWidth / 4, winHeight / 4, 0.5));
    spr->setSize(winWidth / 2, winHeight / 2);

    Interface *ifc = new Interface(engine);
    Label *label = ifc->createLabel(L"D'Enfent enfent-02", WHITE);
    label->setFont(font);
    label->setPosition(Vector3f(30, 0, 0.3));
    bool finish = false;

    int value = -50;
    int inc = 1;

    timer->reset();

    while (!finish)
    {
        while (win->hasAvailableEvent())
        {
            SystemEvent *evt = winsys->getNextEvent(win);
            if (evt != NULL && evt->getType() == WINDOW_CLOSE)
            {
                finish = true;
            }
        }

        if (timer->getElapsedTime() >= 1000 / 24)
        {
            timer->reset();

            value += inc;

            label->setPosition(label->getX(), 50 + value);

            if (value < -50 || value > 50)
                inc *= -1;
        }

        engine->clear(ClearFlag(COLOR | DEPTH));
        spr->render();
        label->render();
        win->swap();
    }

}
