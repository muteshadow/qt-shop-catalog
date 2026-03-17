#include "widgets/productcard.h"
#include <QFont>

ProductCard::ProductCard(const Product &product, QWidget *parent)
    : QWidget(parent)
{
    // Основна картка
    QWidget *card = new QWidget(this);
    card->setObjectName("CardContainer");
    card->setFixedSize(380, 440);
    card->setStyleSheet("border-radius: 10px; border-bottom: 1px solid #D3715A; ");

    mainLayout = new QVBoxLayout(card);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(5);

    // --- Зображення ---
    imageLabel = new QLabel(card);
    imageLabel->setFixedSize(380, 380);
    imageLabel->setPixmap(QPixmap(product.imagePath).scaled(380,380, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("border: none; border-top-right-radius: 10px; border-top-left-radius: 10px");
    mainLayout->addWidget(imageLabel);

    // --- Нижній ряд ---
    QWidget *textButtonRow = new QWidget(card);
    QHBoxLayout *rowLayout = new QHBoxLayout(textButtonRow);
    rowLayout->setContentsMargins(0,0,0,0);
    rowLayout->setSpacing(5);

    // Лівий блок (назва + деталі+ціна)
    QWidget *textBlock = new QWidget(card);
    QVBoxLayout *textLayout = new QVBoxLayout(textBlock);
    textLayout->setContentsMargins(0,0,0,0);
    textLayout->setSpacing(5);

    // --- Назва ---
    nameLabel = new QLabel(product.name, card);
    QFont nameFont("Comic Sans MS");
    nameLabel->setFont(nameFont);
    nameLabel->setWordWrap(true);
    nameLabel->setStyleSheet("font-size: 14px; color: #FBF7F2; border: none;");
    textLayout->addWidget(nameLabel);

    // --- Детальніше + ціна ---
    QWidget *priceDetailsBlock = new QWidget(card);
    QHBoxLayout *pdLayout = new QHBoxLayout(priceDetailsBlock);
    pdLayout->setContentsMargins(0,0,0,0);
    pdLayout->setSpacing(0);

    // Детальніше
    detailsButton = new QPushButton("Детальніше", card);
    detailsButton->setFlat(true);
    detailsButton->setCursor(Qt::PointingHandCursor);
    detailsButton->setFont(nameFont);
    detailsButton->setStyleSheet("QPushButton {font-size: 12px; color: #D3715A; text-decoration: underline; background: none; border: none; }");
    pdLayout->addWidget(detailsButton);

    connect(detailsButton, &QPushButton::clicked, [this, product]() {
        emit detailsRequested(product);
    });

    pdLayout->addStretch();  // Розтягуємо перед ціною вправо

    // Ціна
    priceLabel = new QLabel(QString("%1 $").arg(product.price), card);
    priceLabel->setFont(nameFont);
    priceLabel->setStyleSheet("font-size: 12px; color: #FBF7F2; background-color: transparent; border: none; ");
    pdLayout->addWidget(priceLabel);

    textLayout->addWidget(priceDetailsBlock);
    rowLayout->addWidget(textBlock);

    // --- Вертикальний блок кнопок (+ і ♥) ---
    QWidget *buttonColumn = new QWidget(card);
    QVBoxLayout *btnLayout = new QVBoxLayout(buttonColumn);
    btnLayout->setContentsMargins(0,0,0,0);
    btnLayout->setSpacing(2);

    // --- Кнопка додати ---
    addButton = new QPushButton("+", buttonColumn);
    addButton->setFixedSize(25, 25);
    addButton->setStyleSheet(
        "QPushButton { background-color: #D3715A; color: #000; border-radius: 12px; border: none; font-size: 16px; padding-bottom: 5px; }"
        "QPushButton:hover { background-color: #e08068; }"
    );
    btnLayout->addWidget(addButton);

    connect(addButton, &QPushButton::clicked, [this, product]() {
        emit addToCart(product);
    });

    // --- Кнопка зберегти (сердечко) ---
    heartButton = new QPushButton("♥", buttonColumn);
    heartButton->setFixedSize(25, 25);
    heartButton->setStyleSheet(
        "QPushButton { background-color: #D3715A; color: #000; border-radius: 12px; border: none; font-size: 16px; }"
        "QPushButton:hover { background-color: #e08068; }"
    );
    btnLayout->addWidget(heartButton);

    connect(heartButton, &QPushButton::clicked, [this, product]() {
        emit addToFavorites(product);
    });

    btnLayout->addStretch();
    rowLayout->addWidget(buttonColumn);

    mainLayout->addWidget(textButtonRow);

    // Обгортка
    QVBoxLayout *wrapperLayout = new QVBoxLayout(this);
    wrapperLayout->addWidget(card);
    wrapperLayout->setContentsMargins(0,0,0,0);
}
