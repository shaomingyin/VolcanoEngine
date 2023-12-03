//
//
#ifndef VOLCANO_GAME_OBJECT_HPP
#define VOLCANO_GAME_OBJECT_HPP

#include <QVector3D>
#include <QQuaternion>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Context.hpp>

VOLCANO_GAME_BEGIN

class Object: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enableChanged)

public:
    Object(QObject* parent = nullptr);
    Object(Context& context, QObject* parent = nullptr);
    ~Object(void) override = default;

public:
    bool isEnabled(void) const {
        return is_enabled_;
    }

    void setEnabled(bool v) {
        if (is_enabled_ != v) {
            is_enabled_ = v;
            emit enableChanged(v);
        }
    }

    Context* context();

    const Context* context() const {
        return context_;
    }

    void draw() const {
        if (is_enabled_) {
            buildView();
        }
    }

    void tick(Duration elapsed) const {
        if (is_enabled_) {
            updateState(elapsed);
        }
    }

protected:
    virtual void buildView() const {}
    virtual void updateState(Duration elapsed) const {}

signals:
    void enableChanged(bool v);

private:
    bool is_enabled_;
    Context* context_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_OBJECT_HPP
