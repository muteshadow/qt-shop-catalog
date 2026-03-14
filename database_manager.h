#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QString>
#include <QDebug>

// Структура Product
struct Product {
    int id{};
    QString name;
    double price{};
    QString imagePath;
    QString description;

    bool operator==(const Product &other) const {
        return id == other.id; // порівнюємо по id
    }
};

// Структура товару в кошику
struct CartItem {
    Product product;
    int quantity{1};

    CartItem(const Product &p, int q = 1) : product(p), quantity(q) {}
};

class DatabaseManager {
public:
    static DatabaseManager& instance();

    bool connectToDatabase();

    // Products
    QVector<Product> getAllProducts();
    Product getProductById(int id);
    bool addProduct(const Product &product);
    bool updateProduct(const Product &product);
    bool removeProduct(int productId);

    // Cart
    void addToCart(int productId);
    void removeFromCart(int productId);
    QVector<CartItem> getCartItems();
    void clearCart();
    void decreaseCartItem(int productId);
    bool isInCart(int productId) const;

    // Favorites
    void addToFavorites(int productId);
    QVector<int> getFavoriteProductIds();
    void removeFromFavorites(int productId);    

private:
    DatabaseManager() = default;

    // Helpers
    Product productFromQuery(QSqlQuery &query) const;
    bool cartItemExists(int productId) const;
    int getCartQuantity(int productId) const;

private:
    QSqlDatabase db;
};

#endif // DATABASE_MANAGER_H
