using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using Task1.Interfaces;

namespace Task1.Classes
{
    internal abstract class AbstractBuilder
    {
        private protected string? _nameTV;
        private protected string? _screenType;
        private protected IShowType? _showType = null;

        public AbstractBuilder SetNameTV(string nameTV) {
            _nameTV = nameTV;
            return this; 
        }

        public AbstractBuilder SetScreenType(string screenType) {
            _screenType = screenType;
            return this;
        }

        public AbstractBuilder SetShowType(IShowType showType) {
            _showType = showType;
            return this;
        }

        public abstract TV Build();
    }
}
