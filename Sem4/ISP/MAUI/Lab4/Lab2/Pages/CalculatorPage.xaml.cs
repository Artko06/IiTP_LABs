namespace Lab2.Pages;

public partial class CalculatorPage : ContentPage
{

    private string _currentInput = "0";  // Текущий ввод
    private string _operation = "";     // Последняя выбранная операция
    private double _firstOperand = 0;   // Первый операнд

    public CalculatorPage()
    {
        InitializeComponent();
    }

    // Обработка цифровых кнопок
    private void NumericButton_Clicked(object sender, EventArgs e)
    {
        var button = sender as Button;
        var number = button?.Text;


        if (_currentInput == "0" && number != ".")
            _currentInput = number!;
        else
            _currentInput += number;

        UpdateResultLabel();

        if (_operation != "")
        {
            OperationLabel.Text = $"{_firstOperand} {_operation} {ResultLabel.Text}";
        }
    }

    // Обработка кнопки "C" (сброс)
    private void ClearButton_Clicked(object sender, EventArgs e)
    {
        _currentInput = "0";
        _operation = "";
        _firstOperand = 0;
        UpdateResultLabel();
        OperationLabel.Text = "";
    }

    // Обработка кнопки "←" (удаление последнего символа)
    private void BackspaceButton_Clicked(object sender, EventArgs e)
    {
        if (_currentInput.Length > 1)
            _currentInput = _currentInput.Substring(0, _currentInput.Length - 1);
        else
            _currentInput = "0";

        UpdateResultLabel();
    }

    // Обработка кнопки "."
    private void PointButton_Clicked(object sender, EventArgs e)
    {
        if (!_currentInput.Contains("."))
            _currentInput += ".";
        UpdateResultLabel();
    }

    // Обработка кнопок операций (+, -, ×, ÷)
    private void PlusButton_Clicked(object sender, EventArgs e) => HandleOperator("+");
    private void DifferenceButton_Clicked(object sender, EventArgs e) => HandleOperator("-");
    private void MultiplicationButton_Clicked(object sender, EventArgs e) => HandleOperator("×");
    private void DivideButton_Clicked(object sender, EventArgs e) => HandleOperator("÷");

    private void HandleOperator(string operation)
    {
        if (double.TryParse(_currentInput, out var number))
        {
            if (_operation == "")
            {
                _operation = operation;
                _firstOperand = number;
                _currentInput = "0";
                OperationLabel.Text = $"{_firstOperand} {_operation}";
            }

            else
            {
                equal();
                HandleOperator(operation);
            }
        }

        UpdateResultLabel();
    }

    // Обработка кнопки "="
    private void EqualButton_Clicked(object sender, EventArgs e)
    {
        equal();
    }

    // Обработка кнопки "±"
    private void SignButton_Clicked(object sender, EventArgs e)
    {
        if (double.TryParse(_currentInput, out var number))
        {
            _currentInput = (-number).ToString();
            UpdateResultLabel();
            UpdateOperationLabel();
        }
    }

    // Обработка кнопки "%"
    private void PercentButton_Clicked(object sender, EventArgs e)
    {
        if (double.TryParse(_currentInput, out var number))
        {
            _currentInput = (number / 100).ToString();
            UpdateResultLabel();
            UpdateOperationLabel();
        }
    }

    // Обработка кнопки "√"
    private void SqrtButton_Clicked(object sender, EventArgs e)
    {
        if (double.TryParse(_currentInput, out var number))
        {
            _currentInput = Math.Sqrt(number).ToString();
            UpdateResultLabel();
            UpdateOperationLabel();
        }
    }

    // Обработка кнопки "x²"
    private void SquereButton_Clicked(object sender, EventArgs e)
    {
        if (double.TryParse(_currentInput, out var number))
        {
            _currentInput = Math.Pow(number, 2).ToString();
            UpdateResultLabel();
            UpdateOperationLabel();
        }
    }

    // Обработка кнопки "1/x"
    private void InverseButton_Clicked(object sender, EventArgs e)
    {
        if (double.TryParse(_currentInput, out var number) && number != 0)
        {
            _currentInput = (1 / number).ToString();
            UpdateResultLabel();
            UpdateOperationLabel();
        }
    }

    // Обработка кнопки "|x|"
    private void ModulButton_Clicked(object sender, EventArgs e)
    {
        if (double.TryParse(_currentInput, out var number))
        {
            _currentInput = Math.Abs(number).ToString();
            UpdateResultLabel();
            UpdateOperationLabel();
        }
    }

    // Обновление отображения результата
    private void UpdateResultLabel()
    {
        ResultLabel.Text = _currentInput;
    }

    private void UpdateOperationLabel()
    {
        if (_operation != "")
        {
            OperationLabel.Text = $"{_firstOperand} {_operation} {ResultLabel.Text}";
        }
    }

    private void equal()
    {
        if (double.TryParse(_currentInput, out var secondOperand))
        {
            double result = _firstOperand;

            switch (_operation)
            {
                case "+":
                    result += secondOperand;
                    break;
                case "-":
                    result -= secondOperand;
                    break;
                case "×":
                    result *= secondOperand;
                    break;
                case "÷":
                    result = secondOperand == 0 ? 0 : result / secondOperand;
                    break;
            }

            _currentInput = result.ToString();
            OperationLabel.Text = "";
            _operation = "";
            _firstOperand = 0;

            UpdateResultLabel();
        }
    }
}