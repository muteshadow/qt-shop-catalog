#ifndef CARTWINDOW_H
#define CARTWINDOW_H

#include <QDialog>
#include <vector>
#include "database/database_manager.h"

class QVBoxLayout;

class CartWindow : public QDialog
{
    Q_OBJECT
public:
    explicit CartWindow(QWidget *parent = nullptr);
    void refreshCart(); // Оновлює відображення кошика

signals:
    void cartChanged();

private:
    QVBoxLayout *mainLayout;
};

#endif // CARTWINDOW_H
