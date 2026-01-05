#include "../exercise.h"
#include <memory>

// READ: `std::shared_ptr` <https://zh.cppreference.com/w/cpp/memory/shared_ptr>
// READ: `std::weak_ptr` <https://zh.cppreference.com/w/cpp/memory/weak_ptr>

// TODO: 将下列 `?` 替换为正确的值
int main(int argc, char **argv) {
    // int(10) 这块数据
    // shared_ptr 引用计数 = 1
    // 当前只有 shared 一个强引用
    auto shared = std::make_shared<int>(10);
    // 4 个 shared_ptr
    std::shared_ptr<int> ptrs[]{shared, shared, shared};

    // weak_ptr 不增加引用计数,只是“旁观者”
    std::weak_ptr<int> observer = shared;
    // ASSERT(observer.use_count() == ?, "");
    // 强引用：shared + ptrs[0..2], weak 不算
    ASSERT(observer.use_count() == 4, "");

    // ptrs[0] 放弃所有权, 引用计数 −1
    ptrs[0].reset();
    // ASSERT(observer.use_count() == ?, "");
    ASSERT(observer.use_count() == 3, "");

    // 和 reset() 等价，又少一个：
    ptrs[1] = nullptr;
    // ASSERT(observer.use_count() == ?, "");
    ASSERT(observer.use_count() == 2, "");

    // ptrs[2] 先释放原来那块共享数据, 然后指向 一块新的 int（值拷贝为 10）
    //原来的那块数据，只剩下 shared 一个引用, 新数据和旧数据 完全无关
    ptrs[2] = std::make_shared<int>(*shared);
    // ASSERT(observer.use_count() == ?, "");
    ASSERT(observer.use_count() == 1, "");

    // +1
    ptrs[0] = shared;
    // +1
    ptrs[1] = shared;
    // 不是增加
    // 是把 shared 里的指针“搬”到 ptrs[2]
    // shared 变成 nullptr
    ptrs[2] = std::move(shared);
    // ASSERT(observer.use_count() == ?, "");
    // 现在持有旧数据的是： ptrs[0] ptrs[1] ptrs[2]
    ASSERT(observer.use_count() == 3, "");

    std::ignore = std::move(ptrs[0]);
    ptrs[1] = std::move(ptrs[1]);
    ptrs[1] = std::move(ptrs[2]);
    // ASSERT(observer.use_count() == ?, "");
    ASSERT(observer.use_count() == 2, "");

    shared = observer.lock();
    // ASSERT(observer.use_count() == ?, "");
    ASSERT(observer.use_count() == 3, "");

    shared = nullptr;
    for (auto &ptr : ptrs) ptr = nullptr;
    // ASSERT(observer.use_count() == ?, "");
    ASSERT(observer.use_count() == 0, "");

    shared = observer.lock();
    // ASSERT(observer.use_count() == ?, "");
    ASSERT(observer.use_count() == 0, "");

    return 0;
}
