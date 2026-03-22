#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QRegularExpression>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFile>
#include <QTextStream>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent)
    {
        // Поля ввода с абсолютными координатами
        QLabel *rusLabel = new QLabel("Русское название:", this);
        rusLabel->setGeometry(50, 30, 150, 25);

        rusNameEdit = new QLineEdit(this);
        rusNameEdit->setGeometry(210, 30, 250, 25);

        QLabel *engLabel = new QLabel("Английское название:", this);
        engLabel->setGeometry(50, 70, 150, 25);

        engNameEdit = new QLineEdit(this);
        engNameEdit->setGeometry(210, 70, 250, 25);

        QLabel *weightLabel = new QLabel("Вес (дробное число с точкой):", this);
        weightLabel->setGeometry(50, 110, 200, 25);

        weightEdit = new QLineEdit(this);
        weightEdit->setGeometry(260, 110, 150, 25);

        QLabel *wingspanLabel = new QLabel("Размах крыльев:", this);
        wingspanLabel->setGeometry(50, 150, 150, 25);

        wingspanEdit = new QLineEdit(this);
        wingspanEdit->setGeometry(210, 150, 250, 25);

        // Группа "Возможность летать"
        QGroupBox *flyGroup = new QGroupBox("Возможность летать:", this);
        flyGroup->setGeometry(50, 200, 300, 80);

        flyYes = new QRadioButton("Да", flyGroup);
        flyYes->setGeometry(20, 30, 80, 25);

        flyNo = new QRadioButton("Нет", flyGroup);
        flyNo->setGeometry(120, 30, 80, 25);

        // Создаем группу для радиокнопок
        flyButtonGroup = new QButtonGroup(this);
        flyButtonGroup->addButton(flyYes);
        flyButtonGroup->addButton(flyNo);

        // Запоминаем состояние для логики снятия
        lastChecked = nullptr;

        // Логика для кнопки "Да"
        connect(flyYes, &QRadioButton::clicked, [this]() {
            if (flyYes->isChecked()) {
                // Если кнопка была уже выбрана до клика - снимаем
                if (lastChecked == flyYes) {
                    flyButtonGroup->setExclusive(false);
                    flyYes->setChecked(false);
                    flyButtonGroup->setExclusive(true);
                    lastChecked = nullptr;
                } else {
                    // Иначе запоминаем что выбрали эту
                    lastChecked = flyYes;
                }
            }
        });

        // Логика для кнопки "Нет"
        connect(flyNo, &QRadioButton::clicked, [this]() {
            if (flyNo->isChecked()) {
                // Если кнопка была уже выбрана до клика - снимаем
                if (lastChecked == flyNo) {
                    flyButtonGroup->setExclusive(false);
                    flyNo->setChecked(false);
                    flyButtonGroup->setExclusive(true);
                    lastChecked = nullptr;
                } else {
                    // Иначе запоминаем что выбрали эту
                    lastChecked = flyNo;
                }
            }
        });

        // Группа "Особенности"
        QGroupBox *featuresGroup = new QGroupBox("Особенности:", this);
        featuresGroup->setGeometry(50, 300, 300, 120);

        feature1 = new QCheckBox("Перелетные", featuresGroup);
        feature1->setGeometry(20, 30, 120, 25);

        feature2 = new QCheckBox("Водоплавающие", featuresGroup);
        feature2->setGeometry(20, 60, 120, 25);

        feature3 = new QCheckBox("Домашние", featuresGroup);
        feature3->setGeometry(20, 90, 120, 25);

        // Кнопки
        saveButton = new QPushButton("Сохранить", this);
        saveButton->setGeometry(150, 450, 120, 35);
        saveButton->setStyleSheet("background-color: green; color: white; font-weight: bold;");

        resetButton = new QPushButton("Сброс", this);
        resetButton->setGeometry(300, 450, 120, 35);
        resetButton->setStyleSheet("background-color: red; color: white; font-weight: bold;");

        connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
        connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);

        setWindowTitle("Птицы");
        setFixedSize(550, 550);
    }

private slots:
    void onSaveClicked()
    {
        // Проверка обязательных полей
        if (rusNameEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Русское название' обязательно");
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

        // Проверка русского названия
        QString rusText = rusNameEdit->text();
        QRegularExpression rusRegex("^[А-Я][а-я]*$");
        if (!rusRegex.match(rusText).hasMatch()) {
            QMessageBox::warning(this, "Ошибка",
                                 "Русское название должно начинаться с заглавной буквы и содержать только русские буквы");
            return;
        }

        // Проверка английского названия (необязательное)
        if (!engNameEdit->text().isEmpty()) {
            QString engText = engNameEdit->text();
            QRegularExpression engRegex("^[A-Z][a-z]*$");
            if (!engRegex.match(engText).hasMatch()) {
                QMessageBox::warning(this, "Ошибка",
                                     "Английское название должно начинаться с заглавной буквы и содержать только английские буквы");
                return;
            }
        }

        // Проверка веса
        QString weightText = weightEdit->text();
        QRegularExpression weightRegex("^\\d+\\.\\d+$");
        if (!weightRegex.match(weightText).hasMatch()) {
            QMessageBox::warning(this, "Ошибка",
                                 "Вес должен быть дробным числом с точкой (например: 0.1, 3.14, 0.150)");
            return;
        }

        // Проверка размаха крыльев
        QString wingspanText = wingspanEdit->text();
        QRegularExpression wingspanRegex("^\\d+-\\d+$");
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

        // Проверка что выбран вариант "Возможность летать"
        if (!flyYes->isChecked() && !flyNo->isChecked()) {
            QMessageBox::warning(this, "Ошибка",
                                 "Выберите возможность летать (Да/Нет)");
            return;
        }

        // Сохраняем в файл
        QFile file("result.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
            QTextStream out(&file);
            out << "Русское название: " << rusNameEdit->text() << "\n";

            if (!engNameEdit->text().isEmpty()) {
                out << "Английское название: " << engNameEdit->text() << "\n";
            } else {
                out << "Английское название: (не указано)\n";
            }

            out << "Вес: " << weightEdit->text() << "\n";
            out << "Размах крыльев: " << wingspanEdit->text() << "\n";
            out << "Возможность летать: " << (flyYes->isChecked() ? "Да" : "Нет") << "\n";

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

        // Сбрасываем радиокнопки
        flyButtonGroup->setExclusive(false);
        flyYes->setChecked(false);
        flyNo->setChecked(false);
        flyButtonGroup->setExclusive(true);

        // Обнуляем запомненную кнопку
        lastChecked = nullptr;

        // Сбрасываем чекбоксы
        feature1->setChecked(false);
        feature2->setChecked(false);
        feature3->setChecked(false);
    }

private:
    QLineEdit *rusNameEdit;
    QLineEdit *engNameEdit;
    QLineEdit *weightEdit;
    QLineEdit *wingspanEdit;

    QRadioButton *flyYes;
    QRadioButton *flyNo;
    QButtonGroup *flyButtonGroup;
    QRadioButton *lastChecked;  // Запоминаем последнюю выбранную кнопку

    QCheckBox *feature1;
    QCheckBox *feature2;
    QCheckBox *feature3;

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
