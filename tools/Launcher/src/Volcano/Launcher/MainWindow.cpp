//
//
#include <Volcano/Launcher/MainWindow.h>

VOLCANO_LAUNCHER_BEGIN

MainWindow::MainWindow()
    : state_(State::Idle)
    , frame_last_(Clock::now())
    , frame_count_(0)
    , frame_count_per_second_(0)
    , frame_timer_(startTimer(15, Qt::PreciseTimer))
    , frame_count_timer_(startTimer(1000, Qt::PreciseTimer))
    , game_world_(nullptr) {
}

void MainWindow::initializeGL() {
}

void MainWindow::paintGL() {
}

void MainWindow::paintOverGL() {
}

void MainWindow::paintUnderGL() {
}

void MainWindow::resizeGL(int w, int h) {
}

void MainWindow::keyPressEvent(QKeyEvent *p) {
    QOpenGLWindow::keyPressEvent(p);
}

void MainWindow::keyReleaseEvent(QKeyEvent *p) {
    QOpenGLWindow::keyReleaseEvent(p);
}

void MainWindow::mouseMoveEvent(QMouseEvent *p) {
    QOpenGLWindow::mouseMoveEvent(p);
}

void MainWindow::mousePressEvent(QMouseEvent *p) {
    QOpenGLWindow::mousePressEvent(p);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *p) {
    QOpenGLWindow::mouseReleaseEvent(p);
}

void MainWindow::timerEvent(QTimerEvent* p) {
    if (p->timerId() == frame_timer_) {
        auto curr = Clock::now();
        frame(curr - frame_last_);
        frame_last_ = curr;
        frame_count_ += 1;
        return;
    }

    if (p->timerId() == frame_count_timer_) {
        frame_count_per_second_ = frame_count_;
        frame_count_ = 0;
    }

    QOpenGLWindow::timerEvent(p);
}

void MainWindow::frame(Clock::duration elapsed) {
    switch (state_) {
    case State::Playing:
        playingFrame(elapsed);
        break;
    case State::Idle:
        idleFrame(elapsed);
        break;
    case State::Loading:
        loadingFrame(elapsed);
        break;
    case State::Ready:
        readyFrame(elapsed);
        break;
    case State::Paused:
        pausedFrame(elapsed);
        break;
    case State::Stopping:
        stoppingFrame(elapsed);
        break;
    case State::Error:
        errorFrame(elapsed);
        break;
    }
}

void MainWindow::idleFrame(Clock::duration elapsed) {
}

void MainWindow::loadingFrame(Clock::duration elapsed) {

}

void MainWindow::readyFrame(Clock::duration elapsed) {

}

void MainWindow::playingFrame(Clock::duration elapsed) {

}

void MainWindow::pausedFrame(Clock::duration elapsed) {

}

void MainWindow::stoppingFrame(Clock::duration elapsed) {

}

void MainWindow::errorFrame(Clock::duration elapsed) {

}

VOLCANO_LAUNCHER_END
