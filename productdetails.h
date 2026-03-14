#ifndef PRODUCTDETAILS_H
#define PRODUCTDETAILS_H

#include <QDialog>
#include "database_manager.h"

class QLabel;

class ProductDetails : public QDialog
{
    Q_OBJECT
public:
    explicit ProductDetails(const Product &product, QWidget *parent = nullptr);

signals:
    void addToCartClicked(Product product);

private:
    QLabel *imageLabel;
    QLabel *nameLabel;
    QLabel *priceLabel;
    QLabel *descriptionLabel;
    QPushButton *addToCartButton;
};

#endif // PRODUCTDETAILS_H
