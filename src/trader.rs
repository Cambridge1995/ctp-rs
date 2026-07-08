#![allow(non_camel_case_types)]
use std::fs::create_dir_all;
use std::path::Path;
use std::sync::Arc;
use kanal::Sender;



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
        CreateTraderApi(
            Box::new(TraderSpi { tx }),
            flow_path,
            is_production_mode,
        )
    }
}

#[derive(Debug, Clone)]
pub enum TraderMsg {
    on_front_connected,
    on_front_disconnected(i32),
    on_heart_beat_warning(i32),
    on_rsp_authenticate(
        Arc<crate::RspAuthenticate>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_user_login(
        Arc<crate::RspUserLogin>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_user_logout(
        Arc<crate::UserLogout>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_user_password_update(
        Arc<crate::UserPasswordUpdate>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_trading_account_password_update(
        Arc<crate::TradingAccountPasswordUpdate>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_user_auth_method(
        Arc<crate::RspUserAuthMethod>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_gen_user_captcha(
        Arc<crate::RspGenUserCaptcha>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_gen_user_text(
        Arc<crate::RspGenUserText>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_order_insert(
        Arc<crate::InputOrder>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_parked_order_insert(
        Arc<crate::ParkedOrder>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_parked_order_action(
        Arc<crate::ParkedOrderAction>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_order_action(
        Arc<crate::InputOrderAction>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_max_order_volume(
        Arc<crate::QryMaxOrderVolume>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_settlement_info_confirm(
        Arc<crate::SettlementInfoConfirm>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_remove_parked_order(
        Arc<crate::RemoveParkedOrder>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_remove_parked_order_action(
        Arc<crate::RemoveParkedOrderAction>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_exec_order_insert(
        Arc<crate::InputExecOrder>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_exec_order_action(
        Arc<crate::InputExecOrderAction>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_for_quote_insert(
        Arc<crate::InputForQuote>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_quote_insert(
        Arc<crate::InputQuote>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_quote_action(
        Arc<crate::InputQuoteAction>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_batch_order_action(
        Arc<crate::InputBatchOrderAction>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_option_self_close_insert(
        Arc<crate::InputOptionSelfClose>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_option_self_close_action(
        Arc<crate::InputOptionSelfCloseAction>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_comb_action_insert(
        Arc<crate::InputCombAction>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_order(
        Arc<crate::Order>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_trade(
        Arc<crate::Trade>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_position(
        Arc<crate::InvestorPosition>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_trading_account(
        Arc<crate::TradingAccount>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor(
        Arc<crate::Investor>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_trading_code(
        Arc<crate::TradingCode>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_instrument_margin_rate(
        Arc<crate::InstrumentMarginRate>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_instrument_commission_rate(
        Arc<crate::InstrumentCommissionRate>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_user_session(
        Arc<crate::UserSession>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_exchange(
        Arc<crate::Exchange>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_product(
        Arc<crate::Product>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_instrument(
        Arc<crate::Instrument>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_depth_market_data(
        Arc<crate::DepthMarketData>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_trader_offer(
        Arc<crate::TraderOffer>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_settlement_info(
        Arc<crate::SettlementInfo>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_transfer_bank(
        Arc<crate::TransferBank>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_position_detail(
        Arc<crate::InvestorPositionDetail>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_notice(
        Arc<crate::Notice>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_settlement_info_confirm(
        Arc<crate::SettlementInfoConfirm>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_position_combine_detail(
        Arc<crate::InvestorPositionCombineDetail>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_cfmmc_trading_account_key(
        Arc<crate::CFMMCTradingAccountKey>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_ewarrant_offset(
        Arc<crate::EWarrantOffset>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_product_group_margin(
        Arc<crate::InvestorProductGroupMargin>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_exchange_margin_rate(
        Arc<crate::ExchangeMarginRate>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_exchange_margin_rate_adjust(
        Arc<crate::ExchangeMarginRateAdjust>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_exchange_rate(
        Arc<crate::ExchangeRate>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_sec_agent_acid_map(
        Arc<crate::SecAgentACIDMap>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_product_exch_rate(
        Arc<crate::ProductExchRate>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_product_group(
        Arc<crate::ProductGroup>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_mm_instrument_commission_rate(
        Arc<crate::MMInstrumentCommissionRate>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_mm_option_instr_comm_rate(
        Arc<crate::MMOptionInstrCommRate>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_instrument_order_comm_rate(
        Arc<crate::InstrumentOrderCommRate>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_sec_agent_trading_account(
        Arc<crate::TradingAccount>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_sec_agent_check_mode(
        Arc<crate::SecAgentCheckMode>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_sec_agent_trade_info(
        Arc<crate::SecAgentTradeInfo>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_option_instr_trade_cost(
        Arc<crate::OptionInstrTradeCost>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_option_instr_comm_rate(
        Arc<crate::OptionInstrCommRate>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_exec_order(
        Arc<crate::ExecOrder>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_for_quote(
        Arc<crate::ForQuote>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_quote(
        Arc<crate::Quote>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_option_self_close(
        Arc<crate::OptionSelfClose>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_invest_unit(
        Arc<crate::InvestUnit>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_comb_instrument_guard(
        Arc<crate::CombInstrumentGuard>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_comb_action(
        Arc<crate::CombAction>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_transfer_serial(
        Arc<crate::TransferSerial>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_account_register(
        Arc<crate::AccountRegister>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_error(
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rtn_order(Arc<crate::Order>),
    on_rtn_trade(Arc<crate::Trade>),
    on_err_rtn_order_insert(
        Arc<crate::InputOrder>,
        Arc<crate::RspInfo>,
    ),
    on_err_rtn_order_action(
        Arc<crate::OrderAction>,
        Arc<crate::RspInfo>,
    ),
    on_rtn_instrument_status(Arc<crate::InstrumentStatus>),
    on_rtn_bulletin(Arc<crate::Bulletin>),
    on_rtn_trading_notice(Arc<crate::TradingNoticeInfo>),
    on_rtn_error_conditional_order(Arc<crate::ErrorConditionalOrder>),
    on_rtn_exec_order(Arc<crate::ExecOrder>),
    on_err_rtn_exec_order_insert(
        Arc<crate::InputExecOrder>,
        Arc<crate::RspInfo>,
    ),
    on_err_rtn_exec_order_action(
        Arc<crate::ExecOrderAction>,
        Arc<crate::RspInfo>,
    ),
    on_err_rtn_for_quote_insert(
        Arc<crate::InputForQuote>,
        Arc<crate::RspInfo>,
    ),
    on_rtn_quote(Arc<crate::Quote>),
    on_err_rtn_quote_insert(
        Arc<crate::InputQuote>,
        Arc<crate::RspInfo>,
    ),
    on_err_rtn_quote_action(
        Arc<crate::QuoteAction>,
        Arc<crate::RspInfo>,
    ),
    on_rtn_for_quote_rsp(Arc<crate::ForQuoteRsp>),
    on_rtn_cfmmc_trading_account_token(Arc<crate::CFMMCTradingAccountToken>),
    on_err_rtn_batch_order_action(
        Arc<crate::BatchOrderAction>,
        Arc<crate::RspInfo>,
    ),
    on_rtn_option_self_close(Arc<crate::OptionSelfClose>),
    on_err_rtn_option_self_close_insert(
        Arc<crate::InputOptionSelfClose>,
        Arc<crate::RspInfo>,
    ),
    on_err_rtn_option_self_close_action(
        Arc<crate::OptionSelfCloseAction>,
        Arc<crate::RspInfo>,
    ),
    on_rtn_comb_action(Arc<crate::CombAction>),
    on_err_rtn_comb_action_insert(
        Arc<crate::InputCombAction>,
        Arc<crate::RspInfo>,
    ),
    on_rsp_qry_contract_bank(
        Arc<crate::ContractBank>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_parked_order(
        Arc<crate::ParkedOrder>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_parked_order_action(
        Arc<crate::ParkedOrderAction>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_trading_notice(
        Arc<crate::TradingNotice>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_broker_trading_params(
        Arc<crate::BrokerTradingParams>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_broker_trading_algos(
        Arc<crate::BrokerTradingAlgos>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_query_cfmmc_trading_account_token(
        Arc<crate::QueryCFMMCTradingAccountToken>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rtn_from_bank_to_future_by_bank(Arc<crate::RspTransfer>),
    on_rtn_from_future_to_bank_by_bank(Arc<crate::RspTransfer>),
    on_rtn_repeal_from_bank_to_future_by_bank(Arc<crate::RspRepeal>),
    on_rtn_repeal_from_future_to_bank_by_bank(Arc<crate::RspRepeal>),
    on_rtn_from_bank_to_future_by_future(Arc<crate::RspTransfer>),
    on_rtn_from_future_to_bank_by_future(Arc<crate::RspTransfer>),
    on_rtn_repeal_from_bank_to_future_by_future_manual(Arc<crate::RspRepeal>),
    on_rtn_repeal_from_future_to_bank_by_future_manual(Arc<crate::RspRepeal>),
    on_rtn_query_bank_balance_by_future(Arc<crate::NotifyQueryAccount>),
    on_err_rtn_bank_to_future_by_future(
        Arc<crate::ReqTransfer>,
        Arc<crate::RspInfo>,
    ),
    on_err_rtn_future_to_bank_by_future(
        Arc<crate::ReqTransfer>,
        Arc<crate::RspInfo>,
    ),
    on_err_rtn_repeal_bank_to_future_by_future_manual(
        Arc<crate::ReqRepeal>,
        Arc<crate::RspInfo>,
    ),
    on_err_rtn_repeal_future_to_bank_by_future_manual(
        Arc<crate::ReqRepeal>,
        Arc<crate::RspInfo>,
    ),
    on_err_rtn_query_bank_balance_by_future(
        Arc<crate::ReqQueryAccount>,
        Arc<crate::RspInfo>,
    ),
    on_rtn_repeal_from_bank_to_future_by_future(Arc<crate::RspRepeal>),
    on_rtn_repeal_from_future_to_bank_by_future(Arc<crate::RspRepeal>),
    on_rsp_from_bank_to_future_by_future(
        Arc<crate::ReqTransfer>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_from_future_to_bank_by_future(
        Arc<crate::ReqTransfer>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_query_bank_account_money_by_future(
        Arc<crate::ReqQueryAccount>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rtn_open_account_by_bank(Arc<crate::OpenAccount>),
    on_rtn_cancel_account_by_bank(Arc<crate::CancelAccount>),
    on_rtn_change_account_by_bank(Arc<crate::ChangeAccount>),
    on_rsp_qry_classified_instrument(
        Arc<crate::Instrument>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_comb_promotion_param(
        Arc<crate::CombPromotionParam>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_risk_settle_invest_position(
        Arc<crate::RiskSettleInvestPosition>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_risk_settle_product_status(
        Arc<crate::RiskSettleProductStatus>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spbm_future_parameter(
        Arc<crate::SPBMFutureParameter>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spbm_option_parameter(
        Arc<crate::SPBMOptionParameter>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spbm_intra_parameter(
        Arc<crate::SPBMIntraParameter>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spbm_inter_parameter(
        Arc<crate::SPBMInterParameter>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spbm_portf_definition(
        Arc<crate::SPBMPortfDefinition>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spbm_investor_portf_def(
        Arc<crate::SPBMInvestorPortfDef>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_portf_margin_ratio(
        Arc<crate::InvestorPortfMarginRatio>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_prod_spbm_detail(
        Arc<crate::InvestorProdSPBMDetail>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_commodity_spmm_margin(
        Arc<crate::InvestorCommoditySPMMMargin>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_commodity_group_spmm_margin(
        Arc<crate::InvestorCommodityGroupSPMMMargin>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spmm_inst_param(
        Arc<crate::SPMMInstParam>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spmm_product_param(
        Arc<crate::SPMMProductParam>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_spbm_add_on_inter_parameter(
        Arc<crate::SPBMAddOnInterParameter>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rcams_comb_product_info(
        Arc<crate::RCAMSCombProductInfo>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rcams_instr_parameter(
        Arc<crate::RCAMSInstrParameter>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rcams_intra_parameter(
        Arc<crate::RCAMSIntraParameter>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rcams_inter_parameter(
        Arc<crate::RCAMSInterParameter>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rcams_short_opt_adjust_param(
        Arc<crate::RCAMSShortOptAdjustParam>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rcams_investor_comb_position(
        Arc<crate::RCAMSInvestorCombPosition>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_prod_rcams_margin(
        Arc<crate::InvestorProdRCAMSMargin>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rule_instr_parameter(
        Arc<crate::RULEInstrParameter>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rule_intra_parameter(
        Arc<crate::RULEIntraParameter>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_rule_inter_parameter(
        Arc<crate::RULEInterParameter>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_prod_rule_margin(
        Arc<crate::InvestorProdRULEMargin>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_portf_setting(
        Arc<crate::InvestorPortfSetting>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_investor_info_comm_rec(
        Arc<crate::InvestorInfoCommRec>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_qry_comb_leg(
        Arc<crate::CombLeg>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_offset_setting(
        Arc<crate::InputOffsetSetting>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rsp_cancel_offset_setting(
        Arc<crate::InputOffsetSetting>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
    on_rtn_offset_setting(Arc<crate::OffsetSetting>),
    on_err_rtn_offset_setting(
        Arc<crate::InputOffsetSetting>,
        Arc<crate::RspInfo>,
    ),
    on_err_rtn_cancel_offset_setting(
        Arc<crate::CancelOffsetSetting>,
        Arc<crate::RspInfo>,
    ),
    on_rsp_qry_offset_setting(
        Arc<crate::OffsetSetting>,
        Arc<crate::RspInfo>,
        i32,
        bool,
    ),
}

#[derive(Clone)]
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
                Arc::new(rsp_authenticate),
                Arc::new(rsp_info),
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
                Arc::new(rsp_user_login),
                Arc::new(rsp_info),
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
                Arc::new(user_logout),
                Arc::new(rsp_info),
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
                Arc::new(user_password_update),
                Arc::new(rsp_info),
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
                Arc::new(trading_account_password_update),
                Arc::new(rsp_info),
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
                Arc::new(rsp_user_auth_method),
                Arc::new(rsp_info),
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
                Arc::new(rsp_gen_user_captcha),
                Arc::new(rsp_info),
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
                Arc::new(rsp_gen_user_text),
                Arc::new(rsp_info),
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
                Arc::new(input_order),
                Arc::new(rsp_info),
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
                Arc::new(parked_order),
                Arc::new(rsp_info),
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
                Arc::new(parked_order_action),
                Arc::new(rsp_info),
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
                Arc::new(input_order_action),
                Arc::new(rsp_info),
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
                Arc::new(qry_max_order_volume),
                Arc::new(rsp_info),
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
                Arc::new(settlement_info_confirm),
                Arc::new(rsp_info),
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
                Arc::new(remove_parked_order),
                Arc::new(rsp_info),
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
                Arc::new(remove_parked_order_action),
                Arc::new(rsp_info),
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
                Arc::new(input_exec_order),
                Arc::new(rsp_info),
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
                Arc::new(input_exec_order_action),
                Arc::new(rsp_info),
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
                Arc::new(input_for_quote),
                Arc::new(rsp_info),
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
                Arc::new(input_quote),
                Arc::new(rsp_info),
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
                Arc::new(input_quote_action),
                Arc::new(rsp_info),
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
                Arc::new(input_batch_order_action),
                Arc::new(rsp_info),
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
                Arc::new(input_option_self_close),
                Arc::new(rsp_info),
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
                Arc::new(input_option_self_close_action),
                Arc::new(rsp_info),
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
                Arc::new(input_comb_action),
                Arc::new(rsp_info),
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
                Arc::new(order),
                Arc::new(rsp_info),
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
                Arc::new(trade),
                Arc::new(rsp_info),
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
                Arc::new(investor_position),
                Arc::new(rsp_info),
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
                Arc::new(trading_account),
                Arc::new(rsp_info),
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
                Arc::new(investor),
                Arc::new(rsp_info),
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
                Arc::new(trading_code),
                Arc::new(rsp_info),
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
                Arc::new(instrument_margin_rate),
                Arc::new(rsp_info),
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
                Arc::new(instrument_commission_rate),
                Arc::new(rsp_info),
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
                Arc::new(user_session),
                Arc::new(rsp_info),
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
                Arc::new(exchange),
                Arc::new(rsp_info),
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
                Arc::new(product),
                Arc::new(rsp_info),
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
                Arc::new(instrument),
                Arc::new(rsp_info),
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
                Arc::new(depth_market_data),
                Arc::new(rsp_info),
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
                Arc::new(trader_offer),
                Arc::new(rsp_info),
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
                Arc::new(settlement_info),
                Arc::new(rsp_info),
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
                Arc::new(transfer_bank),
                Arc::new(rsp_info),
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
                Arc::new(investor_position_detail),
                Arc::new(rsp_info),
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
                Arc::new(notice),
                Arc::new(rsp_info),
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
                Arc::new(settlement_info_confirm),
                Arc::new(rsp_info),
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
                Arc::new(investor_position_combine_detail),
                Arc::new(rsp_info),
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
                Arc::new(cfmmc_trading_account_key),
                Arc::new(rsp_info),
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
                Arc::new(ewarrant_offset),
                Arc::new(rsp_info),
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
                Arc::new(investor_product_group_margin),
                Arc::new(rsp_info),
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
                Arc::new(exchange_margin_rate),
                Arc::new(rsp_info),
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
                Arc::new(exchange_margin_rate_adjust),
                Arc::new(rsp_info),
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
                Arc::new(exchange_rate),
                Arc::new(rsp_info),
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
                Arc::new(sec_agent_acid_map),
                Arc::new(rsp_info),
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
                Arc::new(product_exch_rate),
                Arc::new(rsp_info),
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
                Arc::new(product_group),
                Arc::new(rsp_info),
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
                Arc::new(mm_instrument_commission_rate),
                Arc::new(rsp_info),
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
                Arc::new(mm_option_instr_comm_rate),
                Arc::new(rsp_info),
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
                Arc::new(instrument_order_comm_rate),
                Arc::new(rsp_info),
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
                Arc::new(trading_account),
                Arc::new(rsp_info),
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
                Arc::new(sec_agent_check_mode),
                Arc::new(rsp_info),
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
                Arc::new(sec_agent_trade_info),
                Arc::new(rsp_info),
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
                Arc::new(option_instr_trade_cost),
                Arc::new(rsp_info),
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
                Arc::new(option_instr_comm_rate),
                Arc::new(rsp_info),
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
                Arc::new(exec_order),
                Arc::new(rsp_info),
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
                Arc::new(for_quote),
                Arc::new(rsp_info),
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
                Arc::new(quote),
                Arc::new(rsp_info),
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
                Arc::new(option_self_close),
                Arc::new(rsp_info),
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
                Arc::new(invest_unit),
                Arc::new(rsp_info),
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
                Arc::new(comb_instrument_guard),
                Arc::new(rsp_info),
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
                Arc::new(comb_action),
                Arc::new(rsp_info),
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
                Arc::new(transfer_serial),
                Arc::new(rsp_info),
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
                Arc::new(account_register),
                Arc::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rsp_error(
        &self,
        rsp_info: crate::RspInfo,
        request_id: i32,
        is_last: bool,
    ) {
        self.tx
            .send(TraderMsg::on_rsp_error(
                Arc::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rtn_order(
        &self,
        order: crate::Order,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_order(Arc::new(order)))
            .ok();
    }
    pub fn on_rtn_trade(
        &self,
        trade: crate::Trade,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_trade(Arc::new(trade)))
            .ok();
    }
    pub fn on_err_rtn_order_insert(
        &self,
        input_order: crate::InputOrder,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_order_insert(
                Arc::new(input_order),
                Arc::new(rsp_info),
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
                Arc::new(order_action),
                Arc::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_rtn_instrument_status(
        &self,
        instrument_status: crate::InstrumentStatus,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_instrument_status(Arc::new(instrument_status)))
            .ok();
    }
    pub fn on_rtn_bulletin(
        &self,
        bulletin: crate::Bulletin,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_bulletin(Arc::new(bulletin)))
            .ok();
    }
    pub fn on_rtn_trading_notice(
        &self,
        trading_notice_info: crate::TradingNoticeInfo,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_trading_notice(Arc::new(trading_notice_info)))
            .ok();
    }
    pub fn on_rtn_error_conditional_order(
        &self,
        error_conditional_order: crate::ErrorConditionalOrder,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_error_conditional_order(Arc::new(error_conditional_order)))
            .ok();
    }
    pub fn on_rtn_exec_order(
        &self,
        exec_order: crate::ExecOrder,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_exec_order(Arc::new(exec_order)))
            .ok();
    }
    pub fn on_err_rtn_exec_order_insert(
        &self,
        input_exec_order: crate::InputExecOrder,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_exec_order_insert(
                Arc::new(input_exec_order),
                Arc::new(rsp_info),
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
                Arc::new(exec_order_action),
                Arc::new(rsp_info),
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
                Arc::new(input_for_quote),
                Arc::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_rtn_quote(
        &self,
        quote: crate::Quote,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_quote(Arc::new(quote)))
            .ok();
    }
    pub fn on_err_rtn_quote_insert(
        &self,
        input_quote: crate::InputQuote,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_quote_insert(
                Arc::new(input_quote),
                Arc::new(rsp_info),
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
                Arc::new(quote_action),
                Arc::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_rtn_for_quote_rsp(
        &self,
        for_quote_rsp: crate::ForQuoteRsp,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_for_quote_rsp(Arc::new(for_quote_rsp)))
            .ok();
    }
    pub fn on_rtn_cfmmc_trading_account_token(
        &self,
        cfmmc_trading_account_token: crate::CFMMCTradingAccountToken,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_cfmmc_trading_account_token(Arc::new(cfmmc_trading_account_token)))
            .ok();
    }
    pub fn on_err_rtn_batch_order_action(
        &self,
        batch_order_action: crate::BatchOrderAction,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_batch_order_action(
                Arc::new(batch_order_action),
                Arc::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_rtn_option_self_close(
        &self,
        option_self_close: crate::OptionSelfClose,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_option_self_close(Arc::new(option_self_close)))
            .ok();
    }
    pub fn on_err_rtn_option_self_close_insert(
        &self,
        input_option_self_close: crate::InputOptionSelfClose,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_option_self_close_insert(
                Arc::new(input_option_self_close),
                Arc::new(rsp_info),
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
                Arc::new(option_self_close_action),
                Arc::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_rtn_comb_action(
        &self,
        comb_action: crate::CombAction,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_comb_action(Arc::new(comb_action)))
            .ok();
    }
    pub fn on_err_rtn_comb_action_insert(
        &self,
        input_comb_action: crate::InputCombAction,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_comb_action_insert(
                Arc::new(input_comb_action),
                Arc::new(rsp_info),
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
                Arc::new(contract_bank),
                Arc::new(rsp_info),
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
                Arc::new(parked_order),
                Arc::new(rsp_info),
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
                Arc::new(parked_order_action),
                Arc::new(rsp_info),
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
                Arc::new(trading_notice),
                Arc::new(rsp_info),
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
                Arc::new(broker_trading_params),
                Arc::new(rsp_info),
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
                Arc::new(broker_trading_algos),
                Arc::new(rsp_info),
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
                Arc::new(query_cfmmc_trading_account_token),
                Arc::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rtn_from_bank_to_future_by_bank(
        &self,
        rsp_transfer: crate::RspTransfer,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_from_bank_to_future_by_bank(Arc::new(rsp_transfer)))
            .ok();
    }
    pub fn on_rtn_from_future_to_bank_by_bank(
        &self,
        rsp_transfer: crate::RspTransfer,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_from_future_to_bank_by_bank(Arc::new(rsp_transfer)))
            .ok();
    }
    pub fn on_rtn_repeal_from_bank_to_future_by_bank(
        &self,
        rsp_repeal: crate::RspRepeal,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_repeal_from_bank_to_future_by_bank(Arc::new(rsp_repeal)))
            .ok();
    }
    pub fn on_rtn_repeal_from_future_to_bank_by_bank(
        &self,
        rsp_repeal: crate::RspRepeal,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_repeal_from_future_to_bank_by_bank(Arc::new(rsp_repeal)))
            .ok();
    }
    pub fn on_rtn_from_bank_to_future_by_future(
        &self,
        rsp_transfer: crate::RspTransfer,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_from_bank_to_future_by_future(Arc::new(rsp_transfer)))
            .ok();
    }
    pub fn on_rtn_from_future_to_bank_by_future(
        &self,
        rsp_transfer: crate::RspTransfer,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_from_future_to_bank_by_future(Arc::new(rsp_transfer)))
            .ok();
    }
    pub fn on_rtn_repeal_from_bank_to_future_by_future_manual(
        &self,
        rsp_repeal: crate::RspRepeal,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_repeal_from_bank_to_future_by_future_manual(Arc::new(rsp_repeal)))
            .ok();
    }
    pub fn on_rtn_repeal_from_future_to_bank_by_future_manual(
        &self,
        rsp_repeal: crate::RspRepeal,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_repeal_from_future_to_bank_by_future_manual(Arc::new(rsp_repeal)))
            .ok();
    }
    pub fn on_rtn_query_bank_balance_by_future(
        &self,
        notify_query_account: crate::NotifyQueryAccount,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_query_bank_balance_by_future(Arc::new(notify_query_account)))
            .ok();
    }
    pub fn on_err_rtn_bank_to_future_by_future(
        &self,
        req_transfer: crate::ReqTransfer,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_bank_to_future_by_future(
                Arc::new(req_transfer),
                Arc::new(rsp_info),
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
                Arc::new(req_transfer),
                Arc::new(rsp_info),
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
                Arc::new(req_repeal),
                Arc::new(rsp_info),
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
                Arc::new(req_repeal),
                Arc::new(rsp_info),
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
                Arc::new(req_query_account),
                Arc::new(rsp_info),
            ))
            .ok();
    }
    pub fn on_rtn_repeal_from_bank_to_future_by_future(
        &self,
        rsp_repeal: crate::RspRepeal,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_repeal_from_bank_to_future_by_future(Arc::new(rsp_repeal)))
            .ok();
    }
    pub fn on_rtn_repeal_from_future_to_bank_by_future(
        &self,
        rsp_repeal: crate::RspRepeal,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_repeal_from_future_to_bank_by_future(Arc::new(rsp_repeal)))
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
                Arc::new(req_transfer),
                Arc::new(rsp_info),
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
                Arc::new(req_transfer),
                Arc::new(rsp_info),
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
                Arc::new(req_query_account),
                Arc::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rtn_open_account_by_bank(
        &self,
        open_account: crate::OpenAccount,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_open_account_by_bank(Arc::new(open_account)))
            .ok();
    }
    pub fn on_rtn_cancel_account_by_bank(
        &self,
        cancel_account: crate::CancelAccount,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_cancel_account_by_bank(Arc::new(cancel_account)))
            .ok();
    }
    pub fn on_rtn_change_account_by_bank(
        &self,
        change_account: crate::ChangeAccount,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_change_account_by_bank(Arc::new(change_account)))
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
                Arc::new(instrument),
                Arc::new(rsp_info),
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
                Arc::new(comb_promotion_param),
                Arc::new(rsp_info),
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
                Arc::new(risk_settle_invest_position),
                Arc::new(rsp_info),
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
                Arc::new(risk_settle_product_status),
                Arc::new(rsp_info),
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
                Arc::new(spbm_future_parameter),
                Arc::new(rsp_info),
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
                Arc::new(spbm_option_parameter),
                Arc::new(rsp_info),
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
                Arc::new(spbm_intra_parameter),
                Arc::new(rsp_info),
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
                Arc::new(spbm_inter_parameter),
                Arc::new(rsp_info),
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
                Arc::new(spbm_portf_definition),
                Arc::new(rsp_info),
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
                Arc::new(spbm_investor_portf_def),
                Arc::new(rsp_info),
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
                Arc::new(investor_portf_margin_ratio),
                Arc::new(rsp_info),
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
                Arc::new(investor_prod_spbm_detail),
                Arc::new(rsp_info),
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
                Arc::new(investor_commodity_spmm_margin),
                Arc::new(rsp_info),
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
                Arc::new(investor_commodity_group_spmm_margin),
                Arc::new(rsp_info),
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
                Arc::new(spmm_inst_param),
                Arc::new(rsp_info),
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
                Arc::new(spmm_product_param),
                Arc::new(rsp_info),
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
                Arc::new(spbm_add_on_inter_parameter),
                Arc::new(rsp_info),
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
                Arc::new(rcams_comb_product_info),
                Arc::new(rsp_info),
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
                Arc::new(rcams_instr_parameter),
                Arc::new(rsp_info),
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
                Arc::new(rcams_intra_parameter),
                Arc::new(rsp_info),
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
                Arc::new(rcams_inter_parameter),
                Arc::new(rsp_info),
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
                Arc::new(rcams_short_opt_adjust_param),
                Arc::new(rsp_info),
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
                Arc::new(rcams_investor_comb_position),
                Arc::new(rsp_info),
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
                Arc::new(investor_prod_rcams_margin),
                Arc::new(rsp_info),
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
                Arc::new(rule_instr_parameter),
                Arc::new(rsp_info),
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
                Arc::new(rule_intra_parameter),
                Arc::new(rsp_info),
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
                Arc::new(rule_inter_parameter),
                Arc::new(rsp_info),
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
                Arc::new(investor_prod_rule_margin),
                Arc::new(rsp_info),
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
                Arc::new(investor_portf_setting),
                Arc::new(rsp_info),
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
                Arc::new(investor_info_comm_rec),
                Arc::new(rsp_info),
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
                Arc::new(comb_leg),
                Arc::new(rsp_info),
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
                Arc::new(input_offset_setting),
                Arc::new(rsp_info),
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
                Arc::new(input_offset_setting),
                Arc::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
    pub fn on_rtn_offset_setting(
        &self,
        offset_setting: crate::OffsetSetting,
    ) {
        self.tx
            .send(TraderMsg::on_rtn_offset_setting(Arc::new(offset_setting)))
            .ok();
    }
    pub fn on_err_rtn_offset_setting(
        &self,
        input_offset_setting: crate::InputOffsetSetting,
        rsp_info: crate::RspInfo,
    ) {
        self.tx
            .send(TraderMsg::on_err_rtn_offset_setting(
                Arc::new(input_offset_setting),
                Arc::new(rsp_info),
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
                Arc::new(cancel_offset_setting),
                Arc::new(rsp_info),
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
                Arc::new(offset_setting),
                Arc::new(rsp_info),
                request_id,
                is_last,
            ))
            .ok();
    }
}
