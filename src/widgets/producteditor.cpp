#include "producteditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QDebug>

ProductEditor::ProductEditor(QWidget *parent)
    : QDialog(parent), isEditMode(false)
{
    setWindowTitle("Новий товар");
    setMinimumSize(400, 300);
    setupUI();
    connectSignals();
}

ProductEditor::ProductEditor(const Product &product, QWidget *parent)
    : ProductEditor(parent)
{
    setWindowTitle("Редагувати товар");
    isEditMode = true;
    currentProduct = product;

    nameEdit->setText(product.name);
    priceEdit->setValue(product.price);
    descriptionEdit->setText(product.description);
    imagePathEdit->setText(product.imagePath);
}

void ProductEditor::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(10);

    layout->addWidget(new QLabel("Назва:"));
    nameEdit = new QLineEdit(this);
    layout->addWidget(nameEdit);

    layout->addWidget(new QLabel("Ціна:"));
    priceEdit = new QDoubleSpinBox(this);
    priceEdit->setMaximum(100000);
    layout->addWidget(priceEdit);

    layout->addWidget(new QLabel("Опис:"));
    descriptionEdit = new QTextEdit(this);
    layout->addWidget(descriptionEdit);

    layout->addWidget(new QLabel("Шлях до зображення:"));
    imagePathEdit = new QLineEdit(this);
    layout->addWidget(imagePathEdit);

    QPushButton *browseBtn = new QPushButton("Обрати файл", this);
    layout->addWidget(browseBtn);

    connect(browseBtn, &QPushButton::clicked, [this]() {
        QString file = QFileDialog::getOpenFileName(this, "Оберіть зображення", "", "Images (*.png *.jpg *.jpeg)");
        if(!file.isEmpty()) imagePathEdit->setText(file);
    });

    saveBtn = new QPushButton("Зберегти", this);
    saveBtn->setStyleSheet(
        "QPushButton { background-color: #D3715A; color: #000; border-radius: 5px; font-size: 14px; padding: 5px 15px; }"
        "QPushButton:hover { background-color: #e08068; }"
    );
    layout->addWidget(saveBtn);

    cancelBtn = new QPushButton("Відмінити", this);
    cancelBtn->setStyleSheet(
        "QPushButton { background-color: #2C2C2C; color: #FBF7F2; border: 1px solid #888; border-radius: 4px; padding: 5px 10px; }"
        "QPushButton:hover { background-color: #555; }"
    );
    layout->addWidget(cancelBtn);
}

void ProductEditor::connectSignals()
{
    connect(saveBtn, &QPushButton::clicked, this, &ProductEditor::saveProduct);
    connect(cancelBtn, &QPushButton::clicked, this, &ProductEditor::reject);
}

void ProductEditor::saveProduct()
{
    if(nameEdit->text().trimmed().isEmpty()) {
        qDebug() << "Назва товару не може бути порожньою!";
        return;
    }

    currentProduct.name = nameEdit->text();
    currentProduct.price = priceEdit->value();
    currentProduct.description = descriptionEdit->toPlainText();
    currentProduct.imagePath = imagePathEdit->text();

    bool ok = false;
    if(isEditMode) {
        ok = DatabaseManager::instance().updateProduct(currentProduct);
    } else {
        ok = DatabaseManager::instance().addProduct(currentProduct);
    }

    if(ok) {
        emit productSaved();
        accept(); // закриває вікно
    } else {
        qDebug() << "Помилка збереження товару";
    }
}



