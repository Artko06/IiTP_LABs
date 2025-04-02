using Lab4.Entities;
using SQLite;

namespace Lab4.Services
{
    public class SQLiteService : IDbService
    {

        private SQLiteConnection _db;
        private readonly string _dbPath = Path.Combine(FileSystem.AppDataDirectory, "DataBase.db");

        // Метод для сравнения двух списков на полное равенство
        private bool AreListsEqual<T>(List<T> list1, List<T> list2, Func<T, T, bool> equalityComparer)
        {
            if (list1.Count != list2.Count)
                return false;

            for (int i = 0; i < list1.Count; ++i)
            {
                if (!equalityComparer(list1[i], list2[i]))
                    return false;
            }

            return true;
        }

        private bool TableExists(string tableName)
        {
            var tableInfo = _db.GetTableInfo(tableName);
            return tableInfo.Any(); // Если таблица существует, вернется хотя бы одна строка
        }

        public IEnumerable<Author> GetAllAuthors() => _db.Table<Author>().ToList();

        public IEnumerable<Book> GetAuthorBooks(int id) =>
            from book in _db.Table<Book>()
            where book.AuthorId == id
            select book;

        public void Init()
        {
            //File.Delete(_dbPath);
            _db = new SQLiteConnection(_dbPath);

            // список авторов книг
            var authors = new List<Author>
            {
                    new() { Name = "Иван Мелeж" },
                    new() { Name = "Ян Борщевский" },
                    new() { Name = "Владимир Короткевич" },
                    new() { Name = "Кондрат Крапива" },
                    new() { Name = "Змирок Бядуля" },
                    new() { Name = "Янка Купала" },
                    new() { Name = "Якуб Колас" }
            };

            if (TableExists("Authors") && TableExists("Books"))
            {
                var existingAuthors = _db.Table<Author>().ToList();
                var existingBooks = _db.Table<Book>().ToList();
                bool authorWasChanged = false;

                if (!AreListsEqual(authors, existingAuthors, (a1, a2) => a1.Name == a2.Name))
                {
                    // Если списки не совпадают, очищаем таблицу авторов и добавляем новые данные
                    authorWasChanged = true;
                    _db.DeleteAll<Author>();
                    foreach (var author in authors)
                    {
                        _db.Insert(author);
                    }
                }

                // список книг (Нужно сначало вставить авторов, а потом определять книги, чтобы правильно определился Foreign Key)
                var books = new List<Book>
                {
                    new() { Title = "Люди на балоте", CountPages = 512, AuthorId = (authorWasChanged) ? authors[0].Id :existingAuthors[0].Id },
                    new() { Title = "Дыхание грозы", CountPages = 704, AuthorId = (authorWasChanged) ? authors[0].Id :existingAuthors[0].Id },
                    new() { Title = "Минское направление", CountPages = 759, AuthorId = (authorWasChanged) ? authors[0].Id :existingAuthors[0].Id },

                    new() { Title = "Шляхтич Завальня", CountPages = 352, AuthorId = (authorWasChanged) ? authors[1].Id :existingAuthors[1].Id },

                    new() { Title = "Дикая охота короля Стаха", CountPages = 280, AuthorId = (authorWasChanged) ? authors[2].Id :existingAuthors[2].Id },
                    new() { Title = "Колосья под серпом твоим", CountPages = 735, AuthorId = (authorWasChanged) ? authors[2].Id :existingAuthors[2].Id },
                    new() { Title = "Земля под белыми крыльями", CountPages = 192, AuthorId = (authorWasChanged) ? authors[2].Id :existingAuthors[2].Id },

                    new() { Title = "Кто смеётся последним...", CountPages = 188, AuthorId = (authorWasChanged) ? authors[3].Id :existingAuthors[3].Id },
                    new() { Title = "Дипломированный Баран", CountPages = 136, AuthorId = (authorWasChanged) ? authors[3].Id :existingAuthors[3].Id },
                    new() { Title = "Мой сосед", CountPages = 146, AuthorId = (authorWasChanged) ? authors[3].Id :existingAuthors[3].Id },

                    new() { Title = "Бондарь", CountPages = 12, AuthorId = (authorWasChanged) ? authors[4].Id :existingAuthors[4].Id },
                    new() { Title = "Соловей", CountPages = 157, AuthorId = (authorWasChanged) ? authors[4].Id :existingAuthors[4].Id },
                    new() { Title = "Счастье не в золате", CountPages = 326, AuthorId = (authorWasChanged) ? authors[4].Id :existingAuthors[4].Id },

                    new() { Title = "Могила льва", CountPages = 79, AuthorId = (authorWasChanged) ? authors[5].Id :existingAuthors[5].Id },
                    new() { Title = "Жалейка", CountPages = 192, AuthorId = (authorWasChanged) ? authors[5].Id :existingAuthors[5].Id },
                    new() { Title = "Бондаровна", CountPages = 5, AuthorId = (authorWasChanged) ? authors[5].Id :existingAuthors[5].Id },
                    new() { Title = "Курган", CountPages = 64, AuthorId = (authorWasChanged) ? authors[5].Id :existingAuthors[5].Id },
                    new() { Title = "Наследство", CountPages = 72, AuthorId = (authorWasChanged) ? authors[5].Id :existingAuthors[5].Id },

                    new() { Title = "Новая земля", CountPages = 398, AuthorId = (authorWasChanged) ? authors[6].Id :existingAuthors[6].Id },
                    new() { Title = "На росстанях", CountPages = 688, AuthorId = (authorWasChanged) ? authors[6].Id :existingAuthors[6].Id },
                    new() { Title = "Симон-музыкант", CountPages = 304, AuthorId = (authorWasChanged) ? authors[6].Id :existingAuthors[6].Id },
                    new() { Title = "Трясина", CountPages = 232, AuthorId = (authorWasChanged) ? authors[6].Id :existingAuthors[6].Id }
                };

                // Проверяем, совпадают ли списки книг
                if (!AreListsEqual(books, existingBooks, (b1, b2) =>
                    b1.Title == b2.Title &&
                    b1.CountPages == b2.CountPages &&
                    b1.AuthorId == b2.AuthorId))
                {
                    // Если списки не совпадают, очищаем таблицу книг и добавляем новые данные
                    _db.DeleteAll<Book>();
                    foreach (var book in books)
                    {
                        _db.Insert(book);
                    }
                }
            }


            else
            {
                _db.CreateTable<Author>();
                _db.CreateTable<Book>();

                // Вставляем авторов в базу данных и сохраняем их Id
                foreach (var author in authors)
                {
                    _db.Insert(author);
                }

                // список книг (Нужно сначало вставить авторов, а потом определять книги, чтобы правильно определился Foreign Key)
                var books = new List<Book>
                {
                    new() { Title = "Люди на балоте", CountPages = 512, AuthorId = authors[0].Id },
                    new() { Title = "Дыхание грозы", CountPages = 704, AuthorId = authors[0].Id },
                    new() { Title = "Минское направление", CountPages = 759, AuthorId = authors[0].Id },

                    new() { Title = "Шляхтич Завальня", CountPages = 352, AuthorId = authors[1].Id },

                    new() { Title = "Дикая охота короля Стаха", CountPages = 280, AuthorId = authors[2].Id },
                    new() { Title = "Колосья под серпом твоим", CountPages = 735, AuthorId = authors[2].Id },
                    new() { Title = "Земля под белыми крыльями", CountPages = 192, AuthorId = authors[2].Id },

                    new() { Title = "Кто смеётся последним...", CountPages = 188, AuthorId = authors[3].Id },
                    new() { Title = "Дипломированный Баран", CountPages = 136, AuthorId = authors[3].Id },
                    new() { Title = "Мой сосед", CountPages = 146, AuthorId = authors[3].Id },

                    new() { Title = "Бондарь", CountPages = 12, AuthorId = authors[4].Id },
                    new() { Title = "Соловей", CountPages = 157, AuthorId = authors[4].Id },
                    new() { Title = "Счастье не в золате", CountPages = 326, AuthorId = authors[4].Id },

                    new() { Title = "Могила льва", CountPages = 79, AuthorId = authors[5].Id },
                    new() { Title = "Жалейка", CountPages = 192, AuthorId = authors[5].Id },
                    new() { Title = "Бондаровна", CountPages = 5, AuthorId = authors[5].Id },
                    new() { Title = "Курган", CountPages = 64, AuthorId = authors[5].Id },
                    new() { Title = "Наследство", CountPages = 72, AuthorId = authors[5].Id },

                    new() { Title = "Новая земля", CountPages = 398, AuthorId = authors[6].Id },
                    new() { Title = "На росстанях", CountPages = 688, AuthorId = authors[6].Id },
                    new() { Title = "Симон-музыкант", CountPages = 304, AuthorId = authors[6].Id },
                    new() { Title = "Трясина", CountPages = 232, AuthorId = authors[6].Id }
                };

                foreach (var book in books)
                {
                    _db.Insert(book);
                }
            }
        }
    }
}
