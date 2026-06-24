#include "ctp-rs/wrapper/include/TraderApi.h"
#include "ctp-rs/wrapper/include/Converter.h"

// On the macOS-dlopen build the api pointer is typed as the darwin shim
// (CThostFtdcTraderApiDarwinShim) so vtable indices match the dylib's
// compile-time view. The 8 methods linux 6.7.11 added but darwin 6.7.7
// lacks (RegisterWechatUserSystemInfo, SubmitWechatUserSystemInfo,
// ReqQryUserSession, ReqQryInvestorInfoCommRec, ReqQryCombLeg,
// ReqOffsetSetting, ReqCancelOffsetSetting, ReqQryOffsetSetting) aren't
// in the shim's vtable at all — calling them would not link. Their bodies
// are gated with this macro so they short-circuit to -1 ("not supported")
// on the macOS-dlopen build.
#if defined(__APPLE__) && defined(CTP_RS_DARWIN_TRADER_DLOPEN)
#define CTP_RS_LINUX_ONLY_API 0
#else
#define CTP_RS_LINUX_ONLY_API 1
#endif

#if defined(__APPLE__) && defined(CTP_RS_DARWIN_TRADER_DLOPEN)
// Implemented in TraderApiDarwinShim.cpp. Used when the trader-side library
// is the embedded framework dylib; localctp builds bypass these by linking
// the real CThostFtdcTraderApi statically.
extern "C" void* CtpRsDarwinCreateFtdcTraderApi(const char*, bool);
extern "C" const char* CtpRsDarwinGetTraderApiVersion();
#endif

#if defined(CTP_RS_LOCALCTP)
// Implemented in localctp/LocalCTP.cpp. Blocks until LocalCTP's settlement
// timer thread finishes its first-pass init. Without this, user-side SPI
// callbacks can race the timer thread's mid-init mutation of shared state
// and crash with "mutex lock failed: Invalid argument".
extern "C" void localctp_wait_until_ready();
#endif

TraderApi::TraderApi(rust::Box<TraderSpi> gateway, rust::String flow_path, bool is_production_mode) {
    spi = new CTraderSpi(std::move(gateway));
#if defined(__APPLE__) && defined(CTP_RS_DARWIN_TRADER_DLOPEN)
    // Cast to the darwin-shaped shim base — this gives the compiler a vtable
    // layout that matches the dlopened dylib's compile-time view, so every
    // virtual call dispatches to the correct slot. See DarwinSdkShim.h.
    api = static_cast<CtpRsTraderApiBase*>(
        CtpRsDarwinCreateFtdcTraderApi(flow_path.c_str(), is_production_mode));
    api->RegisterSpi(spi);
#else
    api = CThostFtdcTraderApi::CreateFtdcTraderApi(flow_path.c_str(), is_production_mode);
    api->RegisterSpi(spi);
#endif
}

TraderApi::~TraderApi() {
    if (api) {
        api->Release();
        api = nullptr;
    }
    delete spi;
    spi = nullptr;
}

std::unique_ptr<TraderApi> CreateTraderApi(rust::Box<TraderSpi> gateway, rust::String flow_path, bool is_production_mode) {
#if defined(CTP_RS_LOCALCTP)
    localctp_wait_until_ready();
#endif
    return std::make_unique<TraderApi>(std::move(gateway), flow_path, is_production_mode);
}

FrontInfo TraderApi::GetFrontInfo() const {
    CThostFtdcFrontInfoField req{};
    memset(&req, 0, sizeof(req));
    api->GetFrontInfo(&req);
    return Converter::CThostFtdcFrontInfoFieldToRust(&req);
}

rust::String TraderApi::get_api_version() const {
#if defined(__APPLE__) && defined(CTP_RS_DARWIN_TRADER_DLOPEN)
    return CtpRsDarwinGetTraderApiVersion();
#else
    return api->GetApiVersion(
    );
#endif
}

void TraderApi::init() const {
    return api->Init(
    );
}

int32_t TraderApi::join() const {
    return api->Join(
    );
}

rust::String TraderApi::get_trading_day() const {
    return api->GetTradingDay(
    );
}

void TraderApi::register_front(rust::String front_address) const {
    return api->RegisterFront(
        const_cast<char *>(front_address.c_str())
    );
}

void TraderApi::register_name_server(rust::String ns_address) const {
    return api->RegisterNameServer(
        const_cast<char *>(ns_address.c_str())
    );
}

void TraderApi::register_fens_user_info(FensUserInfo fens_user_info) const {
    CThostFtdcFensUserInfoField req(Converter::FensUserInfoToCpp(fens_user_info));
    return api->RegisterFensUserInfo(
        &req
    );
}

void TraderApi::subscribe_private_topic(int32_t resume_type) const {
    return api->SubscribePrivateTopic(
        (THOST_TE_RESUME_TYPE)resume_type
    );
}

void TraderApi::subscribe_public_topic(int32_t resume_type) const {
    return api->SubscribePublicTopic(
        (THOST_TE_RESUME_TYPE)resume_type
    );
}

int32_t TraderApi::req_authenticate(ReqAuthenticate req_authenticate, int32_t request_id) const {
    CThostFtdcReqAuthenticateField req(Converter::ReqAuthenticateToCpp(req_authenticate));
    return api->ReqAuthenticate(
        &req,
        request_id
    );
}

int32_t TraderApi::register_user_system_info(UserSystemInfo user_system_info) const {
    CThostFtdcUserSystemInfoField req(Converter::UserSystemInfoToCpp(user_system_info));
    return api->RegisterUserSystemInfo(
        &req
    );
}

int32_t TraderApi::submit_user_system_info(UserSystemInfo user_system_info) const {
    CThostFtdcUserSystemInfoField req(Converter::UserSystemInfoToCpp(user_system_info));
    return api->SubmitUserSystemInfo(
        &req
    );
}

int32_t TraderApi::register_wechat_user_system_info(WechatUserSystemInfo user_system_info) const {
#if CTP_RS_LINUX_ONLY_API
    CThostFtdcWechatUserSystemInfoField req(Converter::WechatUserSystemInfoToCpp(user_system_info));
    return api->RegisterWechatUserSystemInfo(&req);
#else
    (void)pUserSystemInfo;
    return -1;
#endif
}

int32_t TraderApi::submit_wechat_user_system_info(WechatUserSystemInfo user_system_info) const {
#if CTP_RS_LINUX_ONLY_API
    CThostFtdcWechatUserSystemInfoField req(Converter::WechatUserSystemInfoToCpp(user_system_info));
    return api->SubmitWechatUserSystemInfo(&req);
#else
    (void)pUserSystemInfo;
    return -1;
#endif
}

int32_t TraderApi::req_user_login(ReqUserLogin req_user_login, int32_t request_id) const {
    CThostFtdcReqUserLoginField req(Converter::ReqUserLoginToCpp(req_user_login));
#if defined(__APPLE__) && defined(CTP_RS_DARWIN_TRADER_DLOPEN)
    // Darwin's ReqUserLogin takes two extra args (system-info length + 273-byte
    // blob) because the framework dylib lacks the data-collect helper that the
    // linux/windows dylibs use to auto-embed terminal info into the wire
    // payload. We don't ship that helper for darwin, so pass an empty blob —
    // the call now reaches the right vtable slot with the right argument count
    // (without this fix it would dispatch to a different darwin method
    // entirely). The CFFEX-mandated terminal-info content is still missing,
    // which is why SimNow's front silently drops the resulting login; that's
    // a server-policy issue separate from the ABI fix and isn't addressed
    // here.
    char system_info[273] = {0};
    return api->ReqUserLogin(&req, request_id, 0, system_info);
#else
    return api->ReqUserLogin(&req, request_id);
#endif
}

int32_t TraderApi::req_user_logout(UserLogout user_logout, int32_t request_id) const {
    CThostFtdcUserLogoutField req(Converter::UserLogoutToCpp(user_logout));
    return api->ReqUserLogout(
        &req,
        request_id
    );
}

int32_t TraderApi::req_user_password_update(UserPasswordUpdate user_password_update, int32_t request_id) const {
    CThostFtdcUserPasswordUpdateField req(Converter::UserPasswordUpdateToCpp(user_password_update));
    return api->ReqUserPasswordUpdate(
        &req,
        request_id
    );
}

int32_t TraderApi::req_trading_account_password_update(TradingAccountPasswordUpdate trading_account_password_update, int32_t request_id) const {
    CThostFtdcTradingAccountPasswordUpdateField req(Converter::TradingAccountPasswordUpdateToCpp(trading_account_password_update));
    return api->ReqTradingAccountPasswordUpdate(
        &req,
        request_id
    );
}

int32_t TraderApi::req_user_auth_method(ReqUserAuthMethod req_user_auth_method, int32_t request_id) const {
    CThostFtdcReqUserAuthMethodField req(Converter::ReqUserAuthMethodToCpp(req_user_auth_method));
    return api->ReqUserAuthMethod(
        &req,
        request_id
    );
}

int32_t TraderApi::req_gen_user_captcha(ReqGenUserCaptcha req_gen_user_captcha, int32_t request_id) const {
    CThostFtdcReqGenUserCaptchaField req(Converter::ReqGenUserCaptchaToCpp(req_gen_user_captcha));
    return api->ReqGenUserCaptcha(
        &req,
        request_id
    );
}

int32_t TraderApi::req_gen_user_text(ReqGenUserText req_gen_user_text, int32_t request_id) const {
    CThostFtdcReqGenUserTextField req(Converter::ReqGenUserTextToCpp(req_gen_user_text));
    return api->ReqGenUserText(
        &req,
        request_id
    );
}

int32_t TraderApi::req_user_login_with_captcha(ReqUserLoginWithCaptcha req_user_login_with_captcha, int32_t request_id) const {
    CThostFtdcReqUserLoginWithCaptchaField req(Converter::ReqUserLoginWithCaptchaToCpp(req_user_login_with_captcha));
    return api->ReqUserLoginWithCaptcha(
        &req,
        request_id
    );
}

int32_t TraderApi::req_user_login_with_text(ReqUserLoginWithText req_user_login_with_text, int32_t request_id) const {
    CThostFtdcReqUserLoginWithTextField req(Converter::ReqUserLoginWithTextToCpp(req_user_login_with_text));
    return api->ReqUserLoginWithText(
        &req,
        request_id
    );
}

int32_t TraderApi::req_user_login_with_otp(ReqUserLoginWithOTP req_user_login_with_otp, int32_t request_id) const {
    CThostFtdcReqUserLoginWithOTPField req(Converter::ReqUserLoginWithOTPToCpp(req_user_login_with_otp));
    return api->ReqUserLoginWithOTP(
        &req,
        request_id
    );
}

int32_t TraderApi::req_order_insert(InputOrder input_order, int32_t request_id) const {
    CThostFtdcInputOrderField req(Converter::InputOrderToCpp(input_order));
    return api->ReqOrderInsert(
        &req,
        request_id
    );
}

int32_t TraderApi::req_parked_order_insert(ParkedOrder parked_order, int32_t request_id) const {
    CThostFtdcParkedOrderField req(Converter::ParkedOrderToCpp(parked_order));
    return api->ReqParkedOrderInsert(
        &req,
        request_id
    );
}

int32_t TraderApi::req_parked_order_action(ParkedOrderAction parked_order_action, int32_t request_id) const {
    CThostFtdcParkedOrderActionField req(Converter::ParkedOrderActionToCpp(parked_order_action));
    return api->ReqParkedOrderAction(
        &req,
        request_id
    );
}

int32_t TraderApi::req_order_action(InputOrderAction input_order_action, int32_t request_id) const {
    CThostFtdcInputOrderActionField req(Converter::InputOrderActionToCpp(input_order_action));
    return api->ReqOrderAction(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_max_order_volume(QryMaxOrderVolume qry_max_order_volume, int32_t request_id) const {
    CThostFtdcQryMaxOrderVolumeField req(Converter::QryMaxOrderVolumeToCpp(qry_max_order_volume));
    return api->ReqQryMaxOrderVolume(
        &req,
        request_id
    );
}

int32_t TraderApi::req_settlement_info_confirm(SettlementInfoConfirm settlement_info_confirm, int32_t request_id) const {
    CThostFtdcSettlementInfoConfirmField req(Converter::SettlementInfoConfirmToCpp(settlement_info_confirm));
    return api->ReqSettlementInfoConfirm(
        &req,
        request_id
    );
}

int32_t TraderApi::req_remove_parked_order(RemoveParkedOrder remove_parked_order, int32_t request_id) const {
    CThostFtdcRemoveParkedOrderField req(Converter::RemoveParkedOrderToCpp(remove_parked_order));
    return api->ReqRemoveParkedOrder(
        &req,
        request_id
    );
}

int32_t TraderApi::req_remove_parked_order_action(RemoveParkedOrderAction remove_parked_order_action, int32_t request_id) const {
    CThostFtdcRemoveParkedOrderActionField req(Converter::RemoveParkedOrderActionToCpp(remove_parked_order_action));
    return api->ReqRemoveParkedOrderAction(
        &req,
        request_id
    );
}

int32_t TraderApi::req_exec_order_insert(InputExecOrder input_exec_order, int32_t request_id) const {
    CThostFtdcInputExecOrderField req(Converter::InputExecOrderToCpp(input_exec_order));
    return api->ReqExecOrderInsert(
        &req,
        request_id
    );
}

int32_t TraderApi::req_exec_order_action(InputExecOrderAction input_exec_order_action, int32_t request_id) const {
    CThostFtdcInputExecOrderActionField req(Converter::InputExecOrderActionToCpp(input_exec_order_action));
    return api->ReqExecOrderAction(
        &req,
        request_id
    );
}

int32_t TraderApi::req_for_quote_insert(InputForQuote input_for_quote, int32_t request_id) const {
    CThostFtdcInputForQuoteField req(Converter::InputForQuoteToCpp(input_for_quote));
    return api->ReqForQuoteInsert(
        &req,
        request_id
    );
}

int32_t TraderApi::req_quote_insert(InputQuote input_quote, int32_t request_id) const {
    CThostFtdcInputQuoteField req(Converter::InputQuoteToCpp(input_quote));
    return api->ReqQuoteInsert(
        &req,
        request_id
    );
}

int32_t TraderApi::req_quote_action(InputQuoteAction input_quote_action, int32_t request_id) const {
    CThostFtdcInputQuoteActionField req(Converter::InputQuoteActionToCpp(input_quote_action));
    return api->ReqQuoteAction(
        &req,
        request_id
    );
}

int32_t TraderApi::req_batch_order_action(InputBatchOrderAction input_batch_order_action, int32_t request_id) const {
    CThostFtdcInputBatchOrderActionField req(Converter::InputBatchOrderActionToCpp(input_batch_order_action));
    return api->ReqBatchOrderAction(
        &req,
        request_id
    );
}

int32_t TraderApi::req_option_self_close_insert(InputOptionSelfClose input_option_self_close, int32_t request_id) const {
    CThostFtdcInputOptionSelfCloseField req(Converter::InputOptionSelfCloseToCpp(input_option_self_close));
    return api->ReqOptionSelfCloseInsert(
        &req,
        request_id
    );
}

int32_t TraderApi::req_option_self_close_action(InputOptionSelfCloseAction input_option_self_close_action, int32_t request_id) const {
    CThostFtdcInputOptionSelfCloseActionField req(Converter::InputOptionSelfCloseActionToCpp(input_option_self_close_action));
    return api->ReqOptionSelfCloseAction(
        &req,
        request_id
    );
}

int32_t TraderApi::req_comb_action_insert(InputCombAction input_comb_action, int32_t request_id) const {
    CThostFtdcInputCombActionField req(Converter::InputCombActionToCpp(input_comb_action));
    return api->ReqCombActionInsert(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_order(QryOrder qry_order, int32_t request_id) const {
    CThostFtdcQryOrderField req(Converter::QryOrderToCpp(qry_order));
    return api->ReqQryOrder(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_trade(QryTrade qry_trade, int32_t request_id) const {
    CThostFtdcQryTradeField req(Converter::QryTradeToCpp(qry_trade));
    return api->ReqQryTrade(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_investor_position(QryInvestorPosition qry_investor_position, int32_t request_id) const {
    CThostFtdcQryInvestorPositionField req(Converter::QryInvestorPositionToCpp(qry_investor_position));
    return api->ReqQryInvestorPosition(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_trading_account(QryTradingAccount qry_trading_account, int32_t request_id) const {
    CThostFtdcQryTradingAccountField req(Converter::QryTradingAccountToCpp(qry_trading_account));
    return api->ReqQryTradingAccount(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_investor(QryInvestor qry_investor, int32_t request_id) const {
    CThostFtdcQryInvestorField req(Converter::QryInvestorToCpp(qry_investor));
    return api->ReqQryInvestor(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_trading_code(QryTradingCode qry_trading_code, int32_t request_id) const {
    CThostFtdcQryTradingCodeField req(Converter::QryTradingCodeToCpp(qry_trading_code));
    return api->ReqQryTradingCode(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_instrument_margin_rate(QryInstrumentMarginRate qry_instrument_margin_rate, int32_t request_id) const {
    CThostFtdcQryInstrumentMarginRateField req(Converter::QryInstrumentMarginRateToCpp(qry_instrument_margin_rate));
    return api->ReqQryInstrumentMarginRate(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_instrument_commission_rate(QryInstrumentCommissionRate qry_instrument_commission_rate, int32_t request_id) const {
    CThostFtdcQryInstrumentCommissionRateField req(Converter::QryInstrumentCommissionRateToCpp(qry_instrument_commission_rate));
    return api->ReqQryInstrumentCommissionRate(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_user_session(QryUserSession qry_user_session, int32_t request_id) const {
#if CTP_RS_LINUX_ONLY_API
    CThostFtdcQryUserSessionField req(Converter::QryUserSessionToCpp(qry_user_session));
    return api->ReqQryUserSession(&req, request_id);
#else
    (void)pQryUserSession; (void)nRequestID;
    return -1;
#endif
}

int32_t TraderApi::req_qry_exchange(QryExchange qry_exchange, int32_t request_id) const {
    CThostFtdcQryExchangeField req(Converter::QryExchangeToCpp(qry_exchange));
    return api->ReqQryExchange(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_product(QryProduct qry_product, int32_t request_id) const {
    CThostFtdcQryProductField req(Converter::QryProductToCpp(qry_product));
    return api->ReqQryProduct(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_instrument(QryInstrument qry_instrument, int32_t request_id) const {
    CThostFtdcQryInstrumentField req(Converter::QryInstrumentToCpp(qry_instrument));
    return api->ReqQryInstrument(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_depth_market_data(QryDepthMarketData qry_depth_market_data, int32_t request_id) const {
    CThostFtdcQryDepthMarketDataField req(Converter::QryDepthMarketDataToCpp(qry_depth_market_data));
    return api->ReqQryDepthMarketData(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_trader_offer(QryTraderOffer qry_trader_offer, int32_t request_id) const {
    CThostFtdcQryTraderOfferField req(Converter::QryTraderOfferToCpp(qry_trader_offer));
    return api->ReqQryTraderOffer(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_settlement_info(QrySettlementInfo qry_settlement_info, int32_t request_id) const {
    CThostFtdcQrySettlementInfoField req(Converter::QrySettlementInfoToCpp(qry_settlement_info));
    return api->ReqQrySettlementInfo(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_transfer_bank(QryTransferBank qry_transfer_bank, int32_t request_id) const {
    CThostFtdcQryTransferBankField req(Converter::QryTransferBankToCpp(qry_transfer_bank));
    return api->ReqQryTransferBank(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_investor_position_detail(QryInvestorPositionDetail qry_investor_position_detail, int32_t request_id) const {
    CThostFtdcQryInvestorPositionDetailField req(Converter::QryInvestorPositionDetailToCpp(qry_investor_position_detail));
    return api->ReqQryInvestorPositionDetail(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_notice(QryNotice qry_notice, int32_t request_id) const {
    CThostFtdcQryNoticeField req(Converter::QryNoticeToCpp(qry_notice));
    return api->ReqQryNotice(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_settlement_info_confirm(QrySettlementInfoConfirm qry_settlement_info_confirm, int32_t request_id) const {
    CThostFtdcQrySettlementInfoConfirmField req(Converter::QrySettlementInfoConfirmToCpp(qry_settlement_info_confirm));
    return api->ReqQrySettlementInfoConfirm(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_investor_position_combine_detail(QryInvestorPositionCombineDetail qry_investor_position_combine_detail, int32_t request_id) const {
    CThostFtdcQryInvestorPositionCombineDetailField req(Converter::QryInvestorPositionCombineDetailToCpp(qry_investor_position_combine_detail));
    return api->ReqQryInvestorPositionCombineDetail(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_cfmmc_trading_account_key(QryCFMMCTradingAccountKey qry_cfmmc_trading_account_key, int32_t request_id) const {
    CThostFtdcQryCFMMCTradingAccountKeyField req(Converter::QryCFMMCTradingAccountKeyToCpp(qry_cfmmc_trading_account_key));
    return api->ReqQryCFMMCTradingAccountKey(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_ewarrant_offset(QryEWarrantOffset qry_ewarrant_offset, int32_t request_id) const {
    CThostFtdcQryEWarrantOffsetField req(Converter::QryEWarrantOffsetToCpp(qry_ewarrant_offset));
    return api->ReqQryEWarrantOffset(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_investor_product_group_margin(QryInvestorProductGroupMargin qry_investor_product_group_margin, int32_t request_id) const {
    CThostFtdcQryInvestorProductGroupMarginField req(Converter::QryInvestorProductGroupMarginToCpp(qry_investor_product_group_margin));
    return api->ReqQryInvestorProductGroupMargin(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_exchange_margin_rate(QryExchangeMarginRate qry_exchange_margin_rate, int32_t request_id) const {
    CThostFtdcQryExchangeMarginRateField req(Converter::QryExchangeMarginRateToCpp(qry_exchange_margin_rate));
    return api->ReqQryExchangeMarginRate(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_exchange_margin_rate_adjust(QryExchangeMarginRateAdjust qry_exchange_margin_rate_adjust, int32_t request_id) const {
    CThostFtdcQryExchangeMarginRateAdjustField req(Converter::QryExchangeMarginRateAdjustToCpp(qry_exchange_margin_rate_adjust));
    return api->ReqQryExchangeMarginRateAdjust(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_exchange_rate(QryExchangeRate qry_exchange_rate, int32_t request_id) const {
    CThostFtdcQryExchangeRateField req(Converter::QryExchangeRateToCpp(qry_exchange_rate));
    return api->ReqQryExchangeRate(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_sec_agent_acid_map(QrySecAgentACIDMap qry_sec_agent_acid_map, int32_t request_id) const {
    CThostFtdcQrySecAgentACIDMapField req(Converter::QrySecAgentACIDMapToCpp(qry_sec_agent_acid_map));
    return api->ReqQrySecAgentACIDMap(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_product_exch_rate(QryProductExchRate qry_product_exch_rate, int32_t request_id) const {
    CThostFtdcQryProductExchRateField req(Converter::QryProductExchRateToCpp(qry_product_exch_rate));
    return api->ReqQryProductExchRate(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_product_group(QryProductGroup qry_product_group, int32_t request_id) const {
    CThostFtdcQryProductGroupField req(Converter::QryProductGroupToCpp(qry_product_group));
    return api->ReqQryProductGroup(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_mm_instrument_commission_rate(QryMMInstrumentCommissionRate qry_mm_instrument_commission_rate, int32_t request_id) const {
    CThostFtdcQryMMInstrumentCommissionRateField req(Converter::QryMMInstrumentCommissionRateToCpp(qry_mm_instrument_commission_rate));
    return api->ReqQryMMInstrumentCommissionRate(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_mm_option_instr_comm_rate(QryMMOptionInstrCommRate qry_mm_option_instr_comm_rate, int32_t request_id) const {
    CThostFtdcQryMMOptionInstrCommRateField req(Converter::QryMMOptionInstrCommRateToCpp(qry_mm_option_instr_comm_rate));
    return api->ReqQryMMOptionInstrCommRate(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_instrument_order_comm_rate(QryInstrumentOrderCommRate qry_instrument_order_comm_rate, int32_t request_id) const {
    CThostFtdcQryInstrumentOrderCommRateField req(Converter::QryInstrumentOrderCommRateToCpp(qry_instrument_order_comm_rate));
    return api->ReqQryInstrumentOrderCommRate(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_sec_agent_trading_account(QryTradingAccount qry_trading_account, int32_t request_id) const {
    CThostFtdcQryTradingAccountField req(Converter::QryTradingAccountToCpp(qry_trading_account));
    return api->ReqQrySecAgentTradingAccount(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_sec_agent_check_mode(QrySecAgentCheckMode qry_sec_agent_check_mode, int32_t request_id) const {
    CThostFtdcQrySecAgentCheckModeField req(Converter::QrySecAgentCheckModeToCpp(qry_sec_agent_check_mode));
    return api->ReqQrySecAgentCheckMode(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_sec_agent_trade_info(QrySecAgentTradeInfo qry_sec_agent_trade_info, int32_t request_id) const {
    CThostFtdcQrySecAgentTradeInfoField req(Converter::QrySecAgentTradeInfoToCpp(qry_sec_agent_trade_info));
    return api->ReqQrySecAgentTradeInfo(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_option_instr_trade_cost(QryOptionInstrTradeCost qry_option_instr_trade_cost, int32_t request_id) const {
    CThostFtdcQryOptionInstrTradeCostField req(Converter::QryOptionInstrTradeCostToCpp(qry_option_instr_trade_cost));
    return api->ReqQryOptionInstrTradeCost(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_option_instr_comm_rate(QryOptionInstrCommRate qry_option_instr_comm_rate, int32_t request_id) const {
    CThostFtdcQryOptionInstrCommRateField req(Converter::QryOptionInstrCommRateToCpp(qry_option_instr_comm_rate));
    return api->ReqQryOptionInstrCommRate(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_exec_order(QryExecOrder qry_exec_order, int32_t request_id) const {
    CThostFtdcQryExecOrderField req(Converter::QryExecOrderToCpp(qry_exec_order));
    return api->ReqQryExecOrder(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_for_quote(QryForQuote qry_for_quote, int32_t request_id) const {
    CThostFtdcQryForQuoteField req(Converter::QryForQuoteToCpp(qry_for_quote));
    return api->ReqQryForQuote(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_quote(QryQuote qry_quote, int32_t request_id) const {
    CThostFtdcQryQuoteField req(Converter::QryQuoteToCpp(qry_quote));
    return api->ReqQryQuote(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_option_self_close(QryOptionSelfClose qry_option_self_close, int32_t request_id) const {
    CThostFtdcQryOptionSelfCloseField req(Converter::QryOptionSelfCloseToCpp(qry_option_self_close));
    return api->ReqQryOptionSelfClose(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_invest_unit(QryInvestUnit qry_invest_unit, int32_t request_id) const {
    CThostFtdcQryInvestUnitField req(Converter::QryInvestUnitToCpp(qry_invest_unit));
    return api->ReqQryInvestUnit(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_comb_instrument_guard(QryCombInstrumentGuard qry_comb_instrument_guard, int32_t request_id) const {
    CThostFtdcQryCombInstrumentGuardField req(Converter::QryCombInstrumentGuardToCpp(qry_comb_instrument_guard));
    return api->ReqQryCombInstrumentGuard(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_comb_action(QryCombAction qry_comb_action, int32_t request_id) const {
    CThostFtdcQryCombActionField req(Converter::QryCombActionToCpp(qry_comb_action));
    return api->ReqQryCombAction(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_transfer_serial(QryTransferSerial qry_transfer_serial, int32_t request_id) const {
    CThostFtdcQryTransferSerialField req(Converter::QryTransferSerialToCpp(qry_transfer_serial));
    return api->ReqQryTransferSerial(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_account_register(QryAccountRegister qry_account_register, int32_t request_id) const {
    CThostFtdcQryAccountregisterField req(Converter::QryAccountRegisterToCpp(qry_account_register));
    return api->ReqQryAccountregister(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_contract_bank(QryContractBank qry_contract_bank, int32_t request_id) const {
    CThostFtdcQryContractBankField req(Converter::QryContractBankToCpp(qry_contract_bank));
    return api->ReqQryContractBank(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_parked_order(QryParkedOrder qry_parked_order, int32_t request_id) const {
    CThostFtdcQryParkedOrderField req(Converter::QryParkedOrderToCpp(qry_parked_order));
    return api->ReqQryParkedOrder(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_parked_order_action(QryParkedOrderAction qry_parked_order_action, int32_t request_id) const {
    CThostFtdcQryParkedOrderActionField req(Converter::QryParkedOrderActionToCpp(qry_parked_order_action));
    return api->ReqQryParkedOrderAction(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_trading_notice(QryTradingNotice qry_trading_notice, int32_t request_id) const {
    CThostFtdcQryTradingNoticeField req(Converter::QryTradingNoticeToCpp(qry_trading_notice));
    return api->ReqQryTradingNotice(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_broker_trading_params(QryBrokerTradingParams qry_broker_trading_params, int32_t request_id) const {
    CThostFtdcQryBrokerTradingParamsField req(Converter::QryBrokerTradingParamsToCpp(qry_broker_trading_params));
    return api->ReqQryBrokerTradingParams(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_broker_trading_algos(QryBrokerTradingAlgos qry_broker_trading_algos, int32_t request_id) const {
    CThostFtdcQryBrokerTradingAlgosField req(Converter::QryBrokerTradingAlgosToCpp(qry_broker_trading_algos));
    return api->ReqQryBrokerTradingAlgos(
        &req,
        request_id
    );
}

int32_t TraderApi::req_query_cfmmc_trading_account_token(QueryCFMMCTradingAccountToken query_cfmmc_trading_account_token, int32_t request_id) const {
    CThostFtdcQueryCFMMCTradingAccountTokenField req(Converter::QueryCFMMCTradingAccountTokenToCpp(query_cfmmc_trading_account_token));
    return api->ReqQueryCFMMCTradingAccountToken(
        &req,
        request_id
    );
}

int32_t TraderApi::req_from_bank_to_future_by_future(ReqTransfer req_transfer, int32_t request_id) const {
    CThostFtdcReqTransferField req(Converter::ReqTransferToCpp(req_transfer));
    return api->ReqFromBankToFutureByFuture(
        &req,
        request_id
    );
}

int32_t TraderApi::req_from_future_to_bank_by_future(ReqTransfer req_transfer, int32_t request_id) const {
    CThostFtdcReqTransferField req(Converter::ReqTransferToCpp(req_transfer));
    return api->ReqFromFutureToBankByFuture(
        &req,
        request_id
    );
}

int32_t TraderApi::req_query_bank_account_money_by_future(ReqQueryAccount req_query_account, int32_t request_id) const {
    CThostFtdcReqQueryAccountField req(Converter::ReqQueryAccountToCpp(req_query_account));
    return api->ReqQueryBankAccountMoneyByFuture(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_classified_instrument(QryClassifiedInstrument qry_classified_instrument, int32_t request_id) const {
    CThostFtdcQryClassifiedInstrumentField req(Converter::QryClassifiedInstrumentToCpp(qry_classified_instrument));
    return api->ReqQryClassifiedInstrument(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_comb_promotion_param(QryCombPromotionParam qry_comb_promotion_param, int32_t request_id) const {
    CThostFtdcQryCombPromotionParamField req(Converter::QryCombPromotionParamToCpp(qry_comb_promotion_param));
    return api->ReqQryCombPromotionParam(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_risk_settle_invest_position(QryRiskSettleInvestPosition qry_risk_settle_invest_position, int32_t request_id) const {
    CThostFtdcQryRiskSettleInvstPositionField req(Converter::QryRiskSettleInvestPositionToCpp(qry_risk_settle_invest_position));
    return api->ReqQryRiskSettleInvstPosition(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_risk_settle_product_status(QryRiskSettleProductStatus qry_risk_settle_product_status, int32_t request_id) const {
    CThostFtdcQryRiskSettleProductStatusField req(Converter::QryRiskSettleProductStatusToCpp(qry_risk_settle_product_status));
    return api->ReqQryRiskSettleProductStatus(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_spbm_future_parameter(QrySPBMFutureParameter qry_spbm_future_parameter, int32_t request_id) const {
    CThostFtdcQrySPBMFutureParameterField req(Converter::QrySPBMFutureParameterToCpp(qry_spbm_future_parameter));
    return api->ReqQrySPBMFutureParameter(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_spbm_option_parameter(QrySPBMOptionParameter qry_spbm_option_parameter, int32_t request_id) const {
    CThostFtdcQrySPBMOptionParameterField req(Converter::QrySPBMOptionParameterToCpp(qry_spbm_option_parameter));
    return api->ReqQrySPBMOptionParameter(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_spbm_intra_parameter(QrySPBMIntraParameter qry_spbm_intra_parameter, int32_t request_id) const {
    CThostFtdcQrySPBMIntraParameterField req(Converter::QrySPBMIntraParameterToCpp(qry_spbm_intra_parameter));
    return api->ReqQrySPBMIntraParameter(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_spbm_inter_parameter(QrySPBMInterParameter qry_spbm_inter_parameter, int32_t request_id) const {
    CThostFtdcQrySPBMInterParameterField req(Converter::QrySPBMInterParameterToCpp(qry_spbm_inter_parameter));
    return api->ReqQrySPBMInterParameter(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_spbm_portf_definition(QrySPBMPortfDefinition qry_spbm_portf_definition, int32_t request_id) const {
    CThostFtdcQrySPBMPortfDefinitionField req(Converter::QrySPBMPortfDefinitionToCpp(qry_spbm_portf_definition));
    return api->ReqQrySPBMPortfDefinition(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_spbm_investor_portf_def(QrySPBMInvestorPortfDef qry_spbm_investor_portf_def, int32_t request_id) const {
    CThostFtdcQrySPBMInvestorPortfDefField req(Converter::QrySPBMInvestorPortfDefToCpp(qry_spbm_investor_portf_def));
    return api->ReqQrySPBMInvestorPortfDef(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_investor_portf_margin_ratio(QryInvestorPortfMarginRatio qry_investor_portf_margin_ratio, int32_t request_id) const {
    CThostFtdcQryInvestorPortfMarginRatioField req(Converter::QryInvestorPortfMarginRatioToCpp(qry_investor_portf_margin_ratio));
    return api->ReqQryInvestorPortfMarginRatio(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_investor_prod_spbm_detail(QryInvestorProdSPBMDetail qry_investor_prod_spbm_detail, int32_t request_id) const {
    CThostFtdcQryInvestorProdSPBMDetailField req(Converter::QryInvestorProdSPBMDetailToCpp(qry_investor_prod_spbm_detail));
    return api->ReqQryInvestorProdSPBMDetail(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_investor_commodity_spmm_margin(QryInvestorCommoditySPMMMargin qry_investor_commodity_spmm_margin, int32_t request_id) const {
    CThostFtdcQryInvestorCommoditySPMMMarginField req(Converter::QryInvestorCommoditySPMMMarginToCpp(qry_investor_commodity_spmm_margin));
    return api->ReqQryInvestorCommoditySPMMMargin(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_investor_commodity_group_spmm_margin(QryInvestorCommodityGroupSPMMMargin qry_investor_commodity_group_spmm_margin, int32_t request_id) const {
    CThostFtdcQryInvestorCommodityGroupSPMMMarginField req(Converter::QryInvestorCommodityGroupSPMMMarginToCpp(qry_investor_commodity_group_spmm_margin));
    return api->ReqQryInvestorCommodityGroupSPMMMargin(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_spmm_inst_param(QrySPMMInstParam qry_spmm_inst_param, int32_t request_id) const {
    CThostFtdcQrySPMMInstParamField req(Converter::QrySPMMInstParamToCpp(qry_spmm_inst_param));
    return api->ReqQrySPMMInstParam(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_spmm_product_param(QrySPMMProductParam qry_spmm_product_param, int32_t request_id) const {
    CThostFtdcQrySPMMProductParamField req(Converter::QrySPMMProductParamToCpp(qry_spmm_product_param));
    return api->ReqQrySPMMProductParam(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_spbm_add_on_inter_parameter(QrySPBMAddOnInterParameter qry_spbm_add_on_inter_parameter, int32_t request_id) const {
    CThostFtdcQrySPBMAddOnInterParameterField req(Converter::QrySPBMAddOnInterParameterToCpp(qry_spbm_add_on_inter_parameter));
    return api->ReqQrySPBMAddOnInterParameter(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_rcams_comb_product_info(QryRCAMSCombProductInfo qry_rcams_comb_product_info, int32_t request_id) const {
    CThostFtdcQryRCAMSCombProductInfoField req(Converter::QryRCAMSCombProductInfoToCpp(qry_rcams_comb_product_info));
    return api->ReqQryRCAMSCombProductInfo(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_rcams_instr_parameter(QryRCAMSInstrParameter qry_rcams_instr_parameter, int32_t request_id) const {
    CThostFtdcQryRCAMSInstrParameterField req(Converter::QryRCAMSInstrParameterToCpp(qry_rcams_instr_parameter));
    return api->ReqQryRCAMSInstrParameter(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_rcams_intra_parameter(QryRCAMSIntraParameter qry_rcams_intra_parameter, int32_t request_id) const {
    CThostFtdcQryRCAMSIntraParameterField req(Converter::QryRCAMSIntraParameterToCpp(qry_rcams_intra_parameter));
    return api->ReqQryRCAMSIntraParameter(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_rcams_inter_parameter(QryRCAMSInterParameter qry_rcams_inter_parameter, int32_t request_id) const {
    CThostFtdcQryRCAMSInterParameterField req(Converter::QryRCAMSInterParameterToCpp(qry_rcams_inter_parameter));
    return api->ReqQryRCAMSInterParameter(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_rcams_short_opt_adjust_param(QryRCAMSShortOptAdjustParam qry_rcams_short_opt_adjust_param, int32_t request_id) const {
    CThostFtdcQryRCAMSShortOptAdjustParamField req(Converter::QryRCAMSShortOptAdjustParamToCpp(qry_rcams_short_opt_adjust_param));
    return api->ReqQryRCAMSShortOptAdjustParam(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_rcams_investor_comb_position(QryRCAMSInvestorCombPosition qry_rcams_investor_comb_position, int32_t request_id) const {
    CThostFtdcQryRCAMSInvestorCombPositionField req(Converter::QryRCAMSInvestorCombPositionToCpp(qry_rcams_investor_comb_position));
    return api->ReqQryRCAMSInvestorCombPosition(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_investor_prod_rcams_margin(QryInvestorProdRCAMSMargin qry_investor_prod_rcams_margin, int32_t request_id) const {
    CThostFtdcQryInvestorProdRCAMSMarginField req(Converter::QryInvestorProdRCAMSMarginToCpp(qry_investor_prod_rcams_margin));
    return api->ReqQryInvestorProdRCAMSMargin(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_rule_instr_parameter(QryRULEInstrParameter qry_rule_instr_parameter, int32_t request_id) const {
    CThostFtdcQryRULEInstrParameterField req(Converter::QryRULEInstrParameterToCpp(qry_rule_instr_parameter));
    return api->ReqQryRULEInstrParameter(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_rule_intra_parameter(QryRULEIntraParameter qry_rule_intra_parameter, int32_t request_id) const {
    CThostFtdcQryRULEIntraParameterField req(Converter::QryRULEIntraParameterToCpp(qry_rule_intra_parameter));
    return api->ReqQryRULEIntraParameter(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_rule_inter_parameter(QryRULEInterParameter qry_rule_inter_parameter, int32_t request_id) const {
    CThostFtdcQryRULEInterParameterField req(Converter::QryRULEInterParameterToCpp(qry_rule_inter_parameter));
    return api->ReqQryRULEInterParameter(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_investor_prod_rule_margin(QryInvestorProdRULEMargin qry_investor_prod_rule_margin, int32_t request_id) const {
    CThostFtdcQryInvestorProdRULEMarginField req(Converter::QryInvestorProdRULEMarginToCpp(qry_investor_prod_rule_margin));
    return api->ReqQryInvestorProdRULEMargin(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_investor_portf_setting(QryInvestorPortfSetting qry_investor_portf_setting, int32_t request_id) const {
    CThostFtdcQryInvestorPortfSettingField req(Converter::QryInvestorPortfSettingToCpp(qry_investor_portf_setting));
    return api->ReqQryInvestorPortfSetting(
        &req,
        request_id
    );
}

int32_t TraderApi::req_qry_investor_info_comm_rec(QryInvestorInfoCommRec qry_investor_info_comm_rec, int32_t request_id) const {
#if CTP_RS_LINUX_ONLY_API
    CThostFtdcQryInvestorInfoCommRecField req(Converter::QryInvestorInfoCommRecToCpp(qry_investor_info_comm_rec));
    return api->ReqQryInvestorInfoCommRec(&req, request_id);
#else
    (void)pQryInvestorInfoCommRec; (void)nRequestID;
    return -1;
#endif
}

int32_t TraderApi::req_qry_comb_leg(QryCombLeg qry_comb_leg, int32_t request_id) const {
#if CTP_RS_LINUX_ONLY_API
    CThostFtdcQryCombLegField req(Converter::QryCombLegToCpp(qry_comb_leg));
    return api->ReqQryCombLeg(&req, request_id);
#else
    (void)pQryCombLeg; (void)nRequestID;
    return -1;
#endif
}

int32_t TraderApi::req_offset_setting(InputOffsetSetting input_offset_setting, int32_t request_id) const {
#if CTP_RS_LINUX_ONLY_API
    CThostFtdcInputOffsetSettingField req(Converter::InputOffsetSettingToCpp(input_offset_setting));
    return api->ReqOffsetSetting(&req, request_id);
#else
    (void)pInputOffsetSetting; (void)nRequestID;
    return -1;
#endif
}

int32_t TraderApi::req_cancel_offset_setting(InputOffsetSetting input_offset_setting, int32_t request_id) const {
#if CTP_RS_LINUX_ONLY_API
    CThostFtdcInputOffsetSettingField req(Converter::InputOffsetSettingToCpp(input_offset_setting));
    return api->ReqCancelOffsetSetting(&req, request_id);
#else
    (void)pInputOffsetSetting; (void)nRequestID;
    return -1;
#endif
}

int32_t TraderApi::req_qry_offset_setting(QryOffsetSetting qry_offset_setting, int32_t request_id) const {
#if CTP_RS_LINUX_ONLY_API
    CThostFtdcQryOffsetSettingField req(Converter::QryOffsetSettingToCpp(qry_offset_setting));
    return api->ReqQryOffsetSetting(&req, request_id);
#else
    (void)pQryOffsetSetting; (void)nRequestID;
    return -1;
#endif
}
