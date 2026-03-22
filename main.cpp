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

        // Создаем сетку для полей
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

        // Добавляем в сетку
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

        // Возможность летать (да/нет) - радиокнопки с возможностью снятия
        QGroupBox *flyGroup = new QGroupBox("Возможность летать:");
        QHBoxLayout *flyLayout = new QHBoxLayout();

        flyYes = new QRadioButton("Да");
        flyNo = new QRadioButton("Нет");

        // Отключаем автоматическое исключение
        flyYes->setAutoExclusive(false);
        flyNo->setAutoExclusive(false);

        // Убираем галочку изначально
        flyYes->setChecked(false);
        flyNo->setChecked(false);

        // Логика: если кликнуть по выбранной - снимается, если по невыбранной - выбирается
        connect(flyYes, &QRadioButton::clicked, [this]() {
            if (flyYes->isChecked()) {
                // Если уже была выбрана, то снимаем
                flyYes->setChecked(false);
            } else {
                // Если не была выбрана, то выбираем и снимаем другую
                flyYes->setChecked(true);
                flyNo->setChecked(false);
            }
        });

        connect(flyNo, &QRadioButton::clicked, [this]() {
            if (flyNo->isChecked()) {
                // Если уже была выбрана, то снимаем
                flyNo->setChecked(false);
            } else {
                // Если не была выбрана, то выбираем и снимаем другую
                flyNo->setChecked(true);
                flyYes->setChecked(false);
            }
        });

        flyLayout->addWidget(flyYes);
        flyLayout->addWidget(flyNo);
        flyGroup->setLayout(flyLayout);

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

        optionsLayout->addWidget(flyGroup);
        optionsLayout->addWidget(featuresGroup);

        mainLayout->addLayout(optionsLayout);

        // Кнопки
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        saveButton = new QPushButton("Сохранить");
        resetButton = new QPushButton("Сброс");

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

        // Проверка русского названия
        QString rusText = rusNameEdit->text();
        QRegularExpression rusRegex("^[А-Я][а-я]*$");
        if (!rusRegex.match(rusText).hasMatch()) {
            QMessageBox::warning(this, "Ошибка",
                                 "Русское название должно начинаться с заглавной буквы и содержать только русские буквы");
            return;
        }

        // Проверка английского названия
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
    QLineEdit *wingspanEdit;

    QRadioButton *flyYes;   // Радиокнопки
    QRadioButton *flyNo;

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

        // Создаем сетку для полей
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

        // Добавляем в сетку
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

        // Возможность летать (да/нет) - радиокнопки с возможностью снятия
        QGroupBox *flyGroup = new QGroupBox("Возможность летать:");
        QHBoxLayout *flyLayout = new QHBoxLayout();

        flyYes = new QRadioButton("Да");
        flyNo = new QRadioButton("Нет");

        // Отключаем автоматическое исключение
        flyYes->setAutoExclusive(false);
        flyNo->setAutoExclusive(false);

        // Убираем галочку изначально
        flyYes->setChecked(false);
        flyNo->setChecked(false);

        // Сохраняем указатели
        QRadioButton *yes = flyYes;
        QRadioButton *no = flyNo;

        // Логика кликов
        connect(flyYes, &QRadioButton::clicked, [yes, no]() {
            if (yes->isChecked()) {
                yes->setChecked(false);
            } else {
                yes->setChecked(true);
                no->setChecked(false);
            }
        });

        connect(flyNo, &QRadioButton::clicked, [yes, no]() {
            if (no->isChecked()) {
                no->setChecked(false);
            } else {
                no->setChecked(true);
                yes->setChecked(false);
            }
        });

        flyLayout->addWidget(flyYes);
        flyLayout->addWidget(flyNo);
        flyGroup->setLayout(flyLayout);

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

        optionsLayout->addWidget(flyGroup);
        optionsLayout->addWidget(featuresGroup);

        mainLayout->addLayout(optionsLayout);

        // Кнопки
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        saveButton = new QPushButton("Сохранить");
        resetButton = new QPushButton("Сброс");

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

        // Проверка русского названия
        QString rusText = rusNameEdit->text();
        QRegularExpression rusRegex("^[А-Я][а-я]*$");
        if (!rusRegex.match(rusText).hasMatch()) {
            QMessageBox::warning(this, "Ошибка",
                                 "Русское название должно начинаться с заглавной буквы и содержать только русские буквы");
            return;
        }

        // Проверка английского названия
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
    QLineEdit *wingspanEdit;

    QRadioButton *flyYes;
    QRadioButton *flyNo;

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
#include <QFile>
#include <QTextStream>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent)
    {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        QGridLayout *gridLayout = new QGridLayout();

        QLabel *rusLabel = new QLabel("Русское название:");
        QLabel *engLabel = new QLabel("Английское название:");
        QLabel *weightLabel = new QLabel("Вес (дробное число с точкой):");
        QLabel *wingspanLabel = new QLabel("Размах крыльев:");

        rusNameEdit = new QLineEdit();
        engNameEdit = new QLineEdit();
        weightEdit = new QLineEdit();
        wingspanEdit = new QLineEdit();
        weightEdit->setMaximumWidth(100);

        gridLayout->addWidget(rusLabel, 0, 2);
        gridLayout->addWidget(rusNameEdit, 0, 3);

        gridLayout->addWidget(engLabel, 1, 0);
        gridLayout->addWidget(engNameEdit, 1, 1);

        gridLayout->addWidget(weightLabel, 2, 2);
        gridLayout->addWidget(weightEdit, 2, 3);

        gridLayout->addWidget(wingspanLabel, 3, 0);
        gridLayout->addWidget(wingspanEdit, 3, 1);

        mainLayout->addLayout(gridLayout);

        QHBoxLayout *optionsLayout = new QHBoxLayout();

        // Возможность летать (да/нет) - радиокнопки которые можно снять
        QGroupBox *flyGroup = new QGroupBox("Возможность летать:");
        QHBoxLayout *flyLayout = new QHBoxLayout();

        flyYes = new QRadioButton("Да");
        flyNo = new QRadioButton("Нет");

        // Запоминаем состояние до клика
        connect(flyYes, &QRadioButton::clicked, [this]() {
            if (flyYes->isChecked()) {
                // Если кнопка стала выбранной, проверяем была ли она уже выбрана
                // Для этого используем таймер, чтобы проверить состояние после обработки клика
                QTimer::singleShot(0, [this]() {
                    if (flyYes->isChecked()) {
                        // Если все еще выбрана, значит это был повторный клик - снимаем
                        flyYes->setAutoExclusive(false);
                        flyYes->setChecked(false);
                        flyYes->setAutoExclusive(true);
                    }
                });
            }
        });

        connect(flyNo, &QRadioButton::clicked, [this]() {
            if (flyNo->isChecked()) {
                QTimer::singleShot(0, [this]() {
                    if (flyNo->isChecked()) {
                        flyNo->setAutoExclusive(false);
                        flyNo->setChecked(false);
                        flyNo->setAutoExclusive(true);
                    }
                });
            }
        });

        flyLayout->addWidget(flyYes);
        flyLayout->addWidget(flyNo);
        flyGroup->setLayout(flyLayout);

        // Группа "Особенности"
        QGroupBox *featuresGroup = new QGroupBox("Особенности:");
        QVBoxLayout *featuresLayout = new QVBoxLayout();

        feature1 = new QCheckBox("Перелетные");
        feature2 = new QCheckBox("Водоплавающие");
        feature3 = new QCheckBox("Домашние");

        featuresLayout->addWidget(feature1);
        featuresLayout->addWidget(feature2);
        featuresLayout->addWidget(feature3);

        featuresGroup->setLayout(featuresLayout);

        optionsLayout->addWidget(flyGroup);
        optionsLayout->addWidget(featuresGroup);

        mainLayout->addLayout(optionsLayout);

        // Кнопки
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        saveButton = new QPushButton("Сохранить");
        resetButton = new QPushButton("Сброс");

        saveButton->setStyleSheet("background-color: green; color: white; font-weight: bold;");
        resetButton->setStyleSheet("background-color: red; color: white; font-weight: bold;");

        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(resetButton);

        mainLayout->addLayout(buttonLayout);

        connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
        connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);

        setWindowTitle("Птицы");
        setMinimumSize(600, 350);
    }

private slots:
    void onSaveClicked()
    {
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

        QString rusText = rusNameEdit->text();
        QRegularExpression rusRegex("^[А-Я][а-я]*$");
        if (!rusRegex.match(rusText).hasMatch()) {
            QMessageBox::warning(this, "Ошибка",
                                 "Русское название должно начинаться с заглавной буквы и содержать только русские буквы");
            return;
        }

        if (!engNameEdit->text().isEmpty()) {
            QString engText = engNameEdit->text();
            QRegularExpression engRegex("^[A-Z][a-z]*$");
            if (!engRegex.match(engText).hasMatch()) {
                QMessageBox::warning(this, "Ошибка",
                                     "Английское название должно начинаться с заглавной буквы и содержать только английские буквы");
                return;
            }
        }

        QString weightText = weightEdit->text();
        QRegularExpression weightRegex("^\\d+\\.\\d+$");
        if (!weightRegex.match(weightText).hasMatch()) {
            QMessageBox::warning(this, "Ошибка",
                                 "Вес должен быть дробным числом с точкой (например: 0.1, 3.14, 0.150)");
            return;
        }

        QString wingspanText = wingspanEdit->text();
        QRegularExpression wingspanRegex("^\\d+-\\d+$");
        if (!wingspanRegex.match(wingspanText).hasMatch()) {
            QMessageBox::warning(this, "Ошибка",
                                 "Размах крыльев должен быть в формате: число-число (например, 120-160)");
            return;
        }

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

        if (!flyYes->isChecked() && !flyNo->isChecked()) {
            QMessageBox::warning(this, "Ошибка",
                                 "Выберите возможность летать (Да/Нет)");
            return;
        }

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
        flyYes->setAutoExclusive(false);
        flyNo->setAutoExclusive(false);
        flyYes->setChecked(false);
        flyNo->setChecked(false);
        flyYes->setAutoExclusive(true);
        flyNo->setAutoExclusive(true);

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
#include <QTimer>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent)
    {
        // Надписи и поля ввода
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
        flyGroup->setGeometry(50, 200, 400, 80);
        
        flyYes = new QRadioButton("Да", flyGroup);
        flyYes->setGeometry(20, 30, 80, 25);
        
        flyNo = new QRadioButton("Нет", flyGroup);
        flyNo->setGeometry(120, 30, 80, 25);
        
        // Группа для радиокнопок (обеспечивает выбор только одной)
        flyButtonGroup = new QButtonGroup(this);
        flyButtonGroup->addButton(flyYes);
        flyButtonGroup->addButton(flyNo);
        
        // Логика для снятия галочки при повторном клике
        connect(flyYes, &QRadioButton::clicked, [this]() {
            if (flyYes->isChecked()) {
                QTimer::singleShot(0, [this]() {
                    if (flyYes->isChecked()) {
                        flyButtonGroup->setExclusive(false);
                        flyYes->setChecked(false);
                        flyButtonGroup->setExclusive(true);
                    }
                });
            }
        });
        
        connect(flyNo, &QRadioButton::clicked, [this]() {
            if (flyNo->isChecked()) {
                QTimer::singleShot(0, [this]() {
                    if (flyNo->isChecked()) {
                        flyButtonGroup->setExclusive(false);
                        flyNo->setChecked(false);
                        flyButtonGroup->setExclusive(true);
                    }
                });
            }
        });
        
        // Группа "Особенности"
        QGroupBox *featuresGroup = new QGroupBox("Особенности:", this);
        featuresGroup->setGeometry(50, 300, 400, 120);
        
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
        setFixedSize(600, 550);
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
        
        // Проверка английского названия
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
        // Очищаем поля
        rusNameEdit->clear();
        engNameEdit->clear();
        weightEdit->clear();
        wingspanEdit->clear();
        
        // Сбрасываем радиокнопки
        flyButtonGroup->setExclusive(false);
        flyYes->setChecked(false);
        flyNo->setChecked(false);
        flyButtonGroup->setExclusive(true);
        
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
#include <QTimer>

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
        
        // Группа для радиокнопок (для автоматического переключения)
        flyButtonGroup = new QButtonGroup(this);
        flyButtonGroup->addButton(flyYes);
        flyButtonGroup->addButton(flyNo);
        
        // Логика: повторный клик снимает галочку
        connect(flyYes, &QRadioButton::clicked, [this]() {
            QTimer::singleShot(0, [this]() {
                if (flyYes->isChecked()) {
                    flyButtonGroup->setExclusive(false);
                    flyYes->setChecked(false);
                    flyButtonGroup->setExclusive(true);
                }
            });
        });
        
        connect(flyNo, &QRadioButton::clicked, [this]() {
            QTimer::singleShot(0, [this]() {
                if (flyNo->isChecked()) {
                    flyButtonGroup->setExclusive(false);
                    flyNo->setChecked(false);
                    flyButtonGroup->setExclusive(true);
                }
            });
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
