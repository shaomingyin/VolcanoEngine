//
//
#ifndef VOLCANO_EDITOR_OUTPUTVIEW_HPP
#define VOLCANO_EDITOR_OUTPUTVIEW_HPP

#include <QTextEdit>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class OutputView: public QTextEdit {
    Q_OBJECT

public:
    OutputView(QWidget *parent = nullptr);
    ~OutputView(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_OUTPUTVIEW_HPP
