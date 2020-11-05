//
//
#ifndef VOLCANO_APPLICATIONBASE_HPP
#define VOLCANO_APPLICATIONBASE_HPP

#include <QObject>
#include <QStringList>
#include <QCommandLineParser>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class ApplicationBase: public QObject
{
    Q_OBJECT

public:
    ApplicationBase(QObject *parent = nullptr);
    ~ApplicationBase(void) override;

public:
    virtual bool init(const QStringList &args) = 0;
    virtual int exec(void) = 0;
};

VOLCANO_END

#endif // VOLCANO_APPLICATIONBASE_HPP
