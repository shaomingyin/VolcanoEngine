//
//
#ifndef VOLCANO_EDITOR_RESOURCEVIEW_HPP
#define VOLCANO_EDITOR_RESOURCEVIEW_HPP

#include <QColumnView>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class ResourceView: public QColumnView {
    Q_OBJECT

public:
    ResourceView(QWidget *parent = nullptr);
    ~ResourceView(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_RESOURCEVIEW_HPP
