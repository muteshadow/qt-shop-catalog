#ifndef CATALOGWINDOW_H
#define CATALOGWINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include "database_manager.h"
#include "productcard.h"

class CatalogWindow : public QDialog
{
    Q_OBJECT
public:
    explicit CatalogWindow(QWidget *parent = nullptr);
    void refreshCatalog();

signals:
    void catalogChanged();

private:
    QVBoxLayout *mainLayout;
    QScrollArea *scrollArea;
    QWidget *container;
    QVBoxLayout *containerLayout;
    QPushButton *addNewButton;
    QPushButton *applyButton;
};

#endif // CATALOGWINDOW_H
