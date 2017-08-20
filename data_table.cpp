#include <memory>
#include <functional>
#include <vector>

using namespace std;

class FieldInfoBase
{
    public:
        virtual ~FieldInfoBase(){};
};

template<typename T, typename FuncT>
class FieldInfo: public FieldInfoBase
{
    public:
        using FieldType = T;
        using FieldParseFuncT = FuncT;
    public:
        FieldInfo(const FuncT& parse_func):
            parse_func_(parse_func)
        {}
        FuncT* get_parse_func(){ return &parse_func; }

    private:
        FuncT parse_func_;
};

template <typename T, typename FuncT>
FieldInfoBase* make_field(const FuncT& func)
{
    FieldInfoBase * field_ptr = new FieldInfo<T, FuncT>(func);
    return field_ptr;
}

class FieldData
{
    public:
        FieldData(const string& value_in_str,
                  shared_ptr<FieldInfoBase> field_info)
            : value_in_str(value_in_str),
              p_field_info(field_info)
        {}
        template <typename T>
        bool parse(T& value)
        { 
            return p_field_info->get_parse_func()(value_in_str, value);
        };
    private:
        string value_in_str;
        shared_ptr<FieldInfoBase> p_field_info;
};

class RawData
{
    public:
        RawData(){}
        RawData(const vector<string>& field_str_repr_list,
                const vector<shared_ptr<FieldInfoBase>>& field_info_list)
        {
            assert(field_str_repr_list.size() ==
                   field_info_list.size());
            for(size_t i = 0U; i < field_info_list.size(); ++i)
            {
                field_data_list.emplace_back(field_str_repr_list[i],
                                             field_info_list[i]);
            }
        }
        template<typename T>
        bool get(unsigned index, T& value)
        {
            return field_data_list[index].parse(value);
        }

    public:
        void clear(){ field_data_list.clear();  }
    private:
        vector<FieldData> field_data_list;
};

class FieldInitHelper
{
    public:
        FieldInitHelper(DataTable* data_table_ptr):
            data_table_ptr(data_table_ptr)
    {};
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
        DataTable(istream& is, const string& field_delimiter,
                  bool close_istream_when_deconstruction=true)
            :is(is),
             field_delimieter(field_delimieter),
             close_istream_when_deconstruction(close_istream_when_deconstruction)
        {}
    public:
        DataTable& get_next_row(RawData &row, bool &is_row_ok)
        {
            string line;
            bool read_state = getline(is, line);
            if(read_state != true)
            {
                is_row_ok = false;
                return *this;
            }
            vector<string> field_str_repr_list;
            bool split_state = split_line2fields(line, field_str_repr_list);
            if(split_state == false)
            {
                is_row_ok = false;
                return *this;
            }
            RawData row_tmp(field_str_repr_list, field_info_list);
            swap(row_tmp, row);
            is_row_ok = true;
            return *this;
        }

        /**
         * for get_next_row.
         * do as `while(getline(xxx))`
         * */
        explicit operator bool() const
        {
            return static_cast<bool>(is);
        }
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
        bool split_line2fields(const string& line, vector<string>& field_str_repr_list)
        {
            using std::swap;
            vector<string> fields_tmp(field_info_list.size());
            size_t start_pos = 0u;
            size_t field_index = 0u;
            while(true)
            {
                size_t delim_pos = line.find(field_delimieter, start_pos);
                size_t count = delim_pos == string::npos ? string::npos : delim_pos - start_pos;
                fields[field_index++] = line.substr(start_pos, count;
                if(delim_pos != string::npos)
                {
                    start_pos = delim_pos + 1;
                } 
                else
                {
                    break;
                }
            }
            if(field_index != field_info_list.size()){ return false; }
            else
            {
                swap(fields_tmp, field_str_repr_list);
                return true;
            }
        }
    private:
        vector<shared_ptr<FieldInfoBase>> field_info_list;
        istream is;
        string field_delimiter;
        bool close_istream_when_deconstruction;
};

