#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QDialog>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include <vector>
#include <string>

class GameWidget : public QDialog
{
    Q_OBJECT

public:
    explicit GameWidget(const std::string &mapName, QWidget *parent = nullptr);
    ~GameWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void loadMap(const std::string &mapName);
    void updateGame();
    bool canMove(int x, int y);
    std::vector<std::vector<int>> maze;

    int playerX, playerY;  // 玩家位置
    int goalX, goalY;      // 终点位置
    int cellSize;
    int mapWidth, mapHeight;
    int offsetX, offsetY;  // 视野偏移

    QTimer *gameTimer;
    bool gameRunning;
    bool gameWon;
};

#endif // GAMEWIDGET_H