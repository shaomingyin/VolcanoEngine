//
//
#ifndef VOLCANO_EDITOR_PROPERTYVIEW_HPP
#define VOLCANO_EDITOR_PROPERTYVIEW_HPP

#include <QTableWidget>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class PropertyView: public QTableWidget {
    Q_OBJECT

public:
    PropertyView(QWidget *parent = nullptr);
    ~PropertyView(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROPERTYVIEW_HPP
