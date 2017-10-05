#include <stdexcept>
#include <iostream>
/*********
 * default parser functions
 * ******/
bool str2int(const std::string& val_in_str, int& val)
{
    try { val = stoi(val_in_str); } 
    catch(std::invalid_argument e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

bool str2str(const std::string& val_in_str, std::string& val)
{
    try{ val = val_in_str; }
    catch(std::exception e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}
