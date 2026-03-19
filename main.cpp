#include <QApplication>
#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QRegularExpression>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QFile>
#include <QTextStream>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent)
    {
        // Создаем интерфейс
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        
        QFormLayout *formLayout = new QFormLayout();
        
        rusNameEdit = new QLineEdit();
        engNameEdit = new QLineEdit();
        weightEdit = new QLineEdit();
        wingspanEdit = new QLineEdit();  // Размах крыльев (раньше было range)
        
        formLayout->addRow("Русское название (только первая буква заглавная):", rusNameEdit);
        formLayout->addRow("Английское название (только первая буква заглавная):", engNameEdit);
        formLayout->addRow("Вес (дробное число с точкой, например 0.150):", weightEdit);
        formLayout->addRow("Размах крыльев (120-160, первое меньше второго):", wingspanEdit);
        
        mainLayout->addLayout(formLayout);
        
        // Возможность летать (да/нет)
        QGroupBox *flyGroup = new QGroupBox("Возможность летать:");
        QHBoxLayout *flyLayout = new QHBoxLayout();
        
        flyYes = new QRadioButton("Да");
        flyNo = new QRadioButton("Нет");
        flyNo->setChecked(true); // По умолчанию "Нет"
        
        flyLayout->addWidget(flyYes);
        flyLayout->addWidget(flyNo);
        flyGroup->setLayout(flyLayout);
        mainLayout->addWidget(flyGroup);
        
        // Группа "Особенности" с чекбоксами
        QGroupBox *featuresGroup = new QGroupBox("Особенности:");
        QVBoxLayout *featuresLayout = new QVBoxLayout();
        
        feature1 = new QCheckBox("Перелетные");
        feature2 = new QCheckBox("Водоплавающие");
        feature3 = new QCheckBox("Домашние");
        
        featuresLayout->addWidget(feature1);
        featuresLayout->addWidget(feature2);
        featuresLayout->addWidget(feature3);
        
        featuresGroup->setLayout(featuresLayout);
        mainLayout->addWidget(featuresGroup);
        
        // Кнопки
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        saveButton = new QPushButton("Сохранить");
        resetButton = new QPushButton("Сброс");
        
        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(resetButton);
        
        mainLayout->addLayout(buttonLayout);
        
        // Подключаем кнопки
        connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
        connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
        
        // Очищаем поля
        onResetClicked();
        
        setWindowTitle("Лабораторная работа 4 - Вариант 5");
        setFixedSize(500, 400);
    }

private slots:
    void onSaveClicked()
    {
        // Проверка обязательных полей
        if (rusNameEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Русское название' обязательно");
            return;
        }
        
        if (engNameEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Английское название' обязательно");
            return;
        }
        
        if (weightEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Вес' обязательно");
            return;
        }
        
        if (wingspanEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Размах крыльев' обязательно");
            return;
        }
        
        // Проверка русского названия (первая буква заглавная)
        QString rusText = rusNameEdit->text();
        QRegularExpression rusRegex("^[А-Я][а-я]*$");
        if (!rusRegex.match(rusText).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", 
                "Русское название должно начинаться с заглавной буквы и содержать только русские буквы");
            return;
        }
        
        // Проверка английского названия (первая буква заглавная)
        QString engText = engNameEdit->text();
        QRegularExpression engRegex("^[A-Z][a-z]*$");
        if (!engRegex.match(engText).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", 
                "Английское название должно начинаться с заглавной буквы и содержать только английские буквы");
            return;
        }
        
        // Проверка веса (дробное число с точкой)
        QString weightText = weightEdit->text();
        QRegularExpression weightRegex("^\\d+\\.\\d+$");
        if (!weightRegex.match(weightText).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", 
                "Вес должен быть дробным числом с точкой (например: 0.1, 3.14, 0.150)");
            return;
        }
        
        // Проверка размаха крыльев (диапазон)
        QString wingspanText = wingspanEdit->text();
        QRegularExpression wingspanRegex("^\\d{1,3}-\\d{1,3}$");
        if (!wingspanRegex.match(wingspanText).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", 
                "Размах крыльев должен быть в формате: число-число (например, 120-160)");
            return;
        }
        
        // Проверка что первое число меньше второго
        QStringList wingspanParts = wingspanText.split('-');
        if (wingspanParts.size() == 2) {
            int first = wingspanParts[0].toInt();
            int second = wingspanParts[1].toInt();
            if (first >= second) {
                QMessageBox::warning(this, "Ошибка", 
                    "В размахе крыльев первое число должно быть меньше второго");
                return;
            }
        }
        
        // Сохраняем в файл
        QFile file("result.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
            QTextStream out(&file);
            out << "Русское название: " << rusNameEdit->text() << "\n";
            out << "Английское название: " << engNameEdit->text() << "\n";
            out << "Вес: " << weightEdit->text() << "\n";
            out << "Размах крыльев: " << wingspanEdit->text() << "\n";
            
            // Возможность летать
            out << "Возможность летать: " << (flyYes->isChecked() ? "Да" : "Нет") << "\n";
            
            // Сохраняем особенности
            QStringList features;
            if (feature1->isChecked()) features << "Перелетные";
            if (feature2->isChecked()) features << "Водоплавающие";
            if (feature3->isChecked()) features << "Домашние";
            
            out << "Особенности: " << (features.isEmpty() ? "нет" : features.join(", ")) << "\n";
            out << "-------------------\n";
            file.close();
            
            QMessageBox::information(this, "Успех", "Данные сохранены в result.txt");
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось создать файл");
        }
    }
    
    void onResetClicked()
    {
        rusNameEdit->clear();
        engNameEdit->clear();
        weightEdit->clear();
        wingspanEdit->clear();
        
        // Сбрасываем радио-кнопки
        flyNo->setChecked(true);
        
        // Сбрасываем чекбоксы
        feature1->setChecked(false);
        feature2->setChecked(false);
        feature3->setChecked(false);
    }

private:
    QLineEdit *rusNameEdit;
    QLineEdit *engNameEdit;
    QLineEdit *weightEdit;
    QLineEdit *wingspanEdit;  // Размах крыльев
    
    QRadioButton *flyYes;
    QRadioButton *flyNo;
    
    QCheckBox *feature1;  // Перелетные
    QCheckBox *feature2;  // Водоплавающие
    QCheckBox *feature3;  // Домашние
    
    QPushButton *saveButton;
    QPushButton *resetButton;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}

#include "main.moc"
