#include "mapselect.h"
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QLabel>

MapSelect::MapSelect(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Mazeman - Select Map");
    setFixedSize(400, 500);
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("Select a map to play:", this);
    label->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    label->setAlignment(Qt::AlignCenter);

    mapList = new QListWidget(this);
    mapList->setStyleSheet(
        "QListWidget { font-size: 16px; border: 2px solid #ccc; border-radius: 5px; }"
        "QListWidget::item { padding: 10px; }"
        "QListWidget::item:selected { background-color: #4a90d9; color: white; }"
        );

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    selectButton = new QPushButton("Play", this);
    selectButton->setStyleSheet(
        "QPushButton { font-size: 18px; padding: 10px 30px; background-color: #27ae60; color: white; border: none; border-radius: 5px; }"
        "QPushButton:hover { background-color: #229954; }"
        );

    cancelButton = new QPushButton("Cancel", this);
    cancelButton->setStyleSheet(
        "QPushButton { font-size: 18px; padding: 10px 30px; background-color: #e74c3c; color: white; border: none; border-radius: 5px; }"
        "QPushButton:hover { background-color: #c0392b; }"
        );

    buttonLayout->addStretch();
    buttonLayout->addWidget(selectButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    layout->addWidget(label);
    layout->addWidget(mapList);
    layout->addLayout(buttonLayout);

    connect(selectButton, &QPushButton::clicked, this, &MapSelect::onMapSelected);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(mapList, &QListWidget::itemDoubleClicked, this, &MapSelect::onDoubleClicked);

    refreshMapList();
}

MapSelect::~MapSelect() {}

void MapSelect::refreshMapList()
{
    mapList->clear();
    selectedMapName = "";

    QDir dir(QApplication::applicationDirPath());
    QStringList filters;
    filters << "*.maze";
    QStringList files = dir.entryList(filters, QDir::Files);

    if (files.isEmpty()) {
        QListWidgetItem *item = new QListWidgetItem("Nothing is here");
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setForeground(Qt::gray);
        mapList->addItem(item);
        selectButton->setEnabled(false);
    } else {
        for (const QString &file : files) {
            QFileInfo info(file);
            mapList->addItem(info.baseName());
        }
        selectButton->setEnabled(true);
    }
}

void MapSelect::onMapSelected()
{
    QListWidgetItem *item = mapList->currentItem();
    if (item && item->text() != "Nothing is here") {
        selectedMapName = item->text().toStdString();
        accept();
    }
}

void MapSelect::onDoubleClicked(QListWidgetItem *item)
{
    if (item && item->text() != "Nothing is here") {
        selectedMapName = item->text().toStdString();
        accept();
    }
}

std::string MapSelect::selectedMap() const
{
    return selectedMapName;
}