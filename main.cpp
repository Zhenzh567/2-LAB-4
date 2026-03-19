#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupValidators();
    onResetClicked();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Создаем форму
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
    
    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    saveButton = new QPushButton("Сохранить");
    resetButton = new QPushButton("Сброс");
    
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(resetButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Подключаем сигналы
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    
    setWindowTitle("Лабораторная работа 4 - Вариант 5");
    setFixedSize(450, 300);
}

void MainWindow::setupValidators()
{
    // 1. Русский текст: только первая буква заглавная
    QRegularExpression rusRegex("^[А-Я][а-я]*$");
    rusNameValidator = new QRegularExpressionValidator(rusRegex, this);
    rusNameEdit->setValidator(rusNameValidator);
    
    // 2. Английский текст: только первая буква заглавная
    QRegularExpression engRegex("^[A-Z][a-z]*$");
    engNameValidator = new QRegularExpressionValidator(engRegex, this);
    engNameEdit->setValidator(engNameValidator);
    
    // 3. Дробное число с точкой (любое количество знаков после запятой)
    QRegularExpression numberRegex("^\\d+\\.\\d+$");
    numberValidator = new QRegularExpressionValidator(numberRegex, this);
    numberEdit->setValidator(numberValidator);
    
    // 4. Диапазон (120-160, первое меньше второго)
    QRegularExpression rangeRegex("^\\d{1,3}-\\d{1,3}$");
    rangeValidator = new QRegularExpressionValidator(rangeRegex, this);
    rangeEdit->setValidator(rangeValidator);
}

bool MainWindow::validateFields()
{
    // Проверка обязательных полей
    if (rusNameEdit->text().isEmpty()) {
        showError("Поле 'Русское название' обязательно для заполнения");
        return false;
    }
    
    if (engNameEdit->text().isEmpty()) {
        showError("Поле 'Английское название' обязательно для заполнения");
        return false;
    }
    
    if (numberEdit->text().isEmpty()) {
        showError("Поле 'Число' обязательно для заполнения");
        return false;
    }
    
    if (rangeEdit->text().isEmpty()) {
        showError("Поле 'Диапазон' обязательно для заполнения");
        return false;
    }
    
    // Проверка соответствия регулярным выражениям
    int pos = 0;
    QString text;
    
    text = rusNameEdit->text();
    if (rusNameValidator->validate(text, pos) != QValidator::Acceptable) {
        showError("Поле 'Русское название' должно содержать только русские буквы, первая заглавная");
        return false;
    }
    
    text = engNameEdit->text();
    if (engNameValidator->validate(text, pos) != QValidator::Acceptable) {
        showError("Поле 'Английское название' должно содержать только английские буквы, первая заглавная");
        return false;
    }
    
    text = numberEdit->text();
    if (numberValidator->validate(text, pos) != QValidator::Acceptable) {
        showError("Поле 'Число' должно быть дробным числом с точкой (например: 0.1, 3.14, 0.150)");
        return false;
    }
    
    text = rangeEdit->text();
    if (rangeValidator->validate(text, pos) != QValidator::Acceptable) {
        showError("Поле 'Диапазон' должно быть в формате: число-число (например, 120-160)");
        return false;
    }
    
    // Дополнительная проверка: первое число меньше второго
    QStringList rangeParts = rangeEdit->text().split('-');
    if (rangeParts.size() == 2) {
        int first = rangeParts[0].toInt();
        int second = rangeParts[1].toInt();
        if (first >= second) {
            showError("В поле 'Диапазон' первое число должно быть меньше второго");
            return false;
        }
    }
    
    return true;
}

void MainWindow::showError(const QString &message)
{
    QMessageBox::warning(this, "Ошибка ввода", message);
}

void MainWindow::onSaveClicked()
{
    if (!validateFields()) {
        return;
    }
    
    PersonData data(
        rusNameEdit->text(),
        engNameEdit->text(),
        numberEdit->text().toDouble(),
        rangeEdit->text(),
        featuresEdit->text(),
        latinNameEdit->text()
    );
    
    saveToFile(data);
    
    QMessageBox::information(this, "Успех", "Данные успешно сохранены в файл result.txt");
}

void MainWindow::onResetClicked()
{
    rusNameEdit->clear();
    engNameEdit->clear();
    numberEdit->clear();
    rangeEdit->clear();
    featuresEdit->clear();
    latinNameEdit->clear();
}

void MainWindow::saveToFile(const PersonData &data)
{
    QFile file("result.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        QTextStream out(&file);
        out << data.toString() << "\n";
        file.close();
    }
}
