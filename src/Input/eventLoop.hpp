#ifndef POPHEAD_INPUT_EVENTLOOP_H_
#define POPHEAD_INPUT_EVENTLOOP_H_

namespace PopHead{
    namespace Base{ class GameData; }
namespace Input{


class EventLoop
{
public:
    static void eventLoop();

    static void setGameData(Base::GameData* gameData){mGameData = gameData;}

private:
    static void clear();

private:
    static Base::GameData* mGameData;

    static bool isKeyPressed;
    static bool isKeyReleased;
    static bool isMouseButtonPressed;
    static bool isMouseButtonReleased;
    static bool hasMouseMoved;
};

Base::GameData* EventLoop::mGameData;
bool EventLoop::isKeyPressed;
bool EventLoop::isKeyReleased;
bool EventLoop::isMouseButtonPressed;
bool EventLoop::isMouseButtonReleased;
bool EventLoop::hasMouseMoved;


}}

#endif // !POPHEAD_INPUT_EVENTLOOP_H_
