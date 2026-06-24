//! CTP（综合交易平台）C++ 接口的 Rust 绑定。
//!
//! 由官方 CTP 头文件生成；结构体字段使用符合 Rust 惯例的 snake_case 命名。
//!
//! # [`MdApi`] — 行情接口
//!
//! 通过 [`MdApi::create_md_api_and_spi`] 创建实例，传入一个
//! [`Sender`] 用于接收 [`MdMsg`] 回调消息。
//!
//! # [`TraderApi`] — 交易接口
//!
//! 通过 [`TraderApi::create_trader_api_and_spi`] 创建实例，传入一个
//! [`Sender`] 用于接收 [`TraderMsg`] 回调消息。




pub use ffi::*;
mod enums;
pub use enums::*;
mod md;
pub use md::{MdMsg, MdSpi};
mod trader;
pub use trader::{TraderMsg, TraderSpi};

pub type UniquePtr<T> = cxx::UniquePtr<T>;

#[cxx::bridge]
mod ffi {
    extern "Rust" {
        type MdSpi;

        pub fn on_front_connected(&self);
        pub fn on_front_disconnected(&self, reason:  i32);
        pub fn on_heart_beat_warning(&self, time_lapse:  i32);
        pub fn on_rsp_user_login(&self, rsp_user_login:  RspUserLogin, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_user_logout(&self, user_logout:  UserLogout, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_multicast_instrument(&self, multicast_instrument:  MulticastInstrument, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_error(&self, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_sub_market_data(&self, specific_instrument:  SpecificInstrument, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_un_sub_market_data(&self, specific_instrument:  SpecificInstrument, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_sub_for_quote_rsp(&self, specific_instrument:  SpecificInstrument, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_un_sub_for_quote_rsp(&self, specific_instrument:  SpecificInstrument, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rtn_depth_market_data(&self, depth_market_data:  DepthMarketData);
        pub fn on_rtn_for_quote_rsp(&self, for_quote_rsp:  ForQuoteRsp);
    }

    unsafe extern "C++" {
        include!("ctp-rs/wrapper/include/MdApi.h");
        type MdApi;

        fn CreateMdApi(spi: Box<MdSpi>, flow_path: String, is_using_udp: bool, is_multicast: bool, is_production_mode: bool) -> UniquePtr<MdApi>;

        /// 获取API的版本信息
        ///
        /// # Returns
        /// 获取到的版本号
        fn get_api_version(&self)-> String;
        /// 初始化
        ///
        /// # Remarks
        /// 初始化运行环境,只有调用后,接口才开始工作
        fn init(&self);
        /// 等待接口线程结束运行
        ///
        /// # Returns
        /// 线程退出代码
        fn join(&self)-> i32;
        /// 获取当前交易日
        ///
        /// # Returns
        /// 获取到的交易日
        ///
        /// # Remarks
        /// 只有登录成功后,才能得到正确的交易日
        fn get_trading_day(&self)-> String;
        /// 注册前置机网络地址
        ///
        /// # Parameters
        /// - `pszFrontAddress` — 前置机网络地址。
        ///
        /// # Remarks
        /// 网络地址的格式为：“protocol://ip_address:port”，如：”tcp://127.0.0.1:17001”。
        /// “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
        fn register_front(&self, front_address:  String);
        /// 注册名字服务器网络地址
        ///
        /// # Parameters
        /// - `pszNsAddress` — 名字服务器网络地址。
        ///
        /// # Remarks
        /// 网络地址的格式为：“protocol://ip_address:port”，如：”tcp://127.0.0.1:12001”。
        /// “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”12001”代表服务器端口号。
        /// RegisterNameServer优先于RegisterFront
        fn register_name_server(&self, ns_address:  String);
        /// 注册名字服务器用户信息
        ///
        /// # Parameters
        /// - `pFensUserInfo` — 用户信息。
        fn register_fens_user_info(&self, fens_user_info:  FensUserInfo);
        /// 订阅行情。
        ///
        /// # Parameters
        /// - `ppInstrumentID` — 合约ID
        /// - `nCount` — 要订阅/退订行情的合约个数
        fn subscribe_market_data(&self, instrument_ids:  Vec<String>)-> i32;
        /// 退订行情。
        ///
        /// # Parameters
        /// - `ppInstrumentID` — 合约ID
        /// - `nCount` — 要订阅/退订行情的合约个数
        fn un_subscribe_market_data(&self, instrument_ids:  Vec<String>)-> i32;
        /// 订阅询价。
        ///
        /// # Parameters
        /// - `ppInstrumentID` — 合约ID
        /// - `nCount` — 要订阅/退订行情的合约个数
        fn subscribe_for_quote_rsp(&self, instrument_ids:  Vec<String>)-> i32;
        /// 退订询价。
        ///
        /// # Parameters
        /// - `ppInstrumentID` — 合约ID
        /// - `nCount` — 要订阅/退订行情的合约个数
        fn un_subscribe_for_quote_rsp(&self, instrument_ids:  Vec<String>)-> i32;
        /// 用户登录请求
        fn req_user_login(&self, req_user_login:  ReqUserLogin, request_id:  i32)-> i32;
        /// 登出请求
        fn req_user_logout(&self, user_logout:  UserLogout, request_id:  i32)-> i32;
        /// 请求查询组播合约
        fn req_qry_multicast_instrument(&self, qry_multicast_instrument:  QryMulticastInstrument, request_id:  i32)-> i32;
    }

    extern "Rust" {
        type TraderSpi;

        pub fn on_front_connected(&self);
        pub fn on_front_disconnected(&self, reason:  i32);
        pub fn on_heart_beat_warning(&self, time_lapse:  i32);
        pub fn on_rsp_authenticate(&self, rsp_authenticate:  RspAuthenticate, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_user_login(&self, rsp_user_login:  RspUserLogin, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_user_logout(&self, user_logout:  UserLogout, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_user_password_update(&self, user_password_update:  UserPasswordUpdate, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_trading_account_password_update(&self, trading_account_password_update:  TradingAccountPasswordUpdate, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_user_auth_method(&self, rsp_user_auth_method:  RspUserAuthMethod, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_gen_user_captcha(&self, rsp_gen_user_captcha:  RspGenUserCaptcha, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_gen_user_text(&self, rsp_gen_user_text:  RspGenUserText, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_order_insert(&self, input_order:  InputOrder, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_parked_order_insert(&self, parked_order:  ParkedOrder, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_parked_order_action(&self, parked_order_action:  ParkedOrderAction, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_order_action(&self, input_order_action:  InputOrderAction, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_max_order_volume(&self, qry_max_order_volume:  QryMaxOrderVolume, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_settlement_info_confirm(&self, settlement_info_confirm:  SettlementInfoConfirm, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_remove_parked_order(&self, remove_parked_order:  RemoveParkedOrder, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_remove_parked_order_action(&self, remove_parked_order_action:  RemoveParkedOrderAction, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_exec_order_insert(&self, input_exec_order:  InputExecOrder, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_exec_order_action(&self, input_exec_order_action:  InputExecOrderAction, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_for_quote_insert(&self, input_for_quote:  InputForQuote, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_quote_insert(&self, input_quote:  InputQuote, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_quote_action(&self, input_quote_action:  InputQuoteAction, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_batch_order_action(&self, input_batch_order_action:  InputBatchOrderAction, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_option_self_close_insert(&self, input_option_self_close:  InputOptionSelfClose, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_option_self_close_action(&self, input_option_self_close_action:  InputOptionSelfCloseAction, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_comb_action_insert(&self, input_comb_action:  InputCombAction, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_order(&self, order:  Order, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_trade(&self, trade:  Trade, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_investor_position(&self, investor_position:  InvestorPosition, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_trading_account(&self, trading_account:  TradingAccount, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_investor(&self, investor:  Investor, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_trading_code(&self, trading_code:  TradingCode, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_instrument_margin_rate(&self, instrument_margin_rate:  InstrumentMarginRate, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_instrument_commission_rate(&self, instrument_commission_rate:  InstrumentCommissionRate, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_user_session(&self, user_session:  UserSession, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_exchange(&self, exchange:  Exchange, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_product(&self, product:  Product, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_instrument(&self, instrument:  Instrument, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_depth_market_data(&self, depth_market_data:  DepthMarketData, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_trader_offer(&self, trader_offer:  TraderOffer, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_settlement_info(&self, settlement_info:  SettlementInfo, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_transfer_bank(&self, transfer_bank:  TransferBank, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_investor_position_detail(&self, investor_position_detail:  InvestorPositionDetail, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_notice(&self, notice:  Notice, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_settlement_info_confirm(&self, settlement_info_confirm:  SettlementInfoConfirm, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_investor_position_combine_detail(&self, investor_position_combine_detail:  InvestorPositionCombineDetail, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_cfmmc_trading_account_key(&self, cfmmc_trading_account_key:  CFMMCTradingAccountKey, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_ewarrant_offset(&self, ewarrant_offset:  EWarrantOffset, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_investor_product_group_margin(&self, investor_product_group_margin:  InvestorProductGroupMargin, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_exchange_margin_rate(&self, exchange_margin_rate:  ExchangeMarginRate, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_exchange_margin_rate_adjust(&self, exchange_margin_rate_adjust:  ExchangeMarginRateAdjust, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_exchange_rate(&self, exchange_rate:  ExchangeRate, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_sec_agent_acid_map(&self, sec_agent_acid_map:  SecAgentACIDMap, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_product_exch_rate(&self, product_exch_rate:  ProductExchRate, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_product_group(&self, product_group:  ProductGroup, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_mm_instrument_commission_rate(&self, mm_instrument_commission_rate:  MMInstrumentCommissionRate, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_mm_option_instr_comm_rate(&self, mm_option_instr_comm_rate:  MMOptionInstrCommRate, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_instrument_order_comm_rate(&self, instrument_order_comm_rate:  InstrumentOrderCommRate, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_sec_agent_trading_account(&self, trading_account:  TradingAccount, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_sec_agent_check_mode(&self, sec_agent_check_mode:  SecAgentCheckMode, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_sec_agent_trade_info(&self, sec_agent_trade_info:  SecAgentTradeInfo, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_option_instr_trade_cost(&self, option_instr_trade_cost:  OptionInstrTradeCost, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_option_instr_comm_rate(&self, option_instr_comm_rate:  OptionInstrCommRate, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_exec_order(&self, exec_order:  ExecOrder, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_for_quote(&self, for_quote:  ForQuote, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_quote(&self, quote:  Quote, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_option_self_close(&self, option_self_close:  OptionSelfClose, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_invest_unit(&self, invest_unit:  InvestUnit, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_comb_instrument_guard(&self, comb_instrument_guard:  CombInstrumentGuard, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_comb_action(&self, comb_action:  CombAction, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_transfer_serial(&self, transfer_serial:  TransferSerial, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_account_register(&self, account_register:  AccountRegister, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_error(&self, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rtn_order(&self, order:  Order);
        pub fn on_rtn_trade(&self, trade:  Trade);
        pub fn on_err_rtn_order_insert(&self, input_order:  InputOrder, rsp_info:  RspInfo);
        pub fn on_err_rtn_order_action(&self, order_action:  OrderAction, rsp_info:  RspInfo);
        pub fn on_rtn_instrument_status(&self, instrument_status:  InstrumentStatus);
        pub fn on_rtn_bulletin(&self, bulletin:  Bulletin);
        pub fn on_rtn_trading_notice(&self, trading_notice_info:  TradingNoticeInfo);
        pub fn on_rtn_error_conditional_order(&self, error_conditional_order:  ErrorConditionalOrder);
        pub fn on_rtn_exec_order(&self, exec_order:  ExecOrder);
        pub fn on_err_rtn_exec_order_insert(&self, input_exec_order:  InputExecOrder, rsp_info:  RspInfo);
        pub fn on_err_rtn_exec_order_action(&self, exec_order_action:  ExecOrderAction, rsp_info:  RspInfo);
        pub fn on_err_rtn_for_quote_insert(&self, input_for_quote:  InputForQuote, rsp_info:  RspInfo);
        pub fn on_rtn_quote(&self, quote:  Quote);
        pub fn on_err_rtn_quote_insert(&self, input_quote:  InputQuote, rsp_info:  RspInfo);
        pub fn on_err_rtn_quote_action(&self, quote_action:  QuoteAction, rsp_info:  RspInfo);
        pub fn on_rtn_for_quote_rsp(&self, for_quote_rsp:  ForQuoteRsp);
        pub fn on_rtn_cfmmc_trading_account_token(&self, cfmmc_trading_account_token:  CFMMCTradingAccountToken);
        pub fn on_err_rtn_batch_order_action(&self, batch_order_action:  BatchOrderAction, rsp_info:  RspInfo);
        pub fn on_rtn_option_self_close(&self, option_self_close:  OptionSelfClose);
        pub fn on_err_rtn_option_self_close_insert(&self, input_option_self_close:  InputOptionSelfClose, rsp_info:  RspInfo);
        pub fn on_err_rtn_option_self_close_action(&self, option_self_close_action:  OptionSelfCloseAction, rsp_info:  RspInfo);
        pub fn on_rtn_comb_action(&self, comb_action:  CombAction);
        pub fn on_err_rtn_comb_action_insert(&self, input_comb_action:  InputCombAction, rsp_info:  RspInfo);
        pub fn on_rsp_qry_contract_bank(&self, contract_bank:  ContractBank, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_parked_order(&self, parked_order:  ParkedOrder, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_parked_order_action(&self, parked_order_action:  ParkedOrderAction, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_trading_notice(&self, trading_notice:  TradingNotice, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_broker_trading_params(&self, broker_trading_params:  BrokerTradingParams, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_broker_trading_algos(&self, broker_trading_algos:  BrokerTradingAlgos, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_query_cfmmc_trading_account_token(&self, query_cfmmc_trading_account_token:  QueryCFMMCTradingAccountToken, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rtn_from_bank_to_future_by_bank(&self, rsp_transfer:  RspTransfer);
        pub fn on_rtn_from_future_to_bank_by_bank(&self, rsp_transfer:  RspTransfer);
        pub fn on_rtn_repeal_from_bank_to_future_by_bank(&self, rsp_repeal:  RspRepeal);
        pub fn on_rtn_repeal_from_future_to_bank_by_bank(&self, rsp_repeal:  RspRepeal);
        pub fn on_rtn_from_bank_to_future_by_future(&self, rsp_transfer:  RspTransfer);
        pub fn on_rtn_from_future_to_bank_by_future(&self, rsp_transfer:  RspTransfer);
        pub fn on_rtn_repeal_from_bank_to_future_by_future_manual(&self, rsp_repeal:  RspRepeal);
        pub fn on_rtn_repeal_from_future_to_bank_by_future_manual(&self, rsp_repeal:  RspRepeal);
        pub fn on_rtn_query_bank_balance_by_future(&self, notify_query_account:  NotifyQueryAccount);
        pub fn on_err_rtn_bank_to_future_by_future(&self, req_transfer:  ReqTransfer, rsp_info:  RspInfo);
        pub fn on_err_rtn_future_to_bank_by_future(&self, req_transfer:  ReqTransfer, rsp_info:  RspInfo);
        pub fn on_err_rtn_repeal_bank_to_future_by_future_manual(&self, req_repeal:  ReqRepeal, rsp_info:  RspInfo);
        pub fn on_err_rtn_repeal_future_to_bank_by_future_manual(&self, req_repeal:  ReqRepeal, rsp_info:  RspInfo);
        pub fn on_err_rtn_query_bank_balance_by_future(&self, req_query_account:  ReqQueryAccount, rsp_info:  RspInfo);
        pub fn on_rtn_repeal_from_bank_to_future_by_future(&self, rsp_repeal:  RspRepeal);
        pub fn on_rtn_repeal_from_future_to_bank_by_future(&self, rsp_repeal:  RspRepeal);
        pub fn on_rsp_from_bank_to_future_by_future(&self, req_transfer:  ReqTransfer, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_from_future_to_bank_by_future(&self, req_transfer:  ReqTransfer, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_query_bank_account_money_by_future(&self, req_query_account:  ReqQueryAccount, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rtn_open_account_by_bank(&self, open_account:  OpenAccount);
        pub fn on_rtn_cancel_account_by_bank(&self, cancel_account:  CancelAccount);
        pub fn on_rtn_change_account_by_bank(&self, change_account:  ChangeAccount);
        pub fn on_rsp_qry_classified_instrument(&self, instrument:  Instrument, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_comb_promotion_param(&self, comb_promotion_param:  CombPromotionParam, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_risk_settle_invest_position(&self, risk_settle_invest_position:  RiskSettleInvestPosition, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_risk_settle_product_status(&self, risk_settle_product_status:  RiskSettleProductStatus, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_spbm_future_parameter(&self, spbm_future_parameter:  SPBMFutureParameter, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_spbm_option_parameter(&self, spbm_option_parameter:  SPBMOptionParameter, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_spbm_intra_parameter(&self, spbm_intra_parameter:  SPBMIntraParameter, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_spbm_inter_parameter(&self, spbm_inter_parameter:  SPBMInterParameter, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_spbm_portf_definition(&self, spbm_portf_definition:  SPBMPortfDefinition, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_spbm_investor_portf_def(&self, spbm_investor_portf_def:  SPBMInvestorPortfDef, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_investor_portf_margin_ratio(&self, investor_portf_margin_ratio:  InvestorPortfMarginRatio, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_investor_prod_spbm_detail(&self, investor_prod_spbm_detail:  InvestorProdSPBMDetail, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_investor_commodity_spmm_margin(&self, investor_commodity_spmm_margin:  InvestorCommoditySPMMMargin, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_investor_commodity_group_spmm_margin(&self, investor_commodity_group_spmm_margin:  InvestorCommodityGroupSPMMMargin, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_spmm_inst_param(&self, spmm_inst_param:  SPMMInstParam, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_spmm_product_param(&self, spmm_product_param:  SPMMProductParam, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_spbm_add_on_inter_parameter(&self, spbm_add_on_inter_parameter:  SPBMAddOnInterParameter, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_rcams_comb_product_info(&self, rcams_comb_product_info:  RCAMSCombProductInfo, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_rcams_instr_parameter(&self, rcams_instr_parameter:  RCAMSInstrParameter, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_rcams_intra_parameter(&self, rcams_intra_parameter:  RCAMSIntraParameter, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_rcams_inter_parameter(&self, rcams_inter_parameter:  RCAMSInterParameter, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_rcams_short_opt_adjust_param(&self, rcams_short_opt_adjust_param:  RCAMSShortOptAdjustParam, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_rcams_investor_comb_position(&self, rcams_investor_comb_position:  RCAMSInvestorCombPosition, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_investor_prod_rcams_margin(&self, investor_prod_rcams_margin:  InvestorProdRCAMSMargin, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_rule_instr_parameter(&self, rule_instr_parameter:  RULEInstrParameter, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_rule_intra_parameter(&self, rule_intra_parameter:  RULEIntraParameter, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_rule_inter_parameter(&self, rule_inter_parameter:  RULEInterParameter, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_investor_prod_rule_margin(&self, investor_prod_rule_margin:  InvestorProdRULEMargin, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_investor_portf_setting(&self, investor_portf_setting:  InvestorPortfSetting, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_investor_info_comm_rec(&self, investor_info_comm_rec:  InvestorInfoCommRec, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_qry_comb_leg(&self, comb_leg:  CombLeg, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_offset_setting(&self, input_offset_setting:  InputOffsetSetting, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rsp_cancel_offset_setting(&self, input_offset_setting:  InputOffsetSetting, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
        pub fn on_rtn_offset_setting(&self, offset_setting:  OffsetSetting);
        pub fn on_err_rtn_offset_setting(&self, input_offset_setting:  InputOffsetSetting, rsp_info:  RspInfo);
        pub fn on_err_rtn_cancel_offset_setting(&self, cancel_offset_setting:  CancelOffsetSetting, rsp_info:  RspInfo);
        pub fn on_rsp_qry_offset_setting(&self, offset_setting:  OffsetSetting, rsp_info:  RspInfo, request_id:  i32, is_last: bool);
    }

    unsafe extern "C++" {
        include!("ctp-rs/wrapper/include/TraderApi.h");
        type TraderApi;

        fn CreateTraderApi(spi: Box<TraderSpi>, flow_path: String, is_production_mode: bool) -> UniquePtr<TraderApi>;
        fn GetFrontInfo(&self) -> FrontInfo;

        /// 获取API的版本信息
        ///
        /// # Returns
        /// 获取到的版本号
        fn get_api_version(&self)-> String;
        /// 初始化
        ///
        /// # Remarks
        /// 初始化运行环境,只有调用后,接口才开始工作
        fn init(&self);
        /// 等待接口线程结束运行
        ///
        /// # Returns
        /// 线程退出代码
        fn join(&self)-> i32;
        /// 获取当前交易日
        ///
        /// # Returns
        /// 获取到的交易日
        ///
        /// # Remarks
        /// 只有登录成功后,才能得到正确的交易日
        fn get_trading_day(&self)-> String;
        /// 注册前置机网络地址
        ///
        /// # Parameters
        /// - `pszFrontAddress` — 前置机网络地址。
        ///
        /// # Remarks
        /// 网络地址的格式为：“protocol://ip_address:port”，如：”tcp://127.0.0.1:17001”。
        /// “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
        fn register_front(&self, front_address:  String);
        /// 注册名字服务器网络地址
        ///
        /// # Parameters
        /// - `pszNsAddress` — 名字服务器网络地址。
        ///
        /// # Remarks
        /// 网络地址的格式为：“protocol://ip_address:port”，如：”tcp://127.0.0.1:12001”。
        /// “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”12001”代表服务器端口号。
        /// RegisterNameServer优先于RegisterFront
        fn register_name_server(&self, ns_address:  String);
        /// 注册名字服务器用户信息
        ///
        /// # Parameters
        /// - `pFensUserInfo` — 用户信息。
        fn register_fens_user_info(&self, fens_user_info:  FensUserInfo);
        /// 订阅私有流。
        ///
        /// # Parameters
        /// - `ResumeType` — 私有流重传方式
        ///   - Restart:从本交易日开始重传
        ///   - Resume:从上次收到的续传
        ///   - Quick:只传送登录后私有流的内容
        ///
        /// # Remarks
        /// 该方法要在Init方法前调用。若不调用则不会收到私有流的数据。
        fn subscribe_private_topic(&self, resume_type:  i32);
        /// 订阅公共流。
        ///
        /// # Parameters
        /// - `ResumeType` — 公共流重传方式
        ///   - Restart:从本交易日开始重传
        ///   - Resume:从上次收到的续传
        ///   - Quick:只传送登录后公共流的内容
        ///   - NONE:取消订阅公共流
        ///
        /// # Remarks
        /// 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
        fn subscribe_public_topic(&self, resume_type:  i32);
        /// 客户端认证请求
        fn req_authenticate(&self, req_authenticate:  ReqAuthenticate, request_id:  i32)-> i32;
        /// 注册用户终端信息，用于中继服务器多连接模式
        /// 需要在终端认证成功后，用户登录前调用该接口
        fn register_user_system_info(&self, user_system_info:  UserSystemInfo)-> i32;
        /// 上报用户终端信息，用于中继服务器操作员登录模式
        /// 操作员登录后，可以多次调用该接口上报客户信息
        fn submit_user_system_info(&self, user_system_info:  UserSystemInfo)-> i32;
        /// 注册用户终端信息，用于中继服务器多连接模式.用于微信小程序等应用上报信息.
        fn register_wechat_user_system_info(&self, user_system_info:  WechatUserSystemInfo)-> i32;
        /// 上报用户终端信息，用于中继服务器操作员登录模式.用于微信小程序等应用上报信息.
        fn submit_wechat_user_system_info(&self, user_system_info:  WechatUserSystemInfo)-> i32;
        /// 用户登录请求
        fn req_user_login(&self, req_user_login:  ReqUserLogin, request_id:  i32)-> i32;
        /// 登出请求
        fn req_user_logout(&self, user_logout:  UserLogout, request_id:  i32)-> i32;
        /// 用户口令更新请求
        fn req_user_password_update(&self, user_password_update:  UserPasswordUpdate, request_id:  i32)-> i32;
        /// 资金账户口令更新请求
        fn req_trading_account_password_update(&self, trading_account_password_update:  TradingAccountPasswordUpdate, request_id:  i32)-> i32;
        /// 查询用户当前支持的认证模式
        fn req_user_auth_method(&self, req_user_auth_method:  ReqUserAuthMethod, request_id:  i32)-> i32;
        /// 用户发出获取图形验证码请求
        fn req_gen_user_captcha(&self, req_gen_user_captcha:  ReqGenUserCaptcha, request_id:  i32)-> i32;
        /// 用户发出获取短信验证码请求
        fn req_gen_user_text(&self, req_gen_user_text:  ReqGenUserText, request_id:  i32)-> i32;
        /// 用户发出带有图片验证码的登陆请求
        fn req_user_login_with_captcha(&self, req_user_login_with_captcha:  ReqUserLoginWithCaptcha, request_id:  i32)-> i32;
        /// 用户发出带有短信验证码的登陆请求
        fn req_user_login_with_text(&self, req_user_login_with_text:  ReqUserLoginWithText, request_id:  i32)-> i32;
        /// 用户发出带有动态口令的登陆请求
        fn req_user_login_with_otp(&self, req_user_login_with_otp:  ReqUserLoginWithOTP, request_id:  i32)-> i32;
        /// 报单录入请求
        fn req_order_insert(&self, input_order:  InputOrder, request_id:  i32)-> i32;
        /// 预埋单录入请求
        fn req_parked_order_insert(&self, parked_order:  ParkedOrder, request_id:  i32)-> i32;
        /// 预埋撤单录入请求
        fn req_parked_order_action(&self, parked_order_action:  ParkedOrderAction, request_id:  i32)-> i32;
        /// 报单操作请求
        fn req_order_action(&self, input_order_action:  InputOrderAction, request_id:  i32)-> i32;
        /// 查询最大报单数量请求
        fn req_qry_max_order_volume(&self, qry_max_order_volume:  QryMaxOrderVolume, request_id:  i32)-> i32;
        /// 投资者结算结果确认
        fn req_settlement_info_confirm(&self, settlement_info_confirm:  SettlementInfoConfirm, request_id:  i32)-> i32;
        /// 请求删除预埋单
        fn req_remove_parked_order(&self, remove_parked_order:  RemoveParkedOrder, request_id:  i32)-> i32;
        /// 请求删除预埋撤单
        fn req_remove_parked_order_action(&self, remove_parked_order_action:  RemoveParkedOrderAction, request_id:  i32)-> i32;
        /// 执行宣告录入请求
        fn req_exec_order_insert(&self, input_exec_order:  InputExecOrder, request_id:  i32)-> i32;
        /// 执行宣告操作请求
        fn req_exec_order_action(&self, input_exec_order_action:  InputExecOrderAction, request_id:  i32)-> i32;
        /// 询价录入请求
        fn req_for_quote_insert(&self, input_for_quote:  InputForQuote, request_id:  i32)-> i32;
        /// 报价录入请求
        fn req_quote_insert(&self, input_quote:  InputQuote, request_id:  i32)-> i32;
        /// 报价操作请求
        fn req_quote_action(&self, input_quote_action:  InputQuoteAction, request_id:  i32)-> i32;
        /// 批量报单操作请求
        fn req_batch_order_action(&self, input_batch_order_action:  InputBatchOrderAction, request_id:  i32)-> i32;
        /// 期权自对冲录入请求
        fn req_option_self_close_insert(&self, input_option_self_close:  InputOptionSelfClose, request_id:  i32)-> i32;
        /// 期权自对冲操作请求
        fn req_option_self_close_action(&self, input_option_self_close_action:  InputOptionSelfCloseAction, request_id:  i32)-> i32;
        /// 申请组合录入请求
        fn req_comb_action_insert(&self, input_comb_action:  InputCombAction, request_id:  i32)-> i32;
        /// 请求查询报单
        fn req_qry_order(&self, qry_order:  QryOrder, request_id:  i32)-> i32;
        /// 请求查询成交
        fn req_qry_trade(&self, qry_trade:  QryTrade, request_id:  i32)-> i32;
        /// 请求查询投资者持仓
        fn req_qry_investor_position(&self, qry_investor_position:  QryInvestorPosition, request_id:  i32)-> i32;
        /// 请求查询资金账户
        fn req_qry_trading_account(&self, qry_trading_account:  QryTradingAccount, request_id:  i32)-> i32;
        /// 请求查询投资者
        fn req_qry_investor(&self, qry_investor:  QryInvestor, request_id:  i32)-> i32;
        /// 请求查询交易编码
        fn req_qry_trading_code(&self, qry_trading_code:  QryTradingCode, request_id:  i32)-> i32;
        /// 请求查询合约保证金率
        fn req_qry_instrument_margin_rate(&self, qry_instrument_margin_rate:  QryInstrumentMarginRate, request_id:  i32)-> i32;
        /// 请求查询合约手续费率
        fn req_qry_instrument_commission_rate(&self, qry_instrument_commission_rate:  QryInstrumentCommissionRate, request_id:  i32)-> i32;
        /// 请求查询用户会话
        fn req_qry_user_session(&self, qry_user_session:  QryUserSession, request_id:  i32)-> i32;
        /// 请求查询交易所
        fn req_qry_exchange(&self, qry_exchange:  QryExchange, request_id:  i32)-> i32;
        /// 请求查询产品
        fn req_qry_product(&self, qry_product:  QryProduct, request_id:  i32)-> i32;
        /// 请求查询合约
        fn req_qry_instrument(&self, qry_instrument:  QryInstrument, request_id:  i32)-> i32;
        /// 请求查询行情
        fn req_qry_depth_market_data(&self, qry_depth_market_data:  QryDepthMarketData, request_id:  i32)-> i32;
        /// 请求查询交易员报盘机
        fn req_qry_trader_offer(&self, qry_trader_offer:  QryTraderOffer, request_id:  i32)-> i32;
        /// 请求查询投资者结算结果
        fn req_qry_settlement_info(&self, qry_settlement_info:  QrySettlementInfo, request_id:  i32)-> i32;
        /// 请求查询转帐银行
        fn req_qry_transfer_bank(&self, qry_transfer_bank:  QryTransferBank, request_id:  i32)-> i32;
        /// 请求查询投资者持仓明细
        fn req_qry_investor_position_detail(&self, qry_investor_position_detail:  QryInvestorPositionDetail, request_id:  i32)-> i32;
        /// 请求查询客户通知
        fn req_qry_notice(&self, qry_notice:  QryNotice, request_id:  i32)-> i32;
        /// 请求查询结算信息确认
        fn req_qry_settlement_info_confirm(&self, qry_settlement_info_confirm:  QrySettlementInfoConfirm, request_id:  i32)-> i32;
        /// 请求查询投资者持仓明细
        fn req_qry_investor_position_combine_detail(&self, qry_investor_position_combine_detail:  QryInvestorPositionCombineDetail, request_id:  i32)-> i32;
        /// 请求查询保证金监管系统经纪公司资金账户密钥
        fn req_qry_cfmmc_trading_account_key(&self, qry_cfmmc_trading_account_key:  QryCFMMCTradingAccountKey, request_id:  i32)-> i32;
        /// 请求查询仓单折抵信息
        fn req_qry_ewarrant_offset(&self, qry_ewarrant_offset:  QryEWarrantOffset, request_id:  i32)-> i32;
        /// 请求查询投资者品种/跨品种保证金
        fn req_qry_investor_product_group_margin(&self, qry_investor_product_group_margin:  QryInvestorProductGroupMargin, request_id:  i32)-> i32;
        /// 请求查询交易所保证金率
        fn req_qry_exchange_margin_rate(&self, qry_exchange_margin_rate:  QryExchangeMarginRate, request_id:  i32)-> i32;
        /// 请求查询交易所调整保证金率
        fn req_qry_exchange_margin_rate_adjust(&self, qry_exchange_margin_rate_adjust:  QryExchangeMarginRateAdjust, request_id:  i32)-> i32;
        /// 请求查询汇率
        fn req_qry_exchange_rate(&self, qry_exchange_rate:  QryExchangeRate, request_id:  i32)-> i32;
        /// 请求查询二级代理操作员银期权限
        fn req_qry_sec_agent_acid_map(&self, qry_sec_agent_acid_map:  QrySecAgentACIDMap, request_id:  i32)-> i32;
        /// 请求查询产品报价汇率
        fn req_qry_product_exch_rate(&self, qry_product_exch_rate:  QryProductExchRate, request_id:  i32)-> i32;
        /// 请求查询产品组
        fn req_qry_product_group(&self, qry_product_group:  QryProductGroup, request_id:  i32)-> i32;
        /// 请求查询做市商合约手续费率
        fn req_qry_mm_instrument_commission_rate(&self, qry_mm_instrument_commission_rate:  QryMMInstrumentCommissionRate, request_id:  i32)-> i32;
        /// 请求查询做市商期权合约手续费
        fn req_qry_mm_option_instr_comm_rate(&self, qry_mm_option_instr_comm_rate:  QryMMOptionInstrCommRate, request_id:  i32)-> i32;
        /// 请求查询报单手续费
        fn req_qry_instrument_order_comm_rate(&self, qry_instrument_order_comm_rate:  QryInstrumentOrderCommRate, request_id:  i32)-> i32;
        /// 请求查询资金账户
        fn req_qry_sec_agent_trading_account(&self, qry_trading_account:  QryTradingAccount, request_id:  i32)-> i32;
        /// 请求查询二级代理商资金校验模式
        fn req_qry_sec_agent_check_mode(&self, qry_sec_agent_check_mode:  QrySecAgentCheckMode, request_id:  i32)-> i32;
        /// 请求查询二级代理商信息
        fn req_qry_sec_agent_trade_info(&self, qry_sec_agent_trade_info:  QrySecAgentTradeInfo, request_id:  i32)-> i32;
        /// 请求查询期权交易成本
        fn req_qry_option_instr_trade_cost(&self, qry_option_instr_trade_cost:  QryOptionInstrTradeCost, request_id:  i32)-> i32;
        /// 请求查询期权合约手续费
        fn req_qry_option_instr_comm_rate(&self, qry_option_instr_comm_rate:  QryOptionInstrCommRate, request_id:  i32)-> i32;
        /// 请求查询执行宣告
        fn req_qry_exec_order(&self, qry_exec_order:  QryExecOrder, request_id:  i32)-> i32;
        /// 请求查询询价
        fn req_qry_for_quote(&self, qry_for_quote:  QryForQuote, request_id:  i32)-> i32;
        /// 请求查询报价
        fn req_qry_quote(&self, qry_quote:  QryQuote, request_id:  i32)-> i32;
        /// 请求查询期权自对冲
        fn req_qry_option_self_close(&self, qry_option_self_close:  QryOptionSelfClose, request_id:  i32)-> i32;
        /// 请求查询投资单元
        fn req_qry_invest_unit(&self, qry_invest_unit:  QryInvestUnit, request_id:  i32)-> i32;
        /// 请求查询组合合约安全系数
        fn req_qry_comb_instrument_guard(&self, qry_comb_instrument_guard:  QryCombInstrumentGuard, request_id:  i32)-> i32;
        /// 请求查询申请组合
        fn req_qry_comb_action(&self, qry_comb_action:  QryCombAction, request_id:  i32)-> i32;
        /// 请求查询转帐流水
        fn req_qry_transfer_serial(&self, qry_transfer_serial:  QryTransferSerial, request_id:  i32)-> i32;
        /// 请求查询银期签约关系
        fn req_qry_account_register(&self, qry_account_register:  QryAccountRegister, request_id:  i32)-> i32;
        /// 请求查询签约银行
        fn req_qry_contract_bank(&self, qry_contract_bank:  QryContractBank, request_id:  i32)-> i32;
        /// 请求查询预埋单
        fn req_qry_parked_order(&self, qry_parked_order:  QryParkedOrder, request_id:  i32)-> i32;
        /// 请求查询预埋撤单
        fn req_qry_parked_order_action(&self, qry_parked_order_action:  QryParkedOrderAction, request_id:  i32)-> i32;
        /// 请求查询交易通知
        fn req_qry_trading_notice(&self, qry_trading_notice:  QryTradingNotice, request_id:  i32)-> i32;
        /// 请求查询经纪公司交易参数
        fn req_qry_broker_trading_params(&self, qry_broker_trading_params:  QryBrokerTradingParams, request_id:  i32)-> i32;
        /// 请求查询经纪公司交易算法
        fn req_qry_broker_trading_algos(&self, qry_broker_trading_algos:  QryBrokerTradingAlgos, request_id:  i32)-> i32;
        /// 请求查询监控中心用户令牌
        fn req_query_cfmmc_trading_account_token(&self, query_cfmmc_trading_account_token:  QueryCFMMCTradingAccountToken, request_id:  i32)-> i32;
        /// 期货发起银行资金转期货请求
        fn req_from_bank_to_future_by_future(&self, req_transfer:  ReqTransfer, request_id:  i32)-> i32;
        /// 期货发起期货资金转银行请求
        fn req_from_future_to_bank_by_future(&self, req_transfer:  ReqTransfer, request_id:  i32)-> i32;
        /// 期货发起查询银行余额请求
        fn req_query_bank_account_money_by_future(&self, req_query_account:  ReqQueryAccount, request_id:  i32)-> i32;
        /// 请求查询分类合约
        fn req_qry_classified_instrument(&self, qry_classified_instrument:  QryClassifiedInstrument, request_id:  i32)-> i32;
        /// 请求组合优惠比例
        fn req_qry_comb_promotion_param(&self, qry_comb_promotion_param:  QryCombPromotionParam, request_id:  i32)-> i32;
        /// 投资者风险结算持仓查询
        fn req_qry_risk_settle_invest_position(&self, qry_risk_settle_invest_position:  QryRiskSettleInvestPosition, request_id:  i32)-> i32;
        /// 风险结算产品查询
        fn req_qry_risk_settle_product_status(&self, qry_risk_settle_product_status:  QryRiskSettleProductStatus, request_id:  i32)-> i32;
        /// SPBM期货合约参数查询
        fn req_qry_spbm_future_parameter(&self, qry_spbm_future_parameter:  QrySPBMFutureParameter, request_id:  i32)-> i32;
        /// SPBM期权合约参数查询
        fn req_qry_spbm_option_parameter(&self, qry_spbm_option_parameter:  QrySPBMOptionParameter, request_id:  i32)-> i32;
        /// SPBM品种内对锁仓折扣参数查询
        fn req_qry_spbm_intra_parameter(&self, qry_spbm_intra_parameter:  QrySPBMIntraParameter, request_id:  i32)-> i32;
        /// SPBM跨品种抵扣参数查询
        fn req_qry_spbm_inter_parameter(&self, qry_spbm_inter_parameter:  QrySPBMInterParameter, request_id:  i32)-> i32;
        /// SPBM组合保证金套餐查询
        fn req_qry_spbm_portf_definition(&self, qry_spbm_portf_definition:  QrySPBMPortfDefinition, request_id:  i32)-> i32;
        /// 投资者SPBM套餐选择查询
        fn req_qry_spbm_investor_portf_def(&self, qry_spbm_investor_portf_def:  QrySPBMInvestorPortfDef, request_id:  i32)-> i32;
        /// 投资者新型组合保证金系数查询
        fn req_qry_investor_portf_margin_ratio(&self, qry_investor_portf_margin_ratio:  QryInvestorPortfMarginRatio, request_id:  i32)-> i32;
        /// 投资者产品SPBM明细查询
        fn req_qry_investor_prod_spbm_detail(&self, qry_investor_prod_spbm_detail:  QryInvestorProdSPBMDetail, request_id:  i32)-> i32;
        /// 投资者商品组SPMM记录查询
        fn req_qry_investor_commodity_spmm_margin(&self, qry_investor_commodity_spmm_margin:  QryInvestorCommoditySPMMMargin, request_id:  i32)-> i32;
        /// 投资者商品群SPMM记录查询
        fn req_qry_investor_commodity_group_spmm_margin(&self, qry_investor_commodity_group_spmm_margin:  QryInvestorCommodityGroupSPMMMargin, request_id:  i32)-> i32;
        /// SPMM合约参数查询
        fn req_qry_spmm_inst_param(&self, qry_spmm_inst_param:  QrySPMMInstParam, request_id:  i32)-> i32;
        /// SPMM产品参数查询
        fn req_qry_spmm_product_param(&self, qry_spmm_product_param:  QrySPMMProductParam, request_id:  i32)-> i32;
        /// SPBM附加跨品种抵扣参数查询
        fn req_qry_spbm_add_on_inter_parameter(&self, qry_spbm_add_on_inter_parameter:  QrySPBMAddOnInterParameter, request_id:  i32)-> i32;
        /// RCAMS产品组合信息查询
        fn req_qry_rcams_comb_product_info(&self, qry_rcams_comb_product_info:  QryRCAMSCombProductInfo, request_id:  i32)-> i32;
        /// RCAMS同合约风险对冲参数查询
        fn req_qry_rcams_instr_parameter(&self, qry_rcams_instr_parameter:  QryRCAMSInstrParameter, request_id:  i32)-> i32;
        /// RCAMS品种内风险对冲参数查询
        fn req_qry_rcams_intra_parameter(&self, qry_rcams_intra_parameter:  QryRCAMSIntraParameter, request_id:  i32)-> i32;
        /// RCAMS跨品种风险折抵参数查询
        fn req_qry_rcams_inter_parameter(&self, qry_rcams_inter_parameter:  QryRCAMSInterParameter, request_id:  i32)-> i32;
        /// RCAMS空头期权风险调整参数查询
        fn req_qry_rcams_short_opt_adjust_param(&self, qry_rcams_short_opt_adjust_param:  QryRCAMSShortOptAdjustParam, request_id:  i32)-> i32;
        /// RCAMS策略组合持仓查询
        fn req_qry_rcams_investor_comb_position(&self, qry_rcams_investor_comb_position:  QryRCAMSInvestorCombPosition, request_id:  i32)-> i32;
        /// 投资者品种RCAMS保证金查询
        fn req_qry_investor_prod_rcams_margin(&self, qry_investor_prod_rcams_margin:  QryInvestorProdRCAMSMargin, request_id:  i32)-> i32;
        /// RULE合约保证金参数查询
        fn req_qry_rule_instr_parameter(&self, qry_rule_instr_parameter:  QryRULEInstrParameter, request_id:  i32)-> i32;
        /// RULE品种内对锁仓折扣参数查询
        fn req_qry_rule_intra_parameter(&self, qry_rule_intra_parameter:  QryRULEIntraParameter, request_id:  i32)-> i32;
        /// RULE跨品种抵扣参数查询
        fn req_qry_rule_inter_parameter(&self, qry_rule_inter_parameter:  QryRULEInterParameter, request_id:  i32)-> i32;
        /// 投资者产品RULE保证金查询
        fn req_qry_investor_prod_rule_margin(&self, qry_investor_prod_rule_margin:  QryInvestorProdRULEMargin, request_id:  i32)-> i32;
        /// 投资者新型组合保证金开关查询
        fn req_qry_investor_portf_setting(&self, qry_investor_portf_setting:  QryInvestorPortfSetting, request_id:  i32)-> i32;
        /// 投资者申报费阶梯收取记录查询
        fn req_qry_investor_info_comm_rec(&self, qry_investor_info_comm_rec:  QryInvestorInfoCommRec, request_id:  i32)-> i32;
        /// 组合腿信息查询
        fn req_qry_comb_leg(&self, qry_comb_leg:  QryCombLeg, request_id:  i32)-> i32;
        /// 对冲设置请求
        fn req_offset_setting(&self, input_offset_setting:  InputOffsetSetting, request_id:  i32)-> i32;
        /// 对冲设置撤销请求
        fn req_cancel_offset_setting(&self, input_offset_setting:  InputOffsetSetting, request_id:  i32)-> i32;
        /// 投资者对冲设置查询
        fn req_qry_offset_setting(&self, qry_offset_setting:  QryOffsetSetting, request_id:  i32)-> i32;
    }

    /// 信息分发
    #[derive(Debug, Clone, Default)]
    struct Dissemination {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 序列系列号
        sequence_series:  u16,
        /// 序列号
        sequence_no:  i32,
    }
    /// 用户登录请求
    #[derive(Debug, Clone, Default)]
    struct ReqUserLogin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 密码
        password:  String,
        /// 用户端产品信息
        user_product_info:  String,
        /// 接口端产品信息
        interface_product_info:  String,
        /// 协议信息
        protocol_info:  String,
        /// Mac地址
        mac_address:  String,
        /// 动态密码
        one_time_password:  String,
        /// 登录备注
        login_remark:  String,
        /// 终端IP端口
        client_ip_port:  i32,
        /// 终端IP地址
        client_ip_address:  String,
    }
    /// 用户登录应答
    #[derive(Debug, Clone, Default)]
    struct RspUserLogin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 登录成功时间
        login_time:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 交易系统名称
        system_name:  String,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 最大报单引用
        max_order_ref:  String,
        /// 上期所时间
        shfe_time:  String,
        /// 大商所时间
        dce_time:  String,
        /// 郑商所时间
        czce_time:  String,
        /// 中金所时间
        ffex_time:  String,
        /// 能源中心时间
        ine_time:  String,
        /// 后台版本信息
        sys_version:  String,
        /// 广期所时间
        gfex_time:  String,
        /// 当前登录中心号
        login_dr_identity_id:  i32,
        /// 用户所属中心号
        user_dr_identity_id:  i32,
        /// 上次登陆时间
        last_login_time:  String,
        /// 预留信息
        reserve_info:  String,
    }
    /// 用户登出请求
    #[derive(Debug, Clone, Default)]
    struct UserLogout {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
    }
    /// 强制交易员退出
    #[derive(Debug, Clone, Default)]
    struct ForceUserLogout {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
    }
    /// 客户端认证请求
    #[derive(Debug, Clone, Default)]
    struct ReqAuthenticate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 用户端产品信息
        user_product_info:  String,
        /// 认证码
        auth_code:  String,
        /// App代码
        app_id:  String,
    }
    /// 客户端认证响应
    #[derive(Debug, Clone, Default)]
    struct RspAuthenticate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 用户端产品信息
        user_product_info:  String,
        /// App代码
        app_id:  String,
        /// App类型
        app_type:  u8,
    }
    /// 客户端认证信息
    #[derive(Debug, Clone, Default)]
    struct AuthenticationInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 用户端产品信息
        user_product_info:  String,
        /// 认证信息
        auth_info:  String,
        /// 是否为认证结果
        is_result:  i32,
        /// App代码
        app_id:  String,
        /// App类型
        app_type:  u8,
        /// 终端IP地址
        client_ip_address:  String,
    }
    /// 用户登录应答2
    #[derive(Debug, Clone, Default)]
    struct RspUserLogin2 {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 登录成功时间
        login_time:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 交易系统名称
        system_name:  String,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 最大报单引用
        max_order_ref:  String,
        /// 上期所时间
        shfe_time:  String,
        /// 大商所时间
        dce_time:  String,
        /// 郑商所时间
        czce_time:  String,
        /// 中金所时间
        ffex_time:  String,
        /// 能源中心时间
        ine_time:  String,
        /// 随机串
        random_string:  Vec<u8>,
    }
    /// 银期转帐报文头
    #[derive(Debug, Clone, Default)]
    struct TransferHeader {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 版本号，常量，1.0
        version:  String,
        /// 交易代码，必填
        trade_code:  String,
        /// 交易日期，必填，格式：yyyymmdd
        trade_date:  String,
        /// 交易时间，必填，格式：hhmmss
        trade_time:  String,
        /// 发起方流水号，N/A
        trade_serial:  String,
        /// 期货公司代码，必填
        future_id:  String,
        /// 银行代码，根据查询银行得到，必填
        bank_id:  String,
        /// 银行分中心代码，根据查询银行得到，必填
        bank_branch_id:  String,
        /// 操作员，N/A
        oper_no:  String,
        /// 交易设备类型，N/A
        device_id:  String,
        /// 记录数，N/A
        record_num:  Vec<u8>,
        /// 会话编号，N/A
        session_id:  i32,
        /// 请求编号，N/A
        request_id:  i32,
    }
    /// 银行资金转期货请求，TradeCode=202001
    #[derive(Debug, Clone, Default)]
    struct TransferBankToFutureReq {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 期货资金账户
        future_account:  String,
        /// 密码标志
        future_pwd_flag:  u8,
        /// 密码
        future_acc_pwd:  String,
        /// 转账金额
        trade_amt:  f64,
        /// 客户手续费
        cust_fee:  f64,
        /// 币种：RMB-人民币 USD-美圆 HKD-港元
        currency_code:  String,
    }
    /// 银行资金转期货请求响应
    #[derive(Debug, Clone, Default)]
    struct TransferBankToFutureRsp {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 响应代码
        ret_code:  String,
        /// 响应信息
        ret_info:  String,
        /// 资金账户
        future_account:  String,
        /// 转帐金额
        trade_amt:  f64,
        /// 应收客户手续费
        cust_fee:  f64,
        /// 币种
        currency_code:  String,
    }
    /// 期货资金转银行请求，TradeCode=202002
    #[derive(Debug, Clone, Default)]
    struct TransferFutureToBankReq {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 期货资金账户
        future_account:  String,
        /// 密码标志
        future_pwd_flag:  u8,
        /// 密码
        future_acc_pwd:  String,
        /// 转账金额
        trade_amt:  f64,
        /// 客户手续费
        cust_fee:  f64,
        /// 币种：RMB-人民币 USD-美圆 HKD-港元
        currency_code:  String,
    }
    /// 期货资金转银行请求响应
    #[derive(Debug, Clone, Default)]
    struct TransferFutureToBankRsp {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 响应代码
        ret_code:  String,
        /// 响应信息
        ret_info:  String,
        /// 资金账户
        future_account:  String,
        /// 转帐金额
        trade_amt:  f64,
        /// 应收客户手续费
        cust_fee:  f64,
        /// 币种
        currency_code:  String,
    }
    /// 查询银行资金请求，TradeCode=204002
    #[derive(Debug, Clone, Default)]
    struct TransferQryBankReq {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 期货资金账户
        future_account:  String,
        /// 密码标志
        future_pwd_flag:  u8,
        /// 密码
        future_acc_pwd:  String,
        /// 币种：RMB-人民币 USD-美圆 HKD-港元
        currency_code:  String,
    }
    /// 查询银行资金请求响应
    #[derive(Debug, Clone, Default)]
    struct TransferQryBankRsp {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 响应代码
        ret_code:  String,
        /// 响应信息
        ret_info:  String,
        /// 资金账户
        future_account:  String,
        /// 银行余额
        trade_amt:  f64,
        /// 银行可用余额
        use_amt:  f64,
        /// 银行可取余额
        fetch_amt:  f64,
        /// 币种
        currency_code:  String,
    }
    /// 查询银行交易明细请求，TradeCode=204999
    #[derive(Debug, Clone, Default)]
    struct TransferQryDetailReq {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 期货资金账户
        future_account:  String,
    }
    /// 查询银行交易明细请求响应
    #[derive(Debug, Clone, Default)]
    struct TransferQryDetailRsp {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 交易代码
        trade_code:  String,
        /// 期货流水号
        future_serial:  i32,
        /// 期货公司代码
        future_id:  String,
        /// 资金帐号
        future_account:  String,
        /// 银行流水号
        bank_serial:  i32,
        /// 银行代码
        bank_id:  String,
        /// 银行分中心代码
        bank_branch_id:  String,
        /// 银行账号
        bank_account:  String,
        /// 证件号码
        cert_code:  String,
        /// 货币代码
        currency_code:  String,
        /// 发生金额
        tx_amount:  f64,
        /// 有效标志
        flag:  u8,
    }
    /// 响应信息
    #[derive(Debug, Clone, Default)]
    struct RspInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
    }
    /// 交易所
    #[derive(Debug, Clone, Default)]
    struct Exchange {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 交易所名称
        exchange_name:  String,
        /// 交易所属性
        exchange_property:  u8,
    }
    /// 产品
    #[derive(Debug, Clone, Default)]
    struct Product {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 产品名称
        product_name:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 产品类型
        product_class:  u8,
        /// 合约数量乘数
        volume_multiple:  i32,
        /// 最小变动价位
        price_tick:  f64,
        /// 市价单最大下单量
        max_market_order_volume:  i32,
        /// 市价单最小下单量
        min_market_order_volume:  i32,
        /// 限价单最大下单量
        max_limit_order_volume:  i32,
        /// 限价单最小下单量
        min_limit_order_volume:  i32,
        /// 持仓类型
        position_type:  u8,
        /// 持仓日期类型
        position_date_type:  u8,
        /// 平仓处理类型
        close_deal_type:  u8,
        /// 交易币种类型
        trade_currency_id:  String,
        /// 质押资金可用范围
        mortgage_fund_use_range:  u8,
        /// 合约基础商品乘数
        underlying_multiple:  f64,
        /// 产品代码
        product_id:  String,
        /// 交易所产品代码
        exchange_product_id:  String,
        /// 开仓量限制粒度
        open_limit_control_level:  u8,
        /// 报单频率控制粒度
        order_freq_control_level:  u8,
    }
    /// 合约
    #[derive(Debug, Clone, Default)]
    struct Instrument {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 合约名称
        instrument_name:  String,
        /// 产品类型
        product_class:  u8,
        /// 交割年份
        delivery_year:  i32,
        /// 交割月
        delivery_month:  i32,
        /// 市价单最大下单量
        max_market_order_volume:  i32,
        /// 市价单最小下单量
        min_market_order_volume:  i32,
        /// 限价单最大下单量
        max_limit_order_volume:  i32,
        /// 限价单最小下单量
        min_limit_order_volume:  i32,
        /// 合约数量乘数
        volume_multiple:  i32,
        /// 最小变动价位
        price_tick:  f64,
        /// 创建日
        create_date:  String,
        /// 上市日
        open_date:  String,
        /// 到期日
        expire_date:  String,
        /// 开始交割日
        start_delivery_date:  String,
        /// 结束交割日
        end_delivery_date:  String,
        /// 合约生命周期状态
        inst_life_phase:  u8,
        /// 当前是否交易
        is_trading:  i32,
        /// 持仓类型
        position_type:  u8,
        /// 持仓日期类型
        position_date_type:  u8,
        /// 多头保证金率
        long_margin_ratio:  f64,
        /// 空头保证金率
        short_margin_ratio:  f64,
        /// 是否使用大额单边保证金算法
        max_margin_side_algorithm:  u8,
        /// 执行价
        strike_price:  f64,
        /// 期权类型
        options_type:  u8,
        /// 合约基础商品乘数
        underlying_multiple:  f64,
        /// 组合类型
        combination_type:  u8,
        /// 合约代码
        instrument_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// 产品代码
        product_id:  String,
        /// 基础商品代码
        underlying_instr_id:  String,
    }
    /// 经纪公司
    #[derive(Debug, Clone, Default)]
    struct Broker {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 经纪公司简称
        broker_abbr:  String,
        /// 经纪公司名称
        broker_name:  String,
        /// 是否活跃
        is_active:  i32,
    }
    /// 交易所交易员
    #[derive(Debug, Clone, Default)]
    struct Trader {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 密码
        password:  String,
        /// 安装数量
        install_count:  i32,
        /// 经纪公司代码
        broker_id:  String,
        /// 撤单时选择席位算法
        order_cancel_alg:  u8,
        /// 交易报盘安装数量
        trade_install_count:  i32,
        /// 行情报盘安装数量
        md_install_count:  i32,
    }
    /// 投资者
    #[derive(Debug, Clone, Default)]
    struct Investor {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者代码
        investor_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者分组代码
        investor_group_id:  String,
        /// 投资者名称
        investor_name:  String,
        /// 证件类型
        identified_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 是否活跃
        is_active:  i32,
        /// 联系电话
        telephone:  String,
        /// 通讯地址
        address:  String,
        /// 开户日期
        open_date:  String,
        /// 手机
        mobile:  String,
        /// 手续费率模板代码
        comm_model_id:  String,
        /// 保证金率模板代码
        margin_model_id:  String,
        /// 是否频率控制
        is_order_freq:  u8,
        /// 是否开仓限制
        is_open_vol_limit:  u8,
    }
    /// 交易编码
    #[derive(Debug, Clone, Default)]
    struct TradingCode {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者代码
        investor_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 客户代码
        client_id:  String,
        /// 是否活跃
        is_active:  i32,
        /// 交易编码类型
        client_idtype:  u8,
        /// 营业部编号
        branch_id:  String,
        /// 业务类型
        biz_type:  u8,
        /// 投资单元代码
        invest_unit_id:  String,
    }
    /// 会员编码和经纪公司编码对照表
    #[derive(Debug, Clone, Default)]
    struct PartBroker {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 是否活跃
        is_active:  i32,
    }
    /// 管理用户
    #[derive(Debug, Clone, Default)]
    struct SuperUser {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 用户代码
        user_id:  String,
        /// 用户名称
        user_name:  String,
        /// 密码
        password:  String,
        /// 是否活跃
        is_active:  i32,
    }
    /// 管理用户功能权限
    #[derive(Debug, Clone, Default)]
    struct SuperUserFunction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 用户代码
        user_id:  String,
        /// 功能代码
        function_code:  u8,
    }
    /// 投资者组
    #[derive(Debug, Clone, Default)]
    struct InvestorGroup {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者分组代码
        investor_group_id:  String,
        /// 投资者分组名称
        investor_group_name:  String,
    }
    /// 资金账户
    #[derive(Debug, Clone, Default)]
    struct TradingAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 上次质押金额
        pre_mortgage:  f64,
        /// 上次信用额度
        pre_credit:  f64,
        /// 上次存款额
        pre_deposit:  f64,
        /// 上次结算准备金
        pre_balance:  f64,
        /// 上次占用的保证金
        pre_margin:  f64,
        /// 利息基数
        interest_base:  f64,
        /// 利息收入
        interest:  f64,
        /// 入金金额
        deposit:  f64,
        /// 出金金额
        withdraw:  f64,
        /// 冻结的保证金
        frozen_margin:  f64,
        /// 冻结的资金
        frozen_cash:  f64,
        /// 冻结的手续费
        frozen_commission:  f64,
        /// 当前保证金总额
        curr_margin:  f64,
        /// 资金差额
        cash_in:  f64,
        /// 手续费
        commission:  f64,
        /// 平仓盈亏
        close_profit:  f64,
        /// 持仓盈亏
        position_profit:  f64,
        /// 期货结算准备金
        balance:  f64,
        /// 可用资金
        available:  f64,
        /// 可取资金
        withdraw_quota:  f64,
        /// 基本准备金
        reserve:  f64,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 信用额度
        credit:  f64,
        /// 质押金额
        mortgage:  f64,
        /// 交易所保证金
        exchange_margin:  f64,
        /// 投资者交割保证金
        delivery_margin:  f64,
        /// 交易所交割保证金
        exchange_delivery_margin:  f64,
        /// 保底期货结算准备金
        reserve_balance:  f64,
        /// 币种代码
        currency_id:  String,
        /// 上次货币质入金额
        pre_fund_mortgage_in:  f64,
        /// 上次货币质出金额
        pre_fund_mortgage_out:  f64,
        /// 货币质入金额
        fund_mortgage_in:  f64,
        /// 货币质出金额
        fund_mortgage_out:  f64,
        /// 货币质押余额
        fund_mortgage_available:  f64,
        /// 可质押货币金额
        mortgageable_fund:  f64,
        /// 特殊产品占用保证金
        spec_product_margin:  f64,
        /// 特殊产品冻结保证金
        spec_product_frozen_margin:  f64,
        /// 特殊产品手续费
        spec_product_commission:  f64,
        /// 特殊产品冻结手续费
        spec_product_frozen_commission:  f64,
        /// 特殊产品持仓盈亏
        spec_product_position_profit:  f64,
        /// 特殊产品平仓盈亏
        spec_product_close_profit:  f64,
        /// 根据持仓盈亏算法计算的特殊产品持仓盈亏
        spec_product_position_profit_by_alg:  f64,
        /// 特殊产品交易所保证金
        spec_product_exchange_margin:  f64,
        /// 业务类型
        biz_type:  u8,
        /// 延时换汇冻结金额
        frozen_swap:  f64,
        /// 剩余换汇额度
        remain_swap:  f64,
        /// 期权市值
        option_value:  f64,
    }
    /// 投资者持仓
    #[derive(Debug, Clone, Default)]
    struct InvestorPosition {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 持仓多空方向
        posi_direction:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 持仓日期
        position_date:  u8,
        /// 上日持仓
        yd_position:  i32,
        /// 今日持仓
        position:  i32,
        /// 多头冻结
        long_frozen:  i32,
        /// 空头冻结
        short_frozen:  i32,
        /// 开仓冻结金额
        long_frozen_amount:  f64,
        /// 开仓冻结金额
        short_frozen_amount:  f64,
        /// 开仓量
        open_volume:  i32,
        /// 平仓量
        close_volume:  i32,
        /// 开仓金额
        open_amount:  f64,
        /// 平仓金额
        close_amount:  f64,
        /// 持仓成本
        position_cost:  f64,
        /// 上次占用的保证金
        pre_margin:  f64,
        /// 占用的保证金
        use_margin:  f64,
        /// 冻结的保证金
        frozen_margin:  f64,
        /// 冻结的资金
        frozen_cash:  f64,
        /// 冻结的手续费
        frozen_commission:  f64,
        /// 资金差额
        cash_in:  f64,
        /// 手续费
        commission:  f64,
        /// 平仓盈亏
        close_profit:  f64,
        /// 持仓盈亏
        position_profit:  f64,
        /// 上次结算价
        pre_settlement_price:  f64,
        /// 本次结算价
        settlement_price:  f64,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 开仓成本
        open_cost:  f64,
        /// 交易所保证金
        exchange_margin:  f64,
        /// 组合成交形成的持仓
        comb_position:  i32,
        /// 组合多头冻结
        comb_long_frozen:  i32,
        /// 组合空头冻结
        comb_short_frozen:  i32,
        /// 逐日盯市平仓盈亏
        close_profit_by_date:  f64,
        /// 逐笔对冲平仓盈亏
        close_profit_by_trade:  f64,
        /// 今日持仓
        today_position:  i32,
        /// 保证金率
        margin_rate_by_money:  f64,
        /// 保证金率(按手数)
        margin_rate_by_volume:  f64,
        /// 执行冻结
        strike_frozen:  i32,
        /// 执行冻结金额
        strike_frozen_amount:  f64,
        /// 放弃执行冻结
        abandon_frozen:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 执行冻结的昨仓
        yd_strike_frozen:  i32,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 持仓成本差值
        position_cost_offset:  f64,
        /// tas持仓手数
        tas_position:  i32,
        /// tas持仓成本
        tas_position_cost:  f64,
        /// 合约代码
        instrument_id:  String,
        /// 期权市值
        option_value:  f64,
    }
    /// 合约保证金率
    #[derive(Debug, Clone, Default)]
    struct InstrumentMarginRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 多头保证金率
        long_margin_ratio_by_money:  f64,
        /// 多头保证金费
        long_margin_ratio_by_volume:  f64,
        /// 空头保证金率
        short_margin_ratio_by_money:  f64,
        /// 空头保证金费
        short_margin_ratio_by_volume:  f64,
        /// 是否相对交易所收取
        is_relative:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 合约手续费率
    #[derive(Debug, Clone, Default)]
    struct InstrumentCommissionRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 开仓手续费率
        open_ratio_by_money:  f64,
        /// 开仓手续费
        open_ratio_by_volume:  f64,
        /// 平仓手续费率
        close_ratio_by_money:  f64,
        /// 平仓手续费
        close_ratio_by_volume:  f64,
        /// 平今手续费率
        close_today_ratio_by_money:  f64,
        /// 平今手续费
        close_today_ratio_by_volume:  f64,
        /// 交易所代码
        exchange_id:  String,
        /// 业务类型
        biz_type:  u8,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 深度行情
    #[derive(Debug, Clone, Default)]
    struct DepthMarketData {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 最新价
        last_price:  f64,
        /// 上次结算价
        pre_settlement_price:  f64,
        /// 昨收盘
        pre_close_price:  f64,
        /// 昨持仓量
        pre_open_interest:  f64,
        /// 今开盘
        open_price:  f64,
        /// 最高价
        highest_price:  f64,
        /// 最低价
        lowest_price:  f64,
        /// 数量
        volume:  i32,
        /// 成交金额
        turnover:  f64,
        /// 持仓量
        open_interest:  f64,
        /// 今收盘
        close_price:  f64,
        /// 本次结算价
        settlement_price:  f64,
        /// 涨停板价
        upper_limit_price:  f64,
        /// 跌停板价
        lower_limit_price:  f64,
        /// 昨虚实度
        pre_delta:  f64,
        /// 今虚实度
        curr_delta:  f64,
        /// 最后修改时间
        update_time:  String,
        /// 最后修改毫秒
        update_millisecond:  i32,
        /// 申买价一
        bid_price1:  f64,
        /// 申买量一
        bid_volume1:  i32,
        /// 申卖价一
        ask_price1:  f64,
        /// 申卖量一
        ask_volume1:  i32,
        /// 申买价二
        bid_price2:  f64,
        /// 申买量二
        bid_volume2:  i32,
        /// 申卖价二
        ask_price2:  f64,
        /// 申卖量二
        ask_volume2:  i32,
        /// 申买价三
        bid_price3:  f64,
        /// 申买量三
        bid_volume3:  i32,
        /// 申卖价三
        ask_price3:  f64,
        /// 申卖量三
        ask_volume3:  i32,
        /// 申买价四
        bid_price4:  f64,
        /// 申买量四
        bid_volume4:  i32,
        /// 申卖价四
        ask_price4:  f64,
        /// 申卖量四
        ask_volume4:  i32,
        /// 申买价五
        bid_price5:  f64,
        /// 申买量五
        bid_volume5:  i32,
        /// 申卖价五
        ask_price5:  f64,
        /// 申卖量五
        ask_volume5:  i32,
        /// 当日均价
        average_price:  f64,
        /// 业务日期
        action_day:  String,
        /// 合约代码
        instrument_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// 上带价
        banding_upper_price:  f64,
        /// 下带价
        banding_lower_price:  f64,
    }
    /// 投资者合约交易权限
    #[derive(Debug, Clone, Default)]
    struct InstrumentTradingRight {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易权限
        trading_right:  u8,
        /// 合约代码
        instrument_id:  String,
    }
    /// 经纪公司用户
    #[derive(Debug, Clone, Default)]
    struct BrokerUser {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 用户名称
        user_name:  String,
        /// 用户类型
        user_type:  u8,
        /// 是否活跃
        is_active:  i32,
        /// 是否使用令牌
        is_using_otp:  i32,
        /// 是否强制终端认证
        is_auth_force:  i32,
    }
    /// 经纪公司用户口令
    #[derive(Debug, Clone, Default)]
    struct BrokerUserPassword {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 密码
        password:  String,
        /// 上次修改时间
        last_update_time:  String,
        /// 上次登陆时间
        last_login_time:  String,
        /// 密码过期时间
        expire_date:  String,
        /// 弱密码过期时间
        weak_expire_date:  String,
    }
    /// 经纪公司用户功能权限
    #[derive(Debug, Clone, Default)]
    struct BrokerUserFunction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 经纪公司功能代码
        broker_function_code:  u8,
    }
    /// 交易所交易员报盘机
    #[derive(Debug, Clone, Default)]
    struct TraderOffer {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 密码
        password:  String,
        /// 安装编号
        install_id:  i32,
        /// 本地报单编号
        order_local_id:  String,
        /// 交易所交易员连接状态
        trader_connect_status:  u8,
        /// 发出连接请求的日期
        connect_request_date:  String,
        /// 发出连接请求的时间
        connect_request_time:  String,
        /// 上次报告日期
        last_report_date:  String,
        /// 上次报告时间
        last_report_time:  String,
        /// 完成连接日期
        connect_date:  String,
        /// 完成连接时间
        connect_time:  String,
        /// 启动日期
        start_date:  String,
        /// 启动时间
        start_time:  String,
        /// 交易日
        trading_day:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 本席位最大成交编号
        max_trade_id:  String,
        /// 本席位最大报单备拷
        max_order_message_reference:  Vec<u8>,
        /// 撤单时选择席位算法
        order_cancel_alg:  u8,
    }
    /// 投资者结算结果
    #[derive(Debug, Clone, Default)]
    struct SettlementInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 序号
        sequence_no:  i32,
        /// 消息正文
        content:  Vec<u8>,
        /// 投资者帐号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
    }
    /// 合约保证金率调整
    #[derive(Debug, Clone, Default)]
    struct InstrumentMarginRateAdjust {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 多头保证金率
        long_margin_ratio_by_money:  f64,
        /// 多头保证金费
        long_margin_ratio_by_volume:  f64,
        /// 空头保证金率
        short_margin_ratio_by_money:  f64,
        /// 空头保证金费
        short_margin_ratio_by_volume:  f64,
        /// 是否相对交易所收取
        is_relative:  i32,
        /// 合约代码
        instrument_id:  String,
    }
    /// 交易所保证金率
    #[derive(Debug, Clone, Default)]
    struct ExchangeMarginRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 多头保证金率
        long_margin_ratio_by_money:  f64,
        /// 多头保证金费
        long_margin_ratio_by_volume:  f64,
        /// 空头保证金率
        short_margin_ratio_by_money:  f64,
        /// 空头保证金费
        short_margin_ratio_by_volume:  f64,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 交易所保证金率调整
    #[derive(Debug, Clone, Default)]
    struct ExchangeMarginRateAdjust {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 跟随交易所投资者多头保证金率
        long_margin_ratio_by_money:  f64,
        /// 跟随交易所投资者多头保证金费
        long_margin_ratio_by_volume:  f64,
        /// 跟随交易所投资者空头保证金率
        short_margin_ratio_by_money:  f64,
        /// 跟随交易所投资者空头保证金费
        short_margin_ratio_by_volume:  f64,
        /// 交易所多头保证金率
        exch_long_margin_ratio_by_money:  f64,
        /// 交易所多头保证金费
        exch_long_margin_ratio_by_volume:  f64,
        /// 交易所空头保证金率
        exch_short_margin_ratio_by_money:  f64,
        /// 交易所空头保证金费
        exch_short_margin_ratio_by_volume:  f64,
        /// 不跟随交易所投资者多头保证金率
        no_long_margin_ratio_by_money:  f64,
        /// 不跟随交易所投资者多头保证金费
        no_long_margin_ratio_by_volume:  f64,
        /// 不跟随交易所投资者空头保证金率
        no_short_margin_ratio_by_money:  f64,
        /// 不跟随交易所投资者空头保证金费
        no_short_margin_ratio_by_volume:  f64,
        /// 合约代码
        instrument_id:  String,
    }
    /// 汇率
    #[derive(Debug, Clone, Default)]
    struct ExchangeRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 源币种
        from_currency_id:  String,
        /// 源币种单位数量
        from_currency_unit:  f64,
        /// 目标币种
        to_currency_id:  String,
        /// 汇率
        exchange_rate:  f64,
    }
    /// 结算引用
    #[derive(Debug, Clone, Default)]
    struct SettlementRef {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
    }
    /// 当前时间
    #[derive(Debug, Clone, Default)]
    struct CurrentTime {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 当前交易日
        curr_date:  String,
        /// 当前时间
        curr_time:  String,
        /// 当前时间（毫秒）
        curr_millisecond:  i32,
        /// 自然日期
        action_day:  String,
    }
    /// 通讯阶段
    #[derive(Debug, Clone, Default)]
    struct CommPhase {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 通讯时段编号
        comm_phase_no:  u16,
        /// 系统编号
        system_id:  String,
    }
    /// 登录信息
    #[derive(Debug, Clone, Default)]
    struct LoginInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 登录日期
        login_date:  String,
        /// 登录时间
        login_time:  String,
        /// 用户端产品信息
        user_product_info:  String,
        /// 接口端产品信息
        interface_product_info:  String,
        /// 协议信息
        protocol_info:  String,
        /// 系统名称
        system_name:  String,
        /// 密码,已弃用
        password_deprecated:  Vec<u8>,
        /// 最大报单引用
        max_order_ref:  String,
        /// 上期所时间
        shfe_time:  String,
        /// 大商所时间
        dce_time:  String,
        /// 郑商所时间
        czce_time:  String,
        /// 中金所时间
        ffex_time:  String,
        /// Mac地址
        mac_address:  String,
        /// 动态密码
        one_time_password:  String,
        /// 能源中心时间
        ine_time:  String,
        /// 查询时是否需要流控
        is_qry_control:  i32,
        /// 登录备注
        login_remark:  String,
        /// 密码
        password:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 登录信息
    #[derive(Debug, Clone, Default)]
    struct LogoutAll {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 系统名称
        system_name:  String,
    }
    /// 前置状态
    #[derive(Debug, Clone, Default)]
    struct FrontStatus {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 前置编号
        front_id:  i32,
        /// 上次报告日期
        last_report_date:  String,
        /// 上次报告时间
        last_report_time:  String,
        /// 是否活跃
        is_active:  i32,
    }
    /// 用户口令变更
    #[derive(Debug, Clone, Default)]
    struct UserPasswordUpdate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 原来的口令
        old_password:  String,
        /// 新的口令
        new_password:  String,
    }
    /// 输入报单
    #[derive(Debug, Clone, Default)]
    struct InputOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报单引用
        order_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 报单价格条件
        order_price_type:  u8,
        /// 买卖方向
        direction:  u8,
        /// 组合开平标志
        comb_offset_flag:  String,
        /// 组合投机套保标志
        comb_hedge_flag:  String,
        /// 价格
        limit_price:  f64,
        /// 数量
        volume_total_original:  i32,
        /// 有效期类型
        time_condition:  u8,
        /// GTD日期
        gtd_date:  String,
        /// 成交量类型
        volume_condition:  u8,
        /// 最小成交量
        min_volume:  i32,
        /// 触发条件
        contingent_condition:  u8,
        /// 止损价
        stop_price:  f64,
        /// 强平原因
        force_close_reason:  u8,
        /// 自动挂起标志
        is_auto_suspend:  i32,
        /// 业务单元
        business_unit:  String,
        /// 请求编号
        request_id:  i32,
        /// 用户强平标志
        user_force_close:  i32,
        /// 互换单标志
        is_swap_order:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 资金账号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
        /// 交易编码
        client_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
        /// 报单回显字段
        order_memo:  String,
        /// session上请求计数 api自动维护
        session_req_seq:  i32,
    }
    /// 报单
    #[derive(Debug, Clone, Default)]
    struct Order {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报单引用
        order_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 报单价格条件
        order_price_type:  u8,
        /// 买卖方向
        direction:  u8,
        /// 组合开平标志
        comb_offset_flag:  String,
        /// 组合投机套保标志
        comb_hedge_flag:  String,
        /// 价格
        limit_price:  f64,
        /// 数量
        volume_total_original:  i32,
        /// 有效期类型
        time_condition:  u8,
        /// GTD日期
        gtd_date:  String,
        /// 成交量类型
        volume_condition:  u8,
        /// 最小成交量
        min_volume:  i32,
        /// 触发条件
        contingent_condition:  u8,
        /// 止损价
        stop_price:  f64,
        /// 强平原因
        force_close_reason:  u8,
        /// 自动挂起标志
        is_auto_suspend:  i32,
        /// 业务单元
        business_unit:  String,
        /// 请求编号
        request_id:  i32,
        /// 本地报单编号
        order_local_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 报单提交状态
        order_submit_status:  u8,
        /// 报单提示序号
        notify_sequence:  i32,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 报单编号
        order_sys_id:  String,
        /// 报单来源
        order_source:  u8,
        /// 报单状态
        order_status:  u8,
        /// 报单类型
        order_type:  u8,
        /// 今成交数量
        volume_traded:  i32,
        /// 剩余数量
        volume_total:  i32,
        /// 报单日期
        insert_date:  String,
        /// 委托时间
        insert_time:  String,
        /// 激活时间
        active_time:  String,
        /// 挂起时间
        suspend_time:  String,
        /// 最后修改时间
        update_time:  String,
        /// 撤销时间
        cancel_time:  String,
        /// 最后修改交易所交易员代码
        active_trader_id:  String,
        /// 结算会员编号
        clearing_part_id:  String,
        /// 序号
        sequence_no:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 用户端产品信息
        user_product_info:  String,
        /// 状态信息
        status_msg:  String,
        /// 用户强平标志
        user_force_close:  i32,
        /// 操作用户代码
        active_user_id:  String,
        /// 经纪公司报单编号
        broker_order_seq:  i32,
        /// 相关报单
        relative_order_sys_id:  String,
        /// 郑商所成交数量
        zce_total_traded_volume:  i32,
        /// 互换单标志
        is_swap_order:  i32,
        /// 营业部编号
        branch_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 资金账号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// IP地址
        ip_address:  String,
        /// 报单回显字段
        order_memo:  String,
        /// session上请求计数 api自动维护
        session_req_seq:  i32,
    }
    /// 交易所报单
    #[derive(Debug, Clone, Default)]
    struct ExchangeOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 报单价格条件
        order_price_type:  u8,
        /// 买卖方向
        direction:  u8,
        /// 组合开平标志
        comb_offset_flag:  String,
        /// 组合投机套保标志
        comb_hedge_flag:  String,
        /// 价格
        limit_price:  f64,
        /// 数量
        volume_total_original:  i32,
        /// 有效期类型
        time_condition:  u8,
        /// GTD日期
        gtd_date:  String,
        /// 成交量类型
        volume_condition:  u8,
        /// 最小成交量
        min_volume:  i32,
        /// 触发条件
        contingent_condition:  u8,
        /// 止损价
        stop_price:  f64,
        /// 强平原因
        force_close_reason:  u8,
        /// 自动挂起标志
        is_auto_suspend:  i32,
        /// 业务单元
        business_unit:  String,
        /// 请求编号
        request_id:  i32,
        /// 本地报单编号
        order_local_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 报单提交状态
        order_submit_status:  u8,
        /// 报单提示序号
        notify_sequence:  i32,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 报单编号
        order_sys_id:  String,
        /// 报单来源
        order_source:  u8,
        /// 报单状态
        order_status:  u8,
        /// 报单类型
        order_type:  u8,
        /// 今成交数量
        volume_traded:  i32,
        /// 剩余数量
        volume_total:  i32,
        /// 报单日期
        insert_date:  String,
        /// 委托时间
        insert_time:  String,
        /// 激活时间
        active_time:  String,
        /// 挂起时间
        suspend_time:  String,
        /// 最后修改时间
        update_time:  String,
        /// 撤销时间
        cancel_time:  String,
        /// 最后修改交易所交易员代码
        active_trader_id:  String,
        /// 结算会员编号
        clearing_part_id:  String,
        /// 序号
        sequence_no:  i32,
        /// 营业部编号
        branch_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 交易所报单插入失败
    #[derive(Debug, Clone, Default)]
    struct ExchangeOrderInsertError {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 本地报单编号
        order_local_id:  String,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
    }
    /// 输入报单操作
    #[derive(Debug, Clone, Default)]
    struct InputOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报单操作引用
        order_action_ref:  i32,
        /// 报单引用
        order_ref:  String,
        /// 请求编号
        request_id:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 报单编号
        order_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 价格
        limit_price:  f64,
        /// 数量变化
        volume_change:  i32,
        /// 用户代码
        user_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
        /// 报单回显字段
        order_memo:  String,
        /// session上请求计数 api自动维护
        session_req_seq:  i32,
    }
    /// 报单操作
    #[derive(Debug, Clone, Default)]
    struct OrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报单操作引用
        order_action_ref:  i32,
        /// 报单引用
        order_ref:  String,
        /// 请求编号
        request_id:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 报单编号
        order_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 价格
        limit_price:  f64,
        /// 数量变化
        volume_change:  i32,
        /// 操作日期
        action_date:  String,
        /// 操作时间
        action_time:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 本地报单编号
        order_local_id:  String,
        /// 操作本地编号
        action_local_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 业务单元
        business_unit:  String,
        /// 报单操作状态
        order_action_status:  u8,
        /// 用户代码
        user_id:  String,
        /// 状态信息
        status_msg:  String,
        /// 营业部编号
        branch_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
        /// 报单回显字段
        order_memo:  String,
        /// session上请求计数 api自动维护
        session_req_seq:  i32,
    }
    /// 交易所报单操作
    #[derive(Debug, Clone, Default)]
    struct ExchangeOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 报单编号
        order_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 价格
        limit_price:  f64,
        /// 数量变化
        volume_change:  i32,
        /// 操作日期
        action_date:  String,
        /// 操作时间
        action_time:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 本地报单编号
        order_local_id:  String,
        /// 操作本地编号
        action_local_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 业务单元
        business_unit:  String,
        /// 报单操作状态
        order_action_status:  u8,
        /// 用户代码
        user_id:  String,
        /// 营业部编号
        branch_id:  String,
        /// Mac地址
        mac_address:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 交易所报单操作失败
    #[derive(Debug, Clone, Default)]
    struct ExchangeOrderActionError {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 报单编号
        order_sys_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 本地报单编号
        order_local_id:  String,
        /// 操作本地编号
        action_local_id:  String,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
    }
    /// 交易所成交
    #[derive(Debug, Clone, Default)]
    struct ExchangeTrade {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 成交编号
        trade_id:  String,
        /// 买卖方向
        direction:  u8,
        /// 报单编号
        order_sys_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易角色
        trading_role:  u8,
        /// 开平标志
        offset_flag:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 价格
        price:  f64,
        /// 数量
        volume:  i32,
        /// 成交时期
        trade_date:  String,
        /// 成交时间
        trade_time:  String,
        /// 成交类型
        trade_type:  u8,
        /// 成交价来源
        price_source:  u8,
        /// 交易所交易员代码
        trader_id:  String,
        /// 本地报单编号
        order_local_id:  String,
        /// 结算会员编号
        clearing_part_id:  String,
        /// 业务单元
        business_unit:  String,
        /// 序号
        sequence_no:  i32,
        /// 成交来源
        trade_source:  u8,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
    }
    /// 成交
    #[derive(Debug, Clone, Default)]
    struct Trade {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报单引用
        order_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 成交编号
        trade_id:  String,
        /// 买卖方向
        direction:  u8,
        /// 报单编号
        order_sys_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易角色
        trading_role:  u8,
        /// 开平标志
        offset_flag:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 价格
        price:  f64,
        /// 数量
        volume:  i32,
        /// 成交时期
        trade_date:  String,
        /// 成交时间
        trade_time:  String,
        /// 成交类型
        trade_type:  u8,
        /// 成交价来源
        price_source:  u8,
        /// 交易所交易员代码
        trader_id:  String,
        /// 本地报单编号
        order_local_id:  String,
        /// 结算会员编号
        clearing_part_id:  String,
        /// 业务单元
        business_unit:  String,
        /// 序号
        sequence_no:  i32,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 经纪公司报单编号
        broker_order_seq:  i32,
        /// 成交来源
        trade_source:  u8,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
    }
    /// 用户会话
    #[derive(Debug, Clone, Default)]
    struct UserSession {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 登录日期
        login_date:  String,
        /// 登录时间
        login_time:  String,
        /// 用户端产品信息
        user_product_info:  String,
        /// 接口端产品信息
        interface_product_info:  String,
        /// 协议信息
        protocol_info:  String,
        /// Mac地址
        mac_address:  String,
        /// 登录备注
        login_remark:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 查询最大报单数量
    #[derive(Debug, Clone, Default)]
    struct QryMaxOrderVolume {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 买卖方向
        direction:  u8,
        /// 开平标志
        offset_flag:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 最大允许报单数量
        max_volume:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 投资者结算结果确认信息
    #[derive(Debug, Clone, Default)]
    struct SettlementInfoConfirm {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 确认日期
        confirm_date:  String,
        /// 确认时间
        confirm_time:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 投资者帐号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
    }
    /// 出入金同步
    #[derive(Debug, Clone, Default)]
    struct SyncDeposit {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 出入金流水号
        deposit_seq_no:  Vec<u8>,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 入金金额
        deposit:  f64,
        /// 是否强制进行
        is_force:  i32,
        /// 币种代码
        currency_id:  String,
        /// 是否是个股期权内转
        is_from_sopt:  i32,
        /// 资金密码
        trading_password:  String,
        /// 是否二级代理商的内转
        is_sec_agent_transfer:  i32,
    }
    /// 货币质押同步
    #[derive(Debug, Clone, Default)]
    struct SyncFundMortgage {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 货币质押流水号
        mortgage_seq_no:  Vec<u8>,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 源币种
        from_currency_id:  String,
        /// 质押金额
        mortgage_amount:  f64,
        /// 目标币种
        to_currency_id:  String,
    }
    /// 经纪公司同步
    #[derive(Debug, Clone, Default)]
    struct BrokerSync {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
    }
    /// 正在同步中的投资者
    #[derive(Debug, Clone, Default)]
    struct SyncingInvestor {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者代码
        investor_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者分组代码
        investor_group_id:  String,
        /// 投资者名称
        investor_name:  String,
        /// 证件类型
        identified_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 是否活跃
        is_active:  i32,
        /// 联系电话
        telephone:  String,
        /// 通讯地址
        address:  String,
        /// 开户日期
        open_date:  String,
        /// 手机
        mobile:  String,
        /// 手续费率模板代码
        comm_model_id:  String,
        /// 保证金率模板代码
        margin_model_id:  String,
        /// 是否频率控制
        is_order_freq:  u8,
        /// 是否开仓限制
        is_open_vol_limit:  u8,
    }
    /// 正在同步中的交易代码
    #[derive(Debug, Clone, Default)]
    struct SyncingTradingCode {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者代码
        investor_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 客户代码
        client_id:  String,
        /// 是否活跃
        is_active:  i32,
        /// 交易编码类型
        client_idtype:  u8,
    }
    /// 正在同步中的投资者分组
    #[derive(Debug, Clone, Default)]
    struct SyncingInvestorGroup {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者分组代码
        investor_group_id:  String,
        /// 投资者分组名称
        investor_group_name:  String,
    }
    /// 正在同步中的交易账号
    #[derive(Debug, Clone, Default)]
    struct SyncingTradingAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 上次质押金额
        pre_mortgage:  f64,
        /// 上次信用额度
        pre_credit:  f64,
        /// 上次存款额
        pre_deposit:  f64,
        /// 上次结算准备金
        pre_balance:  f64,
        /// 上次占用的保证金
        pre_margin:  f64,
        /// 利息基数
        interest_base:  f64,
        /// 利息收入
        interest:  f64,
        /// 入金金额
        deposit:  f64,
        /// 出金金额
        withdraw:  f64,
        /// 冻结的保证金
        frozen_margin:  f64,
        /// 冻结的资金
        frozen_cash:  f64,
        /// 冻结的手续费
        frozen_commission:  f64,
        /// 当前保证金总额
        curr_margin:  f64,
        /// 资金差额
        cash_in:  f64,
        /// 手续费
        commission:  f64,
        /// 平仓盈亏
        close_profit:  f64,
        /// 持仓盈亏
        position_profit:  f64,
        /// 期货结算准备金
        balance:  f64,
        /// 可用资金
        available:  f64,
        /// 可取资金
        withdraw_quota:  f64,
        /// 基本准备金
        reserve:  f64,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 信用额度
        credit:  f64,
        /// 质押金额
        mortgage:  f64,
        /// 交易所保证金
        exchange_margin:  f64,
        /// 投资者交割保证金
        delivery_margin:  f64,
        /// 交易所交割保证金
        exchange_delivery_margin:  f64,
        /// 保底期货结算准备金
        reserve_balance:  f64,
        /// 币种代码
        currency_id:  String,
        /// 上次货币质入金额
        pre_fund_mortgage_in:  f64,
        /// 上次货币质出金额
        pre_fund_mortgage_out:  f64,
        /// 货币质入金额
        fund_mortgage_in:  f64,
        /// 货币质出金额
        fund_mortgage_out:  f64,
        /// 货币质押余额
        fund_mortgage_available:  f64,
        /// 可质押货币金额
        mortgageable_fund:  f64,
        /// 特殊产品占用保证金
        spec_product_margin:  f64,
        /// 特殊产品冻结保证金
        spec_product_frozen_margin:  f64,
        /// 特殊产品手续费
        spec_product_commission:  f64,
        /// 特殊产品冻结手续费
        spec_product_frozen_commission:  f64,
        /// 特殊产品持仓盈亏
        spec_product_position_profit:  f64,
        /// 特殊产品平仓盈亏
        spec_product_close_profit:  f64,
        /// 根据持仓盈亏算法计算的特殊产品持仓盈亏
        spec_product_position_profit_by_alg:  f64,
        /// 特殊产品交易所保证金
        spec_product_exchange_margin:  f64,
        /// 延时换汇冻结金额
        frozen_swap:  f64,
        /// 剩余换汇额度
        remain_swap:  f64,
        /// 期权市值
        option_value:  f64,
    }
    /// 正在同步中的投资者持仓
    #[derive(Debug, Clone, Default)]
    struct SyncingInvestorPosition {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 持仓多空方向
        posi_direction:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 持仓日期
        position_date:  u8,
        /// 上日持仓
        yd_position:  i32,
        /// 今日持仓
        position:  i32,
        /// 多头冻结
        long_frozen:  i32,
        /// 空头冻结
        short_frozen:  i32,
        /// 开仓冻结金额
        long_frozen_amount:  f64,
        /// 开仓冻结金额
        short_frozen_amount:  f64,
        /// 开仓量
        open_volume:  i32,
        /// 平仓量
        close_volume:  i32,
        /// 开仓金额
        open_amount:  f64,
        /// 平仓金额
        close_amount:  f64,
        /// 持仓成本
        position_cost:  f64,
        /// 上次占用的保证金
        pre_margin:  f64,
        /// 占用的保证金
        use_margin:  f64,
        /// 冻结的保证金
        frozen_margin:  f64,
        /// 冻结的资金
        frozen_cash:  f64,
        /// 冻结的手续费
        frozen_commission:  f64,
        /// 资金差额
        cash_in:  f64,
        /// 手续费
        commission:  f64,
        /// 平仓盈亏
        close_profit:  f64,
        /// 持仓盈亏
        position_profit:  f64,
        /// 上次结算价
        pre_settlement_price:  f64,
        /// 本次结算价
        settlement_price:  f64,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 开仓成本
        open_cost:  f64,
        /// 交易所保证金
        exchange_margin:  f64,
        /// 组合成交形成的持仓
        comb_position:  i32,
        /// 组合多头冻结
        comb_long_frozen:  i32,
        /// 组合空头冻结
        comb_short_frozen:  i32,
        /// 逐日盯市平仓盈亏
        close_profit_by_date:  f64,
        /// 逐笔对冲平仓盈亏
        close_profit_by_trade:  f64,
        /// 今日持仓
        today_position:  i32,
        /// 保证金率
        margin_rate_by_money:  f64,
        /// 保证金率(按手数)
        margin_rate_by_volume:  f64,
        /// 执行冻结
        strike_frozen:  i32,
        /// 执行冻结金额
        strike_frozen_amount:  f64,
        /// 放弃执行冻结
        abandon_frozen:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 执行冻结的昨仓
        yd_strike_frozen:  i32,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 持仓成本差值
        position_cost_offset:  f64,
        /// tas持仓手数
        tas_position:  i32,
        /// tas持仓成本
        tas_position_cost:  f64,
        /// 合约代码
        instrument_id:  String,
    }
    /// 正在同步中的合约保证金率
    #[derive(Debug, Clone, Default)]
    struct SyncingInstrumentMarginRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 多头保证金率
        long_margin_ratio_by_money:  f64,
        /// 多头保证金费
        long_margin_ratio_by_volume:  f64,
        /// 空头保证金率
        short_margin_ratio_by_money:  f64,
        /// 空头保证金费
        short_margin_ratio_by_volume:  f64,
        /// 是否相对交易所收取
        is_relative:  i32,
        /// 合约代码
        instrument_id:  String,
    }
    /// 正在同步中的合约手续费率
    #[derive(Debug, Clone, Default)]
    struct SyncingInstrumentCommissionRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 开仓手续费率
        open_ratio_by_money:  f64,
        /// 开仓手续费
        open_ratio_by_volume:  f64,
        /// 平仓手续费率
        close_ratio_by_money:  f64,
        /// 平仓手续费
        close_ratio_by_volume:  f64,
        /// 平今手续费率
        close_today_ratio_by_money:  f64,
        /// 平今手续费
        close_today_ratio_by_volume:  f64,
        /// 合约代码
        instrument_id:  String,
    }
    /// 正在同步中的合约交易权限
    #[derive(Debug, Clone, Default)]
    struct SyncingInstrumentTradingRight {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易权限
        trading_right:  u8,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询报单
    #[derive(Debug, Clone, Default)]
    struct QryOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 报单编号
        order_sys_id:  String,
        /// 开始时间
        insert_time_start:  String,
        /// 结束时间
        insert_time_end:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询成交
    #[derive(Debug, Clone, Default)]
    struct QryTrade {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 成交编号
        trade_id:  String,
        /// 开始时间
        trade_time_start:  String,
        /// 结束时间
        trade_time_end:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询投资者持仓
    #[derive(Debug, Clone, Default)]
    struct QryInvestorPosition {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询资金账户
    #[derive(Debug, Clone, Default)]
    struct QryTradingAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 币种代码
        currency_id:  String,
        /// 业务类型
        biz_type:  u8,
        /// 投资者帐号
        account_id:  String,
    }
    /// 查询投资者
    #[derive(Debug, Clone, Default)]
    struct QryInvestor {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
    }
    /// 查询交易编码
    #[derive(Debug, Clone, Default)]
    struct QryTradingCode {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易编码类型
        client_idtype:  u8,
        /// 投资单元代码
        invest_unit_id:  String,
    }
    /// 查询投资者组
    #[derive(Debug, Clone, Default)]
    struct QryInvestorGroup {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
    }
    /// 查询合约保证金率
    #[derive(Debug, Clone, Default)]
    struct QryInstrumentMarginRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询手续费率
    #[derive(Debug, Clone, Default)]
    struct QryInstrumentCommissionRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询合约交易权限
    #[derive(Debug, Clone, Default)]
    struct QryInstrumentTradingRight {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询经纪公司
    #[derive(Debug, Clone, Default)]
    struct QryBroker {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
    }
    /// 查询交易员
    #[derive(Debug, Clone, Default)]
    struct QryTrader {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
    }
    /// 查询管理用户功能权限
    #[derive(Debug, Clone, Default)]
    struct QrySuperUserFunction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 用户代码
        user_id:  String,
    }
    /// 查询用户会话
    #[derive(Debug, Clone, Default)]
    struct QryUserSession {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
    }
    /// 查询经纪公司会员代码
    #[derive(Debug, Clone, Default)]
    struct QryPartBroker {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 会员代码
        participant_id:  String,
    }
    /// 查询前置状态
    #[derive(Debug, Clone, Default)]
    struct QryFrontStatus {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 前置编号
        front_id:  i32,
    }
    /// 查询交易所报单
    #[derive(Debug, Clone, Default)]
    struct QryExchangeOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
    }
    /// 查询报单操作
    #[derive(Debug, Clone, Default)]
    struct QryOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
    }
    /// 查询交易所报单操作
    #[derive(Debug, Clone, Default)]
    struct QryExchangeOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
    }
    /// 查询管理用户
    #[derive(Debug, Clone, Default)]
    struct QrySuperUser {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 用户代码
        user_id:  String,
    }
    /// 查询交易所
    #[derive(Debug, Clone, Default)]
    struct QryExchange {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
    }
    /// 查询产品
    #[derive(Debug, Clone, Default)]
    struct QryProduct {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 产品类型
        product_class:  u8,
        /// 交易所代码
        exchange_id:  String,
        /// 产品代码
        product_id:  String,
    }
    /// 查询合约
    #[derive(Debug, Clone, Default)]
    struct QryInstrument {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// 产品代码
        product_id:  String,
    }
    /// 查询行情
    #[derive(Debug, Clone, Default)]
    struct QryDepthMarketData {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 产品类型
        product_class:  u8,
    }
    /// 查询经纪公司用户
    #[derive(Debug, Clone, Default)]
    struct QryBrokerUser {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
    }
    /// 查询经纪公司用户权限
    #[derive(Debug, Clone, Default)]
    struct QryBrokerUserFunction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
    }
    /// 查询交易员报盘机
    #[derive(Debug, Clone, Default)]
    struct QryTraderOffer {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
    }
    /// 查询出入金流水
    #[derive(Debug, Clone, Default)]
    struct QrySyncDeposit {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 出入金流水号
        deposit_seq_no:  Vec<u8>,
    }
    /// 查询投资者结算结果
    #[derive(Debug, Clone, Default)]
    struct QrySettlementInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易日
        trading_day:  String,
        /// 投资者帐号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
    }
    /// 查询交易所保证金率
    #[derive(Debug, Clone, Default)]
    struct QryExchangeMarginRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询交易所调整保证金率
    #[derive(Debug, Clone, Default)]
    struct QryExchangeMarginRateAdjust {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询汇率
    #[derive(Debug, Clone, Default)]
    struct QryExchangeRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 源币种
        from_currency_id:  String,
        /// 目标币种
        to_currency_id:  String,
    }
    /// 查询货币质押流水
    #[derive(Debug, Clone, Default)]
    struct QrySyncFundMortgage {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 货币质押流水号
        mortgage_seq_no:  Vec<u8>,
    }
    /// 查询报单
    #[derive(Debug, Clone, Default)]
    struct QryHisOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 报单编号
        order_sys_id:  String,
        /// 开始时间
        insert_time_start:  String,
        /// 结束时间
        insert_time_end:  String,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 合约代码
        instrument_id:  String,
    }
    /// 当前期权合约最小保证金
    #[derive(Debug, Clone, Default)]
    struct OptionInstrMiniMargin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 单位（手）期权合约最小保证金
        min_margin:  f64,
        /// 取值方式
        value_method:  u8,
        /// 是否跟随交易所收取
        is_relative:  i32,
        /// 合约代码
        instrument_id:  String,
    }
    /// 当前期权合约保证金调整系数
    #[derive(Debug, Clone, Default)]
    struct OptionInstrMarginAdjust {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机空头保证金调整系数
        s_short_margin_ratio_by_money:  f64,
        /// 投机空头保证金调整系数
        s_short_margin_ratio_by_volume:  f64,
        /// 保值空头保证金调整系数
        h_short_margin_ratio_by_money:  f64,
        /// 保值空头保证金调整系数
        h_short_margin_ratio_by_volume:  f64,
        /// 套利空头保证金调整系数
        a_short_margin_ratio_by_money:  f64,
        /// 套利空头保证金调整系数
        a_short_margin_ratio_by_volume:  f64,
        /// 是否跟随交易所收取
        is_relative:  i32,
        /// 做市商空头保证金调整系数
        m_short_margin_ratio_by_money:  f64,
        /// 做市商空头保证金调整系数
        m_short_margin_ratio_by_volume:  f64,
        /// 合约代码
        instrument_id:  String,
    }
    /// 当前期权合约手续费的详细内容
    #[derive(Debug, Clone, Default)]
    struct OptionInstrCommRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 开仓手续费率
        open_ratio_by_money:  f64,
        /// 开仓手续费
        open_ratio_by_volume:  f64,
        /// 平仓手续费率
        close_ratio_by_money:  f64,
        /// 平仓手续费
        close_ratio_by_volume:  f64,
        /// 平今手续费率
        close_today_ratio_by_money:  f64,
        /// 平今手续费
        close_today_ratio_by_volume:  f64,
        /// 执行手续费率
        strike_ratio_by_money:  f64,
        /// 执行手续费
        strike_ratio_by_volume:  f64,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 期权交易成本
    #[derive(Debug, Clone, Default)]
    struct OptionInstrTradeCost {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 期权合约保证金不变部分
        fixed_margin:  f64,
        /// 期权合约最小保证金
        mini_margin:  f64,
        /// 期权合约权利金
        royalty:  f64,
        /// 交易所期权合约保证金不变部分
        exch_fixed_margin:  f64,
        /// 交易所期权合约最小保证金
        exch_mini_margin:  f64,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 期权交易成本查询
    #[derive(Debug, Clone, Default)]
    struct QryOptionInstrTradeCost {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 期权合约报价
        input_price:  f64,
        /// 标的价格,填0则用昨结算价
        underlying_price:  f64,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 期权手续费率查询
    #[derive(Debug, Clone, Default)]
    struct QryOptionInstrCommRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 股指现货指数
    #[derive(Debug, Clone, Default)]
    struct IndexPrice {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 指数现货收盘价
        close_price:  f64,
        /// 合约代码
        instrument_id:  String,
    }
    /// 输入的执行宣告
    #[derive(Debug, Clone, Default)]
    struct InputExecOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 执行宣告引用
        exec_order_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 数量
        volume:  i32,
        /// 请求编号
        request_id:  i32,
        /// 业务单元
        business_unit:  String,
        /// 开平标志
        offset_flag:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 执行类型
        action_type:  u8,
        /// 保留头寸申请的持仓方向
        posi_direction:  u8,
        /// 期权行权后是否保留期货头寸的标记,该字段已废弃
        reserve_position_flag:  u8,
        /// 期权行权后生成的头寸是否自动平仓
        close_flag:  u8,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 资金账号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
        /// 交易编码
        client_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 输入执行宣告操作
    #[derive(Debug, Clone, Default)]
    struct InputExecOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 执行宣告操作引用
        exec_order_action_ref:  i32,
        /// 执行宣告引用
        exec_order_ref:  String,
        /// 请求编号
        request_id:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 执行宣告操作编号
        exec_order_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 用户代码
        user_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 执行宣告
    #[derive(Debug, Clone, Default)]
    struct ExecOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 执行宣告引用
        exec_order_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 数量
        volume:  i32,
        /// 请求编号
        request_id:  i32,
        /// 业务单元
        business_unit:  String,
        /// 开平标志
        offset_flag:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 执行类型
        action_type:  u8,
        /// 保留头寸申请的持仓方向
        posi_direction:  u8,
        /// 期权行权后是否保留期货头寸的标记,该字段已废弃
        reserve_position_flag:  u8,
        /// 期权行权后生成的头寸是否自动平仓
        close_flag:  u8,
        /// 本地执行宣告编号
        exec_order_local_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 执行宣告提交状态
        order_submit_status:  u8,
        /// 报单提示序号
        notify_sequence:  i32,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 执行宣告编号
        exec_order_sys_id:  String,
        /// 报单日期
        insert_date:  String,
        /// 插入时间
        insert_time:  String,
        /// 撤销时间
        cancel_time:  String,
        /// 执行结果
        exec_result:  u8,
        /// 结算会员编号
        clearing_part_id:  String,
        /// 序号
        sequence_no:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 用户端产品信息
        user_product_info:  String,
        /// 状态信息
        status_msg:  String,
        /// 操作用户代码
        active_user_id:  String,
        /// 经纪公司报单编号
        broker_exec_order_seq:  i32,
        /// 营业部编号
        branch_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 资金账号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 执行宣告操作
    #[derive(Debug, Clone, Default)]
    struct ExecOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 执行宣告操作引用
        exec_order_action_ref:  i32,
        /// 执行宣告引用
        exec_order_ref:  String,
        /// 请求编号
        request_id:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 执行宣告操作编号
        exec_order_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 操作日期
        action_date:  String,
        /// 操作时间
        action_time:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 本地执行宣告编号
        exec_order_local_id:  String,
        /// 操作本地编号
        action_local_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 业务单元
        business_unit:  String,
        /// 报单操作状态
        order_action_status:  u8,
        /// 用户代码
        user_id:  String,
        /// 执行类型
        action_type:  u8,
        /// 状态信息
        status_msg:  String,
        /// 营业部编号
        branch_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 执行宣告查询
    #[derive(Debug, Clone, Default)]
    struct QryExecOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 执行宣告编号
        exec_order_sys_id:  String,
        /// 开始时间
        insert_time_start:  String,
        /// 结束时间
        insert_time_end:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 交易所执行宣告信息
    #[derive(Debug, Clone, Default)]
    struct ExchangeExecOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 数量
        volume:  i32,
        /// 请求编号
        request_id:  i32,
        /// 业务单元
        business_unit:  String,
        /// 开平标志
        offset_flag:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 执行类型
        action_type:  u8,
        /// 保留头寸申请的持仓方向
        posi_direction:  u8,
        /// 期权行权后是否保留期货头寸的标记,该字段已废弃
        reserve_position_flag:  u8,
        /// 期权行权后生成的头寸是否自动平仓
        close_flag:  u8,
        /// 本地执行宣告编号
        exec_order_local_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 执行宣告提交状态
        order_submit_status:  u8,
        /// 报单提示序号
        notify_sequence:  i32,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 执行宣告编号
        exec_order_sys_id:  String,
        /// 报单日期
        insert_date:  String,
        /// 插入时间
        insert_time:  String,
        /// 撤销时间
        cancel_time:  String,
        /// 执行结果
        exec_result:  u8,
        /// 结算会员编号
        clearing_part_id:  String,
        /// 序号
        sequence_no:  i32,
        /// 营业部编号
        branch_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 交易所执行宣告查询
    #[derive(Debug, Clone, Default)]
    struct QryExchangeExecOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
    }
    /// 执行宣告操作查询
    #[derive(Debug, Clone, Default)]
    struct QryExecOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
    }
    /// 交易所执行宣告操作
    #[derive(Debug, Clone, Default)]
    struct ExchangeExecOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 执行宣告操作编号
        exec_order_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 操作日期
        action_date:  String,
        /// 操作时间
        action_time:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 本地执行宣告编号
        exec_order_local_id:  String,
        /// 操作本地编号
        action_local_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 业务单元
        business_unit:  String,
        /// 报单操作状态
        order_action_status:  u8,
        /// 用户代码
        user_id:  String,
        /// 执行类型
        action_type:  u8,
        /// 营业部编号
        branch_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 数量
        volume:  i32,
        /// IP地址
        ip_address:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
    }
    /// 交易所执行宣告操作查询
    #[derive(Debug, Clone, Default)]
    struct QryExchangeExecOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
    }
    /// 错误执行宣告
    #[derive(Debug, Clone, Default)]
    struct ErrExecOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 执行宣告引用
        exec_order_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 数量
        volume:  i32,
        /// 请求编号
        request_id:  i32,
        /// 业务单元
        business_unit:  String,
        /// 开平标志
        offset_flag:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 执行类型
        action_type:  u8,
        /// 保留头寸申请的持仓方向
        posi_direction:  u8,
        /// 期权行权后是否保留期货头寸的标记,该字段已废弃
        reserve_position_flag:  u8,
        /// 期权行权后生成的头寸是否自动平仓
        close_flag:  u8,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 资金账号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
        /// 交易编码
        client_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 查询错误执行宣告
    #[derive(Debug, Clone, Default)]
    struct QryErrExecOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
    }
    /// 错误执行宣告操作
    #[derive(Debug, Clone, Default)]
    struct ErrExecOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 执行宣告操作引用
        exec_order_action_ref:  i32,
        /// 执行宣告引用
        exec_order_ref:  String,
        /// 请求编号
        request_id:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 执行宣告操作编号
        exec_order_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 用户代码
        user_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 查询错误执行宣告操作
    #[derive(Debug, Clone, Default)]
    struct QryErrExecOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
    }
    /// 投资者期权合约交易权限
    #[derive(Debug, Clone, Default)]
    struct OptionInstrTradingRight {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 买卖方向
        direction:  u8,
        /// 交易权限
        trading_right:  u8,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询期权合约交易权限
    #[derive(Debug, Clone, Default)]
    struct QryOptionInstrTradingRight {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 买卖方向
        direction:  u8,
        /// 合约代码
        instrument_id:  String,
    }
    /// 输入的询价
    #[derive(Debug, Clone, Default)]
    struct InputForQuote {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 询价引用
        for_quote_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 询价
    #[derive(Debug, Clone, Default)]
    struct ForQuote {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 询价引用
        for_quote_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 本地询价编号
        for_quote_local_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 报单日期
        insert_date:  String,
        /// 插入时间
        insert_time:  String,
        /// 询价状态
        for_quote_status:  u8,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 状态信息
        status_msg:  String,
        /// 操作用户代码
        active_user_id:  String,
        /// 经纪公司询价编号
        broker_for_quote_seq:  i32,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 询价查询
    #[derive(Debug, Clone, Default)]
    struct QryForQuote {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 开始时间
        insert_time_start:  String,
        /// 结束时间
        insert_time_end:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 交易所询价信息
    #[derive(Debug, Clone, Default)]
    struct ExchangeForQuote {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 本地询价编号
        for_quote_local_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 报单日期
        insert_date:  String,
        /// 插入时间
        insert_time:  String,
        /// 询价状态
        for_quote_status:  u8,
        /// Mac地址
        mac_address:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 交易所询价查询
    #[derive(Debug, Clone, Default)]
    struct QryExchangeForQuote {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
    }
    /// 输入的报价
    #[derive(Debug, Clone, Default)]
    struct InputQuote {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报价引用
        quote_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 卖价格
        ask_price:  f64,
        /// 买价格
        bid_price:  f64,
        /// 卖数量
        ask_volume:  i32,
        /// 买数量
        bid_volume:  i32,
        /// 请求编号
        request_id:  i32,
        /// 业务单元
        business_unit:  String,
        /// 卖开平标志
        ask_offset_flag:  u8,
        /// 买开平标志
        bid_offset_flag:  u8,
        /// 卖投机套保标志
        ask_hedge_flag:  u8,
        /// 买投机套保标志
        bid_hedge_flag:  u8,
        /// 衍生卖报单引用
        ask_order_ref:  String,
        /// 衍生买报单引用
        bid_order_ref:  String,
        /// 应价编号
        for_quote_sys_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 交易编码
        client_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
        /// 被顶单编号
        replace_sys_id:  String,
        /// 有效期类型
        time_condition:  u8,
        /// 报单回显字段
        order_memo:  String,
        /// session上请求计数 api自动维护
        session_req_seq:  i32,
    }
    /// 输入报价操作
    #[derive(Debug, Clone, Default)]
    struct InputQuoteAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报价操作引用
        quote_action_ref:  i32,
        /// 报价引用
        quote_ref:  String,
        /// 请求编号
        request_id:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 报价操作编号
        quote_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 用户代码
        user_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 交易编码
        client_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
        /// 报单回显字段
        order_memo:  String,
        /// session上请求计数 api自动维护
        session_req_seq:  i32,
    }
    /// 报价
    #[derive(Debug, Clone, Default)]
    struct Quote {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报价引用
        quote_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 卖价格
        ask_price:  f64,
        /// 买价格
        bid_price:  f64,
        /// 卖数量
        ask_volume:  i32,
        /// 买数量
        bid_volume:  i32,
        /// 请求编号
        request_id:  i32,
        /// 业务单元
        business_unit:  String,
        /// 卖开平标志
        ask_offset_flag:  u8,
        /// 买开平标志
        bid_offset_flag:  u8,
        /// 卖投机套保标志
        ask_hedge_flag:  u8,
        /// 买投机套保标志
        bid_hedge_flag:  u8,
        /// 本地报价编号
        quote_local_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 报价提示序号
        notify_sequence:  i32,
        /// 报价提交状态
        order_submit_status:  u8,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 报价编号
        quote_sys_id:  String,
        /// 报单日期
        insert_date:  String,
        /// 插入时间
        insert_time:  String,
        /// 撤销时间
        cancel_time:  String,
        /// 报价状态
        quote_status:  u8,
        /// 结算会员编号
        clearing_part_id:  String,
        /// 序号
        sequence_no:  i32,
        /// 卖方报单编号
        ask_order_sys_id:  String,
        /// 买方报单编号
        bid_order_sys_id:  String,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 用户端产品信息
        user_product_info:  String,
        /// 状态信息
        status_msg:  String,
        /// 操作用户代码
        active_user_id:  String,
        /// 经纪公司报价编号
        broker_quote_seq:  i32,
        /// 衍生卖报单引用
        ask_order_ref:  String,
        /// 衍生买报单引用
        bid_order_ref:  String,
        /// 应价编号
        for_quote_sys_id:  String,
        /// 营业部编号
        branch_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 资金账号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// IP地址
        ip_address:  String,
        /// 被顶单编号
        replace_sys_id:  String,
        /// 有效期类型
        time_condition:  u8,
        /// 报单回显字段
        order_memo:  String,
        /// session上请求计数 api自动维护
        session_req_seq:  i32,
    }
    /// 报价操作
    #[derive(Debug, Clone, Default)]
    struct QuoteAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报价操作引用
        quote_action_ref:  i32,
        /// 报价引用
        quote_ref:  String,
        /// 请求编号
        request_id:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 报价操作编号
        quote_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 操作日期
        action_date:  String,
        /// 操作时间
        action_time:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 本地报价编号
        quote_local_id:  String,
        /// 操作本地编号
        action_local_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 业务单元
        business_unit:  String,
        /// 报单操作状态
        order_action_status:  u8,
        /// 用户代码
        user_id:  String,
        /// 状态信息
        status_msg:  String,
        /// 营业部编号
        branch_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
        /// 报单回显字段
        order_memo:  String,
        /// session上请求计数 api自动维护
        session_req_seq:  i32,
    }
    /// 报价查询
    #[derive(Debug, Clone, Default)]
    struct QryQuote {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 报价编号
        quote_sys_id:  String,
        /// 开始时间
        insert_time_start:  String,
        /// 结束时间
        insert_time_end:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 交易所报价信息
    #[derive(Debug, Clone, Default)]
    struct ExchangeQuote {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 卖价格
        ask_price:  f64,
        /// 买价格
        bid_price:  f64,
        /// 卖数量
        ask_volume:  i32,
        /// 买数量
        bid_volume:  i32,
        /// 请求编号
        request_id:  i32,
        /// 业务单元
        business_unit:  String,
        /// 卖开平标志
        ask_offset_flag:  u8,
        /// 买开平标志
        bid_offset_flag:  u8,
        /// 卖投机套保标志
        ask_hedge_flag:  u8,
        /// 买投机套保标志
        bid_hedge_flag:  u8,
        /// 本地报价编号
        quote_local_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 报价提示序号
        notify_sequence:  i32,
        /// 报价提交状态
        order_submit_status:  u8,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 报价编号
        quote_sys_id:  String,
        /// 报单日期
        insert_date:  String,
        /// 插入时间
        insert_time:  String,
        /// 撤销时间
        cancel_time:  String,
        /// 报价状态
        quote_status:  u8,
        /// 结算会员编号
        clearing_part_id:  String,
        /// 序号
        sequence_no:  i32,
        /// 卖方报单编号
        ask_order_sys_id:  String,
        /// 买方报单编号
        bid_order_sys_id:  String,
        /// 应价编号
        for_quote_sys_id:  String,
        /// 营业部编号
        branch_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// IP地址
        ip_address:  String,
        /// 有效期类型
        time_condition:  u8,
    }
    /// 交易所报价查询
    #[derive(Debug, Clone, Default)]
    struct QryExchangeQuote {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
    }
    /// 报价操作查询
    #[derive(Debug, Clone, Default)]
    struct QryQuoteAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
    }
    /// 交易所报价操作
    #[derive(Debug, Clone, Default)]
    struct ExchangeQuoteAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 报价操作编号
        quote_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 操作日期
        action_date:  String,
        /// 操作时间
        action_time:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 本地报价编号
        quote_local_id:  String,
        /// 操作本地编号
        action_local_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 业务单元
        business_unit:  String,
        /// 报单操作状态
        order_action_status:  u8,
        /// 用户代码
        user_id:  String,
        /// Mac地址
        mac_address:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 交易所报价操作查询
    #[derive(Debug, Clone, Default)]
    struct QryExchangeQuoteAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
    }
    /// 期权合约delta值
    #[derive(Debug, Clone, Default)]
    struct OptionInstrDelta {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// Delta值
        delta:  f64,
        /// 合约代码
        instrument_id:  String,
    }
    /// 发给做市商的询价请求
    #[derive(Debug, Clone, Default)]
    struct ForQuoteRsp {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 询价编号
        for_quote_sys_id:  String,
        /// 询价时间
        for_quote_time:  String,
        /// 业务日期
        action_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 当前期权合约执行偏移值的详细内容
    #[derive(Debug, Clone, Default)]
    struct StrikeOffset {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 执行偏移值
        offset:  f64,
        /// 执行偏移类型
        offset_type:  u8,
        /// 合约代码
        instrument_id:  String,
    }
    /// 期权执行偏移值查询
    #[derive(Debug, Clone, Default)]
    struct QryStrikeOffset {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 输入批量报单操作
    #[derive(Debug, Clone, Default)]
    struct InputBatchOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报单操作引用
        order_action_ref:  i32,
        /// 请求编号
        request_id:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 用户代码
        user_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 批量报单操作
    #[derive(Debug, Clone, Default)]
    struct BatchOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报单操作引用
        order_action_ref:  i32,
        /// 请求编号
        request_id:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 操作日期
        action_date:  String,
        /// 操作时间
        action_time:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 操作本地编号
        action_local_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 业务单元
        business_unit:  String,
        /// 报单操作状态
        order_action_status:  u8,
        /// 用户代码
        user_id:  String,
        /// 状态信息
        status_msg:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 交易所批量报单操作
    #[derive(Debug, Clone, Default)]
    struct ExchangeBatchOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 操作日期
        action_date:  String,
        /// 操作时间
        action_time:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 操作本地编号
        action_local_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 业务单元
        business_unit:  String,
        /// 报单操作状态
        order_action_status:  u8,
        /// 用户代码
        user_id:  String,
        /// Mac地址
        mac_address:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 查询批量报单操作
    #[derive(Debug, Clone, Default)]
    struct QryBatchOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
    }
    /// 组合合约安全系数
    #[derive(Debug, Clone, Default)]
    struct CombInstrumentGuard {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 担保比例
        guaranty_ratio:  f64,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 组合合约安全系数查询
    #[derive(Debug, Clone, Default)]
    struct QryCombInstrumentGuard {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 输入的申请组合
    #[derive(Debug, Clone, Default)]
    struct InputCombAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 组合引用
        comb_action_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 买卖方向
        direction:  u8,
        /// 数量
        volume:  i32,
        /// 组合指令方向
        comb_direction:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 交易所代码
        exchange_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 申请组合
    #[derive(Debug, Clone, Default)]
    struct CombAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 组合引用
        comb_action_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 买卖方向
        direction:  u8,
        /// 数量
        volume:  i32,
        /// 组合指令方向
        comb_direction:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 本地申请组合编号
        action_local_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 组合状态
        action_status:  u8,
        /// 报单提示序号
        notify_sequence:  i32,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 序号
        sequence_no:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 用户端产品信息
        user_product_info:  String,
        /// 状态信息
        status_msg:  String,
        /// Mac地址
        mac_address:  String,
        /// 组合编号
        com_trade_id:  String,
        /// 营业部编号
        branch_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 申请组合查询
    #[derive(Debug, Clone, Default)]
    struct QryCombAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 交易所申请组合信息
    #[derive(Debug, Clone, Default)]
    struct ExchangeCombAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 买卖方向
        direction:  u8,
        /// 数量
        volume:  i32,
        /// 组合指令方向
        comb_direction:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 本地申请组合编号
        action_local_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 组合状态
        action_status:  u8,
        /// 报单提示序号
        notify_sequence:  i32,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 序号
        sequence_no:  i32,
        /// Mac地址
        mac_address:  String,
        /// 组合编号
        com_trade_id:  String,
        /// 营业部编号
        branch_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 交易所申请组合查询
    #[derive(Debug, Clone, Default)]
    struct QryExchangeCombAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
    }
    /// 产品报价汇率
    #[derive(Debug, Clone, Default)]
    struct ProductExchRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 报价币种类型
        quote_currency_id:  String,
        /// 汇率
        exchange_rate:  f64,
        /// 交易所代码
        exchange_id:  String,
        /// 产品代码
        product_id:  String,
    }
    /// 产品报价汇率查询
    #[derive(Debug, Clone, Default)]
    struct QryProductExchRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 产品代码
        product_id:  String,
    }
    /// 查询询价价差参数
    #[derive(Debug, Clone, Default)]
    struct QryForQuoteParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 询价价差参数
    #[derive(Debug, Clone, Default)]
    struct ForQuoteParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 最新价
        last_price:  f64,
        /// 价差
        price_interval:  f64,
        /// 合约代码
        instrument_id:  String,
    }
    /// 当前做市商期权合约手续费的详细内容
    #[derive(Debug, Clone, Default)]
    struct MMOptionInstrCommRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 开仓手续费率
        open_ratio_by_money:  f64,
        /// 开仓手续费
        open_ratio_by_volume:  f64,
        /// 平仓手续费率
        close_ratio_by_money:  f64,
        /// 平仓手续费
        close_ratio_by_volume:  f64,
        /// 平今手续费率
        close_today_ratio_by_money:  f64,
        /// 平今手续费
        close_today_ratio_by_volume:  f64,
        /// 执行手续费率
        strike_ratio_by_money:  f64,
        /// 执行手续费
        strike_ratio_by_volume:  f64,
        /// 合约代码
        instrument_id:  String,
    }
    /// 做市商期权手续费率查询
    #[derive(Debug, Clone, Default)]
    struct QryMMOptionInstrCommRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 做市商合约手续费率
    #[derive(Debug, Clone, Default)]
    struct MMInstrumentCommissionRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 开仓手续费率
        open_ratio_by_money:  f64,
        /// 开仓手续费
        open_ratio_by_volume:  f64,
        /// 平仓手续费率
        close_ratio_by_money:  f64,
        /// 平仓手续费
        close_ratio_by_volume:  f64,
        /// 平今手续费率
        close_today_ratio_by_money:  f64,
        /// 平今手续费
        close_today_ratio_by_volume:  f64,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询做市商合约手续费率
    #[derive(Debug, Clone, Default)]
    struct QryMMInstrumentCommissionRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 当前报单手续费的详细内容
    #[derive(Debug, Clone, Default)]
    struct InstrumentOrderCommRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 报单手续费
        order_comm_by_volume:  f64,
        /// 撤单手续费
        order_action_comm_by_volume:  f64,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 报单手续费
        order_comm_by_trade:  f64,
        /// 撤单手续费
        order_action_comm_by_trade:  f64,
    }
    /// 报单手续费率查询
    #[derive(Debug, Clone, Default)]
    struct QryInstrumentOrderCommRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 交易参数
    #[derive(Debug, Clone, Default)]
    struct TradeParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 参数代码
        trade_param_id:  u8,
        /// 参数代码值
        trade_param_value:  Vec<u8>,
        /// 备注
        memo:  String,
    }
    /// 合约保证金率调整
    #[derive(Debug, Clone, Default)]
    struct InstrumentMarginRateUL {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 多头保证金率
        long_margin_ratio_by_money:  f64,
        /// 多头保证金费
        long_margin_ratio_by_volume:  f64,
        /// 空头保证金率
        short_margin_ratio_by_money:  f64,
        /// 空头保证金费
        short_margin_ratio_by_volume:  f64,
        /// 合约代码
        instrument_id:  String,
    }
    /// 期货持仓限制参数
    #[derive(Debug, Clone, Default)]
    struct FutureLimitPosiParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 当日投机开仓数量限制
        spec_open_volume:  i32,
        /// 当日套利开仓数量限制
        arbi_open_volume:  i32,
        /// 当日投机+套利开仓数量限制
        open_volume:  i32,
        /// 产品代码
        product_id:  String,
    }
    /// 禁止登录IP
    #[derive(Debug, Clone, Default)]
    struct LoginForbiddenIP {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// IP地址
        ip_address:  String,
    }
    /// IP列表
    #[derive(Debug, Clone, Default)]
    struct IPList {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 是否白名单
        is_white:  i32,
        /// IP地址
        ip_address:  String,
    }
    /// 输入的期权自对冲
    #[derive(Debug, Clone, Default)]
    struct InputOptionSelfClose {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 期权自对冲引用
        option_self_close_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 数量
        volume:  i32,
        /// 请求编号
        request_id:  i32,
        /// 业务单元
        business_unit:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 期权行权的头寸是否自对冲
        opt_self_close_flag:  u8,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 资金账号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
        /// 交易编码
        client_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 输入期权自对冲操作
    #[derive(Debug, Clone, Default)]
    struct InputOptionSelfCloseAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 期权自对冲操作引用
        option_self_close_action_ref:  i32,
        /// 期权自对冲引用
        option_self_close_ref:  String,
        /// 请求编号
        request_id:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 期权自对冲操作编号
        option_self_close_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 用户代码
        user_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 期权自对冲
    #[derive(Debug, Clone, Default)]
    struct OptionSelfClose {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 期权自对冲引用
        option_self_close_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 数量
        volume:  i32,
        /// 请求编号
        request_id:  i32,
        /// 业务单元
        business_unit:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 期权行权的头寸是否自对冲
        opt_self_close_flag:  u8,
        /// 本地期权自对冲编号
        option_self_close_local_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 期权自对冲提交状态
        order_submit_status:  u8,
        /// 报单提示序号
        notify_sequence:  i32,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 期权自对冲编号
        option_self_close_sys_id:  String,
        /// 报单日期
        insert_date:  String,
        /// 插入时间
        insert_time:  String,
        /// 撤销时间
        cancel_time:  String,
        /// 自对冲结果
        exec_result:  u8,
        /// 结算会员编号
        clearing_part_id:  String,
        /// 序号
        sequence_no:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 用户端产品信息
        user_product_info:  String,
        /// 状态信息
        status_msg:  String,
        /// 操作用户代码
        active_user_id:  String,
        /// 经纪公司报单编号
        broker_option_self_close_seq:  i32,
        /// 营业部编号
        branch_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 资金账号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 期权自对冲操作
    #[derive(Debug, Clone, Default)]
    struct OptionSelfCloseAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 期权自对冲操作引用
        option_self_close_action_ref:  i32,
        /// 期权自对冲引用
        option_self_close_ref:  String,
        /// 请求编号
        request_id:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 期权自对冲操作编号
        option_self_close_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 操作日期
        action_date:  String,
        /// 操作时间
        action_time:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 本地期权自对冲编号
        option_self_close_local_id:  String,
        /// 操作本地编号
        action_local_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 业务单元
        business_unit:  String,
        /// 报单操作状态
        order_action_status:  u8,
        /// 用户代码
        user_id:  String,
        /// 状态信息
        status_msg:  String,
        /// 营业部编号
        branch_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 期权自对冲查询
    #[derive(Debug, Clone, Default)]
    struct QryOptionSelfClose {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 期权自对冲编号
        option_self_close_sys_id:  String,
        /// 开始时间
        insert_time_start:  String,
        /// 结束时间
        insert_time_end:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 交易所期权自对冲信息
    #[derive(Debug, Clone, Default)]
    struct ExchangeOptionSelfClose {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 数量
        volume:  i32,
        /// 请求编号
        request_id:  i32,
        /// 业务单元
        business_unit:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 期权行权的头寸是否自对冲
        opt_self_close_flag:  u8,
        /// 本地期权自对冲编号
        option_self_close_local_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 期权自对冲提交状态
        order_submit_status:  u8,
        /// 报单提示序号
        notify_sequence:  i32,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 期权自对冲编号
        option_self_close_sys_id:  String,
        /// 报单日期
        insert_date:  String,
        /// 插入时间
        insert_time:  String,
        /// 撤销时间
        cancel_time:  String,
        /// 自对冲结果
        exec_result:  u8,
        /// 结算会员编号
        clearing_part_id:  String,
        /// 序号
        sequence_no:  i32,
        /// 营业部编号
        branch_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 期权自对冲操作查询
    #[derive(Debug, Clone, Default)]
    struct QryOptionSelfCloseAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
    }
    /// 交易所期权自对冲操作
    #[derive(Debug, Clone, Default)]
    struct ExchangeOptionSelfCloseAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 期权自对冲操作编号
        option_self_close_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 操作日期
        action_date:  String,
        /// 操作时间
        action_time:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 本地期权自对冲编号
        option_self_close_local_id:  String,
        /// 操作本地编号
        action_local_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 业务单元
        business_unit:  String,
        /// 报单操作状态
        order_action_status:  u8,
        /// 用户代码
        user_id:  String,
        /// 营业部编号
        branch_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 期权行权的头寸是否自对冲
        opt_self_close_flag:  u8,
        /// IP地址
        ip_address:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
    }
    /// 延时换汇同步
    #[derive(Debug, Clone, Default)]
    struct SyncDelaySwap {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 换汇流水号
        delay_swap_seq_no:  Vec<u8>,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 源币种
        from_currency_id:  String,
        /// 源金额
        from_amount:  f64,
        /// 源换汇冻结金额(可用冻结)
        from_frozen_swap:  f64,
        /// 源剩余换汇额度(可提冻结)
        from_remain_swap:  f64,
        /// 目标币种
        to_currency_id:  String,
        /// 目标金额
        to_amount:  f64,
        /// 是否手工换汇
        is_manual_swap:  i32,
        /// 是否将所有外币的剩余换汇额度设置为0
        is_all_remain_set_zero:  i32,
    }
    /// 查询延时换汇同步
    #[derive(Debug, Clone, Default)]
    struct QrySyncDelaySwap {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 延时换汇流水号
        delay_swap_seq_no:  Vec<u8>,
    }
    /// 投资单元
    #[derive(Debug, Clone, Default)]
    struct InvestUnit {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 投资者单元名称
        investor_unit_name:  String,
        /// 投资者分组代码
        investor_group_id:  String,
        /// 手续费率模板代码
        comm_model_id:  String,
        /// 保证金率模板代码
        margin_model_id:  String,
        /// 资金账号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
    }
    /// 查询投资单元
    #[derive(Debug, Clone, Default)]
    struct QryInvestUnit {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
    }
    /// 二级代理商资金校验模式
    #[derive(Debug, Clone, Default)]
    struct SecAgentCheckMode {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者代码
        investor_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 币种
        currency_id:  String,
        /// 境外中介机构资金帐号
        broker_sec_agent_id:  String,
        /// 是否需要校验自己的资金账户
        check_self_account:  i32,
    }
    /// 二级代理商信息
    #[derive(Debug, Clone, Default)]
    struct SecAgentTradeInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 境外中介机构资金帐号
        broker_sec_agent_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 二级代理商姓名
        long_customer_name:  String,
    }
    /// 市场行情
    #[derive(Debug, Clone, Default)]
    struct MarketData {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 最新价
        last_price:  f64,
        /// 上次结算价
        pre_settlement_price:  f64,
        /// 昨收盘
        pre_close_price:  f64,
        /// 昨持仓量
        pre_open_interest:  f64,
        /// 今开盘
        open_price:  f64,
        /// 最高价
        highest_price:  f64,
        /// 最低价
        lowest_price:  f64,
        /// 数量
        volume:  i32,
        /// 成交金额
        turnover:  f64,
        /// 持仓量
        open_interest:  f64,
        /// 今收盘
        close_price:  f64,
        /// 本次结算价
        settlement_price:  f64,
        /// 涨停板价
        upper_limit_price:  f64,
        /// 跌停板价
        lower_limit_price:  f64,
        /// 昨虚实度
        pre_delta:  f64,
        /// 今虚实度
        curr_delta:  f64,
        /// 最后修改时间
        update_time:  String,
        /// 最后修改毫秒
        update_millisecond:  i32,
        /// 业务日期
        action_day:  String,
        /// 合约代码
        instrument_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
    }
    /// 行情基础属性
    #[derive(Debug, Clone, Default)]
    struct MarketDataBase {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 上次结算价
        pre_settlement_price:  f64,
        /// 昨收盘
        pre_close_price:  f64,
        /// 昨持仓量
        pre_open_interest:  f64,
        /// 昨虚实度
        pre_delta:  f64,
    }
    /// 行情静态属性
    #[derive(Debug, Clone, Default)]
    struct MarketDataStatic {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 今开盘
        open_price:  f64,
        /// 最高价
        highest_price:  f64,
        /// 最低价
        lowest_price:  f64,
        /// 今收盘
        close_price:  f64,
        /// 涨停板价
        upper_limit_price:  f64,
        /// 跌停板价
        lower_limit_price:  f64,
        /// 本次结算价
        settlement_price:  f64,
        /// 今虚实度
        curr_delta:  f64,
    }
    /// 行情最新成交属性
    #[derive(Debug, Clone, Default)]
    struct MarketDataLastMatch {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 最新价
        last_price:  f64,
        /// 数量
        volume:  i32,
        /// 成交金额
        turnover:  f64,
        /// 持仓量
        open_interest:  f64,
    }
    /// 行情最优价属性
    #[derive(Debug, Clone, Default)]
    struct MarketDataBestPrice {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 申买价一
        bid_price1:  f64,
        /// 申买量一
        bid_volume1:  i32,
        /// 申卖价一
        ask_price1:  f64,
        /// 申卖量一
        ask_volume1:  i32,
    }
    /// 行情申买二、三属性
    #[derive(Debug, Clone, Default)]
    struct MarketDataBid23 {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 申买价二
        bid_price2:  f64,
        /// 申买量二
        bid_volume2:  i32,
        /// 申买价三
        bid_price3:  f64,
        /// 申买量三
        bid_volume3:  i32,
    }
    /// 行情申卖二、三属性
    #[derive(Debug, Clone, Default)]
    struct MarketDataAsk23 {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 申卖价二
        ask_price2:  f64,
        /// 申卖量二
        ask_volume2:  i32,
        /// 申卖价三
        ask_price3:  f64,
        /// 申卖量三
        ask_volume3:  i32,
    }
    /// 行情申买四、五属性
    #[derive(Debug, Clone, Default)]
    struct MarketDataBid45 {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 申买价四
        bid_price4:  f64,
        /// 申买量四
        bid_volume4:  i32,
        /// 申买价五
        bid_price5:  f64,
        /// 申买量五
        bid_volume5:  i32,
    }
    /// 行情申卖四、五属性
    #[derive(Debug, Clone, Default)]
    struct MarketDataAsk45 {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 申卖价四
        ask_price4:  f64,
        /// 申卖量四
        ask_volume4:  i32,
        /// 申卖价五
        ask_price5:  f64,
        /// 申卖量五
        ask_volume5:  i32,
    }
    /// 行情更新时间属性
    #[derive(Debug, Clone, Default)]
    struct MarketDataUpdateTime {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 最后修改时间
        update_time:  String,
        /// 最后修改毫秒
        update_millisecond:  i32,
        /// 业务日期
        action_day:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 行情上下带价
    #[derive(Debug, Clone, Default)]
    struct MarketDataBandingPrice {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 上带价
        banding_upper_price:  f64,
        /// 下带价
        banding_lower_price:  f64,
    }
    /// 行情交易所代码属性
    #[derive(Debug, Clone, Default)]
    struct MarketDataExchange {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
    }
    /// 指定的合约
    #[derive(Debug, Clone, Default)]
    struct SpecificInstrument {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 合约代码
        instrument_id:  String,
    }
    /// 合约状态
    #[derive(Debug, Clone, Default)]
    struct InstrumentStatus {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 结算组代码
        settlement_group_id:  String,
        /// 合约交易状态
        instrument_status:  u8,
        /// 交易阶段编号
        trading_segment_sn:  i32,
        /// 进入本状态时间
        enter_time:  String,
        /// 进入本状态原因
        enter_reason:  u8,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询合约状态
    #[derive(Debug, Clone, Default)]
    struct QryInstrumentStatus {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
    }
    /// 投资者账户
    #[derive(Debug, Clone, Default)]
    struct InvestorAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
    }
    /// 浮动盈亏算法
    #[derive(Debug, Clone, Default)]
    struct PositionProfitAlgorithm {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 盈亏算法
        algorithm:  u8,
        /// 备注
        memo:  String,
        /// 币种代码
        currency_id:  String,
    }
    /// 会员资金折扣
    #[derive(Debug, Clone, Default)]
    struct Discount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者范围
        investor_range:  u8,
        /// 投资者代码
        investor_id:  String,
        /// 资金折扣比例
        discount:  f64,
    }
    /// 查询转帐银行
    #[derive(Debug, Clone, Default)]
    struct QryTransferBank {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 银行代码
        bank_id:  String,
        /// 银行分中心代码
        bank_branch_id:  String,
    }
    /// 转帐银行
    #[derive(Debug, Clone, Default)]
    struct TransferBank {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 银行代码
        bank_id:  String,
        /// 银行分中心代码
        bank_branch_id:  String,
        /// 银行名称
        bank_name:  String,
        /// 是否活跃
        is_active:  i32,
    }
    /// 查询投资者持仓明细
    #[derive(Debug, Clone, Default)]
    struct QryInvestorPositionDetail {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 投资者持仓明细
    #[derive(Debug, Clone, Default)]
    struct InvestorPositionDetail {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 买卖
        direction:  u8,
        /// 开仓日期
        open_date:  String,
        /// 成交编号
        trade_id:  String,
        /// 数量
        volume:  i32,
        /// 开仓价
        open_price:  f64,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 成交类型
        trade_type:  u8,
        /// 交易所代码
        exchange_id:  String,
        /// 逐日盯市平仓盈亏
        close_profit_by_date:  f64,
        /// 逐笔对冲平仓盈亏
        close_profit_by_trade:  f64,
        /// 逐日盯市持仓盈亏
        position_profit_by_date:  f64,
        /// 逐笔对冲持仓盈亏
        position_profit_by_trade:  f64,
        /// 投资者保证金
        margin:  f64,
        /// 交易所保证金
        exch_margin:  f64,
        /// 保证金率
        margin_rate_by_money:  f64,
        /// 保证金率(按手数)
        margin_rate_by_volume:  f64,
        /// 昨结算价
        last_settlement_price:  f64,
        /// 结算价
        settlement_price:  f64,
        /// 平仓量
        close_volume:  i32,
        /// 平仓金额
        close_amount:  f64,
        /// 先开先平剩余数量
        time_first_volume:  i32,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 特殊持仓标志
        spec_posi_type:  u8,
        /// 合约代码
        instrument_id:  String,
        /// 组合合约代码
        comb_instrument_id:  String,
    }
    /// 资金账户口令域
    #[derive(Debug, Clone, Default)]
    struct TradingAccountPassword {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 密码
        password:  String,
        /// 币种代码
        currency_id:  String,
    }
    /// 交易所行情报盘机
    #[derive(Debug, Clone, Default)]
    struct MDTraderOffer {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 密码
        password:  String,
        /// 安装编号
        install_id:  i32,
        /// 本地报单编号
        order_local_id:  String,
        /// 交易所交易员连接状态
        trader_connect_status:  u8,
        /// 发出连接请求的日期
        connect_request_date:  String,
        /// 发出连接请求的时间
        connect_request_time:  String,
        /// 上次报告日期
        last_report_date:  String,
        /// 上次报告时间
        last_report_time:  String,
        /// 完成连接日期
        connect_date:  String,
        /// 完成连接时间
        connect_time:  String,
        /// 启动日期
        start_date:  String,
        /// 启动时间
        start_time:  String,
        /// 交易日
        trading_day:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 本席位最大成交编号
        max_trade_id:  String,
        /// 本席位最大报单备拷
        max_order_message_reference:  Vec<u8>,
        /// 撤单时选择席位算法
        order_cancel_alg:  u8,
    }
    /// 查询行情报盘机
    #[derive(Debug, Clone, Default)]
    struct QryMDTraderOffer {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
    }
    /// 查询客户通知
    #[derive(Debug, Clone, Default)]
    struct QryNotice {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
    }
    /// 客户通知
    #[derive(Debug, Clone, Default)]
    struct Notice {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 消息正文
        content:  Vec<u8>,
        /// 经纪公司通知内容序列号
        sequence_label:  Vec<u8>,
    }
    /// 用户权限
    #[derive(Debug, Clone, Default)]
    struct UserRight {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 客户权限类型
        user_right_type:  u8,
        /// 是否禁止
        is_forbidden:  i32,
    }
    /// 查询结算信息确认域
    #[derive(Debug, Clone, Default)]
    struct QrySettlementInfoConfirm {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
    }
    /// 装载结算信息
    #[derive(Debug, Clone, Default)]
    struct LoadSettlementInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
    }
    /// 经纪公司可提资金算法表
    #[derive(Debug, Clone, Default)]
    struct BrokerWithdrawAlgorithm {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 可提资金算法
        withdraw_algorithm:  u8,
        /// 资金使用率
        using_ratio:  f64,
        /// 可提是否包含平仓盈利
        include_close_profit:  u8,
        /// 本日无仓且无成交客户是否受可提比例限制
        all_without_trade:  u8,
        /// 可用是否包含平仓盈利
        avail_include_close_profit:  u8,
        /// 是否启用用户事件
        is_broker_user_event:  i32,
        /// 币种代码
        currency_id:  String,
        /// 货币质押比率
        fund_mortgage_ratio:  f64,
        /// 权益算法
        balance_algorithm:  u8,
    }
    /// 资金账户口令变更域
    #[derive(Debug, Clone, Default)]
    struct TradingAccountPasswordUpdateV1 {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 原来的口令
        old_password:  String,
        /// 新的口令
        new_password:  String,
    }
    /// 资金账户口令变更域
    #[derive(Debug, Clone, Default)]
    struct TradingAccountPasswordUpdate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 原来的口令
        old_password:  String,
        /// 新的口令
        new_password:  String,
        /// 币种代码
        currency_id:  String,
    }
    /// 查询组合合约分腿
    #[derive(Debug, Clone, Default)]
    struct QryCombinationLeg {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 单腿编号
        leg_id:  i32,
        /// 组合合约代码
        comb_instrument_id:  String,
        /// 单腿合约代码
        leg_instrument_id:  String,
    }
    /// 查询组合合约分腿
    #[derive(Debug, Clone, Default)]
    struct QrySyncStatus {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
    }
    /// 组合交易合约的单腿
    #[derive(Debug, Clone, Default)]
    struct CombinationLeg {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 单腿编号
        leg_id:  i32,
        /// 买卖方向
        direction:  u8,
        /// 单腿乘数
        leg_multiple:  i32,
        /// 派生层数
        imply_level:  i32,
        /// 组合合约代码
        comb_instrument_id:  String,
        /// 单腿合约代码
        leg_instrument_id:  String,
    }
    /// 数据同步状态
    #[derive(Debug, Clone, Default)]
    struct SyncStatus {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 数据同步状态
        data_sync_status:  u8,
    }
    /// 查询联系人
    #[derive(Debug, Clone, Default)]
    struct QryLinkMan {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
    }
    /// 联系人
    #[derive(Debug, Clone, Default)]
    struct LinkMan {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 联系人类型
        person_type:  u8,
        /// 证件类型
        identified_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 名称
        person_name:  String,
        /// 联系电话
        telephone:  String,
        /// 通讯地址
        address:  String,
        /// 邮政编码
        zip_code:  String,
        /// 优先级
        priority:  i32,
        /// 开户邮政编码
        uoa_zip_code:  String,
        /// 全称
        person_full_name:  String,
    }
    /// 查询经纪公司用户事件
    #[derive(Debug, Clone, Default)]
    struct QryBrokerUserEvent {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 用户事件类型
        user_event_type:  u8,
    }
    /// 查询经纪公司用户事件
    #[derive(Debug, Clone, Default)]
    struct BrokerUserEvent {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 用户事件类型
        user_event_type:  u8,
        /// 用户事件序号
        event_sequence_no:  i32,
        /// 事件发生日期
        event_date:  String,
        /// 事件发生时间
        event_time:  String,
        /// 用户事件信息
        user_event_info:  String,
        /// 投资者代码
        investor_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 交易中心代码
        dr_identity_id:  i32,
        /// 交易日
        trading_day:  String,
    }
    /// 查询签约银行请求
    #[derive(Debug, Clone, Default)]
    struct QryContractBank {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分中心代码
        bank_branch_id:  String,
    }
    /// 查询签约银行响应
    #[derive(Debug, Clone, Default)]
    struct ContractBank {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分中心代码
        bank_branch_id:  String,
        /// 银行名称
        bank_name:  String,
    }
    /// 投资者组合持仓明细
    #[derive(Debug, Clone, Default)]
    struct InvestorPositionCombineDetail {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 开仓日期
        open_date:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 组合编号
        com_trade_id:  String,
        /// 撮合编号
        trade_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 买卖
        direction:  u8,
        /// 持仓量
        total_amt:  i32,
        /// 投资者保证金
        margin:  f64,
        /// 交易所保证金
        exch_margin:  f64,
        /// 保证金率
        margin_rate_by_money:  f64,
        /// 保证金率(按手数)
        margin_rate_by_volume:  f64,
        /// 单腿编号
        leg_id:  i32,
        /// 单腿乘数
        leg_multiple:  i32,
        /// 成交组号
        trade_group_id:  i32,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 组合持仓合约编码
        comb_instrument_id:  String,
    }
    /// 预埋单
    #[derive(Debug, Clone, Default)]
    struct ParkedOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报单引用
        order_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 报单价格条件
        order_price_type:  u8,
        /// 买卖方向
        direction:  u8,
        /// 组合开平标志
        comb_offset_flag:  String,
        /// 组合投机套保标志
        comb_hedge_flag:  String,
        /// 价格
        limit_price:  f64,
        /// 数量
        volume_total_original:  i32,
        /// 有效期类型
        time_condition:  u8,
        /// GTD日期
        gtd_date:  String,
        /// 成交量类型
        volume_condition:  u8,
        /// 最小成交量
        min_volume:  i32,
        /// 触发条件
        contingent_condition:  u8,
        /// 止损价
        stop_price:  f64,
        /// 强平原因
        force_close_reason:  u8,
        /// 自动挂起标志
        is_auto_suspend:  i32,
        /// 业务单元
        business_unit:  String,
        /// 请求编号
        request_id:  i32,
        /// 用户强平标志
        user_force_close:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 预埋报单编号
        parked_order_id:  String,
        /// 用户类型
        user_type:  u8,
        /// 预埋单状态
        status:  u8,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 互换单标志
        is_swap_order:  i32,
        /// 资金账号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
        /// 交易编码
        client_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 输入预埋单操作
    #[derive(Debug, Clone, Default)]
    struct ParkedOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报单操作引用
        order_action_ref:  i32,
        /// 报单引用
        order_ref:  String,
        /// 请求编号
        request_id:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 报单编号
        order_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 价格
        limit_price:  f64,
        /// 数量变化
        volume_change:  i32,
        /// 用户代码
        user_id:  String,
        /// 预埋撤单单编号
        parked_order_action_id:  String,
        /// 用户类型
        user_type:  u8,
        /// 预埋撤单状态
        status:  u8,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 查询预埋单
    #[derive(Debug, Clone, Default)]
    struct QryParkedOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询预埋撤单
    #[derive(Debug, Clone, Default)]
    struct QryParkedOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 删除预埋单
    #[derive(Debug, Clone, Default)]
    struct RemoveParkedOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 预埋报单编号
        parked_order_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
    }
    /// 删除预埋撤单
    #[derive(Debug, Clone, Default)]
    struct RemoveParkedOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 预埋撤单编号
        parked_order_action_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
    }
    /// 经纪公司可提资金算法表
    #[derive(Debug, Clone, Default)]
    struct InvestorWithdrawAlgorithm {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者范围
        investor_range:  u8,
        /// 投资者代码
        investor_id:  String,
        /// 可提资金比例
        using_ratio:  f64,
        /// 币种代码
        currency_id:  String,
        /// 货币质押比率
        fund_mortgage_ratio:  f64,
    }
    /// 查询组合持仓明细
    #[derive(Debug, Clone, Default)]
    struct QryInvestorPositionCombineDetail {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 组合持仓合约编码
        comb_instrument_id:  String,
    }
    /// 成交均价
    #[derive(Debug, Clone, Default)]
    struct MarketDataAveragePrice {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 当日均价
        average_price:  f64,
    }
    /// 校验投资者密码
    #[derive(Debug, Clone, Default)]
    struct VerifyInvestorPassword {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 密码
        password:  String,
    }
    /// 用户IP
    #[derive(Debug, Clone, Default)]
    struct UserIP {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// Mac地址
        mac_address:  String,
        /// IP地址
        ip_address:  String,
        /// IP地址掩码
        ip_mask:  Vec<u8>,
    }
    /// 用户事件通知信息
    #[derive(Debug, Clone, Default)]
    struct TradingNoticeInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 发送时间
        send_time:  String,
        /// 消息正文
        field_content:  Vec<u8>,
        /// 序列系列号
        sequence_series:  u16,
        /// 序列号
        sequence_no:  i32,
        /// 投资单元代码
        invest_unit_id:  String,
    }
    /// 用户事件通知
    #[derive(Debug, Clone, Default)]
    struct TradingNotice {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者范围
        investor_range:  u8,
        /// 投资者代码
        investor_id:  String,
        /// 序列系列号
        sequence_series:  u16,
        /// 用户代码
        user_id:  String,
        /// 发送时间
        send_time:  String,
        /// 序列号
        sequence_no:  i32,
        /// 消息正文
        field_content:  Vec<u8>,
        /// 投资单元代码
        invest_unit_id:  String,
    }
    /// 查询交易事件通知
    #[derive(Debug, Clone, Default)]
    struct QryTradingNotice {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
    }
    /// 查询错误报单
    #[derive(Debug, Clone, Default)]
    struct QryErrOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
    }
    /// 错误报单
    #[derive(Debug, Clone, Default)]
    struct ErrOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报单引用
        order_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 报单价格条件
        order_price_type:  u8,
        /// 买卖方向
        direction:  u8,
        /// 组合开平标志
        comb_offset_flag:  String,
        /// 组合投机套保标志
        comb_hedge_flag:  String,
        /// 价格
        limit_price:  f64,
        /// 数量
        volume_total_original:  i32,
        /// 有效期类型
        time_condition:  u8,
        /// GTD日期
        gtd_date:  String,
        /// 成交量类型
        volume_condition:  u8,
        /// 最小成交量
        min_volume:  i32,
        /// 触发条件
        contingent_condition:  u8,
        /// 止损价
        stop_price:  f64,
        /// 强平原因
        force_close_reason:  u8,
        /// 自动挂起标志
        is_auto_suspend:  i32,
        /// 业务单元
        business_unit:  String,
        /// 请求编号
        request_id:  i32,
        /// 用户强平标志
        user_force_close:  i32,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 互换单标志
        is_swap_order:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 资金账号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
        /// 交易编码
        client_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
        /// 报单回显字段
        order_memo:  String,
        /// session上请求计数 api自动维护
        session_req_seq:  i32,
    }
    /// 查询错误报单操作
    #[derive(Debug, Clone, Default)]
    struct ErrorConditionalOrder {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报单引用
        order_ref:  String,
        /// 用户代码
        user_id:  String,
        /// 报单价格条件
        order_price_type:  u8,
        /// 买卖方向
        direction:  u8,
        /// 组合开平标志
        comb_offset_flag:  String,
        /// 组合投机套保标志
        comb_hedge_flag:  String,
        /// 价格
        limit_price:  f64,
        /// 数量
        volume_total_original:  i32,
        /// 有效期类型
        time_condition:  u8,
        /// GTD日期
        gtd_date:  String,
        /// 成交量类型
        volume_condition:  u8,
        /// 最小成交量
        min_volume:  i32,
        /// 触发条件
        contingent_condition:  u8,
        /// 止损价
        stop_price:  f64,
        /// 强平原因
        force_close_reason:  u8,
        /// 自动挂起标志
        is_auto_suspend:  i32,
        /// 业务单元
        business_unit:  String,
        /// 请求编号
        request_id:  i32,
        /// 本地报单编号
        order_local_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 报单提交状态
        order_submit_status:  u8,
        /// 报单提示序号
        notify_sequence:  i32,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 报单编号
        order_sys_id:  String,
        /// 报单来源
        order_source:  u8,
        /// 报单状态
        order_status:  u8,
        /// 报单类型
        order_type:  u8,
        /// 今成交数量
        volume_traded:  i32,
        /// 剩余数量
        volume_total:  i32,
        /// 报单日期
        insert_date:  String,
        /// 委托时间
        insert_time:  String,
        /// 激活时间
        active_time:  String,
        /// 挂起时间
        suspend_time:  String,
        /// 最后修改时间
        update_time:  String,
        /// 撤销时间
        cancel_time:  String,
        /// 最后修改交易所交易员代码
        active_trader_id:  String,
        /// 结算会员编号
        clearing_part_id:  String,
        /// 序号
        sequence_no:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 用户端产品信息
        user_product_info:  String,
        /// 状态信息
        status_msg:  String,
        /// 用户强平标志
        user_force_close:  i32,
        /// 操作用户代码
        active_user_id:  String,
        /// 经纪公司报单编号
        broker_order_seq:  i32,
        /// 相关报单
        relative_order_sys_id:  String,
        /// 郑商所成交数量
        zce_total_traded_volume:  i32,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 互换单标志
        is_swap_order:  i32,
        /// 营业部编号
        branch_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 资金账号
        account_id:  String,
        /// 币种代码
        currency_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 合约代码
        instrument_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 查询错误报单操作
    #[derive(Debug, Clone, Default)]
    struct QryErrOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
    }
    /// 错误报单操作
    #[derive(Debug, Clone, Default)]
    struct ErrOrderAction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 报单操作引用
        order_action_ref:  i32,
        /// 报单引用
        order_ref:  String,
        /// 请求编号
        request_id:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 报单编号
        order_sys_id:  String,
        /// 操作标志
        action_flag:  u8,
        /// 价格
        limit_price:  f64,
        /// 数量变化
        volume_change:  i32,
        /// 操作日期
        action_date:  String,
        /// 操作时间
        action_time:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 本地报单编号
        order_local_id:  String,
        /// 操作本地编号
        action_local_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 业务单元
        business_unit:  String,
        /// 报单操作状态
        order_action_status:  u8,
        /// 用户代码
        user_id:  String,
        /// 状态信息
        status_msg:  String,
        /// 营业部编号
        branch_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// Mac地址
        mac_address:  String,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 合约代码
        instrument_id:  String,
        /// IP地址
        ip_address:  String,
        /// 报单回显字段
        order_memo:  String,
        /// session上请求计数 api自动维护
        session_req_seq:  i32,
    }
    /// 查询交易所状态
    #[derive(Debug, Clone, Default)]
    struct QryExchangeSequence {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
    }
    /// 交易所状态
    #[derive(Debug, Clone, Default)]
    struct ExchangeSequence {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 序号
        sequence_no:  i32,
        /// 合约交易状态
        market_status:  u8,
    }
    /// 根据价格查询最大报单数量
    #[derive(Debug, Clone, Default)]
    struct QryMaxOrderVolumeWithPrice {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 买卖方向
        direction:  u8,
        /// 开平标志
        offset_flag:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 最大允许报单数量
        max_volume:  i32,
        /// 报单价格
        price:  f64,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询经纪公司交易参数
    #[derive(Debug, Clone, Default)]
    struct QryBrokerTradingParams {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 币种代码
        currency_id:  String,
        /// 投资者帐号
        account_id:  String,
    }
    /// 经纪公司交易参数
    #[derive(Debug, Clone, Default)]
    struct BrokerTradingParams {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 保证金价格类型
        margin_price_type:  u8,
        /// 盈亏算法
        algorithm:  u8,
        /// 可用是否包含平仓盈利
        avail_include_close_profit:  u8,
        /// 币种代码
        currency_id:  String,
        /// 期权权利金价格类型
        option_royalty_price_type:  u8,
        /// 投资者帐号
        account_id:  String,
    }
    /// 查询经纪公司交易算法
    #[derive(Debug, Clone, Default)]
    struct QryBrokerTradingAlgos {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 经纪公司交易算法
    #[derive(Debug, Clone, Default)]
    struct BrokerTradingAlgos {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 持仓处理算法编号
        handle_position_algo_id:  u8,
        /// 寻找保证金率算法编号
        find_margin_rate_algo_id:  u8,
        /// 资金处理算法编号
        handle_trading_account_algo_id:  u8,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询经纪公司资金
    #[derive(Debug, Clone, Default)]
    struct QueryBrokerDeposit {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 交易所代码
        exchange_id:  String,
    }
    /// 经纪公司资金
    #[derive(Debug, Clone, Default)]
    struct BrokerDeposit {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日期
        trading_day:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 上次结算准备金
        pre_balance:  f64,
        /// 当前保证金总额
        curr_margin:  f64,
        /// 平仓盈亏
        close_profit:  f64,
        /// 期货结算准备金
        balance:  f64,
        /// 入金金额
        deposit:  f64,
        /// 出金金额
        withdraw:  f64,
        /// 可提资金
        available:  f64,
        /// 基本准备金
        reserve:  f64,
        /// 冻结的保证金
        frozen_margin:  f64,
    }
    /// 查询保证金监管系统经纪公司密钥
    #[derive(Debug, Clone, Default)]
    struct QryCFMMCBrokerKey {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
    }
    /// 保证金监管系统经纪公司密钥
    #[derive(Debug, Clone, Default)]
    struct CFMMCBrokerKey {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 经纪公司统一编码
        participant_id:  String,
        /// 密钥生成日期
        create_date:  String,
        /// 密钥生成时间
        create_time:  String,
        /// 密钥编号
        key_id:  i32,
        /// 动态密钥
        current_key:  Vec<u8>,
        /// 动态密钥类型
        key_kind:  u8,
    }
    /// 保证金监管系统经纪公司资金账户密钥
    #[derive(Debug, Clone, Default)]
    struct CFMMCTradingAccountKey {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 经纪公司统一编码
        participant_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 密钥编号
        key_id:  i32,
        /// 动态密钥
        current_key:  Vec<u8>,
    }
    /// 请求查询保证金监管系统经纪公司资金账户密钥
    #[derive(Debug, Clone, Default)]
    struct QryCFMMCTradingAccountKey {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
    }
    /// 用户动态令牌参数
    #[derive(Debug, Clone, Default)]
    struct BrokerUserOTPParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 动态令牌提供商
        otp_vendors_id:  String,
        /// 动态令牌序列号
        serial_number:  Vec<u8>,
        /// 令牌密钥
        auth_key:  Vec<u8>,
        /// 漂移值
        last_drift:  i32,
        /// 成功值
        last_success:  i32,
        /// 动态令牌类型
        otp_type:  u8,
    }
    /// 手工同步用户动态令牌
    #[derive(Debug, Clone, Default)]
    struct ManualSyncBrokerUserOTP {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 动态令牌类型
        otp_type:  u8,
        /// 第一个动态密码
        first_otp:  Vec<u8>,
        /// 第二个动态密码
        second_otp:  Vec<u8>,
    }
    /// 投资者手续费率模板
    #[derive(Debug, Clone, Default)]
    struct CommRateModel {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 手续费率模板代码
        comm_model_id:  String,
        /// 模板名称
        comm_model_name:  String,
    }
    /// 请求查询投资者手续费率模板
    #[derive(Debug, Clone, Default)]
    struct QryCommRateModel {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 手续费率模板代码
        comm_model_id:  String,
    }
    /// 投资者保证金率模板
    #[derive(Debug, Clone, Default)]
    struct MarginModel {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 保证金率模板代码
        margin_model_id:  String,
        /// 模板名称
        margin_model_name:  String,
    }
    /// 请求查询投资者保证金率模板
    #[derive(Debug, Clone, Default)]
    struct QryMarginModel {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 保证金率模板代码
        margin_model_id:  String,
    }
    /// 仓单折抵信息
    #[derive(Debug, Clone, Default)]
    struct EWarrantOffset {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日期
        trading_day:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 买卖方向
        direction:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 数量
        volume:  i32,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询仓单折抵信息
    #[derive(Debug, Clone, Default)]
    struct QryEWarrantOffset {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 查询投资者品种/跨品种保证金
    #[derive(Debug, Clone, Default)]
    struct QryInvestorProductGroupMargin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 品种/跨品种标示
        product_group_id:  String,
    }
    /// 投资者品种/跨品种保证金
    #[derive(Debug, Clone, Default)]
    struct InvestorProductGroupMargin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 冻结的保证金
        frozen_margin:  f64,
        /// 多头冻结的保证金
        long_frozen_margin:  f64,
        /// 空头冻结的保证金
        short_frozen_margin:  f64,
        /// 占用的保证金
        use_margin:  f64,
        /// 多头保证金
        long_use_margin:  f64,
        /// 空头保证金
        short_use_margin:  f64,
        /// 交易所保证金
        exch_margin:  f64,
        /// 交易所多头保证金
        long_exch_margin:  f64,
        /// 交易所空头保证金
        short_exch_margin:  f64,
        /// 平仓盈亏
        close_profit:  f64,
        /// 冻结的手续费
        frozen_commission:  f64,
        /// 手续费
        commission:  f64,
        /// 冻结的资金
        frozen_cash:  f64,
        /// 资金差额
        cash_in:  f64,
        /// 持仓盈亏
        position_profit:  f64,
        /// 折抵总金额
        offset_amount:  f64,
        /// 多头折抵总金额
        long_offset_amount:  f64,
        /// 空头折抵总金额
        short_offset_amount:  f64,
        /// 交易所折抵总金额
        exch_offset_amount:  f64,
        /// 交易所多头折抵总金额
        long_exch_offset_amount:  f64,
        /// 交易所空头折抵总金额
        short_exch_offset_amount:  f64,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 交易所代码
        exchange_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 品种/跨品种标示
        product_group_id:  String,
    }
    /// 查询监控中心用户令牌
    #[derive(Debug, Clone, Default)]
    struct QueryCFMMCTradingAccountToken {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投资单元代码
        invest_unit_id:  String,
    }
    /// 监控中心用户令牌
    #[derive(Debug, Clone, Default)]
    struct CFMMCTradingAccountToken {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 经纪公司统一编码
        participant_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 密钥编号
        key_id:  i32,
        /// 动态令牌
        token:  Vec<u8>,
    }
    /// 查询产品组
    #[derive(Debug, Clone, Default)]
    struct QryProductGroup {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 产品代码
        product_id:  String,
    }
    /// 投资者品种/跨品种保证金产品组
    #[derive(Debug, Clone, Default)]
    struct ProductGroup {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 产品代码
        product_id:  String,
        /// 产品组代码
        product_group_id:  String,
    }
    /// 交易所公告
    #[derive(Debug, Clone, Default)]
    struct Bulletin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 交易日
        trading_day:  String,
        /// 公告编号
        bulletin_id:  i32,
        /// 序列号
        sequence_no:  i32,
        /// 公告类型
        news_type:  Vec<u8>,
        /// 紧急程度
        news_urgency:  u8,
        /// 发送时间
        send_time:  String,
        /// 消息摘要

        abstracts: Vec<u8>,
        /// 消息来源
        come_from:  Vec<u8>,
        /// 消息正文
        content:  Vec<u8>,
        /// WEB地址
        url_link:  Vec<u8>,
        /// 市场代码
        market_id:  String,
    }
    /// 查询交易所公告
    #[derive(Debug, Clone, Default)]
    struct QryBulletin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 公告编号
        bulletin_id:  i32,
        /// 序列号
        sequence_no:  i32,
        /// 公告类型
        news_type:  Vec<u8>,
        /// 紧急程度
        news_urgency:  u8,
    }
    /// MulticastInstrument
    #[derive(Debug, Clone, Default)]
    struct MulticastInstrument {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 主题号
        topic_id:  i32,
        /// 合约编号
        instrument_no:  i32,
        /// 基准价
        code_price:  f64,
        /// 合约数量乘数
        volume_multiple:  i32,
        /// 最小变动价位
        price_tick:  f64,
        /// 合约代码
        instrument_id:  String,
    }
    /// QryMulticastInstrument
    #[derive(Debug, Clone, Default)]
    struct QryMulticastInstrument {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 主题号
        topic_id:  i32,
        /// 合约代码
        instrument_id:  String,
    }
    /// App客户端权限分配
    #[derive(Debug, Clone, Default)]
    struct AppIDAuthAssign {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// App代码
        app_id:  String,
        /// 交易中心代码
        dr_identity_id:  i32,
    }
    /// 转帐开户请求
    #[derive(Debug, Clone, Default)]
    struct ReqOpenAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 性别
        gender:  u8,
        /// 国家代码
        country_code:  String,
        /// 客户类型
        cust_type:  u8,
        /// 地址
        address:  String,
        /// 邮编
        zip_code:  String,
        /// 电话号码
        telephone:  String,
        /// 手机
        mobile_phone:  String,
        /// 传真
        fax:  String,
        /// 电子邮件
        email:  String,
        /// 资金账户状态
        money_account_status:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 安装编号
        install_id:  i32,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 汇钞标志
        cash_exchange_code:  u8,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 交易ID
        tid:  i32,
        /// 用户标识
        user_id:  String,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 转帐销户请求
    #[derive(Debug, Clone, Default)]
    struct ReqCancelAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 性别
        gender:  u8,
        /// 国家代码
        country_code:  String,
        /// 客户类型
        cust_type:  u8,
        /// 地址
        address:  String,
        /// 邮编
        zip_code:  String,
        /// 电话号码
        telephone:  String,
        /// 手机
        mobile_phone:  String,
        /// 传真
        fax:  String,
        /// 电子邮件
        email:  String,
        /// 资金账户状态
        money_account_status:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 安装编号
        install_id:  i32,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 汇钞标志
        cash_exchange_code:  u8,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 交易ID
        tid:  i32,
        /// 用户标识
        user_id:  String,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 变更银行账户请求
    #[derive(Debug, Clone, Default)]
    struct ReqChangeAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 性别
        gender:  u8,
        /// 国家代码
        country_code:  String,
        /// 客户类型
        cust_type:  u8,
        /// 地址
        address:  String,
        /// 邮编
        zip_code:  String,
        /// 电话号码
        telephone:  String,
        /// 手机
        mobile_phone:  String,
        /// 传真
        fax:  String,
        /// 电子邮件
        email:  String,
        /// 资金账户状态
        money_account_status:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 新银行帐号
        new_bank_account:  String,
        /// 新银行密码
        new_bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 安装编号
        install_id:  i32,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易ID
        tid:  i32,
        /// 摘要
        digest:  Vec<u8>,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 转账请求
    #[derive(Debug, Clone, Default)]
    struct ReqTransfer {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 安装编号
        install_id:  i32,
        /// 期货公司流水号
        future_serial:  i32,
        /// 用户标识
        user_id:  String,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 转帐金额
        trade_amount:  f64,
        /// 期货可取金额
        future_fetch_amount:  f64,
        /// 费用支付标志
        fee_pay_flag:  u8,
        /// 应收客户费用
        cust_fee:  f64,
        /// 应收期货公司费用
        broker_fee:  f64,
        /// 发送方给接收方的消息
        message:  Vec<u8>,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 转账交易状态
        transfer_status:  u8,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 银行发起银行资金转期货响应
    #[derive(Debug, Clone, Default)]
    struct RspTransfer {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 安装编号
        install_id:  i32,
        /// 期货公司流水号
        future_serial:  i32,
        /// 用户标识
        user_id:  String,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 转帐金额
        trade_amount:  f64,
        /// 期货可取金额
        future_fetch_amount:  f64,
        /// 费用支付标志
        fee_pay_flag:  u8,
        /// 应收客户费用
        cust_fee:  f64,
        /// 应收期货公司费用
        broker_fee:  f64,
        /// 发送方给接收方的消息
        message:  Vec<u8>,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 转账交易状态
        transfer_status:  u8,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 冲正请求
    #[derive(Debug, Clone, Default)]
    struct ReqRepeal {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 冲正时间间隔
        repeal_time_interval:  i32,
        /// 已经冲正次数
        repealed_times:  i32,
        /// 银行冲正标志
        bank_repeal_flag:  u8,
        /// 期商冲正标志
        broker_repeal_flag:  u8,
        /// 被冲正平台流水号
        plate_repeal_serial:  i32,
        /// 被冲正银行流水号
        bank_repeal_serial:  String,
        /// 被冲正期货流水号
        future_repeal_serial:  i32,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 安装编号
        install_id:  i32,
        /// 期货公司流水号
        future_serial:  i32,
        /// 用户标识
        user_id:  String,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 转帐金额
        trade_amount:  f64,
        /// 期货可取金额
        future_fetch_amount:  f64,
        /// 费用支付标志
        fee_pay_flag:  u8,
        /// 应收客户费用
        cust_fee:  f64,
        /// 应收期货公司费用
        broker_fee:  f64,
        /// 发送方给接收方的消息
        message:  Vec<u8>,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 转账交易状态
        transfer_status:  u8,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 冲正响应
    #[derive(Debug, Clone, Default)]
    struct RspRepeal {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 冲正时间间隔
        repeal_time_interval:  i32,
        /// 已经冲正次数
        repealed_times:  i32,
        /// 银行冲正标志
        bank_repeal_flag:  u8,
        /// 期商冲正标志
        broker_repeal_flag:  u8,
        /// 被冲正平台流水号
        plate_repeal_serial:  i32,
        /// 被冲正银行流水号
        bank_repeal_serial:  String,
        /// 被冲正期货流水号
        future_repeal_serial:  i32,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 安装编号
        install_id:  i32,
        /// 期货公司流水号
        future_serial:  i32,
        /// 用户标识
        user_id:  String,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 转帐金额
        trade_amount:  f64,
        /// 期货可取金额
        future_fetch_amount:  f64,
        /// 费用支付标志
        fee_pay_flag:  u8,
        /// 应收客户费用
        cust_fee:  f64,
        /// 应收期货公司费用
        broker_fee:  f64,
        /// 发送方给接收方的消息
        message:  Vec<u8>,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 转账交易状态
        transfer_status:  u8,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 查询账户信息请求
    #[derive(Debug, Clone, Default)]
    struct ReqQueryAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 期货公司流水号
        future_serial:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 查询账户信息响应
    #[derive(Debug, Clone, Default)]
    struct RspQueryAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 期货公司流水号
        future_serial:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 银行可用金额
        bank_use_amount:  f64,
        /// 银行可取金额
        bank_fetch_amount:  f64,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 期商签到签退
    #[derive(Debug, Clone, Default)]
    struct FutureSignIO {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 摘要
        digest:  Vec<u8>,
        /// 币种代码
        currency_id:  String,
        /// 渠道标志
        device_id:  String,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
    }
    /// 期商签到响应
    #[derive(Debug, Clone, Default)]
    struct RspFutureSignIn {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 摘要
        digest:  Vec<u8>,
        /// 币种代码
        currency_id:  String,
        /// 渠道标志
        device_id:  String,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// PIN密钥
        pin_key:  Vec<u8>,
        /// MAC密钥
        mac_key:  Vec<u8>,
    }
    /// 期商签退请求
    #[derive(Debug, Clone, Default)]
    struct ReqFutureSignOut {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 摘要
        digest:  Vec<u8>,
        /// 币种代码
        currency_id:  String,
        /// 渠道标志
        device_id:  String,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
    }
    /// 期商签退响应
    #[derive(Debug, Clone, Default)]
    struct RspFutureSignOut {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 摘要
        digest:  Vec<u8>,
        /// 币种代码
        currency_id:  String,
        /// 渠道标志
        device_id:  String,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
    }
    /// 查询指定流水号的交易结果请求
    #[derive(Debug, Clone, Default)]
    struct ReqQueryTradeResultBySerial {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 流水号
        reference:  i32,
        /// 本流水号发布者的机构类型
        reference_issuer_type:  u8,
        /// 本流水号发布者机构编码
        reference_issuer:  Vec<u8>,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 币种代码
        currency_id:  String,
        /// 转帐金额
        trade_amount:  f64,
        /// 摘要
        digest:  Vec<u8>,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 查询指定流水号的交易结果响应
    #[derive(Debug, Clone, Default)]
    struct RspQueryTradeResultBySerial {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 流水号
        reference:  i32,
        /// 本流水号发布者的机构类型
        reference_issuer_type:  u8,
        /// 本流水号发布者机构编码
        reference_issuer:  Vec<u8>,
        /// 原始返回代码
        origin_return_code:  String,
        /// 原始返回码描述
        origin_descr_info_for_return_code:  String,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 币种代码
        currency_id:  String,
        /// 转帐金额
        trade_amount:  f64,
        /// 摘要
        digest:  Vec<u8>,
    }
    /// 日终文件就绪请求
    #[derive(Debug, Clone, Default)]
    struct ReqDayEndFileReady {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 文件业务功能
        file_business_code:  u8,
        /// 摘要
        digest:  Vec<u8>,
    }
    /// 返回结果
    #[derive(Debug, Clone, Default)]
    struct ReturnResult {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 返回代码
        return_code:  String,
        /// 返回码描述
        descr_info_for_return_code:  String,
    }
    /// 验证期货资金密码
    #[derive(Debug, Clone, Default)]
    struct VerifyFuturePassword {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 安装编号
        install_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 币种代码
        currency_id:  String,
    }
    /// 验证客户信息
    #[derive(Debug, Clone, Default)]
    struct VerifyCustInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 验证期货资金密码和客户信息
    #[derive(Debug, Clone, Default)]
    struct VerifyFuturePasswordAndCustInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 币种代码
        currency_id:  String,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 验证期货资金密码和客户信息
    #[derive(Debug, Clone, Default)]
    struct DepositResultInform {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 出入金流水号，该流水号为银期报盘返回的流水号
        deposit_seq_no:  Vec<u8>,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 入金金额
        deposit:  f64,
        /// 请求编号
        request_id:  i32,
        /// 返回代码
        return_code:  String,
        /// 返回码描述
        descr_info_for_return_code:  String,
    }
    /// 交易核心向银期报盘发出密钥同步请求
    #[derive(Debug, Clone, Default)]
    struct ReqSyncKey {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 交易核心给银期报盘的消息
        message:  Vec<u8>,
        /// 渠道标志
        device_id:  String,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
    }
    /// 交易核心向银期报盘发出密钥同步响应
    #[derive(Debug, Clone, Default)]
    struct RspSyncKey {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 交易核心给银期报盘的消息
        message:  Vec<u8>,
        /// 渠道标志
        device_id:  String,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
    }
    /// 查询账户信息通知
    #[derive(Debug, Clone, Default)]
    struct NotifyQueryAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 期货公司流水号
        future_serial:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 银行可用金额
        bank_use_amount:  f64,
        /// 银行可取金额
        bank_fetch_amount:  f64,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 银期转账交易流水表
    #[derive(Debug, Clone, Default)]
    struct TransferSerial {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 平台流水号
        plate_serial:  i32,
        /// 交易发起方日期
        trade_date:  String,
        /// 交易日期
        trading_day:  String,
        /// 交易时间
        trade_time:  String,
        /// 交易代码
        trade_code:  String,
        /// 会话编号
        session_id:  i32,
        /// 银行编码
        bank_id:  String,
        /// 银行分支机构编码
        bank_branch_id:  String,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 期货公司编码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 期货公司帐号类型
        future_acc_type:  u8,
        /// 投资者帐号
        account_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 期货公司流水号
        future_serial:  i32,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 币种代码
        currency_id:  String,
        /// 交易金额
        trade_amount:  f64,
        /// 应收客户费用
        cust_fee:  f64,
        /// 应收期货公司费用
        broker_fee:  f64,
        /// 有效标志
        availability_flag:  u8,
        /// 操作员
        operator_code:  String,
        /// 新银行帐号
        bank_new_account:  String,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
    }
    /// 请求查询转帐流水
    #[derive(Debug, Clone, Default)]
    struct QryTransferSerial {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 银行编码
        bank_id:  String,
        /// 币种代码
        currency_id:  String,
    }
    /// 期商签到通知
    #[derive(Debug, Clone, Default)]
    struct NotifyFutureSignIn {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 摘要
        digest:  Vec<u8>,
        /// 币种代码
        currency_id:  String,
        /// 渠道标志
        device_id:  String,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// PIN密钥
        pin_key:  Vec<u8>,
        /// MAC密钥
        mac_key:  Vec<u8>,
    }
    /// 期商签退通知
    #[derive(Debug, Clone, Default)]
    struct NotifyFutureSignOut {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 摘要
        digest:  Vec<u8>,
        /// 币种代码
        currency_id:  String,
        /// 渠道标志
        device_id:  String,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
    }
    /// 交易核心向银期报盘发出密钥同步处理结果的通知
    #[derive(Debug, Clone, Default)]
    struct NotifySyncKey {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 交易核心给银期报盘的消息
        message:  Vec<u8>,
        /// 渠道标志
        device_id:  String,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
    }
    /// 请求查询银期签约关系
    #[derive(Debug, Clone, Default)]
    struct QryAccountRegister {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 银行编码
        bank_id:  String,
        /// 银行分支机构编码
        bank_branch_id:  String,
        /// 币种代码
        currency_id:  String,
    }
    /// 客户开销户信息表
    #[derive(Debug, Clone, Default)]
    struct AccountRegister {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日期
        trade_day:  String,
        /// 银行编码
        bank_id:  String,
        /// 银行分支机构编码
        bank_branch_id:  String,
        /// 银行帐号
        bank_account:  String,
        /// 期货公司编码
        broker_id:  String,
        /// 期货公司分支机构编码
        broker_branch_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户姓名
        customer_name:  String,
        /// 币种代码
        currency_id:  String,
        /// 开销户类别
        open_or_destroy:  u8,
        /// 签约日期
        reg_date:  String,
        /// 解约日期
        out_date:  String,
        /// 交易ID
        tid:  i32,
        /// 客户类型
        cust_type:  u8,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 银期开户信息
    #[derive(Debug, Clone, Default)]
    struct OpenAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 性别
        gender:  u8,
        /// 国家代码
        country_code:  String,
        /// 客户类型
        cust_type:  u8,
        /// 地址
        address:  String,
        /// 邮编
        zip_code:  String,
        /// 电话号码
        telephone:  String,
        /// 手机
        mobile_phone:  String,
        /// 传真
        fax:  String,
        /// 电子邮件
        email:  String,
        /// 资金账户状态
        money_account_status:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 安装编号
        install_id:  i32,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 汇钞标志
        cash_exchange_code:  u8,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 交易ID
        tid:  i32,
        /// 用户标识
        user_id:  String,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 银期销户信息
    #[derive(Debug, Clone, Default)]
    struct CancelAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 性别
        gender:  u8,
        /// 国家代码
        country_code:  String,
        /// 客户类型
        cust_type:  u8,
        /// 地址
        address:  String,
        /// 邮编
        zip_code:  String,
        /// 电话号码
        telephone:  String,
        /// 手机
        mobile_phone:  String,
        /// 传真
        fax:  String,
        /// 电子邮件
        email:  String,
        /// 资金账户状态
        money_account_status:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 安装编号
        install_id:  i32,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 汇钞标志
        cash_exchange_code:  u8,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 交易ID
        tid:  i32,
        /// 用户标识
        user_id:  String,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 银期变更银行账号信息
    #[derive(Debug, Clone, Default)]
    struct ChangeAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 性别
        gender:  u8,
        /// 国家代码
        country_code:  String,
        /// 客户类型
        cust_type:  u8,
        /// 地址
        address:  String,
        /// 邮编
        zip_code:  String,
        /// 电话号码
        telephone:  String,
        /// 手机
        mobile_phone:  String,
        /// 传真
        fax:  String,
        /// 电子邮件
        email:  String,
        /// 资金账户状态
        money_account_status:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 新银行帐号
        new_bank_account:  String,
        /// 新银行密码
        new_bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 安装编号
        install_id:  i32,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易ID
        tid:  i32,
        /// 摘要
        digest:  Vec<u8>,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 长客户姓名
        long_customer_name:  String,
    }
    /// 二级代理操作员银期权限
    #[derive(Debug, Clone, Default)]
    struct SecAgentACIDMap {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 资金账户
        account_id:  String,
        /// 币种
        currency_id:  String,
        /// 境外中介机构资金帐号
        broker_sec_agent_id:  String,
    }
    /// 二级代理操作员银期权限查询
    #[derive(Debug, Clone, Default)]
    struct QrySecAgentACIDMap {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 资金账户
        account_id:  String,
        /// 币种
        currency_id:  String,
    }
    /// 灾备中心交易权限
    #[derive(Debug, Clone, Default)]
    struct UserRightsAssign {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 应用单元代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 交易中心代码
        dr_identity_id:  i32,
    }
    /// 经济公司是否有在本标示的交易权限
    #[derive(Debug, Clone, Default)]
    struct BrokerUserRightAssign {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 应用单元代码
        broker_id:  String,
        /// 交易中心代码
        dr_identity_id:  i32,
        /// 能否交易
        tradeable:  i32,
    }
    /// 灾备交易转换报文
    #[derive(Debug, Clone, Default)]
    struct DRTransfer {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 原交易中心代码
        orig_dr_identity_id:  i32,
        /// 目标交易中心代码
        dest_dr_identity_id:  i32,
        /// 原应用单元代码
        orig_broker_id:  String,
        /// 目标易用单元代码
        dest_broker_id:  String,
    }
    /// Fens用户信息
    #[derive(Debug, Clone, Default)]
    struct FensUserInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 登录模式
        login_mode:  u8,
    }
    /// 当前银期所属交易中心
    #[derive(Debug, Clone, Default)]
    struct CurrTransferIdentity {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易中心代码
        identity_id:  i32,
    }
    /// 禁止登录用户
    #[derive(Debug, Clone, Default)]
    struct LoginForbiddenUser {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// IP地址
        ip_address:  String,
    }
    /// 查询禁止登录用户
    #[derive(Debug, Clone, Default)]
    struct QryLoginForbiddenUser {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
    }
    /// 资金账户基本准备金
    #[derive(Debug, Clone, Default)]
    struct TradingAccountReserve {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 基本准备金
        reserve:  f64,
        /// 币种代码
        currency_id:  String,
    }
    /// 查询禁止登录IP
    #[derive(Debug, Clone, Default)]
    struct QryLoginForbiddenIP {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// IP地址
        ip_address:  String,
    }
    /// 查询IP列表
    #[derive(Debug, Clone, Default)]
    struct QryIPList {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// IP地址
        ip_address:  String,
    }
    /// 查询用户下单权限分配表
    #[derive(Debug, Clone, Default)]
    struct QryUserRightsAssign {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 应用单元代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
    }
    /// 银期预约开户确认请求
    #[derive(Debug, Clone, Default)]
    struct ReserveOpenAccountConfirm {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 性别
        gender:  u8,
        /// 国家代码
        country_code:  String,
        /// 客户类型
        cust_type:  u8,
        /// 地址
        address:  String,
        /// 邮编
        zip_code:  String,
        /// 电话号码
        telephone:  String,
        /// 手机
        mobile_phone:  String,
        /// 传真
        fax:  String,
        /// 电子邮件
        email:  String,
        /// 资金账户状态
        money_account_status:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 安装编号
        install_id:  i32,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 交易ID
        tid:  i32,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 预约开户银行流水号
        bank_reserve_open_seq:  Vec<u8>,
        /// 预约开户日期
        book_date:  String,
        /// 预约开户验证密码
        book_psw:  Vec<u8>,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
    }
    /// 银期预约开户
    #[derive(Debug, Clone, Default)]
    struct ReserveOpenAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 性别
        gender:  u8,
        /// 国家代码
        country_code:  String,
        /// 客户类型
        cust_type:  u8,
        /// 地址
        address:  String,
        /// 邮编
        zip_code:  String,
        /// 电话号码
        telephone:  String,
        /// 手机
        mobile_phone:  String,
        /// 传真
        fax:  String,
        /// 电子邮件
        email:  String,
        /// 资金账户状态
        money_account_status:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 安装编号
        install_id:  i32,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 交易ID
        tid:  i32,
        /// 预约开户状态
        reserve_open_acc_stas:  u8,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
    }
    /// 银行账户属性
    #[derive(Debug, Clone, Default)]
    struct AccountProperty {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 银行统一标识类型
        bank_id:  String,
        /// 银行账户
        bank_account:  String,
        /// 银行账户的开户人名称
        open_name:  String,
        /// 银行账户的开户行
        open_bank:  Vec<u8>,
        /// 是否活跃
        is_active:  i32,
        /// 账户来源
        account_source_type:  u8,
        /// 开户日期
        open_date:  String,
        /// 注销日期
        cancel_date:  String,
        /// 录入员代码
        operator_id:  String,
        /// 录入日期
        operate_date:  String,
        /// 录入时间
        operate_time:  String,
        /// 币种代码
        currency_id:  String,
    }
    /// 查询当前交易中心
    #[derive(Debug, Clone, Default)]
    struct QryCurrDRIdentity {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易中心代码
        dr_identity_id:  i32,
    }
    /// 当前交易中心
    #[derive(Debug, Clone, Default)]
    struct CurrDRIdentity {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易中心代码
        dr_identity_id:  i32,
    }
    /// 查询二级代理商资金校验模式
    #[derive(Debug, Clone, Default)]
    struct QrySecAgentCheckMode {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
    }
    /// 查询二级代理商信息
    #[derive(Debug, Clone, Default)]
    struct QrySecAgentTradeInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 境外中介机构资金帐号
        broker_sec_agent_id:  String,
    }
    /// 用户发出获取安全安全登陆方法请求
    #[derive(Debug, Clone, Default)]
    struct ReqUserAuthMethod {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
    }
    /// 用户发出获取安全安全登陆方法回复
    #[derive(Debug, Clone, Default)]
    struct RspUserAuthMethod {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 当前可以用的认证模式
        usable_auth_method:  i32,
    }
    /// 用户发出获取安全安全登陆方法请求
    #[derive(Debug, Clone, Default)]
    struct ReqGenUserCaptcha {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
    }
    /// 生成的图片验证码信息
    #[derive(Debug, Clone, Default)]
    struct RspGenUserCaptcha {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 图片信息长度
        captcha_info_len:  i32,
        /// 图片信息
        captcha_info:  String,
    }
    /// 用户发出获取安全安全登陆方法请求
    #[derive(Debug, Clone, Default)]
    struct ReqGenUserText {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
    }
    /// 短信验证码生成的回复
    #[derive(Debug, Clone, Default)]
    struct RspGenUserText {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 短信验证码序号
        user_text_seq:  i32,
    }
    /// 用户发出带图形验证码的登录请求请求
    #[derive(Debug, Clone, Default)]
    struct ReqUserLoginWithCaptcha {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 密码
        password:  String,
        /// 用户端产品信息
        user_product_info:  String,
        /// 接口端产品信息
        interface_product_info:  String,
        /// 协议信息
        protocol_info:  String,
        /// Mac地址
        mac_address:  String,
        /// 登录备注
        login_remark:  String,
        /// 图形验证码的文字内容
        captcha:  Vec<u8>,
        /// 终端IP端口
        client_ip_port:  i32,
        /// 终端IP地址
        client_ip_address:  String,
    }
    /// 用户发出带短信验证码的登录请求请求
    #[derive(Debug, Clone, Default)]
    struct ReqUserLoginWithText {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 密码
        password:  String,
        /// 用户端产品信息
        user_product_info:  String,
        /// 接口端产品信息
        interface_product_info:  String,
        /// 协议信息
        protocol_info:  String,
        /// Mac地址
        mac_address:  String,
        /// 登录备注
        login_remark:  String,
        /// 短信验证码文字内容
        text:  Vec<u8>,
        /// 终端IP端口
        client_ip_port:  i32,
        /// 终端IP地址
        client_ip_address:  String,
    }
    /// 用户发出带动态验证码的登录请求请求
    #[derive(Debug, Clone, Default)]
    struct ReqUserLoginWithOTP {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 密码
        password:  String,
        /// 用户端产品信息
        user_product_info:  String,
        /// 接口端产品信息
        interface_product_info:  String,
        /// 协议信息
        protocol_info:  String,
        /// Mac地址
        mac_address:  String,
        /// 登录备注
        login_remark:  String,
        /// OTP密码
        otp_password:  String,
        /// 终端IP端口
        client_ip_port:  i32,
        /// 终端IP地址
        client_ip_address:  String,
    }
    /// api握手请求
    #[derive(Debug, Clone, Default)]
    struct ReqApiHandshake {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// api与front通信密钥版本号
        crypto_key_version:  String,
    }
    /// front发给api的握手回复
    #[derive(Debug, Clone, Default)]
    struct RspApiHandshake {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 握手回复数据长度
        front_handshake_data_len:  i32,
        /// 握手回复数据
        front_handshake_data:  Vec<u8>,
        /// API认证是否开启
        is_api_auth_enabled:  i32,
    }
    /// api给front的验证key的请求
    #[derive(Debug, Clone, Default)]
    struct ReqVerifyApiKey {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 握手回复数据长度
        api_handshake_data_len:  i32,
        /// 握手回复数据
        api_handshake_data:  Vec<u8>,
    }
    /// 操作员组织架构关系
    #[derive(Debug, Clone, Default)]
    struct DepartmentUser {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 投资者范围
        investor_range:  u8,
        /// 投资者代码
        investor_id:  String,
    }
    /// 查询频率，每秒查询比数
    #[derive(Debug, Clone, Default)]
    struct QueryFreq {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 查询频率
        query_freq:  i32,
        /// FTD频率
        ftd_pkg_freq:  i32,
    }
    /// 禁止认证IP
    #[derive(Debug, Clone, Default)]
    struct AuthForbiddenIP {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// IP地址
        ip_address:  String,
    }
    /// 查询禁止认证IP
    #[derive(Debug, Clone, Default)]
    struct QryAuthForbiddenIP {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// IP地址
        ip_address:  String,
    }
    /// 换汇可提冻结
    #[derive(Debug, Clone, Default)]
    struct SyncDelaySwapFrozen {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 换汇流水号
        delay_swap_seq_no:  Vec<u8>,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 源币种
        from_currency_id:  String,
        /// 源剩余换汇额度(可提冻结)
        from_remain_swap:  f64,
        /// 是否手工换汇
        is_manual_swap:  i32,
    }
    /// 用户系统信息
    #[derive(Debug, Clone, Default)]
    struct UserSystemInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 用户端系统内部信息长度
        client_system_info_len:  i32,
        /// 用户端系统内部信息
        client_system_info:  String,
        /// 终端IP端口
        client_ip_port:  i32,
        /// 登录成功时间
        client_login_time:  String,
        /// App代码
        client_app_id:  String,
        /// 用户公网IP
        client_public_ip:  String,
        /// 客户登录备注2
        client_login_remark:  String,
    }
    /// 终端用户绑定信息
    #[derive(Debug, Clone, Default)]
    struct AuthUserID {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// App代码
        app_id:  String,
        /// 用户代码
        user_id:  String,
        /// 校验类型
        auth_type:  u8,
    }
    /// 用户IP绑定信息
    #[derive(Debug, Clone, Default)]
    struct AuthIP {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// App代码
        app_id:  String,
        /// 用户代码
        ip_address:  String,
    }
    /// 查询分类合约
    #[derive(Debug, Clone, Default)]
    struct QryClassifiedInstrument {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 合约代码
        instrument_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// 产品代码
        product_id:  String,
        /// 合约交易状态
        trading_type:  u8,
        /// 合约分类类型
        class_type:  u8,
    }
    /// 查询组合优惠比例
    #[derive(Debug, Clone, Default)]
    struct QryCombPromotionParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 组合优惠比例
    #[derive(Debug, Clone, Default)]
    struct CombPromotionParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 投机套保标志
        comb_hedge_flag:  String,
        /// 期权组合保证金比例
        xparameter:  f64,
    }
    /// 国密用户登录请求
    #[derive(Debug, Clone, Default)]
    struct ReqUserLoginSM {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 密码
        password:  String,
        /// 用户端产品信息
        user_product_info:  String,
        /// 接口端产品信息
        interface_product_info:  String,
        /// 协议信息
        protocol_info:  String,
        /// Mac地址
        mac_address:  String,
        /// 动态密码
        one_time_password:  String,
        /// 登录备注
        login_remark:  String,
        /// 终端IP端口
        client_ip_port:  i32,
        /// 终端IP地址
        client_ip_address:  String,
        /// 经纪公司名称
        broker_name:  String,
        /// 认证码
        auth_code:  String,
        /// App代码
        app_id:  String,
        /// PIN码
        pin:  Vec<u8>,
    }
    /// 投资者风险结算持仓查询
    #[derive(Debug, Clone, Default)]
    struct QryRiskSettleInvestPosition {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// 风险结算产品查询
    #[derive(Debug, Clone, Default)]
    struct QryRiskSettleProductStatus {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 产品代码
        product_id:  String,
    }
    /// 投资者风险结算持仓
    #[derive(Debug, Clone, Default)]
    struct RiskSettleInvestPosition {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 合约代码
        instrument_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 持仓多空方向
        posi_direction:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 持仓日期
        position_date:  u8,
        /// 上日持仓
        yd_position:  i32,
        /// 今日持仓
        position:  i32,
        /// 多头冻结
        long_frozen:  i32,
        /// 空头冻结
        short_frozen:  i32,
        /// 开仓冻结金额
        long_frozen_amount:  f64,
        /// 开仓冻结金额
        short_frozen_amount:  f64,
        /// 开仓量
        open_volume:  i32,
        /// 平仓量
        close_volume:  i32,
        /// 开仓金额
        open_amount:  f64,
        /// 平仓金额
        close_amount:  f64,
        /// 持仓成本
        position_cost:  f64,
        /// 上次占用的保证金
        pre_margin:  f64,
        /// 占用的保证金
        use_margin:  f64,
        /// 冻结的保证金
        frozen_margin:  f64,
        /// 冻结的资金
        frozen_cash:  f64,
        /// 冻结的手续费
        frozen_commission:  f64,
        /// 资金差额
        cash_in:  f64,
        /// 手续费
        commission:  f64,
        /// 平仓盈亏
        close_profit:  f64,
        /// 持仓盈亏
        position_profit:  f64,
        /// 上次结算价
        pre_settlement_price:  f64,
        /// 本次结算价
        settlement_price:  f64,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 开仓成本
        open_cost:  f64,
        /// 交易所保证金
        exchange_margin:  f64,
        /// 组合成交形成的持仓
        comb_position:  i32,
        /// 组合多头冻结
        comb_long_frozen:  i32,
        /// 组合空头冻结
        comb_short_frozen:  i32,
        /// 逐日盯市平仓盈亏
        close_profit_by_date:  f64,
        /// 逐笔对冲平仓盈亏
        close_profit_by_trade:  f64,
        /// 今日持仓
        today_position:  i32,
        /// 保证金率
        margin_rate_by_money:  f64,
        /// 保证金率(按手数)
        margin_rate_by_volume:  f64,
        /// 执行冻结
        strike_frozen:  i32,
        /// 执行冻结金额
        strike_frozen_amount:  f64,
        /// 放弃执行冻结
        abandon_frozen:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 执行冻结的昨仓
        yd_strike_frozen:  i32,
        /// 投资单元代码
        invest_unit_id:  String,
        /// 持仓成本差值
        position_cost_offset:  f64,
        /// tas持仓手数
        tas_position:  i32,
        /// tas持仓成本
        tas_position_cost:  f64,
    }
    /// 风险品种
    #[derive(Debug, Clone, Default)]
    struct RiskSettleProductStatus {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 产品编号
        product_id:  String,
        /// 产品结算状态
        product_status:  u8,
    }
    /// 风险结算追平信息
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 追平序号
        sync_delta_sequence_no:  i32,
        /// 追平状态
        sync_delta_status:  u8,
        /// 追平描述
        sync_description:  Vec<u8>,
        /// 是否只有资金追平
        is_only_trd_delta:  i32,
    }
    /// 风险结算追平产品信息
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaProductStatus {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 追平序号
        sync_delta_sequence_no:  i32,
        /// 交易所代码
        exchange_id:  String,
        /// 产品代码
        product_id:  String,
        /// 是否允许交易
        product_status:  u8,
    }
    /// 风险结算追平持仓明细
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaInvestPosDtl {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 合约代码
        instrument_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 买卖
        direction:  u8,
        /// 开仓日期
        open_date:  String,
        /// 成交编号
        trade_id:  String,
        /// 数量
        volume:  i32,
        /// 开仓价
        open_price:  f64,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 成交类型
        trade_type:  u8,
        /// 组合合约代码
        comb_instrument_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 逐日盯市平仓盈亏
        close_profit_by_date:  f64,
        /// 逐笔对冲平仓盈亏
        close_profit_by_trade:  f64,
        /// 逐日盯市持仓盈亏
        position_profit_by_date:  f64,
        /// 逐笔对冲持仓盈亏
        position_profit_by_trade:  f64,
        /// 投资者保证金
        margin:  f64,
        /// 交易所保证金
        exch_margin:  f64,
        /// 保证金率
        margin_rate_by_money:  f64,
        /// 保证金率(按手数)
        margin_rate_by_volume:  f64,
        /// 昨结算价
        last_settlement_price:  f64,
        /// 结算价
        settlement_price:  f64,
        /// 平仓量
        close_volume:  i32,
        /// 平仓金额
        close_amount:  f64,
        /// 先开先平剩余数量
        time_first_volume:  i32,
        /// 特殊持仓标志
        spec_posi_type:  u8,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平组合持仓明细
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaInvestPosCombDtl {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 开仓日期
        open_date:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 组合编号
        com_trade_id:  String,
        /// 撮合编号
        trade_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 买卖
        direction:  u8,
        /// 持仓量
        total_amt:  i32,
        /// 投资者保证金
        margin:  f64,
        /// 交易所保证金
        exch_margin:  f64,
        /// 保证金率
        margin_rate_by_money:  f64,
        /// 保证金率(按手数)
        margin_rate_by_volume:  f64,
        /// 单腿编号
        leg_id:  i32,
        /// 单腿乘数
        leg_multiple:  i32,
        /// 成交组号
        trade_group_id:  i32,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平资金
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaTradingAccount {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 上次质押金额
        pre_mortgage:  f64,
        /// 上次信用额度
        pre_credit:  f64,
        /// 上次存款额
        pre_deposit:  f64,
        /// 上次结算准备金
        pre_balance:  f64,
        /// 上次占用的保证金
        pre_margin:  f64,
        /// 利息基数
        interest_base:  f64,
        /// 利息收入
        interest:  f64,
        /// 入金金额
        deposit:  f64,
        /// 出金金额
        withdraw:  f64,
        /// 冻结的保证金
        frozen_margin:  f64,
        /// 冻结的资金
        frozen_cash:  f64,
        /// 冻结的手续费
        frozen_commission:  f64,
        /// 当前保证金总额
        curr_margin:  f64,
        /// 资金差额
        cash_in:  f64,
        /// 手续费
        commission:  f64,
        /// 平仓盈亏
        close_profit:  f64,
        /// 持仓盈亏
        position_profit:  f64,
        /// 期货结算准备金
        balance:  f64,
        /// 可用资金
        available:  f64,
        /// 可取资金
        withdraw_quota:  f64,
        /// 基本准备金
        reserve:  f64,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 信用额度
        credit:  f64,
        /// 质押金额
        mortgage:  f64,
        /// 交易所保证金
        exchange_margin:  f64,
        /// 投资者交割保证金
        delivery_margin:  f64,
        /// 交易所交割保证金
        exchange_delivery_margin:  f64,
        /// 保底期货结算准备金
        reserve_balance:  f64,
        /// 币种代码
        currency_id:  String,
        /// 上次货币质入金额
        pre_fund_mortgage_in:  f64,
        /// 上次货币质出金额
        pre_fund_mortgage_out:  f64,
        /// 货币质入金额
        fund_mortgage_in:  f64,
        /// 货币质出金额
        fund_mortgage_out:  f64,
        /// 货币质押余额
        fund_mortgage_available:  f64,
        /// 可质押货币金额
        mortgageable_fund:  f64,
        /// 特殊产品占用保证金
        spec_product_margin:  f64,
        /// 特殊产品冻结保证金
        spec_product_frozen_margin:  f64,
        /// 特殊产品手续费
        spec_product_commission:  f64,
        /// 特殊产品冻结手续费
        spec_product_frozen_commission:  f64,
        /// 特殊产品持仓盈亏
        spec_product_position_profit:  f64,
        /// 特殊产品平仓盈亏
        spec_product_close_profit:  f64,
        /// 根据持仓盈亏算法计算的特殊产品持仓盈亏
        spec_product_position_profit_by_alg:  f64,
        /// 特殊产品交易所保证金
        spec_product_exchange_margin:  f64,
        /// 延时换汇冻结金额
        frozen_swap:  f64,
        /// 剩余换汇额度
        remain_swap:  f64,
        /// 期权市值
        option_value:  f64,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 投资者风险结算总保证金
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaInitInvestMargin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 追平前总风险保证金
        last_risk_total_invest_margin:  f64,
        /// 追平前交易所总风险保证金
        last_risk_total_exch_margin:  f64,
        /// 本次追平品种总保证金
        this_sync_invest_margin:  f64,
        /// 本次追平品种交易所总保证金
        this_sync_exch_margin:  f64,
        /// 本次未追平品种总保证金
        remain_risk_invest_margin:  f64,
        /// 本次未追平品种交易所总保证金
        remain_risk_exch_margin:  f64,
        /// 追平前总特殊产品风险保证金
        last_risk_spec_total_invest_margin:  f64,
        /// 追平前总特殊产品交易所风险保证金
        last_risk_spec_total_exch_margin:  f64,
        /// 本次追平品种特殊产品总保证金
        this_sync_spec_invest_margin:  f64,
        /// 本次追平品种特殊产品交易所总保证金
        this_sync_spec_exch_margin:  f64,
        /// 本次未追平品种特殊产品总保证金
        remain_risk_spec_invest_margin:  f64,
        /// 本次未追平品种特殊产品交易所总保证金
        remain_risk_spec_exch_margin:  f64,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平组合优先级
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaDceCombInstrument {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 合约代码
        comb_instrument_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// 成交组号
        trade_group_id:  i32,
        /// 投机套保标志
        comb_hedge_flag:  u8,
        /// 组合类型
        combination_type:  u8,
        /// 买卖
        direction:  u8,
        /// 产品代码
        product_id:  String,
        /// 期权组合保证金比例
        xparameter:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平投资者期货保证金率
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaInvestMarginRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 合约代码
        instrument_id:  String,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 多头保证金率
        long_margin_ratio_by_money:  f64,
        /// 多头保证金费
        long_margin_ratio_by_volume:  f64,
        /// 空头保证金率
        short_margin_ratio_by_money:  f64,
        /// 空头保证金费
        short_margin_ratio_by_volume:  f64,
        /// 是否相对交易所收取
        is_relative:  i32,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平交易所期货保证金率
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaExchMarginRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 多头保证金率
        long_margin_ratio_by_money:  f64,
        /// 多头保证金费
        long_margin_ratio_by_volume:  f64,
        /// 空头保证金率
        short_margin_ratio_by_money:  f64,
        /// 空头保证金费
        short_margin_ratio_by_volume:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平中金现货期权交易所保证金率
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaOptExchMargin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 投机空头保证金调整系数
        s_short_margin_ratio_by_money:  f64,
        /// 投机空头保证金调整系数
        s_short_margin_ratio_by_volume:  f64,
        /// 保值空头保证金调整系数
        h_short_margin_ratio_by_money:  f64,
        /// 保值空头保证金调整系数
        h_short_margin_ratio_by_volume:  f64,
        /// 套利空头保证金调整系数
        a_short_margin_ratio_by_money:  f64,
        /// 套利空头保证金调整系数
        a_short_margin_ratio_by_volume:  f64,
        /// 做市商空头保证金调整系数
        m_short_margin_ratio_by_money:  f64,
        /// 做市商空头保证金调整系数
        m_short_margin_ratio_by_volume:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平中金现货期权投资者保证金率
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaOptInvestMargin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 合约代码
        instrument_id:  String,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机空头保证金调整系数
        s_short_margin_ratio_by_money:  f64,
        /// 投机空头保证金调整系数
        s_short_margin_ratio_by_volume:  f64,
        /// 保值空头保证金调整系数
        h_short_margin_ratio_by_money:  f64,
        /// 保值空头保证金调整系数
        h_short_margin_ratio_by_volume:  f64,
        /// 套利空头保证金调整系数
        a_short_margin_ratio_by_money:  f64,
        /// 套利空头保证金调整系数
        a_short_margin_ratio_by_volume:  f64,
        /// 是否跟随交易所收取
        is_relative:  i32,
        /// 做市商空头保证金调整系数
        m_short_margin_ratio_by_money:  f64,
        /// 做市商空头保证金调整系数
        m_short_margin_ratio_by_volume:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平期权标的调整保证金率
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaInvestMarginRateUL {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 合约代码
        instrument_id:  String,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 多头保证金率
        long_margin_ratio_by_money:  f64,
        /// 多头保证金费
        long_margin_ratio_by_volume:  f64,
        /// 空头保证金率
        short_margin_ratio_by_money:  f64,
        /// 空头保证金费
        short_margin_ratio_by_volume:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平期权手续费率
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaOptInvestCommRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 合约代码
        instrument_id:  String,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 开仓手续费率
        open_ratio_by_money:  f64,
        /// 开仓手续费
        open_ratio_by_volume:  f64,
        /// 平仓手续费率
        close_ratio_by_money:  f64,
        /// 平仓手续费
        close_ratio_by_volume:  f64,
        /// 平今手续费率
        close_today_ratio_by_money:  f64,
        /// 平今手续费
        close_today_ratio_by_volume:  f64,
        /// 执行手续费率
        strike_ratio_by_money:  f64,
        /// 执行手续费
        strike_ratio_by_volume:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平期货手续费率
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaInvestCommRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 合约代码
        instrument_id:  String,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 开仓手续费率
        open_ratio_by_money:  f64,
        /// 开仓手续费
        open_ratio_by_volume:  f64,
        /// 平仓手续费率
        close_ratio_by_money:  f64,
        /// 平仓手续费
        close_ratio_by_volume:  f64,
        /// 平今手续费率
        close_today_ratio_by_money:  f64,
        /// 平今手续费
        close_today_ratio_by_volume:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平交叉汇率
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaProductExchRate {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 产品代码
        product_id:  String,
        /// 报价币种类型
        quote_currency_id:  String,
        /// 汇率
        exchange_rate:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平行情
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaDepthMarketData {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 合约代码
        instrument_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 合约在交易所的代码
        exchange_inst_id:  String,
        /// 最新价
        last_price:  f64,
        /// 上次结算价
        pre_settlement_price:  f64,
        /// 昨收盘
        pre_close_price:  f64,
        /// 昨持仓量
        pre_open_interest:  f64,
        /// 今开盘
        open_price:  f64,
        /// 最高价
        highest_price:  f64,
        /// 最低价
        lowest_price:  f64,
        /// 数量
        volume:  i32,
        /// 成交金额
        turnover:  f64,
        /// 持仓量
        open_interest:  f64,
        /// 今收盘
        close_price:  f64,
        /// 本次结算价
        settlement_price:  f64,
        /// 涨停板价
        upper_limit_price:  f64,
        /// 跌停板价
        lower_limit_price:  f64,
        /// 昨虚实度
        pre_delta:  f64,
        /// 今虚实度
        curr_delta:  f64,
        /// 最后修改时间
        update_time:  String,
        /// 最后修改毫秒
        update_millisecond:  i32,
        /// 申买价一
        bid_price1:  f64,
        /// 申买量一
        bid_volume1:  i32,
        /// 申卖价一
        ask_price1:  f64,
        /// 申卖量一
        ask_volume1:  i32,
        /// 申买价二
        bid_price2:  f64,
        /// 申买量二
        bid_volume2:  i32,
        /// 申卖价二
        ask_price2:  f64,
        /// 申卖量二
        ask_volume2:  i32,
        /// 申买价三
        bid_price3:  f64,
        /// 申买量三
        bid_volume3:  i32,
        /// 申卖价三
        ask_price3:  f64,
        /// 申卖量三
        ask_volume3:  i32,
        /// 申买价四
        bid_price4:  f64,
        /// 申买量四
        bid_volume4:  i32,
        /// 申卖价四
        ask_price4:  f64,
        /// 申卖量四
        ask_volume4:  i32,
        /// 申买价五
        bid_price5:  f64,
        /// 申买量五
        bid_volume5:  i32,
        /// 申卖价五
        ask_price5:  f64,
        /// 申卖量五
        ask_volume5:  i32,
        /// 当日均价
        average_price:  f64,
        /// 业务日期
        action_day:  String,
        /// 上带价
        banding_upper_price:  f64,
        /// 下带价
        banding_lower_price:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平现货指数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaIndexPrice {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 指数现货收盘价
        close_price:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平仓单折抵
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaEWarrantOffset {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日期
        trading_day:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 买卖方向
        direction:  u8,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 数量
        volume:  i32,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// SPBM期货合约保证金参数
    #[derive(Debug, Clone, Default)]
    struct SPBMFutureParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 品种代码
        prod_family_code:  String,
        /// 期货合约因子
        cvf:  i32,
        /// 阶段标识
        time_range:  u8,
        /// 品种保证金标准
        margin_rate:  f64,
        /// 期货合约内部对锁仓费率折扣比例
        lock_rate_x:  f64,
        /// 提高保证金标准
        add_on_rate:  f64,
        /// 昨结算价
        pre_settlement_price:  f64,
        /// 期货合约内部对锁仓附加费率折扣比例
        add_on_lock_rate_x2:  f64,
    }
    /// SPBM期权合约保证金参数
    #[derive(Debug, Clone, Default)]
    struct SPBMOptionParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 品种代码
        prod_family_code:  String,
        /// 期权合约因子
        cvf:  i32,
        /// 期权冲抵价格
        down_price:  f64,
        /// Delta值
        delta:  f64,
        /// 卖方期权风险转换最低值
        slim_delta:  f64,
        /// 昨结算价
        pre_settlement_price:  f64,
    }
    /// SPBM品种内对锁仓折扣参数
    #[derive(Debug, Clone, Default)]
    struct SPBMIntraParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 品种代码
        prod_family_code:  String,
        /// 品种内合约间对锁仓费率折扣比例
        intra_rate_y:  f64,
        /// 品种内合约间对锁仓附加费率折扣比例
        add_on_intra_rate_y2:  f64,
    }
    /// SPBM跨品种抵扣参数
    #[derive(Debug, Clone, Default)]
    struct SPBMInterParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 优先级
        spread_id:  i32,
        /// 品种间对锁仓费率折扣比例
        inter_rate_z:  f64,
        /// 第一腿构成品种
        leg1_prod_family_code:  String,
        /// 第二腿构成品种
        leg2_prod_family_code:  String,
    }
    /// 同步SPBM参数结束
    #[derive(Debug, Clone, Default)]
    struct SyncSPBMParameterEnd {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
    }
    /// SPBM期货合约保证金参数查询
    #[derive(Debug, Clone, Default)]
    struct QrySPBMFutureParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 品种代码
        prod_family_code:  String,
    }
    /// SPBM期权合约保证金参数查询
    #[derive(Debug, Clone, Default)]
    struct QrySPBMOptionParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 品种代码
        prod_family_code:  String,
    }
    /// SPBM品种内对锁仓折扣参数查询
    #[derive(Debug, Clone, Default)]
    struct QrySPBMIntraParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 品种代码
        prod_family_code:  String,
    }
    /// SPBM跨品种抵扣参数查询
    #[derive(Debug, Clone, Default)]
    struct QrySPBMInterParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 第一腿构成品种
        leg1_prod_family_code:  String,
        /// 第二腿构成品种
        leg2_prod_family_code:  String,
    }
    /// 组合保证金套餐
    #[derive(Debug, Clone, Default)]
    struct SPBMPortfDefinition {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 组合保证金套餐代码
        portfolio_def_id:  i32,
        /// 品种代码
        prod_family_code:  String,
        /// 是否启用SPBM
        is_spbm:  i32,
    }
    /// 投资者套餐选择
    #[derive(Debug, Clone, Default)]
    struct SPBMInvestorPortfDef {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 组合保证金套餐代码
        portfolio_def_id:  i32,
    }
    /// 投资者新型组合保证金系数
    #[derive(Debug, Clone, Default)]
    struct InvestorPortfMarginRatio {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者范围
        investor_range:  u8,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 会员对投资者收取的保证金和交易所对投资者收取的保证金的比例
        margin_ratio:  f64,
        /// 产品群代码
        product_group_id:  String,
    }
    /// 组合保证金套餐查询
    #[derive(Debug, Clone, Default)]
    struct QrySPBMPortfDefinition {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 组合保证金套餐代码
        portfolio_def_id:  i32,
        /// 品种代码
        prod_family_code:  String,
    }
    /// 投资者套餐选择查询
    #[derive(Debug, Clone, Default)]
    struct QrySPBMInvestorPortfDef {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
    }
    /// 投资者新型组合保证金系数查询
    #[derive(Debug, Clone, Default)]
    struct QryInvestorPortfMarginRatio {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 产品群代码
        product_group_id:  String,
    }
    /// 投资者产品SPBM明细
    #[derive(Debug, Clone, Default)]
    struct InvestorProdSPBMDetail {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 品种代码
        prod_family_code:  String,
        /// 合约内对锁保证金
        intra_instr_margin:  f64,
        /// 买归集保证金
        b_collecting_margin:  f64,
        /// 卖归集保证金
        s_collecting_margin:  f64,
        /// 品种内合约间对锁保证金
        intra_prod_margin:  f64,
        /// 净保证金
        net_margin:  f64,
        /// 产品间对锁保证金
        inter_prod_margin:  f64,
        /// 裸保证金
        single_margin:  f64,
        /// 附加保证金
        add_on_margin:  f64,
        /// 交割月保证金
        delivery_margin:  f64,
        /// 看涨期权最低风险
        call_option_min_risk:  f64,
        /// 看跌期权最低风险
        put_option_min_risk:  f64,
        /// 卖方期权最低风险
        option_min_risk:  f64,
        /// 买方期权冲抵价值
        option_value_offset:  f64,
        /// 卖方期权权利金
        option_royalty:  f64,
        /// 价值冲抵
        real_option_value_offset:  f64,
        /// 保证金
        margin:  f64,
        /// 交易所保证金
        exch_margin:  f64,
    }
    /// 投资者产品SPBM明细查询
    #[derive(Debug, Clone, Default)]
    struct QryInvestorProdSPBMDetail {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 品种代码
        prod_family_code:  String,
    }
    /// 组保交易参数设置
    #[derive(Debug, Clone, Default)]
    struct PortfTradeParamSetting {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 新型组保算法
        portfolio:  u8,
        /// 撤单是否验资
        is_action_verify:  i32,
        /// 平仓是否验资
        is_close_verify:  i32,
    }
    /// 投资者交易权限设置
    #[derive(Debug, Clone, Default)]
    struct InvestorTradingRight {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 交易权限
        invest_trading_right:  u8,
    }
    /// 质押配比参数
    #[derive(Debug, Clone, Default)]
    struct MortgageParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 质押配比系数
        mortgage_balance:  f64,
        /// 开仓是否验证质押配比
        check_mortgage_ratio:  i32,
    }
    /// 可提控制参数
    #[derive(Debug, Clone, Default)]
    struct WithDrawParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 参数代码
        with_draw_param_id:  u8,
        /// 参数代码值
        with_draw_param_value:  Vec<u8>,
    }
    /// Thost终端用户功能权限
    #[derive(Debug, Clone, Default)]
    struct ThostUserFunction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// Thost终端功能代码
        thost_function_code:  i32,
    }
    /// Thost终端用户功能权限查询
    #[derive(Debug, Clone, Default)]
    struct QryThostUserFunction {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
    }
    /// SPBM附加跨品种抵扣参数
    #[derive(Debug, Clone, Default)]
    struct SPBMAddOnInterParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 优先级
        spread_id:  i32,
        /// 品种间对锁仓附加费率折扣比例
        add_on_inter_rate_z2:  f64,
        /// 第一腿构成品种
        leg1_prod_family_code:  String,
        /// 第二腿构成品种
        leg2_prod_family_code:  String,
    }
    /// SPBM附加跨品种抵扣参数查询
    #[derive(Debug, Clone, Default)]
    struct QrySPBMAddOnInterParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 第一腿构成品种
        leg1_prod_family_code:  String,
        /// 第二腿构成品种
        leg2_prod_family_code:  String,
    }
    /// 投资者商品组SPMM记录查询
    #[derive(Debug, Clone, Default)]
    struct QryInvestorCommoditySPMMMargin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 商品组代码
        commodity_id:  String,
    }
    /// 投资者商品群SPMM记录查询
    #[derive(Debug, Clone, Default)]
    struct QryInvestorCommodityGroupSPMMMargin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 商品群代码
        commodity_group_id:  String,
    }
    /// SPMM合约参数查询
    #[derive(Debug, Clone, Default)]
    struct QrySPMMInstParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 合约代码
        instrument_id:  String,
    }
    /// SPMM产品参数查询
    #[derive(Debug, Clone, Default)]
    struct QrySPMMProductParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 产品代码
        product_id:  String,
    }
    /// 投资者商品组SPMM记录
    #[derive(Debug, Clone, Default)]
    struct InvestorCommoditySPMMMargin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 商品组代码
        commodity_id:  String,
        /// 优惠仓位应收保证金
        margin_before_discount:  f64,
        /// 不优惠仓位应收保证金
        margin_no_discount:  f64,
        /// 多头实仓风险
        long_pos_risk:  f64,
        /// 多头开仓冻结风险
        long_open_frozen_risk:  f64,
        /// 多头被平冻结风险
        long_close_frozen_risk:  f64,
        /// 空头实仓风险
        short_pos_risk:  f64,
        /// 空头开仓冻结风险
        short_open_frozen_risk:  f64,
        /// 空头被平冻结风险
        short_close_frozen_risk:  f64,
        /// SPMM品种内跨期优惠系数
        intra_commodity_rate:  f64,
        /// SPMM期权优惠系数
        option_discount_rate:  f64,
        /// 实仓对冲优惠金额
        pos_discount:  f64,
        /// 开仓报单对冲优惠金额
        open_frozen_discount:  f64,
        /// 品种风险净头
        net_risk:  f64,
        /// 平仓冻结保证金
        close_frozen_margin:  f64,
        /// 冻结的手续费
        frozen_commission:  f64,
        /// 手续费
        commission:  f64,
        /// 冻结的资金
        frozen_cash:  f64,
        /// 资金差额
        cash_in:  f64,
        /// 行权冻结资金
        strike_frozen_margin:  f64,
    }
    /// 投资者商品群SPMM记录
    #[derive(Debug, Clone, Default)]
    struct InvestorCommodityGroupSPMMMargin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 商品群代码
        commodity_group_id:  String,
        /// 优惠仓位应收保证金
        margin_before_discount:  f64,
        /// 不优惠仓位应收保证金
        margin_no_discount:  f64,
        /// 多头风险
        long_risk:  f64,
        /// 空头风险
        short_risk:  f64,
        /// 商品群平仓冻结保证金
        close_frozen_margin:  f64,
        /// SPMM跨品种优惠系数
        inter_commodity_rate:  f64,
        /// 商品群最小保证金比例
        mini_margin_ratio:  f64,
        /// 投资者保证金和交易所保证金的比例
        adjust_ratio:  f64,
        /// SPMM品种内优惠汇总
        intra_commodity_discount:  f64,
        /// SPMM跨品种优惠
        inter_commodity_discount:  f64,
        /// 交易所保证金
        exch_margin:  f64,
        /// 投资者保证金
        investor_margin:  f64,
        /// 冻结的手续费
        frozen_commission:  f64,
        /// 手续费
        commission:  f64,
        /// 冻结的资金
        frozen_cash:  f64,
        /// 资金差额
        cash_in:  f64,
        /// 行权冻结资金
        strike_frozen_margin:  f64,
    }
    /// SPMM合约参数
    #[derive(Debug, Clone, Default)]
    struct SPMMInstParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// SPMM合约保证金算法
        inst_margin_cal_id:  u8,
        /// 商品组代码
        commodity_id:  String,
        /// 商品群代码
        commodity_group_id:  String,
    }
    /// SPMM产品参数
    #[derive(Debug, Clone, Default)]
    struct SPMMProductParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 产品代码
        product_id:  String,
        /// 商品组代码
        commodity_id:  String,
        /// 商品群代码
        commodity_group_id:  String,
    }
    /// 席位与交易中心对应关系维护查询
    #[derive(Debug, Clone, Default)]
    struct QryTraderAssign {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易员代码
        trader_id:  String,
    }
    /// 席位与交易中心对应关系
    #[derive(Debug, Clone, Default)]
    struct TraderAssign {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 应用单元代码
        broker_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 交易中心代码
        dr_identity_id:  i32,
    }
    /// 投资者申报费阶梯收取设置
    #[derive(Debug, Clone, Default)]
    struct InvestorInfoCntSetting {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 商品代码
        product_id:  String,
        /// 是否收取申报费
        is_cal_info_comm:  i32,
        /// 是否限制信息量
        is_limit_info_max:  i32,
        /// 信息量限制笔数
        info_max_limit:  i32,
    }
    /// RCAMS产品组合信息
    #[derive(Debug, Clone, Default)]
    struct RCAMSCombProductInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 产品代码
        product_id:  String,
        /// 商品组代码
        comb_product_id:  String,
        /// 商品群代码
        product_group_id:  String,
    }
    /// RCAMS同合约风险对冲参数
    #[derive(Debug, Clone, Default)]
    struct RCAMSInstrParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 产品代码
        product_id:  String,
        /// 同合约风险对冲比率
        hedge_rate:  f64,
    }
    /// RCAMS品种内风险对冲参数
    #[derive(Debug, Clone, Default)]
    struct RCAMSIntraParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 产品组合代码
        comb_product_id:  String,
        /// 品种内对冲比率
        hedge_rate:  f64,
    }
    /// RCAMS跨品种风险折抵参数
    #[derive(Debug, Clone, Default)]
    struct RCAMSInterParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 商品群代码
        product_group_id:  String,
        /// 优先级
        priority:  i32,
        /// 折抵率
        credit_rate:  f64,
        /// 产品组合代码1
        comb_product1:  String,
        /// 产品组合代码2
        comb_product2:  String,
    }
    /// RCAMS空头期权风险调整参数
    #[derive(Debug, Clone, Default)]
    struct RCAMSShortOptAdjustParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 产品组合代码
        comb_product_id:  String,
        /// 投套标志
        hedge_flag:  u8,
        /// 空头期权风险调整标准
        adjust_value:  f64,
    }
    /// RCAMS策略组合持仓
    #[derive(Debug, Clone, Default)]
    struct RCAMSInvestorCombPosition {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 投套标志
        hedge_flag:  u8,
        /// 持仓多空方向
        posi_direction:  u8,
        /// 组合合约代码
        comb_instrument_id:  String,
        /// 单腿编号
        leg_id:  i32,
        /// 交易所组合合约代码
        exchange_inst_id:  String,
        /// 持仓量
        total_amt:  i32,
        /// 交易所保证金
        exch_margin:  f64,
        /// 投资者保证金
        margin:  f64,
    }
    /// 投资者品种RCAMS保证金
    #[derive(Debug, Clone, Default)]
    struct InvestorProdRCAMSMargin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 产品组合代码
        comb_product_id:  String,
        /// 投套标志
        hedge_flag:  u8,
        /// 商品群代码
        product_group_id:  String,
        /// 品种组合前风险
        risk_before_discount:  f64,
        /// 同合约对冲风险
        intra_instr_risk:  f64,
        /// 品种买持仓风险
        b_pos_risk:  f64,
        /// 品种卖持仓风险
        spos_risk:  f64,
        /// 品种内对冲风险
        intra_prod_risk:  f64,
        /// 品种净持仓风险
        net_risk:  f64,
        /// 品种间对冲风险
        inter_prod_risk:  f64,
        /// 空头期权风险调整
        short_opt_risk_adj:  f64,
        /// 空头期权权利金
        option_royalty:  f64,
        /// 大边组合平仓冻结保证金
        mmsa_close_frozen_margin:  f64,
        /// 策略组合平仓/行权冻结保证金
        close_comb_frozen_margin:  f64,
        /// 平仓/行权冻结保证金
        close_frozen_margin:  f64,
        /// 大边组合开仓冻结保证金
        mmsa_open_frozen_margin:  f64,
        /// 交割月期货开仓冻结保证金
        delivery_open_frozen_margin:  f64,
        /// 开仓冻结保证金
        open_frozen_margin:  f64,
        /// 投资者冻结保证金
        use_frozen_margin:  f64,
        /// 大边组合交易所持仓保证金
        mmsa_exch_margin:  f64,
        /// 交割月期货交易所持仓保证金
        delivery_exch_margin:  f64,
        /// 策略组合交易所保证金
        comb_exch_margin:  f64,
        /// 交易所持仓保证金
        exch_margin:  f64,
        /// 投资者持仓保证金
        use_margin:  f64,
    }
    /// RCAMS产品组合信息查询
    #[derive(Debug, Clone, Default)]
    struct QryRCAMSCombProductInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 产品代码
        product_id:  String,
        /// 商品组代码
        comb_product_id:  String,
        /// 商品群代码
        product_group_id:  String,
    }
    /// RCAMS同合约风险对冲参数查询
    #[derive(Debug, Clone, Default)]
    struct QryRCAMSInstrParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 产品代码
        product_id:  String,
    }
    /// RCAMS品种内风险对冲参数查询
    #[derive(Debug, Clone, Default)]
    struct QryRCAMSIntraParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 产品组合代码
        comb_product_id:  String,
    }
    /// RCAMS跨品种风险折抵参数查询
    #[derive(Debug, Clone, Default)]
    struct QryRCAMSInterParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 商品群代码
        product_group_id:  String,
        /// 产品组合代码1
        comb_product1:  String,
        /// 产品组合代码2
        comb_product2:  String,
    }
    /// RCAMS空头期权风险调整参数查询
    #[derive(Debug, Clone, Default)]
    struct QryRCAMSShortOptAdjustParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 产品组合代码
        comb_product_id:  String,
    }
    /// RCAMS策略组合持仓查询
    #[derive(Debug, Clone, Default)]
    struct QryRCAMSInvestorCombPosition {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 组合合约代码
        comb_instrument_id:  String,
    }
    /// 投资者品种RCAMS保证金查询
    #[derive(Debug, Clone, Default)]
    struct QryInvestorProdRCAMSMargin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 产品组合代码
        comb_product_id:  String,
        /// 商品群代码
        product_group_id:  String,
    }
    /// RULE合约保证金参数
    #[derive(Debug, Clone, Default)]
    struct RULEInstrParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 合约类型
        instrument_class:  u8,
        /// 标准合约
        std_instrument_id:  String,
        /// 投机买折算系数
        b_spec_ratio:  f64,
        /// 投机卖折算系数
        s_spec_ratio:  f64,
        /// 套保买折算系数
        b_hedge_ratio:  f64,
        /// 套保卖折算系数
        s_hedge_ratio:  f64,
        /// 买附加风险保证金
        badd_on_margin:  f64,
        /// 卖附加风险保证金
        s_add_on_margin:  f64,
        /// 商品群号
        commodity_group_id:  i32,
    }
    /// RULE品种内对锁仓折扣参数
    #[derive(Debug, Clone, Default)]
    struct RULEIntraParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 品种代码
        prod_family_code:  String,
        /// 标准合约
        std_instrument_id:  String,
        /// 标准合约保证金
        std_instr_margin:  f64,
        /// 一般月份合约组合保证金系数
        usual_intra_rate:  f64,
        /// 临近交割合约组合保证金系数
        delivery_intra_rate:  f64,
    }
    /// RULE跨品种抵扣参数
    #[derive(Debug, Clone, Default)]
    struct RULEInterParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 优先级
        spread_id:  i32,
        /// 品种间对锁仓费率折扣比例
        inter_rate:  f64,
        /// 第一腿构成品种
        leg1_prod_family_code:  String,
        /// 第二腿构成品种
        leg2_prod_family_code:  String,
        /// 腿1比例系数
        leg1_prop_factor:  i32,
        /// 腿2比例系数
        leg2_prop_factor:  i32,
        /// 商品群号
        commodity_group_id:  i32,
        /// 商品群名称
        commodity_group_name:  String,
    }
    /// RULE合约保证金参数查询
    #[derive(Debug, Clone, Default)]
    struct QryRULEInstrParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
    }
    /// RULE品种内对锁仓折扣参数查询
    #[derive(Debug, Clone, Default)]
    struct QryRULEIntraParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 品种代码
        prod_family_code:  String,
    }
    /// RULE跨品种抵扣参数查询
    #[derive(Debug, Clone, Default)]
    struct QryRULEInterParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 第一腿构成品种
        leg1_prod_family_code:  String,
        /// 第二腿构成品种
        leg2_prod_family_code:  String,
        /// 商品群号
        commodity_group_id:  i32,
    }
    /// 投资者产品RULE保证金
    #[derive(Debug, Clone, Default)]
    struct InvestorProdRULEMargin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 品种代码
        prod_family_code:  String,
        /// 合约类型
        instrument_class:  u8,
        /// 商品群号
        commodity_group_id:  i32,
        /// 买标准持仓
        b_std_position:  f64,
        /// 卖标准持仓
        s_std_position:  f64,
        /// 买标准开仓冻结
        b_std_open_frozen:  f64,
        /// 卖标准开仓冻结
        s_std_open_frozen:  f64,
        /// 买标准平仓冻结
        b_std_close_frozen:  f64,
        /// 卖标准平仓冻结
        s_std_close_frozen:  f64,
        /// 品种内对冲标准持仓
        intra_prod_std_position:  f64,
        /// 品种内单腿标准持仓
        net_std_position:  f64,
        /// 品种间对冲标准持仓
        inter_prod_std_position:  f64,
        /// 单腿标准持仓
        single_std_position:  f64,
        /// 品种内对锁保证金
        intra_prod_margin:  f64,
        /// 品种间对锁保证金
        inter_prod_margin:  f64,
        /// 跨品种单腿保证金
        single_margin:  f64,
        /// 非组合合约保证金
        non_comb_margin:  f64,
        /// 附加保证金
        add_on_margin:  f64,
        /// 交易所保证金
        exch_margin:  f64,
        /// 附加冻结保证金
        add_on_frozen_margin:  f64,
        /// 开仓冻结保证金
        open_frozen_margin:  f64,
        /// 平仓冻结保证金
        close_frozen_margin:  f64,
        /// 品种保证金
        margin:  f64,
        /// 冻结保证金
        frozen_margin:  f64,
    }
    /// 投资者产品RULE保证金查询
    #[derive(Debug, Clone, Default)]
    struct QryInvestorProdRULEMargin {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 品种代码
        prod_family_code:  String,
        /// 商品群号
        commodity_group_id:  i32,
    }
    /// 风险结算追平SPBM组合保证金套餐
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaSPBMPortfDefinition {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 组合保证金套餐代码
        portfolio_def_id:  i32,
        /// 品种代码
        prod_family_code:  String,
        /// 是否启用SPBM
        is_spbm:  i32,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平投资者SPBM套餐选择
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaSPBMInvestPortfDef {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 组合保证金套餐代码
        portfolio_def_id:  i32,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平SPBM期货合约保证金参数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaSPBMFutureParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 品种代码
        prod_family_code:  String,
        /// 期货合约因子
        cvf:  i32,
        /// 阶段标识
        time_range:  u8,
        /// 品种保证金标准
        margin_rate:  f64,
        /// 期货合约内部对锁仓费率折扣比例
        lock_rate_x:  f64,
        /// 提高保证金标准
        add_on_rate:  f64,
        /// 昨结算价
        pre_settlement_price:  f64,
        /// 期货合约内部对锁仓附加费率折扣比例
        add_on_lock_rate_x2:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平SPBM期权合约保证金参数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaSPBMOptionParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 品种代码
        prod_family_code:  String,
        /// 期权合约因子
        cvf:  i32,
        /// 期权冲抵价格
        down_price:  f64,
        /// Delta值
        delta:  f64,
        /// 卖方期权风险转换最低值
        slim_delta:  f64,
        /// 昨结算价
        pre_settlement_price:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平SPBM品种内对锁仓折扣参数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaSPBMIntraParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 品种代码
        prod_family_code:  String,
        /// 品种内合约间对锁仓费率折扣比例
        intra_rate_y:  f64,
        /// 品种内合约间对锁仓附加费率折扣比例
        add_on_intra_rate_y2:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平SPBM跨品种抵扣参数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaSPBMInterParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 优先级
        spread_id:  i32,
        /// 品种间对锁仓费率折扣比例
        inter_rate_z:  f64,
        /// 第一腿构成品种
        leg1_prod_family_code:  String,
        /// 第二腿构成品种
        leg2_prod_family_code:  String,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平SPBM附加跨品种抵扣参数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaSPBMAddOnInterParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 优先级
        spread_id:  i32,
        /// 品种间对锁仓附加费率折扣比例
        add_on_inter_rate_z2:  f64,
        /// 第一腿构成品种
        leg1_prod_family_code:  String,
        /// 第二腿构成品种
        leg2_prod_family_code:  String,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平SPMM合约参数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaSPMMInstParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// SPMM合约保证金算法
        inst_margin_cal_id:  u8,
        /// 商品组代码
        commodity_id:  String,
        /// 商品群代码
        commodity_group_id:  String,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平SPMM产品相关参数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaSPMMProductParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 产品代码
        product_id:  String,
        /// 商品组代码
        commodity_id:  String,
        /// 商品群代码
        commodity_group_id:  String,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平投资者SPMM模板选择
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaInvestorSPMMModel {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// SPMM模板ID
        spmm_model_id:  String,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平SPMM模板参数设置
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaSPMMModelParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// SPMM模板ID
        spmm_model_id:  String,
        /// 商品群代码
        commodity_group_id:  String,
        /// SPMM品种内跨期优惠系数
        intra_commodity_rate:  f64,
        /// SPMM品种间优惠系数
        inter_commodity_rate:  f64,
        /// SPMM期权优惠系数
        option_discount_rate:  f64,
        /// 商品群最小保证金比例
        mini_margin_ratio:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平RCAMS产品组合信息
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaRCAMSCombProdInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 产品代码
        product_id:  String,
        /// 商品组代码
        comb_product_id:  String,
        /// 商品群代码
        product_group_id:  String,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平RCAMS同合约风险对冲参数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaRCAMSInstrParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 产品代码
        product_id:  String,
        /// 同合约风险对冲比率
        hedge_rate:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平RCAMS品种内风险对冲参数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaRCAMSIntraParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 产品组合代码
        comb_product_id:  String,
        /// 品种内对冲比率
        hedge_rate:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平RCAMS跨品种风险折抵参数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaRCAMSInterParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 商品群代码
        product_group_id:  String,
        /// 优先级
        priority:  i32,
        /// 折抵率
        credit_rate:  f64,
        /// 产品组合代码1
        comb_product1:  String,
        /// 产品组合代码2
        comb_product2:  String,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平RCAMS空头期权风险调整参数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaRCAMSSOptAdjParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 产品组合代码
        comb_product_id:  String,
        /// 投套标志
        hedge_flag:  u8,
        /// 空头期权风险调整标准
        adjust_value:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平RCAMS策略组合规则明细
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaRCAMSCombRuleDtl {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 策略产品
        prod_group:  Vec<u8>,
        /// 策略id
        rule_id:  Vec<u8>,
        /// 优先级
        priority:  i32,
        /// 投套标志
        hedge_flag:  u8,
        /// 组合保证金标准
        comb_margin:  f64,
        /// 交易所组合合约代码
        exchange_inst_id:  String,
        /// 单腿编号
        leg_id:  i32,
        /// 单腿合约代码
        leg_instrument_id:  String,
        /// 买卖方向
        direction:  u8,
        /// 单腿乘数
        leg_multiple:  i32,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平RCAMS策略组合持仓
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaRCAMSInvestCombPos {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 投套标志
        hedge_flag:  u8,
        /// 持仓多空方向
        posi_direction:  u8,
        /// 组合合约代码
        comb_instrument_id:  String,
        /// 单腿编号
        leg_id:  i32,
        /// 交易所组合合约代码
        exchange_inst_id:  String,
        /// 持仓量
        total_amt:  i32,
        /// 交易所保证金
        exch_margin:  f64,
        /// 投资者保证金
        margin:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平RULE合约保证金参数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaRULEInstrParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 合约类型
        instrument_class:  u8,
        /// 标准合约
        std_instrument_id:  String,
        /// 投机买折算系数
        b_spec_ratio:  f64,
        /// 投机卖折算系数
        s_spec_ratio:  f64,
        /// 套保买折算系数
        b_hedge_ratio:  f64,
        /// 套保卖折算系数
        s_hedge_ratio:  f64,
        /// 买附加风险保证金
        badd_on_margin:  f64,
        /// 卖附加风险保证金
        s_add_on_margin:  f64,
        /// 商品群号
        commodity_group_id:  i32,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平RULE品种内对锁仓折扣参数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaRULEIntraParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 品种代码
        prod_family_code:  String,
        /// 标准合约
        std_instrument_id:  String,
        /// 标准合约保证金
        std_instr_margin:  f64,
        /// 一般月份合约组合保证金系数
        usual_intra_rate:  f64,
        /// 临近交割合约组合保证金系数
        delivery_intra_rate:  f64,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 风险结算追平RULE跨品种抵扣参数
    #[derive(Debug, Clone, Default)]
    struct SyncDeltaRULEInterParameter {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易日
        trading_day:  String,
        /// 交易所代码
        exchange_id:  String,
        /// 优先级
        spread_id:  i32,
        /// 品种间对锁仓费率折扣比例
        inter_rate:  f64,
        /// 第一腿构成品种
        leg1_prod_family_code:  String,
        /// 第二腿构成品种
        leg2_prod_family_code:  String,
        /// 腿1比例系数
        leg1_prop_factor:  i32,
        /// 腿2比例系数
        leg2_prop_factor:  i32,
        /// 商品群号
        commodity_group_id:  i32,
        /// 商品群名称
        commodity_group_name:  String,
        /// 操作标志
        action_direction:  u8,
        /// 追平序号
        sync_delta_sequence_no:  i32,
    }
    /// 服务地址参数
    #[derive(Debug, Clone, Default)]
    struct IpAddrParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 服务地址
        address:  String,
        /// 交易中心代码
        dr_identity_id:  i32,
        /// 交易中心名称
        dr_identity_name:  String,
        /// 交易地址OR行情地址
        addr_srv_mode:  u8,
        /// 地址版本
        addr_ver:  u8,
        /// 服务地址编号
        addr_no:  i32,
        /// 服务地址名称
        addr_name:  String,
        /// 是否是国密地址
        is_sm:  i32,
        /// 是否是内网地址
        is_local_addr:  i32,
        /// 地址补充信息
        remark:  String,
        /// 站点
        site:  Vec<u8>,
        /// 网络运营商
        net_operator:  Vec<u8>,
    }
    /// 服务地址参数查询
    #[derive(Debug, Clone, Default)]
    struct QryIpAddrParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
    }
    /// 服务地址参数
    #[derive(Debug, Clone, Default)]
    struct TGIpAddrParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 服务地址
        address:  String,
        /// 交易中心代码
        dr_identity_id:  i32,
        /// 交易中心名称
        dr_identity_name:  String,
        /// 交易地址OR行情地址
        addr_srv_mode:  u8,
        /// 地址版本
        addr_ver:  u8,
        /// 服务地址编号
        addr_no:  i32,
        /// 服务地址名称
        addr_name:  String,
        /// 是否是国密地址
        is_sm:  i32,
        /// 是否是内网地址
        is_local_addr:  i32,
        /// 地址补充信息
        remark:  String,
        /// 站点
        site:  Vec<u8>,
        /// 网络运营商
        net_operator:  Vec<u8>,
    }
    /// 服务地址参数查询
    #[derive(Debug, Clone, Default)]
    struct QryTGIpAddrParam {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// App代码
        app_id:  String,
    }
    /// TGate会话查询状态
    #[derive(Debug, Clone, Default)]
    struct TGSessionQryStatus {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 最近30s的查询频率
        last_qry_freq:  i32,
        /// 查询状态
        qry_status:  u8,
    }
    /// 内网地址配置
    #[derive(Debug, Clone, Default)]
    struct LocalAddrConfig {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 对端地址
        peer_addr:  String,
        /// 子网掩码
        net_mask:  Vec<u8>,
        /// 交易中心代码
        dr_identity_id:  i32,
        /// 内网服务地址
        local_address:  String,
    }
    /// 内网地址配置查询
    #[derive(Debug, Clone, Default)]
    struct QryLocalAddrConfig {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
    }
    /// 次席查询银行资金账户信息请求
    #[derive(Debug, Clone, Default)]
    struct ReqQueryBankAccountBySec {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 期货公司流水号
        future_serial:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 长客户姓名
        long_customer_name:  String,
        /// 交易中心代码
        dr_identity_id:  i32,
        /// 次中心发起转账期货公司流水号
        sec_future_serial:  i32,
    }
    /// 次席查询银行资金账户信息回报
    #[derive(Debug, Clone, Default)]
    struct RspQueryBankAccountBySec {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 期货公司流水号
        future_serial:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 银行可用金额
        bank_use_amount:  f64,
        /// 银行可取金额
        bank_fetch_amount:  f64,
        /// 长客户姓名
        long_customer_name:  String,
        /// 交易中心代码
        dr_identity_id:  i32,
        /// 次中心发起转账期货公司流水号
        sec_future_serial:  i32,
    }
    /// 次中心发起的转帐交易
    #[derive(Debug, Clone, Default)]
    struct ReqTransferBySec {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 安装编号
        install_id:  i32,
        /// 期货公司流水号
        future_serial:  i32,
        /// 用户标识
        user_id:  String,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 转帐金额
        trade_amount:  f64,
        /// 期货可取金额
        future_fetch_amount:  f64,
        /// 费用支付标志
        fee_pay_flag:  u8,
        /// 应收客户费用
        cust_fee:  f64,
        /// 应收期货公司费用
        broker_fee:  f64,
        /// 发送方给接收方的消息
        message:  Vec<u8>,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 转账交易状态
        transfer_status:  u8,
        /// 长客户姓名
        long_customer_name:  String,
        /// 交易中心代码
        dr_identity_id:  i32,
        /// 次中心发起转账期货公司流水号
        sec_future_serial:  i32,
    }
    /// 次中心发起的转帐交易回报
    #[derive(Debug, Clone, Default)]
    struct RspTransferBySec {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 安装编号
        install_id:  i32,
        /// 期货公司流水号
        future_serial:  i32,
        /// 用户标识
        user_id:  String,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 转帐金额
        trade_amount:  f64,
        /// 期货可取金额
        future_fetch_amount:  f64,
        /// 费用支付标志
        fee_pay_flag:  u8,
        /// 应收客户费用
        cust_fee:  f64,
        /// 应收期货公司费用
        broker_fee:  f64,
        /// 发送方给接收方的消息
        message:  Vec<u8>,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 转账交易状态
        transfer_status:  u8,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 长客户姓名
        long_customer_name:  String,
        /// 交易中心代码
        dr_identity_id:  i32,
        /// 次中心发起转账期货公司流水号
        sec_future_serial:  i32,
    }
    /// 查询银行资金账户信息通知 要发往次席
    #[derive(Debug, Clone, Default)]
    struct NotifyQueryFutureAccountBySec {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 业务功能码
        trade_code:  String,
        /// 银行代码
        bank_id:  String,
        /// 银行分支机构代码
        bank_branch_id:  String,
        /// 期商代码
        broker_id:  String,
        /// 期商分支机构代码
        broker_branch_id:  String,
        /// 交易日期
        trade_date:  String,
        /// 交易时间
        trade_time:  String,
        /// 银行流水号
        bank_serial:  String,
        /// 交易系统日期
        trading_day:  String,
        /// 银期平台消息流水号
        plate_serial:  i32,
        /// 最后分片标志
        last_fragment:  u8,
        /// 会话号
        session_id:  i32,
        /// 客户姓名
        customer_name:  String,
        /// 证件类型
        id_card_type:  u8,
        /// 证件号码
        identified_card_no:  String,
        /// 客户类型
        cust_type:  u8,
        /// 银行帐号
        bank_account:  String,
        /// 银行密码
        bank_pass_word:  String,
        /// 投资者帐号
        account_id:  String,
        /// 期货密码
        password:  String,
        /// 期货公司流水号
        future_serial:  i32,
        /// 安装编号
        install_id:  i32,
        /// 用户标识
        user_id:  String,
        /// 验证客户证件号码标志
        verify_cert_no_flag:  u8,
        /// 币种代码
        currency_id:  String,
        /// 摘要
        digest:  Vec<u8>,
        /// 银行帐号类型
        bank_acc_type:  u8,
        /// 渠道标志
        device_id:  String,
        /// 期货单位帐号类型
        bank_secu_acc_type:  u8,
        /// 期货公司银行编码
        broker_id_by_bank:  Vec<u8>,
        /// 期货单位帐号
        bank_secu_acc:  Vec<u8>,
        /// 银行密码标志
        bank_pwd_flag:  u8,
        /// 期货资金密码核对标志
        secu_pwd_flag:  u8,
        /// 交易柜员
        oper_no:  String,
        /// 请求编号
        request_id:  i32,
        /// 交易ID
        tid:  i32,
        /// 银行可用金额
        bank_use_amount:  f64,
        /// 银行可取金额
        bank_fetch_amount:  f64,
        /// 错误代码
        error_id:  i32,
        /// 错误信息
        error_msg:  String,
        /// 长客户姓名
        long_customer_name:  String,
        /// 交易中心代码
        dr_identity_id:  i32,
        /// 次中心发起转账期货公司流水号
        sec_future_serial:  i32,
    }
    /// 退出紧急状态参数
    #[derive(Debug, Clone, Default)]
    struct ExitEmergency {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
    }
    /// 新组保保证金系数投资者模板对应关系
    #[derive(Debug, Clone, Default)]
    struct InvestorPortfMarginModel {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 保证金系数模板
        margin_model_id:  String,
    }
    /// 投资者新组保设置
    #[derive(Debug, Clone, Default)]
    struct InvestorPortfSetting {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者编号
        investor_id:  String,
        /// 投机套保标志
        hedge_flag:  u8,
        /// 是否开启新组保
        use_portf:  i32,
    }
    /// 投资者新组保设置查询
    #[derive(Debug, Clone, Default)]
    struct QryInvestorPortfSetting {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者编号
        investor_id:  String,
    }
    /// 来自次席的用户口令变更
    #[derive(Debug, Clone, Default)]
    struct UserPasswordUpdateFromSec {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 原来的口令
        old_password:  String,
        /// 新的口令
        new_password:  String,
        /// 次席的交易中心代码
        from_sec:  i32,
    }
    /// 来自次席的结算结果确认
    #[derive(Debug, Clone, Default)]
    struct SettlementInfoConfirmFromSec {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 确认日期
        confirm_date:  String,
        /// 确认时间
        confirm_time:  String,
        /// 次席的交易中心代码
        from_sec:  i32,
    }
    /// 来自次席的资金账户口令变更
    #[derive(Debug, Clone, Default)]
    struct TradingAccountPasswordUpdateFromSec {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者帐号
        account_id:  String,
        /// 原来的口令
        old_password:  String,
        /// 新的口令
        new_password:  String,
        /// 币种代码
        currency_id:  String,
        /// 次席的交易中心代码
        from_sec:  i32,
    }
    /// 风控禁止的合约交易权限
    #[derive(Debug, Clone, Default)]
    struct RiskForbiddenRight {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者编号
        investor_id:  String,
        /// 合约/产品代码
        instrument_id:  String,
        /// 用户代码
        user_id:  String,
    }
    /// 投资者申报费阶梯收取记录
    #[derive(Debug, Clone, Default)]
    struct InvestorInfoCommRec {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 交易所代码
        exchange_id:  String,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 商品代码
        instrument_id:  String,
        /// 报单总笔数
        order_count:  i32,
        /// 撤单总笔数
        order_action_count:  i32,
        /// 询价总次数
        for_quote_cnt:  i32,
        /// 申报费
        info_comm:  f64,
        /// 是否期权系列
        is_opt_series:  i32,
        /// 品种代码
        product_id:  String,
        /// 信息量总量
        info_cnt:  i32,
    }
    /// 投资者申报费阶梯收取记录查询
    #[derive(Debug, Clone, Default)]
    struct QryInvestorInfoCommRec {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 投资者代码
        investor_id:  String,
        /// 商品代码
        instrument_id:  String,
        /// 经纪公司代码
        broker_id:  String,
    }
    /// 组合腿信息
    #[derive(Debug, Clone, Default)]
    struct CombLeg {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 组合合约代码
        comb_instrument_id:  String,
        /// 单腿编号
        leg_id:  i32,
        /// 单腿合约代码
        leg_instrument_id:  String,
        /// 买卖方向
        direction:  u8,
        /// 单腿乘数
        leg_multiple:  i32,
        /// 派生层数
        imply_level:  i32,
    }
    /// 组合腿信息查询
    #[derive(Debug, Clone, Default)]
    struct QryCombLeg {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 单腿合约代码
        leg_instrument_id:  String,
    }
    /// 输入的对冲设置
    #[derive(Debug, Clone, Default)]
    struct InputOffsetSetting {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 标的期货合约代码
        underlying_instr_id:  String,
        /// 产品代码
        product_id:  String,
        /// 对冲类型
        offset_type:  u8,
        /// 申请对冲的合约数量
        volume:  i32,
        /// 是否对冲
        is_offset:  i32,
        /// 请求编号
        request_id:  i32,
        /// 用户代码
        user_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// IP地址
        ip_address:  String,
        /// Mac地址
        mac_address:  String,
    }
    /// 对冲设置
    #[derive(Debug, Clone, Default)]
    struct OffsetSetting {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 标的期货合约代码
        underlying_instr_id:  String,
        /// 产品代码
        product_id:  String,
        /// 对冲类型
        offset_type:  u8,
        /// 申请对冲的合约数量
        volume:  i32,
        /// 是否对冲
        is_offset:  i32,
        /// 请求编号
        request_id:  i32,
        /// 用户代码
        user_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// IP地址
        ip_address:  String,
        /// Mac地址
        mac_address:  String,
        /// 交易所合约代码
        exchange_inst_id:  String,
        /// 交易所期权系列号
        exchange_serial_no:  Vec<u8>,
        /// 交易所产品代码
        exchange_product_id:  String,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 对冲提交状态
        order_submit_status:  u8,
        /// 交易日
        trading_day:  String,
        /// 结算编号
        settlement_id:  i32,
        /// 报单日期
        insert_date:  String,
        /// 插入时间
        insert_time:  String,
        /// 撤销时间
        cancel_time:  String,
        /// 对冲设置结果
        exec_result:  u8,
        /// 序号
        sequence_no:  i32,
        /// 前置编号
        front_id:  i32,
        /// 会话编号
        session_id:  i32,
        /// 状态信息
        status_msg:  String,
        /// 操作用户代码
        active_user_id:  String,
        /// 经纪公司报单编号
        broker_offset_setting_seq:  i32,
        /// 申请来源
        apply_src:  u8,
    }
    /// 撤销对冲设置
    #[derive(Debug, Clone, Default)]
    struct CancelOffsetSetting {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 合约代码
        instrument_id:  String,
        /// 标的期货合约代码
        underlying_instr_id:  String,
        /// 产品代码
        product_id:  String,
        /// 对冲类型
        offset_type:  u8,
        /// 申请对冲的合约数量
        volume:  i32,
        /// 是否对冲
        is_offset:  i32,
        /// 请求编号
        request_id:  i32,
        /// 用户代码
        user_id:  String,
        /// 交易所代码
        exchange_id:  String,
        /// IP地址
        ip_address:  String,
        /// Mac地址
        mac_address:  String,
        /// 交易所合约代码
        exchange_inst_id:  String,
        /// 交易所期权系列号
        exchange_serial_no:  Vec<u8>,
        /// 交易所产品代码
        exchange_product_id:  String,
        /// 交易所交易员代码
        trader_id:  String,
        /// 安装编号
        install_id:  i32,
        /// 会员代码
        participant_id:  String,
        /// 客户代码
        client_id:  String,
        /// 报单操作状态
        order_action_status:  u8,
        /// 状态信息
        status_msg:  String,
        /// 操作本地编号
        action_local_id:  String,
        /// 操作日期
        action_date:  String,
        /// 操作时间
        action_time:  String,
    }
    /// 查询对冲设置
    #[derive(Debug, Clone, Default)]
    struct QryOffsetSetting {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 产品代码
        product_id:  String,
        /// 对冲类型
        offset_type:  u8,
    }
    /// 服务地址和AppID的关系
    #[derive(Debug, Clone, Default)]
    struct AddrAppIDRelation {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 服务地址
        address:  String,
        /// 交易中心代码
        dr_identity_id:  i32,
        /// App代码
        app_id:  String,
    }
    /// 服务地址和AppID的关系查询
    #[derive(Debug, Clone, Default)]
    struct QryAddrAppIDRelation {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
    }
    /// 微信小程序等用户系统信息
    #[derive(Debug, Clone, Default)]
    struct WechatUserSystemInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 微信小程序等用户端系统内部信息长度
        wechat_clt_sys_info_len:  i32,
        /// 微信小程序等用户端系统内部信息
        wechat_clt_sys_info:  String,
        /// 终端IP端口
        client_ip_port:  i32,
        /// 登录成功时间
        client_login_time:  String,
        /// App代码
        client_app_id:  String,
        /// 用户公网IP
        client_public_ip:  String,
        /// 客户登录备注2
        client_login_remark:  String,
    }
    /// 投资者预留信息
    #[derive(Debug, Clone, Default)]
    struct InvestorReserveInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 用户代码
        user_id:  String,
        /// 预留信息
        reserve_info:  String,
    }
    /// 查询组织架构投资者对应关系
    #[derive(Debug, Clone, Default)]
    struct QryInvestorDepartmentFlat {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
    }
    /// 组织架构投资者对应关系
    #[derive(Debug, Clone, Default)]
    struct InvestorDepartmentFlat {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
        /// 投资者代码
        investor_id:  String,
        /// 组织架构代码
        department_id:  String,
    }
    /// 查询操作员组织架构关系
    #[derive(Debug, Clone, Default)]
    struct QryDepartmentUser {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 经纪公司代码
        broker_id:  String,
    }
    /// 前置信息
    #[derive(Debug, Clone, Default)]
    struct FrontInfo {
        /// C++ 端传入的整体 Field 是否为 `nullptr`
        is_null: bool,
        /// 前置地址
        front_addr:  String,
        /// 查询流控
        qry_freq:  i32,
        /// FTD流控
        ftd_pkg_freq:  i32,
    }
}