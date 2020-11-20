/*
 *
 */
#ifndef VOLCANO_LOOPER_H
#define VOLCANO_LOOPER_H

#include <Volcano/Common.h>
#include <Volcano/List.h>

VOLCANO_BEGIN_C_DECLS

/* Context */

#define VOLCANO_LOOPER_RUNNING 0x1
#define VOLCANO_LOOPER_KICKED 0x2

typedef struct {
    SDL_threadID threadId;
    int flags;
    SDL_mutex *flagsMutex;
    SDL_cond *flagsCond;
    Uint32 now;
    VolcanoList handles;
    VolcanoList handlesAdded;
    VolcanoList handlesRemoved;
} VolcanoLooperContext;

int volcanoLooperInit(VolcanoLooperContext *context);
void volcanoLooperDestroy(VolcanoLooperContext *context);
void volcanoLooperRun(VolcanoLooperContext *context);
void volcanoLooperStop(VolcanoLooperContext *context);
void volcanoLooperKick(VolcanoLooperContext *context);

/* Handle */

typedef struct VolcanoLooperHandleStruct VolcanoLooperHandle;
typedef Uint32 (*VolcanoLooperHandleUpdate)(VolcanoLooperHandle *handle);

struct VolcanoLooperHandleStruct {
    VolcanoListNode node;
    VolcanoListNode nodeAdded;
    VolcanoListNode nodeRemoved;
    VolcanoListNode nodePending;
    VolcanoLooperContext *context;
    VolcanoLooperHandleUpdate update;
    void *userData;
};

void volcanoLooperHandleClose(VolcanoLooperHandle *handle);

/* Prepare */

typedef struct VolcanoLooperPrepareStruct VolcanoLooperPrepare;
typedef void (*VolcanoLooperPrepareCallback)(VolcanoLooperPrepare *handle);

struct VolcanoLooperPrepareStruct {
    VolcanoLooperHandle base;
    VolcanoLooperPrepareCallback cb;
};

void volcanoLooperPrepareInit(VolcanoLooperContext *context, VolcanoLooperPrepare *handle, VolcanoLooperPrepareCallback cb);

/* Timer */

typedef struct VolcanoLooperTimerStruct VolcanoLooperTimer;
typedef void (*VolcanoLooperTimerCallback)(VolcanoLooperTimer *handle);

struct VolcanoLooperTimerStruct {
    VolcanoLooperHandle base;
    VolcanoLooperTimerCallback cb;
    Uint32 expire;
    Uint32 repeat;
};

void volcanoLooperTimerInit(VolcanoLooperContext *context, VolcanoLooperTimer *handle);
void volcanoLooperTimerStart(VolcanoLooperTimer *handle, VolcanoLooperTimerCallback cb, Uint32 timeout, Uint32 repeat);
void volcanoLooperTimerStop(VolcanoLooperTimer *handle);

/* Frame */

typedef struct VolcanoLooperFrameStruct VolcanoLooperFrame;
typedef void (*VolcanoLooperFrameCallback)(VolcanoLooperFrame *handle);

struct VolcanoLooperFrameStruct {
    VolcanoLooperHandle base;
    VolcanoLooperFrameCallback cb;
    int fps;
};

void volcanoLooperFrameInit(VolcanoLooperContext *context, VolcanoLooperFrame *handle);
void volcanoLooperFrameStart(VolcanoLooperFrame *handle, VolcanoLooperFrameCallback cb, int fps);
void volcanoLooperFrameStop(VolcanoLooperFrame *handle);

/* Async */

typedef enum {
    VOLCANO_LOOPER_ASYNC_IDLE = 0,
    VOLCANO_LOOPER_ASYNC_ACTIVITED,
    VOLCANO_LOOPER_ASYNC_PENDING
} VolcanoLooperAsyncState;

typedef struct VolcanoLooperAsyncStruct VolcanoLooperAsync;
typedef void (*VolcanoLooperAsyncCallback)(VolcanoLooperAsync *handle);

struct VolcanoLooperAsyncStruct {
    VolcanoLooperHandle base;
    VolcanoLooperAsyncCallback cb;
    SDL_atomic_t state;
};

void volcanoLooperAsyncInit(VolcanoLooperContext *context, VolcanoLooperAsync *handle, VolcanoLooperAsyncCallback cb);
void volcanoLooperAsyncSend(VolcanoLooperAsync *handle);

VOLCANO_END_C_DECLS

#endif /* VOLCANO_LOOPER_H */
