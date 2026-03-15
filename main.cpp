#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Вариант 5 — Птицы");
        resize(400, 300);

        // Создаём центральный виджет и макет
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        // Поле 1: Название (Р)
        layout->addWidget(new QLabel("Название (Р):"));
        nameRusEdit = new QLineEdit;
        layout->addWidget(nameRusEdit);

        // Поле 2: Название (Л)
        layout->addWidget(new QLabel("Название (Л):"));
        nameEngEdit = new QLineEdit;
        layout->addWidget(nameEngEdit);

        // Поле 3: Вес
        layout->addWidget(new QLabel("Вес:"));
        weightEdit = new QLineEdit;
        layout->addWidget(weightEdit);

        // Поле 4: Размах крыльев
        layout->addWidget(new QLabel("Размах крыльев:"));
        wingspanEdit = new QLineEdit;
        layout->addWidget(wingspanEdit);

        // Поле 5: Может летать
        layout->addWidget(new QLabel("Может летать:"));
        canFlyCombo = new QComboBox;
        canFlyCombo->addItems({"да", "нет"});
        layout->addWidget(canFlyCombo);

        // Поле 6: Особенности
        layout->addWidget(new QLabel("Особенности:"));
        featuresCombo = new QComboBox;
        featuresCombo->addItems({"перелётные", "водоплавающие", "домашние"});
        layout->addWidget(featuresCombo);

        // Кнопки
        QHBoxLayout *buttonLayout = new QHBoxLayout;
        QPushButton *saveButton = new QPushButton("Сохранить");
        QPushButton *resetButton = new QPushButton("Сброс");
        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(resetButton);
        layout->addLayout(buttonLayout);

        setCentralWidget(centralWidget);

        // Подключаем сигналы
        connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSave);
        connect(resetButton, &QPushButton::clicked, this, &MainWindow::onReset);
    }

private slots:
    void onSave() {
        if (!validateForm()) return;

        // Собираем данные
        QString data =
            "Название (Р): " + nameRusEdit->text() + "\n" +
            "Название (Л): " + nameEngEdit->text() + "\n" +
            "Вес: " + weightEdit->text() + "\n" +
            "Размах крыльев: " + wingspanEdit->text() + "\n" +
            "Может летать: " + canFlyCombo->currentText() + "\n" +
            "Особенности: " + featuresCombo->currentText() + "\n" +
            "--------------------\n";

        // Сохраняем в файл
        QFile file("result.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
            QTextStream out(&file);
            out << data;
            file.close();
            QMessageBox::information(this, "Успех", "Данные сохранены!");
            onReset();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл!");
        }
    }

    void onReset() {
        nameRusEdit->clear();
        nameEngEdit->clear();
        weightEdit->clear();
        wingspanEdit->clear();
        canFlyCombo->setCurrentIndex(0);
        featuresCombo->setCurrentIndex(0);
    }

private:
    bool validateForm() {
        // Проверка русского названия: заглавная первая буква, далее строчные, слова через пробел
        if (!QRegularExpression(R"(^[А-ЯЁ][а-яё]*(\s[А-ЯЁ][а-яё]*)*$)").match(nameRusEdit->text()).hasMatch()) {
            showError("Название (Р): каждое слово должно начинаться с заглавной буквы");
            return false;
        }

        // Проверка английского названия: аналогично русскому, но латиница
        if (!QRegularExpression(R"(^[A-Z][a-z]*(\s[A-Z][a-z]*)*$)").match(nameEngEdit->text()).hasMatch()) {
            showError("Название (Л): каждое слово должно начинаться с заглавной буквы");
            return false;
        }

        // Проверка веса: дробное число, 1–3 знака после точки
        if (!QRegularExpression(R"(^\d+\.\d{1,3}$)").match(weightEdit->text()).hasMatch()) {
            showError("Вес: введите дробное число (например, 0.15)");
            return false;
        }

        // Проверка размаха крыльев: два числа через дефис, первое меньше второго
        QRegularExpression regex(R"(^(\d{1,3})-(\d{1,3})$)");
        QRegularExpressionMatch match = regex.match(wingspanEdit->text());
        if (!match.hasMatch()) {
            showError("Размах крыльев: формат «min-max» (например, 120-160)");
            return false;
        }
        int min = match.captured(1).toInt();
        int max = match.captured(2).toInt();
        if (min >= max) {
            showError("Размах крыльев: первое число должно быть меньше второго");
            return false;
        }
        if (min <= 0 || max > 500) {
            showError("Размах крыльев: значения должны быть в диапазоне 1–500 см");
            return false;
        }

        return true;
    }

    void showError(const QString& message) {
        QMessageBox::warning(this, "Ошибка ввода", message);
    }

    QLineEdit *nameRusEdit;
    QLineEdit *nameEngEdit;
    QLineEdit *weightEdit;
    QLineEdit *wingspanEdit;
    QComboBox *canFlyCombo;
    QComboBox *featuresCombo;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
