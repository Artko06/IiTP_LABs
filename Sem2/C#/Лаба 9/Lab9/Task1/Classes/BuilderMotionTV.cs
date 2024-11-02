using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace Task1.Classes
{
    internal class BuilderMotionTV : AbstractBuilder
    {
        public override TV Build()
        {
            MotionTV motionTV = new MotionTV(nameTV: _nameTV, screenType: _screenType);
            if (_showType != null)
            {
                motionTV.showType = _showType;
            }
            return motionTV;
        }
    }
}
