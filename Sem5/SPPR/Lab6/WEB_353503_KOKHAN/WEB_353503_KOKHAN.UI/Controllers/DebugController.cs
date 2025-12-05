using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using System.Security.Claims;

namespace WEB_353503_KOKHAN.UI.Controllers
{
    [Authorize]
    public class DebugController : Controller
    {
        public IActionResult Claims()
        {
            ViewBag.Claims = User.Claims.Select(c => new { c.Type, c.Value }).ToList();
            ViewBag.Roles = User.Claims.Where(c => c.Type == ClaimTypes.Role || c.Type == "role").Select(c => c.Value).ToList();
            return View();
        }
    }
}
