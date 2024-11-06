#include <iostream>
#include <chrono>
#include <random>
#include <vector>

// 用于打印一个作用域的执行耗时
class ClockGuard
{
public:
    explicit ClockGuard(std::string inDest) : m_desc(std::move(inDest))
    {
        m_startTime = std::chrono::steady_clock::now();
    }

    ~ClockGuard()
    {
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        double durationNs = std::chrono::duration<double, std::nano>(currentTime - m_startTime).count();
        std::cout << m_desc << " Cost: " << durationNs << " Ns" << std::endl;
    }

private:
    std::string m_desc;
    std::chrono::steady_clock::time_point m_startTime;
};

static constexpr size_t BiggerThanCachesize = 10 * 1024 * 1024;
static auto g_ptr = new long[BiggerThanCachesize];

std::random_device g_rd;
std::mt19937 g_gen(g_rd());
std::uniform_int_distribution<long> g_urd;

// 尝试清空缓存
void ClearCPUCache()
{
    for (int i = 0; i < BiggerThanCachesize; ++i)
        g_ptr[i] = g_urd(g_gen);
}

int main()
{
    // dumpbin /headers Memory.exe | Select-String "stack"
    // 查看size of stack reserve和size of stack commit
    // 栈默认预留大小为1M
    // char arr[10000000];  // stack overflow

    std::vector<int*> continuous(1000);
    std::vector<int*> dispersive(1000);

    for (int*& item: continuous)
        item = new int;

    constexpr int spacing = 10000;      // 内存间隔，调整该数以查看cache的影响
    for (int*& item: dispersive)
        item = new int[spacing];

    // 访问连续内存
    ClearCPUCache();
    {
        ClockGuard guard("continuous");
        for (int*& item: continuous)
            *item = 1;
    }
    // 访问离散内存
    ClearCPUCache();
    {
        ClockGuard guard("dispersive");
        for (int*& item: dispersive)
            *item = 1;
    }

    std::vector<int> src(10000);
    std::vector<int> dst(10000);

    // 直接复制
    ClearCPUCache();
    {
        ClockGuard guard("Copy");
        for (int i = 0; i < dst.size(); ++i)
            dst[i] = src[i];
    }
    // 使用memcpy复制
    ClearCPUCache();
    {
        ClockGuard guard("Map");
        memcpy(dst.data(), src.data(), sizeof(int) * dst.size());
    }

    return 0;
}
