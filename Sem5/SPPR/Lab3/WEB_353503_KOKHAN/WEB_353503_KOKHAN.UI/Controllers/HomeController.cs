using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using WEB_353503_KOKHAN.UI.Models;

namespace WEB_353503_KOKHAN.UI.Controllers
{
    public class HomeController : Controller
    {
        private readonly ILogger<HomeController> _logger;

        public HomeController(ILogger<HomeController> logger)
        {
            _logger = logger;
        }

        public IActionResult Index()
        {
            ViewData["LabWorkTitle"] = "Лабораторная работа №3";

            List<string> items1 = new List<string>
            {
                "элемент 1 списка",
                "элемент 2 списка",
                "элемент 3 списка"
            };
            ViewBag.Items = items1;

            var items = new List<ListDemo>
            {
                new ListDemo { Id = 1, Name = "Элемент 1" },
                new ListDemo { Id = 2, Name = "Элемент 2" },
                new ListDemo { Id = 3, Name = "Элемент 3" }
            };

            ViewBag.ListItems = new SelectList(items, "Id", "Name");

            return View();
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}
