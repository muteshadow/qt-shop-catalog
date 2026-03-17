#ifndef PRODUCTEDITOR_H
#define PRODUCTEDITOR_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include "database/database_manager.h"

class ProductEditor : public QDialog
{
    Q_OBJECT
public:
    explicit ProductEditor(QWidget *parent = nullptr);
    explicit ProductEditor(const Product &product, QWidget *parent = nullptr);

signals:
    void productSaved(); // сигнал, що товар збережено

private slots:
    void saveProduct();

private:
    void setupUI();
    void connectSignals();

private:
    bool isEditMode;
    Product currentProduct;

    QLineEdit *nameEdit;
    QDoubleSpinBox *priceEdit;
    QTextEdit *descriptionEdit;
    QLineEdit *imagePathEdit;

    QPushButton *saveBtn;
    QPushButton *cancelBtn;
};

#endif // PRODUCTEDITOR_H
