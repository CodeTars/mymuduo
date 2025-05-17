基于C++11实现的主从Reactor异步网络库，通过Epoll LT模式+非阻塞I/O实现事件驱动，采用异步回调链处理事件响应，使用线程池进行任务调度

1.	基于Reactor多线程模型构建事件循环EventLoop，是模型中的事件分发器（Dispatcher），驱动事件循环loop()，调用Poller::poll()获取活跃事件，通过ChannelList分发到对应的事件处理器。
2.	设计了线程池EventLoopThreadPool，实现了对所有线程的统一管理, 确保每个线程有且仅有一个事件循环（one loop per thread）；每个EventLoop运行在独立线程中，主线程处理连接，子线程处理I/O事件，通过eventfd实现线程间唤醒；通过轮询算法为线程内的Loop分配任务，确保负载均衡。
3.	封装epoll相关操作设计了事件监听器EpollPoller，实现对监视fd的增删改，以及对两类事件：新连接（mainReactor管理）、I/O就绪（subReactor管理）的监听。
4.	设计了Channel作为事件处理器（Handler），负责事件注册、回调绑定和回调触发；采用异步回调链将事件响应逐级上抛，实现灵活的事件处理。
5.	设计Acceptor类负责管理fd的listen和accept操作；TcpConnection类负责连接的管理；封装sockaddr_in，提供IP、port等信息的读写接口；封装socket，提供创建非阻塞socket、绑定、监听及接受连接等功能；
6.	实现了分区管理（预留缓冲区+可读缓冲区+可写缓冲区）的用户区Buffer。利用eventfd作为事件通知描述符，方便高效派发事件到其他线程执行异步任务。使用C++11的智能指针、atomic原子类型去除muduo的boost依赖。
