//
//
#ifndef VOLCANO_MODEL_HPP
#define VOLCANO_MODEL_HPP

#include <QVector>

#include <Volcano/Common.hpp>
#include <Volcano/Resource.hpp>
#include <Volcano/Component.hpp>

VOLCANO_BEGIN

class VOLCANO_API Model: public Component
{
    Q_OBJECT

public:
    Model(QObject *parent = nullptr);
    ~Model(void) override;

private:
    void release(void);

private:
};

VOLCANO_END

#endif // VOLCANO_MODEL_HPP
