//
//
#ifndef VOLCANO_LOOPER_HPP
#define VOLCANO_LOOPER_HPP

#include <thread>

#include <Volcano/Looper.h>
#include <Volcano/Common.hpp>

#define VOLCANO_LOOPER_BEGIN VOLCANO_BEGIN namespace Looper {
#define VOLCANO_LOOPER_END } VOLCANO_END

VOLCANO_LOOPER_BEGIN

using Context = VolcanoLooperContext;

using Handle = VolcanoLooperHandle;

using PrepareCallback = VolcanoLooperPrepareCallback;
using Prepare = VolcanoLooperPrepare;

using TimerCallback = VolcanoLooperTimerCallback;
using Timer = VolcanoLooperTimer;

using FrameCallback = VolcanoLooperFrameCallback;
using Frame = VolcanoLooperFrame;

using AsyncCallback = VolcanoLooperAsyncCallback;
using Async = VolcanoLooperAsync;

VOLCANO_INLINE static int init(Context *context)
{
    return volcanoLooperInit(context);
}

VOLCANO_INLINE static void destroy(Context *context)
{
    volcanoLooperDestroy(context);
}

VOLCANO_INLINE static void run(Context *context)
{
    volcanoLooperRun(context);
}

VOLCANO_INLINE static void stop(Context *context)
{
    volcanoLooperStop(context);
}

VOLCANO_INLINE static void kick(Context *context)
{
    volcanoLooperKick(context);
}

VOLCANO_INLINE static void handleClose(Handle *handle)
{
    volcanoLooperHandleClose(handle);
}

VOLCANO_INLINE static void prepareInit(Context *context, Prepare *handle, PrepareCallback cb)
{
    volcanoLooperPrepareInit(context, handle, cb);
}

VOLCANO_INLINE static void timerInit(Context *context, Timer *handle)
{
    volcanoLooperTimerInit(context, handle);
}

VOLCANO_INLINE static void timerStart(Timer *handle, TimerCallback cb, Uint32 timeout, Uint32 repeat)
{
    volcanoLooperTimerStart(handle, cb, timeout, repeat);
}

VOLCANO_INLINE static void timerStop(Timer *handle)
{
    volcanoLooperTimerStop(handle);
}

VOLCANO_INLINE static void frameInit(Context *context, Frame *handle)
{
    volcanoLooperFrameInit(context, handle);
}

VOLCANO_INLINE static void frameStart(Frame *handle, FrameCallback cb, int fps)
{
    volcanoLooperFrameStart(handle, cb, fps);
}

VOLCANO_INLINE static void frameStop(Frame *handle)
{
    volcanoLooperFrameStop(handle);
}

VOLCANO_INLINE static void asyncInit(Context *context, Async *handle, AsyncCallback cb)
{
    volcanoLooperAsyncInit(context, handle, cb);
}

VOLCANO_INLINE static void asyncSend(Async *handle)
{
    volcanoLooperAsyncSend(handle);
}

Context *defaultContext(void);
void kickThread(std::thread::id threadId);

VOLCANO_LOOPER_END

#endif // VOLCANO_LOOPER_HPP
