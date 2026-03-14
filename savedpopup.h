#ifndef SAVEDPOPUP_H
#define SAVEDPOPUP_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFont>

class SavedPopup : public QDialog {
    Q_OBJECT
public:
    explicit SavedPopup(const QString &message, QWidget *parent = nullptr)
        : QDialog(parent)
    {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        setFixedSize(250, 120);
        setStyleSheet("background-color: #120E11; border-radius: 10px;");

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(15, 15, 15, 15);

        QFont font("Comic Sans MS", 10);

        QLabel *label = new QLabel(message, this);
        label->setStyleSheet("color: #FBF7F2; font-size: 14px;");
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);

        QPushButton *closeBtn = new QPushButton("Закрити", this);
        closeBtn->setFont(font);
        closeBtn->setStyleSheet(
            "QPushButton { background-color: #D3715A; color: #FBF7F2; border-radius: 5px; padding: 5px; }"
            "QPushButton:hover { background-color: #e08068; }"
            );
        connect(closeBtn, &QPushButton::clicked, this, &SavedPopup::accept);
        layout->addWidget(closeBtn, 0, Qt::AlignCenter);
    }
};

#endif // SAVEDPOPUR_H
