OmniRTOS/
├── CMakeLists.txt              <-- [构建] 顶级构建脚本
├── app/                        <-- [应用] 用户业务逻辑
│   └── main.c                  <-- [入口] 用户主函数 (main_app)
├── kernel/                     <-- [内核] 纯软件逻辑 (核心)
│   ├── include/
│   │   ├── os_config.h         <-- [配置] 系统参数 (优先级数、栈大小)
│   │   ├── os_errno.h          <-- [定义] 统一错误码
│   │   ├── os_list.h           <-- [结构] 侵入式双向链表
│   │   ├── os_object.h         <-- [对象] 内核对象基类 (OOP)
│   │   ├── os_task.h           <-- [对象] 任务控制块 (TCB)
│   │   └── os_types.h          <-- [基础] 类型定义 & container_of
│   └── src/
│       ├── os_core.c           <-- [核心] 调度触发器 (os_schedule)
│       ├── os_idle.c           <-- [任务] 空闲任务实现
│       ├── os_object.c         <-- [对象] 对象初始化实现
│       ├── os_sched.c          <-- [核心] O(1) 调度算法 (Bitmap)
│       ├── os_startup.c        <-- [启动] 内核启动流程
│       └── os_task.c           <-- [任务] 栈初始化 & TCB管理
├── arch/                       <-- [架构] 硬件相关 (汇编)
│   └── arm/
│       └── cortex-m4/
│           └── os_context_gcc.S <-- [汇编] PendSV 上下文切换
├── board/                      <-- [板级] (后续存放 Linker 脚本)
└── drivers/                    <-- [驱动] (后续存放设备框架)
