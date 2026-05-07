#include "mazeeditor.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QApplication>

MazeEditor::MazeEditor(QWidget *parent)
    : QDialog(parent), gridSize(25), cellSize(20), painting(true)
{
    setWindowTitle("Mazeman - Map Editor");
    setFixedSize(700, 750);
    setModal(true);

    // 初始化迷宫，全部为路径(0)
    maze.resize(gridSize, std::vector<int>(gridSize, 0));

    // 计算偏移使地图居中
    offsetX = (700 - gridSize * cellSize) / 2;
    offsetY = (700 - gridSize * cellSize) / 2;
    if (offsetX < 0) offsetX = 0;
    if (offsetY < 0) offsetY = 0;

    // 底部按钮
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 10);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();

    QPushButton *undoButton = new QPushButton("Delete (Undo)", this);
    undoButton->setStyleSheet(
        "QPushButton { font-size: 16px; padding: 8px 20px; background-color: #f39c12; color: white; border: none; border-radius: 5px; }"
        "QPushButton:hover { background-color: #e67e22; }"
        );

    QPushButton *clearButton = new QPushButton("Clear All", this);
    clearButton->setStyleSheet(
        "QPushButton { font-size: 16px; padding: 8px 20px; background-color: #e74c3c; color: white; border: none; border-radius: 5px; }"
        "QPushButton:hover { background-color: #c0392b; }"
        );

    QPushButton *saveButton = new QPushButton("Finish & Save", this);
    saveButton->setStyleSheet(
        "QPushButton { font-size: 16px; padding: 8px 20px; background-color: #27ae60; color: white; border: none; border-radius: 5px; }"
        "QPushButton:hover { background-color: #229954; }"
        );

    buttonLayout->addWidget(undoButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch();

    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    connect(undoButton, &QPushButton::clicked, this, &MazeEditor::onUndoClicked);
    connect(clearButton, &QPushButton::clicked, this, &MazeEditor::onClearClicked);
    connect(saveButton, &QPushButton::clicked, this, &MazeEditor::onSaveClicked);

    setMouseTracking(true);
}

MazeEditor::~MazeEditor() {}

void MazeEditor::onUndoClicked()
{
    if (!undoStack.empty()) {
        std::vector<int> lastEdit = undoStack.back();
        undoStack.pop_back();
        int x = lastEdit[0];
        int y = lastEdit[1];
        maze[y][x] = lastEdit[2];  // 恢复原来的值
        update();
    }
}

void MazeEditor::onClearClicked()
{
    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            maze[y][x] = 0;
        }
    }
    undoStack.clear();
    update();
}

void MazeEditor::onSaveClicked()
{
    bool ok;
    QString mapName = QInputDialog::getText(this, "Save Map", "Enter map name:", QLineEdit::Normal, "", &ok);

    if (ok && !mapName.isEmpty()) {
        // 保存到文件
        QDir dir(QApplication::applicationDirPath());
        QString filePath = dir.filePath(mapName + ".maze");

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "Cannot save file!");
            return;
        }

        QTextStream out(&file);
        for (int y = 0; y < gridSize; y++) {
            for (int x = 0; x < gridSize; x++) {
                out << maze[y][x];
            }
            if (y < gridSize - 1) out << "\n";
        }

        file.close();

        // 确保出入口可通行
        maze[gridSize - 2][1] = 0;  // 左下角出生点
        maze[1][gridSize - 2] = 0;  // 右上角终点

        QMessageBox::information(this, "Success", "Map saved successfully!");
        accept();  // 返回主菜单
    }
}

void MazeEditor::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 背景
    painter.fillRect(rect(), QColor(200, 200, 200));

    // 绘制网格
    drawGrid(painter);

    // 绘制墙壁
    drawWalls(painter);
}

void MazeEditor::drawGrid(QPainter &painter)
{
    painter.setPen(QPen(Qt::gray, 1));

    for (int i = 0; i <= gridSize; i++) {
        // 竖线
        painter.drawLine(offsetX + i * cellSize, offsetY,
                         offsetX + i * cellSize, offsetY + gridSize * cellSize);
        // 横线
        painter.drawLine(offsetX, offsetY + i * cellSize,
                         offsetX + gridSize * cellSize, offsetY + i * cellSize);
    }
}

void MazeEditor::drawWalls(QPainter &painter)
{
    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            if (maze[y][x] == 1) {
                painter.fillRect(offsetX + x * cellSize + 1, offsetY + y * cellSize + 1,
                                 cellSize - 2, cellSize - 2, Qt::black);
            }
        }
    }
}

void MazeEditor::mousePressEvent(QMouseEvent *event)
{
    int x = (event->pos().x() - offsetX) / cellSize;
    int y = (event->pos().y() - offsetY) / cellSize;

    if (x >= 0 && x < gridSize && y >= 0 && y < gridSize) {
        // 左下角和右上角保留为出入口
        if ((x == 1 && y == gridSize - 2) || (x == gridSize - 2 && y == 1)) {
            return;
        }

        // 保存当前状态用于撤销
        std::vector<int> state = {x, y, maze[y][x]};
        undoStack.push_back(state);

        // 切换墙壁/路径
        maze[y][x] = (maze[y][x] == 0) ? 1 : 0;

        update();
    }
}

void MazeEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int x = (event->pos().x() - offsetX) / cellSize;
        int y = (event->pos().y() - offsetY) / cellSize;

        if (x >= 0 && x < gridSize && y >= 0 && y < gridSize) {
            // 左下角和右上角保留为出入口
            if ((x == 1 && y == gridSize - 2) || (x == gridSize - 2 && y == 1)) {
                return;
            }

            if (maze[y][x] == 0) {  // 只画墙，不移除
                std::vector<int> state = {x, y, maze[y][x]};
                undoStack.push_back(state);
                maze[y][x] = 1;
                update();
            }
        }
    }
}