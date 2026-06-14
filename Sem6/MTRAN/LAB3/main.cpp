#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

enum class TokenKind
{
    Reserved,
    Identifier,
    TypeName,
    Number,
    StringLiteral,
    CharLiteral,
    Operator,
    Separator,
    Newline,
    EndOfFile,
    Error
};

struct Token
{
    TokenKind kind;
    string value;
    int line;
    int column;
};

struct AstNode
{
    string label;
    vector<shared_ptr<AstNode>> children;

    explicit AstNode(string text) : label(std::move(text)) {}
};

shared_ptr<AstNode> makeNode(const string &label)
{
    return make_shared<AstNode>(label);
}

struct LogicalLine
{
    int lineNumber = 0;
    int indent = 1;
    vector<Token> tokens;
};

#include "../common/idris_cst_graphviz.hpp"

class IdrisLexer
{
private:
    string sourceCode;
    size_t position = 0;
    int line = 1;
    int column = 1;
    vector<Token> tokens;

    const set<string> reservedWords = {"import", "let", "in", "if", "then", "else", "case", "of", "do"};
    const set<string> knownTypes = {"Int", "Double", "String", "List", "Vect", "Nat", "IO", "Char", "Integer", "Bits8", "SortedMap"};

    char peek(size_t offset = 0) const
    {
        if (position + offset >= sourceCode.size())
        {
            return '\0';
        }
        return sourceCode[position + offset];
    }

    char advance()
    {
        if (position >= sourceCode.size())
        {
            return '\0';
        }

        char current = sourceCode[position++];
        if (current == '\n')
        {
            line++;
            column = 1;
        }
        else
        {
            column++;
        }
        return current;
    }

    void addToken(TokenKind kind, const string &value, int tokenLine, int tokenColumn)
    {
        tokens.push_back({kind, value, tokenLine, tokenColumn});
    }

    void lexNewline()
    {
        int tokenLine = line;
        int tokenColumn = column;
        advance();
        addToken(TokenKind::Newline, "\\n", tokenLine, tokenColumn);
    }

    void skipWhitespace()
    {
        while (true)
        {
            char current = peek();
            if (current == ' ' || current == '\t' || current == '\r')
            {
                advance();
                continue;
            }

            if (current == '-' && peek(1) == '-')
            {
                while (peek() != '\n' && peek() != '\0')
                {
                    advance();
                }
                continue;
            }

            if (current == '{' && peek(1) == '-')
            {
                int startLine = line;
                int startColumn = column;
                advance();
                advance();

                while (!(peek() == '-' && peek(1) == '}'))
                {
                    if (peek() == '\0')
                    {
                        addToken(TokenKind::Error, "Unclosed multi-line comment", startLine, startColumn);
                        return;
                    }
                    advance();
                }

                advance();
                advance();
                continue;
            }

            break;
        }
    }

    void lexIdentifier()
    {
        int startLine = line;
        int startColumn = column;
        string value;

        while (isalnum(static_cast<unsigned char>(peek())) || peek() == '_' || peek() == '\'')
        {
            value += advance();
        }

        if (reservedWords.count(value) != 0U)
        {
            addToken(TokenKind::Reserved, value, startLine, startColumn);
        }
        else if (knownTypes.count(value) != 0U)
        {
            addToken(TokenKind::TypeName, value, startLine, startColumn);
        }
        else
        {
            addToken(TokenKind::Identifier, value, startLine, startColumn);
        }
    }

    void lexNumber()
    {
        int startLine = line;
        int startColumn = column;
        string value;

        enum class State
        {
            Integer,
            Dot,
            Fraction,
            Exponent,
            ExponentSign,
            ExponentDigits
        };

        State state = State::Integer;

        while (true)
        {
            char current = peek();
            switch (state)
            {
            case State::Integer:
                if (isdigit(static_cast<unsigned char>(current)))
                {
                    value += advance();
                }
                else if (current == '.')
                {
                    state = State::Dot;
                    value += advance();
                }
                else if (current == 'e' || current == 'E')
                {
                    state = State::Exponent;
                    value += advance();
                }
                else
                {
                    addToken(TokenKind::Number, value, startLine, startColumn);
                    return;
                }
                break;

            case State::Dot:
                if (isdigit(static_cast<unsigned char>(current)))
                {
                    state = State::Fraction;
                    value += advance();
                }
                else
                {
                    addToken(TokenKind::Error, value + " (incomplete fraction)", startLine, startColumn);
                    return;
                }
                break;

            case State::Fraction:
                if (isdigit(static_cast<unsigned char>(current)))
                {
                    value += advance();
                }
                else if (current == 'e' || current == 'E')
                {
                    state = State::Exponent;
                    value += advance();
                }
                else if (current == '.')
                {
                    value += advance();
                    while (isdigit(static_cast<unsigned char>(peek())))
                    {
                        value += advance();
                    }
                    addToken(TokenKind::Error, value + " (invalid number format)", startLine, startColumn);
                    return;
                }
                else
                {
                    addToken(TokenKind::Number, value, startLine, startColumn);
                    return;
                }
                break;

            case State::Exponent:
                if (current == '+' || current == '-')
                {
                    state = State::ExponentSign;
                    value += advance();
                }
                else if (isdigit(static_cast<unsigned char>(current)))
                {
                    state = State::ExponentDigits;
                    value += advance();
                }
                else
                {
                    addToken(TokenKind::Error, value + " (invalid exponent)", startLine, startColumn);
                    return;
                }
                break;

            case State::ExponentSign:
                if (isdigit(static_cast<unsigned char>(current)))
                {
                    state = State::ExponentDigits;
                    value += advance();
                }
                else
                {
                    addToken(TokenKind::Error, value + " (exponent missing digits)", startLine, startColumn);
                    return;
                }
                break;

            case State::ExponentDigits:
                if (isdigit(static_cast<unsigned char>(current)))
                {
                    value += advance();
                }
                else
                {
                    addToken(TokenKind::Number, value, startLine, startColumn);
                    return;
                }
                break;
            }
        }
    }

    void lexString()
    {
        int startLine = line;
        int startColumn = column;
        advance();

        string value;
        while (peek() != '"' && peek() != '\0' && peek() != '\n')
        {
            if (peek() == '\\' && peek(1) != '\0')
            {
                value += advance();
            }
            value += advance();
        }

        if (peek() == '"')
        {
            advance();
            addToken(TokenKind::StringLiteral, value, startLine, startColumn);
            return;
        }

        addToken(TokenKind::Error, "Unclosed string literal", startLine, startColumn);
    }

    void lexChar()
    {
        int startLine = line;
        int startColumn = column;
        advance();

        string value;
        if (peek() == '\\' && peek(1) != '\0')
        {
            value += advance();
            value += advance();
        }
        else if (peek() != '\0' && peek() != '\n')
        {
            value += advance();
        }

        if (peek() == '\'')
        {
            advance();
            addToken(TokenKind::CharLiteral, value, startLine, startColumn);
            return;
        }

        addToken(TokenKind::Error, "Unclosed character literal", startLine, startColumn);
    }

    void lexOperatorOrSeparator()
    {
        static const vector<string> multiCharOperators = {"->", "=>", "&&", "||", "++", "::", "==", "/=", "<=", ">="};
        static const string singleOperators = ":=><+-*/";
        static const string separators = "(),[].{}";

        int startLine = line;
        int startColumn = column;

        for (const auto &candidate : multiCharOperators)
        {
            bool match = true;
            for (size_t i = 0; i < candidate.size(); ++i)
            {
                if (peek(i) != candidate[i])
                {
                    match = false;
                    break;
                }
            }

            if (match)
            {
                for (size_t i = 0; i < candidate.size(); ++i)
                {
                    advance();
                }
                addToken(TokenKind::Operator, candidate, startLine, startColumn);
                return;
            }
        }

        char current = peek();
        if (singleOperators.find(current) != string::npos)
        {
            advance();
            addToken(TokenKind::Operator, string(1, current), startLine, startColumn);
            return;
        }

        if (separators.find(current) != string::npos)
        {
            advance();
            addToken(TokenKind::Separator, string(1, current), startLine, startColumn);
            return;
        }

        advance();
        addToken(TokenKind::Error, string(1, current), startLine, startColumn);
    }

public:
    explicit IdrisLexer(string code) : sourceCode(std::move(code)) {}

    vector<Token> analyze()
    {
        while (position < sourceCode.size())
        {
            if (peek() == '\n')
            {
                lexNewline();
                continue;
            }

            skipWhitespace();

            if (position >= sourceCode.size())
            {
                break;
            }

            if (peek() == '\n')
            {
                lexNewline();
                continue;
            }

            char current = peek();
            if (isalpha(static_cast<unsigned char>(current)) || current == '_')
            {
                lexIdentifier();
            }
            else if (isdigit(static_cast<unsigned char>(current)))
            {
                lexNumber();
            }
            else if (current == '"')
            {
                lexString();
            }
            else if (current == '\'')
            {
                lexChar();
            }
            else
            {
                lexOperatorOrSeparator();
            }
        }

        addToken(TokenKind::EndOfFile, "<EOF>", line, column);
        return tokens;
    }
};

class IdrisParser
{
private:
    vector<Token> tokens;
    vector<LogicalLine> lines;
    size_t lineIndex = 0;
    vector<string> syntaxErrors;

    static string tokenKindToString(TokenKind kind)
    {
        switch (kind)
        {
        case TokenKind::Reserved:
            return "Reserved";
        case TokenKind::Identifier:
            return "Identifier";
        case TokenKind::TypeName:
            return "Type";
        case TokenKind::Number:
            return "Number";
        case TokenKind::StringLiteral:
            return "String";
        case TokenKind::CharLiteral:
            return "Char";
        case TokenKind::Operator:
            return "Operator";
        case TokenKind::Separator:
            return "Separator";
        case TokenKind::Newline:
            return "Newline";
        case TokenKind::EndOfFile:
            return "EOF";
        case TokenKind::Error:
            return "Error";
        }
        return "Unknown";
    }

    static bool isLeafToken(const Token &token)
    {
        return token.kind == TokenKind::Identifier || token.kind == TokenKind::TypeName || token.kind == TokenKind::Number ||
               token.kind == TokenKind::StringLiteral || token.kind == TokenKind::CharLiteral;
    }

    static string quoteIfNeeded(const Token &token)
    {
        if (token.kind == TokenKind::StringLiteral)
        {
            return "\"" + token.value + "\"";
        }
        if (token.kind == TokenKind::CharLiteral)
        {
            return "'" + token.value + "'";
        }
        return token.value;
    }

    void recordSyntaxError(const vector<Token> &context, const string &message)
    {
        if (!context.empty())
        {
            const Token &token = context.front();
            syntaxErrors.push_back("Line " + to_string(token.line) + ", column " + to_string(token.column) + ": " + message);
        }
        else
        {
            syntaxErrors.push_back(message);
        }
    }

    static string tokensToText(const vector<Token> &lineTokens)
    {
        string result;
        for (size_t i = 0; i < lineTokens.size(); ++i)
        {
            if (i != 0U)
            {
                result += ' ';
            }
            result += quoteIfNeeded(lineTokens[i]);
        }
        return result;
    }

    static bool hasLexicalError(const LogicalLine &line)
    {
        for (const auto &token : line.tokens)
        {
            if (token.kind == TokenKind::Error)
            {
                return true;
            }
        }
        return false;
    }

    static vector<LogicalLine> buildLines(const vector<Token> &allTokens)
    {
        vector<LogicalLine> result;
        LogicalLine current;
        bool hasContent = false;

        for (const auto &token : allTokens)
        {
            if (token.kind == TokenKind::Newline)
            {
                if (hasContent)
                {
                    result.push_back(current);
                    current = LogicalLine{};
                    hasContent = false;
                }
                continue;
            }

            if (token.kind == TokenKind::EndOfFile)
            {
                if (hasContent)
                {
                    result.push_back(current);
                }
                break;
            }

            if (!hasContent)
            {
                current.lineNumber = token.line;
                current.indent = token.column;
                hasContent = true;
            }

            current.tokens.push_back(token);
        }

        return result;
    }

    static bool isFullyWrapped(const vector<Token> &slice, const string &openToken, const string &closeToken)
    {
        if (slice.size() < 2U || slice.front().value != openToken || slice.back().value != closeToken)
        {
            return false;
        }

        int depth = 0;
        for (size_t i = 0; i < slice.size(); ++i)
        {
            if (slice[i].value == openToken)
            {
                depth++;
            }
            else if (slice[i].value == closeToken)
            {
                depth--;
                if (depth == 0 && i + 1 != slice.size())
                {
                    return false;
                }
            }
        }

        return depth == 0;
    }

    static int findTopLevelToken(const vector<Token> &slice, const set<string> &targets, bool searchFromRight = false)
    {
        int parenDepth = 0;
        int bracketDepth = 0;

        if (!searchFromRight)
        {
            for (size_t i = 0; i < slice.size(); ++i)
            {
                const string &value = slice[i].value;
                if (value == "(")
                {
                    parenDepth++;
                    continue;
                }
                if (value == ")")
                {
                    parenDepth--;
                    continue;
                }
                if (value == "[")
                {
                    bracketDepth++;
                    continue;
                }
                if (value == "]")
                {
                    bracketDepth--;
                    continue;
                }

                if (parenDepth == 0 && bracketDepth == 0 && targets.count(value) != 0U)
                {
                    return static_cast<int>(i);
                }
            }
            return -1;
        }

        for (int i = static_cast<int>(slice.size()) - 1; i >= 0; --i)
        {
            const string &value = slice[static_cast<size_t>(i)].value;
            if (value == ")")
            {
                parenDepth++;
                continue;
            }
            if (value == "(")
            {
                parenDepth--;
                continue;
            }
            if (value == "]")
            {
                bracketDepth++;
                continue;
            }
            if (value == "[")
            {
                bracketDepth--;
                continue;
            }

            if (parenDepth == 0 && bracketDepth == 0 && targets.count(value) != 0U)
            {
                return i;
            }
        }

        return -1;
    }

    static vector<vector<Token>> splitTopLevel(const vector<Token> &slice, const string &delimiter)
    {
        vector<vector<Token>> parts;
        vector<Token> current;
        int parenDepth = 0;
        int bracketDepth = 0;

        for (const auto &token : slice)
        {
            if (token.value == "(")
            {
                parenDepth++;
            }
            else if (token.value == ")")
            {
                parenDepth--;
            }
            else if (token.value == "[")
            {
                bracketDepth++;
            }
            else if (token.value == "]")
            {
                bracketDepth--;
            }

            if (parenDepth == 0 && bracketDepth == 0 && token.value == delimiter)
            {
                parts.push_back(current);
                current.clear();
                continue;
            }

            current.push_back(token);
        }

        parts.push_back(current);
        return parts;
    }

    static size_t consumeTerm(const vector<Token> &slice, size_t start)
    {
        if (start >= slice.size())
        {
            return start;
        }

        const string &value = slice[start].value;
        if (value == "(" || value == "[")
        {
            string openToken = value;
            string closeToken = value == "(" ? ")" : "]";
            int depth = 0;

            for (size_t i = start; i < slice.size(); ++i)
            {
                if (slice[i].value == openToken)
                {
                    depth++;
                }
                else if (slice[i].value == closeToken)
                {
                    depth--;
                    if (depth == 0)
                    {
                        return i + 1;
                    }
                }
            }
            return slice.size();
        }

        return start + 1;
    }

    static vector<vector<Token>> splitApplicationTerms(const vector<Token> &slice)
    {
        vector<vector<Token>> terms;
        size_t index = 0;

        while (index < slice.size())
        {
            size_t next = consumeTerm(slice, index);
            if (next <= index)
            {
                break;
            }
            terms.emplace_back(slice.begin() + static_cast<long>(index), slice.begin() + static_cast<long>(next));
            index = next;
        }

        return terms;
    }

    static vector<Token> flattenLines(size_t start, size_t end, const vector<LogicalLine> &allLines)
    {
        vector<Token> merged;
        for (size_t i = start; i < end; ++i)
        {
            merged.insert(merged.end(), allLines[i].tokens.begin(), allLines[i].tokens.end());
        }
        return merged;
    }

    size_t findBlockEnd(size_t start, int parentIndent) const
    {
        size_t index = start;
        while (index < lines.size() && lines[index].indent > parentIndent)
        {
            ++index;
        }
        return index;
    }

    shared_ptr<AstNode> parseLeaf(const Token &token) const
    {
        return makeNode(tokenKindToString(token.kind) + ": " + quoteIfNeeded(token));
    }

    shared_ptr<AstNode> parseTypeTokens(const vector<Token> &slice)
    {
        if (slice.empty())
        {
            return makeNode("Type: <empty>");
        }

        if (isFullyWrapped(slice, "(", ")"))
        {
            vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            if (inner.empty())
            {
                return makeNode("UnitType");
            }
            auto tupleParts = splitTopLevel(inner, ",");
            if (tupleParts.size() > 1U)
            {
                auto tupleNode = makeNode("TupleType");
                for (const auto &part : tupleParts)
                {
                    tupleNode->children.push_back(parseTypeTokens(part));
                }
                return tupleNode;
            }
            return parseTypeTokens(inner);
        }

        int arrowIndex = findTopLevelToken(slice, {"->"});
        if (arrowIndex >= 0)
        {
            auto arrowNode = makeNode("TypeArrow");
            vector<Token> left(slice.begin(), slice.begin() + arrowIndex);
            vector<Token> right(slice.begin() + arrowIndex + 1, slice.end());
            arrowNode->children.push_back(parseTypeTokens(left));
            arrowNode->children.push_back(parseTypeTokens(right));
            return arrowNode;
        }

        auto terms = splitApplicationTerms(slice);
        if (terms.size() > 1U)
        {
            auto applyNode = makeNode("TypeApply");
            for (const auto &term : terms)
            {
                applyNode->children.push_back(parseTypeTokens(term));
            }
            return applyNode;
        }

        if (slice.size() == 1U)
        {
            return parseLeaf(slice.front());
        }

        auto fallback = makeNode("TypeExpr");
        fallback->children.push_back(makeNode(tokensToText(slice)));
        return fallback;
    }

    shared_ptr<AstNode> parsePatternTokens(const vector<Token> &slice)
    {
        if (slice.empty())
        {
            return makeNode("Pattern: <empty>");
        }

        if (isFullyWrapped(slice, "(", ")"))
        {
            vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            if (inner.empty())
            {
                return makeNode("UnitPattern");
            }
            auto tupleParts = splitTopLevel(inner, ",");
            if (tupleParts.size() > 1U)
            {
                auto tupleNode = makeNode("TuplePattern");
                for (const auto &part : tupleParts)
                {
                    tupleNode->children.push_back(parsePatternTokens(part));
                }
                return tupleNode;
            }
            return parsePatternTokens(inner);
        }

        if (isFullyWrapped(slice, "[", "]"))
        {
            vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            auto listNode = makeNode("ListPattern");
            if (!inner.empty())
            {
                auto parts = splitTopLevel(inner, ",");
                for (const auto &part : parts)
                {
                    listNode->children.push_back(parsePatternTokens(part));
                }
            }
            return listNode;
        }

        auto parts = splitApplicationTerms(slice);
        if (parts.size() > 1U)
        {
            auto appNode = makeNode("PatternApply");
            for (const auto &part : parts)
            {
                appNode->children.push_back(parsePatternTokens(part));
            }
            return appNode;
        }

        if (slice.size() == 1U)
        {
            return parseLeaf(slice.front());
        }

        auto fallback = makeNode("Pattern");
        fallback->children.push_back(makeNode(tokensToText(slice)));
        return fallback;
    }

    shared_ptr<AstNode> parseBindingTokens(const vector<Token> &slice, bool requireIn)
    {
        auto bindingNode = makeNode(requireIn ? "LetBinding" : "LetStatement");
        if (slice.empty())
        {
            bindingNode->children.push_back(makeNode("<empty>"));
            return bindingNode;
        }

        int equalIndex = findTopLevelToken(slice, {"="});
        if (equalIndex < 0)
        {
            recordSyntaxError(slice, "expected '=' in let binding");
            bindingNode->children.push_back(makeNode(tokensToText(slice)));
            return bindingNode;
        }

        int colonIndex = findTopLevelToken(vector<Token>(slice.begin(), slice.begin() + equalIndex), {":"});
        if (slice.front().kind == TokenKind::Identifier || slice.front().kind == TokenKind::TypeName)
        {
            bindingNode->children.push_back(makeNode("Name: " + slice.front().value));
        }
        else
        {
            recordSyntaxError(slice, "let binding must start with an identifier");
            bindingNode->children.push_back(makeNode("Name: <invalid>"));
        }

        if (colonIndex >= 0)
        {
            vector<Token> typeTokens(slice.begin() + colonIndex + 1, slice.begin() + equalIndex);
            auto typeNode = makeNode("Type");
            typeNode->children.push_back(parseTypeTokens(typeTokens));
            bindingNode->children.push_back(typeNode);
        }

        int valueStart = colonIndex >= 0 ? equalIndex + 1 : equalIndex + 1;
        vector<Token> valueTokens(slice.begin() + valueStart, slice.end());
        auto valueNode = makeNode("Value");
        valueNode->children.push_back(parseExpressionTokens(valueTokens));
        bindingNode->children.push_back(valueNode);
        return bindingNode;
    }

    shared_ptr<AstNode> parseExpressionTokens(const vector<Token> &slice)
    {
        if (slice.empty())
        {
            return makeNode("Expr: <empty>");
        }

        if (isFullyWrapped(slice, "(", ")"))
        {
            vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            if (inner.empty())
            {
                return makeNode("Unit");
            }
            auto tupleParts = splitTopLevel(inner, ",");
            if (tupleParts.size() > 1U)
            {
                auto tupleNode = makeNode("Tuple");
                for (const auto &part : tupleParts)
                {
                    tupleNode->children.push_back(parseExpressionTokens(part));
                }
                return tupleNode;
            }
            return parseExpressionTokens(inner);
        }

        if (slice.front().value == "if")
        {
            int thenIndex = findTopLevelToken(slice, {"then"});
            int elseIndex = findTopLevelToken(slice, {"else"});

            if (thenIndex < 0 || elseIndex < 0 || thenIndex >= elseIndex)
            {
                recordSyntaxError(slice, "invalid if expression");
                return makeNode("If: <invalid>");
            }

            vector<Token> condTokens(slice.begin() + 1, slice.begin() + thenIndex);
            vector<Token> thenTokens(slice.begin() + thenIndex + 1, slice.begin() + elseIndex);
            vector<Token> elseTokens(slice.begin() + elseIndex + 1, slice.end());

            auto ifNode = makeNode("If");
            auto condNode = makeNode("Condition");
            condNode->children.push_back(parseExpressionTokens(condTokens));
            auto thenNode = makeNode("Then");
            thenNode->children.push_back(parseExpressionTokens(thenTokens));
            auto elseNode = makeNode("Else");
            elseNode->children.push_back(parseExpressionTokens(elseTokens));
            ifNode->children = {condNode, thenNode, elseNode};
            return ifNode;
        }

        if (slice.front().value == "let")
        {
            int inIndex = findTopLevelToken(slice, {"in"});
            if (inIndex < 0)
            {
                return parseBindingTokens(vector<Token>(slice.begin() + 1, slice.end()), false);
            }

            vector<Token> bindingTokens(slice.begin() + 1, slice.begin() + inIndex);
            vector<Token> bodyTokens(slice.begin() + inIndex + 1, slice.end());

            auto letNode = makeNode("LetIn");
            letNode->children.push_back(parseBindingTokens(bindingTokens, true));
            auto inNode = makeNode("In");
            inNode->children.push_back(parseExpressionTokens(bodyTokens));
            letNode->children.push_back(inNode);
            return letNode;
        }

        static const vector<set<string>> precedence = {
            {"&&", "||"},
            {">", "<", ">=", "<=", "==", "/="},
            {"++"},
            {"+", "-"},
            {"*", "/"}};

        for (const auto &level : precedence)
        {
            int opIndex = findTopLevelToken(slice, level, true);
            if (opIndex > 0 && static_cast<size_t>(opIndex + 1) < slice.size())
            {
                auto opNode = makeNode("Operator: " + slice[static_cast<size_t>(opIndex)].value);
                vector<Token> left(slice.begin(), slice.begin() + opIndex);
                vector<Token> right(slice.begin() + opIndex + 1, slice.end());
                opNode->children.push_back(parseExpressionTokens(left));
                opNode->children.push_back(parseExpressionTokens(right));
                return opNode;
            }
        }

        if (isFullyWrapped(slice, "[", "]"))
        {
            vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            auto listNode = makeNode("List");
            if (!inner.empty())
            {
                auto parts = splitTopLevel(inner, ",");
                for (const auto &part : parts)
                {
                    listNode->children.push_back(parseExpressionTokens(part));
                }
            }
            return listNode;
        }

        auto parts = splitApplicationTerms(slice);
        if (parts.size() > 1U)
        {
            auto applyNode = makeNode("Apply");
            for (const auto &part : parts)
            {
                applyNode->children.push_back(parseExpressionTokens(part));
            }
            return applyNode;
        }

        if (slice.size() == 1U)
        {
            if (slice.front().kind == TokenKind::Error)
            {
                recordSyntaxError(slice, "lexical error inside expression");
                return makeNode("ErrorExpr");
            }
            return parseLeaf(slice.front());
        }

        recordSyntaxError(slice, "could not parse expression: " + tokensToText(slice));
        auto fallback = makeNode("Expr");
        fallback->children.push_back(makeNode(tokensToText(slice)));
        return fallback;
    }

    shared_ptr<AstNode> parseCaseBlock(size_t start, size_t end)
    {
        const auto &caseLine = lines[start];
        auto caseNode = makeNode("Case");
        int ofIndex = findTopLevelToken(caseLine.tokens, {"of"});
        if (ofIndex < 0)
        {
            recordSyntaxError(caseLine.tokens, "case expression must contain 'of'");
            caseNode->children.push_back(makeNode(tokensToText(caseLine.tokens)));
            return caseNode;
        }

        vector<Token> subjectTokens(caseLine.tokens.begin() + 1, caseLine.tokens.begin() + ofIndex);
        auto subjectNode = makeNode("Subject");
        subjectNode->children.push_back(parseExpressionTokens(subjectTokens));
        caseNode->children.push_back(subjectNode);

        size_t branchStart = start + 1;
        if (branchStart >= end || lines[branchStart].indent <= caseLine.indent)
        {
            recordSyntaxError(caseLine.tokens, "case expression must have indented branches");
            return caseNode;
        }

        while (branchStart < end)
        {
            const auto &branchLine = lines[branchStart];
            if (branchLine.indent <= caseLine.indent)
            {
                break;
            }

            size_t branchEnd = branchStart + 1;
            while (branchEnd < end && lines[branchEnd].indent > branchLine.indent)
            {
                ++branchEnd;
            }

            auto branchNode = makeNode("Branch");
            int arrowIndex = findTopLevelToken(branchLine.tokens, {"=>"});
            if (arrowIndex < 0)
            {
                recordSyntaxError(branchLine.tokens, "case branch must contain '=>'");
                branchNode->children.push_back(makeNode(tokensToText(branchLine.tokens)));
            }
            else
            {
                vector<Token> patternTokens(branchLine.tokens.begin(), branchLine.tokens.begin() + arrowIndex);
                vector<Token> rhsTokens(branchLine.tokens.begin() + arrowIndex + 1, branchLine.tokens.end());

                auto patternNode = makeNode("Pattern");
                patternNode->children.push_back(parsePatternTokens(patternTokens));
                branchNode->children.push_back(patternNode);

                auto valueNode = makeNode("Value");
                if (!rhsTokens.empty())
                {
                    valueNode->children.push_back(parseExpressionTokens(rhsTokens));
                }
                else if (branchEnd > branchStart + 1)
                {
                    valueNode->children.push_back(parseExpressionTokens(flattenLines(branchStart + 1, branchEnd, lines)));
                }
                else
                {
                    recordSyntaxError(branchLine.tokens, "empty case branch body");
                    valueNode->children.push_back(makeNode("<empty>"));
                }
                branchNode->children.push_back(valueNode);
            }

            caseNode->children.push_back(branchNode);
            branchStart = branchEnd;
        }

        return caseNode;
    }

    shared_ptr<AstNode> parseDoBlock(size_t start, size_t end, const vector<Token> &inlineTokens = {})
    {
        auto doNode = makeNode("Do");
        if (!inlineTokens.empty())
        {
            vector<Token> tail(inlineTokens.begin() + 1, inlineTokens.end());
            if (!tail.empty())
            {
                auto statementNode = makeNode("Statement");
                statementNode->children.push_back(parseExpressionTokens(tail));
                doNode->children.push_back(statementNode);
            }
        }

        if (start >= end)
        {
            recordSyntaxError(inlineTokens, "do block must contain at least one statement");
            return doNode;
        }

        size_t statementIndex = start;
        int blockIndent = lines[start].indent;
        while (statementIndex < end)
        {
            if (lines[statementIndex].indent != blockIndent)
            {
                recordSyntaxError(lines[statementIndex].tokens, "unexpected indentation inside do block");
                ++statementIndex;
                continue;
            }

            size_t statementEnd = statementIndex + 1;
            while (statementEnd < end && lines[statementEnd].indent > lines[statementIndex].indent)
            {
                ++statementEnd;
            }

            const auto &statementLine = lines[statementIndex];
            auto statementNode = makeNode("Statement");

            if (!statementLine.tokens.empty() && statementLine.tokens.front().value == "let")
            {
                statementNode->children.push_back(parseBindingTokens(vector<Token>(statementLine.tokens.begin() + 1, statementLine.tokens.end()), false));
            }
            else if (!statementLine.tokens.empty() && statementLine.tokens.front().value == "case")
            {
                statementNode->children.push_back(parseCaseBlock(statementIndex, statementEnd));
            }
            else if (!statementLine.tokens.empty() && statementLine.tokens.front().value == "do")
            {
                statementNode->children.push_back(parseDoBlock(statementIndex + 1, statementEnd, statementLine.tokens));
            }
            else
            {
                vector<Token> expressionTokens = statementLine.tokens;
                if (statementEnd > statementIndex + 1)
                {
                    auto nested = flattenLines(statementIndex + 1, statementEnd, lines);
                    expressionTokens.insert(expressionTokens.end(), nested.begin(), nested.end());
                }
                statementNode->children.push_back(parseExpressionTokens(expressionTokens));
            }

            doNode->children.push_back(statementNode);
            statementIndex = statementEnd;
        }

        return doNode;
    }

    shared_ptr<AstNode> parseIndentedExpression(size_t start, size_t end)
    {
        if (start >= end)
        {
            return makeNode("Expr: <empty>");
        }

        const auto &firstLine = lines[start];
        if (!firstLine.tokens.empty() && firstLine.tokens.front().value == "case")
        {
            return parseCaseBlock(start, end);
        }
        if (!firstLine.tokens.empty() && firstLine.tokens.front().value == "do")
        {
            return parseDoBlock(start + 1, end, firstLine.tokens);
        }

        return parseExpressionTokens(flattenLines(start, end, lines));
    }

    shared_ptr<AstNode> parseImport()
    {
        const auto &line = lines[lineIndex];
        auto node = makeNode("Import");
        if (line.tokens.size() < 2U)
        {
            recordSyntaxError(line.tokens, "import statement is incomplete");
            node->children.push_back(makeNode("<missing module name>"));
        }
        else
        {
            string moduleName;
            for (size_t i = 1; i < line.tokens.size(); ++i)
            {
                moduleName += line.tokens[i].value;
            }
            node->children.push_back(makeNode("Module: " + moduleName));
        }
        ++lineIndex;
        return node;
    }

    shared_ptr<AstNode> parseTypeSignature()
    {
        const auto &line = lines[lineIndex];
        auto node = makeNode("TypeSignature");

        int colonIndex = findTopLevelToken(line.tokens, {":"});
        if (colonIndex <= 0)
        {
            recordSyntaxError(line.tokens, "invalid type signature");
            node->children.push_back(makeNode(tokensToText(line.tokens)));
            ++lineIndex;
            return node;
        }

        node->children.push_back(makeNode("Name: " + line.tokens.front().value));
        vector<Token> typeTokens(line.tokens.begin() + colonIndex + 1, line.tokens.end());
        auto typeNode = makeNode("Type");
        typeNode->children.push_back(parseTypeTokens(typeTokens));
        node->children.push_back(typeNode);

        ++lineIndex;
        return node;
    }

    shared_ptr<AstNode> parseFunctionDefinition()
    {
        const auto &line = lines[lineIndex];
        auto node = makeNode("FunctionDefinition");

        int equalIndex = findTopLevelToken(line.tokens, {"="});
        if (equalIndex <= 0)
        {
            recordSyntaxError(line.tokens, "function definition must contain '='");
            node->children.push_back(makeNode(tokensToText(line.tokens)));
            ++lineIndex;
            return node;
        }

        node->children.push_back(makeNode("Name: " + line.tokens.front().value));

        auto paramsNode = makeNode("Parameters");
        vector<Token> lhsTokens(line.tokens.begin() + 1, line.tokens.begin() + equalIndex);
        auto parameterSlices = splitApplicationTerms(lhsTokens);
        for (const auto &parameter : parameterSlices)
        {
            paramsNode->children.push_back(parsePatternTokens(parameter));
        }
        node->children.push_back(paramsNode);

        size_t blockEnd = findBlockEnd(lineIndex + 1, line.indent);
        vector<Token> rhsInline(line.tokens.begin() + equalIndex + 1, line.tokens.end());

        auto bodyNode = makeNode("Body");
        if (!rhsInline.empty())
        {
            if (rhsInline.front().value == "do")
            {
                bodyNode->children.push_back(parseDoBlock(lineIndex + 1, blockEnd, rhsInline));
            }
            else
            {
                vector<Token> fullExpression = rhsInline;
                if (blockEnd > lineIndex + 1)
                {
                    auto nestedTokens = flattenLines(lineIndex + 1, blockEnd, lines);
                    fullExpression.insert(fullExpression.end(), nestedTokens.begin(), nestedTokens.end());
                }
                bodyNode->children.push_back(parseExpressionTokens(fullExpression));
            }
        }
        else if (blockEnd > lineIndex + 1)
        {
            bodyNode->children.push_back(parseIndentedExpression(lineIndex + 1, blockEnd));
        }
        else
        {
            recordSyntaxError(line.tokens, "missing function body");
            bodyNode->children.push_back(makeNode("<empty>"));
        }
        node->children.push_back(bodyNode);

        lineIndex = blockEnd;
        return node;
    }

public:
    explicit IdrisParser(vector<Token> allTokens) : tokens(std::move(allTokens)), lines(buildLines(tokens)) {}

    shared_ptr<AstNode> parseProgram()
    {
        auto root = makeNode("Program");

        while (lineIndex < lines.size())
        {
            const auto &line = lines[lineIndex];

            if (hasLexicalError(line))
            {
                auto errorNode = makeNode("LexicalErrorLine");
                errorNode->children.push_back(makeNode("Line " + to_string(line.lineNumber) + ": " + tokensToText(line.tokens)));
                root->children.push_back(errorNode);
                ++lineIndex;
                continue;
            }

            if (line.indent != 1)
            {
                recordSyntaxError(line.tokens, "unexpected indentation at top level");
                auto errorNode = makeNode("SyntaxErrorLine");
                errorNode->children.push_back(makeNode(tokensToText(line.tokens)));
                root->children.push_back(errorNode);
                ++lineIndex;
                continue;
            }

            if (!line.tokens.empty() && line.tokens.front().value == "import")
            {
                root->children.push_back(parseImport());
                continue;
            }

            bool hasColon = findTopLevelToken(line.tokens, {":"}) >= 0;
            bool hasEqual = findTopLevelToken(line.tokens, {"="}) >= 0;

            if (hasColon && !hasEqual)
            {
                root->children.push_back(parseTypeSignature());
                continue;
            }

            if (hasEqual)
            {
                root->children.push_back(parseFunctionDefinition());
                continue;
            }

            recordSyntaxError(line.tokens, "unknown top-level construct");
            auto errorNode = makeNode("UnknownTopLevel");
            errorNode->children.push_back(makeNode(tokensToText(line.tokens)));
            root->children.push_back(errorNode);
            ++lineIndex;
        }

        return root;
    }

    const vector<string> &getSyntaxErrors() const
    {
        return syntaxErrors;
    }

    const vector<LogicalLine> &getLines() const
    {
        return lines;
    }
};

string readInputFile()
{
    const vector<string> candidates = {"input.txt", "build/input.txt", "lab3/build/input.txt"};

    for (const auto &path : candidates)
    {
        ifstream input(path);
        if (!input.is_open())
        {
            continue;
        }

        stringstream buffer;
        buffer << input.rdbuf();
        return buffer.str();
    }

    return {};
}

string tokenKindToString(TokenKind kind)
{
    switch (kind)
    {
    case TokenKind::Reserved:
        return "Reserved";
    case TokenKind::Identifier:
        return "Identifier";
    case TokenKind::TypeName:
        return "Type";
    case TokenKind::Number:
        return "Number";
    case TokenKind::StringLiteral:
        return "String";
    case TokenKind::CharLiteral:
        return "Char";
    case TokenKind::Operator:
        return "Operator";
    case TokenKind::Separator:
        return "Separator";
    case TokenKind::Newline:
        return "Newline";
    case TokenKind::EndOfFile:
        return "EOF";
    case TokenKind::Error:
        return "Error";
    }
    return "Unknown";
}

void printTree(const shared_ptr<AstNode> &node, const string &prefix = "", bool isLast = true, bool isRoot = true)
{
    if (!node)
    {
        return;
    }

    if (isRoot)
    {
        cout << node->label << '\n';
    }
    else
    {
        cout << prefix;
        cout << (isLast ? "└── " : "├── ");
        cout << node->label << '\n';
    }

    for (size_t i = 0; i < node->children.size(); ++i)
    {
        bool childIsLast = i + 1 == node->children.size();
        string childPrefix = isRoot ? "" : prefix + (isLast ? "    " : "│   ");
        printTree(node->children[i], childPrefix, childIsLast, false);
    }
}

void printTokens(const vector<Token> &tokens)
{
    cout << string(70, '=') << '\n';
    cout << "TOKEN TABLE\n";
    cout << string(70, '=') << '\n';
    cout << left << setw(14) << "TYPE" << setw(28) << "VALUE" << setw(8) << "LINE" << setw(8) << "COLUMN" << '\n';
    cout << string(70, '-') << '\n';

    for (const auto &token : tokens)
    {
        cout << left << setw(14) << tokenKindToString(token.kind) << setw(28)
             << (token.kind == TokenKind::StringLiteral ? "\"" + token.value + "\"" : token.kind == TokenKind::CharLiteral ? "'" + token.value + "'" : token.value)
             << setw(8) << token.line << setw(8) << token.column << '\n';
    }
}

void printErrors(const vector<Token> &tokens, const vector<string> &syntaxErrors)
{
    vector<string> lexicalErrors;
    for (const auto &token : tokens)
    {
        if (token.kind == TokenKind::Error)
        {
            lexicalErrors.push_back("Line " + to_string(token.line) + ", column " + to_string(token.column) + ": " + token.value);
        }
    }

    cout << "\n" << string(70, '=') << '\n';
    cout << "LEXICAL ERRORS\n";
    cout << string(70, '=') << '\n';
    if (lexicalErrors.empty())
    {
        cout << "No lexical errors found.\n";
    }
    else
    {
        for (const auto &error : lexicalErrors)
        {
            cout << error << '\n';
        }
    }

    cout << "\n" << string(70, '=') << '\n';
    cout << "SYNTAX ERRORS\n";
    cout << string(70, '=') << '\n';
    if (syntaxErrors.empty())
    {
        cout << "No syntax errors found.\n";
    }
    else
    {
        for (const auto &error : syntaxErrors)
        {
            cout << error << '\n';
        }
    }
}

int main()
{
    string sourceCode = readInputFile();
    if (sourceCode.empty())
    {
        cout << "Error: could not open input.txt" << endl;
        return 1;
    }

    cout << string(70, '=') << '\n';
    cout << "SOURCE CODE\n";
    cout << string(70, '=') << '\n';
    cout << sourceCode << '\n';

    IdrisLexer lexer(sourceCode);
    vector<Token> tokens = lexer.analyze();
    printTokens(tokens);

    IdrisParser parser(tokens);
    (void)parser.parseProgram();
    idris_cst::IdrisCstBuilder cstBuilder(parser.getLines());
    auto cst = cstBuilder.buildProgram();

    cout << "\n" << string(70, '=') << '\n';
    cout << "CONCRETE SYNTAX TREE (CST)\n";
    cout << string(70, '=') << '\n';
    idris_cst::printTree(cst);

    const string svgPath = "lab3_cst.svg";
    const string dotPath = "lab3_cst.dot";
    string graphvizError;

    cout << "\n" << string(70, '=') << '\n';
    cout << "GRAPHVIZ VISUALIZATION\n";
    cout << string(70, '=') << '\n';
    if (idris_cst::renderGraphviz(cst, svgPath, dotPath, &graphvizError))
    {
        cout << "Saved SVG: " << svgPath << '\n';
        cout << "Saved DOT: " << dotPath << '\n';
    }
    else
    {
        cout << "Graphviz export failed: " << graphvizError << '\n';
    }

    printErrors(tokens, parser.getSyntaxErrors());
    return 0;
}
