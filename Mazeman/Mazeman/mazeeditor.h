#ifndef MAZEEDITOR_H
#define MAZEEDITOR_H

#include <QDialog>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <vector>

class MazeEditor : public QDialog
{
    Q_OBJECT

public:
    explicit MazeEditor(QWidget *parent = nullptr);
    ~MazeEditor();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onClearClicked();
    void onSaveClicked();
    void onUndoClicked();

private:
    void drawGrid(QPainter &painter);
    void drawWalls(QPainter &painter);

    std::vector<std::vector<int>> maze;  // 0=路径, 1=墙壁
    std::vector<std::vector<int>> undoStack;  // 用于撤销
    int gridSize;  // 格子数量 (gridSize x gridSize)
    int cellSize;  // 每个格子的像素大小
    bool painting;  // 当前是在画墙还是擦墙
    int offsetX, offsetY;  // 地图偏移
};

#endif // MAZEEDITOR_H