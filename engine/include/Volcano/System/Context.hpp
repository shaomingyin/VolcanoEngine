//
//
#ifndef VOLCANO_SYSTEM_CONTEXT_HPP
#define VOLCANO_SYSTEM_CONTEXT_HPP

#include <QObject>
#include <QDataStream>

#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class Context: public QObject {
    Q_OBJECT

public:
    Context(QObject *parent = nullptr);
    ~Context(void) override;

protected:
    void setIODevice(QIODevice *p);
    QDataStream &stream(void);

private:
    QDataStream m_stream;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CONTEXT_HPP
