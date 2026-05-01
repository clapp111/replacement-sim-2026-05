#include <charconv>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string_view>

#include "replacement_sim/trace_parser.hpp"

namespace replacement_sim {
namespace {

/**
 * 문자열의 앞뒤 공백을 제거하는 함수
 */
std::string_view trim(std::string_view value) {
    while (!value.empty() &&
           std::isspace(static_cast<unsigned char>(value.front())) != 0) {
        value.remove_prefix(1);
    }

    while (!value.empty() &&
           std::isspace(static_cast<unsigned char>(value.back())) != 0) {
        value.remove_suffix(1);
    }

    return value;
}

/**
 * 유효하지 않은 트레이스 라인에 대한 예외를 던지는 함수
 */
TraceParseError parse_error(std::size_t line_number, std::string_view line, std::string_view reason) {
    std::ostringstream message;
    message << "invalid trace at line " << line_number << ": " << reason
            << " ('" << line << "')";
    return TraceParseError(message.str());
}

/**
 * 문자열에서 page_id를 파싱하는 함수
 */
std::uint64_t parse_page_id(std::string_view line, std::size_t line_number) {
    if (line.front() == '-') {
        throw parse_error(line_number, line, "page id must be non-negative");
    }

    std::uint64_t page_id = 0;
    const char* begin = line.data();
    const char* end = line.data() + line.size();
    const auto result = std::from_chars(begin, end, page_id);

    // page_id가 숫자가 아닌 경우
    if (result.ec == std::errc::invalid_argument) {
        throw parse_error(line_number, line, "expected an unsigned integer page id");
    }

    // unit64_t 범위를 벗어난 page_id는 허용되지 않음
    if (result.ec == std::errc::result_out_of_range) {
        throw parse_error(line_number, line, "page id is out of uint64_t range");
    }

    // page_id 뒤에 공백이 아닌 문자가 있으면 유효하지 않은 라인으로 간주
    if (result.ptr != end) {
        throw parse_error(line_number, line, "unexpected characters after page id");
    }

    return page_id;
}

}  // namespace

TraceParseError::TraceParseError(const std::string& message)
    : std::runtime_error(message) {}


/**
 * 입력 스트림에서 트레이스를 파싱하는 함수
 */
std::vector<PageAccess> parse_trace(std::istream& input) {
    std::vector<PageAccess> trace;
    std::string line;
    std::size_t line_number = 0;

    while (std::getline(input, line)) {
        ++line_number;

        // 빈 줄과 주석 라인( '#'로 시작하는 라인)은 무시
        const std::string_view trimmed = trim(line);
        if (trimmed.empty() || trimmed.front() == '#') {
            continue;
        }

        trace.push_back(PageAccess{parse_page_id(trimmed, line_number)});
    }

    return trace;
}


/**
 * 경로에서 트레이스 파일을 읽어서 파싱하는 함수
 */
std::vector<PageAccess> parse_trace_file(const std::filesystem::path& path) {
    std::ifstream input(path);

    // 파일을 열 수 없는 경우에는 예외를 던짐
    if (!input) {
        std::ostringstream message;
        message << "failed to open trace file: " << path;
        throw TraceParseError(message.str());
    }

    return parse_trace(input);
}

}  // namespace replacement_sim
