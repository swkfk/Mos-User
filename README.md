# MOS User Submodule

> BUAA 操作系统课程设计

该子模块包含下列内容：

- 文件系统服务进程
- 磁盘构建工具（`fsformat`）
- 需要载入文件系统中的用户程序

## 构建

- `make` 将生成内容至 `target/user`
- `MOS_USER=1 make` 将生成内容至 `../target/user`

> 使用 `clean` 目标时同理


## Todo-List

- [x] 把全部内容换成今年的（复制错了，呜呜呜）
- [ ] 载入系统时换个 LOGO
- [ ] 优化输出内容，保持终端清爽
- [ ] 终端增加省略 `.b` 的支持，方便测试