//  Copyright (c) 2015-2018 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <carrot/target_info.hpp>

#include <boost/locale/generator.hpp>

#ifdef __unix__

#include <unistd.h>

#endif

#ifdef HAVE_TERMINFO

extern "C" int setupterm(char* term, int filedes, int* errret);
extern "C" struct term* set_curterm(struct term* termp);
extern "C" int del_curterm(struct term* termp);
extern "C" int tigetnum(char* capname);

#include <locale>
#include <mutex>

#endif

namespace carrot
{

namespace
{

#ifdef HAVE_TERMINFO
std::mutex term_mutex;
#endif

bool terminal_has_colors(int fd) noexcept
{
#ifdef HAVE_TERMINFO
    std::lock_guard<std::mutex> guard(term_mutex);

    int errret = 0;
    if (setupterm(static_cast<char*>(nullptr), fd, &errret) != 0)
        return false;

    // Disable the const_cast check for the next line, tigetnum is not const correct
    // and does not actually modify the passed string.
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    bool has_colors = tigetnum(const_cast<char*>("colors")) > 0;

    struct term* termp = set_curterm(static_cast<struct term*>(nullptr));
    (void)del_curterm(termp);

    return has_colors;
#else
    return false;
#endif
}

bool has_color_support(int fd) noexcept
{
#ifdef __unix__
    bool is_a_tty = isatty(fd) > 0;
    bool has_colors = terminal_has_colors(fd);

    return is_a_tty && has_colors;
#else
    return false;
#endif
}
} // namespace

invalid_target_error::invalid_target_error(const std::string& reason_)
: runtime_error("Invalid target: " + reason_)
{
}

target_info::target_info(const std::locale& locale_, bool supports_colorized_output_,
                         long int tab_width_) noexcept
: locale_(locale_), supports_colorized_output_(supports_colorized_output_), tab_width_(tab_width_)
{
}

bool target_info::supports_colorized_output() const noexcept
{
    return supports_colorized_output_;
}

long int target_info::tab_width() const noexcept
{
    return tab_width_;
}

const std::locale& target_info::locale() const noexcept
{
    return locale_;
}

namespace
{
std::locale get_default_locale() noexcept
{
    return std::locale("");
}
} // namespace

target_info get_stdout_target(long int tab_width) noexcept
{
    return get_stdout_target(get_default_locale(), tab_width);
}

target_info get_stdout_target(const std::locale& locale, long int tab_width) noexcept
{
#ifdef __unix__
    bool colorize_output = has_color_support(STDOUT_FILENO);
#else
    bool colorize_output = false;
#endif
    return target_info(locale, colorize_output, tab_width);
}

target_info get_file_target(long int tab_width) noexcept
{
    return get_file_target(get_default_locale(), tab_width);
}

target_info get_file_target(const std::locale& locale, long int tab_width) noexcept
{
    return target_info(locale, false, tab_width);
}

target_info get_colorized_target(long int tab_width) noexcept
{
    return get_colorized_target(get_default_locale(), tab_width);
}

target_info get_colorized_target(const std::locale& locale, long int tab_width) noexcept
{
    return target_info(locale, true, tab_width);
}

} // namespace carrot