#include <iostream>
#include <vector>  
#include <string>  
#include <stdio.h>  
#include <stdlib.h> 
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <stack>
#include <algorithm>
#include <cmath>
#include <map>
#include <functional>û
 
#include <time.h> 

using namespace std;

string parser_result;
string text_n;
//==================================================
//
//  WHAT's this interpretator can?
// 
//  TYPES: Integer, Float, String, Boolean
//  Also:  Arrays
//  
//  Operations : + - * /
//  Logic operations: == != === !== 
//                     >  <  >=  <=
//  Operations with string: "" + "" / "" * "" 
//  
//  Comments: // /*...*/
// 
//  Key words: var 
//             if {} else {}
//             while {}
//             continue
//             break 
// 
//  U don't need the ";" but program skip it :)
//
//=================================================


enum type_of_lexs {
    LEX_PEX, LEX_INT, LEX_FLOAT, LEX_NAME, LEX_STRING, LEX_DELIMITER, LEX_FIN, LEX_KEYWORD
};

struct Tokini {
    int		 type_token;
    string	 name;
    int		 val_int;
    float	 val_float;
};

class Scaner {
public:
    std::string text_in;
    int cursor = 0;

    Scaner(std::string& kot) {
        text_in = kot;
    };

    int next()
    {
        if (cursor >= text_in.size()) return -1;
        return text_in[cursor++];
    };
    int current()
    {
        if (cursor >= text_in.size()) return -1;
        return text_in[cursor];
    };
    void german_gref()
    {
        cursor--;
    };

};

class Tokinizator {

public:
    Tokini  tokini_ball;

    Scaner scaner;

    Tokinizator(std::string text_in) : scaner(text_in) {

    };

    bool get_token()
    {
    tokenize:
        if (scaner.current() == -1)
            return false;

        // skip white
        while (is_white(scaner.current()))
        {
            if (!has_token()) return false;
            scaner.next();
        }

        // skip /* */ and //
        if (scaner.current() == '/')
        {
            scaner.next();

            // /*
            if (scaner.current() == '*') {
                scaner.next();

                while (1) {

                    if (!has_token()) throw "Expected * in */";

                    if (scaner.current() == '*') {
                        scaner.next();

                        if (scaner.current() == '/') {
                            scaner.next();
                            break;
                        }
                    }
                    else
                        scaner.next();
                }

                goto tokenize;
            }
            else if (scaner.current() == '/') {
                while (1) {
                    if (!has_token()) return false;

                    if (scaner.current() == '\n')
                        break;

                    scaner.next();
                }

                goto tokenize;
            }
            else {
                scaner.german_gref();
            }
        }




        if (isalpha(scaner.current()))
        {
            std::string str_line;
            str_line = scaner.current();
            scaner.next();
            while (((isalpha(scaner.current())) || (isdigit(scaner.current())) || (scaner.current() == '_')))
            {
                str_line += scaner.current();
                scaner.next();
            }

            if (str_line == "var" || str_line == "while" || str_line == "do" || str_line == "for" || str_line == "return" || str_line == "break" || str_line == "continue" || str_line == "if" || str_line == "else")
                tokini_ball.type_token = LEX_KEYWORD;
            else
                tokini_ball.type_token = LEX_NAME;

            tokini_ball.name = str_line;
            return true;

        }
        else if (isdigit(scaner.current()))
        {
            int num = scaner.current() - 48;

            float num_f = 0;
            scaner.next();

            while (isdigit(scaner.current()))
            {

                num = num * 10 + scaner.current() - 48;
                scaner.next();
            }

            if (scaner.current() == '.')
            {
                num_f = (float)num;
                num = 1;
                while (isdigit(scaner.current()))
                {
                    num_f = num_f + (scaner.current() - 48) * pow(0.1, num);
                    num++;
                    scaner.next();
                }
                if ((scaner.current() == 'e') || (scaner.current() == 'E'))
                {
                    int znak = 1;
                    scaner.next();
                    if ((scaner.current() == '+')) scaner.next();
                    if ((scaner.current() == '-')) {
                        znak = -1; scaner.next();
                    }
                    num = 0;

                    while (isdigit(scaner.current()))
                    {
                        num = num * 10 + scaner.current() - 48;
                        scaner.next();
                    }
                    num_f = num_f * pow(1, num);
                }
                tokini_ball.type_token = LEX_FLOAT;
                tokini_ball.val_float = num_f;
                return true;
            }
            else
            {

                tokini_ball.type_token = LEX_INT;
                tokini_ball.val_int = num;
                return true;
            }
        }
        else if ((scaner.current() == '"') || (scaner.current() == '\''))
        {
            char s = scaner.current();
            scaner.next();
            std::string str_line;
            while (scaner.current() != s)
            {
                if (!has_token()) throw "extected \" at the end of string";
                str_line += scaner.current();
                scaner.next();
            }
            scaner.next();
            tokini_ball.type_token = LEX_STRING;
            tokini_ball.name = str_line;
            return true;

        }
        else
        {
            switch (scaner.current())
            {
            case '+':
            {
                scaner.next();
 

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = "+";
                return true;
            }
            case '-':
            {
                scaner.next();
 

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = "-";
                return true;
            }
            case '=':
            {
                scaner.next();
                if (scaner.current() == '=')
                {
                    scaner.next();

                    if (scaner.current() == '=')
                    {
                        scaner.next();
                        tokini_ball.type_token = LEX_DELIMITER;
                        tokini_ball.name = "===";
                        return true;
                    }

                    tokini_ball.type_token = LEX_DELIMITER;
                    tokini_ball.name = "==";
                    return true;

                }

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = "=";
                return true;
            }
            case '!':
            {
                scaner.next();
                if (scaner.current() == '=')
                {
                    scaner.next();

                    if (scaner.current() == '=')
                    {
                        scaner.next();
                        tokini_ball.type_token = LEX_DELIMITER;
                        tokini_ball.name = "!==";
                        return true;
                    }

                    tokini_ball.type_token = LEX_DELIMITER;
                    tokini_ball.name = "!=";
                    return true;
                }

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = "!";
                return true;
            }
            case '<':
            {
                scaner.next();
                if (scaner.current() == '=')
                {
                    scaner.next();
                    tokini_ball.type_token = LEX_DELIMITER;
                    tokini_ball.name = "<=";
                    return true;
                }

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = "<";
                return true;
            }
            case '>':
            {
                scaner.next();
                if (scaner.current() == '=')
                {
                    scaner.next();
                    tokini_ball.type_token = LEX_DELIMITER;
                    tokini_ball.name = ">=";
                    return true;
                }

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = ">";
                return true;
            }
 
            case '*':
            {
                scaner.next();
 

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = "*";
                return true;
            }
            case '/':
            {
                scaner.next();
 

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = "/";
                return true;
            }
            case '(':
            {
                scaner.next();

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = "(";
                return true;
            }
            case ')':
            {
                scaner.next();

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = ")";
                return true;
            }
            case '[':
            {
                scaner.next();

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = "[";
                return true;
            }
            case ']':
            {
                scaner.next();

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = "]";
                return true;
            }
            case ',':
            {
                scaner.next();

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = ",";
                return true;
            }
            case ';':
            {
                scaner.next();

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = ";";
                return true;
            }
            case '.':
            {
                scaner.next();

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = ".";
                return true;
            }
            case '{':
            {
                scaner.next();

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = "{";
                return true;
            }
            case '}':
            {
                scaner.next();

                tokini_ball.type_token = LEX_DELIMITER;
                tokini_ball.name = "}";
                return true;
            }
            case -1: return false;
            default:
                throw "invalid char";
            }

            tokini_ball.type_token = LEX_DELIMITER;
            tokini_ball.val_int = scaner.current();
            scaner.next();
            return true;
        }

    };

    bool has_token()
    {
        return scaner.cursor < scaner.text_in.size();
    };

    bool is_white(int c)
    {
        return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
    };

};


vector<Tokini> push_tokini(std::string& code_str)
{
    Tokinizator t(code_str);
    vector<Tokini> tokens;

    while (t.get_token())
    {
        tokens.push_back(t.tokini_ball);
        t.tokini_ball.name = "";
        t.tokini_ball.val_int = 0;
        t.tokini_ball.val_float = 0;
        t.tokini_ball.type_token = 0;


    }

    return tokens;
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


class Obj
{
public:

    virtual int ToInt() { return 0; };
    virtual float ToFloat() { return 0; };
    virtual string ToString() { return ""; };

    virtual Obj* get(const string& key) { return NULL; };
    virtual void set(const string& key, Obj* value) {};

};

// forward
class Dict;
void init_dict();
class Function;
void init_function();
class Int;
void init_int();
class Float;
void init_float();
class String;
void init_string();
class Boolean;
void init_boolean();
class Array;
void init_array();
class Scope;
void init_scope();


class Scope : public Obj
{
public:
    static Obj* prototype;
    Scope* parent;

    Scope(Scope* p) { this->parent = p; };

    std::map <std::string, Obj*> values;

    virtual int ToInt() { return values.size(); };
    virtual float ToFloat() { return values.size(); };
    virtual string ToString() { return "Scope.toString: Not Implemented "; };

    virtual Obj* get(const string& key)
    {
        map <string, Obj*> ::iterator it = values.find(key);

        if (it != values.end())
        {
            return it->second;
        }
        if (parent != nullptr)
        {
            return parent->get(key);
        }
        if (this != prototype)
        {
            return prototype->get(key);
        }
        return nullptr;

    };
    virtual void set(const string& key, Obj* value)
    {
        Scope* s = this;

        while (s) {
            map <string, Obj*> ::iterator it = s->values.find(key);

            if (it != s->values.end())
            {
                it->second = value;
                return;
            }
            else
                s = s->parent;
        }

        put(key, value);
    };
    virtual void put(const string& key, Obj* value)
    {
        values[key] = value;
    };
};
Obj* Scope::prototype = NULL;

class Dict : public Obj
{

public:
    static Obj* prototype;

    std::map <std::string, Obj*> values;

    virtual int ToInt() { return values.size(); };
    virtual float ToFloat() { return values.size(); };
    virtual string ToString() { return "Dict.toString: Not Implemented "; };

    virtual Obj* get(const string& key)
    {
        map <string, Obj*> ::iterator it = values.find(key);

        if (it != values.end())
        {
            return it->second;
        }
        if (this != prototype)
        {
            return prototype->get(key);
        }
        return nullptr;

    };
    virtual void set(const string& key, Obj* value)
    {
        values[key] = value;
    };
};
Obj* Dict::prototype = NULL;

class Callable
{
public:
    virtual Obj* call(vector <Obj*>& args)
    {
        return NULL;
    };
};

class Function : public Obj, public Callable
{
public:
    std::function <Obj* (vector <Obj*>)> fun;

    Function(std::function <Obj* (vector <Obj*>)> f) { fun = f; };

    virtual Obj* call(vector <Obj*>& args)
    {
        return fun(args);
    };

    static Obj* prototype;


    virtual int ToInt() { return 1; };
    virtual float ToFloat() { return 1; };
    virtual string ToString() { return "1"; };

    virtual Obj* get(const string& key) { return prototype->get(key); };

};
Obj* Function::prototype = NULL;

class Int : public Obj
{
public:
    static Obj* prototype;


    int values;

    Int(int i) { values = i; };


    virtual int ToInt() { return values; };
    virtual float ToFloat() { return values; };
    virtual string ToString() { return std::to_string(values); };

    virtual Obj* get(const string& key) { return prototype->get(key); };


};
Obj* Int::prototype = NULL;

class Float : public Obj
{
public:
    static Obj* prototype;

    float values;
    Float(float i) { values = i; };

    virtual int ToInt() { return (int)values; };
    virtual float ToFloat() { return values; };
    virtual string ToString() { return std::to_string(values); };

    virtual Obj* get(const string& key) { return prototype->get(key); };
};
Obj* Float::prototype = NULL;

class String : public Obj
{
public:
    static Obj* prototype;

    string values;
    String(string i) { values = i; };

    virtual int ToInt() { return stoi(values); };
    virtual float ToFloat() { return stoi(values); };
    virtual string ToString() { return values; };

    virtual Obj* get(const string& key)
    {
        try
        {
            int index = stoi(key);
            if (values.size() == 0) return nullptr;
            index = ((index % values.size()) + values.size()) % values.size();
            return new String(values.substr(index, 1));

        }
        catch (...)
        {
            return prototype->get(key);
        };

    };
};
Obj* String::prototype = NULL;

class Boolean : public Obj
{
public:
    static Obj* prototype;

    bool values;

    Boolean(bool i) { values = i; };

    virtual int ToInt() { return values; };
    virtual float ToFloat() { return values; };
    virtual string ToString() { return values ? "true" : "false"; };

    virtual Obj* get(const string& key) { return prototype->get(key); };
};
Obj* Boolean::prototype = NULL;

class Array : public Obj
{
public:
    static Obj* prototype;
    vector <Obj*> values;
    Array(vector <Obj*>& v) { values = v; };



    virtual int ToInt() { return values.size(); };
    virtual float ToFloat() { return values.size(); };
    virtual string ToString() {
        string s = "[ ";

        for (int i = 0; i < values.size(); ++i) {
            if (values[i] != nullptr)
                s += values[i]->ToString();
            else
                s += "undefined";

            if (i < values.size() - 1)
                s += ", ";
        }

        return s + " ]";
    };

    virtual Obj* get(const string& key)
    {
        try
        {
            int index = stoi(key);
            if (values.size() == 0) return nullptr;
            index = ((index % values.size()) + values.size()) % values.size();
            return values[index];

        }
        catch (...)
        {
            return prototype->get(key);
        };

    };

    virtual void set(const string& key, Obj* value)
    {
        try
        {
            int index = stoi(key);
            if (values.size() == 0) return;
            index = ((index % values.size()) + values.size()) % values.size();
            values[index] = value;

        }
        catch (...)
        {
            return;
        };

    };
};
Obj* Array::prototype = NULL;

class Error
{
public:
    string message;
    Error(string& s)
    {
        message = s;
    };
};

class Break
{

};

class Continue
{
};


// I N I T 

void init_dict() {
    Dict::prototype = new Dict();



}

void init_function() {
    String::prototype = new Dict();



}

void init_int() {
    Int::prototype = new Dict();



    Int::prototype->set("+", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Int(i->values + b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Float(i->values + b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Int(i->values + b->values);
            if (String* b = dynamic_cast<String*> (args[1])) return new String(std::to_string(i->values) + b->values);
            if (args[1]) return new Int(i->values + args[1]->ToInt());
            return args[0];
        }));
    Int::prototype->set("-", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Int(i->values - b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Float(i->values - b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Int(i->values - b->values);
            if (String* b = dynamic_cast<String*> (args[1])) return new String(std::to_string(i->values) + "-" + b->values);
            if (args[1]) return new Int(i->values - args[1]->ToInt());
            return args[0];
        }));
    Int::prototype->set("*", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Int(i->values * b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Float(i->values * b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Int(i->values * b->values);
            if (String* b = dynamic_cast<String*> (args[1]))
            {
                string s = "";
                for (int k = 0; k < i->values; ++k)
                {
                    s += b->values;
                }
                return new String(s);
            };
            if (args[1]) return new Int(i->values * args[1]->ToInt());
            return args[0];
        }));
    Int::prototype->set("/", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return b->values != 0 ? new Int(i->values / b->values) : NULL;
            if (Float* b = dynamic_cast<Float*> (args[1])) return b->values != 0 ? new Float(i->values / b->values) : NULL;
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return b->values != 0 ? new Int(i->values / b->values) : NULL;
            if (String* b = dynamic_cast<String*> (args[1]))  return  (b->values).size() != 0 ? new Int(i->values / (b->values).size()) : NULL;

            if (args[1]) {
                int k = args[1]->ToInt();
                return k != 0 ? new Int(i->values / k) : NULL;
            }
            return NULL;
        }));
    Int::prototype->set("==", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values == b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values == b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values == b->values);


            if (args[1]) return new Boolean(i->values == args[1]->ToInt());
            return new Boolean(1);
        }));
    Int::prototype->set("===", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values == b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values == b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values == b->values);


            if (args[1]) return new Boolean(i->values == args[1]->ToInt());
            return new Boolean(1);
        }));
    Int::prototype->set("!=", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values != b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values != b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values != b->values);


            if (args[1]) return new Boolean(i->values != args[1]->ToInt());
            return new Boolean(0);
        }));
    Int::prototype->set("!==", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values != b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values != b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values != b->values);


            if (args[1]) return new Boolean(i->values != args[1]->ToInt());
            return new Boolean(0);
        }));
    Int::prototype->set(">", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values > b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values > b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values > b->values);


            if (args[1]) return new Boolean(i->values > args[1]->ToInt());
            return new Boolean(0);
        }));
    Int::prototype->set("<", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values < b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values < b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values < b->values);


            if (args[1]) return new Boolean(i->values < args[1]->ToInt());
            return new Boolean(0);
        }));
    Int::prototype->set(">=", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values >= b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values >= b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values >= b->values);


            if (args[1]) return new Boolean(i->values >= args[1]->ToInt());
            return new Boolean(1);
        }));
    Int::prototype->set("<=", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values <= b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values <= b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values <= b->values);


            if (args[1]) return new Boolean(i->values <= args[1]->ToInt());
            return new Boolean(1);
        }));

}

void init_float() {
    Float::prototype = new Dict();


    Float::prototype->set("+", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Float* i = dynamic_cast<Float*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Float(i->values + b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Float(i->values + b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Float(i->values + b->values);
            if (String* b = dynamic_cast<String*> (args[1])) return new String(std::to_string(i->values) + b->values);
            if (args[1]) return new Float(i->values + args[1]->ToFloat());
            return args[0];
        }));
    Float::prototype->set("-", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Float* i = dynamic_cast<Float*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Float(i->values - b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Float(i->values - b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Float(i->values - b->values);
            if (String* b = dynamic_cast<String*> (args[1])) return new String(std::to_string(i->values) + "-" + b->values);
            if (args[1]) return new Float(i->values - args[1]->ToFloat());
            return args[0];
        }));
    Float::prototype->set("*", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Float* i = dynamic_cast<Float*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Float(i->values * b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Float(i->values * b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Float(i->values * b->values);
            if (String* b = dynamic_cast<String*> (args[1]))
            {
                string s = "";
                for (int k = 0; k < i->values; ++k)
                {
                    s += b->values;
                }
                return new String(s);
            };
            if (args[1]) return new Float(i->values * args[1]->ToFloat());
            return args[0];
        }));
    Float::prototype->set("/", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Float* i = dynamic_cast<Float*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return b->values != 0 ? new Int(i->values / b->values) : NULL;
            if (Float* b = dynamic_cast<Float*> (args[1])) return b->values != 0 ? new Float(i->values / b->values) : NULL;
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return b->values != 0 ? new Float(i->values / b->values) : NULL;
            if (String* b = dynamic_cast<String*> (args[1]))  return  (b->values).size() != 0 ? new Float(i->values / (b->values).size()) : NULL;

            if (args[1]) {
                int k = args[1]->ToFloat();
                return k != 0 ? new Float(i->values / k) : NULL;
            }
            return NULL;
        }));
    Float::prototype->set("==", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Float* i = dynamic_cast<Float*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values == b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values == b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values == b->values);


            if (args[1]) return new Boolean(i->values == args[1]->ToFloat());
            return new Boolean(1);
        }));
    Float::prototype->set("===", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Float* i = dynamic_cast<Float*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values == b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values == b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values == b->values);


            if (args[1]) return new Boolean(i->values == args[1]->ToFloat());
            return new Boolean(1);
        }));
    Float::prototype->set("!=", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Float* i = dynamic_cast<Float*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values != b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values != b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values != b->values);


            if (args[1]) return new Boolean(i->values != args[1]->ToFloat());
            return new Boolean(0);
        }));
    Float::prototype->set("!==", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Float* i = dynamic_cast<Float*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values != b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values != b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values != b->values);


            if (args[1]) return new Boolean(i->values != args[1]->ToFloat());
            return new Boolean(0);
        }));
    Float::prototype->set(">", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Float* i = dynamic_cast<Float*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values > b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values > b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values > b->values);


            if (args[1]) return new Boolean(i->values > args[1]->ToFloat());
            return new Boolean(0);
        }));
    Float::prototype->set("<", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Float* i = dynamic_cast<Float*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values < b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values < b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values < b->values);


            if (args[1]) return new Boolean(i->values < args[1]->ToFloat());
            return new Boolean(0);
        }));
    Float::prototype->set(">=", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Float* i = dynamic_cast<Float*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values >= b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values >= b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values >= b->values);


            if (args[1]) return new Boolean(i->values >= args[1]->ToFloat());
            return new Boolean(1);
        }));
    Float::prototype->set("<=", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Float* i = dynamic_cast<Float*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values <= b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values <= b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values <= b->values);


            if (args[1]) return new Boolean(i->values <= args[1]->ToFloat());
            return new Boolean(1);
        }));


}

void init_string() {
    String::prototype = new Dict();

    String::prototype->set("+", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            string a = args[0] ? args[0]->ToString() : "undefined";
            string b = args[1] ? args[1]->ToString() : "undefined";

            return new String(a + b);
        }));
    String::prototype->set("-", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;

            string a = args[0] ? args[0]->ToString() : "undefined";
            string b = args[1] ? args[1]->ToString() : "undefined";

            return new String(a + "-" + b);
        }));
    String::prototype->set("*", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;

            string a = args[0] ? args[0]->ToString() : "undefined";
            string b = args[1] ? args[1]->ToString() : "undefined";
            return new String(to_string(a.size() * b.size()));

        }));
    String::prototype->set("/", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            string a = args[0] ? args[0]->ToString() : "undefined";
            string b = args[1] ? args[1]->ToString() : "undefined";

            if (!b.size()) return NULL;
            return new String(to_string(a.size() / b.size()));

        }));
    String::prototype->set("==", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            string a = args[0] ? args[0]->ToString() : "undefined";
            string b = args[1] ? args[1]->ToString() : "undefined";

            return new Boolean(a == b);
        }));
    String::prototype->set("===", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            string a = args[0] ? args[0]->ToString() : "undefined";
            string b = args[1] ? args[1]->ToString() : "undefined";

            return new Boolean(a == b);
        }));
    String::prototype->set("!=", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            string a = args[0] ? args[0]->ToString() : "undefined";
            string b = args[1] ? args[1]->ToString() : "undefined";

            return new Boolean(a != b);
        }));
    String::prototype->set("!==", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            string a = args[0] ? args[0]->ToString() : "undefined";
            string b = args[1] ? args[1]->ToString() : "undefined";

            return new Boolean(a != b);
        }));
    String::prototype->set(">", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            string a = args[0] ? args[0]->ToString() : "undefined";
            string b = args[1] ? args[1]->ToString() : "undefined";

            return new Boolean(a > b);
        }));
    String::prototype->set("<", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            string a = args[0] ? args[0]->ToString() : "undefined";
            string b = args[1] ? args[1]->ToString() : "undefined";

            return new Boolean(a < b);
        }));
    String::prototype->set(">=", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            string a = args[0] ? args[0]->ToString() : "undefined";
            string b = args[1] ? args[1]->ToString() : "undefined";

            return new Boolean(a >= b);
        }));
    String::prototype->set("<=", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            string a = args[0] ? args[0]->ToString() : "undefined";
            string b = args[1] ? args[1]->ToString() : "undefined";

            return new Boolean(a <= b);
        }));


}

void init_boolean() {
    Boolean::prototype = new Dict();


    Boolean::prototype->set("+", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values + b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values + b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values + b->values);
            if (String* b = dynamic_cast<String*> (args[1])) return new String(std::to_string(i->values) + b->values);
            if (args[1]) return new Boolean(i->values + args[1]->ToInt());
            return args[0];
        }));
    Boolean::prototype->set("-", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values - b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values - b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values - b->values);
            if (String* b = dynamic_cast<String*> (args[1])) return new String(std::to_string(i->values) + "-" + b->values);
            if (args[1]) return new Boolean(i->values - args[1]->ToInt());
            return args[0];
        }));
    Boolean::prototype->set("*", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values * b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values * b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values * b->values);
            if (String* b = dynamic_cast<String*> (args[1]))
            {
                string s = "";
                for (int k = 0; k < i->values; ++k)
                {
                    s += b->values;
                }
                return new String(s);
            };
            if (args[1]) return new Boolean(i->values * args[1]->ToInt());
            return args[0];
        }));
    Boolean::prototype->set("/", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return b->values != 0 ? new Boolean(i->values / b->values) : NULL;
            if (Float* b = dynamic_cast<Float*> (args[1])) return b->values != 0 ? new Boolean(i->values / b->values) : NULL;
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return b->values != 0 ? new Boolean(i->values / b->values) : NULL;
            if (String* b = dynamic_cast<String*> (args[1]))  return  (b->values).size() != 0 ? new Boolean(i->values / (b->values).size()) : NULL;

            if (args[1]) {
                int k = args[1]->ToInt();
                return k != 0 ? new Boolean(i->values / k) : NULL;
            }
            return NULL;
        }));
    Boolean::prototype->set("==", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values == b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values == b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values == b->values);


            if (args[1]) return new Boolean(i->values == args[1]->ToInt());
            return new Boolean(1);
        }));
    Boolean::prototype->set("===", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values == b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values == b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values == b->values);


            if (args[1]) return new Boolean(i->values == args[1]->ToInt());
            return new Boolean(1);
        }));
    Boolean::prototype->set("!=", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values != b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values != b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values != b->values);


            if (args[1]) return new Boolean(i->values != args[1]->ToInt());
            return new Boolean(0);
        }));
    Boolean::prototype->set("!==", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values != b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values != b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values != b->values);


            if (args[1]) return new Boolean(i->values != args[1]->ToInt());
            return new Boolean(0);
        }));
    Boolean::prototype->set(">", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values > b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values > b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values > b->values);


            if (args[1]) return new Boolean(i->values > args[1]->ToInt());
            return new Boolean(0);
        }));
    Boolean::prototype->set("<", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values < b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values < b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values < b->values);


            if (args[1]) return new Boolean(i->values < args[1]->ToInt());
            return new Boolean(0);
        }));
    Boolean::prototype->set(">=", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values >= b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values >= b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values >= b->values);


            if (args[1]) return new Boolean(i->values >= args[1]->ToInt());
            return new Boolean(1);
        }));
    Boolean::prototype->set("<=", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;
            Int* i = dynamic_cast<Int*> (args[0]);
            if (!i) return NULL;
            if (Int* b = dynamic_cast<Int*> (args[1])) return new Boolean(i->values <= b->values);
            if (Float* b = dynamic_cast<Float*> (args[1])) return new Boolean(i->values <= b->values);
            if (Boolean* b = dynamic_cast<Boolean*> (args[1])) return new Boolean(i->values <= b->values);


            if (args[1]) return new Boolean(i->values <= args[1]->ToInt());
            return new Boolean(1);
        }));


}

void init_array() {
    Array::prototype = new Dict();

    Array::prototype->set("===", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;

            if (Array* a = dynamic_cast<Array*>(args[0])) {
                if (Array* b = dynamic_cast<Array*>(args[1])) {
                    if (a->values.size() != b->values.size())
                        return new Boolean(0);

                    for (int i = 0; i < a->values.size(); ++i)
                        if (a->values[i] != b->values[i])
                            return new Boolean(0);

                    return new Boolean(1);
                }

                return new Boolean(0);
            }

            return new Boolean(0);
        }));

    Array::prototype->set("!==", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;

            if (Array* a = dynamic_cast<Array*>(args[0])) {
                if (Array* b = dynamic_cast<Array*>(args[1])) {
                    if (a->values.size() != b->values.size())
                        return new Boolean(0);

                    for (int i = 0; i < a->values.size(); ++i)
                        if (a->values[i] != b->values[i])
                            return new Boolean(1);

                    return new Boolean(0);
                }

                return new Boolean(1);
            }

            return new Boolean(1);
        }));

    Array::prototype->set("==", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;

            return new Boolean(args[0] == args[1]);
        }));

    Array::prototype->set("==", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;

            return new Boolean(args[0] != args[1]);
        }));

    Array::prototype->set("+", new Function([](vector <Obj*> args)->Obj*
        {
            if (args.size() != 2) return NULL;

            if (Array* a = dynamic_cast<Array*>(args[0])) {
                if (Array* b = dynamic_cast<Array*>(args[1])) {
                    vector<Obj*> v = a->values;
                    v.insert(v.end(), b->values.begin(), b->values.end());
                    return new Array(v);
                }

                return new String(a->ToString() + (args[1] == nullptr ? "undefined" : args[1]->ToString()));
            }

            return new String((args[0] == nullptr ? "undefined" : args[0]->ToString()) + (args[1] == nullptr ? "undefined" : args[1]->ToString()));
        }));
}

void init_scope() {
    Scope::prototype = new Dict();


}



class AST {
public:
    virtual void print() {

    }
    virtual Obj* visit(Scope* s)
    {
        return NULL;
    };

};

class ASTBlock : public AST
{
public:
    vector <AST*> value;

    ASTBlock(vector <AST*>& v) {
        value = v;
    }
    virtual void print() {
        std::cout << " { ";
        for (int i = 0; i < value.size(); ++i)
        {
            value[i]->print();


            if (i != value.size() - 1)
                std::cout << " ";
        }

        std::cout << " } ";
    };
    virtual Obj* visit(Scope* s)
    {
        s = new Scope(s);
        for (int i = 0; i < value.size(); ++i)
        {
            value[i]->visit(s);
        }
        return NULL;
    };
};

class ASTEmpty : public AST
{
public:

    ASTEmpty() { };
    virtual void print()
    {
        std::cout << " ; ";
    };
};

class ASTAssignVar : public AST {
public:
    string name;
    AST* expr;

    ASTAssignVar(string n, AST* e) {
        name = n;
        expr = e;
    };
    virtual void print()
    {
        std::cout << " var " << name;
        if (expr == NULL) std::cout << " ; ";
        else
        {
            std::cout << " = ";
            expr->print();
            std::cout << " ; ";
        };
    };
    virtual Obj* visit(Scope* s)
    {

        Obj* o = NULL;
        if (expr != NULL) o = expr->visit(s);
        s->put(name, o);
        return NULL;
    };
};

class ASTOperations : public AST {
public:
    string operation;
    AST* left;
    AST* right;
    ASTOperations(AST* left, AST* right, string operation)
    {
        this->left = left;
        this->right = right;
        this->operation = operation;
    }

    virtual void print() {
        std::cout << " ( ";
        if (left == NULL)
            std::cout << "NULL";
        else
            left->print();
        std::cout << " ) ";

        std::cout << " " << operation << " ";

        std::cout << " ( ";
        if (right == NULL)
            std::cout << "NULL";
        else
            right->print();
        std::cout << " ) ";
    };

    virtual Obj* visit(Scope* s)
    {
        Obj* a;
        Obj* b;
        a = left->visit(s);
        b = right->visit(s);

        //
        if (!a)
            return NULL;

        Obj* f = a->get(operation);
        Callable* c = NULL;

        if ((f != NULL) && (c = dynamic_cast<Callable*> (f)))
        {
            vector <Obj*> v;
            v.push_back(a);
            v.push_back(b);
            return c->call(v);
        }



        return NULL;
    };
};

class ASTInt : public AST {
public:
    int value;
    ASTInt(int val_int) { value = val_int; };

    virtual void print() {
        std::cout << value;
    }
    virtual Obj* visit(Scope* s)
    {
        return new Int(value);
    };
};

class ASTFloat : public AST {
public:
    float value;
    ASTFloat(float val_float) { value = val_float; };

    virtual void print() {
        std::cout << value;
    }
    virtual Obj* visit(Scope* s)
    {
        return new Float(value);
    };
};

class ASTName : public AST {
public:
    string value;
    ASTName(string val_str) { value = val_str; };

    virtual void print() {
        std::cout << value;
    }

    virtual Obj* visit(Scope* s)
    {
        //
        if (!s)
            return NULL;
        return s->get(value);
    };
};

class ASTString : public AST {
public:
    string value;
    ASTString(string val_str) { value = val_str; };

    virtual void print() {
        std::cout << '"' << value << '"';
    }

    virtual Obj* visit(Scope* s)
    {
        return new String(value);
    };
};

class ASTNameAssign : public AST {
public:
    string value;
    AST* exp;
    string oreration;

    ASTNameAssign(string val_str, AST* v, string o)
    {
        value = val_str;
        exp = v;
        oreration = o;
    };

    virtual void print() {
        std::cout << value;
        std::cout << " " << oreration << " ";
        exp->print();

    }
    virtual Obj* visit(Scope* s)
    {
        //
        if (!s)
            return NULL;
        Obj* o = exp->visit(s);
        s->set(value, o);
        return o;
    };
};

class ASTArrayInit : public AST
{
public:
    vector <AST*> elements;
    ASTArrayInit(vector <AST*>& elem)
    {
        elements = elem;
    };


    virtual void print() {
        std::cout << " [ ";
        for (int i = 0; i < elements.size(); ++i)
        {
            elements[i]->print();

            if (i != elements.size() - 1)
                std::cout << " , ";
        }

        std::cout << " ] ";
    }
    virtual Obj* visit(Scope* s)
    {
        vector <Obj*>  o;
        for (int k = 0; k < elements.size(); k++) o.push_back(elements[k]->visit(s));

        return new Array(o);

    };

};

class ASTBoolean : public AST
{
public:
    bool value;
    ASTBoolean(bool b)
    {
        value = b;
    };

    virtual void print() {
        std::cout << (value ? "true" : "false");
    }
    virtual Obj* visit(Scope* s)
    {
        return new Boolean(value);
    };
};

class ASTField : public AST
{
public:
    AST* expr;
    string name;

    ASTField(AST* e, string& name)
    {
        expr = e;
        this->name = name;
    };

    virtual void print() {

        std::cout << " ( ";
        expr->print();
        std::cout << " ) . " << name;
    }
    virtual Obj* visit(Scope* s)
    {
        Obj* o = expr->visit(s);

        //
        if (!o)
            return NULL;

        return o->get(name);
    };

};

class ASTFieldAssign : public AST
{
public:
    AST* expr;
    string name;
    AST* value;


    ASTFieldAssign(AST* e, string& name, AST* v, string o)
    {
        expr = e;
        this->name = name;
        if (o != "=") value = new ASTOperations(new ASTField(e, name), v, o.substr(0, o.size() - 1));
        else value = v;

    };

    virtual void print()
    {
        std::cout << " ( ";
        expr->print();
        std::cout << " ) . " << name;
        std::cout << " ";
        value->print();
    }

    virtual Obj* visit(Scope* s)
    {
        Obj* o = expr->visit(s);
        Obj* b = value->visit(s);

        //
        if (!o)
            return NULL;

        o->set(name, b);
        return b;
    };
};

class ASTMember : public AST
{
public:
    AST* expr;
    AST* item;

    ASTMember(AST* e1, AST* e2)
    {
        expr = e1;
        item = e2;
    };

    virtual void print() {

        std::cout << " ( ";
        expr->print();
        std::cout << " ) [ ";
        item->print();
        std::cout << " ] ";
    };
    virtual Obj* visit(Scope* s)
    {
        Obj* o = expr->visit(s);
        Obj* a = item->visit(s);

        //
        if (!o)
            return NULL;
        if (!a)
            return NULL;

        return o->get(a->ToString());

    };
};

class ASTMemberAssign : public AST
{
public:
    AST* expr;
    AST* item;
    AST* value;


    ASTMemberAssign(AST* e1, AST* e2, AST* e3, string o)
    {
        expr = e1;
        item = e2;

        if (o != "=") value = new ASTOperations(new ASTMember(e1, e2), e3, o.substr(0, o.size() - 1));
        else value = e3;
    };

    virtual void print() {

        std::cout << " ( ";
        expr->print();
        std::cout << " ) [ ";
        item->print();
        std::cout << " ] ";

        value->print();
    };
    virtual Obj* visit(Scope* s)
    {
        Obj* o = expr->visit(s);
        Obj* a = item->visit(s);
        Obj* b = value->visit(s);

        //
        if (!o)
            return NULL;
        if (!a)
            return NULL;

        o->set(a->ToString(), b);
        return b;

    };
};

class ASTCall : public AST
{
public:
    AST* expr;
    vector <AST*> args;

    ASTCall(AST* e, vector <AST*>& a)
    {
        expr = e;
        args = a;
    };

    virtual void print() {
        std::cout << " ( ";
        expr->print();
        std::cout << " ) ( ";
        for (int i = 0; i < args.size(); ++i)
        {
            args[i]->print();

            if (i != args.size() - 1)
                std::cout << " , ";
        }

        std::cout << " ) ";
    }
    virtual Obj* visit(Scope* s)
    {
        Obj* o = expr->visit(s);

        if (Callable* c = dynamic_cast<Callable*>(o)) {
            vector <Obj*>  args_;
            for (int k = 0; k < args.size(); k++)
                args_.push_back(args[k]->visit(s));

            return c->call(args_);
        }
        cout << "DEBUG: reference in call is undefined" << std::endl;;
        return NULL;
    };
};

class ASTBreak : public AST
{
public:
    virtual void print() {

        std::cout << " break ";
    }
    virtual Obj* visit(Scope* s)
    {
        throw Break();
    }
};

class ASTContinue : public AST
{
public:
    virtual void print() {

        std::cout << " continue ";
    }
    virtual Obj* visit(Scope* s)
    {
        throw Continue();
    }
};

class ASTIfElse : public AST
{
public:
    AST* a;
    AST* b;
    AST* e;

    ASTIfElse(AST* ea, AST* aa, AST* ba)
    {
        e = ea;
        a = aa;
        b = ba;
    };
    virtual void print() {

        std::cout << " if ( ";
        e->print();
        std::cout << " ) ";
        a->print();
        if (b != NULL)
        {
            std::cout << " else ";
            b->print();
        }
    }
    virtual Obj* visit(Scope* s)
    {
        Obj* cond = e->visit(s);

        if (cond && cond->ToInt())
            a->visit(s);
        else if (b)
            b->visit(s);

        return NULL;
    }
};

class ASTWhile : public AST
{
public:

    AST* a;
    AST* e;

    ASTWhile(AST* ea, AST* ba)
    {
        e = ea;
        a = ba;
    };
    virtual void print() {

        std::cout << " while ( ";
        e->print();
        std::cout << " ) ";
        a->print();

    }
    virtual Obj* visit(Scope* s)
    {
        while (1) {
            try {
                Obj* cond = e->visit(s);

                if (cond && cond->ToInt())
                    a->visit(s);
                else
                    return NULL;

                continue;
            }
            catch (const Break& b) {
                return NULL;
            }
            catch (const Continue& c) {
                continue;
            }
        }
    }
};

class Parser {
public:
    vector<Tokini> tokens;


    int cursor = 0;
    Tokini fin = { LEX_FIN, "", 0, 0 };

    Parser(std::string& code_str)
    {
        tokens = push_tokini(code_str);
    };

    Tokini& next_token()
    {
        if (cursor >= tokens.size()) return fin;
        return tokens[cursor++];
    };
    Tokini& current_token()
    {
        if (cursor >= tokens.size()) return fin;
        return tokens[cursor];
    };
    void german_gref_token()
    {
        cursor--;
    };

    AST* statement()
    {
        AST* expr;

        if ((current_token().name == "var") && (current_token().type_token == LEX_KEYWORD))
        {
            next_token();
            if (current_token().type_token != LEX_NAME)
                throw "ERROR expected variable name";
            string name = current_token().name;
            next_token();
            if (current_token().type_token != LEX_DELIMITER) throw "ERROR expected = or ; in variable declaration";
            if (current_token().name == ";")
            {
                next_token();
                return new ASTAssignVar(name, NULL);
            };
            if (current_token().name == "=")
            {
                next_token();
                expr = new ASTAssignVar(name, expression());

                if ((current_token().name == ";") && (current_token().type_token == LEX_DELIMITER))
                {
                    next_token();
                    return expr;

                }
                return expr;
            };

        }
        else if ((current_token().name == "break") && (current_token().type_token == LEX_KEYWORD))
        {
            next_token();
            return new ASTBreak();
        }
        else if ((current_token().name == "continue") && (current_token().type_token == LEX_KEYWORD))
        {
            next_token();
            return new ASTContinue();
        }
        else if ((current_token().name == "if") && (current_token().type_token == LEX_KEYWORD))
        {
            next_token();
            if ((current_token().name != "(") || (current_token().type_token != LEX_DELIMITER))  throw "ERROR expected ( before condition";
            next_token();
            AST* e = expression();

            if ((current_token().name != ")") || (current_token().type_token != LEX_DELIMITER))
            {
                delete e;
                throw "ERROR expected ) after condition";
            }

            next_token();
            AST* a = block();

            if ((current_token().name == "else") && (current_token().type_token == LEX_KEYWORD))
            {
                next_token();
                AST* b = block();
                return new ASTIfElse(e, a, b);
            }

            return new ASTIfElse(e, a, NULL);
        }
        else if ((current_token().name == "while") && (current_token().type_token == LEX_KEYWORD))
        {
            next_token();
            if ((current_token().name != "(") || (current_token().type_token != LEX_DELIMITER))  throw "ERROR expected ( before condition";
            next_token();
            AST* e = expression();

            if ((current_token().name != ")") || (current_token().type_token != LEX_DELIMITER))
            {
                delete e;
                throw "ERROR expected ) after condition";
            }

            next_token();
            AST* a = block();


            return new ASTWhile(e, a);
        }
        else if ((current_token().name == "{") && (current_token().type_token == LEX_DELIMITER))
        {
            return block();
        }
        else if ((current_token().name == ";") && (current_token().type_token == LEX_DELIMITER))
        {
            next_token();
            return new ASTEmpty();
        }
        else
        {
            expr = expression();
            if ((current_token().name == ";") && (current_token().type_token == LEX_DELIMITER))
            {
                next_token();
                return expr;
                //throw  "ERROR expected ; after expression";

            }
            //next_token();
            return expr;
        };


    };

    AST* parse()
    {

        vector <AST*> elems;

        if (current_token().type_token == LEX_FIN)
        {
            return new ASTBlock(elems);
        }


        while (1)
        {
            AST* e = statement();
            elems.push_back(e);

            if (current_token().type_token == LEX_FIN)
            {
                return new ASTBlock(elems);
            }

        }

        return statement();

    };

    AST* block()
    {

        if (current_token().type_token == LEX_DELIMITER && current_token().name == "{")
        {
            next_token();
            vector <AST*> elems;

            if (current_token().type_token == LEX_DELIMITER)
            {
                if (current_token().name == "}") {
                    next_token();
                    return new ASTBlock(elems);
                }
                //throw  "ERROR expected ] in empty array";
            };

            while (1)
            {
                AST* e = statement();
                elems.push_back(e);

                if (current_token().type_token == LEX_DELIMITER)
                {

                    if (current_token().name == "}")
                    {
                        next_token();
                        return new ASTBlock(elems);
                    };

                };

            }

        }
        return statement();

    };

    AST* expression()
    {
        AST* expr;

        try
        {
            expr = e_logic();
            return expr;
        }
        catch (const char* msg)
        {
            std::cout << msg << std::endl;
            throw msg;
        }

        return NULL;
    };

    AST* e_logic()
    {
        AST* a = e_summ();
        string operation;
        AST* b = NULL;

        if ((current_token().type_token == LEX_DELIMITER) && ((current_token().name == "<") || (current_token().name == ">") || (current_token().name == ">=") || (current_token().name == "<=") || (current_token().name == "==") || (current_token().name == "!=") || (current_token().name == "===") || (current_token().name == "!==")))
        {
            operation = current_token().name;
            next_token();
            b = e_logic();
            return new ASTOperations(a, b, operation);
        }
        else
        {
            return a;
        }

    };

    AST* e_summ()
    {
        AST* a = e_mul();
        string operation;
        AST* b = NULL;

        if ((current_token().type_token == LEX_DELIMITER) && ((current_token().name == "+") || (current_token().name == "-")))
        {
            operation = current_token().name;
            next_token();
            b = e_summ();
            return new ASTOperations(a, b, operation);
        }
        else
        {
            return a;
        }

    };



    AST* e_mul()
    {
        AST* a = e_ass();
        string operation;
        AST* b = NULL;

        if ((current_token().type_token == LEX_DELIMITER) && ((current_token().name == "*") || (current_token().name == "/")))
        {
            operation = current_token().name;
            next_token();
            b = e_mul();
            return new ASTOperations(a, b, operation);
        }
        else
        {
            return a;
        }
    };

    AST* e_ass()
    {
        AST* e = e_ass2();
        AST* E;

        if ((current_token().type_token == LEX_DELIMITER) && ((current_token().name == "=") || (current_token().name == "+=") || (current_token().name == "-=") || (current_token().name == "*=") || (current_token().name == "/=")))
        {
            string oper = current_token().name;
            next_token();
            if (ASTField* v = dynamic_cast<ASTField*>(e))
            {
                E = expression();

                e = new ASTFieldAssign(v->expr, v->name, E, oper);
                v->expr = NULL;
                delete v;
            }
            else if (ASTMember* v = dynamic_cast<ASTMember*>(e))
            {
                E = expression();   

                e = new ASTMemberAssign(v->expr, v->item, E, oper);
                v->expr = NULL;
                v->item = NULL;
                delete v;

            }
            else if (ASTName* v = dynamic_cast<ASTName*>(e))
            {
                E = expression();

                e = new ASTNameAssign(v->value, E, oper);
                delete v;

            }
            else
            {
                delete e;
                throw "ERROR Expected lvalue in assignment";
            }
        }


        return e;
    };

    AST* e_ass2()
    {
        AST* e;
        string operation;
        AST* b = NULL;


        e = e_atom();
        while (1)
        {
            if (current_token().type_token == LEX_DELIMITER)
            {

                if (current_token().name == ".")
                {
                    next_token();
                    if (current_token().type_token != LEX_NAME)
                    {
                        delete e;
                        throw  "ERROR expected name after .";
                    };

                    e = new ASTField(e, current_token().name);
                    next_token();


                }
                else if (current_token().name == "[")
                {
                    next_token();
                    AST* E;

                    try {
                        E = expression();
                    }
                    catch (...)
                    {
                        delete e; throw  "ERROR ";  
                    };
                    if ((current_token().type_token != LEX_DELIMITER) || (current_token().name != "]"))
                    {
                        delete e;
                        delete E;
                        throw  "ERROR expected item expression after [";
                    };
                    next_token();
                    e = new ASTMember(e, E);

                }
                else if (current_token().name == "(")
                {
                    next_token();
                    vector <AST*> elems;

                    if (current_token().type_token == LEX_DELIMITER)
                    {
                        if (current_token().name == ")") {
                            next_token();
                            e = new ASTCall(e, elems);
                            continue;
                        }
                         
                    };

                    while (1)
                    {
                        AST* e = expression();
                        elems.push_back(e);

                        if (current_token().type_token == LEX_DELIMITER)
                        {
                            if (current_token().name == ",") {
                                next_token();
                                continue;
                            };
                            if (current_token().name == ")")
                            {
                                next_token();
                                break;
                            };

                        };

                        for (int i = 0; i < elems.size(); ++i)
                            delete elems[i];

                        throw  "ERROR expected , or ) in call args";
                    };

                    e = new ASTCall(e, elems);
                    continue;
                }
                else return e;

            }
            else return e;
        };

    };

    AST* e_atom()
    {

        switch (current_token().type_token)
        {
        case LEX_INT: { ASTInt* ret = new ASTInt(current_token().val_int); next_token(); return ret; }
        case LEX_FLOAT: { ASTFloat* ret = new ASTFloat(current_token().val_float); next_token(); return ret; }
        case LEX_NAME:
        {
            if (current_token().name == "true") return new ASTBoolean(1);
            if (current_token().name == "false") return new ASTBoolean(0);

            ASTName* ret = new ASTName(current_token().name);
            next_token();
            return ret;
        }
        case LEX_STRING: { ASTString* ret = new ASTString(current_token().name); next_token(); return ret; }
        case LEX_DELIMITER:
        {
            if (current_token().name == "(")
            {
                next_token();
                AST* e = expression();
                if (current_token().name != ")") { delete e; throw  "ERROR expected ) after () expression"; } 
                next_token();
                return e;
            }
            else if (current_token().name == "[")
            {
                next_token();
                vector <AST*> elems;

                if (current_token().type_token == LEX_DELIMITER)
                {
                    if (current_token().name == "]") {
                        next_token();
                        return new ASTArrayInit(elems);
                    }
                    
                };

                while (1)
                {
                    AST* e = expression();
                    elems.push_back(e);

                    if (current_token().type_token == LEX_DELIMITER)
                    {

                        if (current_token().name == ",") {
                            next_token();
                            continue;
                        };
                        if (current_token().name == "]")
                        {
                            next_token();
                            break;
                        };

                    };

                    for (int i = 0; i < elems.size(); ++i)
                        delete elems[i];

                    throw  "ERROR expected ] or , in array def";
                };

                return new ASTArrayInit(elems);

            }
        }
        default: {
            cursor = tokens.size();
            throw  "ERROR unexpected token " + current_token().name + " " + to_string(current_token().type_token);
            break; }

        }
    };

};


//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------


int main(int argc, char *argv[])
{
	 
    int size_request = atoi(argv[0]);
    

    for (int i = 0; i < size_request; i++)
        text_n += argv[1][i];
   
    
    
    
     
    try {
        

        Parser p(text_n);

        AST* s = p.parse();
         
        std::cout << "\nYour request:\n";
        s->print();   
        std::cout << "\nAnswer:             ";

        

        init_dict();
        init_function();
        init_int();
        init_float();
        init_string();
        init_boolean();
        init_array();
        init_scope();

        Scope* hs = new Scope(NULL);

        hs->put("print", new Function([](vector <Obj*> args)->Obj*
            {
                for (int i = 0; i < args.size(); i++)
                {
                    if (args[i])
                    {
                        
                        std::cout << args[i]->ToString();
                    }
                    else std::cout << "undefined";
                }

                return new Int(args.size());
            }));

        try {
            s->visit(hs);
        }
        catch (const Error& e) {
            std::cout << "Error: " << e.message << std::endl;
        }
        catch (...) {
            std::cout << "Unknown Error" << std::endl;
        }
    }
    catch (const string& msg) {
       
        cout << msg << endl;
    }
    catch (const char* msg) {
      
        cout << msg << endl;
    }

    
	
	 
	return 0;

}


 