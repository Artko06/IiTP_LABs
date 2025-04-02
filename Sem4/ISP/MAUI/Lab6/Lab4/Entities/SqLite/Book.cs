using SQLite;

namespace Lab4.Entities
{
    [Table("Books")]
    public class Book
    {
        [PrimaryKey, AutoIncrement, Indexed]
        public int Id { get; set; }
        public string Title { get; set; }
        public int CountPages { get; set; }

        [Indexed]
        public int AuthorId { get; set; }
    }
}
