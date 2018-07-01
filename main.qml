import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.impl 2.4
import QtQuick.Layouts 1.3
import Qt.labs.settings 1.0
import Qt.labs.platform 1.0
import QtWebEngine 1.7
import QtWebChannel 1.0
import org.nemomobile.mpris 1.0
import se.aqba.qt.youtubemusic 1.0
import "qrc:/components"

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1024
    height: 768
    x: 0
    y: 0
    visibility: ApplicationWindow.AutomaticVisibility
    title: qsTr("Youtube Music")

    onVisibilityChanged: {
        if(visibility === ApplicationWindow.FullScreen)  {
            menuBar.visible = false
        } else {
            menuBar.visible = true
        }
    }

    onClosing: {
        // The tray icon causes the program to continue running, so hide it if we don't want that to happen.
        if(!trayIcon.shouldShow || !trayIcon.closeToTray) {
            trayIcon.hide()
        }
    }

    menuBar: ThemedMenuBar {
        ThemedMenu {
            title: qsTr("&File")

            ThemedMenu {
                title: qsTr("Theme")

                Component.onCompleted: {
                    while(this.count > 0) {
                        this.removeItem(this.itemAt(0))
                    }

                    var themes = themeManager.getThemes()
                    for(var key in themes) {
                        var theme = themes[key]

                        var actionComponent = Qt.createComponent("components/ThemedAction.qml")
                        var action = actionComponent.createObject(this, {name: theme.name, displayName: theme.displayName})

                        this.addAction(action)
                    }
                }
            }

            ThemedMenuSeparator { }

            Action {
                text: qsTr("&Quit")
                onTriggered: {
                    Qt.quit()
                }
            }
        }

        ThemedMenu {
            id: userMenu
            title: qsTr("User")
            visibility: false

            Action {
                text: qsTr("History")
                onTriggered: webIntegration.gotoPage("history")
            }

            Action {
                text: qsTr("Settings")
                onTriggered: webIntegration.gotoPage("settings")
            }

            Action {
                text: qsTr("Help")
                onTriggered: webIntegration.gotoPage("help")
            }

            Action {
                text: qsTr("Feedback")
                onTriggered: webIntegration.gotoPage("feedback")
            }

            ThemedMenuSeparator { }

            Action {
                text: qsTr("Sign out")
                onTriggered: webIntegration.gotoPage("signout")
            }
        }

        ThemedMenu {
            title: qsTr("&Tools")

            ThemedMenu {
                title: qsTr("Settings")

                Action {
                    text: qsTr("Tray icon")
                    onTriggered: trayIcon.shouldShow = !trayIcon.shouldShow
                }

                Action {
                    text: qsTr("Close to tray")
                    enabled: trayIcon.shouldShow
                    onTriggered: trayIcon.closeToTray = !trayIcon.closeToTray
                }
            }
        }
    }

    WebEngineView {
        id: webView
        objectName: "webView"
        anchors.fill: parent
        url: "https://music.youtube.com/"
        webChannel: WebChannel {
            registeredObjects: [
                webIntegration
            ]
        }
        userScripts: [
            WebEngineScript {
                injectionPoint: WebEngineScript.DocumentReady
                worldId: WebEngineScript.MainWorld
                name: "Youtube"
                sourceUrl: "qrc:/web/youtube.js"
            }
        ]

        onFullScreenRequested: function(request) {
            if (request.toggleOn)
                mainWindow.showFullScreen()
            else
                mainWindow.showNormal()

            request.accept()
        }
    }

    WebEngineViewFix {
        anchors.fill: webView
        onButtonChanged: {
            if(button === WebEngineViewFix.ForwardButton) {
                webView.goForward()
            } else if(button === WebEngineViewFix.BackButton) {
                webView.goBack()
            }
        }
    }

    MprisPlayer {
        id: mprisPlayer

        property var songInfo

        serviceName: "youtubemusic"

        // Mpris2 Root Interface
        identity: "Youtube Music"

        // Mpris2 Player Interface
        canControl: true

        canGoNext: true
        canGoPrevious: true
        canPause: playbackStatus === Mpris.Playing
        canPlay: playbackStatus !== Mpris.Playing
        canSeek: true

        playbackStatus: Mpris.Stopped
        loopStatus: Mpris.None
        shuffle: false
        volume: 1

        onPauseRequested: webIntegration.pause()
        onPlayRequested: webIntegration.play()
        onPlayPauseRequested: webIntegration.playPause()
        onStopRequested: webIntegration.stop()
        onNextRequested: webIntegration.next()
        onPreviousRequested: webIntegration.previous()
        onSeekRequested: webIntegration.seek(offset / 1000 / 1000)
        onSetPositionRequested: webIntegration.setPosition(position / 1000 / 1000)
        onLoopStatusRequested: {
            var loop = Mpris.None;
            if(loopStatus === Mpris.Playlist) {
                loop = 1;
            } else if(loopStatus === Mpris.Track) {
                loop = 2;
            }

            webIntegration.loopStatus(loop);
        }
        onShuffleRequested: webIntegration.shuffle()
        onSongInfoChanged: {
            var metadata = mprisPlayer.metadata
            metadata[Mpris.metadataToString(Mpris.TrackId)] = "/org/mpris/MediaPlayer2/Track/" + songInfo.trackId
            metadata[Mpris.metadataToString(Mpris.Title)] = songInfo.title
            metadata[Mpris.metadataToString(Mpris.Artist)] = songInfo.artist
            metadata[Mpris.metadataToString(Mpris.Album)] = songInfo.album
            metadata[Mpris.metadataToString(Mpris.ArtUrl)] = songInfo.albumArt
            metadata[Mpris.metadataToString(Mpris.Length)] = songInfo.length * 1000 * 1000
            mprisPlayer.metadata = metadata
        }
    }

    QtObject {
        id: webIntegration
        WebChannel.id: "webIntegration"

        signal themeChanged(var data)

        signal gotoPage(var page)

        signal pause()
        signal play()
        signal pausePlay()
        signal stop()
        signal next()
        signal previous()
        signal seek(var offset)
        signal setPosition(var position)
        signal loopStatus(var loop)
        signal shuffle()

        function songChanged(songInfo) {
            mprisPlayer.songInfo = songInfo
        }

        function songPositionChanged(songPosition) {
            mprisPlayer.setPosition(songPosition * 1000 * 1000)
        }

        function songStateChanged(songState) {
            if(songState.playing === 1) {
                mprisPlayer.playbackStatus = Mpris.Playing;
            } else if(songState.playing === 2) {
                mprisPlayer.playbackStatus = Mpris.Paused;
            } else {
                mprisPlayer.playbackStatus = Mpris.Stopped;
            }

            mprisPlayer.loopStatus = Mpris.None;
            if(songState.repeat === 1) {
                mprisPlayer.loopStatus = Mpris.Track;
            } else if(songState.repeat === 2) {
                mprisPlayer.loopStatus = Mpris.Playlist;
            }
        }

        function volumeChanged(volume) {
            mprisPlayer.setVolume(volume)
        }

        function userChanged(user) {
            userMenu.title = user.name
            userMenu.visibility = true
        }

        function loadTheme() {
            themeManager.onThemeChanged()
        }
    }

    ThemeManager {
        id: themeManager
        theme: ""

        onThemeChanged: {
            webIntegration.themeChanged(this.themeData())
            mainWindow.requestUpdate()
        }
    }

    SystemTrayIcon {
        property bool closeToTray: true
        property bool shouldShow: true

        id: trayIcon
        visible: shouldShow
        iconName: "youtubemusic"

        menu: Menu {
            MenuItem {
                text: qsTr("&Show")
                onTriggered: trayIcon.activated(SystemTrayIcon.Trigger)
            }

            MenuItem {
                text: qsTr("&Quit")
                onTriggered: Qt.quit()
            }
        }

        onActivated: {
            if(reason !== SystemTrayIcon.Context) {
                mainWindow.show()
                mainWindow.raise()
                mainWindow.requestActivate()
            }
        }

        Component.onCompleted: {
            if(shouldShow) {
                this.show()
            } else {
                this.hide()
            }
        }
    }

    Settings {
        id: windowState
        property alias x: mainWindow.x
        property alias y: mainWindow.y
        property alias width: mainWindow.width
        property alias height: mainWindow.height
        property alias visibility: mainWindow.visibility
    }

    Settings {
        id: mainSettings
        property alias theme: themeManager.theme
        property alias trayIconShow: trayIcon.shouldShow
        property alias trayIconClose: trayIcon.closeToTray
    }

    Settings {
        id: webSettings
        property alias url: webView.url
    }
}
