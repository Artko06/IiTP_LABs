using Lab3.Entities;

namespace Lab3.Services
{
    public interface IDbService
    {
        IEnumerable<Author> GetAllAuthors();
        IEnumerable<Book> GetAuthorBooks(int id);
        void Init();
    }
}
