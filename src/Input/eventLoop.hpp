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
    static Base::GameData* mGameData;
};

Base::GameData* EventLoop::mGameData;


}}

#endif // !POPHEAD_INPUT_EVENTLOOP_H_
