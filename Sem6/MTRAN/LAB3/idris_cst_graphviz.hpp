#pragma once

#include <graphviz/gvc.h>

#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace idris_cst
{

struct CstNode
{
    std::string label;
    std::vector<std::shared_ptr<CstNode>> children;

    explicit CstNode(std::string text) : label(std::move(text)) {}
};

using CstNodePtr = std::shared_ptr<CstNode>;

inline CstNodePtr makeNode(const std::string &label)
{
    return std::make_shared<CstNode>(label);
}

class IdrisCstBuilder
{
private:
    std::vector<LogicalLine> lines;
    size_t lineIndex = 0;

    static std::string tokenKindToString(TokenKind kind)
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

    static std::string quoteIfNeeded(const Token &token)
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

    static bool isFullyWrapped(const std::vector<Token> &slice, const std::string &openToken, const std::string &closeToken)
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

    static int findTopLevelToken(const std::vector<Token> &slice, const std::set<std::string> &targets, bool searchFromRight = false)
    {
        int parenDepth = 0;
        int bracketDepth = 0;

        if (!searchFromRight)
        {
            for (size_t i = 0; i < slice.size(); ++i)
            {
                const std::string &value = slice[i].value;
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
            const std::string &value = slice[static_cast<size_t>(i)].value;
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

    static std::vector<std::vector<Token>> splitApplicationTerms(const std::vector<Token> &slice)
    {
        std::vector<std::vector<Token>> terms;
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

    static size_t consumeTerm(const std::vector<Token> &slice, size_t start)
    {
        if (start >= slice.size())
        {
            return start;
        }

        const std::string &value = slice[start].value;
        if (value == "(" || value == "[")
        {
            const std::string closeToken = value == "(" ? ")" : "]";
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

    static std::vector<Token> flattenLines(size_t start, size_t end, const std::vector<LogicalLine> &allLines)
    {
        std::vector<Token> merged;
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

    CstNodePtr buildTokenLeaf(const Token &token) const
    {
        return makeNode(tokenKindToString(token.kind) + ": " + quoteIfNeeded(token));
    }

    CstNodePtr buildEmptyNode(const std::string &label) const
    {
        return makeNode(label + ": <empty>");
    }

    CstNodePtr buildTokenSequenceNode(const std::string &label, const std::vector<Token> &slice) const
    {
        auto node = makeNode(label);
        if (slice.empty())
        {
            node->children.push_back(makeNode("<empty>"));
            return node;
        }

        for (const auto &token : slice)
        {
            node->children.push_back(buildTokenLeaf(token));
        }
        return node;
    }

    template <typename BuildFn>
    void appendDelimitedParts(const CstNodePtr &node, const std::vector<Token> &inner, const std::string &delimiter, BuildFn buildPart)
    {
        std::vector<Token> current;
        int parenDepth = 0;
        int bracketDepth = 0;

        for (const auto &token : inner)
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
                if (current.empty())
                {
                    node->children.push_back(makeNode("MissingPart"));
                }
                else
                {
                    node->children.push_back(buildPart(current));
                    current.clear();
                }
                node->children.push_back(buildTokenLeaf(token));
                continue;
            }

            current.push_back(token);
        }

        if (current.empty() && !inner.empty())
        {
            node->children.push_back(makeNode("MissingPart"));
            return;
        }

        if (!current.empty())
        {
            node->children.push_back(buildPart(current));
        }
    }

    CstNodePtr buildType(const std::vector<Token> &slice)
    {
        if (slice.empty())
        {
            return buildEmptyNode("Type");
        }

        if (isFullyWrapped(slice, "(", ")"))
        {
            std::vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            if (inner.empty())
            {
                auto node = makeNode("UnitType");
                node->children.push_back(buildTokenLeaf(slice.front()));
                node->children.push_back(buildTokenLeaf(slice.back()));
                return node;
            }

            if (findTopLevelToken(inner, {","}) >= 0)
            {
                auto node = makeNode("TupleType");
                node->children.push_back(buildTokenLeaf(slice.front()));
                appendDelimitedParts(node, inner, ",", [this](const std::vector<Token> &part) {
                    return buildType(part);
                });
                node->children.push_back(buildTokenLeaf(slice.back()));
                return node;
            }

            auto node = makeNode("ParenthesizedType");
            node->children.push_back(buildTokenLeaf(slice.front()));
            node->children.push_back(buildType(inner));
            node->children.push_back(buildTokenLeaf(slice.back()));
            return node;
        }

        int arrowIndex = findTopLevelToken(slice, {"->"});
        if (arrowIndex >= 0)
        {
            auto node = makeNode("FunctionType");
            std::vector<Token> left(slice.begin(), slice.begin() + arrowIndex);
            std::vector<Token> right(slice.begin() + arrowIndex + 1, slice.end());
            node->children.push_back(buildType(left));
            node->children.push_back(buildTokenLeaf(slice[static_cast<size_t>(arrowIndex)]));
            node->children.push_back(buildType(right));
            return node;
        }

        auto terms = splitApplicationTerms(slice);
        if (terms.size() > 1U)
        {
            auto node = makeNode("AppliedType");
            for (const auto &term : terms)
            {
                node->children.push_back(buildType(term));
            }
            return node;
        }

        if (slice.size() == 1U)
        {
            return buildTokenLeaf(slice.front());
        }

        return buildTokenSequenceNode("TypeTokens", slice);
    }

    CstNodePtr buildPattern(const std::vector<Token> &slice)
    {
        if (slice.empty())
        {
            return buildEmptyNode("Pattern");
        }

        if (isFullyWrapped(slice, "(", ")"))
        {
            std::vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            if (inner.empty())
            {
                auto node = makeNode("UnitPattern");
                node->children.push_back(buildTokenLeaf(slice.front()));
                node->children.push_back(buildTokenLeaf(slice.back()));
                return node;
            }

            if (findTopLevelToken(inner, {","}) >= 0)
            {
                auto node = makeNode("TuplePattern");
                node->children.push_back(buildTokenLeaf(slice.front()));
                appendDelimitedParts(node, inner, ",", [this](const std::vector<Token> &part) {
                    return buildPattern(part);
                });
                node->children.push_back(buildTokenLeaf(slice.back()));
                return node;
            }

            auto node = makeNode("ParenthesizedPattern");
            node->children.push_back(buildTokenLeaf(slice.front()));
            node->children.push_back(buildPattern(inner));
            node->children.push_back(buildTokenLeaf(slice.back()));
            return node;
        }

        if (isFullyWrapped(slice, "[", "]"))
        {
            std::vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            auto node = makeNode("ListPattern");
            node->children.push_back(buildTokenLeaf(slice.front()));
            if (!inner.empty())
            {
                appendDelimitedParts(node, inner, ",", [this](const std::vector<Token> &part) {
                    return buildPattern(part);
                });
            }
            node->children.push_back(buildTokenLeaf(slice.back()));
            return node;
        }

        auto parts = splitApplicationTerms(slice);
        if (parts.size() > 1U)
        {
            auto node = makeNode("PatternApplication");
            for (const auto &part : parts)
            {
                node->children.push_back(buildPattern(part));
            }
            return node;
        }

        if (slice.size() == 1U)
        {
            return buildTokenLeaf(slice.front());
        }

        return buildTokenSequenceNode("PatternTokens", slice);
    }

    CstNodePtr buildBinding(const std::vector<Token> &slice, bool requireIn)
    {
        auto node = makeNode(requireIn ? "LetBinding" : "LetStatement");
        if (slice.empty())
        {
            node->children.push_back(makeNode("<empty>"));
            return node;
        }

        int equalIndex = findTopLevelToken(slice, {"="});
        if (equalIndex < 0)
        {
            node->children.push_back(buildTokenSequenceNode("BindingTokens", slice));
            return node;
        }

        if (!slice.empty())
        {
            node->children.push_back(buildTokenLeaf(slice.front()));
        }

        int colonIndex = findTopLevelToken(std::vector<Token>(slice.begin(), slice.begin() + equalIndex), {":"});
        if (colonIndex > 1)
        {
            std::vector<Token> head(slice.begin() + 1, slice.begin() + colonIndex);
            node->children.push_back(buildTokenSequenceNode("BindingHead", head));
        }

        if (colonIndex >= 0)
        {
            node->children.push_back(buildTokenLeaf(slice[static_cast<size_t>(colonIndex)]));
            std::vector<Token> typeTokens(slice.begin() + colonIndex + 1, slice.begin() + equalIndex);
            node->children.push_back(buildType(typeTokens));
        }
        else if (equalIndex > 1)
        {
            std::vector<Token> head(slice.begin() + 1, slice.begin() + equalIndex);
            if (!head.empty())
            {
                node->children.push_back(buildTokenSequenceNode("BindingHead", head));
            }
        }

        node->children.push_back(buildTokenLeaf(slice[static_cast<size_t>(equalIndex)]));
        std::vector<Token> valueTokens(slice.begin() + equalIndex + 1, slice.end());
        node->children.push_back(buildExpression(valueTokens));
        return node;
    }

    CstNodePtr buildMultilineExpression(const std::vector<Token> &inlineTokens, size_t start, size_t end)
    {
        auto node = makeNode("MultilineExpression");

        if (!inlineTokens.empty())
        {
            node->children.push_back(buildTokenSequenceNode("InlinePart", inlineTokens));
        }

        for (size_t i = start; i < end; ++i)
        {
            auto lineNode = makeNode("ContinuationLine " + std::to_string(lines[i].lineNumber));
            lineNode->children.push_back(buildTokenSequenceNode("Tokens", lines[i].tokens));
            node->children.push_back(lineNode);
        }

        if (inlineTokens.empty() && start >= end)
        {
            node->children.push_back(makeNode("<empty>"));
        }

        return node;
    }

    CstNodePtr buildCombinedExpression(const std::vector<Token> &inlineTokens, size_t start, size_t end)
    {
        std::vector<Token> merged = inlineTokens;
        auto nestedTokens = flattenLines(start, end, lines);
        merged.insert(merged.end(), nestedTokens.begin(), nestedTokens.end());
        if (merged.empty())
        {
            return buildEmptyNode("Expression");
        }
        return buildExpression(merged);
    }

    CstNodePtr buildExpression(const std::vector<Token> &slice)
    {
        if (slice.empty())
        {
            return buildEmptyNode("Expression");
        }

        if (isFullyWrapped(slice, "(", ")"))
        {
            std::vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            if (inner.empty())
            {
                auto node = makeNode("UnitExpression");
                node->children.push_back(buildTokenLeaf(slice.front()));
                node->children.push_back(buildTokenLeaf(slice.back()));
                return node;
            }

            if (findTopLevelToken(inner, {","}) >= 0)
            {
                auto node = makeNode("TupleExpression");
                node->children.push_back(buildTokenLeaf(slice.front()));
                appendDelimitedParts(node, inner, ",", [this](const std::vector<Token> &part) {
                    return buildExpression(part);
                });
                node->children.push_back(buildTokenLeaf(slice.back()));
                return node;
            }

            auto node = makeNode("ParenthesizedExpression");
            node->children.push_back(buildTokenLeaf(slice.front()));
            node->children.push_back(buildExpression(inner));
            node->children.push_back(buildTokenLeaf(slice.back()));
            return node;
        }

        if (slice.front().value == "if")
        {
            int thenIndex = findTopLevelToken(slice, {"then"});
            int elseIndex = findTopLevelToken(slice, {"else"});

            if (thenIndex > 0 && elseIndex > thenIndex)
            {
                std::vector<Token> condTokens(slice.begin() + 1, slice.begin() + thenIndex);
                std::vector<Token> thenTokens(slice.begin() + thenIndex + 1, slice.begin() + elseIndex);
                std::vector<Token> elseTokens(slice.begin() + elseIndex + 1, slice.end());

                auto node = makeNode("IfExpression");
                node->children.push_back(buildTokenLeaf(slice.front()));
                node->children.push_back(buildExpression(condTokens));
                node->children.push_back(buildTokenLeaf(slice[static_cast<size_t>(thenIndex)]));
                node->children.push_back(buildExpression(thenTokens));
                node->children.push_back(buildTokenLeaf(slice[static_cast<size_t>(elseIndex)]));
                node->children.push_back(buildExpression(elseTokens));
                return node;
            }
        }

        if (slice.front().value == "let")
        {
            int inIndex = findTopLevelToken(slice, {"in"});
            if (inIndex > 0)
            {
                auto node = makeNode("LetExpression");
                std::vector<Token> bindingTokens(slice.begin() + 1, slice.begin() + inIndex);
                std::vector<Token> bodyTokens(slice.begin() + inIndex + 1, slice.end());
                node->children.push_back(buildTokenLeaf(slice.front()));
                node->children.push_back(buildBinding(bindingTokens, true));
                node->children.push_back(buildTokenLeaf(slice[static_cast<size_t>(inIndex)]));
                node->children.push_back(buildExpression(bodyTokens));
                return node;
            }

            auto node = makeNode("LetExpression");
            node->children.push_back(buildTokenLeaf(slice.front()));
            node->children.push_back(buildBinding(std::vector<Token>(slice.begin() + 1, slice.end()), false));
            return node;
        }

        static const std::vector<std::set<std::string>> precedence = {
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
                auto node = makeNode("BinaryExpression");
                std::vector<Token> left(slice.begin(), slice.begin() + opIndex);
                std::vector<Token> right(slice.begin() + opIndex + 1, slice.end());
                node->children.push_back(buildExpression(left));
                node->children.push_back(buildTokenLeaf(slice[static_cast<size_t>(opIndex)]));
                node->children.push_back(buildExpression(right));
                return node;
            }
        }

        if (isFullyWrapped(slice, "[", "]"))
        {
            std::vector<Token> inner(slice.begin() + 1, slice.end() - 1);
            auto node = makeNode("ListExpression");
            node->children.push_back(buildTokenLeaf(slice.front()));
            if (!inner.empty())
            {
                appendDelimitedParts(node, inner, ",", [this](const std::vector<Token> &part) {
                    return buildExpression(part);
                });
            }
            node->children.push_back(buildTokenLeaf(slice.back()));
            return node;
        }

        auto parts = splitApplicationTerms(slice);
        if (parts.size() > 1U)
        {
            auto node = makeNode("ApplicationExpression");
            for (const auto &part : parts)
            {
                node->children.push_back(buildExpression(part));
            }
            return node;
        }

        if (slice.size() == 1U)
        {
            return buildTokenLeaf(slice.front());
        }

        return buildTokenSequenceNode("ExpressionTokens", slice);
    }

    CstNodePtr buildCaseExpression(const std::vector<Token> &caseTokens, size_t branchStart, size_t end, int parentIndent)
    {
        auto node = makeNode("CaseExpression");
        if (caseTokens.empty())
        {
            node->children.push_back(makeNode("<empty>"));
            return node;
        }

        int ofIndex = findTopLevelToken(caseTokens, {"of"});
        if (ofIndex < 0)
        {
            node->children.push_back(buildTokenSequenceNode("CaseTokens", caseTokens));
            return node;
        }

        node->children.push_back(buildTokenLeaf(caseTokens.front()));
        std::vector<Token> subjectTokens(caseTokens.begin() + 1, caseTokens.begin() + ofIndex);
        node->children.push_back(buildExpression(subjectTokens));
        node->children.push_back(buildTokenLeaf(caseTokens[static_cast<size_t>(ofIndex)]));

        auto branchesNode = makeNode("CaseBranches");
        while (branchStart < end)
        {
            const auto &branchLine = lines[branchStart];
            if (branchLine.indent <= parentIndent)
            {
                break;
            }

            size_t branchEnd = branchStart + 1;
            while (branchEnd < end && lines[branchEnd].indent > branchLine.indent)
            {
                ++branchEnd;
            }

            auto branchNode = makeNode("CaseBranch");
            int arrowIndex = findTopLevelToken(branchLine.tokens, {"=>"});
            if (arrowIndex < 0)
            {
                branchNode->children.push_back(buildTokenSequenceNode("BranchTokens", branchLine.tokens));
            }
            else
            {
                std::vector<Token> patternTokens(branchLine.tokens.begin(), branchLine.tokens.begin() + arrowIndex);
                std::vector<Token> rhsTokens(branchLine.tokens.begin() + arrowIndex + 1, branchLine.tokens.end());

                branchNode->children.push_back(buildPattern(patternTokens));
                branchNode->children.push_back(buildTokenLeaf(branchLine.tokens[static_cast<size_t>(arrowIndex)]));

                if (!rhsTokens.empty())
                {
                    if (rhsTokens.front().value == "case" && branchEnd > branchStart + 1)
                    {
                        branchNode->children.push_back(buildCaseExpression(rhsTokens, branchStart + 1, branchEnd, branchLine.indent));
                    }
                    else if (rhsTokens.front().value == "do")
                    {
                        branchNode->children.push_back(buildDoBlock(branchStart + 1, branchEnd, rhsTokens));
                    }
                    else if (branchEnd > branchStart + 1)
                    {
                        branchNode->children.push_back(buildCombinedExpression(rhsTokens, branchStart + 1, branchEnd));
                    }
                    else
                    {
                        branchNode->children.push_back(buildExpression(rhsTokens));
                    }
                }
                else if (branchEnd > branchStart + 1)
                {
                    branchNode->children.push_back(buildIndentedExpression(branchStart + 1, branchEnd));
                }
                else
                {
                    branchNode->children.push_back(makeNode("<empty>"));
                }
            }

            branchesNode->children.push_back(branchNode);
            branchStart = branchEnd;
        }

        if (branchesNode->children.empty())
        {
            branchesNode->children.push_back(makeNode("<no branches>"));
        }

        node->children.push_back(branchesNode);
        return node;
    }

    CstNodePtr buildDoBlock(size_t start, size_t end, const std::vector<Token> &inlineTokens = {})
    {
        auto node = makeNode("DoExpression");
        if (!inlineTokens.empty())
        {
            node->children.push_back(buildTokenLeaf(inlineTokens.front()));
            std::vector<Token> tail(inlineTokens.begin() + 1, inlineTokens.end());
            if (!tail.empty())
            {
                auto statementNode = makeNode("DoStatement");
                statementNode->children.push_back(buildExpression(tail));
                node->children.push_back(statementNode);
            }
        }

        if (start >= end)
        {
            if (inlineTokens.size() <= 1U)
            {
                node->children.push_back(makeNode("<empty block>"));
            }
            return node;
        }

        size_t statementIndex = start;
        int blockIndent = lines[start].indent;
        while (statementIndex < end)
        {
            if (lines[statementIndex].indent != blockIndent)
            {
                ++statementIndex;
                continue;
            }

            size_t statementEnd = statementIndex + 1;
            while (statementEnd < end && lines[statementEnd].indent > lines[statementIndex].indent)
            {
                ++statementEnd;
            }

            const auto &statementLine = lines[statementIndex];
            auto statementNode = makeNode("DoStatement");

            if (!statementLine.tokens.empty() && statementLine.tokens.front().value == "let")
            {
                statementNode->children.push_back(buildTokenLeaf(statementLine.tokens.front()));
                statementNode->children.push_back(buildBinding(std::vector<Token>(statementLine.tokens.begin() + 1, statementLine.tokens.end()), false));
            }
            else if (!statementLine.tokens.empty() && statementLine.tokens.front().value == "case")
            {
                statementNode->children.push_back(buildCaseExpression(statementLine.tokens, statementIndex + 1, statementEnd, statementLine.indent));
            }
            else if (!statementLine.tokens.empty() && statementLine.tokens.front().value == "do")
            {
                statementNode->children.push_back(buildDoBlock(statementIndex + 1, statementEnd, statementLine.tokens));
            }
            else if (statementEnd > statementIndex + 1)
            {
                statementNode->children.push_back(buildCombinedExpression(statementLine.tokens, statementIndex + 1, statementEnd));
            }
            else
            {
                statementNode->children.push_back(buildExpression(statementLine.tokens));
            }

            node->children.push_back(statementNode);
            statementIndex = statementEnd;
        }

        return node;
    }

    CstNodePtr buildIndentedExpression(size_t start, size_t end)
    {
        if (start >= end)
        {
            return buildEmptyNode("Expression");
        }

        const auto &firstLine = lines[start];
        if (!firstLine.tokens.empty() && firstLine.tokens.front().value == "case")
        {
            return buildCaseExpression(firstLine.tokens, start + 1, end, firstLine.indent);
        }
        if (!firstLine.tokens.empty() && firstLine.tokens.front().value == "do")
        {
            return buildDoBlock(start + 1, end, firstLine.tokens);
        }
        if (end > start + 1)
        {
            return buildCombinedExpression(firstLine.tokens, start + 1, end);
        }
        return buildExpression(firstLine.tokens);
    }

    CstNodePtr buildImport()
    {
        const auto &line = lines[lineIndex];
        auto node = makeNode("ImportDeclaration");
        for (const auto &token : line.tokens)
        {
            node->children.push_back(buildTokenLeaf(token));
        }
        ++lineIndex;
        return node;
    }

    CstNodePtr buildTypeSignature()
    {
        const auto &line = lines[lineIndex];
        auto node = makeNode("TypeSignature");

        int colonIndex = findTopLevelToken(line.tokens, {":"});
        if (colonIndex <= 0)
        {
            node->children.push_back(buildTokenSequenceNode("SignatureTokens", line.tokens));
            ++lineIndex;
            return node;
        }

        node->children.push_back(buildTokenLeaf(line.tokens.front()));
        if (colonIndex > 1)
        {
            std::vector<Token> nameTail(line.tokens.begin() + 1, line.tokens.begin() + colonIndex);
            if (!nameTail.empty())
            {
                node->children.push_back(buildTokenSequenceNode("SignatureHead", nameTail));
            }
        }
        node->children.push_back(buildTokenLeaf(line.tokens[static_cast<size_t>(colonIndex)]));
        std::vector<Token> typeTokens(line.tokens.begin() + colonIndex + 1, line.tokens.end());
        node->children.push_back(buildType(typeTokens));

        ++lineIndex;
        return node;
    }

    CstNodePtr buildFunctionDefinition()
    {
        const auto &line = lines[lineIndex];
        auto node = makeNode("FunctionDefinition");

        int equalIndex = findTopLevelToken(line.tokens, {"="});
        if (equalIndex <= 0)
        {
            node->children.push_back(buildTokenSequenceNode("DefinitionTokens", line.tokens));
            ++lineIndex;
            return node;
        }

        node->children.push_back(buildTokenLeaf(line.tokens.front()));

        auto paramsNode = makeNode("FunctionParameters");
        std::vector<Token> lhsTokens(line.tokens.begin() + 1, line.tokens.begin() + equalIndex);
        auto parameterSlices = splitApplicationTerms(lhsTokens);
        for (const auto &parameter : parameterSlices)
        {
            paramsNode->children.push_back(buildPattern(parameter));
        }
        if (!paramsNode->children.empty())
        {
            node->children.push_back(paramsNode);
        }

        node->children.push_back(buildTokenLeaf(line.tokens[static_cast<size_t>(equalIndex)]));

        size_t blockEnd = findBlockEnd(lineIndex + 1, line.indent);
        std::vector<Token> rhsInline(line.tokens.begin() + equalIndex + 1, line.tokens.end());

        auto bodyNode = makeNode("FunctionBody");
        if (!rhsInline.empty())
        {
            if (rhsInline.front().value == "do")
            {
                bodyNode->children.push_back(buildDoBlock(lineIndex + 1, blockEnd, rhsInline));
            }
            else if (rhsInline.front().value == "case" && blockEnd > lineIndex + 1)
            {
                bodyNode->children.push_back(buildCaseExpression(rhsInline, lineIndex + 1, blockEnd, line.indent));
            }
            else if (blockEnd > lineIndex + 1)
            {
                bodyNode->children.push_back(buildCombinedExpression(rhsInline, lineIndex + 1, blockEnd));
            }
            else
            {
                bodyNode->children.push_back(buildExpression(rhsInline));
            }
        }
        else if (blockEnd > lineIndex + 1)
        {
            bodyNode->children.push_back(buildIndentedExpression(lineIndex + 1, blockEnd));
        }
        else
        {
            bodyNode->children.push_back(makeNode("<empty>"));
        }

        node->children.push_back(bodyNode);

        lineIndex = blockEnd;
        return node;
    }

public:
    explicit IdrisCstBuilder(const std::vector<LogicalLine> &logicalLines) : lines(logicalLines) {}

    CstNodePtr buildProgram()
    {
        auto root = makeNode("Program");

        while (lineIndex < lines.size())
        {
            const auto &line = lines[lineIndex];

            if (hasLexicalError(line))
            {
                auto node = makeNode("LexicalErrorLine " + std::to_string(line.lineNumber));
                node->children.push_back(buildTokenSequenceNode("Tokens", line.tokens));
                root->children.push_back(node);
                ++lineIndex;
                continue;
            }

            if (line.indent != 1)
            {
                auto node = makeNode("UnexpectedIndentedTopLevel " + std::to_string(line.lineNumber));
                node->children.push_back(buildTokenSequenceNode("Tokens", line.tokens));
                root->children.push_back(node);
                ++lineIndex;
                continue;
            }

            if (!line.tokens.empty() && line.tokens.front().value == "import")
            {
                root->children.push_back(buildImport());
                continue;
            }

            bool hasColon = findTopLevelToken(line.tokens, {":"}) >= 0;
            bool hasEqual = findTopLevelToken(line.tokens, {"="}) >= 0;

            if (hasColon && !hasEqual)
            {
                root->children.push_back(buildTypeSignature());
                continue;
            }

            if (hasEqual)
            {
                root->children.push_back(buildFunctionDefinition());
                continue;
            }

            auto node = makeNode("UnknownTopLevel " + std::to_string(line.lineNumber));
            node->children.push_back(buildTokenSequenceNode("Tokens", line.tokens));
            root->children.push_back(node);
            ++lineIndex;
        }

        return root;
    }
};

inline void printTree(const CstNodePtr &node, const std::string &prefix = "", bool isLast = true, bool isRoot = true, std::ostream &out = std::cout)
{
    if (!node)
    {
        return;
    }

    if (isRoot)
    {
        out << node->label << '\n';
    }
    else
    {
        out << prefix << (isLast ? "└── " : "├── ") << node->label << '\n';
    }

    for (size_t i = 0; i < node->children.size(); ++i)
    {
        bool childIsLast = i + 1 == node->children.size();
        std::string childPrefix = isRoot ? "" : prefix + (isLast ? "    " : "│   ");
        printTree(node->children[i], childPrefix, childIsLast, false, out);
    }
}

inline bool renderGraphviz(const CstNodePtr &root, const std::string &svgPath, const std::string &dotPath, std::string *errorMessage = nullptr)
{
    if (!root)
    {
        if (errorMessage != nullptr)
        {
            *errorMessage = "empty CST";
        }
        return false;
    }

    GVC_t *context = gvContext();
    if (context == nullptr)
    {
        if (errorMessage != nullptr)
        {
            *errorMessage = "could not create Graphviz context";
        }
        return false;
    }

    Agraph_t *graph = agopen(const_cast<char *>("cst"), Agdirected, nullptr);
    if (graph == nullptr)
    {
        gvFreeContext(context);
        if (errorMessage != nullptr)
        {
            *errorMessage = "could not create Graphviz graph";
        }
        return false;
    }

    agsafeset(graph, const_cast<char *>("rankdir"), const_cast<char *>("TB"), const_cast<char *>(""));
    agsafeset(graph, const_cast<char *>("splines"), const_cast<char *>("ortho"), const_cast<char *>(""));
    agsafeset(graph, const_cast<char *>("nodesep"), const_cast<char *>("0.35"), const_cast<char *>(""));
    agsafeset(graph, const_cast<char *>("ranksep"), const_cast<char *>("0.55"), const_cast<char *>(""));

    size_t nextId = 0;

    auto addNode = [&](const CstNodePtr &node, const std::string &nodeId) -> Agnode_t * {
        Agnode_t *graphNode = agnode(graph, const_cast<char *>(nodeId.c_str()), 1);
        agsafeset(graphNode, const_cast<char *>("label"), const_cast<char *>(node->label.c_str()), const_cast<char *>(""));
        agsafeset(graphNode, const_cast<char *>("fontname"), const_cast<char *>("Helvetica"), const_cast<char *>(""));
        if (node->children.empty())
        {
            agsafeset(graphNode, const_cast<char *>("shape"), const_cast<char *>("note"), const_cast<char *>(""));
            agsafeset(graphNode, const_cast<char *>("style"), const_cast<char *>("filled"), const_cast<char *>(""));
            agsafeset(graphNode, const_cast<char *>("fillcolor"), const_cast<char *>("#E8F1F8"), const_cast<char *>(""));
        }
        else
        {
            agsafeset(graphNode, const_cast<char *>("shape"), const_cast<char *>("box"), const_cast<char *>(""));
            agsafeset(graphNode, const_cast<char *>("style"), const_cast<char *>("rounded,filled"), const_cast<char *>(""));
            agsafeset(graphNode, const_cast<char *>("fillcolor"), const_cast<char *>("#F7F3E8"), const_cast<char *>(""));
        }
        return graphNode;
    };

    auto connectTree = [&](const auto &self, const CstNodePtr &node, Agnode_t *parent) -> void {
        const std::string nodeId = "n" + std::to_string(nextId++);
        Agnode_t *graphNode = addNode(node, nodeId);
        if (parent != nullptr)
        {
            agedge(graph, parent, graphNode, nullptr, 1);
        }

        for (const auto &child : node->children)
        {
            self(self, child, graphNode);
        }
    };

    connectTree(connectTree, root, nullptr);

    bool ok = true;
    if (gvLayout(context, graph, "dot") != 0)
    {
        ok = false;
        if (errorMessage != nullptr)
        {
            *errorMessage = "Graphviz layout failed";
        }
    }
    else
    {
        if (!dotPath.empty() && gvRenderFilename(context, graph, "dot", dotPath.c_str()) != 0)
        {
            ok = false;
            if (errorMessage != nullptr)
            {
                *errorMessage = "could not write DOT file";
            }
        }

        if (ok && !svgPath.empty() && gvRenderFilename(context, graph, "svg", svgPath.c_str()) != 0)
        {
            ok = false;
            if (errorMessage != nullptr)
            {
                *errorMessage = "could not write SVG file";
            }
        }
    }

    gvFreeLayout(context, graph);
    agclose(graph);
    gvFreeContext(context);
    return ok;
}

} // namespace idris_cst
