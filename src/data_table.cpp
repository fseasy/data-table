#include <fstream>
#include "data_table.h"

DataTable::DataTable(std::istream& is, const std::string& field_delim,
        bool auto_close_stream)
    : is(is),
    field_delim(field_delim),
    auto_close_stream(auto_close_stream)
{}

DataTable::~DataTable()
{
    for(auto *ptr : parser_ptr_list)
    {
        delete ptr;
        ptr = nullptr;
    }
    if (auto_close_stream) 
    { 
        std::ifstream *ifs_ptr = dynamic_cast<std::ifstream *>(&is);
        if(ifs_ptr){ ifs_ptr->close(); }
    }
}


DataTable& DataTable::getrow(Row& row)
{
    std::string line;
    if(!std::getline(is, line))
    {
        row.set_status(false);
        return *this;
    }
    parse_line2row(line, row);
    return *this;
}
