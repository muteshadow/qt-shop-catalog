#include "windows/mainwindow.h"
#include "ui/ui_mainwindow.h"

#include "database/database_manager.h"
#include "widgets/productdetails.h"
#include "widgets/productcard.h"
#include "windows/catalogwindow.h"
#include "windows/favoriteswindow.h"
#include "windows/cartwindow.h"
#include "widgets/savedpopup.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Підключення кнопок navbar
    connect(ui->btnSaved, &QPushButton::clicked, this, &MainWindow::openFavoritesWindow);
    connect(ui->btnCart, &QPushButton::clicked, this, &MainWindow::openCartWindow);
    connect(ui->btnCatalog, &QPushButton::clicked, this, &MainWindow::openCatalogWindow);

    setFixedSize(size());

    // Підключення до БД
    if(!DatabaseManager::instance().connectToDatabase()) {
        qDebug() << "Не вдалося підключитися до БД!";
        return;
    }

    updateCounters();

    // Основний layout у scrollArea
    QVBoxLayout *layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layout->setContentsMargins(20,0,20,0);
    layout->setSpacing(20);
    layout->addSpacing(650);

    // Завантаження карток
    QVector<Product> products = DatabaseManager::instance().getAllProducts();
    qDebug() << "Кількість товарів:" << products.size();

    for (const Product &p : products) {
        ProductCard *card = new ProductCard(p);
        layout->addWidget(card);
        setupProductCardSignals(card);
    }

    layout->addWidget(createFooter());
    layout->addStretch();
}

// -------------------------
// Допоміжний метод: підключення сигналів картки
// -------------------------
void MainWindow::setupProductCardSignals(ProductCard *card) {
    connect(card, &ProductCard::addToCart, this, [this](const Product &prod){
        addProductToCart(prod);
    });

    connect(card, &ProductCard::addToFavorites, this, [this](const Product &prod){
        addProductToFavorites(prod);
    });

    connect(card, &ProductCard::detailsRequested, this, [this](const Product &prod){
        ProductDetails *details = new ProductDetails(prod, this);

        connect(details, &ProductDetails::addToCartClicked, this, [this](const Product &p){
            addProductToCart(p);
        });

        details->exec();
        details->deleteLater(); // після закриття діалогу
    });
}

// -------------------------
// Допоміжний метод: footer
// -------------------------
QWidget* MainWindow::createFooter() {
    QWidget *footer = new QWidget(ui->scrollAreaWidgetContents);
    footer->setFixedHeight(45);
    footer->setStyleSheet("background-color: #D3715A;");

    QHBoxLayout *footerLayout = new QHBoxLayout(footer);
    footerLayout->setContentsMargins(15, 0, 15, 0);
    footerLayout->setSpacing(20);

    QStringList icons = { "assets/instagram.png",
                         "assets/tiktok.png",
                         "assets/twitter.png",
                         "assets/youtube.png" };

    for (const QString &iconPath : icons) {
        QLabel *icon = new QLabel(footer);
        icon->setPixmap(QPixmap(iconPath).scaled(18, 18, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        icon->setFixedSize(18, 18);
        footerLayout->addWidget(icon);
    }

    return footer;

}

// -------------------------
// Оновлення каталогу
// -------------------------
void MainWindow::openCatalogWindow() {
    CatalogWindow *catalog = new CatalogWindow(this);

    connect(catalog, &CatalogWindow::catalogChanged, this, [this]() {
        updateCounters();
        refreshProducts();
    });

    catalog->exec();
    catalog->deleteLater();
}

// -------------------------
// Оновлення всіх товарів
// -------------------------
void MainWindow::refreshProducts() {
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    // Очистка layout
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    layout->addSpacing(650);

    QVector<Product> products = DatabaseManager::instance().getAllProducts();
    for (const Product &p : products) {
        ProductCard *card = new ProductCard(p);
        layout->addWidget(card);
        setupProductCardSignals(card);
    }

    layout->addWidget(createFooter());
    layout->addStretch();
}

// -------------------------
// Вікна улюблених / кошика
// -------------------------
void MainWindow::openFavoritesWindow() {
    if (!favWin) {
        favWin = new FavoritesWindow(this);

        connect(favWin, &FavoritesWindow::favoritesChanged, this, &MainWindow::updateCounters);

        connect(favWin, &QDialog::finished, this, [this]() {
            favWin->deleteLater();
            favWin = nullptr;
        });
    }
    favWin->show();
}

void MainWindow::openCartWindow() {
    if (!cartWin) {
        cartWin = new CartWindow(this);

        connect(cartWin, &CartWindow::cartChanged, this, &MainWindow::updateCounters);

        connect(cartWin, &QDialog::finished, this, [this]() {
            cartWin->deleteLater();
            cartWin = nullptr;
        });
    }
    cartWin->show();
}

// -------------------------
// Лічильники
// -------------------------
void MainWindow::updateCounters() {
    int savedCount = DatabaseManager::instance().getFavoriteProductIds().size();

    int totalUnits = 0;
    auto cartItems = DatabaseManager::instance().getCartItems();
    for (const auto &item : cartItems) {
        totalUnits += item.quantity;   // сума одиниць
    }

    ui->lblSavedCounter->setText(QString::number(savedCount));
    ui->lblCartCounter->setText(QString::number(totalUnits));
}

// -------------------------
// Робота з улюбленими
// -------------------------
void MainWindow::addProductToFavorites(const Product &product) {
    QVector<int> favIds = DatabaseManager::instance().getFavoriteProductIds();

    if (favIds.contains(product.id)) {
        SavedPopup popup("Товар уже в улюблених!");
        popup.exec();
        return;
    }

    DatabaseManager::instance().addToFavorites(product.id);

    updateCounters();
    SavedPopup popup("Товар додано в улюблені!");
    popup.exec();
}


// -------------------------
// Робота з кошиком
// -------------------------
void MainWindow::addProductToCart(const Product &product) {
    DatabaseManager::instance().addToCart(product.id); // додаємо одиницю
    updateCounters();
}

void MainWindow::increaseCartItem(const Product &product) {
    DatabaseManager::instance().addToCart(product.id); // +1
    updateCounters();
}

void MainWindow::decreaseCartItem(const Product &product) {
    DatabaseManager::instance().decreaseCartItem(product.id); // -1

    if (!DatabaseManager::instance().isInCart(product.id)) {
        SavedPopup popup("Товар видалено з корзини");
        popup.exec();
    }

    updateCounters();
}

MainWindow::~MainWindow()
{
    delete ui;
}
