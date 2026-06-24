#![allow(non_camel_case_types)]
use std::fs::create_dir_all;
use std::path::Path;
use std::sync::mpsc::Sender;

use crate::{CreateTraderApi, UniquePtr};

unsafe impl Send for crate::TraderApi {}
unsafe impl Sync for crate::TraderApi {}

impl crate::TraderApi {
    pub fn create_trader_api_and_spi(
        tx: Sender<TraderMsg>,
        flow_path: String,
        is_production_mode: bool,
    ) -> UniquePtr<crate::TraderApi> {
        if !Path::new(&flow_path).exists() {
            create_dir_all(&flow_path).unwrap();
        }
        CreateTraderApi(Box::new(TraderSpi { tx }), flow_path, is_production_mode)
    }
}

#[derive(Debug, Clone)]
pub enum TraderMsg {
    on_front_connected,
    on_front_disconnected(i32),
    on_heart_beat_warning(i32),
    on_rsp_authenticate(Box<crate::RspAuthenticate>, Box<crate::RspInfo>, i32, bool),
    on_rsp_user_login(Box<crate::RspUserLogin>, Box<crate::RspInfo>, i32, bool),
    on_rsp_user_logout(Box<crate::UserLogout>, Box<crate::RspInfo>, i32, bool),
    on_rsp_user_password_update(
        Box<crate::UserPasswordUpdate>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_trading_account_password_update(
        Box<crate::TradingAccountPasswordUpdate>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_user_auth_method(Box<crate::RspUserAuthMethod>, Box<crate::RspInfo>, i32, bool),
    on_rsp_gen_user_captcha(Box<crate::RspGenUserCaptcha>, Box<crate::RspInfo>, i32, bool),
    on_rsp_gen_user_text(Box<crate::RspGenUserText>, Box<crate::RspInfo>, i32, bool),
    on_rsp_order_insert(Box<crate::InputOrder>, Box<crate::RspInfo>, i32, bool),
    on_rsp_parked_order_insert(Box<crate::ParkedOrder>, Box<crate::RspInfo>, i32, bool),
    on_rsp_parked_order_action(Box<crate::ParkedOrderAction>, Box<crate::RspInfo>, i32, bool),
    on_rsp_order_action(Box<crate::InputOrderAction>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_max_order_volume(
        Box<crate::QryMaxOrderVolume>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_settlement_info_confirm(
        Box<crate::SettlementInfoConfirm>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_remove_parked_order(
        Box<crate::RemoveParkedOrder>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_remove_parked_order_action(
        Box<crate::RemoveParkedOrderAction>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_exec_order_insert(Box<crate::InputExecOrder>, Box<crate::RspInfo>, i32, bool),
    on_rsp_exec_order_action(
        Box<crate::InputExecOrderAction>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_for_quote_insert(Box<crate::InputForQuote>, Box<crate::RspInfo>, i32, bool),
    on_rsp_quote_insert(Box<crate::InputQuote>, Box<crate::RspInfo>, i32, bool),
    on_rsp_quote_action(Box<crate::InputQuoteAction>, Box<crate::RspInfo>, i32, bool),
    on_rsp_batch_order_action(
        Box<crate::InputBatchOrderAction>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_option_self_close_insert(
        Box<crate::InputOptionSelfClose>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_option_self_close_action(
        Box<crate::InputOptionSelfCloseAction>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_comb_action_insert(Box<crate::InputCombAction>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_order(Box<crate::Order>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_trade(Box<crate::Trade>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_investor_position(
        Box<crate::InvestorPosition>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_trading_account(Box<crate::TradingAccount>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_investor(Box<crate::Investor>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_trading_code(Box<crate::TradingCode>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_instrument_margin_rate(
        Box<crate::InstrumentMarginRate>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_instrument_commission_rate(
        Box<crate::InstrumentCommissionRate>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_user_session(Box<crate::UserSession>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_exchange(Box<crate::Exchange>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_product(Box<crate::Product>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_instrument(Box<crate::Instrument>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_depth_market_data(
        Box<crate::DepthMarketData>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_trader_offer(Box<crate::TraderOffer>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_settlement_info(Box<crate::SettlementInfo>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_transfer_bank(Box<crate::TransferBank>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_investor_position_detail(
        Box<crate::InvestorPositionDetail>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_notice(Box<crate::Notice>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_settlement_info_confirm(
        Box<crate::SettlementInfoConfirm>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_position_combine_detail(
        Box<crate::InvestorPositionCombineDetail>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_cfmmc_trading_account_key(
        Box<crate::CFMMCTradingAccountKey>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_ewarrant_offset(Box<crate::EWarrantOffset>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_investor_product_group_margin(
        Box<crate::InvestorProductGroupMargin>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_exchange_margin_rate(
        Box<crate::ExchangeMarginRate>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_exchange_margin_rate_adjust(
        Box<crate::ExchangeMarginRateAdjust>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_exchange_rate(Box<crate::ExchangeRate>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_sec_agent_acid_map(
        Box<crate::SecAgentACIDMap>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_product_exch_rate(
        Box<crate::ProductExchRate>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_product_group(Box<crate::ProductGroup>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_mm_instrument_commission_rate(
        Box<crate::MMInstrumentCommissionRate>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_mm_option_instr_comm_rate(
        Box<crate::MMOptionInstrCommRate>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_instrument_order_comm_rate(
        Box<crate::InstrumentOrderCommRate>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_sec_agent_trading_account(
        Box<crate::TradingAccount>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_sec_agent_check_mode(
        Box<crate::SecAgentCheckMode>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_sec_agent_trade_info(
        Box<crate::SecAgentTradeInfo>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_option_instr_trade_cost(
        Box<crate::OptionInstrTradeCost>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_option_instr_comm_rate(
        Box<crate::OptionInstrCommRate>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_exec_order(Box<crate::ExecOrder>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_for_quote(Box<crate::ForQuote>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_quote(Box<crate::Quote>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_option_self_close(
        Box<crate::OptionSelfClose>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_invest_unit(Box<crate::InvestUnit>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_comb_instrument_guard(
        Box<crate::CombInstrumentGuard>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_comb_action(Box<crate::CombAction>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_transfer_serial(Box<crate::TransferSerial>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_account_register(Box<crate::AccountRegister>, Box<crate::RspInfo>, i32, bool),
    on_rsp_error(Box<crate::RspInfo>, i32, bool),
    on_rtn_order(Box<crate::Order>),
    on_rtn_trade(Box<crate::Trade>),
    on_err_rtn_order_insert(Box<crate::InputOrder>, Box<crate::RspInfo>),
    on_err_rtn_order_action(Box<crate::OrderAction>, Box<crate::RspInfo>),
    on_rtn_instrument_status(Box<crate::InstrumentStatus>),
    on_rtn_bulletin(Box<crate::Bulletin>),
    on_rtn_trading_notice(Box<crate::TradingNoticeInfo>),
    on_rtn_error_conditional_order(Box<crate::ErrorConditionalOrder>),
    on_rtn_exec_order(Box<crate::ExecOrder>),
    on_err_rtn_exec_order_insert(Box<crate::InputExecOrder>, Box<crate::RspInfo>),
    on_err_rtn_exec_order_action(Box<crate::ExecOrderAction>, Box<crate::RspInfo>),
    on_err_rtn_for_quote_insert(Box<crate::InputForQuote>, Box<crate::RspInfo>),
    on_rtn_quote(Box<crate::Quote>),
    on_err_rtn_quote_insert(Box<crate::InputQuote>, Box<crate::RspInfo>),
    on_err_rtn_quote_action(Box<crate::QuoteAction>, Box<crate::RspInfo>),
    on_rtn_for_quote_rsp(Box<crate::ForQuoteRsp>),
    on_rtn_cfmmc_trading_account_token(Box<crate::CFMMCTradingAccountToken>),
    on_err_rtn_batch_order_action(Box<crate::BatchOrderAction>, Box<crate::RspInfo>),
    on_rtn_option_self_close(Box<crate::OptionSelfClose>),
    on_err_rtn_option_self_close_insert(
        Box<crate::InputOptionSelfClose>,
        Box<crate::RspInfo>,
    ),
    on_err_rtn_option_self_close_action(
        Box<crate::OptionSelfCloseAction>,
        Box<crate::RspInfo>,
    ),
    on_rtn_comb_action(Box<crate::CombAction>),
    on_err_rtn_comb_action_insert(Box<crate::InputCombAction>, Box<crate::RspInfo>),
    on_rsp_qry_contract_bank(Box<crate::ContractBank>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_parked_order(Box<crate::ParkedOrder>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_parked_order_action(
        Box<crate::ParkedOrderAction>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_trading_notice(Box<crate::TradingNotice>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_broker_trading_params(
        Box<crate::BrokerTradingParams>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_broker_trading_algos(
        Box<crate::BrokerTradingAlgos>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_query_cfmmc_trading_account_token(
        Box<crate::QueryCFMMCTradingAccountToken>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rtn_from_bank_to_future_by_bank(Box<crate::RspTransfer>),
    on_rtn_from_future_to_bank_by_bank(Box<crate::RspTransfer>),
    on_rtn_repeal_from_bank_to_future_by_bank(Box<crate::RspRepeal>),
    on_rtn_repeal_from_future_to_bank_by_bank(Box<crate::RspRepeal>),
    on_rtn_from_bank_to_future_by_future(Box<crate::RspTransfer>),
    on_rtn_from_future_to_bank_by_future(Box<crate::RspTransfer>),
    on_rtn_repeal_from_bank_to_future_by_future_manual(Box<crate::RspRepeal>),
    on_rtn_repeal_from_future_to_bank_by_future_manual(Box<crate::RspRepeal>),
    on_rtn_query_bank_balance_by_future(Box<crate::NotifyQueryAccount>),
    on_err_rtn_bank_to_future_by_future(Box<crate::ReqTransfer>, Box<crate::RspInfo>),
    on_err_rtn_future_to_bank_by_future(Box<crate::ReqTransfer>, Box<crate::RspInfo>),
    on_err_rtn_repeal_bank_to_future_by_future_manual(
        Box<crate::ReqRepeal>,
        Box<crate::RspInfo>,
    ),
    on_err_rtn_repeal_future_to_bank_by_future_manual(
        Box<crate::ReqRepeal>,
        Box<crate::RspInfo>,
    ),
    on_err_rtn_query_bank_balance_by_future(
        Box<crate::ReqQueryAccount>,
        Box<crate::RspInfo>,
    ),
    on_rtn_repeal_from_bank_to_future_by_future(Box<crate::RspRepeal>),
    on_rtn_repeal_from_future_to_bank_by_future(Box<crate::RspRepeal>),
    on_rsp_from_bank_to_future_by_future(
        Box<crate::ReqTransfer>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_from_future_to_bank_by_future(
        Box<crate::ReqTransfer>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_query_bank_account_money_by_future(
        Box<crate::ReqQueryAccount>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rtn_open_account_by_bank(Box<crate::OpenAccount>),
    on_rtn_cancel_account_by_bank(Box<crate::CancelAccount>),
    on_rtn_change_account_by_bank(Box<crate::ChangeAccount>),
    on_rsp_qry_classified_instrument(Box<crate::Instrument>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_comb_promotion_param(
        Box<crate::CombPromotionParam>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_risk_settle_invest_position(
        Box<crate::RiskSettleInvestPosition>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_risk_settle_product_status(
        Box<crate::RiskSettleProductStatus>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spbm_future_parameter(
        Box<crate::SPBMFutureParameter>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spbm_option_parameter(
        Box<crate::SPBMOptionParameter>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spbm_intra_parameter(
        Box<crate::SPBMIntraParameter>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spbm_inter_parameter(
        Box<crate::SPBMInterParameter>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spbm_portf_definition(
        Box<crate::SPBMPortfDefinition>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spbm_investor_portf_def(
        Box<crate::SPBMInvestorPortfDef>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_portf_margin_ratio(
        Box<crate::InvestorPortfMarginRatio>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_prod_spbm_detail(
        Box<crate::InvestorProdSPBMDetail>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_commodity_spmm_margin(
        Box<crate::InvestorCommoditySPMMMargin>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_commodity_group_spmm_margin(
        Box<crate::InvestorCommodityGroupSPMMMargin>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spmm_inst_param(Box<crate::SPMMInstParam>, Box<crate::RspInfo>, i32, bool),
    on_rsp_qry_spmm_product_param(
        Box<crate::SPMMProductParam>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spbm_add_on_inter_parameter(
        Box<crate::SPBMAddOnInterParameter>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rcams_comb_product_info(
        Box<crate::RCAMSCombProductInfo>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rcams_instr_parameter(
        Box<crate::RCAMSInstrParameter>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rcams_intra_parameter(
        Box<crate::RCAMSIntraParameter>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rcams_inter_parameter(
        Box<crate::RCAMSInterParameter>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rcams_short_opt_adjust_param(
        Box<crate::RCAMSShortOptAdjustParam>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rcams_investor_comb_position(
        Box<crate::RCAMSInvestorCombPosition>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_prod_rcams_margin(
        Box<crate::InvestorProdRCAMSMargin>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rule_instr_parameter(
        Box<crate::RULEInstrParameter>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rule_intra_parameter(
        Box<crate::RULEIntraParameter>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rule_inter_parameter(
        Box<crate::RULEInterParameter>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_prod_rule_margin(
        Box<crate::InvestorProdRULEMargin>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_portf_setting(
        Box<crate::InvestorPortfSetting>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_info_comm_rec(
        Box<crate::InvestorInfoCommRec>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_comb_leg(Box<crate::CombLeg>, Box<crate::RspInfo>, i32, bool),
    on_rsp_offset_setting(Box<crate::InputOffsetSetting>, Box<crate::RspInfo>, i32, bool),
    on_rsp_cancel_offset_setting(
        Box<crate::InputOffsetSetting>,
        Box<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rtn_offset_setting(Box<crate::OffsetSetting>),
    on_err_rtn_offset_setting(Box<crate::InputOffsetSetting>, Box<crate::RspInfo>),
    on_err_rtn_cancel_offset_setting(Box<crate::CancelOffsetSetting>, Box<crate::RspInfo>),
    on_rsp_qry_offset_setting(Box<crate::OffsetSetting>, Box<crate::RspInfo>, i32, bool),
}

pub struct TraderSpi {
    tx: Sender<TraderMsg>,
}

impl TraderSpi {
    pub fn on_front_connected(&self) {
        self.tx.send(TraderMsg::on_front_connected).ok();
    }
    pub fn on_front_disconnected(&self, reason: i32) {
        self.tx.send(TraderMsg::on_front_disconnected(reason)).ok();
    }
    pub fn on_heart_beat_warning(&self, time_lapse: i32) {
        self.tx.send(TraderMsg::on_heart_beat_warning(time_lapse)).ok();
    }
    pub fn on_rsp_authenticate(
        &self,
        rsp_authenticate: crate::RspAuthenticate,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_authenticate(
                Box::new(rsp_authenticate),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_user_login(
        &self,
        rsp_user_login: crate::RspUserLogin,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_user_login(
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
            .send(TraderMsg::on_rsp_user_logout(
                Box::new(user_logout),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_user_password_update(
        &self,
        user_password_update: crate::UserPasswordUpdate,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_user_password_update(
                Box::new(user_password_update),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_trading_account_password_update(
        &self,
        trading_account_password_update: crate::TradingAccountPasswordUpdate,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_trading_account_password_update(
                Box::new(trading_account_password_update),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_user_auth_method(
        &self,
        rsp_user_auth_method: crate::RspUserAuthMethod,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_user_auth_method(
                Box::new(rsp_user_auth_method),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_gen_user_captcha(
        &self,
        rsp_gen_user_captcha: crate::RspGenUserCaptcha,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_gen_user_captcha(
                Box::new(rsp_gen_user_captcha),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_gen_user_text(
        &self,
        rsp_gen_user_text: crate::RspGenUserText,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_gen_user_text(
                Box::new(rsp_gen_user_text),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_order_insert(
        &self,
        input_order: crate::InputOrder,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_order_insert(
                Box::new(input_order),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_parked_order_insert(
        &self,
        parked_order: crate::ParkedOrder,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_parked_order_insert(
                Box::new(parked_order),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_parked_order_action(
        &self,
        parked_order_action: crate::ParkedOrderAction,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_parked_order_action(
                Box::new(parked_order_action),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_order_action(
        &self,
        input_order_action: crate::InputOrderAction,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_order_action(
                Box::new(input_order_action),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_max_order_volume(
        &self,
        qry_max_order_volume: crate::QryMaxOrderVolume,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_max_order_volume(
                Box::new(qry_max_order_volume),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_settlement_info_confirm(
        &self,
        settlement_info_confirm: crate::SettlementInfoConfirm,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_settlement_info_confirm(
                Box::new(settlement_info_confirm),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_remove_parked_order(
        &self,
        remove_parked_order: crate::RemoveParkedOrder,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_remove_parked_order(
                Box::new(remove_parked_order),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_remove_parked_order_action(
        &self,
        remove_parked_order_action: crate::RemoveParkedOrderAction,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_remove_parked_order_action(
                Box::new(remove_parked_order_action),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_exec_order_insert(
        &self,
        input_exec_order: crate::InputExecOrder,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_exec_order_insert(
                Box::new(input_exec_order),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_exec_order_action(
        &self,
        input_exec_order_action: crate::InputExecOrderAction,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_exec_order_action(
                Box::new(input_exec_order_action),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_for_quote_insert(
        &self,
        input_for_quote: crate::InputForQuote,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_for_quote_insert(
                Box::new(input_for_quote),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_quote_insert(
        &self,
        input_quote: crate::InputQuote,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_quote_insert(
                Box::new(input_quote),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_quote_action(
        &self,
        input_quote_action: crate::InputQuoteAction,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_quote_action(
                Box::new(input_quote_action),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_batch_order_action(
        &self,
        input_batch_order_action: crate::InputBatchOrderAction,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_batch_order_action(
                Box::new(input_batch_order_action),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_option_self_close_insert(
        &self,
        input_option_self_close: crate::InputOptionSelfClose,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_option_self_close_insert(
                Box::new(input_option_self_close),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_option_self_close_action(
        &self,
        input_option_self_close_action: crate::InputOptionSelfCloseAction,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_option_self_close_action(
                Box::new(input_option_self_close_action),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_comb_action_insert(
        &self,
        input_comb_action: crate::InputCombAction,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_comb_action_insert(
                Box::new(input_comb_action),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_order(
        &self,
        order: crate::Order,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_order(
                Box::new(order),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_trade(
        &self,
        trade: crate::Trade,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_trade(
                Box::new(trade),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_investor_position(
        &self,
        investor_position: crate::InvestorPosition,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_investor_position(
                Box::new(investor_position),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_trading_account(
        &self,
        trading_account: crate::TradingAccount,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_trading_account(
                Box::new(trading_account),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_investor(
        &self,
        investor: crate::Investor,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_investor(
                Box::new(investor),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_trading_code(
        &self,
        trading_code: crate::TradingCode,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_trading_code(
                Box::new(trading_code),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_instrument_margin_rate(
        &self,
        instrument_margin_rate: crate::InstrumentMarginRate,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_instrument_margin_rate(
                Box::new(instrument_margin_rate),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_instrument_commission_rate(
        &self,
        instrument_commission_rate: crate::InstrumentCommissionRate,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_instrument_commission_rate(
                Box::new(instrument_commission_rate),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_user_session(
        &self,
        user_session: crate::UserSession,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_user_session(
                Box::new(user_session),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_exchange(
        &self,
        exchange: crate::Exchange,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_exchange(
                Box::new(exchange),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_product(
        &self,
        product: crate::Product,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_product(
                Box::new(product),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_instrument(
        &self,
        instrument: crate::Instrument,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_instrument(
                Box::new(instrument),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_depth_market_data(
        &self,
        depth_market_data: crate::DepthMarketData,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_depth_market_data(
                Box::new(depth_market_data),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_trader_offer(
        &self,
        trader_offer: crate::TraderOffer,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_trader_offer(
                Box::new(trader_offer),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_settlement_info(
        &self,
        settlement_info: crate::SettlementInfo,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_settlement_info(
                Box::new(settlement_info),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_transfer_bank(
        &self,
        transfer_bank: crate::TransferBank,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_transfer_bank(
                Box::new(transfer_bank),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_investor_position_detail(
        &self,
        investor_position_detail: crate::InvestorPositionDetail,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_investor_position_detail(
                Box::new(investor_position_detail),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_notice(
        &self,
        notice: crate::Notice,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_notice(
                Box::new(notice),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_settlement_info_confirm(
        &self,
        settlement_info_confirm: crate::SettlementInfoConfirm,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_settlement_info_confirm(
                Box::new(settlement_info_confirm),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_investor_position_combine_detail(
        &self,
        investor_position_combine_detail: crate::InvestorPositionCombineDetail,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_investor_position_combine_detail(
                Box::new(investor_position_combine_detail),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_cfmmc_trading_account_key(
        &self,
        cfmmc_trading_account_key: crate::CFMMCTradingAccountKey,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_cfmmc_trading_account_key(
                Box::new(cfmmc_trading_account_key),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_ewarrant_offset(
        &self,
        ewarrant_offset: crate::EWarrantOffset,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_ewarrant_offset(
                Box::new(ewarrant_offset),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_investor_product_group_margin(
        &self,
        investor_product_group_margin: crate::InvestorProductGroupMargin,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_investor_product_group_margin(
                Box::new(investor_product_group_margin),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_exchange_margin_rate(
        &self,
        exchange_margin_rate: crate::ExchangeMarginRate,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_exchange_margin_rate(
                Box::new(exchange_margin_rate),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_exchange_margin_rate_adjust(
        &self,
        exchange_margin_rate_adjust: crate::ExchangeMarginRateAdjust,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_exchange_margin_rate_adjust(
                Box::new(exchange_margin_rate_adjust),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_exchange_rate(
        &self,
        exchange_rate: crate::ExchangeRate,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_exchange_rate(
                Box::new(exchange_rate),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_sec_agent_acid_map(
        &self,
        sec_agent_acid_map: crate::SecAgentACIDMap,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_sec_agent_acid_map(
                Box::new(sec_agent_acid_map),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_product_exch_rate(
        &self,
        product_exch_rate: crate::ProductExchRate,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_product_exch_rate(
                Box::new(product_exch_rate),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_product_group(
        &self,
        product_group: crate::ProductGroup,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_product_group(
                Box::new(product_group),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_mm_instrument_commission_rate(
        &self,
        mm_instrument_commission_rate: crate::MMInstrumentCommissionRate,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_mm_instrument_commission_rate(
                Box::new(mm_instrument_commission_rate),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_mm_option_instr_comm_rate(
        &self,
        mm_option_instr_comm_rate: crate::MMOptionInstrCommRate,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_mm_option_instr_comm_rate(
                Box::new(mm_option_instr_comm_rate),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_instrument_order_comm_rate(
        &self,
        instrument_order_comm_rate: crate::InstrumentOrderCommRate,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_instrument_order_comm_rate(
                Box::new(instrument_order_comm_rate),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_sec_agent_trading_account(
        &self,
        trading_account: crate::TradingAccount,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_sec_agent_trading_account(
                Box::new(trading_account),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_sec_agent_check_mode(
        &self,
        sec_agent_check_mode: crate::SecAgentCheckMode,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_sec_agent_check_mode(
                Box::new(sec_agent_check_mode),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_sec_agent_trade_info(
        &self,
        sec_agent_trade_info: crate::SecAgentTradeInfo,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_sec_agent_trade_info(
                Box::new(sec_agent_trade_info),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_option_instr_trade_cost(
        &self,
        option_instr_trade_cost: crate::OptionInstrTradeCost,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_option_instr_trade_cost(
                Box::new(option_instr_trade_cost),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_option_instr_comm_rate(
        &self,
        option_instr_comm_rate: crate::OptionInstrCommRate,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_option_instr_comm_rate(
                Box::new(option_instr_comm_rate),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_exec_order(
        &self,
        exec_order: crate::ExecOrder,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_exec_order(
                Box::new(exec_order),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_for_quote(
        &self,
        for_quote: crate::ForQuote,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_for_quote(
                Box::new(for_quote),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_quote(
        &self,
        quote: crate::Quote,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_quote(
                Box::new(quote),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_option_self_close(
        &self,
        option_self_close: crate::OptionSelfClose,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_option_self_close(
                Box::new(option_self_close),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_invest_unit(
        &self,
        invest_unit: crate::InvestUnit,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_invest_unit(
                Box::new(invest_unit),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_comb_instrument_guard(
        &self,
        comb_instrument_guard: crate::CombInstrumentGuard,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_comb_instrument_guard(
                Box::new(comb_instrument_guard),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_comb_action(
        &self,
        comb_action: crate::CombAction,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_comb_action(
                Box::new(comb_action),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_transfer_serial(
        &self,
        transfer_serial: crate::TransferSerial,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_transfer_serial(
                Box::new(transfer_serial),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_account_register(
        &self,
        account_register: crate::AccountRegister,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_account_register(
                Box::new(account_register),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_error(&self, rsp_info: crate::RspInfo, request_id: i32, is_last: bool) {
        self.tx
            .send(TraderMsg::on_rsp_error(Box::new(rsp_info), request_id, is_last))
            .ok();
    }
    pub fn on_rtn_order(&self, order: crate::Order) {
        self.tx.send(TraderMsg::on_rtn_order(Box::new(order))).ok();
    }
    pub fn on_rtn_trade(&self, trade: crate::Trade) {
        self.tx.send(TraderMsg::on_rtn_trade(Box::new(trade))).ok();
    }
    pub fn on_err_rtn_order_insert(&self, input_order: crate::InputOrder, rsp_info: crate::RspInfo) {
        self.tx
            .send(TraderMsg::on_err_rtn_order_insert(
                Box::new(input_order),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_err_rtn_order_action(
        &self,
        order_action: crate::OrderAction,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_order_action(
                Box::new(order_action),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_rtn_instrument_status(&self, instrument_status: crate::InstrumentStatus) {
        self.tx
            .send(TraderMsg::on_rtn_instrument_status(Box::new(instrument_status)))
            .ok();
    }
    pub fn on_rtn_bulletin(&self, bulletin: crate::Bulletin) {
        self.tx.send(TraderMsg::on_rtn_bulletin(Box::new(bulletin))).ok();
    }
    pub fn on_rtn_trading_notice(&self, trading_notice_info: crate::TradingNoticeInfo) {
        self.tx
            .send(TraderMsg::on_rtn_trading_notice(Box::new(trading_notice_info)))
            .ok();
    }
    pub fn on_rtn_error_conditional_order(
        &self,
        error_conditional_order: crate::ErrorConditionalOrder,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_error_conditional_order(Box::new(
                error_conditional_order,
            )))
            .ok();
    }
    pub fn on_rtn_exec_order(&self, exec_order: crate::ExecOrder) {
        self.tx
            .send(TraderMsg::on_rtn_exec_order(Box::new(exec_order)))
            .ok();
    }
    pub fn on_err_rtn_exec_order_insert(
        &self,
        input_exec_order: crate::InputExecOrder,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_exec_order_insert(
                Box::new(input_exec_order),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_err_rtn_exec_order_action(
        &self,
        exec_order_action: crate::ExecOrderAction,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_exec_order_action(
                Box::new(exec_order_action),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_err_rtn_for_quote_insert(
        &self,
        input_for_quote: crate::InputForQuote,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_for_quote_insert(
                Box::new(input_for_quote),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_rtn_quote(&self, quote: crate::Quote) {
        self.tx.send(TraderMsg::on_rtn_quote(Box::new(quote))).ok();
    }
    pub fn on_err_rtn_quote_insert(&self, input_quote: crate::InputQuote, rsp_info: crate::RspInfo) {
        self.tx
            .send(TraderMsg::on_err_rtn_quote_insert(
                Box::new(input_quote),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_err_rtn_quote_action(
        &self,
        quote_action: crate::QuoteAction,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_quote_action(
                Box::new(quote_action),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_rtn_for_quote_rsp(&self, for_quote_rsp: crate::ForQuoteRsp) {
        self.tx
            .send(TraderMsg::on_rtn_for_quote_rsp(Box::new(for_quote_rsp)))
            .ok();
    }
    pub fn on_rtn_cfmmc_trading_account_token(
        &self,
        cfmmc_trading_account_token: crate::CFMMCTradingAccountToken,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_cfmmc_trading_account_token(Box::new(
                cfmmc_trading_account_token,
            )))
            .ok();
    }
    pub fn on_err_rtn_batch_order_action(
        &self,
        batch_order_action: crate::BatchOrderAction,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_batch_order_action(
                Box::new(batch_order_action),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_rtn_option_self_close(&self, option_self_close: crate::OptionSelfClose) {
        self.tx
            .send(TraderMsg::on_rtn_option_self_close(Box::new(option_self_close)))
            .ok();
    }
    pub fn on_err_rtn_option_self_close_insert(
        &self,
        input_option_self_close: crate::InputOptionSelfClose,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_option_self_close_insert(
                Box::new(input_option_self_close),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_err_rtn_option_self_close_action(
        &self,
        option_self_close_action: crate::OptionSelfCloseAction,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_option_self_close_action(
                Box::new(option_self_close_action),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_rtn_comb_action(&self, comb_action: crate::CombAction) {
        self.tx
            .send(TraderMsg::on_rtn_comb_action(Box::new(comb_action)))
            .ok();
    }
    pub fn on_err_rtn_comb_action_insert(
        &self,
        input_comb_action: crate::InputCombAction,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_comb_action_insert(
                Box::new(input_comb_action),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_rsp_qry_contract_bank(
        &self,
        contract_bank: crate::ContractBank,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_contract_bank(
                Box::new(contract_bank),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_parked_order(
        &self,
        parked_order: crate::ParkedOrder,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_parked_order(
                Box::new(parked_order),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_parked_order_action(
        &self,
        parked_order_action: crate::ParkedOrderAction,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_parked_order_action(
                Box::new(parked_order_action),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_trading_notice(
        &self,
        trading_notice: crate::TradingNotice,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_trading_notice(
                Box::new(trading_notice),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_broker_trading_params(
        &self,
        broker_trading_params: crate::BrokerTradingParams,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_broker_trading_params(
                Box::new(broker_trading_params),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_broker_trading_algos(
        &self,
        broker_trading_algos: crate::BrokerTradingAlgos,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_broker_trading_algos(
                Box::new(broker_trading_algos),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_query_cfmmc_trading_account_token(
        &self,
        query_cfmmc_trading_account_token: crate::QueryCFMMCTradingAccountToken,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_query_cfmmc_trading_account_token(
                Box::new(query_cfmmc_trading_account_token),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rtn_from_bank_to_future_by_bank(&self, rsp_transfer: crate::RspTransfer) {
        self.tx
            .send(TraderMsg::on_rtn_from_bank_to_future_by_bank(Box::new(
                rsp_transfer,
            )))
            .ok();
    }
    pub fn on_rtn_from_future_to_bank_by_bank(&self, rsp_transfer: crate::RspTransfer) {
        self.tx
            .send(TraderMsg::on_rtn_from_future_to_bank_by_bank(Box::new(
                rsp_transfer,
            )))
            .ok();
    }
    pub fn on_rtn_repeal_from_bank_to_future_by_bank(&self, rsp_repeal: crate::RspRepeal) {
        self.tx
            .send(TraderMsg::on_rtn_repeal_from_bank_to_future_by_bank(Box::new(
                rsp_repeal,
            )))
            .ok();
    }
    pub fn on_rtn_repeal_from_future_to_bank_by_bank(&self, rsp_repeal: crate::RspRepeal) {
        self.tx
            .send(TraderMsg::on_rtn_repeal_from_future_to_bank_by_bank(Box::new(
                rsp_repeal,
            )))
            .ok();
    }
    pub fn on_rtn_from_bank_to_future_by_future(&self, rsp_transfer: crate::RspTransfer) {
        self.tx
            .send(TraderMsg::on_rtn_from_bank_to_future_by_future(Box::new(
                rsp_transfer,
            )))
            .ok();
    }
    pub fn on_rtn_from_future_to_bank_by_future(&self, rsp_transfer: crate::RspTransfer) {
        self.tx
            .send(TraderMsg::on_rtn_from_future_to_bank_by_future(Box::new(
                rsp_transfer,
            )))
            .ok();
    }
    pub fn on_rtn_repeal_from_bank_to_future_by_future_manual(
        &self,
        rsp_repeal: crate::RspRepeal,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_repeal_from_bank_to_future_by_future_manual(
                Box::new(rsp_repeal),
            ))
            .ok();
    }
    pub fn on_rtn_repeal_from_future_to_bank_by_future_manual(
        &self,
        rsp_repeal: crate::RspRepeal,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_repeal_from_future_to_bank_by_future_manual(
                Box::new(rsp_repeal),
            ))
            .ok();
    }
    pub fn on_rtn_query_bank_balance_by_future(
        &self,
        notify_query_account: crate::NotifyQueryAccount,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_query_bank_balance_by_future(Box::new(
                notify_query_account,
            )))
            .ok();
    }
    pub fn on_err_rtn_bank_to_future_by_future(
        &self,
        req_transfer: crate::ReqTransfer,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_bank_to_future_by_future(
                Box::new(req_transfer),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_err_rtn_future_to_bank_by_future(
        &self,
        req_transfer: crate::ReqTransfer,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_future_to_bank_by_future(
                Box::new(req_transfer),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_err_rtn_repeal_bank_to_future_by_future_manual(
        &self,
        req_repeal: crate::ReqRepeal,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_repeal_bank_to_future_by_future_manual(
                Box::new(req_repeal),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_err_rtn_repeal_future_to_bank_by_future_manual(
        &self,
        req_repeal: crate::ReqRepeal,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_repeal_future_to_bank_by_future_manual(
                Box::new(req_repeal),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_err_rtn_query_bank_balance_by_future(
        &self,
        req_query_account: crate::ReqQueryAccount,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_query_bank_balance_by_future(
                Box::new(req_query_account),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_rtn_repeal_from_bank_to_future_by_future(&self, rsp_repeal: crate::RspRepeal) {
        self.tx
            .send(TraderMsg::on_rtn_repeal_from_bank_to_future_by_future(Box::new(
                rsp_repeal,
            )))
            .ok();
    }
    pub fn on_rtn_repeal_from_future_to_bank_by_future(&self, rsp_repeal: crate::RspRepeal) {
        self.tx
            .send(TraderMsg::on_rtn_repeal_from_future_to_bank_by_future(Box::new(
                rsp_repeal,
            )))
            .ok();
    }
    pub fn on_rsp_from_bank_to_future_by_future(
        &self,
        req_transfer: crate::ReqTransfer,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_from_bank_to_future_by_future(
                Box::new(req_transfer),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_from_future_to_bank_by_future(
        &self,
        req_transfer: crate::ReqTransfer,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_from_future_to_bank_by_future(
                Box::new(req_transfer),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_query_bank_account_money_by_future(
        &self,
        req_query_account: crate::ReqQueryAccount,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_query_bank_account_money_by_future(
                Box::new(req_query_account),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rtn_open_account_by_bank(&self, open_account: crate::OpenAccount) {
        self.tx
            .send(TraderMsg::on_rtn_open_account_by_bank(Box::new(open_account)))
            .ok();
    }
    pub fn on_rtn_cancel_account_by_bank(&self, cancel_account: crate::CancelAccount) {
        self.tx
            .send(TraderMsg::on_rtn_cancel_account_by_bank(Box::new(cancel_account)))
            .ok();
    }
    pub fn on_rtn_change_account_by_bank(&self, change_account: crate::ChangeAccount) {
        self.tx
            .send(TraderMsg::on_rtn_change_account_by_bank(Box::new(change_account)))
            .ok();
    }
    pub fn on_rsp_qry_classified_instrument(
        &self,
        instrument: crate::Instrument,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_classified_instrument(
                Box::new(instrument),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_comb_promotion_param(
        &self,
        comb_promotion_param: crate::CombPromotionParam,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_comb_promotion_param(
                Box::new(comb_promotion_param),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_risk_settle_invest_position(
        &self,
        risk_settle_invest_position: crate::RiskSettleInvestPosition,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_risk_settle_invest_position(
                Box::new(risk_settle_invest_position),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_risk_settle_product_status(
        &self,
        risk_settle_product_status: crate::RiskSettleProductStatus,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_risk_settle_product_status(
                Box::new(risk_settle_product_status),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_spbm_future_parameter(
        &self,
        spbm_future_parameter: crate::SPBMFutureParameter,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_spbm_future_parameter(
                Box::new(spbm_future_parameter),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_spbm_option_parameter(
        &self,
        spbm_option_parameter: crate::SPBMOptionParameter,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_spbm_option_parameter(
                Box::new(spbm_option_parameter),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_spbm_intra_parameter(
        &self,
        spbm_intra_parameter: crate::SPBMIntraParameter,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_spbm_intra_parameter(
                Box::new(spbm_intra_parameter),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_spbm_inter_parameter(
        &self,
        spbm_inter_parameter: crate::SPBMInterParameter,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_spbm_inter_parameter(
                Box::new(spbm_inter_parameter),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_spbm_portf_definition(
        &self,
        spbm_portf_definition: crate::SPBMPortfDefinition,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_spbm_portf_definition(
                Box::new(spbm_portf_definition),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_spbm_investor_portf_def(
        &self,
        spbm_investor_portf_def: crate::SPBMInvestorPortfDef,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_spbm_investor_portf_def(
                Box::new(spbm_investor_portf_def),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_investor_portf_margin_ratio(
        &self,
        investor_portf_margin_ratio: crate::InvestorPortfMarginRatio,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_investor_portf_margin_ratio(
                Box::new(investor_portf_margin_ratio),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_investor_prod_spbm_detail(
        &self,
        investor_prod_spbm_detail: crate::InvestorProdSPBMDetail,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_investor_prod_spbm_detail(
                Box::new(investor_prod_spbm_detail),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_investor_commodity_spmm_margin(
        &self,
        investor_commodity_spmm_margin: crate::InvestorCommoditySPMMMargin,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_investor_commodity_spmm_margin(
                Box::new(investor_commodity_spmm_margin),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_investor_commodity_group_spmm_margin(
        &self,
        investor_commodity_group_spmm_margin: crate::InvestorCommodityGroupSPMMMargin,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_investor_commodity_group_spmm_margin(
                Box::new(investor_commodity_group_spmm_margin),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_spmm_inst_param(
        &self,
        spmm_inst_param: crate::SPMMInstParam,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_spmm_inst_param(
                Box::new(spmm_inst_param),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_spmm_product_param(
        &self,
        spmm_product_param: crate::SPMMProductParam,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_spmm_product_param(
                Box::new(spmm_product_param),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_spbm_add_on_inter_parameter(
        &self,
        spbm_add_on_inter_parameter: crate::SPBMAddOnInterParameter,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_spbm_add_on_inter_parameter(
                Box::new(spbm_add_on_inter_parameter),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_rcams_comb_product_info(
        &self,
        rcams_comb_product_info: crate::RCAMSCombProductInfo,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_rcams_comb_product_info(
                Box::new(rcams_comb_product_info),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_rcams_instr_parameter(
        &self,
        rcams_instr_parameter: crate::RCAMSInstrParameter,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_rcams_instr_parameter(
                Box::new(rcams_instr_parameter),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_rcams_intra_parameter(
        &self,
        rcams_intra_parameter: crate::RCAMSIntraParameter,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_rcams_intra_parameter(
                Box::new(rcams_intra_parameter),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_rcams_inter_parameter(
        &self,
        rcams_inter_parameter: crate::RCAMSInterParameter,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_rcams_inter_parameter(
                Box::new(rcams_inter_parameter),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_rcams_short_opt_adjust_param(
        &self,
        rcams_short_opt_adjust_param: crate::RCAMSShortOptAdjustParam,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_rcams_short_opt_adjust_param(
                Box::new(rcams_short_opt_adjust_param),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_rcams_investor_comb_position(
        &self,
        rcams_investor_comb_position: crate::RCAMSInvestorCombPosition,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_rcams_investor_comb_position(
                Box::new(rcams_investor_comb_position),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_investor_prod_rcams_margin(
        &self,
        investor_prod_rcams_margin: crate::InvestorProdRCAMSMargin,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_investor_prod_rcams_margin(
                Box::new(investor_prod_rcams_margin),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_rule_instr_parameter(
        &self,
        rule_instr_parameter: crate::RULEInstrParameter,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_rule_instr_parameter(
                Box::new(rule_instr_parameter),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_rule_intra_parameter(
        &self,
        rule_intra_parameter: crate::RULEIntraParameter,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_rule_intra_parameter(
                Box::new(rule_intra_parameter),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_rule_inter_parameter(
        &self,
        rule_inter_parameter: crate::RULEInterParameter,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_rule_inter_parameter(
                Box::new(rule_inter_parameter),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_investor_prod_rule_margin(
        &self,
        investor_prod_rule_margin: crate::InvestorProdRULEMargin,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_investor_prod_rule_margin(
                Box::new(investor_prod_rule_margin),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_investor_portf_setting(
        &self,
        investor_portf_setting: crate::InvestorPortfSetting,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_investor_portf_setting(
                Box::new(investor_portf_setting),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_investor_info_comm_rec(
        &self,
        investor_info_comm_rec: crate::InvestorInfoCommRec,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_investor_info_comm_rec(
                Box::new(investor_info_comm_rec),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_qry_comb_leg(
        &self,
        comb_leg: crate::CombLeg,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_comb_leg(
                Box::new(comb_leg),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_offset_setting(
        &self,
        input_offset_setting: crate::InputOffsetSetting,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_offset_setting(
                Box::new(input_offset_setting),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_cancel_offset_setting(
        &self,
        input_offset_setting: crate::InputOffsetSetting,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_cancel_offset_setting(
                Box::new(input_offset_setting),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rtn_offset_setting(&self, offset_setting: crate::OffsetSetting) {
        self.tx
            .send(TraderMsg::on_rtn_offset_setting(Box::new(offset_setting)))
            .ok();
    }
    pub fn on_err_rtn_offset_setting(
        &self,
        input_offset_setting: crate::InputOffsetSetting,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_offset_setting(
                Box::new(input_offset_setting),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_err_rtn_cancel_offset_setting(
        &self,
        cancel_offset_setting: crate::CancelOffsetSetting,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_cancel_offset_setting(
                Box::new(cancel_offset_setting),
                Box::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_rsp_qry_offset_setting(
        &self,
        offset_setting: crate::OffsetSetting,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_qry_offset_setting(
                Box::new(offset_setting),
                Box::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
}
