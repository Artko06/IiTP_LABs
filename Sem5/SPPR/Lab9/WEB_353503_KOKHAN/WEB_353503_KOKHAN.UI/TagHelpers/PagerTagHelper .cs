using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.AspNetCore.Razor.TagHelpers;

namespace WEB_353503_KOKHAN.UI.TagHelpers
{
    [HtmlTargetElement("pager")]
    public class PagerTagHelper : TagHelper
    {
        private readonly LinkGenerator _linkGenerator;
        private readonly IHttpContextAccessor _httpContextAccessor;

        public PagerTagHelper(LinkGenerator linkGenerator, IHttpContextAccessor httpContextAccessor)
        {
            _linkGenerator = linkGenerator;
            _httpContextAccessor = httpContextAccessor;
        }

        [HtmlAttributeName("current-page")]
        public int CurrentPage { get; set; }

        [HtmlAttributeName("total-pages")]
        public int TotalPages { get; set; }

        [HtmlAttributeName("category")]
        public string? Category { get; set; }

        [HtmlAttributeName("admin")]
        public bool Admin { get; set; } = false;

        public override void Process(TagHelperContext context, TagHelperOutput output)
        {
            output.TagName = "nav";
            output.Attributes.SetAttribute("aria-label", "Page navigation");

            var ul = new TagBuilder("ul");
            ul.AddCssClass("pagination justify-content-center");

            ul.InnerHtml.AppendHtml(CreatePageItem(CurrentPage - 1, "&laquo;", "»",
                CurrentPage == 1));

            for (int i = 1; i <= TotalPages; i++)
            {
                ul.InnerHtml.AppendHtml(CreatePageItem(i, i.ToString(), null, false));
            }

            ul.InnerHtml.AppendHtml(CreatePageItem(CurrentPage + 1, "&raquo;", "«",
                CurrentPage == TotalPages));

            output.Content.AppendHtml(ul);
        }

        private TagBuilder CreatePageItem(int page, string text, string? ariaLabel, bool disabled)
        {
            var li = new TagBuilder("li");
            li.AddCssClass("page-item");

            if (disabled)
            {
                li.AddCssClass("disabled");
            }
            else if (page == CurrentPage)
            {
                li.AddCssClass("active");
            }

            // Для disabled кнопок используем span вместо ссылки
            var linkTag = disabled ? new TagBuilder("span") : new TagBuilder("a");
            linkTag.AddCssClass("page-link");

            if (!disabled)
            {
                linkTag.Attributes["href"] = GeneratePageLink(page);
            }

            if (!string.IsNullOrEmpty(ariaLabel))
            {
                linkTag.Attributes["aria-label"] = ariaLabel;
            }

            linkTag.InnerHtml.AppendHtml(text);
            li.InnerHtml.AppendHtml(linkTag);

            return li;
        }

        private string GeneratePageLink(int page)
        {
            var httpContext = _httpContextAccessor.HttpContext;
            if (httpContext == null) return "#";

            var values = new RouteValueDictionary
            {
                { "pageNo", page }
            };

            if (!string.IsNullOrEmpty(Category))
            {
                values["category"] = Category;
            }

            string? url;

            if (Admin)
            {
                url = _linkGenerator.GetPathByPage(
                    page: "/Admin/Cars",
                    values: values,
                    httpContext: httpContext);
            }
            else
            {
                url = _linkGenerator.GetPathByAction(
                    action: "Index",
                    controller: "Car",
                    values: values,
                    httpContext: httpContext);
            }

            return url ?? "#";
        }
    }
}