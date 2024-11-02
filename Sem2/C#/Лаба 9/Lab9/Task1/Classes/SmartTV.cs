using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using Task1.Interfaces;

namespace Task1.Classes
{
    internal class SmartTV : TV, IBrowser
    {
        public SmartTV(string nameTV, string screenType)
        {
            this.nameTV = nameTV;
            this.screenType = screenType;
        }

        public string OpenYouTybe() => 
            "Открытие YouTybe...\nYouTybe успешно открыт";

        public string BrowseInternet() =>
            "Выполняется взаимодействие с браузером";

        public override string GetInfo() => 
            $"SmartTV, {nameTV}, {screenType}\nТелевизор подключен к интернету";

        public override string Show() => 
            (showType == null) ? "-" : showType.Show();
    }
}