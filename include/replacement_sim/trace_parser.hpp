#pragma once

#include <cstdint>
#include <filesystem>
#include <istream>
#include <stdexcept>
#include <string>
#include <vector>

namespace replacement_sim {

/**
 * 페이지 접근을 나타내는 구조체
 * 추후 필요에 따라 읽기/쓰기 구분 등의 추가 필드를 포함하도록 확장 가능
 */
struct PageAccess {
    std::uint64_t page_id;
};

/**
 * 트레이스 파싱 중 발생하는 예외를 나타내는 클래스
 */
class TraceParseError : public std::runtime_error {
public:
    explicit TraceParseError(const std::string& message);
};

/**
 * trace_parser.cpp에서 구현된 트레이스 파싱 함수
 * 유효하지 않은 트레이스 라인에 대해서는 TraceParseError 예외를 던짐
 */
std::vector<PageAccess> parse_trace(std::istream& input);
std::vector<PageAccess> parse_trace_file(const std::filesystem::path& path);

}  // namespace replacement_sim
