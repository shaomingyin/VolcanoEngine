//
//
#ifndef VOLCANO_SYSTEM_LOOPBACK_HPP
#define VOLCANO_SYSTEM_LOOPBACK_HPP

#include <map>

#include <QString>
#include <QIODevice>
#include <QBuffer>
#include <QMutex>
#include <QSharedPointer>
#include <QObject>

#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class Loopback;
using LoopbackPtr = QSharedPointer<Loopback>;

class Loopback: public QObject {
    Q_OBJECT

private:
    Loopback(const QString &name, QObject *parent = nullptr);
    ~Loopback(void) override;

public:
    static LoopbackPtr create(const QString &name);
    static void destory(const QString &name);
    static LoopbackPtr get(const QString &name);
    const QString &name(void) const;
    QIODevice &txChannel(void);
    QIODevice &rxChannel(void);

private:
    bool init(void);

private:
    using LoopbackPtrMap = std::map<QString, LoopbackPtr>;

    static QMutex c_mutex;
    static LoopbackPtrMap c_loopbackPtrMap;

    QString m_name;
    QBuffer m_txChannel;
    QBuffer m_rxChannel;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_LOOPBACK_HPP
