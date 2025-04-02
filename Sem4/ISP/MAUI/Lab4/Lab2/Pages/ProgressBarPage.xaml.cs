namespace Lab2.Pages;

public partial class ProgressBarPage : ContentPage
{
    private CancellationTokenSource Token;

    public ProgressBarPage()
	{
		InitializeComponent();
	}

    private async void StartButton_Clicked(object sender, EventArgs e)
    {
        Token?.Cancel();
        Token = new CancellationTokenSource();
        try
        {
            StatusLabel.Text = "Вычисление...";
            ProgressLabel.Text = "0%";
            double result = await Task.Run(() => CalculateIntegralAsync(Token.Token));

            StatusLabel.Text = $"Результат: {result:F8}";
            ProgressLabel.Text = "100%";
        }
        catch (OperationCanceledException)
        {
            StatusLabel.Text = "Вычисление отменено";
        }
    }

    private async Task<double> CalculateIntegralAsync(CancellationToken cancellationToken)
    {
        const short fromRange = 0;
        const short beforeRange = 1;
        const double step = 0.00000001;

        double progressBar;
        double result = 0;

        for (double x = fromRange; x <= beforeRange; x += step)
        {
            cancellationToken.ThrowIfCancellationRequested();

            result += Math.Sin(x - step * 0.5) * step; // Метод средних прямоугольников
            progressBar = (x - fromRange) / (beforeRange - fromRange) * 100;

            if ((int)(x / step) % 100000 == 0)
            {
                await Task.Yield(); // Освобождаем поток каждые 100000 шагов

                MainThread.BeginInvokeOnMainThread(() =>
                {
                    ProgressLabel.Text = $"{progressBar:F3}%";
                    ProgressBar.Progress = progressBar / 100;
                });

                //Dispatcher.Dispatch(() =>
                //{
                //    ProgressLabel.Text = $"{progressBar:F3}%";
                //    ProgressBar.Progress = progressBar / 100;
                //});
            }
        }

        return result;
    }

    private void CancelButton_Clicked(object sender, EventArgs e)
    {
        Token?.Cancel();
    }
}