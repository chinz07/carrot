#include <carrot/text_block.hpp>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <iterator>
#include <functional>

namespace carrot
{

text_block::text_block(const std::string& content_)
{
    boost::split(rows_, content_, boost::is_any_of("\n"));
}

void text_block::render(matrix& mat) const
{
    for (long int row = 0; row < rows_.size(); ++row)
    {
        for (long int column = 0; column < rows_[row].size(); ++column)
        {
            mat.set(row, column, rows_[row][column]);
        }
    }
}

std::array<long int, 2> text_block::extent() const
{
    long int rows = rows_.size();

    std::function<long int(const std::string&)> get_row_lenght = [](const std::string& value)
    {
        return value.size();
    };

    auto row_lenghts = rows_ | boost::adaptors::transformed(get_row_lenght);

    auto max_row_lenght = boost::max_element(row_lenghts);

    long int columns = max_row_lenght != std::end(row_lenghts) ? *max_row_lenght : 0;

    return std::array<long int, 2>{rows, columns};
}
}