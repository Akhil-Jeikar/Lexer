#include <iostream>
#include <vector>
#include <cctype>
#include <unordered_set>
#include <unordered_map>

// Prototype 0.0.1

// only for the main
namespace command
{
    using string = std::string;
    template <typename t>
    using uset = std::unordered_set<t>;

    template <typename T>
    using umap = std::unordered_map<string, uset<T>>;
}

enum class Tokens
{
    IDENTIFIER,
    OPTION,
    STRING_LITERAL,
    NUMBER_LITERAL,
    END_OF_FILE
};

command::umap<command::string> typelist = {{"DDL", {"CREATE", "DROP", "ALTER", "TRUNCATE"}},
                                           {"DML", {"INSERT", "DELETE", "UPDATE"}},
                                           {"DQL", {"SELECT"}}};

struct Token // remaind
{
    Tokens type;
    std::string value;
};
class lexer
{
public:
    explicit lexer(const std::string &input) : input(input), pos(0) {}
    std::vector<Token> tokens;

    void Tokeniser()
    {
        // pre remove all the forward whitespaces
        Check_For_Irregularities();
        while (pos < input.length())
        {
            functionchecker();
        }
    }

private:
    std::string input;
    size_t pos;
    // Execution order=1 ,Execution frequency=only once
    void Check_For_Irregularities()
    {
        /*pre - removes white space
        Runtime Execution - May or Maynot*/

        // started with whitespace (NOTE: Always keep this at top)
        if (std::isspace(input[pos]))
        {
            while (pos < input.length() && std::isspace(input[pos]))
            {
                pos++;
            }
        }
        // start_with_num condition
        if (std::isdigit(input[pos]))
        {
            std::cout << "INVALID SYNTAX , STARTED WITH A NUMERIC";
            exit(1);
        }
        // Check for invalid symbols (includes other than alpha||numeric values)
        if (!std::isalnum(input[pos]) && !std::isspace(input[pos]))
        {
            std::cout << "INVALID SYNTAX, UNRECOGNIZED SYMNOLS";
            exit(1);
        }
    }
    void functionchecker()
    {
        int s = pos; // write a refernce to the start of the function identifier
        int e = pos; // write a end refernce to track the end of the function identifier
        while (s < input.length() && !std::isspace(input[pos]))
        {
            pos++;
            e++;
        }
        // check if the function is in the assigned functions
        std::string User_Value = input.substr(s, e);
        int v = 0;
        for (const auto &[grp, Assigned_funcs] : typelist)
        {
            if (Assigned_funcs.find(User_Value) != Assigned_funcs.end())
            {
                if (grp == "DDL")
                {
                    v = 1;
                    break;
                }
                if (grp == "DML")
                {
                    v = 2;
                    break;
                }
                if (grp == "DQL")
                {
                    v = 3;
                    break;
                }
            }
        }
        // return error for invalid functions
        if (v == 0)
        {

            std::cout << "INVALID FUNCTION NAME";
            exit(1);
        }
        else
        {
            switch (v)
            {
            case 1:
                std::cout << "DDL";
                break;
            case 2:
                std::cout << "DML";
                break;
            case 3:
                std::cout << "DQL";
                break;
            }
        }
    }
};
int main()
{
    command::string a = "INSERT";
    lexer lex(a);
    lex.Tokeniser();
}

// select * from dname
// insert  into dbname

// cmd ----->ddl,dml --->ddllex or dmllex