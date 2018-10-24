//  Copyright (c) 2015-2018 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef CARROT_FORM_HPP
#define CARROT_FORM_HPP

#include <carrot/glyph.hpp>
#include <carrot/target_info.hpp>

#include "carrot_export.hpp"

#include <boost/multi_array.hpp>

namespace carrot
{

class CARROT_EXPORT form
{
public:
    form() = default;
    virtual ~form() = default;

    form(const form&) = delete;
    form& operator=(const form&) = delete;

    form(form&&) = delete;
    form& operator=(form&&) = delete;

    virtual void set(long int row, long int column, glyph value) = 0;

    virtual const target_info& target() const = 0;
};
}

#endif
