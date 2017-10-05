#ifndef SRC_DATA_TABLE_H
#define SRC_DATA_TBALE_H
#include <iostream>
#include "element.h"
#include "str_utils.h"

class DataTable
{
    public:
        DataTable(std::istream& is, const std::string& field_delim,
                bool auto_close_stream=false);
        ~DataTable();

    public:
        template <typename T>
        DataTable& add_field(std::function<bool(const std::string&, T&)> parser_action);

        DataTable& getrow(Row& row);

    public:
        explicit operator bool() const;

    public:
        std::size_t read_line_count() const noexcept { return line_cnt; }
        const std::string& current_line() const noexcept { return current_line_; } 

    protected:
        void parse_line2row(const std::string& line, Row& row);

    private:
        std::istream &is;
        std::string field_delim;
        std::vector<FieldParserBase*> parser_ptr_list;
        bool auto_close_stream;

    // save read status
    private:
        std::size_t line_cnt;
        std::string current_line_;
};


/******************
 * inline implementation
 */

template <typename T>
inline
DataTable& DataTable::add_field(std::function<bool(const std::string&, T&)> parser_action)
{
    FieldParser<T> *parser_ptr = new FieldParser<T>(parser_action);
    parser_ptr_list.push_back(parser_ptr);
}

inline
void DataTable::parse_line2row(const std::string& line, Row& row)
{
    std::vector<std::string> parts;
    split(line, parts, field_delim);
    row = Row(parts, parser_ptr_list);
}

inline
DataTable::operator bool() const
{
    return is.good();
}

#endif
