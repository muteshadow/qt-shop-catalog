#ifndef PRODUCTCARD_H
#define PRODUCTCARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "database/database_manager.h"

class QLabel;
class QVBoxLayout;

class ProductCard : public QWidget
{
    Q_OBJECT
public:
    explicit ProductCard(const Product &product, QWidget *parent = nullptr);

signals:
    void detailsRequested(Product product);
    void addToCart(Product product);
    void addToFavorites(Product product);

private:
    QLabel *imageLabel;
    QLabel *nameLabel;
    QLabel *priceLabel;
    QPushButton *detailsButton;
    QPushButton *addButton;
    QPushButton *heartButton;
    QVBoxLayout *mainLayout;
};

#endif // PRODUCTCARD_H
