#include "windows/cartwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

CartWindow::CartWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Кошик");
    setMinimumSize(400, 400);
    setStyleSheet("background-color: #120E11; color: #FBF7F2;");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    refreshCart();

}

void CartWindow::refreshCart()
{
    // Очищення попередніх елементів
    QLayoutItem *child;
    while ((child = mainLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    QVector<CartItem> items = DatabaseManager::instance().getCartItems();

    if(items.isEmpty()) {
        QLabel *empty = new QLabel("Кошик порожній", this);
        empty->setAlignment(Qt::AlignCenter);
        empty->setStyleSheet("font-size: 9pt; font-family: 'Comic Sans MS'; color: #FBF7F2;");
        mainLayout->addWidget(empty);
        return;
    }

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    QWidget *container = new QWidget();
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setSpacing(5);
    containerLayout->setContentsMargins(5,5,5,5);

    double totalSum = 0;

    QString btnStyle =
        "color: #D3715A;"
        "border: 1px solid #D3715A;"
        "border-radius: 4px;"
        "padding: 4px 10px;"
        "background: none;"
        "font-family: 'Comic Sans MS';"
        "font-size: 9pt;";

    for (const CartItem &item : items) {
        Product p = item.product;
        int quantity = item.quantity;
        double itemSum = p.price * quantity;
        totalSum += itemSum;

        // Рядок товару
        QWidget *row = new QWidget(container);
        row->setStyleSheet(
            "background-color: #1C161A;"
            "border: 1px solid #D3715A;"
            "border-radius: 8px;"
            );
        row->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        QVBoxLayout *rowLayout = new QVBoxLayout(row);
        rowLayout->setContentsMargins(5, 5, 5, 5);
        rowLayout->setSpacing(5);

        // Назва товару з відступами зверху і знизу по 20px
        QLabel *label = new QLabel(
            QString("%1 ×%2 — %3 $").arg(p.name).arg(quantity).arg(itemSum), row
            );
        label->setStyleSheet(
            "color: #FBF7F2;"
            "font-size: 10pt;"
            "font-family: 'Comic Sans MS';"
            "font-weight: bold;"
            "padding-top: 20px;"
            "padding-bottom: 20px;"
            );
        rowLayout->addWidget(label);

        // Кнопки під назвою
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->setContentsMargins(0,0,0,0);
        buttonLayout->setSpacing(10);

        QPushButton *plusBtn = new QPushButton("+");
        plusBtn->setStyleSheet(btnStyle);
        plusBtn->setFixedSize(120, 30);

        QPushButton *minusBtn = new QPushButton("-");
        minusBtn->setStyleSheet(btnStyle);
        minusBtn->setFixedSize(120, 30);

        QPushButton *removeBtn = new QPushButton("Видалити");
        removeBtn->setStyleSheet(btnStyle);
        removeBtn->setFixedSize(120, 30);

        buttonLayout->addWidget(plusBtn);
        buttonLayout->addWidget(minusBtn);
        buttonLayout->addWidget(removeBtn);

        rowLayout->addLayout(buttonLayout);
        containerLayout->addWidget(row);

        int productId = p.id;

        connect(plusBtn, &QPushButton::clicked, [this, productId]() {
            DatabaseManager::instance().addToCart(productId);
            refreshCart();
            emit cartChanged();
        });

        connect(minusBtn, &QPushButton::clicked, [this, productId]() {
            DatabaseManager::instance().decreaseCartItem(productId);
            refreshCart();
            emit cartChanged();
        });

        connect(removeBtn, &QPushButton::clicked, [this, productId]() {
            DatabaseManager::instance().removeFromCart(productId);
            refreshCart();
            emit cartChanged();
        });
    }

    scroll->setWidget(container);
    mainLayout->addWidget(scroll);

    QLabel *sumLabel = new QLabel(QString("Загальна сума: %1 $").arg(totalSum), this);
    sumLabel->setAlignment(Qt::AlignCenter);
    sumLabel->setStyleSheet(
        "font-size: 12pt;"
        "font-family: 'Comic Sans MS';"
        "font-weight: bold;"
        "color: #FBF7F2;"
        );
    mainLayout->addWidget(sumLabel);

}
