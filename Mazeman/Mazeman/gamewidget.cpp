#include "gamewidget.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>
#include <cmath>

GameWidget::GameWidget(const std::string &mapName, QWidget *parent)
    : QDialog(parent), gameRunning(true), gameWon(false)
{
    setWindowTitle("Mazeman - Playing");
    setFixedSize(700, 700);
    setModal(true);

    loadMap(mapName);

    // 设置玩家在左下角
    playerX = 1;
    playerY = mapHeight - 2;
    goalX = mapWidth - 2;
    goalY = 1;

    cellSize = 20;
    offsetX = 0;
    offsetY = 0;

    // 游戏循环
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameWidget::updateGame);
    gameTimer->start(50); // 20 FPS

    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

GameWidget::~GameWidget()
{
    gameTimer->stop();
}

void GameWidget::loadMap(const std::string &mapName)
{
    maze.clear();

    QFile file(QString::fromStdString(mapName + ".maze"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot load map file!");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        std::vector<int> row;
        for (QChar c : line) {
            if (c == '0') row.push_back(0);
            else if (c == '1') row.push_back(1);
        }
        if (!row.empty()) maze.push_back(row);
    }

    file.close();

    if (!maze.empty()) {
        mapHeight = maze.size();
        mapWidth = maze[0].size();
    }
}

void GameWidget::updateGame()
{
    update(); // 触发重绘
}

bool GameWidget::canMove(int x, int y)
{
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) return false;
    return maze[y][x] == 0;
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (!gameRunning) return;

    int newX = playerX;
    int newY = playerY;

    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        newY--;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        newY++;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        newX--;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        newX++;
        break;
    case Qt::Key_Escape:
        reject();
        return;
    default:
        return;
    }

    if (canMove(newX, newY)) {
        playerX = newX;
        playerY = newY;

        // 检查是否到达终点
        if (playerX == goalX && playerY == goalY) {
            gameRunning = false;
            gameWon = true;
            QMessageBox::information(this, "Congratulations!", "You escaped the maze!");
            accept();
            return;
        }
    }

    update();
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 计算视野范围（地图九分之一的大小）
    int viewWidth = mapWidth / 3;
    int viewHeight = mapHeight / 3;
    if (viewWidth < 5) viewWidth = 5;
    if (viewHeight < 5) viewHeight = 5;

    // 计算视野偏移，使玩家在视野中心
    offsetX = playerX - viewWidth / 2;
    offsetY = playerY - viewHeight / 2;

    // 确保视野不超出地图边界
    if (offsetX < 0) offsetX = 0;
    if (offsetY < 0) offsetY = 0;
    if (offsetX + viewWidth > mapWidth) offsetX = mapWidth - viewWidth;
    if (offsetY + viewHeight > mapHeight) offsetY = mapHeight - viewHeight;

    // 绘制视野之外（黑色）
    painter.fillRect(rect(), Qt::black);

    // 计算每个格子在屏幕上的大小
    int screenCellSize = 700 / std::max(viewWidth, viewHeight);
    if (screenCellSize < 10) screenCellSize = 10;

    int screenOffsetX = (700 - viewWidth * screenCellSize) / 2;
    int screenOffsetY = (700 - viewHeight * screenCellSize) / 2;

    // 绘制视野内的迷宫
    for (int y = offsetY; y < offsetY + viewHeight && y < mapHeight; y++) {
        for (int x = offsetX; x < offsetX + viewWidth && x < mapWidth; x++) {
            int screenX = screenOffsetX + (x - offsetX) * screenCellSize;
            int screenY = screenOffsetY + (y - offsetY) * screenCellSize;

            if (maze[y][x] == 1) {
                // 墙壁 - 黑色
                painter.fillRect(screenX, screenY, screenCellSize, screenCellSize, Qt::black);
            } else {
                // 地板 - 白色
                painter.fillRect(screenX, screenY, screenCellSize, screenCellSize, Qt::white);
            }

            // 绘制网格线
            painter.setPen(QPen(Qt::gray, 1));
            painter.drawRect(screenX, screenY, screenCellSize, screenCellSize);
        }
    }

    // 绘制终点（如果视野内）
    if (goalX >= offsetX && goalX < offsetX + viewWidth &&
        goalY >= offsetY && goalY < offsetY + viewHeight) {
        int gx = screenOffsetX + (goalX - offsetX) * screenCellSize;
        int gy = screenOffsetY + (goalY - offsetY) * screenCellSize;
        painter.setBrush(Qt::yellow);
        painter.setPen(Qt::NoPen);
        painter.drawRect(gx + 2, gy + 2, screenCellSize - 4, screenCellSize - 4);
    }

    // 绘制玩家（黑色小球）
    int px = screenOffsetX + (playerX - offsetX) * screenCellSize + screenCellSize / 2;
    int py = screenOffsetY + (playerY - offsetY) * screenCellSize + screenCellSize / 2;
    painter.setBrush(Qt::black);
    painter.setPen(QPen(Qt::white, 2));
    painter.drawEllipse(QPoint(px, py), screenCellSize / 2 - 2, screenCellSize / 2 - 2);
}