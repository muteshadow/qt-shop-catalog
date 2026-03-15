#include "catalogwindow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include "productdetails.h"
#include "producteditor.h"

CatalogWindow::CatalogWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Каталог");
    setMinimumSize(500, 500);
    setStyleSheet("background-color: #120E11; color: #FBF7F2;");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20,20,20,20);
    mainLayout->setSpacing(20);

    // Кнопка додати новий товар
    addNewButton = new QPushButton("Додати новий товар", this);
    addNewButton->setStyleSheet(
        "QPushButton { background-color: #000000; color: #D3715A; border: 2px solid #D3715A; border-radius: 5px; padding: 5px 15px; }"
        "QPushButton:hover { background-color: #D3715A; color: #FBF7F2; }"
    );
    mainLayout->addWidget(addNewButton, 0, Qt::AlignRight);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    container = new QWidget();
    containerLayout = new QVBoxLayout(container);
    containerLayout->setSpacing(10);
    containerLayout->setContentsMargins(0,0,0,0);

    scrollArea->setWidget(container);
    mainLayout->addWidget(scrollArea);

    // Кнопка застосувати
    applyButton = new QPushButton("Застосувати", this);
    applyButton->setStyleSheet(
        "QPushButton { background-color: #D3715A; color: #FBF7F2; border-radius: 5px; padding: 5px 15px; }"
        "QPushButton:hover { background-color: #FBF7F2; color: #D3715A; }"
    );
    mainLayout->addWidget(applyButton, 0, Qt::AlignRight);

    connect(applyButton, &QPushButton::clicked, [this]() {
        emit catalogChanged();
        this->accept();
    });

    connect(addNewButton, &QPushButton::clicked, [this]() {
        ProductEditor editor(this);
        connect(&editor, &ProductEditor::productSaved, this, [this]() {
            refreshCatalog();
            emit catalogChanged();
        });
        editor.exec();
    });

    refreshCatalog();
}

void CatalogWindow::refreshCatalog()
{
    // Очищення попередніх елементів
    QLayoutItem *child;
    while ((child = containerLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    QVector<Product> products = DatabaseManager::instance().getAllProducts();

    for (const Product &p : products) {
        QWidget *row = new QWidget(container);
        row->setStyleSheet("background-color: #1C161A; border: 1px solid #D3715A; border-radius: 8px;");

        QVBoxLayout *rowLayout = new QVBoxLayout(row); // вертикальний layout
        rowLayout->setContentsMargins(5,5,5,5);
        rowLayout->setSpacing(5);

        // Назва + ціна
        QLabel *label = new QLabel(QString("%1 — %2 $").arg(p.name).arg(p.price), row);
        label->setStyleSheet("color: #FBF7F2; font-size: 10pt; font-family: 'Comic Sans MS'; font-weight: bold;");
        label->setContentsMargins(0, 10, 0, 10);
        rowLayout->addWidget(label);

        // Рядок кнопок під назвою
        QHBoxLayout *btnLayout = new QHBoxLayout();
        btnLayout->setSpacing(10);

        QPushButton *editBtn = new QPushButton("Редагувати", row);
        editBtn->setStyleSheet("color: #D3715A; border: 1px solid #D3715A; border-radius: 4px; padding: 4px 10px;");
        btnLayout->addWidget(editBtn);

        QPushButton *deleteBtn = new QPushButton("Видалити", row);
        deleteBtn->setStyleSheet("color: #D3715A; border: 1px solid #D3715A; border-radius: 4px; padding: 4px 10px;");
        btnLayout->addWidget(deleteBtn);

        rowLayout->addLayout(btnLayout);
        containerLayout->addWidget(row);

        // Сигнали кнопок
        connect(editBtn, &QPushButton::clicked, [this, p]() {
            ProductEditor editor(p, this);
            connect(&editor, &ProductEditor::productSaved, this, [this]() {
                refreshCatalog();
                emit catalogChanged();
            });
            editor.exec();
        });

        // Після видалення
        connect(deleteBtn, &QPushButton::clicked, [this, p, row]() {
            if(DatabaseManager::instance().removeProduct(p.id)) {
                row->deleteLater();
                emit catalogChanged(); // сигнал про зміну каталогу
            }
        });
    }
}
