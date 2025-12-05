namespace WEB_353503_KOKHAN.Domain.Entities
{
    public class Car
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public int CategoryId { get; set; }
        public Category? Category { get; set; }
        public double Price { get; set; }
        public string? ImagePath { get; set; }
        public string? ImageMimeType { get; set; }
    }
}
