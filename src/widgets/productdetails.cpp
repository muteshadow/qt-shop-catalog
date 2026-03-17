#include "widgets/productdetails.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QFont>
#include "widgets/savedpopup.h"

ProductDetails::ProductDetails(const Product &product, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Деталі товару");
    setMinimumSize(400, 500);
    setStyleSheet("background-color: #120E11;");

    // Основний контейнер
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    // --- Зображення ---
    imageLabel = new QLabel(this);
    imageLabel->setPixmap(QPixmap(product.imagePath).scaled(300, 300, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(imageLabel);

    // --- Назва ---
    nameLabel = new QLabel(product.name, this);
    QFont titleFont("Comic Sans MS", 10, QFont::Bold);
    nameLabel->setFont(titleFont);
    nameLabel->setStyleSheet("color: #FBF7F2;");
    nameLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(nameLabel);

    // --- Ціна ---
    priceLabel = new QLabel(QString("%1 $").arg(product.price), this);
    QFont priceFont("Comic Sans MS", 9, QFont::Bold);
    priceLabel->setFont(priceFont);
    priceLabel->setStyleSheet("color: #D3715A;");
    priceLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(priceLabel);

    // --- Опис ---
    descriptionLabel = new QLabel(product.description, this);
    QFont descFont("Comic Sans MS", 9);
    descriptionLabel->setFont(descFont);
    descriptionLabel->setStyleSheet("color: #FBF7F2; text-align: justify");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setAlignment(Qt::AlignTop);
    layout->addWidget(descriptionLabel);

    // --- Кнопка Додати в кошик ---
    addToCartButton = new QPushButton("Додати в кошик", this);
    addToCartButton->setFixedHeight(35);
    addToCartButton->setStyleSheet(
        "QPushButton { background-color: #000000; color: #D3715A; border: 2px solid #D3715A; border-radius: 5px; font-size: 14px; padding: 5px 15px; }"
        "QPushButton:hover { background-color: #D3715A; color: #FBF7F2; }"
    );
    addToCartButton->setCursor(Qt::PointingHandCursor);
    layout->addWidget(addToCartButton, 0, Qt::AlignCenter);

    // Підключаємо сигнал кнопки
    connect(addToCartButton, &QPushButton::clicked, [this, product]() {
        emit addToCartClicked(product);
        SavedPopup *popup = new SavedPopup("Товар додано в корзину!", this);
        popup->exec();
    });
}
