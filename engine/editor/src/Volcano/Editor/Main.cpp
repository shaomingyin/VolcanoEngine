//
//
#include <mutex>
#include <memory>

#include <spdlog/sinks/base_sink.h>

#include <QScopedPointer>
#include <QApplication>

#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/MainWindow.hpp>

VOLCANO_EDITOR_BEGIN

template<typename Mutex>
class QSpdlogSink : public spdlog::sinks::base_sink<Mutex>
{
protected:
	void sink_it_(const spdlog::details::log_msg &msg) override
	{
		spdlog::memory_buf_t formatted;
		spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
		std::string str = fmt::to_string(formatted);

		switch (msg.level) {
		case spdlog::level::err:
			qCritical("%s", str.c_str());
			break;
		case spdlog::level::warn:
			qWarning("%s", str.c_str());
			break;
		case spdlog::level::info:
			qInfo("%s", str.c_str());
			break;
		case spdlog::level::debug:
			qDebug("%s", str.c_str());
			break;
		default:
			qInfo("%s", str.c_str());
			break;
		}
	}

	void flush_(void) override
	{
	}
};

int main(int argc, char *argv[])
{
	auto sink = std::make_shared<QSpdlogSink<std::mutex>>();
	auto logger = std::make_shared<spdlog::logger>("Editor", sink);
	makeLogger(logger);
	spdlog::set_default_logger(logger);

	QApplication app(argc, argv);

	spdlog::info("Create main window...");
	QScopedPointer<MainWindow> mainWindow(new MainWindow);
	if (!mainWindow || !mainWindow->init()) {
		spdlog::error("Failed to create main window.");
		return EXIT_FAILURE;
	}

	mainWindow->show();

	return app.exec();
}

VOLCANO_EDITOR_END

int main(int argc, char *argv[])
{
	return Volcano::Editor::main(argc, argv);
}
