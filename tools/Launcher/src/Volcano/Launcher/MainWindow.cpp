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

void MainWindow::keyPressEvent(QKeyEvent *p) {
    QQuickView::keyPressEvent(p);
}

void MainWindow::keyReleaseEvent(QKeyEvent *p) {
    QQuickView::keyReleaseEvent(p);
}

void MainWindow::mouseMoveEvent(QMouseEvent *p) {
    QQuickView::mouseMoveEvent(p);
}

void MainWindow::mousePressEvent(QMouseEvent *p) {
    QQuickView::mousePressEvent(p);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *p) {
    QQuickView::mouseReleaseEvent(p);
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

    QQuickView::timerEvent(p);
}

void MainWindow::frame(Duration elapsed) {
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

void MainWindow::idleFrame(Duration elapsed) {
}

void MainWindow::loadingFrame(Duration elapsed) {

}

void MainWindow::readyFrame(Duration elapsed) {

}

void MainWindow::playingFrame(Duration elapsed) {

}

void MainWindow::pausedFrame(Duration elapsed) {

}

void MainWindow::stoppingFrame(Duration elapsed) {

}

void MainWindow::errorFrame(Duration elapsed) {

}

VOLCANO_LAUNCHER_END
