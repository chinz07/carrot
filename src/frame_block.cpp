#include <carrot/frame_block.hpp>

#include <carrot/form_view.hpp>

#include <utility>

namespace carrot
{

frame_block::frame_block(std::shared_ptr<block> framed_block_, long int margin_)
: framed_block_(framed_block_), margin_(margin_)
{
}

void frame_block::render(form & mat) const
{
    auto extent = framed_block_->extent();

    for (long int i = 0; i < extent[1] + 2 * margin_ + 2; ++i)
    {
        mat.set(0, i, '-');
        mat.set(extent[0] + 2 * margin_ + 1, i, '-');
    }

    for (long int i = 1; i < extent[0] + 2 * margin_ + 1; ++i)
    {
        mat.set(i, 0, '|');
        mat.set(i, extent[1] + 2 * margin_ + 1, '|');
    }

    form_view view(mat, 1 + margin_, 1 + margin_);

    framed_block_->render(view);
}

std::array<long int, 2> frame_block::extent() const
{
    auto extent = framed_block_->extent();

    return std::array<long int, 2>{extent[0] + 2 * margin_ + 2, extent[1] + 2 * margin_ + 2};
}

std::shared_ptr<block> frame(std::shared_ptr<block> framed_block, long int margin)
{
    return std::make_shared<frame_block>(std::move(framed_block), margin);
}

}
