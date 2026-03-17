#ifndef FAVORITESWINDOW_H
#define FAVORITESWINDOW_H

#include <QDialog>
#include <vector>
#include "database/database_manager.h"

class QVBoxLayout;

class FavoritesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FavoritesWindow(QWidget *parent = nullptr);

signals:
    void favoritesChanged();

private:
    QVBoxLayout *mainLayout;

    void refreshFavorites();
};

#endif // FAVORITESWINDOW_H
