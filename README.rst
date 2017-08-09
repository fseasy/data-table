=======================
data table 数据表解析
=======================

在百度 goodcoder中有这个题目，需要实现一个支持解析用户自定义类型的数据表解析工具。

有些像数据库，又有些像 ``boost::program_options`` 的功能。

期待的样子
==============

```

data_table.add_fields()
    ( DataTable::int_field() )
    ( DataTable::int_array_field<7>() )
    ( DataTable::user_defined_field(self_defined_parse_function) )
    ( DataTable::user_defined_field<user_defined_t>());

for(auto row_iter = data_table.begin();
    row_iter != data_table.end();
    ++row_iter)
{
    row_iter->get<int>(0);
    row_iter->get<int[7]>(1);
    row_iter->get<traits_t>(2);
    row_iter->get<user_defined_t>(3);
    // or like this
    // int col0;
    // bool get_state = row_iter->get(0, col0);
    // ...
}

```


需要实现的功能
===============

1. 类似 ``boost::any``

2. 迭代器

3. 给定格式的数据表解析能力
