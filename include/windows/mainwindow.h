#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

#include "database_manager.h"
#include "favoriteswindow.h"
#include "cartwindow.h"
#include "productcard.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFavoritesWindow();    
    void openCartWindow();
    void openCatalogWindow();

    void updateCounters();
    void refreshProducts();

    void addProductToFavorites(const Product &product);
    void addProductToCart(const Product &product);
    void increaseCartItem(const Product &product);
    void decreaseCartItem(const Product &product);

signals:
    void detailsRequested(const Product &product);

private:
    Ui::MainWindow *ui;

    FavoritesWindow *favWin = nullptr;
    CartWindow *cartWin = nullptr;

    QWidget* createFooter();
    void setupProductCardSignals(ProductCard *card);
};

#endif // MAINWINDOW_H
