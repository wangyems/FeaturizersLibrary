// ----------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License
// ----------------------------------------------------------------------
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../Strings.h"

using namespace Microsoft::Featurizer::Strings;

bool isWhiteSpace(char c) {
    if (c == ' ')
        return true;
    return false;
}

void IteratorVectorGeneratorTest(std::string const & input) {
    std::vector<std::string::const_iterator> iterVec(Details::IteratorVectorGenerator(input.begin(), input.end()));
    CHECK(input == std::string(iterVec[0], iterVec[iterVec.size() - 1]));
}

void ParseNgramCharHelperTest(std::string const & input,
                              std::vector<std::string> const & label,
                              size_t ngramRangeMin,
                              size_t ngramRangeMax) {
    std::vector<std::string> output;
    Details::ParseNgramCharHelper<std::string::const_iterator>(
        input.begin(),
        input.end(),
        ngramRangeMin,
        ngramRangeMax,
        [&output] (std::string::const_iterator iterBegin, std::string::const_iterator iterEnd) {
            output.emplace_back(std::string(iterBegin, iterEnd));
        }
    );
    CHECK(output == label);
}

void ParseTest(std::string const & input, std::vector<std::string> const & label) {
    std::vector<std::string> output1;
    Details::Parse<std::string::const_iterator>(
        input.begin(),
        input.end(),
        isWhiteSpace,
        [&output1] (std::string::const_iterator iterBegin, std::string::const_iterator iterEnd) {
            output1.emplace_back(std::string(iterBegin, iterEnd));
        }
    );
    CHECK(output1 == label);
    std::vector<std::string> output2;
    Parse<std::string::const_iterator>(
        input,
        isWhiteSpace,
        [&output2] (std::string::const_iterator iterBegin, std::string::const_iterator iterEnd) {
            output2.emplace_back(std::string(iterBegin, iterEnd));
        }
    );
    CHECK(output2 == label);
}

void ParseRegexTest(std::string const & input,
                    std::vector<std::string> const & label,
                    std::string const & token = "[^\\s]+") {
    std::vector<std::string> output1;
    ParseRegex(
        input,
        token,
        [&output1] (std::string::const_iterator iterBegin, std::string::const_iterator iterEnd) {
            output1.emplace_back(std::string(iterBegin, iterEnd));
        }
    );
    CHECK(output1 == label);
    std::vector<std::string> output2;
    ParseRegex(
        input,
        token,
        [&output2] (std::string::const_iterator iterBegin, std::string::const_iterator iterEnd) {
            output2.emplace_back(std::string(iterBegin, iterEnd));
        }
    );
    CHECK(output2 == label);
}

void ParseNgramWordTest(std::string & input,
                        std::vector<std::string> const & label,
                        size_t ngramRangeMin,
                        size_t ngramRangeMax) {
    std::vector<std::string> output1;
    ParseNgramWord<std::string::const_iterator>(
        input,
        isWhiteSpace,
        ngramRangeMin,
        ngramRangeMax,
        [&output1] (std::string::const_iterator iterBegin, std::string::const_iterator iterEnd) {
            output1.emplace_back(std::string(iterBegin, iterEnd));
        }
    );
    CHECK(output1 == label);
}

void ParseNgramCharTest(std::string & input,
                        std::vector<std::string> const & label,
                        size_t ngramRangeMin,
                        size_t ngramRangeMax) {
    std::vector<std::string> output1;
    ParseNgramChar<std::string::const_iterator>(
        input,
        ngramRangeMin,
        ngramRangeMax,
        [&output1] (std::string::const_iterator iterBegin, std::string::const_iterator iterEnd) {
            output1.emplace_back(std::string(iterBegin, iterEnd));
        }
    );
    CHECK(output1 == label);
}

void ParseNgramCharwbTest(std::string & input,
                          std::vector<std::string> const & label,
                          size_t ngramRangeMin,
                          size_t ngramRangeMax) {
    std::vector<std::string> output1;
    ParseNgramCharwb<std::string::const_iterator>(
        input,
        isWhiteSpace,
        ngramRangeMin,
        ngramRangeMax,
        [&output1] (std::string::const_iterator iterBegin, std::string::const_iterator iterEnd) {
            output1.emplace_back(std::string(iterBegin, iterEnd));
        }
    );
    CHECK(output1 == label);
}

TEST_CASE("IteratorVectorGenerator") {
    IteratorVectorGeneratorTest("this is a document");
}

TEST_CASE("ToLower") {
    std::string input("THIS IS THE FIRST DOCUMENT.");
    std::string label("this is the first document.");
    std::string output(ToLower(input));
    CHECK(output == label);
}

TEST_CASE("ToUpper") {
    std::string input("this is the first document.");
    std::string label("THIS IS THE FIRST DOCUMENT.");
    std::string output(ToUpper(input));
    CHECK(output == label);
}

TEST_CASE("TrimLeft") {
    std::string input("    this is the first document.");
    std::string label("this is the first document.");
    std::string output(TrimLeft(input, isWhiteSpace));
    CHECK(output == label);
}

TEST_CASE("TrimRight") {
    std::string input("this is the first document.        ");
    std::string label("this is the first document.");
    std::string output(TrimRight(input, isWhiteSpace));
    CHECK(output == label);
}

TEST_CASE("Trim") {
    std::string input("     this is the first document.     ");
    std::string label("this is the first document.");
    std::string output(Trim(input, isWhiteSpace));
    CHECK(output == label);
}

TEST_CASE("ReplaceAndTrimDuplicate") {
    std::string input("!is  this the   * first#document  ?");
    std::string label(" is this the first document ");
    std::string output(Details::ReplaceAndDeDuplicate<std::function<bool (char)>>(input));
    CHECK(output == label);
}

TEST_CASE("Parse") {
    ParseTest("this is a document", {"this", "is", "a", "document"});
    ParseTest("this   is a   document ", {"this", "is", "a", "document"});
    ParseTest(" this is   a document", {"this", "is", "a", "document"});
    ParseTest(" this is a document ", {"this", "is", "a", "document"});
    ParseTest(" this   is a   document  ", {"this", "is", "a", "document"});
    ParseTest("this", {"this"});
    ParseTest(" this", {"this"});
    ParseTest("this ", {"this"});
    ParseTest(" this ", {"this"});
    ParseTest("  this  ", {"this"});
    ParseTest("", {});
    ParseTest(" ", {});
    ParseTest("  ", {});
}

TEST_CASE("ParseRegex") {
    ParseRegexTest("this is a document", {"this", "is", "a", "document"});
    ParseRegexTest("this   is a   document ", {"this", "is", "a", "document"});
    ParseRegexTest(" this is   a document", {"this", "is", "a", "document"});
    ParseRegexTest(" this is a document ", {"this", "is", "a", "document"});
    ParseRegexTest(" this   is a   document  ", {"this", "is", "a", "document"});
}

TEST_CASE("ParseNgramWord") {
    std::string inputRaw("? this$is a   document  &");
    std::string input0(Details::ReplaceAndDeDuplicate<std::function<bool (char)>>(inputRaw));
    ParseNgramWordTest(input0, {"this", "is", "a", "document"}, 1, 1);
    ParseNgramWordTest(input0, {"this is a", "is a document"}, 3, 3);
    ParseNgramWordTest(input0, {"this is", "is a", "a document"}, 2, 2);
    ParseNgramWordTest(input0, {"this is a document"}, 4, 4);
    ParseNgramWordTest(input0, {"this", "is", "a", "document", "this is", "is a", "a document"}, 1, 2);
    ParseNgramWordTest(input0, {"this is", "is a", "a document", "this is a", "is a document"}, 2, 3);
    ParseNgramWordTest(input0, {"this is a", "is a document", "this is a document"}, 3, 4);
    ParseNgramWordTest(input0, {"this", "is", "a", "document", "this is", "is a", "a document", "this is a", "is a document"}, 1, 3);
    ParseNgramWordTest(input0, {"this is", "is a", "a document", "this is a", "is a document", "this is a document"}, 2, 4);
    ParseNgramWordTest(input0, {"this", "is", "a", "document", "this is", "is a", "a document", "this is a", "is a document", "this is a document"}, 1, 4);
    std::string input(" bi grams are cool ");
    std::string emptyInput(" ");
    CHECK_THROWS_WITH(ParseNgramWordTest(emptyInput, {}, 1, 1), "wordIterPairVector.size() == 0");
    CHECK_THROWS_WITH(ParseNgramWordTest(input, {}, 0, 8), "ngramRangeMin and ngramRangeMax not valid");
    ParseNgramWordTest(input, {"bi", "grams", "are", "cool", "bi grams", "grams are", "are cool"}, 1, 2);
    ParseNgramWordTest(input, {"bi grams", "grams are", "are cool", "bi grams are", "grams are cool"}, 2, 3);
}

TEST_CASE("ParseNgramCharHelper") {
    ParseNgramCharHelperTest(" jumpy ", {" jump", "jumpy", "umpy "}, 5, 5);
}

TEST_CASE("ParseNgramChar") {
    std::string inputRaw("jumpy   ?? fox");
    std::string input(Details::ReplaceAndDeDuplicate<std::function<bool (char)>>(inputRaw));
    ParseNgramCharTest(input, {"j", "u", "m", "p", "y", " ", "f", "o", "x"}, 1, 1);
    ParseNgramCharTest(input, {"ju", "um", "mp", "py", "y ", " f", "fo", "ox"}, 2, 2);
    ParseNgramCharTest(input, {"jum", "ump", "mpy", "py ", "y f", " fo", "fox"}, 3, 3);
    ParseNgramCharTest(input, {"jump", "umpy", "mpy ", "py f", "y fo", " fox"}, 4, 4);
    ParseNgramCharTest(input, {"jumpy", "umpy ", "mpy f", "py fo", "y fox"}, 5, 5);
    ParseNgramCharTest(input, {"jumpy ", "umpy f", "mpy fo", "py fox"}, 6, 6);
    ParseNgramCharTest(input, {"jumpy f", "umpy fo", "mpy fox"}, 7, 7);
    ParseNgramCharTest(input, {"jumpy fo", "umpy fox"}, 8, 8);
    ParseNgramCharTest(input, {"jumpy fox"}, 9, 9);
    std::string emptyInput(" ");
    CHECK_THROWS_WITH(ParseNgramCharTest(emptyInput, {}, 0, 3), "ngramRangeMin and ngramRangeMax not valid");
    CHECK_THROWS_WITH(ParseNgramCharTest(input, {}, 10, 10), "ngramRangeMin and ngramRangeMax not valid");
    ParseNgramCharTest(input, {"jumpy fox"}, 9, 9);
}

TEST_CASE("ParseNgramCharwb") {
    std::string inputRaw = "? jumpy ^fox )";
    std::string inputPre(Details::ReplaceAndDeDuplicate<std::function<bool (char)>>(inputRaw));
    std::string input0(Details::StringPadding<std::function<bool (char)>>(inputPre, isWhiteSpace));
    CHECK_THROWS_WITH(ParseNgramCharwbTest(input0, {}, 0, 3), "ngramRangeMin and ngramRangeMax not valid");
    ParseNgramCharwbTest(input0, {" ", "j", "u", "m", "p", "y", " ", " ", "f", "o", "x", " "}, 1, 1);
    ParseNgramCharwbTest(input0, {" j", "ju", "um", "mp", "py", "y ", " f", "fo", "ox", "x "}, 2, 2);
    ParseNgramCharwbTest(input0, {" ju", "jum", "ump", "mpy", "py ", " fo", "fox", "ox "}, 3, 3);
    ParseNgramCharwbTest(input0, {" jum", "jump", "umpy", "mpy ", " fox", "fox "}, 4, 4);
    ParseNgramCharwbTest(input0, {" jump", "jumpy", "umpy ", " fox "}, 5, 5);
    ParseNgramCharwbTest(input0, {" jumpy", "jumpy "}, 6, 6);
    ParseNgramCharwbTest(input0, {" jumpy "}, 7, 7);
    ParseNgramCharwbTest(input0, {}, 8, 8);
    std::string input("? jumpy ^fox )");
    CHECK_THROWS_WITH(ParseNgramCharwbTest(input, {}, 0, 8), "ngramRangeMin and ngramRangeMax not valid");
    ParseNgramCharwbTest(input, {}, 8, 8);
}
