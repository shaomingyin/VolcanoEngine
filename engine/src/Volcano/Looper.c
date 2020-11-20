/*
 *
 */
#include <Volcano/Looper.h>

/* Context */

int volcanoLooperInit(VolcanoLooperContext *context)
{
    VOLCANO_ASSERT(context != NULL);

    context->flagsMutex = SDL_CreateMutex();
    if (context->flagsMutex == NULL)
        return -1;

    context->flagsCond = SDL_CreateCond();
    if (context->flagsCond == NULL) {
        SDL_DestroyMutex(context->flagsMutex);
        return -1;
    }

    context->flags = 0;
    context->now = SDL_GetTicks();
    context->threadId = SDL_ThreadID();

    volcanoListReset(&context->handles);
    volcanoListReset(&context->handlesAdded);
    volcanoListReset(&context->handlesRemoved);

    return 0;
}

void volcanoLooperDestroy(VolcanoLooperContext *context)
{
    VOLCANO_ASSERT(context != NULL);
    VOLCANO_ASSERT(SDL_ThreadID() == context->threadId);

    VOLCANO_ASSERT(volcanoListIsEmpty(&context->handles));
    VOLCANO_ASSERT(volcanoListIsEmpty(&context->handlesAdded));
    VOLCANO_ASSERT(volcanoListIsEmpty(&context->handlesRemoved));

    VOLCANO_ASSERT(context->flagsCond != NULL);
    SDL_DestroyCond(context->flagsCond);
    context->flagsCond = NULL;

    VOLCANO_ASSERT(context->flagsMutex != NULL);
    SDL_DestroyMutex(context->flagsMutex);
    context->flagsMutex = NULL;
}

VOLCANO_INLINE static Uint32 volcanoLooperUpdateHandles(VolcanoLooperContext *context)
{
    Uint32 result;
    Uint32 timeout = SDL_MAX_UINT32;
    VolcanoList handlesPending;
    VolcanoListNode *node;
    VolcanoLooperHandle *handle;

    VOLCANO_ASSERT(SDL_ThreadID() == context->threadId);

    while (!volcanoListIsEmpty(&context->handlesRemoved)) {
        node = volcanoListRemoveHead(&context->handlesRemoved);
        handle = VOLCANO_MEMBEROF(node, VolcanoLooperHandle, nodeRemoved);
        volcanoListNodeUnlink(&handle->node);
    }

    volcanoListReset(&handlesPending);

    VOLCANO_LIST_FOREACH(node, &context->handles) {
        handle = VOLCANO_MEMBEROF(node, VolcanoLooperHandle, node);
        volcanoListAppend(&handlesPending, &handle->nodePending);
    }

    while (!volcanoListIsEmpty(&context->handlesAdded)) {
        node = volcanoListRemoveHead(&context->handlesAdded);
        handle = VOLCANO_MEMBEROF(node, VolcanoLooperHandle, nodeAdded);
        volcanoListAppend(&context->handles, &handle->node);
        volcanoListAppend(&handlesPending, &handle->nodePending);
    }

    while (!volcanoListIsEmpty(&handlesPending)) {
        node = volcanoListRemoveHead(&handlesPending);
        handle = VOLCANO_MEMBEROF(node, VolcanoLooperHandle, nodePending);
        if (!volcanoListNodeIsLinked(&handle->nodeRemoved)) {
            result = handle->update(handle);
            if (result < timeout)
                timeout = result;
        }
        while (!volcanoListIsEmpty(&context->handlesAdded)) {
            node = volcanoListRemoveHead(&context->handlesAdded);
            handle = VOLCANO_MEMBEROF(node, VolcanoLooperHandle, nodeAdded);
            volcanoListAppend(&context->handles, &handle->node);
            volcanoListAppend(&handlesPending, &handle->nodePending);
        }
    }

    while (!volcanoListIsEmpty(&context->handlesRemoved)) {
        node = volcanoListRemoveHead(&context->handlesRemoved);
        handle = VOLCANO_MEMBEROF(node, VolcanoLooperHandle, nodeRemoved);
        volcanoListNodeUnlink(&handle->node);
    }

    return timeout;
}

void volcanoLooperRun(VolcanoLooperContext *context)
{
    VOLCANO_ASSERT(context != NULL);
    VOLCANO_ASSERT(SDL_ThreadID() == context->threadId);
    VOLCANO_ASSERT(!(context->flags & VOLCANO_LOOPER_RUNNING));
    VOLCANO_ASSERT(context->flagsCond != NULL);
    VOLCANO_ASSERT(context->flagsMutex != NULL);

    Uint32 result;
    Uint32 timeout = SDL_MAX_UINT32;

    context->flags |= VOLCANO_LOOPER_RUNNING;

    while (context->flags & VOLCANO_LOOPER_RUNNING) {
        context->now = SDL_GetTicks();
        result = volcanoLooperUpdateHandles(context);
        if (result < timeout)
            timeout = result;
        if (timeout < SDL_MAX_UINT32) {
            if (timeout > 0) {
                SDL_LockMutex(context->flagsMutex);
                if (context->flags & VOLCANO_LOOPER_KICKED)
                    context->flags &= ~VOLCANO_LOOPER_KICKED;
                else
                    SDL_CondWait(context->flagsCond, context->flagsMutex);
                SDL_UnlockMutex(context->flagsMutex);
            }
        } else {
            SDL_LockMutex(context->flagsMutex);
            if (context->flags & VOLCANO_LOOPER_KICKED)
                context->flags &= ~VOLCANO_LOOPER_KICKED;
            else
                SDL_CondWaitTimeout(context->flagsCond, context->flagsMutex, timeout);
            SDL_UnlockMutex(context->flagsMutex);
        }
    }
}

void volcanoLooperStop(VolcanoLooperContext *context)
{
    VOLCANO_ASSERT(context != NULL);
    VOLCANO_ASSERT(SDL_ThreadID() == context->threadId);
    VOLCANO_ASSERT(context->flagsMutex != NULL);

    SDL_LockMutex(context->flagsMutex);
    context->flags &= ~VOLCANO_LOOPER_RUNNING;
    SDL_UnlockMutex(context->flagsMutex);
}

void volcanoLooperKick(VolcanoLooperContext *context)
{
    VOLCANO_ASSERT(context != NULL);
    VOLCANO_ASSERT(context->flagsCond != NULL);
    VOLCANO_ASSERT(context->flagsMutex != NULL);
    VOLCANO_ASSERT(SDL_ThreadID() != context->threadId);

    SDL_LockMutex(context->flagsMutex);
    context->flags |= VOLCANO_LOOPER_KICKED;
    SDL_CondSignal(context->flagsCond);
    SDL_UnlockMutex(context->flagsMutex);
}

/* Handle */

VOLCANO_INLINE static void volcanoLooperHandleInit(VolcanoLooperContext *context, VolcanoLooperHandle *handle, VolcanoLooperHandleUpdate update)
{
    VOLCANO_ASSERT(SDL_ThreadID() == context->threadId);

    handle->context = context;
    handle->update = update;
    handle->userData = NULL;

    volcanoListNodeReset(&handle->node);
    volcanoListNodeReset(&handle->nodeAdded);
    volcanoListNodeReset(&handle->nodeRemoved);
    volcanoListNodeReset(&handle->nodePending);
}

void volcanoLooperHandleClose(VolcanoLooperHandle *handle)
{
    VOLCANO_ASSERT(handle != NULL);
    VOLCANO_ASSERT(SDL_ThreadID() == handle->context->threadId);

    volcanoListNodeUnlink(&handle->node);
    volcanoListNodeUnlink(&handle->nodeAdded);
    volcanoListNodeUnlink(&handle->nodeRemoved);
    volcanoListNodeUnlink(&handle->nodePending);
}

/* Prepare */

static Uint32 volcanoLooperPrepareUpdate(VolcanoLooperHandle *handle)
{
    VOLCANO_ASSERT(handle != NULL);

    VolcanoLooperPrepare *prepare = (VolcanoLooperPrepare *)handle;
    prepare->cb(prepare);

    return SDL_MAX_UINT32;
}

void volcanoLooperPrepareInit(VolcanoLooperContext *context, VolcanoLooperPrepare *handle, VolcanoLooperPrepareCallback cb)
{
    VOLCANO_ASSERT(context != NULL);
    VOLCANO_ASSERT(SDL_ThreadID() == context->threadId);
    VOLCANO_ASSERT(handle != NULL);
    VOLCANO_ASSERT(cb != NULL);

    volcanoLooperHandleInit(context, &handle->base, volcanoLooperPrepareUpdate);
    volcanoListAppend(&context->handlesAdded, &handle->base.nodeAdded);

    handle->cb = cb;
}

/* Timer */

static Uint32 volcanoLooperTimerUpdate(VolcanoLooperHandle *handle)
{
    VOLCANO_ASSERT(handle != NULL);

    VolcanoLooperTimer *timer = (VolcanoLooperTimer *)handle;

    Uint32 now = handle->context->now;
    if (now >= timer->expire) {
        timer->cb(timer);
        if (timer->repeat > 0) {
            timer->expire = now + timer->repeat;
            return timer->repeat;
        } else
            volcanoListAppend(&handle->context->handlesRemoved, &timer->base.nodeRemoved);
    }

    return SDL_MAX_UINT32;
}

void volcanoLooperTimerInit(VolcanoLooperContext *context, VolcanoLooperTimer *handle)
{
    VOLCANO_ASSERT(context != NULL);
    VOLCANO_ASSERT(SDL_ThreadID() == context->threadId);
    VOLCANO_ASSERT(handle != NULL);

    volcanoLooperHandleInit(context, &handle->base, volcanoLooperTimerUpdate);

    handle->expire = 0;
    handle->repeat = 0;
    handle->cb = NULL;
}

void volcanoLooperTimerStart(VolcanoLooperTimer *handle, VolcanoLooperTimerCallback cb, Uint32 timeout, Uint32 repeat)
{
    VOLCANO_ASSERT(handle != NULL);
    VOLCANO_ASSERT(SDL_ThreadID() == handle->base.context->threadId);
    VOLCANO_ASSERT(cb != NULL);
    VOLCANO_ASSERT(timeout >= 0);
    VOLCANO_ASSERT(repeat >= 0);

    handle->cb = cb;
    handle->expire = handle->base.context->now + timeout;
    handle->repeat = repeat;

    volcanoListAppend(&handle->base.context->handlesAdded, &handle->base.nodeAdded);
    volcanoListNodeUnlink(&handle->base.nodeRemoved);
}

void volcanoLooperTimerStop(VolcanoLooperTimer *handle)
{
    VOLCANO_ASSERT(handle != NULL);
    VOLCANO_ASSERT(SDL_ThreadID() == handle->base.context->threadId);

    volcanoListAppend(&handle->base.context->handlesRemoved, &handle->base.nodeRemoved);
    volcanoListNodeUnlink(&handle->base.nodeAdded);
}

/* Frame */

static Uint32 volcanoLooperFrameUpdate(VolcanoLooperHandle *handle)
{
    VOLCANO_ASSERT(handle != NULL);

    VolcanoLooperFrame *frame = (VolcanoLooperFrame *)handle;

    // TODO

    return SDL_MAX_UINT32;
}

void volcanoLooperFrameInit(VolcanoLooperContext *context, VolcanoLooperFrame *handle)
{
    VOLCANO_ASSERT(context != NULL);
    VOLCANO_ASSERT(SDL_ThreadID() == context->threadId);
    VOLCANO_ASSERT(handle != NULL);

    volcanoLooperHandleInit(context, &handle->base, volcanoLooperFrameUpdate);

    handle->cb = NULL;
    handle->fps = 0;
}

void volcanoLooperFrameStart(VolcanoLooperFrame *handle, VolcanoLooperFrameCallback cb, int fps)
{
    VOLCANO_ASSERT(handle != NULL);
    VOLCANO_ASSERT(cb != NULL);
    VOLCANO_ASSERT(fps > 0);
    VOLCANO_ASSERT(SDL_ThreadID() == handle->base.context->threadId);

    handle->cb = cb;
    handle->fps = fps;

    volcanoListAppend(&handle->base.context->handlesAdded, &handle->base.nodeAdded);
    volcanoListNodeUnlink(&handle->base.nodeRemoved);
}

void volcanoLooperFrameStop(VolcanoLooperFrame *handle)
{
    VOLCANO_ASSERT(handle != NULL);
    VOLCANO_ASSERT(SDL_ThreadID() == handle->base.context->threadId);

    volcanoListAppend(&handle->base.context->handlesRemoved, &handle->base.nodeRemoved);
    volcanoListNodeUnlink(&handle->base.nodeAdded);
}

/* Async */

static Uint32 volcanoLooperAsyncUpdate(VolcanoLooperHandle *handle)
{
    VOLCANO_ASSERT(handle != NULL);

    VolcanoLooperAsync *async = (VolcanoLooperAsync *)handle;

    if (SDL_AtomicCAS(&async->state, VOLCANO_LOOPER_ASYNC_ACTIVITED, VOLCANO_LOOPER_ASYNC_PENDING)) {
        async->cb(async);
        SDL_AtomicSet(&async->state, VOLCANO_LOOPER_ASYNC_IDLE);
    }

    return SDL_MAX_UINT32;
}

void volcanoLooperAsyncInit(VolcanoLooperContext *context, VolcanoLooperAsync *handle, VolcanoLooperAsyncCallback cb)
{
    VOLCANO_ASSERT(context != NULL);
    VOLCANO_ASSERT(SDL_ThreadID() == context->threadId);
    VOLCANO_ASSERT(handle != NULL);
    VOLCANO_ASSERT(cb != NULL);

    volcanoLooperHandleInit(context, &handle->base, volcanoLooperAsyncUpdate);
    SDL_AtomicSet(&handle->state, VOLCANO_LOOPER_ASYNC_IDLE);

    volcanoListAppend(&context->handlesAdded, &handle->base.nodeAdded);
}

void volcanoLooperAsyncSend(VolcanoLooperAsync *handle)
{
    VOLCANO_ASSERT(handle != NULL);
    VOLCANO_ASSERT(SDL_ThreadID() != handle->base.context->threadId);

    while (!SDL_AtomicCAS(&handle->state, VOLCANO_LOOPER_ASYNC_IDLE, VOLCANO_LOOPER_ASYNC_ACTIVITED));
    volcanoLooperKick(handle->base.context);
}
