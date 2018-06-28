#include <QDebug>
#include <QSettings>
#include <QWindow>
#include "mainwindow.h"

MainWindow::MainWindow(QObject *parent) : QObject(parent)
{
    mParent = parent;
    mWebView = mParent->findChild<QObject *>("webView");
}
