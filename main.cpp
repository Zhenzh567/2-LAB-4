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
#include <QRegularExpressionValidator>
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
        numberEdit = new QLineEdit();
        rangeEdit = new QLineEdit();
        featuresEdit = new QLineEdit();
        latinNameEdit = new QLineEdit();
        
        formLayout->addRow("Русское название (только первая буква капсом):", rusNameEdit);
        formLayout->addRow("Английское название (только первая буква капсом):", engNameEdit);
        formLayout->addRow("Число (дробное с точкой, например 0.150):", numberEdit);
        formLayout->addRow("Диапазон (120-160, первое меньше второго):", rangeEdit);
        formLayout->addRow("Особенности (необязательно):", featuresEdit);
        formLayout->addRow("Латинское название (необязательно):", latinNameEdit);
        
        mainLayout->addLayout(formLayout);
        
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        saveButton = new QPushButton("Сохранить");
        resetButton = new QPushButton("Сброс");
        
        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(resetButton);
        
        mainLayout->addLayout(buttonLayout);
        
        // Настраиваем валидаторы
        QRegularExpression rusRegex("^[А-Я][а-я]*$");
        rusNameEdit->setValidator(new QRegularExpressionValidator(rusRegex, this));
        
        QRegularExpression engRegex("^[A-Z][a-z]*$");
        engNameEdit->setValidator(new QRegularExpressionValidator(engRegex, this));
        
        QRegularExpression numberRegex("^\\d+\\.\\d+$");
        numberEdit->setValidator(new QRegularExpressionValidator(numberRegex, this));
        
        QRegularExpression rangeRegex("^\\d{1,3}-\\d{1,3}$");
        rangeEdit->setValidator(new QRegularExpressionValidator(rangeRegex, this));
        
        // Подключаем кнопки
        connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
        connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
        
        // Очищаем поля
        onResetClicked();
        
        setWindowTitle("Лабораторная работа 4 - Вариант 5");
        setFixedSize(450, 300);
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
        
        if (numberEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Число' обязательно");
            return;
        }
        
        if (rangeEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Диапазон' обязательно");
            return;
        }
        
        // Проверка валидаторов
        int pos = 0;
        QString text;
        
        text = rusNameEdit->text();
        if (rusNameEdit->validator()->validate(text, pos) != QValidator::Acceptable) {
            QMessageBox::warning(this, "Ошибка", "Русское название: только русские буквы, первая заглавная");
            return;
        }
        
        text = engNameEdit->text();
        if (engNameEdit->validator()->validate(text, pos) != QValidator::Acceptable) {
            QMessageBox::warning(this, "Ошибка", "Английское название: только английские буквы, первая заглавная");
            return;
        }
        
        text = numberEdit->text();
        if (numberEdit->validator()->validate(text, pos) != QValidator::Acceptable) {
            QMessageBox::warning(this, "Ошибка", "Число должно быть дробным с точкой (например: 0.1, 3.14, 0.150)");
            return;
        }
        
        text = rangeEdit->text();
        if (rangeEdit->validator()->validate(text, pos) != QValidator::Acceptable) {
            QMessageBox::warning(this, "Ошибка", "Диапазон должен быть в формате: число-число");
            return;
        }
        
        // Проверка диапазона (первое число меньше второго)
        QStringList rangeParts = rangeEdit->text().split('-');
        if (rangeParts.size() == 2) {
            int first = rangeParts[0].toInt();
            int second = rangeParts[1].toInt();
            if (first >= second) {
                QMessageBox::warning(this, "Ошибка", "В диапазоне первое число должно быть меньше второго");
                return;
            }
        }
        
        // Сохраняем в файл
        QFile file("result.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
            QTextStream out(&file);
            out << "Русское название: " << rusNameEdit->text() << "\n";
            out << "Английское название: " << engNameEdit->text() << "\n";
            out << "Число: " << numberEdit->text() << "\n";
            out << "Диапазон: " << rangeEdit->text() << "\n";
            out << "Особенности: " << (featuresEdit->text().isEmpty() ? "(не указано)" : featuresEdit->text()) << "\n";
            out << "Латинское название: " << (latinNameEdit->text().isEmpty() ? "(не указано)" : latinNameEdit->text()) << "\n";
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
        numberEdit->clear();
        rangeEdit->clear();
        featuresEdit->clear();
        latinNameEdit->clear();
    }

private:
    QLineEdit *rusNameEdit;
    QLineEdit *engNameEdit;
    QLineEdit *numberEdit;
    QLineEdit *rangeEdit;
    QLineEdit *featuresEdit;
    QLineEdit *latinNameEdit;
    
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

