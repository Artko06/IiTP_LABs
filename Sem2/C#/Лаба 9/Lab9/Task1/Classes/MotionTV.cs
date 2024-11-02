using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Task1.Interfaces;

namespace Task1.Classes
{
    internal class MotionTV : TV, IMotionRecognition
    {
        public MotionTV(string nameTV, string screenType) 
        { 
            this.nameTV = nameTV;
            this.screenType = screenType;
        }

        public string PlayToTheRecognitionGame() => 
            "Запущена игра, где нужно выполнять различные движения за время";

        public string OutputRecognizeMotionOnScreen() => 
            "Телевизор выводит ваши движения на экран";

        public override string GetInfo() =>
            $"MotionTV, {nameTV}, {screenType}\nТелевизор распознаёт движения";

        public override string Show() =>
           (showType == null) ? "-" : showType.Show();
    }
}
