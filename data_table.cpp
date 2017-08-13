#include <memory>
#include <functional>
#include <vector>

using namespace std;

class FieldInfoBase
{
    virtual ~FieldInfoBase(){};
};

template<typename T>
class FieldInfo: public FieldInfoBase
{

    public:
        using FieldType = T;
        using FieldParseFuncT = function<T(const string&)>;
    public:
        FieldInfo(const function<T(const string&)>& parse_func);

    private:
        FieldParseFuncT parse_func_;

};

class DataTable
{
    public:
        
    private:
       vector<sharted_ptr<FieldInfoBase>> field_info_list;
};
