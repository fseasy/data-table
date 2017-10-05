#ifndef UTILS_STR_UTIL_H
#define UTILS_STR_UTIL_H
#include <vector>
#include <string>


void split(const std::string& line, std::vector<std::string>& parts,
        const std::string& delim);

inline
void split(const std::string& line, std::vector<std::string>& parts,
        const std::string& delim)
{
    using std::swap;
    std::vector<std::string> tmp_parts;
    std::size_t start_pos = 0u;
    while(true)
    {
        std::size_t delim_pos = line.find(delim, start_pos);
        std::size_t count = delim_pos == std::string::npos ? 
            std::string::npos : 
            delim_pos - start_pos;
        tmp_parts.push_back(line.substr(start_pos, count));
        if(delim_pos != std::string::npos) { start_pos = delim_pos + 1; } 
        else { break; }
    }
    swap(parts, tmp_parts);
}


#endif
