//
//
#ifndef VOLCANO_GAME_OBJECT_H
#define VOLCANO_GAME_OBJECT_H

#include <memory>

#include <QObject>
#include <QString>
#include <QQmlComponent>
#include <QQmlParserStatus>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Context.h>

VOLCANO_GAME_BEGIN

class Object: public QObject, public QQmlParserStatus {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_INTERFACES(QQmlParserStatus)

public:
    Object(QObject* parent = nullptr);

public:
    Context& context() noexcept;

    bool isEnabled() const {
        return enabled_;
    }

    void setEnabled(bool v) {
        if (enabled_ != v) {
            enabled_ = v;
            emit enabledChanged(v);
        }
    }

    void classBegin() override;
    void componentComplete() override;
    QNetworkAccessManager* networkAccessManager();
    static QString toQml(const QMetaObject& meta_object, const QString& url = QString());
    static std::unique_ptr<QQmlComponent> toQmlComponent(QQmlEngine& qml_engine, const QMetaObject& meta_object, const QString& url = QString());

signals:
    void enabledChanged(bool v);

private:
    Context* context_;
    bool enabled_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_OBJECT_H
