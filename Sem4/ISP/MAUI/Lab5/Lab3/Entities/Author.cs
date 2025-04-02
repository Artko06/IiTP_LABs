using SQLite;

namespace Lab3.Entities
{
    [Table("Authors")]
    public class Author
    {
        [PrimaryKey, AutoIncrement, Indexed]
        public int Id { get; set; }
        public string Name { get; set; }
    }
}
