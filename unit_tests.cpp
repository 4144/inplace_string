#define SMALL_STRING_SANITY_CHECKS
#include "small_string.h"

#include <boost/noncopyable.hpp>
#include <gtest/gtest.h>

TEST(small_string, init)
{
	small_string s;
	ASSERT_TRUE(s.empty());
	ASSERT_EQ(s.size(), 0);

	s = small_string(31, 'a');
	std::cout << sizeof(s) << std::endl;
	std::cout << s << std::endl;
	printf("%s\n", s.c_str());
}

TEST(small_string, init_basic_string)
{
	std::string str = "foo";
	small_string ss(str);

	ASSERT_FALSE(ss.empty());
	ASSERT_EQ(ss.size(), 3);
	ASSERT_EQ("foo", ss);
	ASSERT_EQ("foo", std::string(ss.c_str()));
}

TEST(small_string, substr)
{
	small_string s("123456");
	small_string ss = s.substr();

	EXPECT_EQ(s, ss);

	ss = s.substr(4);
	EXPECT_EQ(2, int(ss.size()));
	EXPECT_EQ('5', ss.at(0));
	EXPECT_EQ('6', ss.at(1));

	ss = s.substr(4, 1);
	EXPECT_EQ(1, int(ss.size()));
	EXPECT_EQ('5', ss.at(0));
}

TEST(small_string, iterator)
{
	small_string s("123456");
	small_string::size_type sum = 0;

	for (const char ch : s)
		sum += ch;

	EXPECT_EQ(int('1') * 6 + 15, sum);
}

TEST(small_string, compare_eq)
{
	small_string s1("123456");
	small_string s2("123456");
	EXPECT_TRUE(s1 == s2);
	EXPECT_EQ(s1.compare(s2), 0);
	EXPECT_TRUE(s1 == "123456");
	EXPECT_TRUE("123456" == s1);
}

TEST(small_string, compare_ne)
{
	small_string s1("123456");
	small_string s2("123356");
	EXPECT_TRUE(s1 != s2);
	EXPECT_NE(s1.compare(s2), 0);
}

TEST(small_string, compare_lt)
{
	small_string s1("123456");
	small_string s2("123456789");
	EXPECT_TRUE(s1 < s2);
	EXPECT_LT(s1.compare(s2), 0);

	s1 = "123456";
	s2 = "123466";
	EXPECT_TRUE(s1 < s2);
	EXPECT_LT(s1.compare(s2), 0);
}

TEST(small_string, compare_lte)
{
	small_string s1("123456");
	small_string s2("123456789");
	EXPECT_TRUE(s1 <= s2);
	EXPECT_LE(s1.compare(s2), 0);

	s1 = "123456";
	s2 = "123456";
	EXPECT_TRUE(s1 <= s2);
	EXPECT_LE(s1.compare(s2), 0);
}

TEST(small_string, compare_gt)
{
	small_string s1("123356789");
	small_string s2("123356");
	EXPECT_TRUE(s1 > s2);
	EXPECT_GT(s1.compare(s2), 0);

	s1 = "123466";
	s2 = "123456";
	EXPECT_TRUE(s1 > s2);
	EXPECT_GT(s1.compare(s2), 0);
}

TEST(small_string, compare_gte)
{
	small_string s1("123356789");
	small_string s2("123356");
	EXPECT_TRUE(s1 >= s2);
	EXPECT_GE(s1.compare(s2), 0);

	s1 = "123456";
	s2 = "123456";
	EXPECT_TRUE(s1 >= s2);
	EXPECT_GE(s1.compare(s2), 0);
}

TEST(small_string, append)
{
	small_string s("foo");
	s.append("bar");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", s);
	EXPECT_EQ("foobar", std::string(s.c_str()));

	s = "foo";
	s.append(std::string("baz"));
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobaz", s);

	s = "foo";
	s.append("burbar", 3);
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobur", s);

	s = "foo";
	s.append(std::experimental::string_view("buz"));
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobuz", s);

	s = "foo";
	s.append(3, 'z');
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foozzz", s);

	s = "foo";

	struct A : std::experimental::string_view
	{
		A() : std::experimental::string_view("bar") { }
	};
	A a;
	s.append(a, 0);
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", s);
}

TEST(small_string, append_operator)
{
	small_string s("foo");
	s += "bar";
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", s);
	EXPECT_EQ("foobar", std::string(s.c_str()));

	s = "foo";
	s += std::string("baz");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobaz", s);

	s = "foo";
	s += 'b';
	EXPECT_EQ(4, s.size());
	EXPECT_EQ("foob", s);

	s = "foo";
	s += std::experimental::string_view("buz");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobuz", s);

	s = "foo";
	s += {'b', 'a', 'r'};
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", s);
}

TEST(small_string, resize)
{
	small_string s("foo");
	s.resize(3, 'z');
	EXPECT_EQ("foo", s);

	s.resize(6, 'z');
	EXPECT_EQ("foozzz", s);

	s.resize(2);
	EXPECT_EQ("fo", s);

	s.resize(6, 'z');
	EXPECT_EQ("fozzzz", s);

	s.resize(6, 'o');
	EXPECT_EQ("fozzzz", s);
}

TEST(small_string, replace_basic)
{
	small_string s("fooFOO");

	s.replace(3, 3, "bar");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", std::string(s.c_str()));

	s.replace(0, 3, "FOO");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("FOObar", std::string(s.c_str()));

	s.replace(3, 3, "BARFOO");
	EXPECT_EQ(9, s.size());
	EXPECT_EQ("FOOBARFOO", std::string(s.c_str()));

	s.replace(3, 6, "BUZ");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("FOOBUZ", std::string(s.c_str()));

	s.replace(0, 3, "foobar");
	EXPECT_EQ(9, s.size());
	EXPECT_EQ("foobarBUZ", std::string(s.c_str()));

	s.replace(0, 6, "FOO");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("FOOBUZ", std::string(s.c_str()));

	s.replace(0, 6, "foo");
	EXPECT_EQ(3, s.size());
	EXPECT_EQ("foo", std::string(s.c_str()));

	s.replace(0, 3, "foobarbuzbaz");
	EXPECT_EQ(12, s.size());
	EXPECT_EQ("foobarbuzbaz", std::string(s.c_str()));

	s.replace(0, 1, "FOOBARf");
	EXPECT_EQ(18, s.size());
	EXPECT_EQ("FOOBARfoobarbuzbaz", std::string(s.c_str()));

	s.replace(0, 7, "f");
	EXPECT_EQ(12, s.size());
	EXPECT_EQ("foobarbuzbaz", std::string(s.c_str()));
}

TEST(small_string, replace)
{
	small_string s("fooFOOBAR");

	s.replace(3, 6, small_string("bar"));
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", std::string(s.c_str()));

	s.replace(0, 6, std::string("FOObar"));
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("FOObar", std::string(s.c_str()));

	s.replace(s.begin(), s.begin() + 3, std::string("FOOBAR"));
	EXPECT_EQ(9, s.size());
	EXPECT_EQ("FOOBARbar", std::string(s.c_str()));

	s.replace(s.begin(), s.begin() + 6, small_string("foo"));
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foobar", std::string(s.c_str()));

	s.replace(3, 3, std::string("BARFOO"), 0, 3);
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("fooBAR", std::string(s.c_str()));

	s.replace(0, 3, small_string("BARFOO"), 3, 3);
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("FOOBAR", std::string(s.c_str()));

	std::string str("foobarfoo");
	s.replace(s.begin(), s.end(), str.begin(), str.end());
	EXPECT_EQ(9, s.size());
	EXPECT_EQ("foobarfoo", std::string(s.c_str()));

	s.replace(3, 6, "foobar", 3);
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("foofoo", std::string(s.c_str()));

	s.replace(s.begin() + 3, s.end(), "FOOBARfoo", 6);
	EXPECT_EQ(9, s.size());
	EXPECT_EQ("fooFOOBAR", std::string(s.c_str()));

	s = small_string(31, 'a');
	s.replace(s.begin(), s.end(), "foo");
	EXPECT_EQ(3, s.size());
	EXPECT_EQ("foo", std::string(s.c_str()));


#if 0
basic_small_string_t& replace(size_type pos, size_type count, size_type count2, value_type ch)
basic_small_string_t& replace(const_iterator first, const_iterator last, size_type count2, value_type ch)
basic_small_string_t& replace(const_iterator first, const_iterator last, std::initializer_list<value_type> ilist)
basic_small_string_t& replace(size_type pos, size_type count, std::experimental::basic_string_view<CharT, Traits> sv)
basic_small_string_t& replace(const_iterator first, const_iterator last, std::experimental::basic_string_view<CharT, Traits> sv)
#endif


	s.replace(1, 2, "OOBAR");
	EXPECT_EQ(6, s.size());
	EXPECT_EQ("fOOBAR", std::string(s.c_str()));

	s.replace(3, 3, s.max_size() - 3, 'z');
	EXPECT_EQ(s.max_size(), s.size());
	EXPECT_EQ("foo" + std::string(s.max_size() - 3, 'z'), std::string(s.c_str()));
}
