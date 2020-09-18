//
//
#ifndef VOLCANO_PICTURE_HPP
#define VOLCANO_PICTURE_HPP

#include <QObject>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API Picture: public QObject
{
    Q_OBJECT
public:
    Picture(QObject *parent = nullptr);
    ~Picture(void) override;
};

VOLCANO_END

#endif // VOLCANO_PICTURE_HPP
