#include <QApplication>
#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
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
        // Создаем основной вертикальный layout
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // Создаем сетку для полей (слева - названия, справа - поля ввода)
        QGridLayout *gridLayout = new QGridLayout();

        // Создаем поля
        QLabel *rusLabel = new QLabel("Русское название:");
        QLabel *engLabel = new QLabel("Английское название:");
        QLabel *weightLabel = new QLabel("Вес (дробное число с точкой):");
        QLabel *wingspanLabel = new QLabel("Размах крыльев:");

        rusNameEdit = new QLineEdit();
        engNameEdit = new QLineEdit();
        weightEdit = new QLineEdit();
        wingspanEdit = new QLineEdit();
        weightEdit->setMaximumWidth(100);

        // Добавляем в сетку (строка, колонка)
        gridLayout->addWidget(rusLabel, 0, 2);
        gridLayout->addWidget(rusNameEdit, 0, 3);

        gridLayout->addWidget(engLabel, 1, 0);
        gridLayout->addWidget(engNameEdit, 1, 1);

        gridLayout->addWidget(weightLabel, 2, 2);
        gridLayout->addWidget(weightEdit, 2, 3);

        gridLayout->addWidget(wingspanLabel, 3, 0);
        gridLayout->addWidget(wingspanEdit, 3, 1);

        mainLayout->addLayout(gridLayout);

        // Горизонтальный layout для радио-кнопок и чекбоксов
        QHBoxLayout *optionsLayout = new QHBoxLayout();

        // Возможность летать (да/нет)
        QGroupBox *flyGroupBox = new QGroupBox("Возможность летать:");
        QHBoxLayout *flyLayout = new QHBoxLayout();

        flyYes = new QRadioButton("Да");
        flyNo = new QRadioButton("Нет");

        // Создаем группу для радиокнопок
        flyGroup = new QButtonGroup(this);
        flyGroup->addButton(flyYes);
        flyGroup->addButton(flyNo);

        // Убираем галочку изначально
        flyYes->setChecked(false);
        flyNo->setChecked(false);

        // Добавляем возможность снимать галочку
        connect(flyYes, &QRadioButton::clicked, [this]() {
            if (flyYes->isChecked()) {
                flyYes->setChecked(false);
            }
        });

        connect(flyNo, &QRadioButton::clicked, [this]() {
            if (flyNo->isChecked()) {
                flyNo->setChecked(false);
            }
        });

        flyLayout->addWidget(flyYes);
        flyLayout->addWidget(flyNo);
        flyGroupBox->setLayout(flyLayout);

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

        optionsLayout->addWidget(flyGroupBox);
        optionsLayout->addWidget(featuresGroup);

        mainLayout->addLayout(optionsLayout);

        // Кнопки
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        saveButton = new QPushButton("Сохранить");
        resetButton = new QPushButton("Сброс");

        // Устанавливаем цвета кнопок
        saveButton->setStyleSheet("background-color: green; color: white; font-weight: bold;");
        resetButton->setStyleSheet("background-color: red; color: white; font-weight: bold;");

        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(resetButton);

        mainLayout->addLayout(buttonLayout);

        // Подключаем кнопки
        connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
        connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);

        // Очищаем поля
        onResetClicked();

        setWindowTitle("Птицы");
        setMinimumSize(600, 350);
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

        // Проверка русского названия (первая буква заглавная)
        QString rusText = rusNameEdit->text();
        QRegularExpression rusRegex("^[А-Я][а-я]*$");
        if (!rusRegex.match(rusText).hasMatch()) {
            QMessageBox::warning(this, "Ошибка",
                                 "Русское название должно начинаться с заглавной буквы и содержать только русские буквы");
            return;
        }

        // Проверка английского названия (только если поле не пустое)
        if (!engNameEdit->text().isEmpty()) {
            QString engText = engNameEdit->text();
            QRegularExpression engRegex("^[A-Z][a-z]*$");
            if (!engRegex.match(engText).hasMatch()) {
                QMessageBox::warning(this, "Ошибка",
                                     "Английское название должно начинаться с заглавной буквы и содержать только английские буквы");
                return;
            }
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

            // Английское название сохраняем только если оно есть
            if (!engNameEdit->text().isEmpty()) {
                out << "Английское название: " << engNameEdit->text() << "\n";
            } else {
                out << "Английское название: (не указано)\n";
            }

            out << "Вес: " << weightEdit->text() << "\n";
            out << "Размах крыльев: " << wingspanEdit->text() << "\n";

            // Возможность летать
            out << "Возможность летать: " << (flyYes->isChecked() ? "Да" : "Нет") << "\n";

            // Сохраняем особенности (только выбранные)
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

        // Сбрасываем радио-кнопки (убираем обе галочки)
        flyYes->setChecked(false);
        flyNo->setChecked(false);

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
    QButtonGroup *flyGroup;  // Группа для радиокнопок

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
