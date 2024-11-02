using System.IO;
using Task1.Classes;

namespace Task1
{
    file class Program
    {
        static void Main(string[] args)
        {
           List<TV> listOfTV = new List<TV>();

            listOfTV.AddRange(new TV[]
             {
                DirectorBuild.GetTVandStartCabelShow("LG", "AMOLED", new BuilderSmartTV()),
                DirectorBuild.GetTVandStartLiveShow("Samsung", "OLED", new BuilderSmartTV()),
                DirectorBuild.GetTVandStartWirelessShow("Horizont", "IPS", new BuilderMotionTV()),
                DirectorBuild.GetTV("Витязь", "ЖК", new BuilderSmartTV())
             }) ;

            foreach (var tv in listOfTV)
            {
                if (tv is SmartTV smartTV)
                {
                    Console.WriteLine($"Get info: {smartTV.GetInfo()}");
                    Console.WriteLine($"Show: {smartTV.Show()}");
                    Console.WriteLine($"Browse internet: {smartTV.BrowseInternet()}");
                    Console.WriteLine($"Open YouTybe: {smartTV.OpenYouTybe()}\n");
                }
                else if (tv is MotionTV motionTV)
                {   
                    Console.WriteLine($"Get info: {motionTV.GetInfo()}");
                    Console.WriteLine($"Show: {motionTV.Show()}");
                    Console.WriteLine($"Output recognize motion on screen: {motionTV.OutputRecognizeMotionOnScreen()}");
                    Console.WriteLine($"Play to the recognition game: {motionTV.PlayToTheRecognitionGame()}\n");
                }
            }
        }
    }
}