#include <iostream>
#include <fstream>
#include "data_table.h"
#include "default_parser_action.h"

using namespace std;

int main(int argc, char* argv[])
{
    const string input_path = "./test/access.log";
    const string field_delim = "\t";
    
    fstream is(input_path);
    if(!is)
    {
        cerr << "failed to open " << input_path << endl;
        return 1;
    }
    DataTable table(is, field_delim, false);
    table.add_field<int>(&str2int);
    table.add_field<string>(&str2str);
    Row row;
    while(table.getrow(row))
    {
        if (!row.is_good())
        {
            cerr << "parse error for line: " << table.current_line() << endl;
            continue;
        }
        int id;
        string target;
        bool read_status;
        read_status &= row.at(0).parse(id);
        read_status &= row.at(1).parse(target);
        if (read_status == false)
        {
            cerr << "parse error for line: " << table.current_line() << endl;
            continue;
        }
        cout << id << "\t" << target << endl;
    }

    return 0;
}
