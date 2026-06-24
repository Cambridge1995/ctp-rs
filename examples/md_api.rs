use ctp_rs::{MdApi, MdMsg, ReqUserLogin};
use std::sync::{Arc, mpsc::channel};

// 模拟环境地址列表见：http://www.openctp.cn/simenv.html
const FRONT_ADDR: &str = "tcp://182.254.243.31:40011";
const FLOW_PATH: &str = "flow/md/";
const INSTRUMENTS: &[&str] = &["ag2608", "pp2609"];

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
                    broker_id: "9999".to_string(),
                    user_id: "251795".to_string(),
                    password: "wjq!15074971011".to_string(),
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
