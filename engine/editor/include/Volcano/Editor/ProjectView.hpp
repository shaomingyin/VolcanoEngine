//
//
#ifndef VOLCANO_EDITOR_PROJECTVIEW_HPP
#define VOLCANO_EDITOR_PROJECTVIEW_HPP

#include <QListView>
#include <QFileSystemModel>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class ProjectView: public QListView {
	Q_OBJECT

public:
	ProjectView(QWidget *parent = nullptr);
	~ProjectView(void) override;

public:
	bool init(void);

private:
	QFileSystemModel m_model;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECTVIEW_HPP
