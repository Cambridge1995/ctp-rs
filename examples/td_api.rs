use ctp_rs::{
    QryInstrument, ReqAuthenticate, ReqUserLogin, ResumeType, TraderApi,
    TraderMsg,
};
use std::sync::{Arc, mpsc::channel};

const BROKER_ID: &str = "9999";
const USER_ID: &str = "251795";
const PASSWORD: &str = "wjq!15074971011";
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
