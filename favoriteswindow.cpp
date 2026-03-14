#include "favoriteswindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

FavoritesWindow::FavoritesWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Улюблені");
    setMinimumSize(400, 400);
    setStyleSheet("background-color: #120E11; color: #FBF7F2;");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    refreshFavorites();
}

void FavoritesWindow::refreshFavorites()
{
    // Очищаємо старі віджети
    QLayoutItem *child;
    while ((child = mainLayout->takeAt(0)) != nullptr) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }

    QVector<int> favIds = DatabaseManager::instance().getFavoriteProductIds();
    if(favIds.isEmpty()) {
        QLabel *empty = new QLabel("Список улюблених порожній", this);
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("font-size: 9pt; font-family: 'Comic Sans MS'; color: #FBF7F2;");
        mainLayout->addWidget(empty);
        return;
    }

    // Скролл для великої кількості товарів
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    QWidget *container = new QWidget();
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setSpacing(10);

    for(int id : favIds) {
        Product p = DatabaseManager::instance().getProductById(id);

        QWidget *row = new QWidget(container);
        row->setStyleSheet("background-color: #1C161A; border: 1px solid #D3715A; border-radius: 8px; padding: 10px;");
        QVBoxLayout *rowLayout = new QVBoxLayout(row);

        QLabel *label = new QLabel(QString("%1 - %2 $").arg(p.name).arg(p.price), row);
        label->setStyleSheet("color: #FBF7F2; font-size: 9pt; font-family: 'Comic Sans MS'; font-weight: bold;");
        rowLayout->addWidget(label);

        QPushButton *remove = new QPushButton("Видалити", row);
        remove->setStyleSheet("color: #D3715A; border: 1px solid #D3715A; border-radius: 4px; padding: 5px 10px; background: none; font-family: 'Comic Sans MS'; font-size: 9pt;");
        rowLayout->addWidget(remove);

        containerLayout->addWidget(row);

        connect(remove, &QPushButton::clicked, [this, id]() {
            DatabaseManager::instance().removeFromFavorites(id);
            refreshFavorites();
            emit favoritesChanged();
        });
    }

    scroll->setWidget(container);
    mainLayout->addWidget(scroll);

    // Кнопка Очистити список
    QPushButton *clearButton = new QPushButton("Очистити список", this);
    clearButton->setFixedHeight(35);
    clearButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #D3715A;"
        "   color: #FBF7F2;"
        "   border: 2px solid #D3715A;"
        "   border-radius: 5px;"
        "   font-size: 12pt;"
        "   padding: 5px 15px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e08068;"
        "}"
        );
    clearButton->setCursor(Qt::PointingHandCursor);
    mainLayout->addWidget(clearButton, 0, Qt::AlignCenter);

    connect(clearButton, &QPushButton::clicked, [this]() {
        QVector<int> favIds = DatabaseManager::instance().getFavoriteProductIds();
        for(int id : favIds)
            DatabaseManager::instance().removeFromFavorites(id);
        refreshFavorites();
        emit favoritesChanged();
    });
}
