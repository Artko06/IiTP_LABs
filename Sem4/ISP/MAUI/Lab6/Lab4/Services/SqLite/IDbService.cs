using Lab4.Entities;

namespace Lab4.Services
{
    public interface IDbService
    {
        IEnumerable<Author> GetAllAuthors();
        IEnumerable<Book> GetAuthorBooks(int id);
        void Init();
    }
}
