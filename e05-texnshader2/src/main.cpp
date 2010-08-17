
#include <windows/eeWindowSystemFactory.h>
#include <engines/eeGLEngine.h>
#include <cameras/eeOrthoCamera.h>
#include <interface/eeGUIBuilder.h>
#include <tools/eeTextureTool.h>
#include <tools/eeTimerFactory.h>

#include <iostream>
#include <algorithm>

using namespace EE;

bool handleKey(int);
void updateTexCoords(Sprite *);
void inc(float *, const char*);
void dec(float *, const char*);

Sprite *spr = NULL;
Program *prog = NULL;
GPUStateManager *smgr = NULL;

// Variable that is exposed to the fragment shader to
// control the interpolation between the 2 textures.
float interpol = 0.0;

float xd = 0.0;

bool shader = false;

int main(void) {
    int winHeight = 1024;
    int winWidth = 768;
    WindowSystem *winsys = WindowSystemFactory::createWindowSystem();

    Timer *timer = TimerFactory::createTimer();

    EE::Window *win = winsys->createWindow("enfent-05", 0, 0, winWidth, winHeight);
    winsys->show(win);

    GLEngine *engine = new GLEngine();
    engine->initialize(win);

    smgr = engine->getStateManager();

    smgr->setDepthTestState(DepthTestState(LEQUAL));
    smgr->setClearDepthValue(1.0);
    smgr->setClearColorValue(0.0, 0.0, 0.0, 0.0);

    Shader *pix = engine->createPixelShader();
    ASSERT(pix->load("data/tex.fsh", true, SCF_LOG_ERRORS));

    Shader *vex = engine->createVertexShader();
    ASSERT(vex->load("data/tex.vsh", true, SCF_LOG_ERRORS));

    prog = engine->createProgram();
    prog->setPixelShader(pix);
    prog->setVertexShader(vex);

    ASSERT(prog->link(SCF_LOG_ERRORS));

    OrthoCamera *cam2d = new OrthoCamera(engine, win->getClientViewport(), false, true);
    cam2d->apply();

    Sprite *bfly = GUIBuilder::createSpriteFromFile(engine, std::string("data/butterfly.png"));
    CHECK_POINTER(bfly);

    bfly->setPosition(Vector3f(winWidth / 4, winHeight / 4, 0.5));
    bfly->setSize(winWidth / 2, winHeight / 2);
    bfly->setAlphaTestState(AlphaTestState(true, GREATER, 0));

    Texture *tex1 = TextureTool::loadFromFile(engine, std::string("data/shingle64.png"), false);
    CHECK_POINTER(tex1);

    Texture *tex2 = TextureTool::loadFromFile(engine, std::string("data/water.png"), false);
    CHECK_POINTER(tex2);

    TextureSampler sampler = TextureSampler(NEAREST, LINEAR);
    sampler.sTexture = REPEAT;
    sampler.tTexture = REPEAT;

    TexturesState state = TexturesState(tex1, sampler);
    state.slots[1] = tex2;
    state.samplers[1] = state.samplers[0];

    spr = new Sprite(engine);
    CHECK_POINTER(spr);

    spr->setPosition(Vector3f(winWidth / 8, winHeight / 8, 0.5));
    spr->setSize(64 * 10, 64 * 10);
    spr->setTexturesState(state);

    updateTexCoords(spr);

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
                        if(handleKey( ((KeyEvent *) evt)->getKeyCode() ))
                            finish = true;
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

        // Setting the shader variables only works when shader program is active.
        smgr->setShadersState(prog);
        prog->setFloat("time", ((float) timer->getElapsedTime()) / 500);
        prog->setFloat("interpol", interpol);
        prog->setTexture("tex1", 0);
        prog->setTexture("tex2", 1);
        spr->render();
        smgr->setShadersState(NULL);

        bfly->render();
        win->swap();
    }

}

bool handleKey(int keyCode) {
    switch (keyCode) {
        case '+':
            xd += 0.1f;
            break;
        case '-':
            xd -= 0.1f;
            break;
        case 'e':
            inc(&interpol, "interpol");
            break;
        case 'd':
            dec(&interpol, "interpol");
            break;
        case 'q':
            return true;
        default:
            break;
    }

    updateTexCoords(spr);

    return false;
}

void updateTexCoords(Sprite *s)
{
    s->setTexCoords(0, 0, 0);
    s->setTexCoords(1, 1 + xd, 0);
    s->setTexCoords(2, 1 + xd, 1 + xd);
    s->setTexCoords(3, 0, 1 + xd);
}

void inc(float *f, const char*name)
{
    *f = std::min(*f + 0.1, 10.0);
    std::cerr << name << ": " << *f << "\n";
}

void dec(float *f, const char*name)
{
    *f = std::max(*f - 0.1, -10.0);
    std::cerr << name << ": " << *f << "\n";
}
