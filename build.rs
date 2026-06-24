use std::{
    fs,
    path::{Path, PathBuf},
    process::Command,
};

// CTP 原生库（头文件 + framework dylib + Windows .dll/.lib + Linux .so）
// 体积超过 crates.io 压缩包限制，因此托管在 R2 上，首次构建时下载，
// 而非随 crate 一起发布。zip 解压到 `$OUT_DIR/lib/`（build.rs 不得写入
// 源码树——`cargo publish` 会拒绝）。zip 顶层结构与 build.rs 预期一致：
// darwin/*.framework、lib*.so、*.dll/.lib、headers、error.xml。
const LIB_ASSET_URL: &str = "https://ctp-api.ruiqilei.com/ctp.6.7.11.darwin.6.7.7.zip";
const LIB_ASSET_VERSION: &str = "ctp.6.7.11.darwin.6.7.7";

fn main() {
    // docs.rs 在无网络访问的沙箱中构建，`ensure_lib_dir` 中的原生库下载
    // 会失败。rustdoc 仅对 crate 做类型检查（cxx::bridge 在 Rust 侧展开；
    // 没有链接步骤），因此可以安全跳过下载 + C++ 编译 + 库拷贝。
    if std::env::var_os("DOCS_RS").is_some() {
        return;
    }

    let root = env!("CARGO_MANIFEST_DIR");
    let out_dir = PathBuf::from(std::env::var("OUT_DIR").unwrap());
    // 原生库存放在 OUT_DIR（而非源码树），这样 `cargo publish` 验证
    // 不会因为构建脚本修改源码而拒绝。
    let lib_dir = out_dir.join("lib");

    ensure_lib_dir(&lib_dir);

    let use_localctp = std::env::var("CARGO_FEATURE_LOCALCTP").is_ok();

    // 当交易端库为嵌入式 framework dylib（即默认的非 localctp 构建）时，
    // 此值为 true。cxx 封装层检查 CTP_RS_DARWIN_TRADER_DLOPEN，
    // 将 CreateFtdcTraderApi / GetApiVersion 路由到运行时 shim，
    // 而非调用静态类方法，因为动态库加载时这些符号无法链接解析。
    let darwin_trader_dlopen = cfg!(target_os = "macos") && !use_localctp;

    // OUT_DIR 在 lib_dir 之前搜索，这样 LocalCTP 编译的
    // libthosttraderapi_se.{so,a,lib} 在 OUT_DIR 中优先于 lib_dir 中
    // 预编译的 SimNow 库。否则，在 Linux 上使用 --features localctp 时，
    // 链接器会将 -lthosttraderapi_se 解析为预编译的 .so（该 .so
    // 不导出 localctp_wait_until_ready），导致封装层链接失败。
    println!("cargo:rustc-link-search={}", out_dir.display());
    println!("cargo:rustc-link-search={}", lib_dir.display());
    if cfg!(target_os = "macos") {
        // mdapi：始终从 lib/darwin/thostmduserapi_se.framework 嵌入，
        // 并在运行时 dlopen()。无需链接。
        // traderapi：localctp 的静态库（use_localctp=true）或嵌入式
        // framework dylib（use_localctp=false），在下方配置。
        if use_localctp {
            println!("cargo:rustc-link-lib=static=thosttraderapi_se");
        }
        // Converter.cpp 在 Unix 上使用 iconv；macOS 单独提供 libiconv。
        println!("cargo:rustc-link-lib=iconv");
        // 由 framework dylib 在运行时加载使用。
        println!("cargo:rustc-link-lib=dylib=c++");
    } else {
        println!("cargo:rustc-link-lib=thostmduserapi_se");
        println!("cargo:rustc-link-lib=thosttraderapi_se");
    }

    let mut cpp_files: Vec<String> = vec![
        "wrapper/src/MdApi.cpp".into(),
        "wrapper/src/TraderApi.cpp".into(),
        "wrapper/src/CMdSpi.cpp".into(),
        "wrapper/src/CTraderSpi.cpp".into(),
        "wrapper/src/Converter.cpp".into(),
    ];
    if cfg!(target_os = "macos") {
        // DarwinDylibLoader 辅助类由 md 和 trader shim 共用。
        cpp_files.push("wrapper/src/DarwinDylibLoader.cpp".into());
        // mdapi shim：桥接 linux 头文件的 4 参数 CreateFtdcMdApi 到
        // darwin dylib 的 3 参数版本，并 dlopen() 嵌入式 blob。
        cpp_files.push("wrapper/src/MdApiDarwinShim.cpp".into());

        let md_blob_cpp =
            embed_macos_framework_dylib(&lib_dir, &out_dir, "thostmduserapi_se", "md");
        cpp_files.push(md_blob_cpp);

        if darwin_trader_dlopen {
            cpp_files.push("wrapper/src/TraderApiDarwinShim.cpp".into());
            let trader_blob_cpp =
                embed_macos_framework_dylib(&lib_dir, &out_dir, "thosttraderapi_se", "trader");
            cpp_files.push(trader_blob_cpp);
        }
    }

    let rust_files = vec!["src/lib.rs"];
    let wrapper_files = vec![
        "wrapper/include/Converter.h",
        "wrapper/include/CMdSpi.h",
        "wrapper/include/CTraderSpi.h",
        "wrapper/include/DarwinDylibLoader.h",
        "wrapper/include/MdApi.h",
        "wrapper/include/TraderApi.h",
        "wrapper/src/Converter.cpp",
        "wrapper/src/CMdSpi.cpp",
        "wrapper/src/CTraderSpi.cpp",
        "wrapper/src/DarwinDylibLoader.cpp",
        "wrapper/src/MdApi.cpp",
        "wrapper/src/MdApiDarwinShim.cpp",
        "wrapper/src/TraderApi.cpp",
        "wrapper/src/TraderApiDarwinShim.cpp",
    ];

    let mut build = cxx_build::bridges(rust_files);
    // CTP 头文件位于 OUT_DIR 下的 `lib_dir` 中（随预编译库一起）。
    // 封装头文件以 `<ThostFtdc*.h>` 形式引用它们。
    build.include(&lib_dir);
    build
        .define("CXX_RS", None)
        .flag_if_supported("/EHsc")
        .flag_if_supported("/std:c++20")
        .flag_if_supported("/utf-8")
        .flag_if_supported("/w")
        .flag_if_supported("-std=c++20")
        .flag_if_supported("-w");
    if darwin_trader_dlopen {
        build.define("CTP_RS_DARWIN_TRADER_DLOPEN", None);
    }
    if use_localctp {
        // 切换 wrapper/src/TraderApi.cpp 中对 localctp_wait_until_ready()
        // 的调用，该调用会阻塞 CreateTraderApi() 直到 LocalCTP
        // 首轮结算初始化完成（避免与 SPI 回调路径的启动竞争）。
        build.define("CTP_RS_LOCALCTP", None);
    }
    build.files(cpp_files).compile("ctp_rs");

    println!("cargo:rerun-if-changed=src/lib.rs");
    for file in wrapper_files.iter() {
        println!("cargo:rerun-if-changed={}", file);
    }

    // 按 profile 区分的 target 目录（如 target/debug/）——
    // 可执行文件所在目录，也是 Windows/Linux 动态库加载器
    // 查找同级 .dll/.so 的位置。macOS 无需运行时拷贝：
    // md 和 trader 均为嵌入式（默认）或静态链接（localctp），
    // 二进制完全自包含。
    let target_dir = {
        let mut path = PathBuf::from(std::env::var("OUT_DIR").unwrap());
        _ = path.pop() && path.pop() && path.pop();
        path
    };

    // 行情库：win/linux 上将动态库拷贝到二进制同级目录。
    // macOS 将 framework dylib 嵌入二进制本身。
    if cfg!(target_os = "windows") {
        let md = "thostmduserapi_se.dll";
        fs::copy(lib_dir.join(md), target_dir.join(md))
            .unwrap_or_else(|e| panic!("Copy {} failed: {}", md, e));
    } else if cfg!(target_os = "linux") {
        let md = "libthostmduserapi_se.so";
        fs::copy(lib_dir.join(md), target_dir.join(md))
            .unwrap_or_else(|e| panic!("Copy {} failed: {}", md, e));
    }

    // 交易库：
    //   * win/linux 非 localctp：从 lib/ 拷贝预编译的动态库。
    //   * win/linux 使用 localctp：编译后拷贝结果到二进制同级。
    //   * macOS 使用 localctp：localctp 编译为静态库，
    //     链接器通过 cargo:rustc-link-lib=static= 拉入。无需运行时拷贝。
    //   * macOS 非 localctp：trader framework dylib 已嵌入，无需拷贝。
    if use_localctp {
        println!("cargo:rerun-if-changed=localctp");
        let built = build_localctp(&Path::new(root).join("localctp"), &lib_dir);
        if !cfg!(target_os = "macos") {
            let trader = if cfg!(target_os = "windows") {
                "thosttraderapi_se.dll"
            } else {
                "libthosttraderapi_se.so"
            };
            fs::copy(&built, target_dir.join(trader))
                .unwrap_or_else(|e| panic!("Copy {} failed: {}", trader, e));
        }
    } else if !cfg!(target_os = "macos") {
        let trader = if cfg!(target_os = "windows") {
            "thosttraderapi_se.dll"
        } else {
            "libthosttraderapi_se.so"
        };
        fs::copy(lib_dir.join(trader), target_dir.join(trader))
            .unwrap_or_else(|e| panic!("Copy {} failed: {}", trader, e));
    }
}

/// 如果 CTP 原生库包尚不存在，则下载并解压到 `lib_dir`
///（位于 `$OUT_DIR` 下）。包托管在 R2 上（见 `LIB_ASSET_URL`），
/// 解压后结构与 build.rs 其余部分预期一致：`darwin/*.framework`、
/// `lib*.so`、`*.dll`/`*.lib`、顶层 CTP 头文件以及 `error.xml`。
///
/// `OUT_DIR` 按构建（target/profile/build-script hash）区分，
/// 任何对 `LIB_ASSET_URL` 的修改都会重新哈希 build.rs 并得到
/// 新的 `OUT_DIR`。因此如果 `lib_dir` 已存在，必然是本构建所需
/// 的包——无需版本检查。解压先写入同级 `.partial` 目录，
/// 完成后原子重命名，确保不完整的解压永远不会被误认为完整的。
///
/// 使用 `curl` + `unzip`（Unix）/ `tar`（Windows），而非 Rust HTTP/zip
///  crate：后者会增加数 MB 的构建依赖，且无法保留 framework 内部
/// 的符号链接布局（codesign/dlopen 依赖这些符号链接）。系统工具
/// 在支持的平台上普遍可用——详见 README "构建要求"。
fn ensure_lib_dir(lib_dir: &Path) {
    if lib_dir.exists() {
        return;
    }

    let staging = lib_dir.with_file_name("lib.partial");
    if staging.exists() {
        fs::remove_dir_all(&staging)
            .unwrap_or_else(|e| panic!("failed to clear {}: {}", staging.display(), e));
    }
    fs::create_dir_all(&staging)
        .unwrap_or_else(|e| panic!("failed to create {}: {}", staging.display(), e));

    let zip_path = staging.join(".ctp-rs-asset.zip");
    // 不用 `cargo:warning=` —— cargo 会将其缓存在构建脚本的 `output`
    // 文件里，并在该包后续每次构建时重放，即使 build.rs
    // 没有重新运行也是如此。这样一次性的下载就会产生永久警告。
    // `eprintln!` 输出到捕获的 stderr（`cargo build -vv` 或失败时可见），
    // 不会被重放。
    eprintln!(
        "ctp-rs: downloading native libraries ({}) — this happens once per OUT_DIR",
        LIB_ASSET_VERSION
    );
    let status = Command::new("curl")
        .arg("-fsSL")
        .arg("--retry")
        .arg("3")
        .arg("-o")
        .arg(&zip_path)
        .arg(LIB_ASSET_URL)
        .status()
        .expect("`curl` not found — required to fetch CTP native libraries (see README)");
    assert!(
        status.success(),
        "failed to download CTP native libraries from {}",
        LIB_ASSET_URL
    );

    // 解压：Unix 上优先使用 `unzip`（macOS 自带，所有主流
    // Linux 发行版基础仓库中均有）。Windows 上回退到
    // `tar -xf`（Windows 10 1803+ 自带，通过 libarchive 处理 zip）。
    // 两者均保留 framework 内部符号链接，运行时 codesign 依赖它们。
    let extract_status = if cfg!(target_os = "windows") {
        Command::new("tar")
            .arg("-xf")
            .arg(&zip_path)
            .arg("-C")
            .arg(&staging)
            .status()
            .expect("`tar` not found — required to extract on Windows (see README)")
    } else {
        Command::new("unzip")
            .arg("-q")
            .arg("-o")
            .arg(&zip_path)
            .arg("-d")
            .arg(&staging)
            .status()
            .expect("`unzip` not found — required to extract CTP libraries (see README)")
    };
    assert!(
        extract_status.success(),
        "failed to extract CTP native libraries into {}",
        staging.display()
    );

    // __MACOSX/ 是 macOS Finder 压缩时注入的 AppleDouble
    // 资源分支数据；在所有平台上都是噪音，只会让目录杂乱。
    let macosx_junk = staging.join("__MACOSX");
    if macosx_junk.exists() {
        let _ = fs::remove_dir_all(&macosx_junk);
    }
    let _ = fs::remove_file(&zip_path);

    // 原子发布：lib_dir 仅在解压成功后才存在。
    fs::rename(&staging, lib_dir).unwrap_or_else(|e| {
        panic!(
            "failed to rename {} → {}: {}",
            staging.display(),
            lib_dir.display(),
            e
        )
    });
}

/// 重签名并嵌入 CTP framework dylib，使其可在运行时从 /tmp
/// `dlopen`。返回生成的 .cpp 文件路径（位于 OUT_DIR 下），
/// 该文件使用 `.incbin` 将重签名后的字节拼接到 __DATA,__const 段，
/// 并以 `_ctp_rs_<sym_prefix>_dylib_{start,end}` 标记边界。
///
/// 预编译的 framework 使用 "Apple Development" 证书签名，
/// Gatekeeper 会拒绝从任意路径 `dlopen`。我们制作一份
/// OUT_DIR 私有副本，剥离现有签名，再做 ad-hoc 重签名；
/// 重签名后的字节最终嵌入二进制，因此终端用户无需
/// 自行运行 `codesign`。
fn embed_macos_framework_dylib(
    lib_dir: &Path,
    out_dir: &Path,
    framework_name: &str, // 如 "thostmduserapi_se"——既是 framework 目录名也是二进制名
    sym_prefix: &str,     // 如 "md" → _ctp_rs_md_dylib_{start,end}
) -> String {
    let src_dylib = lib_dir
        .join("darwin")
        .join(format!("{}.framework", framework_name))
        .join("Versions")
        .join("A")
        .join(framework_name);
    assert!(
        src_dylib.exists(),
        "missing macOS framework dylib at {} — the auto-downloaded CTP bundle \
         appears incomplete; run `cargo clean` and rebuild to re-fetch, or \
         build with --features localctp to use the embedded simulator instead",
        src_dylib.display(),
    );

    let signed_dylib = out_dir.join(format!("{}_adhoc.dylib", framework_name));
    fs::copy(&src_dylib, &signed_dylib)
        .unwrap_or_else(|e| panic!("copy {}: {}", framework_name, e));
    // 清除随 framework 下载附带的隔离扩展属性及其他扩展属性
    // ——否则 codesign 会拒绝操作。
    let _ = Command::new("xattr").arg("-c").arg(&signed_dylib).status();
    // 尽力删除签名；如果没有签名，报错也忽略
    let _ = Command::new("codesign")
        .arg("--remove-signature")
        .arg(&signed_dylib)
        .status();
    let status = Command::new("codesign")
        .arg("--force")
        .arg("--sign")
        .arg("-")
        .arg(&signed_dylib)
        .status()
        .expect("codesign not found — required to re-sign CTP dylibs on macOS");
    assert!(
        status.success(),
        "codesign --sign - failed for {}",
        signed_dylib.display()
    );

    // .incbin 路径必须是绝对路径——clang 的内联汇编器相对于其
    // CWD 解析，我们在此无法控制 CWD。
    let dylib_abs = signed_dylib.canonicalize().unwrap();
    let blob_cpp = out_dir.join(format!("{}_dylib_blob.cpp", sym_prefix));
    let blob_src = format!(
        "// 由 build.rs 自动生成——请勿编辑。\n\
         __asm__(\n\
         \"    .section __DATA,__const\\n\"\n\
         \"    .p2align 4\\n\"\n\
         \"    .globl _ctp_rs_{prefix}_dylib_start\\n\"\n\
         \"_ctp_rs_{prefix}_dylib_start:\\n\"\n\
         \"    .incbin \\\"{path}\\\"\\n\"\n\
         \"    .globl _ctp_rs_{prefix}_dylib_end\\n\"\n\
         \"_ctp_rs_{prefix}_dylib_end:\\n\"\n\
         );\n",
        prefix = sym_prefix,
        path = dylib_abs.display()
    );
    fs::write(&blob_cpp, blob_src).unwrap();
    println!("cargo:rerun-if-changed={}", src_dylib.display());
    blob_cpp.to_string_lossy().into_owned()
}

fn build_localctp(localctp_dir: &Path, lib_dir: &Path) -> PathBuf {
    let build_out = PathBuf::from(std::env::var("OUT_DIR").unwrap());

    // 所有 C++ 源文件——dllmain.cpp 内部由 #ifdef _WIN32 保护，Linux/macOS 上也可以安全编译
    let cpp_sources = [
        "LocalCTP.cpp",
        "LocalTraderApi.cpp",
        "CSqliteHandler.cpp",
        "LeeDateTime.cpp",
        "dllmain.cpp",
        "Properties.cpp",
        "stdafx.cpp",
        "auto_generated_code/CTPSQLWrapper.cpp",
    ];
    // SQLite 必须以 C 编译，不能用 C++
    let c_sources = ["sqlite/sqlite3.c"];

    if cfg!(target_os = "windows") {
        let output = build_out.join("thosttraderapi_se.dll");
        let obj_dir = build_out.join("localctp_obj");
        fs::create_dir_all(&obj_dir).unwrap();

        // 用 cc 定位 cl.exe 并设置 MSVC 环境（PATH、INCLUDE、LIB）
        let compiler = cc::Build::new().cpp(true).get_compiler();
        let mut cmd = Command::new(compiler.path());
        for (k, v) in compiler.env() {
            cmd.env(k, v);
        }

        // Release|x64 设置，来自 LocalCTP.vcxproj
        cmd.arg("/nologo")
            .arg("/LD") // 构建 DLL
            .arg(format!("/Fe:{}", output.display()))
            .arg(format!("/Fo{}\\", obj_dir.display()))
            .arg("/EHsc")
            .arg("/O2")
            .arg("/Gy") // 函数级链接
            .arg("/Oi") // 内联函数
            .arg("/MT") // MultiThreaded 静态运行时（vcxproj Release|x64）
            .arg("/source-charset:.936") // localctp 源码为 GBK/CP936 编码
            .arg("/std:c++20")
            .arg("/w");

        for def in &[
            "NDEBUG",
            "LOCALCTP_EXPORTS",
            "_WINDOWS",
            "_USRDLL",
            "_CRT_SECURE_NO_WARNINGS",
            "_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING",
        ] {
            cmd.arg(format!("/D{}", def));
        }

        cmd.arg(format!(
            "/I{}",
            localctp_dir.join("auto_generated_code").display()
        ))
        .arg(format!("/I{}", lib_dir.display()));

        for src in &cpp_sources {
            cmd.arg(localctp_dir.join(src));
        }
        for src in &c_sources {
            cmd.arg(localctp_dir.join(src));
        }

        // 从 build_out 运行，使生成的 .lib/.exp 落在该目录而非源码树
        cmd.current_dir(&build_out);

        let status = cmd
            .status()
            .expect("cl.exe not found — install Visual Studio Build Tools");
        assert!(status.success(), "localctp MSVC build failed");

        output
    } else {
        let is_macos = cfg!(target_os = "macos");
        let cxx = if is_macos { "c++" } else { "g++" };
        let cc = if is_macos { "cc" } else { "gcc" };
        let obj_dir = build_out.join("localctp_obj");
        fs::create_dir_all(&obj_dir).unwrap();

        let includes = [
            format!("-I{}", localctp_dir.join("auto_generated_code").display()),
            format!("-I{}", lib_dir.display()),
        ];

        // 第 1 步：编译 C++ 源文件（makefile CFLAGS: -std=c++11 -Wall -Wno-format-security -fPIC）
        let mut cpp_objs: Vec<PathBuf> = Vec::new();
        for src in &cpp_sources {
            let obj = obj_dir
                .join(Path::new(src).file_stem().unwrap())
                .with_extension("o");
            let mut cmd = Command::new(cxx);
            cmd.arg("-c")
                .arg("-fPIC")
                .arg("-std=c++11")
                .arg("-Wall")
                .arg("-Wno-format-security")
                .arg("-DLOCALCTP_EXPORTS")
                .args(&includes)
                .arg("-o")
                .arg(&obj)
                .arg(localctp_dir.join(src));
            let status = cmd.status().unwrap_or_else(|_| panic!("{} not found", cxx));
            assert!(status.success(), "localctp: failed to compile {}", src);
            cpp_objs.push(obj);
        }

        // 第 2 步：编译 C 源文件（makefile CFLAGS2: -Wall -fPIC，无 -std=c++11）
        let mut c_objs: Vec<PathBuf> = Vec::new();
        for src in &c_sources {
            let obj = obj_dir
                .join(Path::new(src).file_stem().unwrap())
                .with_extension("o");
            let mut cmd = Command::new(cc);
            cmd.arg("-c")
                .arg("-fPIC")
                .arg("-Wall")
                .arg("-o")
                .arg(&obj)
                .arg(localctp_dir.join(src));
            let status = cmd.status().unwrap_or_else(|_| panic!("{} not found", cc));
            assert!(status.success(), "localctp: failed to compile {}", src);
            c_objs.push(obj);
        }

        if is_macos {
            // 打包为静态库。cargo 通过 cargo:rustc-link-lib=static=thosttraderapi_se
            // 将其链接到最终二进制。macOS 上 dlopen 和 pthread 均属于
            // libSystem，二进制层面无需额外链接标志。
            let output = build_out.join("libthosttraderapi_se.a");
            let _ = fs::remove_file(&output);
            let mut cmd = Command::new("ar");
            cmd.arg("rcs").arg(&output);
            cmd.args(&cpp_objs).args(&c_objs);
            let status = cmd.status().expect("ar not found");
            assert!(status.success(), "localctp: archiving failed");
            output
        } else {
            // Linux：链接共享库（makefile: g++ $^ -o $@ -pthread -ldl -shared）
            let output = build_out.join("libthosttraderapi_se.so");
            let mut cmd = Command::new(cxx);
            cmd.arg("-shared").arg("-o").arg(&output);
            cmd.args(&cpp_objs).args(&c_objs);
            cmd.arg("-pthread").arg("-ldl");
            let status = cmd.status().unwrap_or_else(|_| panic!("{} not found", cxx));
            assert!(status.success(), "localctp: linking failed");
            output
        }
    }
}
