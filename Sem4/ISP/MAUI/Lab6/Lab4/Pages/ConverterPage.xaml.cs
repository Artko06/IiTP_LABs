using Lab4.Entities.Http;
using Lab4.Services.Http;
using System.Collections.ObjectModel;

namespace Lab4.Pages;

public partial class ConverterPage : ContentPage
{
    private ObservableCollection<Rate> rates;
    private ObservableCollection<Currency> currencies;
    private IRateService rateService;

    public ConverterPage(IRateService rateService)
    {
        InitializeComponent();

        this.rateService = rateService;

        rates = new ObservableCollection<Rate>();
        currencies = new ObservableCollection<Currency>();
        LoadCurrencies();

        RatesListView.ItemsSource = rates;
        CurrencyPicker.ItemsSource = currencies;

        DatePicker.Date = DateTime.Today;
        DatePicker.MaximumDate = DateTime.Today;
    }

    private async void OnDateSelected(object sender, DateChangedEventArgs e)
    {
        DateTime newDate = e.NewDate;
        var ratesForDate = await rateService.GetRates(newDate);
        rates.Clear();
        foreach (var rate in ratesForDate)
        {
            rates.Add(rate);
        }
    }


    private async void LoadCurrencies()
    {
        var currencyList = new List<Currency>
            {
                new Currency { Cur_Abbreviation = "RUB", Cur_Name = "Russian Ruble" },
                new Currency { Cur_Abbreviation = "EUR", Cur_Name = "Euro" },
                new Currency { Cur_Abbreviation = "USD", Cur_Name = "US Dollar" },
                new Currency { Cur_Abbreviation = "CHF", Cur_Name = "Swiss Franc" },
                new Currency { Cur_Abbreviation = "CNY", Cur_Name = "Chinese Yuan" },
                new Currency { Cur_Abbreviation = "GBP", Cur_Name = "British Pound Sterling" }
            };

        foreach (var currency in currencyList)
        {
            currencies.Add(currency);
        }
        var ratesForDate = await rateService.GetRates(DatePicker.Date);
        rates.Clear();
        foreach (var rate in ratesForDate)
        {
            rates.Add(rate);
        }
    }

    private void ConvertCurrency(decimal amount, Rate selectedRate)
    {
        if (selectedRate != null)
        {
            decimal convertedAmount = amount * (decimal)selectedRate.Cur_OfficialRate / selectedRate.Cur_Scale;
            ResultLabel.Text = $"{amount} {selectedRate.Cur_Abbreviation} = {convertedAmount} BYN";
        }
        else
        {
            ResultLabel.Text = "Invalid currency rate.";
        }
    }

    private async void OnConvertClicked(object sender, EventArgs e)
    {
        if (CurrencyPicker.SelectedItem != null && !string.IsNullOrEmpty(ValueEntry.Text))
        {
            DateTime selectedDate = DatePicker.Date;
            Currency selectedCurrency = (Currency)CurrencyPicker.SelectedItem;
            decimal? amount = null;

            if(decimal.TryParse(ValueEntry.Text, out decimal resualt))
            {
                amount = resualt;
            }

            if(amount.HasValue)
            {
                var ratesForDate = await rateService.GetRates(selectedDate);

                Rate selectedRate = ratesForDate.FirstOrDefault(rate => rate.Cur_Abbreviation == selectedCurrency.Cur_Abbreviation);

                ConvertCurrency(amount.Value, selectedRate);
            }
        }
    }
}