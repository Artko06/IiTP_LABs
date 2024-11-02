using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1.Classes
{
    internal static class DirectorBuild
    {
        public static TV GetTVandStartCabelShow(string nameTV, string screenType, AbstractBuilder builder)
        { 
            return builder.SetNameTV(nameTV)
                .SetScreenType(screenType)
                .SetShowType(new CabelShow())
                .Build();
        }

        public static TV GetTVandStartLiveShow(string nameTV, string screenType, AbstractBuilder builder)
        {
            return builder.SetNameTV(nameTV)
                .SetScreenType(screenType)
                .SetShowType(new LiveShow())
                .Build();
        }

        public static TV GetTVandStartWirelessShow(string nameTV, string screenType, AbstractBuilder builder)
        {
            return builder.SetNameTV(nameTV)
                .SetScreenType(screenType)
                .SetShowType(new WirelessShow())
                .Build();
        }

        public static TV GetTV(string nameTV, string screenType, AbstractBuilder builder)
        {
            return builder.SetNameTV(nameTV)
                .SetScreenType(screenType)
                .Build();
        }
    }
}
