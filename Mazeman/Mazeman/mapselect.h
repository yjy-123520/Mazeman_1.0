#ifndef MAPSELECT_H
#define MAPSELECT_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

class MapSelect : public QDialog
{
    Q_OBJECT

public:
    explicit MapSelect(QWidget *parent = nullptr);
    ~MapSelect();

    std::string selectedMap() const;

private slots:
    void onMapSelected();
    void onDoubleClicked(QListWidgetItem *item);

private:
    void refreshMapList();

    QListWidget *mapList;
    QPushButton *selectButton;
    QPushButton *cancelButton;
    std::string selectedMapName;
};

#endif // MAPSELECT_H