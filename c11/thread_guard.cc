//对thread进行封装，避免没有调用join或者detach可导致程序出错的情况出现
//一个是调用join()，保证线程函数的生命周期和线程对象的生命周期相同
//另一个是调用detach()，将线程和线程对象分离，这里需要注意，如果线程已经和对象分离，那我们就再也无法控制线程什么时候结束了，不能再通过join来等待线程执行完
//链接：https://mp.weixin.qq.com/s?__biz=MzkyODE5NjU2Mw==&mid=2247484782&idx=1&sn=e2f36d7896fe168e17957efea5ecdf21&source=41#wechat_redirect


#include <iostream>
#include <thread>

using namespace std;


class ThreadGuard {
public:
	enum class DesAction { join, detach };
	ThreadGuard(std::thread&& t, DesAction a) : t_(std::move(t)), action_(a) {}
	~ThreadGuard() {
		if (t_.joinable()) {
			if (action_ == DesAction::join) {
				t_.join();
			} else {
				t_.detach();
			}
		}
	}

	ThreadGuard(ThreadGuard&&) = default;
	ThreadGuard& operator=(ThreadGuard&&) = default;

	std::thread& get() { return t_; }

private:
	std::thread t_;
	DesAction action_;
};

int main() {
	std::thread t([]() {
		std::cout << "hello world" << std::endl;
	});
	ThreadGuard tg(std::move(t), ThreadGuard::DesAction::join);
	std::this_thread::sleep_for(std::chrono::seconds(5));
	cout << "当前线程ID " << t.get_id() << endl;
	cout << "当前cpu个数 " << std::thread::hardware_concurrency() << endl;
	auto handle = t.native_handle();// handle可用于pthread相关操作
	std::this_thread::sleep_for(std::chrono::seconds(5));
	return 0;
}
