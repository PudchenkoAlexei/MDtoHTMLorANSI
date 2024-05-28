#include "pch.h"
#include "CppUnitTest.h"

#include "..\mark2\mark2.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace MarkdownTests
{
    TEST_CLASS(MarkdownConversionTests)
    {
    public:

        TEST_METHOD(TestProcessMarkdownToHTML_SimpleText)
        {
            string input = "Hello, world!";
            string expected = "<p>Hello, world!</p>\n";
            string actual = processMarkdownToHTML(input);
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TestProcessMarkdownToHTML_ItalicText)
        {
            string input = "This is _italic_ text.";
            string expected = "<p>This is <i>italic</i> text.</p>\n";
            string actual = processMarkdownToHTML(input);
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TestProcessMarkdownToHTML_MonospaceText)
        {
            string input = "This is `monospace` text.";
            string expected = "<p>This is <tt>monospace</tt> text.</p>\n";
            string actual = processMarkdownToHTML(input);
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TestProcessMarkdownToHTML_BoldText)
        {
            string input = "This is **bold** text.";
            string expected = "<p>This is <b>bold</b> text.</p>\n";
            string actual = processMarkdownToHTML(input);
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TestProcessMarkdownToHTML_PreformattedText)
        {
            string input = "```\npreformatted text\n```";
            string expected = "<p><pre>\npreformatted text\n</pre></p>\n";
            string actual = processMarkdownToHTML(input);
            Assert::AreEqual(expected.c_str(), actual.c_str());
        }

        TEST_METHOD(TestProcessMarkdownToANSI_SimpleText)
        {
            string input = "Hello, world!";
            string expected = "Hello, world!\n";
            string actual = processMarkdownToANSI(input);
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TestProcessMarkdownToANSI_BoldText)
        {
            string input = "This is **bold** text.";
            string expected = "This is \033[1mbold\033[0m text.\n";
            string actual = processMarkdownToANSI(input);
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TestProcessMarkdownToANSI_ItalicText)
        {
            string input = "This is _italic_ text.";
            string expected = "This is \033[3mitalic\033[0m text.\n";
            string actual = processMarkdownToANSI(input);
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TestProcessMarkdownToANSI_MonospaceText)
        {
            string input = "This is `monospace` text.";
            string expected = "This is \033[7mmonospace\033[0m text.\n";
            string actual = processMarkdownToANSI(input);
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TestProcessMarkdownToANSI_PreformattedText)
        {
            string input = "```\npreformatted text\n```";
            string expected = "\033[7m\npreformatted text\n\033[0m\n";
            string actual = processMarkdownToANSI(input);
            Assert::AreEqual(expected.c_str(), actual.c_str());
        }
    };
}
