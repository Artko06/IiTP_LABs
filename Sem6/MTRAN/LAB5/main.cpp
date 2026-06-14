#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
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

struct FunctionType
{
    vector<string> parameterTypes;
    string returnType = "Unknown";
    int line = 0;
};

struct SymbolEntry
{
    string scope;
    string name;
    string type;
    int line = 0;
};

class SemanticAnalyzer
{
private:
    const vector<Token> &tokens;
    const vector<LogicalLine> &lines;
    map<string, FunctionType> functionTable;
    map<string, string> globalValues;
    vector<SymbolEntry> symbols;
    vector<string> semanticErrors;

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
            const string closeToken = value == "(" ? ")" : "]";
            int depth = 0;
            for (size_t i = start; i < slice.size(); ++i)
            {
                if (slice[i].value == value)
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

    void addError(const vector<Token> &context, const string &message)
    {
        if (!context.empty())
        {
            semanticErrors.push_back("Line " + to_string(context.front().line) + ", column " + to_string(context.front().column) + ": " + message);
        }
        else
        {
            semanticErrors.push_back(message);
        }
    }

    void addSymbol(const string &scope, const string &name, const string &type, int line)
    {
        symbols.push_back({scope, name, type, line});
    }

    static string join(const vector<string> &parts, const string &separator)
    {
        string result;
        for (size_t i = 0; i < parts.size(); ++i)
        {
            if (i != 0U)
            {
                result += separator;
            }
            result += parts[i];
        }
        return result;
    }

    string normalizeTypeTokens(const vector<Token> &slice) const
    {
        if (slice.empty())
        {
            return "Unknown";
        }

        if (isFullyWrapped(slice, "(", ")"))
        {
            vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            if (inner.empty())
            {
                return "()";
            }

            auto parts = splitTopLevel(inner, ",");
            if (parts.size() > 1U)
            {
                vector<string> normalized;
                for (const auto &part : parts)
                {
                    normalized.push_back(normalizeTypeTokens(part));
                }
                return "(" + join(normalized, ", ") + ")";
            }
            return normalizeTypeTokens(inner);
        }

        auto terms = splitApplicationTerms(slice);
        if (terms.size() > 1U)
        {
            vector<string> normalized;
            for (const auto &term : terms)
            {
                normalized.push_back(normalizeTypeTokens(term));
            }
            return join(normalized, " ");
        }

        return slice.front().value;
    }

    pair<vector<string>, string> parseFunctionType(const vector<Token> &typeTokens) const
    {
        vector<string> parts;
        size_t start = 0;

        while (start < typeTokens.size())
        {
            vector<Token> rest(typeTokens.begin() + static_cast<long>(start), typeTokens.end());
            int arrowIndex = findTopLevelToken(rest, {"->"});
            if (arrowIndex < 0)
            {
                parts.push_back(normalizeTypeTokens(rest));
                break;
            }

            vector<Token> chunk(rest.begin(), rest.begin() + arrowIndex);
            parts.push_back(normalizeTypeTokens(chunk));
            start += static_cast<size_t>(arrowIndex) + 1;
        }

        if (parts.empty())
        {
            return {{}, "Unknown"};
        }

        string returnType = parts.back();
        parts.pop_back();
        return {parts, returnType};
    }

    static bool isNumericType(const string &type)
    {
        return type == "Int" || type == "Double" || type == "Nat" || type == "Integer" || type == "Bits8" ||
               type == "Numeric" || type == "IntLiteral" || type == "DoubleLiteral";
    }

    static bool isIntegralType(const string &type)
    {
        return type == "Int" || type == "Nat" || type == "Integer" || type == "Bits8" || type == "IntLiteral";
    }

    static bool isIOType(const string &type)
    {
        return type == "IO ()" || (type.size() > 3U && type.rfind("IO ", 0) == 0);
    }

    static bool isListLike(const string &type)
    {
        return type.rfind("List ", 0) == 0 || type.rfind("Vect ", 0) == 0;
    }

    static string listElementType(const string &type)
    {
        if (type.rfind("List ", 0) == 0)
        {
            return type.substr(5);
        }
        if (type.rfind("Vect ", 0) == 0)
        {
            size_t firstSpace = type.find(' ');
            size_t secondSpace = type.find(' ', firstSpace + 1);
            if (secondSpace != string::npos)
            {
                return type.substr(secondSpace + 1);
            }
        }
        return "Unknown";
    }

    static vector<string> parseTupleType(const string &type)
    {
        vector<string> result;
        if (type.size() < 2U || type.front() != '(' || type.back() != ')')
        {
            return result;
        }

        string inner = type.substr(1, type.size() - 2);
        string current;
        int depth = 0;
        for (char ch : inner)
        {
            if (ch == '(')
            {
                depth++;
            }
            else if (ch == ')')
            {
                depth--;
            }

            if (ch == ',' && depth == 0)
            {
                if (!current.empty() && current.front() == ' ')
                {
                    current.erase(current.begin());
                }
                result.push_back(current);
                current.clear();
                continue;
            }

            current += ch;
        }

        if (!current.empty())
        {
            if (current.front() == ' ')
            {
                current.erase(current.begin());
            }
            result.push_back(current);
        }
        return result;
    }

    static bool areTypesCompatible(const string &expected, const string &actual)
    {
        if (expected == "Unknown" || actual == "Unknown")
        {
            return true;
        }

        if (expected == actual)
        {
            return true;
        }

        if (expected == "Double" && (actual == "IntLiteral" || actual == "DoubleLiteral" || actual == "Int" || actual == "Nat" || actual == "Integer" || actual == "Bits8"))
        {
            return true;
        }

        if (isIntegralType(expected) && actual == "IntLiteral")
        {
            return true;
        }

        if (isNumericType(expected) && isNumericType(actual))
        {
            return true;
        }

        if (isListLike(expected) && isListLike(actual))
        {
            return areTypesCompatible(listElementType(expected), listElementType(actual));
        }

        vector<string> expectedTuple = parseTupleType(expected);
        vector<string> actualTuple = parseTupleType(actual);
        if (!expectedTuple.empty() && expectedTuple.size() == actualTuple.size())
        {
            for (size_t i = 0; i < expectedTuple.size(); ++i)
            {
                if (!areTypesCompatible(expectedTuple[i], actualTuple[i]))
                {
                    return false;
                }
            }
            return true;
        }

        return false;
    }

    static string mergeTypes(const string &left, const string &right)
    {
        if (left == right)
        {
            return left;
        }
        if (left == "Unknown")
        {
            return right;
        }
        if (right == "Unknown")
        {
            return left;
        }
        if (isNumericType(left) && isNumericType(right))
        {
            if (left == "Double" || right == "Double" || left == "DoubleLiteral" || right == "DoubleLiteral")
            {
                return "Double";
            }
            return "Int";
        }
        return "Unknown";
    }

    string inferAtomType(const vector<Token> &slice, const map<string, string> &env)
    {
        if (slice.empty())
        {
            return "Unknown";
        }

        if (isFullyWrapped(slice, "(", ")"))
        {
            vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            if (inner.empty())
            {
                return "()";
            }

            auto tupleParts = splitTopLevel(inner, ",");
            if (tupleParts.size() > 1U)
            {
                vector<string> tupleTypes;
                for (const auto &part : tupleParts)
                {
                    tupleTypes.push_back(inferExpressionType(part, env));
                }
                return "(" + join(tupleTypes, ", ") + ")";
            }

            return inferExpressionType(inner, env);
        }

        if (isFullyWrapped(slice, "[", "]"))
        {
            vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            if (inner.empty())
            {
                return "List Unknown";
            }

            auto parts = splitTopLevel(inner, ",");
            string elementType = "Unknown";
            for (const auto &part : parts)
            {
                elementType = mergeTypes(elementType, inferExpressionType(part, env));
            }
            return "List " + elementType;
        }

        if (slice.size() == 1U)
        {
            const Token &token = slice.front();
            if (token.kind == TokenKind::Number)
            {
                return token.value.find('.') != string::npos ? "DoubleLiteral" : "IntLiteral";
            }
            if (token.kind == TokenKind::StringLiteral)
            {
                return "String";
            }
            if (token.kind == TokenKind::CharLiteral)
            {
                return "Char";
            }
            if (token.kind == TokenKind::Identifier || token.kind == TokenKind::TypeName)
            {
                auto localIt = env.find(token.value);
                if (localIt != env.end())
                {
                    return localIt->second;
                }
                auto globalValueIt = globalValues.find(token.value);
                if (globalValueIt != globalValues.end())
                {
                    return globalValueIt->second;
                }
                if (token.value == "Z")
                {
                    return "Nat";
                }
                if (token.value == "_")
                {
                    return "Unknown";
                }
            }
        }

        return "Unknown";
    }

    string inferApplicationType(const vector<vector<Token>> &terms, const map<string, string> &env)
    {
        if (terms.empty())
        {
            return "Unknown";
        }

        if (terms.size() == 1U)
        {
            return inferAtomType(terms.front(), env);
        }

        if (terms.front().size() == 1U)
        {
            const string functionName = terms.front().front().value;

            if (functionName == "S")
            {
                if (terms.size() != 2U)
                {
                    addError(terms.front(), "constructor S expects one argument");
                    return "Nat";
                }
                string argumentType = inferExpressionType(terms[1], env);
                if (!areTypesCompatible("Nat", argumentType))
                {
                    addError(terms[1], "constructor S expects argument of type Nat, got " + argumentType);
                }
                return "Nat";
            }

            if (functionName == "cast")
            {
                if (terms.size() != 2U)
                {
                    addError(terms.front(), "function cast expects one argument");
                }
                return "Numeric";
            }

            if (functionName == "show")
            {
                if (terms.size() != 2U)
                {
                    addError(terms.front(), "function show expects one argument");
                }
                return "String";
            }

            auto functionIt = functionTable.find(functionName);
            if (functionIt != functionTable.end())
            {
                const FunctionType &functionType = functionIt->second;
                const size_t argumentCount = terms.size() - 1;
                if (argumentCount != functionType.parameterTypes.size())
                {
                    addError(terms.front(), "function " + functionName + " expects " + to_string(functionType.parameterTypes.size()) +
                                                " argument(s), got " + to_string(argumentCount));
                }

                size_t comparable = min(argumentCount, functionType.parameterTypes.size());
                for (size_t i = 0; i < comparable; ++i)
                {
                    string actualType = inferExpressionType(terms[i + 1], env);
                    if (!areTypesCompatible(functionType.parameterTypes[i], actualType))
                    {
                        addError(terms[i + 1], "argument " + to_string(i + 1) + " of " + functionName + " has type " + actualType +
                                                   ", expected " + functionType.parameterTypes[i]);
                    }
                }
                return functionType.returnType;
            }
        }

        return "Unknown";
    }

    string inferExpressionType(const vector<Token> &slice, const map<string, string> &env)
    {
        if (slice.empty())
        {
            return "Unknown";
        }

        if (slice.front().value == "if")
        {
            int thenIndex = findTopLevelToken(slice, {"then"});
            int elseIndex = findTopLevelToken(slice, {"else"});
            if (thenIndex < 0 || elseIndex < 0 || thenIndex >= elseIndex)
            {
                return "Unknown";
            }

            vector<Token> condition(slice.begin() + 1, slice.begin() + thenIndex);
            vector<Token> thenTokens(slice.begin() + thenIndex + 1, slice.begin() + elseIndex);
            vector<Token> elseTokens(slice.begin() + elseIndex + 1, slice.end());

            string conditionType = inferExpressionType(condition, env);
            if (!areTypesCompatible("Bool", conditionType))
            {
                addError(condition, "if condition must have type Bool, got " + conditionType);
            }

            string thenType = inferExpressionType(thenTokens, env);
            string elseType = inferExpressionType(elseTokens, env);
            if (!areTypesCompatible(thenType, elseType) && !areTypesCompatible(elseType, thenType))
            {
                addError(slice, "if branches have incompatible types: " + thenType + " and " + elseType);
                return "Unknown";
            }
            return mergeTypes(thenType, elseType);
        }

        if (slice.front().value == "let")
        {
            int inIndex = findTopLevelToken(slice, {"in"});
            if (inIndex < 0)
            {
                return "Unknown";
            }

            vector<Token> binding(slice.begin() + 1, slice.begin() + inIndex);
            vector<Token> body(slice.begin() + inIndex + 1, slice.end());
            map<string, string> nestedEnv = env;
            analyzeLetBinding(binding, nestedEnv, "let");
            return inferExpressionType(body, nestedEnv);
        }

        if (isFullyWrapped(slice, "(", ")") || isFullyWrapped(slice, "[", "]"))
        {
            return inferAtomType(slice, env);
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
                vector<Token> left(slice.begin(), slice.begin() + opIndex);
                vector<Token> right(slice.begin() + opIndex + 1, slice.end());
                const string op = slice[static_cast<size_t>(opIndex)].value;
                const string leftType = inferExpressionType(left, env);
                const string rightType = inferExpressionType(right, env);

                if (op == "&&" || op == "||")
                {
                    if (!areTypesCompatible("Bool", leftType))
                    {
                        addError(left, "left operand of " + op + " must be Bool, got " + leftType);
                    }
                    if (!areTypesCompatible("Bool", rightType))
                    {
                        addError(right, "right operand of " + op + " must be Bool, got " + rightType);
                    }
                    return "Bool";
                }

                if (op == "++")
                {
                    if (areTypesCompatible("String", leftType) && areTypesCompatible("String", rightType))
                    {
                        return "String";
                    }
                    if (isListLike(leftType) && isListLike(rightType))
                    {
                        return leftType;
                    }
                    addError(slice, "operator ++ requires String or compatible list operands, got " + leftType + " and " + rightType);
                    return "Unknown";
                }

                if (op == ">" || op == "<" || op == ">=" || op == "<=")
                {
                    if (!(isNumericType(leftType) && isNumericType(rightType)))
                    {
                        addError(slice, "comparison operator " + op + " requires numeric operands, got " + leftType + " and " + rightType);
                    }
                    return "Bool";
                }

                if (op == "==" || op == "/=")
                {
                    if (!areTypesCompatible(leftType, rightType) && !areTypesCompatible(rightType, leftType))
                    {
                        addError(slice, "equality operator " + op + " requires compatible operands, got " + leftType + " and " + rightType);
                    }
                    return "Bool";
                }

                if (!(isNumericType(leftType) && isNumericType(rightType)))
                {
                    addError(slice, "operator " + op + " requires numeric operands, got " + leftType + " and " + rightType);
                    return "Unknown";
                }
                return mergeTypes(leftType, rightType);
            }
        }

        auto terms = splitApplicationTerms(slice);
        if (!terms.empty())
        {
            return inferApplicationType(terms, env);
        }

        return "Unknown";
    }

    void bindPattern(const vector<Token> &pattern, const string &expectedType, map<string, string> &env, const string &scope)
    {
        if (pattern.empty())
        {
            return;
        }

        if (isFullyWrapped(pattern, "(", ")"))
        {
            vector<Token> inner(pattern.begin() + 1, pattern.end() - 1);
            if (inner.empty())
            {
                if (!areTypesCompatible("()", expectedType))
                {
                    addError(pattern, "unit pattern is incompatible with type " + expectedType);
                }
                return;
            }

            auto parts = splitTopLevel(inner, ",");
            if (parts.size() > 1U)
            {
                vector<string> tupleTypes = parseTupleType(expectedType);
                if (!tupleTypes.empty() && tupleTypes.size() != parts.size())
                {
                    addError(pattern, "tuple pattern arity does not match type " + expectedType);
                }

                for (size_t i = 0; i < parts.size(); ++i)
                {
                    string partType = i < tupleTypes.size() ? tupleTypes[i] : "Unknown";
                    bindPattern(parts[i], partType, env, scope);
                }
                return;
            }

            bindPattern(inner, expectedType, env, scope);
            return;
        }

        auto terms = splitApplicationTerms(pattern);
        if (terms.size() > 1U && terms.front().size() == 1U && terms.front().front().value == "S")
        {
            if (!areTypesCompatible("Nat", expectedType))
            {
                addError(pattern, "pattern S ... expects Nat, got " + expectedType);
            }
            bindPattern(terms[1], "Nat", env, scope);
            return;
        }

        if (pattern.size() == 1U)
        {
            const Token &token = pattern.front();
            if (token.value == "_")
            {
                return;
            }

            if (token.kind == TokenKind::Identifier && !token.value.empty() && isupper(static_cast<unsigned char>(token.value.front())))
            {
                if (token.value == "Z" && !areTypesCompatible("Nat", expectedType))
                {
                    addError(pattern, "pattern Z expects Nat, got " + expectedType);
                }
                return;
            }

            if (token.kind == TokenKind::Identifier || token.kind == TokenKind::TypeName)
            {
                env[token.value] = expectedType;
                addSymbol(scope, token.value, expectedType, token.line);
                return;
            }

            string literalType = inferExpressionType(pattern, env);
            if (!areTypesCompatible(expectedType, literalType))
            {
                addError(pattern, "pattern literal has type " + literalType + ", expected " + expectedType);
            }
        }
    }

    void analyzeLetBinding(const vector<Token> &binding, map<string, string> &env, const string &scope)
    {
        int equalIndex = findTopLevelToken(binding, {"="});
        if (equalIndex < 0 || binding.empty())
        {
            addError(binding, "invalid let binding");
            return;
        }

        string name = binding.front().value;
        int colonIndex = findTopLevelToken(vector<Token>(binding.begin(), binding.begin() + equalIndex), {":"});
        string declaredType = "Unknown";
        if (colonIndex >= 0)
        {
            vector<Token> typeTokens(binding.begin() + colonIndex + 1, binding.begin() + equalIndex);
            declaredType = normalizeTypeTokens(typeTokens);
        }

        vector<Token> valueTokens(binding.begin() + equalIndex + 1, binding.end());
        string actualType = inferExpressionType(valueTokens, env);
        string finalType = declaredType == "Unknown" ? actualType : declaredType;

        if (declaredType != "Unknown" && !areTypesCompatible(declaredType, actualType))
        {
            addError(valueTokens, "value of let binding " + name + " has type " + actualType + ", expected " + declaredType);
        }

        env[name] = finalType;
        addSymbol(scope, name, finalType, binding.front().line);
    }

    string analyzeCaseBlock(size_t start, size_t end, const map<string, string> &env, const string &scope)
    {
        const auto &caseLine = lines[start];
        int ofIndex = findTopLevelToken(caseLine.tokens, {"of"});
        if (ofIndex < 0)
        {
            addError(caseLine.tokens, "case expression must contain 'of'");
            return "Unknown";
        }

        vector<Token> subjectTokens(caseLine.tokens.begin() + 1, caseLine.tokens.begin() + ofIndex);
        string subjectType = inferExpressionType(subjectTokens, env);

        string resultType = "Unknown";
        size_t branchIndex = start + 1;
        while (branchIndex < end)
        {
            const auto &branchLine = lines[branchIndex];
            if (branchLine.indent <= caseLine.indent)
            {
                break;
            }

            size_t branchEnd = branchIndex + 1;
            while (branchEnd < end && lines[branchEnd].indent > branchLine.indent)
            {
                ++branchEnd;
            }

            int arrowIndex = findTopLevelToken(branchLine.tokens, {"=>"});
            if (arrowIndex < 0)
            {
                addError(branchLine.tokens, "case branch must contain '=>'");
                branchIndex = branchEnd;
                continue;
            }

            map<string, string> branchEnv = env;
            vector<Token> patternTokens(branchLine.tokens.begin(), branchLine.tokens.begin() + arrowIndex);
            bindPattern(patternTokens, subjectType, branchEnv, scope);

            vector<Token> bodyTokens(branchLine.tokens.begin() + arrowIndex + 1, branchLine.tokens.end());
            string branchType = bodyTokens.empty()
                                    ? inferExpressionType(flattenLines(branchIndex + 1, branchEnd, lines), branchEnv)
                                    : inferExpressionType(bodyTokens, branchEnv);

            if (resultType == "Unknown")
            {
                resultType = branchType;
            }
            else if (!areTypesCompatible(resultType, branchType) && !areTypesCompatible(branchType, resultType))
            {
                addError(branchLine.tokens, "case branches have incompatible types: " + resultType + " and " + branchType);
                resultType = "Unknown";
            }

            branchIndex = branchEnd;
        }

        return resultType;
    }

    string analyzeDoBlock(size_t start, size_t end, const map<string, string> &incomingEnv, const string &scope)
    {
        map<string, string> env = incomingEnv;
        string blockType = "IO ()";

        size_t statementIndex = start;
        while (statementIndex < end)
        {
            const auto &line = lines[statementIndex];
            size_t statementEnd = statementIndex + 1;
            while (statementEnd < end && lines[statementEnd].indent > line.indent)
            {
                ++statementEnd;
            }

            if (!line.tokens.empty() && line.tokens.front().value == "let")
            {
                analyzeLetBinding(vector<Token>(line.tokens.begin() + 1, line.tokens.end()), env, scope);
                statementIndex = statementEnd;
                continue;
            }

            string statementType = "Unknown";
            if (!line.tokens.empty() && line.tokens.front().value == "case")
            {
                statementType = analyzeCaseBlock(statementIndex, statementEnd, env, scope);
            }
            else
            {
                vector<Token> expressionTokens = line.tokens;
                if (statementEnd > statementIndex + 1)
                {
                    auto nested = flattenLines(statementIndex + 1, statementEnd, lines);
                    expressionTokens.insert(expressionTokens.end(), nested.begin(), nested.end());
                }
                statementType = inferExpressionType(expressionTokens, env);
            }

            if (!isIOType(statementType) && statementType != "Unknown")
            {
                addError(line.tokens, "statement inside do-block must have IO type, got " + statementType);
            }

            blockType = statementType;
            statementIndex = statementEnd;
        }

        return blockType;
    }

    void registerBuiltins()
    {
        functionTable["putStrLn"] = {{"String"}, "IO ()", 0};
        functionTable["show"] = {{"Numeric"}, "String", 0};
        functionTable["cast"] = {{"Numeric"}, "Numeric", 0};
        functionTable["insert"] = {{"String", "Int", "SortedMap String Int"}, "SortedMap String Int", 0};
        functionTable["forLoop"] = {{"Nat"}, "IO ()", 0};
        globalValues["empty"] = "SortedMap String Int";
    }

    void collectGlobalSignatures()
    {
        for (const auto &line : lines)
        {
            if (line.indent != 1 || hasLexicalError(line))
            {
                continue;
            }

            int colonIndex = findTopLevelToken(line.tokens, {":"});
            int equalIndex = findTopLevelToken(line.tokens, {"="});
            if (colonIndex <= 0 || equalIndex >= 0)
            {
                continue;
            }

            string name = line.tokens.front().value;
            vector<Token> typeTokens(line.tokens.begin() + colonIndex + 1, line.tokens.end());
            auto parsed = parseFunctionType(typeTokens);
            functionTable[name] = {parsed.first, parsed.second, line.lineNumber};
            addSymbol("global", name, normalizeTypeTokens(typeTokens), line.lineNumber);
        }
    }

    void analyzeFunctionDefinitions()
    {
        size_t index = 0;
        while (index < lines.size())
        {
            const auto &line = lines[index];
            if (line.indent != 1 || hasLexicalError(line))
            {
                ++index;
                continue;
            }

            int equalIndex = findTopLevelToken(line.tokens, {"="});
            if (equalIndex <= 0)
            {
                ++index;
                continue;
            }

            string functionName = line.tokens.front().value;
            size_t blockEnd = findBlockEnd(index + 1, line.indent);
            map<string, string> env;

            auto functionIt = functionTable.find(functionName);
            FunctionType declaredType;
            if (functionIt != functionTable.end())
            {
                declaredType = functionIt->second;
            }

            vector<Token> lhsTokens(line.tokens.begin() + 1, line.tokens.begin() + equalIndex);
            auto parameterPatterns = splitApplicationTerms(lhsTokens);
            if (!declaredType.parameterTypes.empty() && parameterPatterns.size() != declaredType.parameterTypes.size())
            {
                addError(line.tokens, "definition of " + functionName + " has " + to_string(parameterPatterns.size()) +
                                          " parameter pattern(s), but signature declares " + to_string(declaredType.parameterTypes.size()));
            }

            for (size_t i = 0; i < parameterPatterns.size(); ++i)
            {
                string expectedType = i < declaredType.parameterTypes.size() ? declaredType.parameterTypes[i] : "Unknown";
                bindPattern(parameterPatterns[i], expectedType, env, functionName);
            }

            vector<Token> rhsInline(line.tokens.begin() + equalIndex + 1, line.tokens.end());
            string actualType = "Unknown";

            if (!rhsInline.empty())
            {
                if (rhsInline.front().value == "do")
                {
                    actualType = analyzeDoBlock(index + 1, blockEnd, env, functionName);
                }
                else if (rhsInline.front().value == "case")
                {
                    actualType = analyzeCaseBlock(index, blockEnd, env, functionName);
                }
                else
                {
                    vector<Token> fullExpression = rhsInline;
                    if (blockEnd > index + 1)
                    {
                        auto nested = flattenLines(index + 1, blockEnd, lines);
                        fullExpression.insert(fullExpression.end(), nested.begin(), nested.end());
                    }
                    actualType = inferExpressionType(fullExpression, env);
                }
            }
            else if (blockEnd > index + 1)
            {
                if (!lines[index + 1].tokens.empty() && lines[index + 1].tokens.front().value == "case")
                {
                    actualType = analyzeCaseBlock(index + 1, blockEnd, env, functionName);
                }
                else if (!lines[index + 1].tokens.empty() && lines[index + 1].tokens.front().value == "do")
                {
                    actualType = analyzeDoBlock(index + 2, blockEnd, env, functionName);
                }
                else
                {
                    actualType = inferExpressionType(flattenLines(index + 1, blockEnd, lines), env);
                }
            }

            if (!declaredType.returnType.empty() && declaredType.returnType != "Unknown" &&
                !areTypesCompatible(declaredType.returnType, actualType))
            {
                addError(line.tokens, "function " + functionName + " returns " + actualType + ", but signature declares " + declaredType.returnType);
            }

            index = blockEnd;
        }
    }

public:
    SemanticAnalyzer(const vector<Token> &allTokens, const vector<LogicalLine> &logicalLines)
        : tokens(allTokens), lines(logicalLines) {}

    void analyze()
    {
        registerBuiltins();
        collectGlobalSignatures();
        analyzeFunctionDefinitions();
    }

    const vector<string> &getErrors() const
    {
        return semanticErrors;
    }

    const vector<SymbolEntry> &getSymbols() const
    {
        return symbols;
    }
};

struct RuntimeValue
{
    enum class Kind
    {
        Int,
        Double,
        String,
        Char,
        Bool,
        Tuple,
        List,
        SortedMap,
        Unit
    };

    Kind kind = Kind::Unit;
    int intValue = 0;
    double doubleValue = 0.0;
    bool boolValue = false;
    char charValue = '\0';
    string stringValue;
    vector<RuntimeValue> elements;
    map<string, RuntimeValue> mapValue;

    static RuntimeValue makeInt(int value)
    {
        RuntimeValue result;
        result.kind = Kind::Int;
        result.intValue = value;
        return result;
    }

    static RuntimeValue makeDouble(double value)
    {
        RuntimeValue result;
        result.kind = Kind::Double;
        result.doubleValue = value;
        return result;
    }

    static RuntimeValue makeString(const string &value)
    {
        RuntimeValue result;
        result.kind = Kind::String;
        result.stringValue = value;
        return result;
    }

    static RuntimeValue makeChar(char value)
    {
        RuntimeValue result;
        result.kind = Kind::Char;
        result.charValue = value;
        return result;
    }

    static RuntimeValue makeBool(bool value)
    {
        RuntimeValue result;
        result.kind = Kind::Bool;
        result.boolValue = value;
        return result;
    }

    static RuntimeValue makeTuple(const vector<RuntimeValue> &items)
    {
        RuntimeValue result;
        result.kind = Kind::Tuple;
        result.elements = items;
        return result;
    }

    static RuntimeValue makeList(const vector<RuntimeValue> &items)
    {
        RuntimeValue result;
        result.kind = Kind::List;
        result.elements = items;
        return result;
    }

    static RuntimeValue makeSortedMap(const map<string, RuntimeValue> &items)
    {
        RuntimeValue result;
        result.kind = Kind::SortedMap;
        result.mapValue = items;
        return result;
    }

    static RuntimeValue makeUnit()
    {
        return RuntimeValue{};
    }

    string toString() const
    {
        switch (kind)
        {
        case Kind::Int:
            return to_string(intValue);
        case Kind::Double:
        {
            ostringstream out;
            out << doubleValue;
            return out.str();
        }
        case Kind::String:
            return stringValue;
        case Kind::Char:
            return string(1, charValue);
        case Kind::Bool:
            return boolValue ? "True" : "False";
        case Kind::Tuple:
        {
            vector<string> parts;
            for (const auto &item : elements)
            {
                parts.push_back(item.toString());
            }
            string joined;
            for (size_t i = 0; i < parts.size(); ++i)
            {
                if (i != 0U)
                {
                    joined += ", ";
                }
                joined += parts[i];
            }
            return "(" + joined + ")";
        }
        case Kind::List:
        {
            vector<string> parts;
            for (const auto &item : elements)
            {
                parts.push_back(item.toString());
            }
            string joined;
            for (size_t i = 0; i < parts.size(); ++i)
            {
                if (i != 0U)
                {
                    joined += ", ";
                }
                joined += parts[i];
            }
            return "[" + joined + "]";
        }
        case Kind::SortedMap:
        {
            string result = "{";
            bool first = true;
            for (const auto &entry : mapValue)
            {
                if (!first)
                {
                    result += ", ";
                }
                result += entry.first + ": " + entry.second.toString();
                first = false;
            }
            result += "}";
            return result;
        }
        case Kind::Unit:
            return "()";
        }
        return "()";
    }
};

struct FunctionClause
{
    string name;
    vector<vector<Token>> patterns;
    vector<Token> rhsInline;
    size_t nestedStart = 0;
    size_t nestedEnd = 0;
    int line = 0;
};

class IdrisInterpreter
{
private:
    const vector<Token> &tokens;
    const vector<LogicalLine> &lines;
    map<string, vector<FunctionClause>> functions;
    vector<string> runtimeOutput;
    vector<string> runtimeErrors;

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
            const string closeToken = value == "(" ? ")" : "]";
            int depth = 0;
            for (size_t i = start; i < slice.size(); ++i)
            {
                if (slice[i].value == value)
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

    void addRuntimeError(const vector<Token> &context, const string &message)
    {
        if (!context.empty())
        {
            runtimeErrors.push_back("Line " + to_string(context.front().line) + ", column " + to_string(context.front().column) + ": " + message);
        }
        else
        {
            runtimeErrors.push_back(message);
        }
    }

    double asNumber(const RuntimeValue &value) const
    {
        if (value.kind == RuntimeValue::Kind::Int)
        {
            return static_cast<double>(value.intValue);
        }
        if (value.kind == RuntimeValue::Kind::Double)
        {
            return value.doubleValue;
        }
        return 0.0;
    }

    bool equals(const RuntimeValue &left, const RuntimeValue &right) const
    {
        if (left.kind != right.kind)
        {
            if ((left.kind == RuntimeValue::Kind::Int || left.kind == RuntimeValue::Kind::Double) &&
                (right.kind == RuntimeValue::Kind::Int || right.kind == RuntimeValue::Kind::Double))
            {
                return asNumber(left) == asNumber(right);
            }
            return false;
        }

        switch (left.kind)
        {
        case RuntimeValue::Kind::Int:
            return left.intValue == right.intValue;
        case RuntimeValue::Kind::Double:
            return left.doubleValue == right.doubleValue;
        case RuntimeValue::Kind::String:
            return left.stringValue == right.stringValue;
        case RuntimeValue::Kind::Char:
            return left.charValue == right.charValue;
        case RuntimeValue::Kind::Bool:
            return left.boolValue == right.boolValue;
        case RuntimeValue::Kind::Unit:
            return true;
        default:
            return left.toString() == right.toString();
        }
    }

    bool matchPattern(const vector<Token> &pattern, const RuntimeValue &value, map<string, RuntimeValue> &bindings)
    {
        if (pattern.empty())
        {
            return false;
        }

        if (isFullyWrapped(pattern, "(", ")"))
        {
            vector<Token> inner(pattern.begin() + 1, pattern.end() - 1);
            if (inner.empty())
            {
                return value.kind == RuntimeValue::Kind::Unit;
            }

            auto parts = splitTopLevel(inner, ",");
            if (parts.size() > 1U)
            {
                if (value.kind != RuntimeValue::Kind::Tuple || value.elements.size() != parts.size())
                {
                    return false;
                }

                for (size_t i = 0; i < parts.size(); ++i)
                {
                    if (!matchPattern(parts[i], value.elements[i], bindings))
                    {
                        return false;
                    }
                }
                return true;
            }

            return matchPattern(inner, value, bindings);
        }

        auto terms = splitApplicationTerms(pattern);
        if (terms.size() == 2U && terms.front().size() == 1U && terms.front().front().value == "S")
        {
            if (value.kind != RuntimeValue::Kind::Int || value.intValue <= 0)
            {
                return false;
            }
            RuntimeValue predecessor = RuntimeValue::makeInt(value.intValue - 1);
            return matchPattern(terms[1], predecessor, bindings);
        }

        if (pattern.size() == 1U)
        {
            const Token &token = pattern.front();
            if (token.value == "_")
            {
                return true;
            }
            if (token.value == "Z")
            {
                return value.kind == RuntimeValue::Kind::Int && value.intValue == 0;
            }
            if (token.kind == TokenKind::StringLiteral)
            {
                return value.kind == RuntimeValue::Kind::String && value.stringValue == token.value;
            }
            if (token.kind == TokenKind::CharLiteral)
            {
                return value.kind == RuntimeValue::Kind::Char && value.charValue == token.value.front();
            }
            if (token.kind == TokenKind::Number)
            {
                if (token.value.find('.') != string::npos)
                {
                    return value.kind == RuntimeValue::Kind::Double && value.doubleValue == stod(token.value);
                }
                return value.kind == RuntimeValue::Kind::Int && value.intValue == stoi(token.value);
            }
            if (token.kind == TokenKind::Identifier || token.kind == TokenKind::TypeName)
            {
                if (!token.value.empty() && isupper(static_cast<unsigned char>(token.value.front())))
                {
                    return false;
                }
                bindings[token.value] = value;
                return true;
            }
        }

        return false;
    }

    RuntimeValue callFunction(const string &name, const vector<RuntimeValue> &arguments)
    {
        if (name == "putStrLn")
        {
            if (arguments.size() != 1U)
            {
                runtimeErrors.push_back("putStrLn expects one argument");
                return RuntimeValue::makeUnit();
            }
            runtimeOutput.push_back(arguments[0].toString());
            return RuntimeValue::makeUnit();
        }

        if (name == "show")
        {
            if (arguments.size() != 1U)
            {
                runtimeErrors.push_back("show expects one argument");
                return RuntimeValue::makeString("");
            }
            return RuntimeValue::makeString(arguments[0].toString());
        }

        if (name == "cast")
        {
            if (arguments.size() != 1U)
            {
                runtimeErrors.push_back("cast expects one argument");
                return RuntimeValue::makeInt(0);
            }
            return arguments[0];
        }

        if (name == "insert")
        {
            if (arguments.size() != 3U)
            {
                runtimeErrors.push_back("insert expects three arguments");
                return RuntimeValue::makeSortedMap({});
            }

            map<string, RuntimeValue> result = arguments[2].mapValue;
            result[arguments[0].toString()] = arguments[1];
            return RuntimeValue::makeSortedMap(result);
        }

        if (name == "empty")
        {
            return RuntimeValue::makeSortedMap({});
        }

        auto functionIt = functions.find(name);
        if (functionIt == functions.end())
        {
            runtimeErrors.push_back("unknown function: " + name);
            return RuntimeValue::makeUnit();
        }

        for (const auto &clause : functionIt->second)
        {
            if (clause.patterns.size() != arguments.size())
            {
                continue;
            }

            map<string, RuntimeValue> env;
            bool matched = true;
            for (size_t i = 0; i < clause.patterns.size(); ++i)
            {
                if (!matchPattern(clause.patterns[i], arguments[i], env))
                {
                    matched = false;
                    break;
                }
            }

            if (!matched)
            {
                continue;
            }

            if (!clause.rhsInline.empty())
            {
                if (clause.rhsInline.front().value == "do")
                {
                    return evaluateDoBlock(clause.nestedStart, clause.nestedEnd, env);
                }
                return evaluateExpression(clause.rhsInline, env);
            }

            if (clause.nestedStart < clause.nestedEnd)
            {
                if (!lines[clause.nestedStart].tokens.empty() && lines[clause.nestedStart].tokens.front().value == "case")
                {
                    return evaluateCaseBlock(clause.nestedStart, clause.nestedEnd, env);
                }
                if (!lines[clause.nestedStart].tokens.empty() && lines[clause.nestedStart].tokens.front().value == "do")
                {
                    return evaluateDoBlock(clause.nestedStart + 1, clause.nestedEnd, env);
                }
                return evaluateExpression(flattenLines(clause.nestedStart, clause.nestedEnd, lines), env);
            }

            return RuntimeValue::makeUnit();
        }

        runtimeErrors.push_back("no matching clause for function: " + name);
        return RuntimeValue::makeUnit();
    }

    RuntimeValue evaluateAtom(const vector<Token> &slice, map<string, RuntimeValue> &env)
    {
        if (slice.empty())
        {
            return RuntimeValue::makeUnit();
        }

        if (isFullyWrapped(slice, "(", ")"))
        {
            vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            if (inner.empty())
            {
                return RuntimeValue::makeUnit();
            }

            auto parts = splitTopLevel(inner, ",");
            if (parts.size() > 1U)
            {
                vector<RuntimeValue> values;
                for (const auto &part : parts)
                {
                    values.push_back(evaluateExpression(part, env));
                }
                return RuntimeValue::makeTuple(values);
            }
            return evaluateExpression(inner, env);
        }

        if (isFullyWrapped(slice, "[", "]"))
        {
            vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            vector<RuntimeValue> values;
            if (!inner.empty())
            {
                auto parts = splitTopLevel(inner, ",");
                for (const auto &part : parts)
                {
                    values.push_back(evaluateExpression(part, env));
                }
            }
            return RuntimeValue::makeList(values);
        }

        if (slice.size() == 1U)
        {
            const Token &token = slice.front();
            if (token.kind == TokenKind::Number)
            {
                if (token.value.find('.') != string::npos)
                {
                    return RuntimeValue::makeDouble(stod(token.value));
                }
                try
                {
                    return RuntimeValue::makeInt(stoi(token.value));
                }
                catch (const out_of_range &)
                {
                    return RuntimeValue::makeString(token.value);
                }
            }
            if (token.kind == TokenKind::StringLiteral)
            {
                return RuntimeValue::makeString(token.value);
            }
            if (token.kind == TokenKind::CharLiteral)
            {
                return RuntimeValue::makeChar(token.value.empty() ? '\0' : token.value.front());
            }
            if (token.kind == TokenKind::Identifier || token.kind == TokenKind::TypeName)
            {
                auto envIt = env.find(token.value);
                if (envIt != env.end())
                {
                    return envIt->second;
                }
                if (token.value == "empty")
                {
                    return RuntimeValue::makeSortedMap({});
                }
                auto fnIt = functions.find(token.value);
                if (fnIt != functions.end() && !fnIt->second.empty() && fnIt->second.front().patterns.empty())
                {
                    return callFunction(token.value, {});
                }
            }
        }

        addRuntimeError(slice, "unable to evaluate atom");
        return RuntimeValue::makeUnit();
    }

    RuntimeValue evaluateExpression(const vector<Token> &slice, map<string, RuntimeValue> &env)
    {
        if (slice.empty())
        {
            return RuntimeValue::makeUnit();
        }

        if (slice.front().value == "if")
        {
            int thenIndex = findTopLevelToken(slice, {"then"});
            int elseIndex = findTopLevelToken(slice, {"else"});
            vector<Token> conditionTokens(slice.begin() + 1, slice.begin() + thenIndex);
            vector<Token> thenTokens(slice.begin() + thenIndex + 1, slice.begin() + elseIndex);
            vector<Token> elseTokens(slice.begin() + elseIndex + 1, slice.end());

            RuntimeValue condition = evaluateExpression(conditionTokens, env);
            if (condition.kind != RuntimeValue::Kind::Bool)
            {
                addRuntimeError(conditionTokens, "if condition is not boolean");
                return RuntimeValue::makeUnit();
            }

            return condition.boolValue ? evaluateExpression(thenTokens, env) : evaluateExpression(elseTokens, env);
        }

        if (slice.front().value == "let")
        {
            int inIndex = findTopLevelToken(slice, {"in"});
            if (inIndex < 0)
            {
                addRuntimeError(slice, "let expression must contain in");
                return RuntimeValue::makeUnit();
            }

            vector<Token> binding(slice.begin() + 1, slice.begin() + inIndex);
            vector<Token> body(slice.begin() + inIndex + 1, slice.end());
            map<string, RuntimeValue> nestedEnv = env;
            evaluateLetBinding(binding, nestedEnv);
            return evaluateExpression(body, nestedEnv);
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
                vector<Token> leftTokens(slice.begin(), slice.begin() + opIndex);
                vector<Token> rightTokens(slice.begin() + opIndex + 1, slice.end());
                RuntimeValue left = evaluateExpression(leftTokens, env);
                RuntimeValue right = evaluateExpression(rightTokens, env);
                const string op = slice[static_cast<size_t>(opIndex)].value;

                if (op == "&&")
                {
                    return RuntimeValue::makeBool(left.boolValue && right.boolValue);
                }
                if (op == "||")
                {
                    return RuntimeValue::makeBool(left.boolValue || right.boolValue);
                }
                if (op == "++")
                {
                    return RuntimeValue::makeString(left.toString() + right.toString());
                }
                if (op == ">")
                {
                    return RuntimeValue::makeBool(asNumber(left) > asNumber(right));
                }
                if (op == "<")
                {
                    return RuntimeValue::makeBool(asNumber(left) < asNumber(right));
                }
                if (op == ">=")
                {
                    return RuntimeValue::makeBool(asNumber(left) >= asNumber(right));
                }
                if (op == "<=")
                {
                    return RuntimeValue::makeBool(asNumber(left) <= asNumber(right));
                }
                if (op == "==" || op == "/=")
                {
                    bool result = equals(left, right);
                    return RuntimeValue::makeBool(op == "==" ? result : !result);
                }
                if (op == "+")
                {
                    if (left.kind == RuntimeValue::Kind::Double || right.kind == RuntimeValue::Kind::Double)
                    {
                        return RuntimeValue::makeDouble(asNumber(left) + asNumber(right));
                    }
                    return RuntimeValue::makeInt(static_cast<int>(asNumber(left) + asNumber(right)));
                }
                if (op == "-")
                {
                    if (left.kind == RuntimeValue::Kind::Double || right.kind == RuntimeValue::Kind::Double)
                    {
                        return RuntimeValue::makeDouble(asNumber(left) - asNumber(right));
                    }
                    return RuntimeValue::makeInt(static_cast<int>(asNumber(left) - asNumber(right)));
                }
                if (op == "*")
                {
                    if (left.kind == RuntimeValue::Kind::Double || right.kind == RuntimeValue::Kind::Double)
                    {
                        return RuntimeValue::makeDouble(asNumber(left) * asNumber(right));
                    }
                    return RuntimeValue::makeInt(static_cast<int>(asNumber(left) * asNumber(right)));
                }
                if (op == "/")
                {
                    return RuntimeValue::makeDouble(asNumber(left) / asNumber(right));
                }
            }
        }

        auto terms = splitApplicationTerms(slice);
        if (terms.size() > 1U && terms.front().size() == 1U)
        {
            string functionName = terms.front().front().value;
            vector<RuntimeValue> arguments;
            for (size_t i = 1; i < terms.size(); ++i)
            {
                arguments.push_back(evaluateExpression(terms[i], env));
            }
            return callFunction(functionName, arguments);
        }

        return evaluateAtom(slice, env);
    }

    void evaluateLetBinding(const vector<Token> &binding, map<string, RuntimeValue> &env)
    {
        int equalIndex = findTopLevelToken(binding, {"="});
        if (equalIndex < 0 || binding.empty())
        {
            addRuntimeError(binding, "invalid let binding");
            return;
        }

        int colonIndex = findTopLevelToken(vector<Token>(binding.begin(), binding.begin() + equalIndex), {":"});
        int valueStart = equalIndex + 1;
        string name = binding.front().value;
        vector<Token> valueTokens(binding.begin() + valueStart, binding.end());
        env[name] = evaluateExpression(valueTokens, env);

        (void)colonIndex;
    }

    RuntimeValue evaluateCaseBlock(size_t start, size_t end, map<string, RuntimeValue> &env)
    {
        const auto &caseLine = lines[start];
        int ofIndex = findTopLevelToken(caseLine.tokens, {"of"});
        if (ofIndex < 0)
        {
            addRuntimeError(caseLine.tokens, "case expression must contain of");
            return RuntimeValue::makeUnit();
        }

        vector<Token> subjectTokens(caseLine.tokens.begin() + 1, caseLine.tokens.begin() + ofIndex);
        RuntimeValue subject = evaluateExpression(subjectTokens, env);

        size_t branchIndex = start + 1;
        while (branchIndex < end)
        {
            const auto &branchLine = lines[branchIndex];
            if (branchLine.indent <= caseLine.indent)
            {
                break;
            }

            size_t branchEnd = branchIndex + 1;
            while (branchEnd < end && lines[branchEnd].indent > branchLine.indent)
            {
                ++branchEnd;
            }

            int arrowIndex = findTopLevelToken(branchLine.tokens, {"=>"});
            if (arrowIndex < 0)
            {
                addRuntimeError(branchLine.tokens, "case branch must contain =>");
                branchIndex = branchEnd;
                continue;
            }

            vector<Token> patternTokens(branchLine.tokens.begin(), branchLine.tokens.begin() + arrowIndex);
            map<string, RuntimeValue> branchEnv = env;
            if (matchPattern(patternTokens, subject, branchEnv))
            {
                vector<Token> valueTokens(branchLine.tokens.begin() + arrowIndex + 1, branchLine.tokens.end());
                if (!valueTokens.empty())
                {
                    return evaluateExpression(valueTokens, branchEnv);
                }
                return evaluateExpression(flattenLines(branchIndex + 1, branchEnd, lines), branchEnv);
            }

            branchIndex = branchEnd;
        }

        addRuntimeError(caseLine.tokens, "no matching case branch");
        return RuntimeValue::makeUnit();
    }

    RuntimeValue evaluateDoBlock(size_t start, size_t end, map<string, RuntimeValue> &env)
    {
        RuntimeValue last = RuntimeValue::makeUnit();
        size_t statementIndex = start;

        while (statementIndex < end)
        {
            const auto &line = lines[statementIndex];
            size_t statementEnd = statementIndex + 1;
            while (statementEnd < end && lines[statementEnd].indent > line.indent)
            {
                ++statementEnd;
            }

            if (!line.tokens.empty() && line.tokens.front().value == "let")
            {
                evaluateLetBinding(vector<Token>(line.tokens.begin() + 1, line.tokens.end()), env);
            }
            else if (!line.tokens.empty() && line.tokens.front().value == "case")
            {
                last = evaluateCaseBlock(statementIndex, statementEnd, env);
            }
            else
            {
                vector<Token> expressionTokens = line.tokens;
                if (statementEnd > statementIndex + 1)
                {
                    auto nested = flattenLines(statementIndex + 1, statementEnd, lines);
                    expressionTokens.insert(expressionTokens.end(), nested.begin(), nested.end());
                }
                last = evaluateExpression(expressionTokens, env);
            }

            statementIndex = statementEnd;
        }

        return last;
    }

    void collectFunctions()
    {
        size_t index = 0;
        while (index < lines.size())
        {
            const auto &line = lines[index];
            if (line.indent != 1 || hasLexicalError(line))
            {
                ++index;
                continue;
            }

            int equalIndex = findTopLevelToken(line.tokens, {"="});
            if (equalIndex <= 0)
            {
                ++index;
                continue;
            }

            FunctionClause clause;
            clause.name = line.tokens.front().value;
            clause.line = line.lineNumber;

            vector<Token> lhsTokens(line.tokens.begin() + 1, line.tokens.begin() + equalIndex);
            clause.patterns = splitApplicationTerms(lhsTokens);
            clause.rhsInline = vector<Token>(line.tokens.begin() + equalIndex + 1, line.tokens.end());
            clause.nestedStart = index + 1;
            clause.nestedEnd = findBlockEnd(index + 1, line.indent);
            functions[clause.name].push_back(clause);
            index = clause.nestedEnd;
        }
    }

public:
    IdrisInterpreter(const vector<Token> &allTokens, const vector<LogicalLine> &logicalLines)
        : tokens(allTokens), lines(logicalLines) {}

    void interpret()
    {
        collectFunctions();
        if (functions.find("main") == functions.end())
        {
            runtimeErrors.push_back("main function not found");
            return;
        }

        vector<RuntimeValue> noArgs;
        (void)callFunction("main", noArgs);
    }

    const vector<string> &getOutput() const
    {
        return runtimeOutput;
    }

    const vector<string> &getErrors() const
    {
        return runtimeErrors;
    }
};

string readInputFile()
{
    const vector<string> candidates = {
        "../../lab4/build/input.txt",
        "../lab4/build/input.txt",
        "lab4/build/input.txt",
        "input.txt",
        "build/input.txt",
        "../../lab5/build/input.txt"};

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

void printSemanticSymbols(const vector<SymbolEntry> &symbols)
{
    const int totalWidth = 96;
    const int scopeWidth = 18;
    const int nameWidth = 18;
    const int typeWidth = 48;
    const int lineWidth = 8;

    cout << "\n" << string(totalWidth, '=') << '\n';
    cout << "SEMANTIC SYMBOL TABLE\n";
    cout << string(totalWidth, '=') << '\n';
    cout << left << setw(scopeWidth) << "SCOPE"
         << setw(nameWidth) << "NAME"
         << setw(typeWidth) << "TYPE"
         << setw(lineWidth) << "LINE" << '\n';
    cout << string(totalWidth, '-') << '\n';

    for (const auto &entry : symbols)
    {
        cout << left << setw(scopeWidth) << entry.scope
             << setw(nameWidth) << entry.name
             << setw(typeWidth) << entry.type
             << setw(lineWidth) << entry.line << '\n';
    }
}

void printSemanticErrors(const vector<string> &semanticErrors)
{
    cout << "\n" << string(70, '=') << '\n';
    cout << "SEMANTIC ERRORS\n";
    cout << string(70, '=') << '\n';
    if (semanticErrors.empty())
    {
        cout << "No semantic errors found.\n";
        return;
    }

    for (const auto &error : semanticErrors)
    {
        cout << error << '\n';
    }
}

void printInterpreterOutput(const vector<string> &output)
{
    cout << "\n" << string(70, '=') << '\n';
    cout << "INTERPRETER OUTPUT\n";
    cout << string(70, '=') << '\n';
    if (output.empty())
    {
        cout << "<no output>\n";
        return;
    }

    for (const auto &line : output)
    {
        cout << line << '\n';
    }
}

void printRuntimeErrors(const vector<string> &runtimeErrors)
{
    cout << "\n" << string(70, '=') << '\n';
    cout << "RUNTIME ERRORS\n";
    cout << string(70, '=') << '\n';
    if (runtimeErrors.empty())
    {
        cout << "No runtime errors found.\n";
        return;
    }

    for (const auto &error : runtimeErrors)
    {
        cout << error << '\n';
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

    const string svgPath = "lab5_cst.svg";
    const string dotPath = "lab5_cst.dot";
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

    SemanticAnalyzer semanticAnalyzer(tokens, parser.getLines());
    semanticAnalyzer.analyze();

    IdrisInterpreter interpreter(tokens, parser.getLines());
    interpreter.interpret();

    printSemanticSymbols(semanticAnalyzer.getSymbols());
    printErrors(tokens, parser.getSyntaxErrors());
    printSemanticErrors(semanticAnalyzer.getErrors());
    printInterpreterOutput(interpreter.getOutput());
    printRuntimeErrors(interpreter.getErrors());
    return 0;
}
