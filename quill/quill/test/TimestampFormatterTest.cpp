#include "quill/detail/TimestampFormatter.h"
#include <gtest/gtest.h>

using namespace quill::detail;

/***/
TEST(TimestampFormatter, simple_format_string)
{
  // invalid format strings
  EXPECT_THROW(TimestampFormatter ts_formatter{"%I:%M%p%Qms%S%Qus z"}, std::runtime_error);
  EXPECT_THROW(TimestampFormatter ts_formatter{"%I:%M%p%Qms%S%Qus%Qns z"}, std::runtime_error);
  EXPECT_THROW(TimestampFormatter ts_formatter{"%I:%M%p%S%Qus%Qns z"}, std::runtime_error);

  // valid simple string
  EXPECT_NO_THROW(TimestampFormatter ts_formatter{"%I:%M%p%S%Qns z"});
}

/***/
TEST(TimestampFormatter, format_string_no_additional_specifier)
{
  const std::chrono::nanoseconds timestamp{1587161887987654321};

  // simple formats without any ms/us/ns specifiers
  {
    TimestampFormatter ts_formatter{"%H:%M:%S", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "22:18:07");
  }

  {
    TimestampFormatter ts_formatter{"%F %H:%M:%S", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "2020-04-17 22:18:07");
  }

  // large simple string to cause reallocation
  {
    TimestampFormatter ts_formatter{"%A %B %d %T %Y %F", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "Friday April 17 22:18:07 2020 2020-04-17");
  }
}

/***/
TEST(TimestampFormatter, format_string_with_millisecond_precision)
{
  // simple
  {
    const std::chrono::nanoseconds timestamp{1587161887987654321};
    TimestampFormatter ts_formatter{"%H:%M:%S.%Qms", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "22:18:07.987");
  }

  // with double formatting
  {
    const std::chrono::nanoseconds timestamp{1587161887803654321};
    TimestampFormatter ts_formatter{"%H:%M:%S.%Qms %D", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "22:18:07.803 04/17/20");
  }

  // with double formatting 2
  {
    const std::chrono::nanoseconds timestamp{1587161887023654321};
    TimestampFormatter ts_formatter{"%H:%M:%S.%Qms-%G", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "22:18:07.023-2020");
  }

  // with zeros
  {
    const std::chrono::nanoseconds timestamp{1587161887009654321};
    TimestampFormatter ts_formatter{"%H:%M:%S.%Qms", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "22:18:07.009");
  }
}

/***/
TEST(TimestampFormatter, format_string_with_microsecond_precision)
{
  // simple
  {
    const std::chrono::nanoseconds timestamp{1587161887987654321};
    TimestampFormatter ts_formatter{"%H:%M:%S.%Qus", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "22:18:07.987654");
  }

  // with double formatting
  {
    const std::chrono::nanoseconds timestamp{1587161887803654321};
    TimestampFormatter ts_formatter{"%H:%M:%S.%Qus %D", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "22:18:07.803654 04/17/20");
  }

  // with double formatting 2
  {
    const std::chrono::nanoseconds timestamp{1587161887010654321};
    TimestampFormatter ts_formatter{"%H:%M:%S.%Qus-%G", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "22:18:07.010654-2020");
  }

  // with zeros
  {
    const std::chrono::nanoseconds timestamp{1587161887000004321};
    TimestampFormatter ts_formatter{"%H:%M:%S.%Qus", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "22:18:07.000004");
  }
}

/***/
TEST(TimestampFormatter, format_string_with_nanosecond_precision)
{
  // simple
  {
    const std::chrono::nanoseconds timestamp{1587161887987654321};
    TimestampFormatter ts_formatter{"%H:%M:%S.%Qns", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "22:18:07.987654321");
  }

  // with double formatting
  {
    const std::chrono::nanoseconds timestamp{1587161887803654320};
    TimestampFormatter ts_formatter{"%H:%M:%S.%Qns %D", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "22:18:07.803654320 04/17/20");
  }

  // with double formatting 2
  {
    const std::chrono::nanoseconds timestamp{1587161887000654321};
    TimestampFormatter ts_formatter{"%H:%M:%S.%Qns-%G", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "22:18:07.000654321-2020");
  }

  // with zeros
  {
    const std::chrono::nanoseconds timestamp{1587161887000000009};
    TimestampFormatter ts_formatter{"%H:%M:%S.%Qns", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "22:18:07.000000009");
  }

  // with max
  {
    const std::chrono::nanoseconds timestamp{1587161887999999999};
    TimestampFormatter ts_formatter{"%H:%M:%S.%Qns", quill::Timezone::GmtTime};

    auto const& result = ts_formatter.format_timestamp(timestamp);
    EXPECT_STREQ(result, "22:18:07.999999999");
  }
}