namespace Lab3.Pages;
using Lab3.Entities;
using Lab3.Services;
using System.Collections.ObjectModel;

public partial class SQLitePage : ContentPage
{

    private readonly IDbService _sqLiteService;
    private List<Author> _authors;
    private List<Book> _books;

    public SQLitePage(IDbService dbService)
	{
		InitializeComponent();

        _sqLiteService = dbService;
        _sqLiteService.Init();
    }

    private void LoadAuthors(object sender, EventArgs e) 
    {
        _authors = new List<Author>(_sqLiteService.GetAllAuthors());
        authorPicker.ItemsSource = _authors;
    }

    private void OnAuthorSelected(object sender, EventArgs e)
    {
        var selectedAuthor = (Author)authorPicker.SelectedItem;

        if (selectedAuthor != null)
        {
            _books =  new List<Book>(_sqLiteService.GetAuthorBooks(selectedAuthor.Id));
            booksCollectionView.ItemsSource = _books;
        }
    }
}