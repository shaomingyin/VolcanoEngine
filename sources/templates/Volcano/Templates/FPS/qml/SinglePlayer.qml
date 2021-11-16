
import QtQuick
import QtQuick.Controls
import Volcnao.Graphics
import Volcnao.System

Window {
    property ClientWorld clientWorld
    property Server server: Server {
        gameWorld: gameWorldLoader.item
    }

    Loader {
        id: gameWorldLoader
        source: "vfs://World.qml"
    }

    ProgressBar {
        id: loadingBar
        visible: gameWorldLoader.status == Loader.Loading
        position: gameWorldLoader.progress
    }

    CameraView {
        visible: !loadingBar.visible
        anchors.fill: parent
        background: false

        world: clientWorld {
            server: server
        }
    }
}
