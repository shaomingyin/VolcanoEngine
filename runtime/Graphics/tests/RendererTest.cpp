#include <array>
#include <memory>

#include <QFile>
#include <QGuiApplication>
#include <QOffscreenSurface>
#include <QOpenGLFunctions>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <QTemporaryDir>
#include <QThread>
#include <QVariant>

#include <gtest/gtest.h>

#include <Volcano/Graphics/Renderer.h>
#include <Volcano/World/Camera.h>
#include <Volcano/World/ColorEnvironment.h>
#include <Volcano/World/Light.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/Scene.h>
#include <Volcano/World/SkyboxEnvironment.h>

VOLCANO_GRAPHICS_BEGIN

TEST(GraphicsRenderer, BindsQmlModelsAndClearsAttachmentsOnRemoval) {
    int argc = 1;
    char name[] = "GraphicsTests";
    char* argv[] = {name, nullptr};
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")) {
        qputenv("QT_QPA_PLATFORM", "offscreen");
    }
    QGuiApplication application(argc, argv);
    World::Scene::registerQmlTypes();
    qmlRegisterType<QQuickItem>("Volcano.GraphicsTests", 1, 0, "Item");
    World::Scene unconfiguredScene;
    EXPECT_EQ(unconfiguredScene.environment(), nullptr);
    QQmlEngine engine;
    const QTemporaryDir directory;
    ASSERT_TRUE(directory.isValid());
    QFile sourceFile(directory.filePath(QStringLiteral("Hud.qml")));
    ASSERT_TRUE(sourceFile.open(QIODevice::WriteOnly));
    const QByteArray sourceQml = "import Volcano.GraphicsTests 1.0\nItem { objectName: \"externalHud\" }";
    ASSERT_EQ(sourceFile.write(sourceQml), sourceQml.size());
    sourceFile.close();
    QQmlComponent qml(&engine);
    qml.setData(R"(
        import Volcano.World 1.0
        import Volcano.GraphicsTests 1.0
        Scene {
            environment: SkyboxEnvironment {
                source: "qrc:/environments/courtyard.ktx2"
                ambientLight { strength: 0.25 }
            }
            Entity {
                Camera {
                    fov: 60
                    aspectRatio: 1.5
                    overlay: Item {
                        objectName: "cameraHud"
                        Item { objectName: "status" }
                    }
                }
                DirectionalLight {}
                PointLight {}
                SpotLight {}
                Model { source: "file:///initial.model" }
            }
        }
    )", QUrl::fromLocalFile(directory.filePath(QStringLiteral("Scene.qml"))));
    ASSERT_TRUE(qml.isReady()) << qml.errorString().toStdString();
    const std::unique_ptr<QObject> object(qml.create());
    auto* scene = qobject_cast<World::Scene*>(object.get());
    ASSERT_NE(scene, nullptr) << qml.errorString().toStdString();
    auto* skybox = qobject_cast<World::SkyboxEnvironment*>(scene->environment());
    ASSERT_NE(skybox, nullptr);
    EXPECT_EQ(skybox->source(), QUrl("qrc:/environments/courtyard.ktx2"));
    EXPECT_FLOAT_EQ(skybox->ambientLight()->strength(), 0.25f);
    EXPECT_EQ(skybox->ambientLight()->color(), QColor(Qt::white));
    {
        QQmlComponent colorQml(&engine);
        colorQml.setData("import Volcano.World 1.0\nColorEnvironment {}", QUrl());
        ASSERT_TRUE(colorQml.isReady()) << colorQml.errorString().toStdString();
        const std::unique_ptr<QObject> colorObject(colorQml.create());
        auto* color = qobject_cast<World::ColorEnvironment*>(colorObject.get());
        ASSERT_NE(color, nullptr);
        scene->setEnvironment(color);
        EXPECT_EQ(scene->environment(), color);
        EXPECT_EQ(color->color(), QColor(Qt::black));
        EXPECT_FLOAT_EQ(color->ambientLight()->strength(), 0.0f);
    }
    EXPECT_EQ(scene->environment(), nullptr);
    scene->setEnvironment(skybox);
    ASSERT_EQ(scene->entityCount(), 1);
    auto* entity = scene->entityAt(0);
    ASSERT_EQ(entity->componentCount(), 5);
    auto* camera = qobject_cast<World::Camera*>(entity->componentAt(0));
    ASSERT_NE(camera, nullptr);
    ASSERT_NE(camera->overlay(), nullptr);
    const std::unique_ptr<QObject> inlineOverlay(camera->overlay()->create());
    auto* overlay = qobject_cast<QQuickItem*>(inlineOverlay.get());
    ASSERT_NE(overlay, nullptr);
    EXPECT_EQ(overlay->objectName(), QStringLiteral("cameraHud"));
    ASSERT_EQ(overlay->childItems().size(), 1);
    EXPECT_EQ(overlay->childItems().first()->objectName(), QStringLiteral("status"));
    EXPECT_EQ(overlay->parentItem(), nullptr);
    {
        QQmlComponent replacement(&engine);
        camera->setOverlay(&replacement);
        EXPECT_EQ(camera->overlay(), &replacement);
    }
    EXPECT_EQ(camera->overlay(), nullptr);

    QQmlComponent externalCameraQml(&engine);
    externalCameraQml.setData(R"(
        import Volcano.World 1.0
        Camera { overlay: Qt.createComponent("Hud.qml") }
    )", QUrl::fromLocalFile(directory.filePath(QStringLiteral("ExternalCamera.qml"))));
    ASSERT_TRUE(externalCameraQml.isReady()) << externalCameraQml.errorString().toStdString();
    const std::unique_ptr<QObject> externalCameraObject(externalCameraQml.create());
    const auto* externalCamera = qobject_cast<World::Camera*>(externalCameraObject.get());
    ASSERT_NE(externalCamera, nullptr) << externalCameraQml.errorString().toStdString();
    ASSERT_NE(externalCamera->overlay(), nullptr);
    ASSERT_TRUE(externalCamera->overlay()->isReady()) << externalCamera->overlay()->errorString().toStdString();
    const std::unique_ptr<QObject> externalOverlay(externalCamera->overlay()->create());
    auto* externalItem = qobject_cast<QQuickItem*>(externalOverlay.get());
    ASSERT_NE(externalItem, nullptr);
    EXPECT_EQ(externalItem->objectName(), QStringLiteral("externalHud"));
    QMatrix4x4 projection;
    projection.perspective(60.0f, 1.5f, camera->nearPlane(), camera->farPlane());
    EXPECT_EQ(camera->projectionMatrix(), projection);
    for (qsizetype index = 1; index < 4; ++index) {
        EXPECT_NE(qobject_cast<World::Light*>(entity->componentAt(index)), nullptr);
    }
    auto* component = qobject_cast<World::Model*>(entity->componentAt(4));
    ASSERT_NE(component, nullptr);
    EXPECT_EQ(component->source(), QUrl("file:///initial.model"));

    QOpenGLContext context;
    if (!context.create()) {
        GTEST_SKIP() << "The Qt platform has no OpenGL context";
    }
    QOffscreenSurface surface;
    surface.setFormat(context.format());
    surface.create();
    if (!surface.isValid()) {
        GTEST_SKIP() << "The Qt platform has no usable offscreen OpenGL surface";
    }

    constexpr char binding[] = "volcano.graphics.model";
    auto renderer = std::make_unique<Renderer>(*scene);
    EXPECT_EQ(component->property(binding).value<QObject*>(), renderer.get());

    component->setSource(QUrl("file:///replacement.model"));
    entity->transform()->setTranslation(QVector3D(1, 2, 3));
    renderer->update(*camera, QRect(0, 0, 10, 10));
    renderer->update(*camera, QRect(3, 5, 64, 48));
    std::array<GLint, 4> renderedViewport{};
    bool madeCurrent = false;
    const std::unique_ptr<QThread> renderThread(QThread::create([&] {
        madeCurrent = context.makeCurrent(&surface);
        if (madeCurrent) {
            renderer->render();
            context.functions()->glGetIntegerv(GL_VIEWPORT, renderedViewport.data());
            renderer->releaseRenderResources();
            context.doneCurrent();
        }
        context.moveToThread(application.thread());
    }));
    context.moveToThread(renderThread.get());
    renderThread->start();
    renderThread->wait();
    ASSERT_TRUE(madeCurrent);
    EXPECT_EQ(renderedViewport, (std::array<GLint, 4>{3, 5, 64, 48}));

    scene->removeLastEntity();
    EXPECT_FALSE(component->property(binding).isValid());
    entity->removeLastComponent();
    entity->appendComponent(component);
    EXPECT_FALSE(component->property(binding).isValid());

    scene->appendEntity(entity);
    EXPECT_TRUE(component->property(binding).isValid());
    entity->removeLastComponent();
    EXPECT_FALSE(component->property(binding).isValid());
    entity->appendComponent(component);
    EXPECT_TRUE(component->property(binding).isValid());
    renderer.reset();
    EXPECT_FALSE(component->property(binding).isValid());
}

VOLCANO_GRAPHICS_END
