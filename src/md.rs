#![allow(non_camel_case_types)]
use std::fs::create_dir_all;
use std::path::Path;
use std::sync::mpsc::Sender;



use crate::{CreateMdApi, UniquePtr};

unsafe impl Send for crate::MdApi {}
unsafe impl Sync for crate::MdApi {}

impl crate::MdApi {
    pub fn create_md_api_and_spi(
        tx: Sender<MdMsg>,
        flow_path: String,
        is_using_udp: bool,
        is_multicast: bool,
        is_production_mode: bool,
    ) -> UniquePtr<crate::MdApi> {
        if !Path::new(&flow_path).exists() {
            create_dir_all(&flow_path).unwrap();
        }
        CreateMdApi(
            Box::new(MdSpi { tx }),
            flow_path,
            is_using_udp,
            is_multicast,
            is_production_mode,
        )
    }
}

#[derive(Debug, Clone)]
pub enum MdMsg {
    on_front_connected,
    on_front_disconnected(i32),
    on_heart_beat_warning(i32),
    on_rsp_user_login(Box<crate::RspUserLogin>, Box<crate::RspInfo>, i32, bool),
    on_rsp_user_logout(Box<crate::UserLogout>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_multicast_instrument(
        Box<crate::MulticastInstrument>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_error(Box<crate::RspInfo>, i32, bool),
    on_rsp_sub_market_data(
        Box<crate::SpecificInstrument>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_un_sub_market_data(
        Box<crate::SpecificInstrument>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_sub_for_quote_rsp(
        Box<crate::SpecificInstrument>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_un_sub_for_quote_rsp(
        Box<crate::SpecificInstrument>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rtn_depth_market_data(Box<crate::DepthMarketData>),
    on_rtn_for_quote_rsp(Box<crate::ForQuoteRsp>),
}

pub struct MdSpi {
    tx: Sender<MdMsg>,
}

impl MdSpi {
    pub fn on_front_connected(&self) {
        self.tx.send(MdMsg::on_front_connected).ok();
    }
    pub fn on_front_disconnected(&self, reason: i32) {
        self.tx.send(MdMsg::on_front_disconnected(reason)).ok();
    }
    pub fn on_heart_beat_warning(&self, time_lapse: i32) {
        self.tx.send(MdMsg::on_heart_beat_warning(time_lapse)).ok();
    }
    pub fn on_rsp_user_login(
        &self,
        rsp_user_login: crate::RspUserLogin,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(MdMsg::on_rsp_user_login(
                Box::new(rsp_user_login),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_user_logout(
        &self,
        user_logout: crate::UserLogout,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(MdMsg::on_rsp_user_logout(
                Box::new(user_logout),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_multicast_instrument(
        &self,
        multicast_instrument: crate::MulticastInstrument,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(MdMsg::on_rsp_qry_multicast_instrument(
                Box::new(multicast_instrument),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_error(&self, rsp_info: crate::RspInfo, request_id: i32, is_last: bool) {
        self.tx
            .send(MdMsg::on_rsp_error(Box::new(rsp_info), request_id, is_last))
            .ok();
    }
    pub fn on_rsp_sub_market_data(
        &self,
        specific_instrument: crate::SpecificInstrument,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(MdMsg::on_rsp_sub_market_data(
                Box::new(specific_instrument),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_un_sub_market_data(
        &self,
        specific_instrument: crate::SpecificInstrument,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(MdMsg::on_rsp_un_sub_market_data(
                Box::new(specific_instrument),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_sub_for_quote_rsp(
        &self,
        specific_instrument: crate::SpecificInstrument,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(MdMsg::on_rsp_sub_for_quote_rsp(
                Box::new(specific_instrument),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_un_sub_for_quote_rsp(
        &self,
        specific_instrument: crate::SpecificInstrument,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(MdMsg::on_rsp_un_sub_for_quote_rsp(
                Box::new(specific_instrument),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rtn_depth_market_data(&self, depth_market_data: crate::DepthMarketData) {
        self.tx
            .send(MdMsg::on_rtn_depth_market_data(Box::new(depth_market_data)))
            .ok();
    }
    pub fn on_rtn_for_quote_rsp(&self, for_quote_rsp: crate::ForQuoteRsp) {
        self.tx
            .send(MdMsg::on_rtn_for_quote_rsp(Box::new(for_quote_rsp)))
            .ok();
    }
}
