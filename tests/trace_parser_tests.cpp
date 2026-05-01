#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "replacement_sim/trace_parser.hpp"

namespace {

/**
 * 페이지 접근 벡터가 예상된 페이지 ID 시퀀스와 일치하는지 검증하는 헬퍼 함수
 */
void assert_pages(const std::vector<replacement_sim::PageAccess>& trace,
                  const std::vector<std::uint64_t>& expected) {
    assert(trace.size() == expected.size());

    for (std::size_t i = 0; i < expected.size(); ++i) {
        assert(trace[i].page_id == expected[i]);
    }
}

/**
 * 기본 페이지 ID를 파싱하는지 테스트
 */
void parses_basic_page_ids() {
    std::istringstream input("0\n1\n2\n0\n3\n");

    const auto trace = replacement_sim::parse_trace(input);

    assert_pages(trace, {0, 1, 2, 0, 3});
}

/**
 * 빈 줄과 주석을 무시하는지 테스트
 */
void ignores_blank_lines_and_comments() {
    std::istringstream input(
        "# simple trace\n"
        "\n"
        "  10  \n"
        "\t\n"
        "20\n"
        "   # comment after leading spaces\n"
        "30\n");

    const auto trace = replacement_sim::parse_trace(input);

    assert_pages(trace, {10, 20, 30});
}

/**
 * 음수 페이지 ID를 거부하는지 테스트
 */
void rejects_negative_page_ids() {
    std::istringstream input("-1\n");

    try {
        (void)replacement_sim::parse_trace(input);
        assert(false);
    } catch (const replacement_sim::TraceParseError& error) {
        const std::string message = error.what();
        assert(message.find("line 1") != std::string::npos);
        assert(message.find("non-negative") != std::string::npos);
    }
}

/**
 * 페이지 ID 뒤에 추가 토큰이 있는 경우를 거부하는지 테스트
 */
void rejects_trailing_tokens() {
    std::istringstream input("42 extra\n");

    try {
        (void)replacement_sim::parse_trace(input);
        assert(false);
    } catch (const replacement_sim::TraceParseError& error) {
        const std::string message = error.what();
        assert(message.find("line 1") != std::string::npos);
        assert(message.find("unexpected characters") != std::string::npos);
    }
}

/**
 * 파일에서 트레이스를 파싱하는지 테스트
 */
void parses_trace_file() {
    const auto path = std::filesystem::temp_directory_path() /
                      "replacement_sim_trace_parser_test.trace";

    {
        std::ofstream output(path);
        output << "# file input\n";
        output << "7\n";
        output << "8\n";
        output << "7\n";
    }

    const auto trace = replacement_sim::parse_trace_file(path);
    std::filesystem::remove(path);

    assert_pages(trace, {7, 8, 7});
}

}  // namespace

void run_trace_parser_tests() {
    parses_basic_page_ids();
    ignores_blank_lines_and_comments();
    rejects_negative_page_ids();
    rejects_trailing_tokens();
    parses_trace_file();
}
