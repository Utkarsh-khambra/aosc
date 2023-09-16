#include "frwd_decl.hpp"
#include <array>
#include <cstdlib>
#include <fmt/core.h>
#include <fstream>
#include <memory>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

class Pipeline {
public:
  virtual void execute(std::string &str) noexcept {
    for (auto &part : m_pipeline) {
      part->execute(str);
    }
    fmt::print("{}", str);
  }

  void add(std::unique_ptr<Pipeline> p) noexcept {
    m_pipeline.emplace_back(std::move(p));
  }
  virtual ~Pipeline() {}

private:
  std::vector<std::unique_ptr<Pipeline>> m_pipeline;
};

class ShowTabs : public Pipeline {
public:
  void execute(std::string &str) noexcept override {
    size_t pos = 0;
    while (true) {
      auto result = str.find("\t", pos);
      if (result == std::string::npos)
        break;
      str.at(result) = '^';
      str.insert(result + 1, "I");
      pos = result;
      if (pos >= str.size())
        break;
    }
  };
};

class SqueezBlank : public Pipeline {
public:
  void execute(std::string &str) noexcept override {
    fmt::print("Called from SqueezBlank \n");
    // fmt::print("Called from SqueezBlank {}\n", str);
  };
};
class Number : public Pipeline {
public:
  Number(bool show_num_on_empty_line)
      : m_show_num_on_empty_line(show_num_on_empty_line) {}
  void execute(std::string &str) noexcept override {
    size_t pos = 0;
    while (true) {
      auto result = str.find('\n', pos);
      if (result == std::string::npos) {
        break;
      }
      auto temp = std::to_string(m_lineNr++);
      str.insert(pos, "  " + temp + "  ");
      pos = result + 5 + temp.size();
      if (pos >= str.size())
        break;
    }
  };
  virtual ~Number() {}
  int m_lineNr = 1;
  bool const m_show_num_on_empty_line = true;
};

class ShowEnd : public Pipeline {
public:
  void execute(std::string &str) noexcept override {
    size_t pos = 0;
    while (true) {
      auto result = str.find("\n", pos);
      if (result == std::string::npos)
        break;
      str.insert(result, "$");
      pos = result + 2;
      if (pos >= str.size())
        break;
    }
    // fmt::print("Called from ShowEnd {}\n", str);
  };
};

enum class CAT_FLAGS {
  NONE = 0,
  SHOW_ALL = 1,
  NUMBER_NONBLANK = 1 << 2,
  SHOW_END = 1 << 3,
  NUMBER = 1 << 4,
  SQUEEZ_BLANK = 1 << 5,
  SHOW_TABS = 1 << 6
};

[[nodiscard]] constexpr CAT_FLAGS operator|(CAT_FLAGS lhs,
                                            CAT_FLAGS rhs) noexcept {
  return static_cast<CAT_FLAGS>(std::to_underlying(lhs) |
                                std::to_underlying(rhs));
}

[[nodiscard]] constexpr bool operator&(CAT_FLAGS lhs, CAT_FLAGS rhs) noexcept {
  return std::to_underlying(lhs) & std::to_underlying(rhs);
}

template <size_t s>
std::string_view to_strview(std::array<char, s> &arr) noexcept {
  return std::string_view(arr.data(), arr.size());
}

constexpr CAT_FLAGS remove_flag(CAT_FLAGS flags, CAT_FLAGS flag) noexcept {
  if (flags & flag) {
    flags = static_cast<CAT_FLAGS>(std::to_underlying(flags) &
                                   (~std::to_underlying(flag)));
  }
  return flags;
}

auto parse_arguments(int argc, char **argv) {
  int opt;
  CAT_FLAGS flags = CAT_FLAGS::NONE;
  using enum CAT_FLAGS;
  while ((opt = getopt(argc, argv, "AbEnsT")) != -1) {
    switch (opt) {
    case 'A':
      flags = flags | SHOW_ALL;
      break;
    case 'b':
      flags = flags | NUMBER_NONBLANK;
      break;
    case 'E':
      flags = flags | SHOW_END;
      break;
    case 'n':
      flags = flags | NUMBER;
      break;
    case 's':
      flags = flags | SQUEEZ_BLANK;
      break;
    case 'T':
      flags = flags | SHOW_TABS;
      break;
    default:
      optind += 1;
    }
  }
  std::vector<std::string> filenames;
  if (flags & SHOW_ALL) {
    flags = remove_flag(flags, SHOW_END);
    flags = remove_flag(flags, SHOW_TABS);
  }
  for (auto i = optind; i < argc; ++i)
    filenames.emplace_back(argv[i]);

  return std::make_pair(flags, filenames);
}

auto show_end(std::string &line) noexcept { line += '&'; }
auto show_tabs() noexcept {}
auto show_number(std::string &line, size_t lineNr) noexcept {
  line = std::to_string(lineNr) + line;
}
auto squeez_blank() noexcept {}

void day1(int argc, char **argv) noexcept {
  using enum CAT_FLAGS;
  std::string buffer;
  buffer.resize(4096);
  auto [flags, filenames] = parse_arguments(argc, argv);

  Pipeline pipeline;

  if (flags & SHOW_ALL) {
    pipeline.add(std::make_unique<ShowEnd>());
    pipeline.add(std::make_unique<ShowTabs>());
  }
  if (flags & SHOW_END) {
    pipeline.add(std::make_unique<ShowEnd>());
  }
  if (flags & NUMBER_NONBLANK) {
    fmt::print("Got non blank all\n");
    pipeline.add(std::make_unique<Number>(true));
  } else if (flags & NUMBER) {
    pipeline.add(std::make_unique<Number>(false));
  }
  if (flags & SQUEEZ_BLANK) {
    fmt::print("Got SQUEEZ_BLANK\n");
    pipeline.add(std::make_unique<SqueezBlank>());
  }
  if (flags & SHOW_TABS) {
    pipeline.add(std::make_unique<ShowTabs>());
  }

  for (const auto &filename : filenames) {
    std::ifstream file(filename);
    if (!file.is_open())
      fmt::print("Couldn't open file\n");
    while (true) {
      file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
      if (static_cast<size_t>(file.gcount()) < buffer.size()) {
        buffer.resize(static_cast<size_t>(file.gcount()));
        pipeline.execute(buffer);
        buffer.resize(4096);
        break;
      }
      pipeline.execute(buffer);
      buffer.resize(4096);
    }
  }
}
