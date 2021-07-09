//
//
#ifndef VOLCANO_EDITOR_WORLDVIEW_HPP
#define VOLCANO_EDITOR_WORLDVIEW_HPP

#include <QTreeView>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class WorldView: public QTreeView {
    Q_OBJECT

public:
    WorldView(QWidget *parent = nullptr);
    ~WorldView(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_WORLDVIEW_HPP
