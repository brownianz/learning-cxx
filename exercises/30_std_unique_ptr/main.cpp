#include "../exercise.h"
#include <cstring>
#include <memory>
#include <string>
#include <vector>

// READ: `std::unique_ptr` <https://zh.cppreference.com/w/cpp/memory/unique_ptr>

std::vector<std::string> RECORDS;

class Resource {
    std::string _records;

public:
    void record(char record) {
        _records.push_back(record);
    }

    ~Resource() {
        RECORDS.push_back(_records);
    }
};

using Unique = std::unique_ptr<Resource>;
Unique reset(Unique ptr) {
    if (ptr) ptr->record('r');
    return std::make_unique<Resource>();
}
Unique drop(Unique ptr) {
    if (ptr) ptr->record('d');
    return nullptr;
}
Unique forward(Unique ptr) {
    if (ptr) ptr->record('f');
    return ptr;
}

int main(int argc, char **argv) {
    std::vector<std::string> problems[3];

    drop(forward(reset(nullptr)));
    problems[0] = std::move(RECORDS);

    // reset(nullptr) 创建资源 A
    // 两次 forward：A 记录 "ff"
    // reset(A)：A 再记录 'r' 变 "ffr"，同时创建新资源 B
    // drop(B)：B 记录 'd'，然后被丢弃
    // 最终会析构两个资源：
      // A 的记录是 "ffr"
      // B 的记录是 "d"
    forward(drop(reset(forward(forward(reset(nullptr))))));
    problems[1] = std::move(RECORDS);

    // reset(nullptr) 创建 A
    // reset(A)：A 记 'r'，并创建 B；A 最终析构记录 "r"
    // 然后外层继续处理 B：
      // drop(B)：B 记 'd'，最终析构记录 "d"
    // 再往外：
      // reset(nullptr) 创建 C
      // drop(C)：C 记 'd'，最终析构记录 "d"
    // 所以最终资源记录集合是：A="r", B="d", C="d"。
    // NOTE:
    drop(drop(reset(drop(reset(reset(nullptr))))));
    problems[2] = std::move(RECORDS);

    // ---- 不要修改以上代码 ----
    // // ===== 临时调试打印 =====
    // for (int i = 0; i < 3; ++i) {
    //     std::cout << "problems[" << i << "]:" << std::endl;
    //     for (auto const &s : problems[i]) {
    //         std::cout << "  \"" << s << "\"" << std::endl;
    //     }
    // }
    std::vector<const char *> answers[]{
        {"fd"},
        // TODO: 分析 problems[1] 中资源的生命周期，将记录填入 `std::vector`
        // NOTICE: 此题结果依赖对象析构逻辑，平台相关，提交时以 CI 实际运行平台为准
        // {"", "", "", "", "", "", "", ""},
        // {"", "", "", "", "", "", "", ""},
        {"d", "ffr"},
        {"d", "d", "r"},
    };

    // ---- 不要修改以下代码 ----

    for (auto i = 0; i < 3; ++i) {
        ASSERT(problems[i].size() == answers[i].size(), "wrong size");
        for (auto j = 0; j < problems[i].size(); ++j) {
            ASSERT(std::strcmp(problems[i][j].c_str(), answers[i][j]) == 0, "wrong location");
        }
    }

    return 0;
}
