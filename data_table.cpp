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
        FieldInfo(const function<T(const string&)>& parse_func):
            parse_func_(parse_func)
        {}

    private:
        FieldParseFuncT parse_func_;
};

template <typename T>
FieldInfoBase* make_field(function<T(const string&)> func)
{
    FieldInfoBase * field_ptr = new FieldInfo<T>(func);
    return field_ptr;
}

class FieldInitHelper
{
    public:
        FieldInitHelper(DataTable* data_table_ptr);
    public:
        FieldInitHelper& operator(FieldInfoBase* field_base)
        {
            shared_ptr<FieldInfoBase> ptr(field_base);
            data_table_ptr->add(ptr);
            return *this;
        }
    private:
        DataTable* data_table_ptr;
};

class DataTable
{
    public:
        FieldInitHelper add_fields()
        {
            return FieldInitHelper(this);

        }
        void add(shared_ptr<FieldInfoBase> field_info)
        {
            field_info_list.push_back(field_info);
        }
    private:
        vector<shared_ptr<FieldInfoBase>> field_info_list;
};

