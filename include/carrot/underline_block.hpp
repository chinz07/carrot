#ifndef CARROT_UNDERLINE_BLOCK_HPP
#define CARROT_UNDERLINE_BLOCK_HPP

#include <carrot/block.hpp>

#include <memory>

namespace carrot
{

class underline_block : public block
{
public:
    explicit underline_block(std::shared_ptr<block> underlined_element_);

    void render(matrix& mat) const override;

    std::array<long int, 2> extent() const override;

private:
    std::shared_ptr<block> underlined_element_;
};
}

#endif