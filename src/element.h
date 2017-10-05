#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <stdexcept>

class FieldParserBase
{
    public:
       virtual ~FieldParserBase(){}
};

template <typename T>
class FieldParser: public FieldParserBase
{
    public:
        using ParserActionT = std::function<bool(const std::string&, T&)>;
    public:
    FieldParser(ParserActionT parser_action)
        :parser_action(parser_action)
    {}

    public:
    bool parse(const std::string& val_in_str, T& val)
    {
        return parser_action(val_in_str, val);
    }

    private:
    ParserActionT parser_action;
};

class Field
{
    public:
        Field(){}
        Field(const std::string& val_in_str, FieldParserBase* field_parser):
            val_in_str(val_in_str),
            field_parser(field_parser)
    {}
    
    public:
        template <typename T>
        bool parse(T& val)
        {
            FieldParser<T> *derived_parser = static_cast<FieldParser<T> *>(field_parser);
            return derived_parser->parse(val_in_str, val);
        }

    private:
        FieldParserBase *field_parser;
        std::string val_in_str;
};


class Row
{
    public:
        Row():is_good_(false) {}
        Row(const std::vector<std::string>& val_list, 
            const std::vector<FieldParserBase*>& parser_ptr_list)
        {
            std::size_t val_num = val_list.size();
           if (val_num != parser_ptr_list.size())
           {
               is_good_ = false;
               return;
           }
           field_list.reserve(val_num);
           for (std::size_t i = 0; i < val_num; i++)
           {
                field_list.emplace_back(val_list[i], parser_ptr_list[i]);
           }
           is_good_ = true;
        }
        
        Row(const Row&) = default;
        Row& operator=(const Row&) = default;
        Row(Row&& rhs) noexcept:
            is_good_(rhs.is_good_),
            field_list(std::move(rhs.field_list))
        {
            rhs.is_good_ = false;
            rhs.field_list = {};
        }
        
        Row& operator=(Row&& rhs) noexcept
        {
            if(&rhs == this){ return *this; }
            is_good_ = rhs.is_good_;
            field_list = std::move(rhs.field_list);
            rhs.is_good_ = false;
            rhs.field_list = {};
            return *this;
        }

    public:
        Field& at(std::size_t i)
        {
            const Field& f = (const_cast<const Row&>(*this)).at(i);
            return const_cast<Field &>(f);
        }

        const Field& at(std::size_t i) const
        {
           return field_list[i];
        }

    public:
        bool is_good(){ return is_good_; }
        bool set_status(bool is_good_){ this->is_good_ = is_good_; } 
    private:
        bool is_good_;
        std::vector<Field> field_list;
};
