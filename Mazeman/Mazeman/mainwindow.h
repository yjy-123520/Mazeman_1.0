#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onStartClicked();
    void onMapClicked();
    void onExitClicked();
private:
    void setupUI();
    void showMainMenu();

    QWidget *centralWidget;
    QVBoxLayout *layout;
    QLabel *titleLabel;
    QPushButton *startButton;
    QPushButton *mapButton;
    QPushButton *exitButton;
};
#endif // MAINWINDOW_H