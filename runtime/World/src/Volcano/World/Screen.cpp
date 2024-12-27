//
//
#include <QUrl>
#include <QString>

#include <Volcano/World/Screen.h>

VOLCANO_WORLD_BEGIN

Screen::Screen(QObject* parent)
    : Transformable(parent) {
}

Screen::~Screen() {
    if (component_ != nullptr) {
        delete component_;
    }
}

void Screen::setSource(const QVariant& v) {
    if (source_ == v) {
        return;
    }

    if (component_ != nullptr) {
        delete component_;
    }

    if (item_ != nullptr) {
        delete item_;
    }

    if (v.canConvert<QUrl>()) {
        component_ = new QQmlComponent(qmlEngine(this), v.value<QUrl>(), QQmlComponent::Asynchronous);
    } else if (v.canConvert<QString>()) {
        component_ = new QQmlComponent(qmlEngine(this), v.value<QString>(), QQmlComponent::Asynchronous);
    } else if (v.canConvert<QQmlComponent*>()) {
        component_ = v.value<QQmlComponent*>();
    } else {
        component_ = nullptr;
    }

    if (component_ != nullptr) {
        if (component_->isReady()) {
            onComponentStatusChanged(QQmlComponent::Ready);
        } else {
            connect(component_, &QQmlComponent::statusChanged, this, &Screen::onComponentStatusChanged);
        }
    }

    emit sourceChanged(v);
}

void Screen::onComponentStatusChanged(QQmlComponent::Status st) {
    if (st == QQmlComponent::Ready) {
        auto object = component_->create();
        if (object != nullptr) {
            item_ = qobject_cast<QQuickItem*>(object);
        }
    }
}

VOLCANO_WORLD_END
