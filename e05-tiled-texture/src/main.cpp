
#include <windows/eeWindowSystemFactory.h>
#include <engines/eeGLEngine.h>
#include <cameras/eeOrthoCamera.h>
#include <interface/eeGUIBuilder.h>
#include <tools/eeTextureTool.h>

using namespace EE;

void handleKey(int);

Sprite *spr = NULL;
float xd = 0.0;

int main(void) {
    int winHeight = 1024;
    int winWidth = 768;
    WindowSystem *winsys = WindowSystemFactory::createWindowSystem();

    EE::Window *win = winsys->createWindow("enfent-05", 0, 0, winWidth, winHeight);
    winsys->show(win);

    GLEngine *engine = new GLEngine();
    engine->initialize(win);

    GPUStateManager *smgr = engine->getStateManager();

    smgr->setDepthTestState(DepthTestState(LEQUAL));
    smgr->setClearDepthValue(1.0);
    smgr->setClearColorValue(0.0, 0.0, 0.0, 0.0);

    OrthoCamera *cam2d = new OrthoCamera(engine, win->getClientViewport(), false, true);
    cam2d->apply();

    Texture *tex = TextureTool::loadFromFile(engine, std::string("data/shingle64.png"), false);
    CHECK_POINTER(tex);

    TextureSampler sampler = TextureSampler(NEAREST, LINEAR);
    sampler.sTexture = REPEAT;
    sampler.tTexture = REPEAT;

    TexturesState state = TexturesState(tex, sampler);

    spr = new Sprite(engine);
    CHECK_POINTER(spr);

    spr->setPosition(Vector3f(winWidth / 8, winHeight / 8, 0.5));
    spr->setSize(64 * 10, 64 * 10);
    spr->setTexturesState(state);

    spr->setTexCoords(0, 0, 0);
    spr->setTexCoords(1, 10, 0);
    spr->setTexCoords(2, 10, 10);
    spr->setTexCoords(3, 0, 10);

    spr->setAlphaTestState(AlphaTestState(true, GREATER, 0));


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
                        handleKey( ((KeyEvent *) evt)->getKeyCode() );
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

void handleKey(int keyCode) {
    switch (keyCode) {
        case '+':
            xd += 0.1f;
            break;
        case '-':
            xd -= 0.1f;
            break;
        default:
            break;
    }

    spr->setTexCoords(0, 0, 0);
    spr->setTexCoords(1, 1 + xd, 0);
    spr->setTexCoords(2, 1 + xd, 1 + xd);
    spr->setTexCoords(3, 0, 1 + xd);

}
