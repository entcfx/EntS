#include "parser.hpp"
#include <algorithm>

std::vector<std::string> existing_types;
std::vector<std::string> existing_identifiers;
std::vector<std::string> existing_functions;

std::unique_ptr<Node> Parser::parseProgram()
{
    std::vector<std::unique_ptr<Node>> statements;
    while (!isEOT())
    {
        statements.push_back(parseSTMT());
    }
    return std::make_unique<Node>(
        "PRG",
        std::move(statements)

    );
}
std::unique_ptr<Node> Parser::parseSTMT()
{
    if (match(TokenType::Header))
    {
        return parseHeader();
    }
    else if (match(TokenType::Typedef))
    {
        return parseTypedef();
    }
    else if (match(TokenType::Function))
    {
        return parseFunction();
    }
    else
    {
        errorf("unexpected statement in program");
    }
    errorf("somehow reached end of parseSTMT");
    return NULL; // Make GCC happy
}
std::unique_ptr<Node> Parser::parseHeader()
{
    std::vector<std::unique_ptr<Node>> prototypes;

    expect(TokenType::Header);
    expect(TokenType::OpenCurly);
    while (!match(TokenType::CloseCurly) && !isEOT())
    {
        prototypes.push_back(parsePrototype());
    }
    expect(TokenType::CloseBracket);
    return std::make_unique<Node>(
        "HEADER",
        std::move(prototypes)

    );
}
std::unique_ptr<Node> Parser::parsePrototype()
{
    // function helloWorld(int count, char* string) {} void;
    // -> Leaf: Name
    // Wrapper -> args -> FuncParam (leaf: type, children: NAME (leaf: name))
    //          -> returnType (leaf: type)
    std::vector<std::unique_ptr<Node>> wrapper;
    std::string name;
    std::unique_ptr<Node> params;
    std::unique_ptr<Node> returnType;

    expect(TokenType::Function);
    name = consume().value.value();
    expect(TokenType::OpenBracket);
    params = parseFuncParams();
    expect(TokenType::CloseBracket);
    // Body empty
    expect(TokenType::OpenCurly);
    expect(TokenType::CloseCurly);
    returnType = parseReturnType();
    expect(TokenType::Semicolon);

    wrapper.push_back(std::move(params));
    wrapper.push_back(std::move(returnType));

    existing_functions.push_back(name);
    return std::make_unique<Node>(
        "FUNCTION_PROTOTYPE",
        std::move(wrapper),
        std::optional<std::string>(name)

    );
}
std::unique_ptr<Node> Parser::parseTypedef()
{
    // typedef struct {} newType;
    // typedef uint8_t hello;
    // -> Leaf: newType
    // -> Children: Wrapper -> Struct
    //                      -> OLDTYPE (Leaf: oldTypeName)
    std::vector<std::unique_ptr<Node>> wrapper;
    std::string newType;

    expect(TokenType::Typedef);
    if (match(TokenType::Struct))
    {
        std::unique_ptr<Node> oldType = parseStruct();
        wrapper.push_back(std::move(oldType));
    }
    else
    {
        std::string oldTypeName = consume().value.value();
        auto it = std::find(existing_types.begin(), existing_types.end(), oldTypeName);

        if (it == existing_types.end())
        {
            errorf("type \'%s\' does not exist!", oldTypeName.c_str());
        }
        std::unique_ptr<Node> oldType = std::make_unique<Node>(
            "OLDTYPE",
            std::optional<std::string>(oldTypeName)

        );
        wrapper.push_back(std::move(oldType));
    }
    newType = consume().value.value();

    existing_types.push_back(newType);
    expect(TokenType::Semicolon);
    return std::make_unique<Node>(
        "TYPEDEF",
        std::move(wrapper),
        std::optional<std::string>(newType)

    );
}
std::unique_ptr<Node> Parser::parseFuncParams()
{
}
std::unique_ptr<Node> Parser::parseFunction()
{
}
std::unique_ptr<Node> Parser::parseFuncParam()
{
}
std::unique_ptr<Node> Parser::parseReturnType()
{
}
std::unique_ptr<Node> Parser::parseBody()
{
}
std::unique_ptr<Node> Parser::parseStruct()
{
}

std::unique_ptr<Node> Parser::parseEquality()
{
}
std::unique_ptr<Node> Parser::parseComparison()
{
}
std::unique_ptr<Node> Parser::parseAddition()
{
}
std::unique_ptr<Node> Parser::parseMultiplication()
{
}
std::unique_ptr<Node> Parser::parseUnary()
{
}
std::unique_ptr<Node> Parser::parsePrimary()
{
}