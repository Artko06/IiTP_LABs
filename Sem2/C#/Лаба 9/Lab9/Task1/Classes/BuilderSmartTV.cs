namespace Task1.Classes
{
    internal class BuilderSmartTV : AbstractBuilder
    {
        public override TV Build()
        {
            SmartTV smartTV = new SmartTV(nameTV: _nameTV, screenType: _screenType);
            if (_showType != null)
            {
                smartTV.showType = _showType;
            }
            return smartTV;
        }
    }
}
