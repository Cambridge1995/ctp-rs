# ctp-rs

## 本项目
- **注意**: 本项目原开发者为 @rn7sr 的ctp-rs,只进行了函数、结构体、常量、结构体字段的命名上的改造。
- **原因**： 个人不喜欢camelCase驼峰命名法，且原ctp有些单词命名错误，编译器提示，本人有强迫症，看不下去，所以全给改成snake_case命名法，并将错误拼写改掉。并未改变整体架构与逻辑。
- 如果喜欢驼峰命名法，且希望项目保持更新频率，请前往原项目。


## 原 ctp-rs 地址

[![GitHub](https://img.shields.io/badge/GitHub-Rn7s2/ctp--rs-green&logo=github)](https://github.com/rn7s2/ctp-rs)
[![Rust](https://github.com/rn7s2/ctp-rs/actions/workflows/rust.yml/badge.svg?branch=master)](https://github.com/rn7s2/ctp-rs/actions/workflows/rust.yml)
[![Crate](https://img.shields.io/crates/v/ctp-rs.svg)](https://crates.io/crates/ctp-rs)
[![API](https://docs.rs/ctp-rs/badge.svg)](https://docs.rs/ctp-rs)

CTP 期货接口的安全、符合 Rust 惯用法的绑定。支持 Windows、Linux 与 macOS 三平台，并内置 LocalCTP 本地模拟交易功能，无需连接真实服务器即可开发和测试交易逻辑。

Safe & idiomatic Rust bindings for CTP — cross-platform (Windows / Linux / macOS) with built-in LocalCTP for offline simulation.

### 快速开始

可直接从 [`crates.io`](https://crates.io/crates/ctp-rs) 安装：

```sh
cargo add ctp-rs
```

如需本地模拟交易（LocalCTP），启用对应 feature：

```sh
cargo add ctp-rs --features localctp
```

发布在 `crates.io` 的包采用 SemVer 版本号系统。\
底层绑定的 CTP C++ 版本号被包含在 build meta 中，如 `0.2.1+ctp.6.7.11.darwin.6.7.7`：Windows/Linux 跟随上游，macOS 使用上游目前最新的 darwin SDK。

## 特性

- **跨平台**：支持 Windows、Linux（x86_64）与 macOS（Apple Silicon / x86_64）
- **安全 API**：所有接口均采用 Rust 字符串与所有权语义，字符串编码自动转换，消除 C++ 裸指针风险
- **内置 LocalCTP**：无需连接真实交易服务器，本地离线即可模拟下单、持仓、结算全流程
- **基于 channel 的消息传递**：SPI 回调通过标准库 `mpsc::channel` 传递，可与异步运行时或多线程程序无缝集成


## 本项目示例

### 1. 行情接口（MdApi）

```rs
use ctp_rs::{MdApi, MdMsg, ReqUserLogin};
use std::sync::{Arc, mpsc::channel};

// 模拟环境地址列表见：http://www.openctp.cn/simenv.html
const FRONT_ADDR: &str = "tcp://tcp://182.254.243.31:40011";
// 缓存文件夹(必须)
const FLOW_PATH: &str = "flow/md/";
// 需要订阅的合约代码
const INSTRUMENTS: &[&str] = &["...", "..."];

fn main() {
    let (tx, rx) = channel();
    let api = Arc::new(MdApi::create_md_api_and_spi(
        tx,
        FLOW_PATH.to_string(),
        false,
        false,
        true,
    ));
    api.register_front(FRONT_ADDR.to_string());
    api.init();

    loop {
        let msg = rx.recv().unwrap();
        match msg {
            MdMsg::on_front_connected => {
                println!("front connected");
                let req = ReqUserLogin {
                    broker_id: "".to_string(),
                    user_id: "".to_string(),
                    password: "".to_string(),
                    ..Default::default()
                };
                api.req_user_login(req, 0);
            }
            MdMsg::on_rsp_user_login(_, rsp_info, _, _) => {
                if rsp_info.error_id != 0 {
                    println!("user login failed: {:?}", rsp_info);
                    continue;
                } else {
                    println!("user login success: {:?}", rsp_info);
                    let instruments: Vec<String> =
                        INSTRUMENTS.iter().map(|&s| s.to_string()).collect();
                    api.subscribe_market_data(instruments);
                }
            }
            MdMsg::on_rtn_depth_market_data(tick) => {
                println!("{:?}", tick);
            }
            _ => {}
        }
    }
}
```

### 2. 交易接口（TraderApi）

```rs
use ctp_rs::{
    QryInstrument, ReqAuthenticate, ReqUserLogin, ResumeType, TraderApi,
    TraderMsg,
};
use std::sync::{Arc, mpsc::channel};

const BROKER_ID: &str = "...";
const USER_ID: &str = "...";
const PASSWORD: &str = "...";
const APP_ID: &str = "simnow_client_test";
const AUTH_CODE: &str = "0000000000000000";
const FRONT_ADDR: &str = "tcp://182.254.243.31:40001";
const FLOW_PATH: &str = "flow/trader/";

fn main() {
    let (tx, rx) = channel();
    let api = Arc::new(TraderApi::create_trader_api_and_spi(
        tx,
        FLOW_PATH.to_string(),
        true,
    ));
    api.register_front(FRONT_ADDR.to_string());
    api.subscribe_public_topic(ResumeType::Quick as i32);
    api.subscribe_private_topic(ResumeType::Restart as i32);
    api.init();

    loop {
        let msg = rx.recv().unwrap();
        match msg {
            TraderMsg::on_front_connected => {
                println!("front connected");
                let req = ReqAuthenticate {
                    broker_id: BROKER_ID.to_string(),
                    user_id: USER_ID.to_string(),
                    auth_code: AUTH_CODE.to_string(),
                    app_id: APP_ID.to_string(),
                    ..Default::default()
                };
                api.req_authenticate(req, 0);
            }
            TraderMsg::on_rsp_authenticate(_, rsp_info, _, _) => {
                if rsp_info.error_id != 0 {
                    println!("auth failed: {:?}", rsp_info);
                    std::process::exit(1);
                } else {
                    println!("auth success: {:?}", rsp_info);

                    let req = ReqUserLogin {
                        broker_id: BROKER_ID.to_string(),
                        user_id: USER_ID.to_string(),
                        password: PASSWORD.to_string(),
                        ..Default::default()
                    };
                    api.req_user_login(req, 0);
                }
            }
            TraderMsg::on_rsp_user_login(_, rsp_info, _, _) => {
                if rsp_info.error_id != 0 {
                    println!("user login failed: {:?}", rsp_info);
                    std::process::exit(1);
                } else {
                    println!("user login success: {:?}", rsp_info);

                    api.req_qry_instrument(QryInstrument::default(), 0);
                }
            }
            TraderMsg::on_rsp_qry_instrument(instrument, rsp_info, _, _) => {
                if instrument.is_null {
                    eprintln!("qry instrument: {:?}", rsp_info);
                    std::process::exit(1);
                }

                println!("{:?}", instrument);
            }
            _ => {}
        }
    }
}

```

## LocalCTP（本地模拟交易）

ctp-rs 内置了 [LocalCTP](https://github.com/dearleeyoung/LocalCTP)（MIT 协议），提供完整的 TraderApi 本地模拟实现。LocalCTP 以 SQLite 存储委托、持仓与资金状态，支持从 CSV 文件加载合约定义，可在无网络环境下完成下单、撤单、结算等全流程测试。

### 启用方式

```toml
# Cargo.toml
[dependencies]
ctp-rs = { version = "...", features = ["localctp"] }
```

### 平台行为

- **默认（不启用任何 feature）**：三平台均使用预编译的真实 CTP 动态库。Windows/Linux 是 `.dll`/`.so`，macOS 是嵌入到二进制中的 framework dylib，详见下方 [macOS framework 处理](#macos-framework-处理)。
- **启用 `localctp`**：TraderApi 切换为 LocalCTP，MdApi 仍使用真实 CTP。Windows/Linux 上 LocalCTP 编译为动态库并与可执行文件并排部署；macOS 上 LocalCTP 编译为静态库直接链入二进制。

当前内置 LocalCTP 版本见 [localctp/VERSION.md](localctp/VERSION.md)。

## 注意事项

### CTP 回报中的空指针

- CTP 返回空指针时，对应结构体的 `is_null` 字段会为 `true`。

### 字符串编码

- 大部分接口实现了字符串自动编码转换，可在 Rust 中直接使用 `String`。\
  少部分字段（如结算单）因 CTP 截断汉字导致编码转换失败，这些字段保留为 `Vec<u8>`。

  如需打印含中文的 `Vec<u8>` 字段，可使用 [`encoding_rs`](https://crates.io/crates/encoding_rs)：

  ```rs
  use encoding_rs::GBK;
  let contents = GBK.decode(&bytes).0.to_string();
  ```

### 构建要求

CTP 原生库（headers + macOS framework + Windows `.dll`/`.lib` + Linux `.so`）超过了 `crates.io` 的压缩包体积限制，因此**不随 crate 发布**。`build.rs` 在第一次构建时会从 Cloudflare R2 下载并解压到 `$OUT_DIR/lib/`，同一 `OUT_DIR` 内的后续构建直接复用（`$OUT_DIR` 是按 target/profile/构建脚本哈希区分的，如需强制重新下载，`cargo clean` 即可）。

下载源（约 12 MB）：

- <https://ctp-api.ruiqilei.com/ctp.6.7.11.darwin.6.7.7.zip>

为避免引入额外的 Rust HTTP/zip 构建依赖（多 MB 编译开销，且无法保留 macOS framework 内部的符号链接），`build.rs` 直接调用系统工具完成下载与解压。**首次构建机器需具备**：

| 平台    | 必需                  | 备注                                                                                                |
| ------- | --------------------- | --------------------------------------------------------------------------------------------------- |
| macOS   | `curl`、`unzip`       | 系统自带，无需额外安装                                                                              |
| Linux   | `curl`、`unzip`       | 大部分发行版自带；最小镜像（如 `ubuntu:*-slim`、`debian:*-slim`）需 `apt-get install -y curl unzip` |
| Windows | `curl.exe`、`tar.exe` | Windows 10 1803+（2018 年 4 月）/ Windows Server 2019+ 自带                                         |

构建机器必须可访问 `ctp-api.ruiqilei.com`。

### macOS framework 处理

macOS 平台的真实 CTP 由 `$OUT_DIR/lib/darwin/` 下的 `thostmduserapi_se.framework` 与 `thosttraderapi_se.framework` 提供（darwin 6.7.7，由 `build.rs` 解压到 `OUT_DIR`）。这两个 framework 的处理方式与 Windows/Linux 上的 `.dll`/`.so` 不同：

- **构建期嵌入**：`build.rs` 把两个 framework 内部的 dylib 字节通过 `.incbin` 拼进 `__DATA,__const` 段，最终二进制是自包含的——不需要把任何 dylib 与可执行文件一起分发。
- **运行期 `dlopen`**：第一次调用 MdApi/TraderApi 时，会把嵌入的字节写到 `$TMPDIR/ctp-rs.<pid>.<name>.dylib`，再用 `RTLD_NOW | RTLD_LOCAL` 打开。`CreateFtdcMdApi` / `CreateFtdcTraderApi` / `GetApiVersion` 通过 `dlsym` 拿到，其余方法均为虚函数，走 framework 自带的 vtable。
- **临时再签名**：Gatekeeper 不允许从任意路径 `dlopen`。`build.rs` 会在构建时调用 `codesign --remove-signature` 后再 `codesign --sign -` 做 ad-hoc 签名，最终用户机器上无需再做任何签名操作。
- **darwin SDK 与 Linux/Windows 不一致**：上游目前 macOS 仅发布到 6.7.7，Linux/Windows 已是 6.7.11。两者的 wire protocol 兼容，但极个别字段或常量可能不同；行为差异请参考 zip 包内 `darwin/thostmduserapi_se.framework/Versions/A/Headers/` 下的实际头文件（解压后位于 `$OUT_DIR/lib/darwin/...`）。
- **CreateFtdcXxxApi 签名差异**：darwin 的 `CreateFtdcMdApi` / `CreateFtdcTraderApi` 比 Linux 头文件少一个 `bIsProductionMode` 参数，wrapper 内部已自动桥接，对 Rust 侧完全透明。

### 对上游 LocalCTP 的修改

详见 [localctp/VERSION.md](localctp/VERSION.md)。简要列举：

- **macOS 支持**：`LeeDateTime.h` / `LeeDateTime.cpp` 中所有 `#if defined(__linux__)` 分支扩展为 `__linux__ || __APPLE__`，让 POSIX 时间 API 路径在 macOS 上同样生效
- **UTC+8 钉住**：`LeeDateTime` 不再读取操作系统时区，始终按 Asia/Shanghai 计算交易日和结算时间，避免在非北京时区主机上出现日期错位
- **启动期同步**：`CSettlementHandler` 新增 `WaitUntilReady()`，让用户线程阻塞到 `m_timerThread` 完成首轮 `checkSettlement` 后再下发 SPI 回调，避免启动窗口内并发触碰同一批静态数据结构
- **移除 sqlite/shell.c**：上游附带的 SQLite CLI shell 在 LocalCTP 作为库构建时不需要
