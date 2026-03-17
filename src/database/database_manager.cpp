#include "database/database_manager.h"
#include <QSqlError>

// Singleton
DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager inst;
    return inst;
}

// Підключення до БД
bool DatabaseManager::connectToDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data/products.db");

    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
        return false;
    }
    return true;
}

// ---------------- HELPER: Product
Product DatabaseManager::productFromQuery(QSqlQuery &query) const {
    Product p;
    p.id = query.value(0).toInt();
    p.name = query.value(1).toString();
    p.price = query.value(2).toDouble();
    p.imagePath = query.value(3).toString();
    p.description = query.value(4).toString();
    return p;
}

// ====================== PRODUCTS ======================
QVector<Product> DatabaseManager::getAllProducts() {
    QVector<Product> products;

    QSqlQuery query("SELECT id, name, price, imagePath, description FROM products");
    while (query.next()) {
        products.append(productFromQuery(query));
    }
    return products;
}

Product DatabaseManager::getProductById(int id) {
    QSqlQuery query;
    query.prepare("SELECT id, name, price, imagePath, description FROM products WHERE id=:id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return productFromQuery(query);
    }
    return {};
}

bool DatabaseManager::addProduct(const Product &product) {
    QSqlQuery query;
    query.prepare("INSERT INTO products (name, price, description, imagePath) "
                  "VALUES (:name, :price, :description, :imagePath)");

    query.bindValue(":name", product.name);
    query.bindValue(":price", product.price);
    query.bindValue(":description", product.description);
    query.bindValue(":imagePath", product.imagePath);

    if(!query.exec()) {
        qDebug() << "Error adding product:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateProduct(const Product &product) {
    QSqlQuery query;
    query.prepare("UPDATE products SET name=:name, price=:price, description=:description, imagePath=:imagePath "
                  "WHERE id=:id");

    query.bindValue(":name", product.name);
    query.bindValue(":price", product.price);
    query.bindValue(":description", product.description);
    query.bindValue(":imagePath", product.imagePath);
    query.bindValue(":id", product.id);

    if(!query.exec()) {
        qDebug() << "Error updating product:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::removeProduct(int productId) {
    QSqlQuery query;
    query.prepare("DELETE FROM products WHERE id=:id");
    query.bindValue(":id", productId);

    if(!query.exec()) {
        qDebug() << "Error removing product:" << query.lastError().text();
        return false;
    }
    return true;
}

// ---------------- HELPER: Cart
bool DatabaseManager::cartItemExists(int productId) const {
    QSqlQuery query;
    query.prepare("SELECT 1 FROM cart WHERE product_id=:id LIMIT 1");
    query.bindValue(":id", productId);
    query.exec();
    return query.next();
}

int DatabaseManager::getCartQuantity(int productId) const {
    QSqlQuery query;
    query.prepare("SELECT quantity FROM cart WHERE product_id=:id");
    query.bindValue(":id", productId);
    query.exec();
    if (query.next()) return query.value(0).toInt();
    return 0;
}

// ====================== CART ======================
void DatabaseManager::addToCart(int productId) {
    if(cartItemExists(productId)) {
        int qty = getCartQuantity(productId) + 1;
        QSqlQuery query;
        query.prepare("UPDATE cart SET quantity = :q WHERE product_id = :id");
        query.bindValue(":q", qty);
        query.bindValue(":id", productId);
        query.exec();
    } else {
        QSqlQuery query;
        query.prepare("INSERT INTO cart (product_id, quantity) VALUES (:id, 1)");
        query.bindValue(":id", productId);
        query.exec();
    }
}

void DatabaseManager::removeFromCart(int productId) {
    QSqlQuery query;
    query.prepare("DELETE FROM cart WHERE product_id=:id");
    query.bindValue(":id", productId);
    query.exec();
}

QVector<CartItem> DatabaseManager::getCartItems() {
    QVector<CartItem> items;

    QSqlQuery query("SELECT product_id, quantity FROM cart");
    while(query.next()) {
        int id = query.value(0).toInt();
        int qty  = query.value(1).toInt();
        items.append(CartItem(getProductById(id), qty));
    }
    return items;
}

void DatabaseManager::clearCart() {
    QSqlQuery query("DELETE FROM cart");
    query.exec();
}

void DatabaseManager::decreaseCartItem(int productId) {  
    int qty = getCartQuantity(productId);

    if (qty <= 1) {
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE cart SET quantity=:q WHERE product_id=:id");
    query.bindValue(":q", qty - 1);
    query.bindValue(":id", productId);
    query.exec();
}

bool DatabaseManager::isInCart(int productId) const {
    return cartItemExists(productId);
}

// ====================== FAVORITES ======================
void DatabaseManager::addToFavorites(int productId) {
    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO favorites (product_id) VALUES (:id)");
    query.bindValue(":id", productId);
    query.exec();
}

QVector<int> DatabaseManager::getFavoriteProductIds() {
    QVector<int> ids;

    QSqlQuery query("SELECT product_id FROM favorites");
    while(query.next()) {
        ids.append(query.value(0).toInt());
    }
    return ids;
}

void DatabaseManager::removeFromFavorites(int productId) {
    QSqlQuery query;
    query.prepare("DELETE FROM favorites WHERE product_id=:id");
    query.bindValue(":id", productId);
    query.exec();
}
