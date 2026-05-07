#include "mainwindow.h"
#include "gamewidget.h"
#include "mazeeditor.h"
#include "mapselect.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), centralWidget(nullptr), layout(nullptr)
{
    setWindowTitle("Mazeman");
    setFixedSize(700, 700);
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    showMainMenu();
}

void MainWindow::showMainMenu()
{
    // 清除旧内容
    if (centralWidget) {
        delete centralWidget;
        centralWidget = nullptr;
    }

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    layout = new QVBoxLayout(centralWidget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    // 标题
    titleLabel = new QLabel("Mazeman", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 80px;"
        "font-weight: bold;"
        "color: #333;"
        "margin-bottom: 50px;"
        );

    // 按钮样式
    QString buttonStyle =
        "QPushButton {"
        "  font-size: 24px;"
        "  padding: 15px 60px;"
        "  background-color: #4a90d9;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #357abd;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #2a5f9e;"
        "}";

    // Start按钮
    startButton = new QPushButton("Start", this);
    startButton->setStyleSheet(buttonStyle);

    // Map按钮
    mapButton = new QPushButton("Map", this);
    mapButton->setStyleSheet(buttonStyle);

    // Exit按钮
    exitButton = new QPushButton("Exit", this);
    exitButton->setStyleSheet(
        "QPushButton {"
        "  font-size: 24px;"
        "  padding: 15px 60px;"
        "  background-color: #e74c3c;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #c0392b;"
        "}"
        );

    layout->addWidget(titleLabel);
    layout->addWidget(startButton);
    layout->addWidget(mapButton);
    layout->addWidget(exitButton);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(mapButton, &QPushButton::clicked, this, &MainWindow::onMapClicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::onExitClicked);
}

void MainWindow::onStartClicked()
{
    MapSelect *mapSelect = new MapSelect(this);
    mapSelect->exec();

    if (mapSelect->selectedMap() != "") {
        GameWidget *game = new GameWidget(mapSelect->selectedMap(), this);
        game->exec();
        showMainMenu(); // 游戏结束后返回主菜单
    }

    delete mapSelect;
}

void MainWindow::onMapClicked()
{
    MazeEditor *editor = new MazeEditor(this);
    editor->exec();
    showMainMenu(); // 编辑完成后返回主菜单
    delete editor;
}

void MainWindow::onExitClicked()
{
    close();
}