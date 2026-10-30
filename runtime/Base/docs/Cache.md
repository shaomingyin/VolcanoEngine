# Cache

`Volcano::Cache<T, Identifier>` 是 Base 的缓存骨架。`Cache<T, Identifier>::Policy` 定义策略接口；默认策略类与骨架统一定义在 `Cache.h`，但不嵌套在 Cache 中。不依赖 Resources、PhysFS、World 或 Graphics，不使用 RTTI。

```cpp
#include <Volcano/Cache.h>

Volcano::Cache<Image> cache(std::make_unique<Volcano::LRUCachePolicy<Image>>(64));
auto image = cache.load("image", [] { return std::make_shared<Image>(); });
cache.gc();
```

## 接口与所有权

- `load(identifier, loader)`：先复用仍存活的对象，未命中时在锁外调用 loader，再发布结果。成功访问后评估策略并执行淘汰。loader 返回非空 shared_ptr，异常直接传播；返回空指针抛出 runtime_error。
- `gc()`：重新评估策略，释放选中条目的缓存强引用，清理已经失效的弱引用索引。无需发生新加载即可执行，例如 Player 移动后由调用方触发。
- `clear()`：释放全部缓存强引用并清空索引及策略状态，调用方已有句柄继续有效。

Cache 持有资源强引用及弱引用索引，策略只保存决策所需的元数据，不持有资源、不加载资源、不直接释放资源。淘汰后仍存活的对象保留弱引用索引，下次访问复用原对象并重新参与保留决策。回收只释放缓存的所有权，无法强制销毁外部仍在使用的资源。刚淘汰的资源在解锁后才析构，其失效弱引用可在后续 gc 或访问中清理。

## 策略

构造时移交非空 unique_ptr<Policy>，Cache 独占策略实例。自定义策略实现：

- `access(identifier, const T&)`：记录成功访问，资源引用只在调用期间有效。
- `selectEvictions(const Entries&)`：接收当前缓存强引用条目的只读快照（标识到 const T*），返回待淘汰标识列表，不修改策略元数据。快照及资源指针只在调用期间有效。
- `remove(identifier) noexcept`：Cache 执行淘汰时删除对应策略元数据。
- `clear() noexcept`：清空策略元数据。

| 默认策略 | 保留规则 |
| --- | --- |
| `WeakCachePolicy<T, Identifier>` | 选择全部条目淘汰，不额外持有资源，最后一个外部句柄释放后立即销毁 |
| `LRUCachePolicy<T, Identifier>` | 最多保留 N 项，每次访问更新顺序，淘汰最近最少使用的条目 |
| `FIFOCachePolicy<T, Identifier>` | 最多保留 N 项，命中不改变顺序，淘汰最早进入保留队列的条目 |

容量限制的是缓存强引用条目数，不是活跃对象总数或字节预算；零容量表示不额外保留。默认策略仅存储标识和顺序。自定义距离策略可以从调用方获得 Player 位置，结合资源信息选择淘汰对象；策略数据的并发修改由调用方同步。主动判断并加载附近资源属于 Framework 或调用方职责。

## 并发与异常

Cache 互斥保护索引及策略调用，loader 在锁外执行，可以加载依赖资源。同一标识并发未命中允许重复构建，最终复用先发布且仍存活的对象。clear 不等待或取消已启动的 loader，正在加载的对象可能在 clear 后发布。

策略在缓存锁内执行，不得重入同一 Cache。access 抛出异常时必须保持策略状态不变，Cache 撤销本次新索引。selectEvictions 及快照分配失败时不执行淘汰，已经成功记录的访问和发布的资源保留，可重试 gc。remove、clear 和析构不得抛出异常。每次评估会构造只读快照，开销随保留条目数增长。

淘汰和 clear 在锁内移交资源所有权，资源在解锁后析构。Cache 不创建线程或调度析构；GPU 资源的最后一次释放须由调用方保证在正确线程且所属 GL context 有效。Cache 析构期间不得重入 Cache。

Resources 各类型继续明确选择 WeakCachePolicy，生命周期不变。Mesh/Texture 的 GL context 分区仍由 Resources 负责。
