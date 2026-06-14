#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <sstream>
#include <iomanip>

using namespace std;

enum TokenType
{
    KEYWORD,
    IDENTIFIER,
    TYPE,
    CONSTANT,
    OPERATOR,
    SEPARATOR,
    STRING_LITERAL,
    CHAR_LITERAL,
    ERROR
};

struct Token
{
    TokenType type;
    string value;
    int line;
    int position;
};

class IdrisLexer
{
private:
    string sourceCode;
    int position;
    int line;
    int column;
    vector<Token> tokens;

    vector<string> keywords = {
        "let", "in", "do", "if", "then", "else", "case", "of",
        "import", "main", "putStrLn", "show", "insert", "empty"};

    vector<string> types = {
        "Int", "Double", "String", "List", "Vect", "Nat", "IO",
        "Char", "Integer", "Bits8", "SortedMap"};

    char peek()
    {
        if (position >= sourceCode.length())
            return '\0';
        return sourceCode[position];
    }

    char peekNext()
    {
        if (position + 1 >= sourceCode.length())
            return '\0';
        return sourceCode[position + 1];
    }

    char advance()
    {
        if (position >= sourceCode.length())
            return '\0';
        char c = sourceCode[position];
        position++;
        column++;
        return c;
    }

    void addToken(TokenType type, string value, int lexemeLength)
    {
        Token token;
        token.type = type;
        token.value = value;
        token.line = line;
        token.position = column - lexemeLength;
        tokens.push_back(token);
    }

    void addToken(TokenType type, string value)
    {
        addToken(type, value, value.length());
    }

    bool isKeyword(string word)
    {
        for (const auto &kw : keywords)
        {
            if (kw == word)
                return true;
        }
        return false;
    }

    bool isType(string word)
    {
        for (const auto &t : types)
        {
            if (t == word)
                return true;
        }
        return false;
    }

    void skipWhitespace()
    {
        while (true)
        {
            char c = peek();
            if (c == ' ' || c == '\t' || c == '\r')
            {
                advance();
            }
            else if (c == '\n')
            {
                advance();
                line++;
                column = 1;
            }
            else if (c == '-' && peekNext() == '-')
            {
                // Single-line comment
                while (peek() != '\n' && peek() != '\0')
                {
                    advance();
                }
            }
            else if (c == '{' && peekNext() == '-')
            {
                // Multi-line comment
                advance(); // '{'
                advance(); // '-'
                while (true)
                {
                    if (peek() == '\0') {
                        addToken(ERROR, "Unclosed multi-line comment", 2);
                        break;
                    }
                    if (peek() == '-' && peekNext() == '}') {
                        advance(); // '-'
                        advance(); // '}'
                        break;
                    }
                    if (peek() == '\n') {
                        advance();
                        line++;
                        column = 1;
                    } else {
                        advance();
                    }
                }
            }
            else
            {
                break;
            }
        }
    }

    void processIdentifier()
    {
        int start = position;
        if (isalpha(peek()) || peek() == '_')
        {
            advance();
        } else {
            addToken(ERROR, string(1, advance()));
            return;
        }
        
        while (isalnum(peek()) || peek() == '_')
        {
            advance();
        }

        string ident = sourceCode.substr(start, position - start);

        if (isKeyword(ident))
        {
            addToken(KEYWORD, ident);
        }
        else if (isType(ident))
        {
            addToken(TYPE, ident);
        }
        else
        {
            addToken(IDENTIFIER, ident);
        }
    }

    void processNumber()
    {
        int start = position;
        string numStr;

        enum class State { START, INTEGER, DOT, FRACTION, EXP, SIGN, EXP_DIGITS };
        State state = State::START;

        while (position < sourceCode.length()) {
            char c = peek();
            switch (state) {
                case State::START:
                    if (isdigit(c)) {
                        state = State::INTEGER;
                        numStr += advance();
                    } else {
                        goto end_loop; 
                    }
                    break;
                case State::INTEGER:
                    if (isdigit(c)) {
                        numStr += advance();
                    } else if (c == '.') {
                        state = State::DOT;
                        numStr += advance();
                    } else if (c == 'e' || c == 'E') {
                        state = State::EXP;
                        numStr += advance();
                    } else {
                        goto end_loop;
                    }
                    break;
                case State::DOT:
                    if (isdigit(c)) {
                        state = State::FRACTION;
                        numStr += advance();
                    } else {
                        addToken(ERROR, numStr + " (incomplete fraction)", numStr.length());
                        return;
                    }
                    break;
                case State::FRACTION:
                    if (isdigit(c)) {
                        numStr += advance();
                    } else if (c == 'e' || c == 'E') {
                        state = State::EXP;
                        numStr += advance();
                    } else if (c == '.') { 
                        numStr += advance();
                        while(isdigit(peek())){
                            numStr += advance();
                        }
                        addToken(ERROR, numStr + " (invalid number format)", numStr.length());
                        return;
                    }
                    else {
                        goto end_loop;
                    }
                    break;
                case State::EXP:
                    if (c == '+' || c == '-') {
                        state = State::SIGN;
                        numStr += advance();
                    } else if (isdigit(c)) {
                        state = State::EXP_DIGITS;
                        numStr += advance();
                    } else {
                        addToken(ERROR, numStr + " (invalid exponent)", numStr.length());
                        return;
                    }
                    break;
                case State::SIGN:
                    if (isdigit(c)) {
                        state = State::EXP_DIGITS;
                        numStr += advance();
                    } else {
                        addToken(ERROR, numStr + " (exponent missing digits)", numStr.length());
                        return;
                    }
                    break;
                case State::EXP_DIGITS:
                    if (isdigit(c)) {
                        numStr += advance();
                    } else {
                        goto end_loop;
                    }
                    break;
            }
        }

    end_loop:
        if (state == State::INTEGER || state == State::FRACTION || state == State::EXP_DIGITS) {
            addToken(CONSTANT, numStr);
        }
        else {
            addToken(ERROR, numStr + " (incomplete number)", numStr.length());
        }
    }

    void processString()
    {
        advance();
        int start = position;

        while (peek() != '"' && peek() != '\0')
        {
            advance();
        }

        string str = sourceCode.substr(start, position - start);

        if (peek() == '"')
        {
            advance();
            addToken(STRING_LITERAL, str);
        }
        else
        {
            addToken(ERROR, "Unclosed string", str.length() + 1);
        }
    }

    void processChar()
    {
        advance();
        char c = advance();

        if (peek() == '\'')
        {
            advance();
            addToken(CHAR_LITERAL, string(1, c));
        }
        else
        {
            addToken(ERROR, "Unclosed character literal", 2);
        }
    }

    void processOperator()
    {
        char c = advance();
        string op(1, c);

        if (c == ':' && peek() == ':')
        {
            advance();
            op = "::";
        }
        else if (c == '-' && peek() == '>')
        {
            advance();
            op = "->";
        }
        else if (c == '&' && peek() == '&')
        {
            advance();
            op = "&&";
        }
        else if (c == '=' && peek() == '>')
        {
            advance();
            op = "=>";
        }

        addToken(OPERATOR, op);
    }

    void printLexemeSequence()
    {
        cout << "\n\n" << string(50, '=') << "\n";
        cout << "4. LEXEME SEQUENCE" << endl;
        cout << string(60, '-') << endl;

        map<string, int> unifiedTable;
        int idCounter = 1;

        for (const auto& token : tokens) {
            if (token.type == IDENTIFIER || token.type == CONSTANT || token.type == STRING_LITERAL || token.type == CHAR_LITERAL) {
                if (unifiedTable.find(token.value) == unifiedTable.end()) {
                    unifiedTable[token.value] = idCounter++;
                }
            }
        }
        
        for (const auto& token : tokens) {
            if (token.type == IDENTIFIER || token.type == CONSTANT || token.type == STRING_LITERAL || token.type == CHAR_LITERAL) {
                cout << "<ИД" << unifiedTable.at(token.value) << ">";
            } else if (token.type == OPERATOR || token.type == SEPARATOR) {
                cout << token.value;
            } else if (token.type == KEYWORD || token.type == TYPE) {
                cout << token.value;
            }
        }
        cout << endl;
    }

public:
    IdrisLexer(const string &code) : sourceCode(code), position(0), line(1), column(1) {}

    vector<Token> analyze()
    {
        while (position < sourceCode.length())
        {
            skipWhitespace();
            char c = peek();

            if (c == '\0')
                break;

            if (isalpha(c) || c == '_')
            {
                processIdentifier();
            }
            else if (isdigit(c))
            {
                processNumber();
            }
            else if (c == '"')
            {
                processString();
            }
            else if (c == '\'')
            {
                processChar();
            }
            else if (c == ',' || c == ';' || c == '.' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}')
            {
                addToken(SEPARATOR, string(1, advance()));
            }
            else if (c == ':' || c == '=' || c == '>' || c == '&' || c == '+' || c == '*' || c == '-')
            {
                processOperator();
            }
            else
            {
                addToken(ERROR, string(1, advance()));
            }
        }

        return tokens;
    }

    void printTable(const string& tableName, const map<string, int>& table)
    {
        cout << "\n\n" << tableName << ":" << endl;
        cout << string(40, '-') << endl;
        cout << left << setw(5) << "ID" << left << setw(35) << "VALUE" << endl;
        cout << string(40, '-') << endl;
        
        map<int, string> sortedTable;
        for(const auto& pair : table) {
            sortedTable[pair.second] = pair.first;
        }

        for (const auto &entry : sortedTable)
        {
            cout << left << setw(5) << entry.first
                 << left << setw(35) << entry.second << endl;
        }
    }

    void printResults()
    {
        cout << "\n"
             << string(50, '=') << endl;
        cout << "IDRIS CODE LEXICAL ANALYSIS" << endl;
        cout << string(50, '=') << "\n"
             << endl;

        // 1. TOKEN LIST
        cout << "1. TOKEN TABLE:" << endl;
        cout << string(80, '-') << endl;
        cout << left << setw(18) << "TOKEN TYPE"
             << left << setw(45) << "VALUE"
             << left << setw(8) << "LINE"
             << left << setw(10) << "POSITION" << endl;
        cout << string(80, '-') << endl;

        for (const auto &token : tokens)
        {
            string typeName;
            switch (token.type)
            {
            case KEYWORD: typeName = "Keyword"; break;
            case IDENTIFIER: typeName = "Identifier"; break;
            case TYPE: typeName = "Type"; break;
            case CONSTANT: typeName = "Constant"; break;
            case OPERATOR: typeName = "Operator"; break;
            case SEPARATOR: typeName = "Separator"; break;
            case STRING_LITERAL: typeName = "String Literal"; break;
            case CHAR_LITERAL: typeName = "Char Literal"; break;
            case ERROR: typeName = "ERROR"; break;
            }
            cout << left << setw(18) << typeName
                 << left << setw(45) << token.value
                 << left << setw(8) << token.line
                 << left << setw(10) << token.position << endl;
        }

        // 2. SYMBOL TABLES
        cout << "\n\n" << string(50, '=') << "\n";
        cout << "2. SYMBOL TABLES" << endl;

        map<string, int> idTable, constTable, keywordTable, typeTable, opTable, sepTable;
        int idCounter = 1, constCounter = 1, keywordCounter = 1, typeCounter = 1, opCounter = 1, sepCounter = 1;

        for (const auto &token : tokens) {
            switch (token.type) {
                case IDENTIFIER:
                    if (idTable.find(token.value) == idTable.end()) idTable[token.value] = idCounter++;
                    break;
                case CONSTANT:
                case STRING_LITERAL:
                case CHAR_LITERAL:
                    if (constTable.find(token.value) == constTable.end()) constTable[token.value] = constCounter++;
                    break;
                case KEYWORD:
                    if (keywordTable.find(token.value) == keywordTable.end()) keywordTable[token.value] = keywordCounter++;
                    break;
                case TYPE:
                     if (typeTable.find(token.value) == typeTable.end()) typeTable[token.value] = typeCounter++;
                    break;
                case OPERATOR:
                     if (opTable.find(token.value) == opTable.end()) opTable[token.value] = opCounter++;
                    break;
                case SEPARATOR:
                     if (sepTable.find(token.value) == sepTable.end()) sepTable[token.value] = sepCounter++;
                    break;
                case ERROR:
                    // Ну ошибки там кароче в конце выводятся, если они есть
                    break;
            }
        }
        
        printTable("Identifiers", idTable);
        printTable("Constants", constTable);
        printTable("Keywords", keywordTable);
        printTable("Data Types", typeTable);
        printTable("Operators", opTable);
        printTable("Separators", sepTable);


        // 3. ANALYSIS STATISTICS
        cout << "\n\n" << string(50, '=') << "\n";
        cout << "3. ANALYSIS STATISTICS:" << endl;
        cout << string(40, '-') << endl;

        int errorCount = 0;
        map<TokenType, int> typeCount;
        for (const auto &token : tokens) {
            typeCount[token.type]++;
            if (token.type == ERROR) errorCount++;
        }
        
        int totalConstants = typeCount[CONSTANT] + typeCount[STRING_LITERAL] + typeCount[CHAR_LITERAL];

        cout << left << setw(20) << "Total Tokens:" << setw(15) << tokens.size() << endl;
        cout << left << setw(20) << "Keywords:" << setw(15) << typeCount[KEYWORD] << endl;
        cout << left << setw(20) << "Identifiers:" << setw(15) << typeCount[IDENTIFIER] << endl;
        cout << left << setw(20) << "Data Types:" << setw(15) << typeCount[TYPE] << endl;
        cout << left << setw(20) << "Constants:" << setw(15) << totalConstants << endl;
        cout << left << setw(20) << "Errors:" << setw(15) << errorCount << endl;

        printLexemeSequence();

        // 5. ERRORS
        if (errorCount > 0)
        {
            cout << "\n\n" << string(50, '=') << "\n";
            cout << "5. DETECTED ERRORS:" << endl;
            cout << string(60, '-') << endl;
            for (const auto &token : tokens)
            {
                if (token.type == ERROR)
                {
                    cout << "Line " << setw(3) << token.line
                         << ", position " << setw(3) << token.position
                         << ": " << token.value << endl;
                }
            }
        }
    }
};

int main()
{
    ifstream inputFile("input.txt");

    if (!inputFile.is_open())
    {
        cout << "Error: Could not open file input.txt" << endl;
        return 1;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();
    string idrisCode = buffer.str();
    inputFile.close();

    cout << "\n"
         << string(50, '=') << endl;
    cout << "SOURCE CODE FROM input.txt" << endl;
    cout << string(50, '=') << "\n"
         << endl;
    cout << idrisCode << endl;

    IdrisLexer lexer(idrisCode);
    lexer.analyze();
    lexer.printResults();

    return 0;
}