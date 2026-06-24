#include "ctp-rs/wrapper/include/CTraderSpi.h"
#include "ctp-rs/wrapper/include/Converter.h"

#if defined(__APPLE__) && defined(CTP_RS_DARWIN_TRADER_DLOPEN)
#include <cstring>
// darwin 6.7.7 SDK struct size bridging
namespace {
template <typename T>
inline T widen_from_darwin(T* src, std::size_t darwin_size) {
    T widened{};
    if (src != nullptr) {
        std::memcpy(&widened, src, darwin_size);
    }
    return widened;
}
constexpr std::size_t kDarwinRspUserLoginSize = 204;
constexpr std::size_t kDarwinTradingAccountSize = 400;
} // namespace
#endif

CTraderSpi::CTraderSpi(rust::Box<TraderSpi> gateway) : gateway(std::move(gateway)) { }

void CTraderSpi::OnFrontConnected() {
    this->gateway->on_front_connected(
    );
}

void CTraderSpi::OnFrontDisconnected(int32_t nReason) {
    this->gateway->on_front_disconnected(
        nReason
    );
}

void CTraderSpi::OnHeartBeatWarning(int32_t nTimeLapse) {
    this->gateway->on_heart_beat_warning(
        nTimeLapse
    );
}

void CTraderSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_authenticate(
        Converter::CThostFtdcRspAuthenticateFieldToRust(pRspAuthenticateField),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
#if defined(__APPLE__) && defined(CTP_RS_DARWIN_TRADER_DLOPEN)
    auto widened = widen_from_darwin(pRspUserLogin, kDarwinRspUserLoginSize);
    CThostFtdcRspUserLoginField* p = (pRspUserLogin != nullptr) ? &widened : nullptr;
#else
    CThostFtdcRspUserLoginField* p = pRspUserLogin;
#endif
    this->gateway->on_rsp_user_login(
        Converter::CThostFtdcRspUserLoginFieldToRust(p),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_user_logout(
        Converter::CThostFtdcUserLogoutFieldToRust(pUserLogout),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField* pUserPasswordUpdate, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_user_password_update(
        Converter::CThostFtdcUserPasswordUpdateFieldToRust(pUserPasswordUpdate),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField* pTradingAccountPasswordUpdate, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_trading_account_password_update(
        Converter::CThostFtdcTradingAccountPasswordUpdateFieldToRust(pTradingAccountPasswordUpdate),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspUserAuthMethod(CThostFtdcRspUserAuthMethodField* pRspUserAuthMethod, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_user_auth_method(
        Converter::CThostFtdcRspUserAuthMethodFieldToRust(pRspUserAuthMethod),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspGenUserCaptcha(CThostFtdcRspGenUserCaptchaField* pRspGenUserCaptcha, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_gen_user_captcha(
        Converter::CThostFtdcRspGenUserCaptchaFieldToRust(pRspGenUserCaptcha),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspGenUserText(CThostFtdcRspGenUserTextField* pRspGenUserText, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_gen_user_text(
        Converter::CThostFtdcRspGenUserTextFieldToRust(pRspGenUserText),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_order_insert(
        Converter::CThostFtdcInputOrderFieldToRust(pInputOrder),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspParkedOrderInsert(CThostFtdcParkedOrderField* pParkedOrder, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_parked_order_insert(
        Converter::CThostFtdcParkedOrderFieldToRust(pParkedOrder),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField* pParkedOrderAction, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_parked_order_action(
        Converter::CThostFtdcParkedOrderActionFieldToRust(pParkedOrderAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField* pInputOrderAction, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_order_action(
        Converter::CThostFtdcInputOrderActionFieldToRust(pInputOrderAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryMaxOrderVolume(CThostFtdcQryMaxOrderVolumeField* pQryMaxOrderVolume, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_max_order_volume(
        Converter::CThostFtdcQryMaxOrderVolumeFieldToRust(pQryMaxOrderVolume),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_settlement_info_confirm(
        Converter::CThostFtdcSettlementInfoConfirmFieldToRust(pSettlementInfoConfirm),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField* pRemoveParkedOrder, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_remove_parked_order(
        Converter::CThostFtdcRemoveParkedOrderFieldToRust(pRemoveParkedOrder),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField* pRemoveParkedOrderAction, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_remove_parked_order_action(
        Converter::CThostFtdcRemoveParkedOrderActionFieldToRust(pRemoveParkedOrderAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspExecOrderInsert(CThostFtdcInputExecOrderField* pInputExecOrder, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_exec_order_insert(
        Converter::CThostFtdcInputExecOrderFieldToRust(pInputExecOrder),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField* pInputExecOrderAction, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_exec_order_action(
        Converter::CThostFtdcInputExecOrderActionFieldToRust(pInputExecOrderAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspForQuoteInsert(CThostFtdcInputForQuoteField* pInputForQuote, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_for_quote_insert(
        Converter::CThostFtdcInputForQuoteFieldToRust(pInputForQuote),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQuoteInsert(CThostFtdcInputQuoteField* pInputQuote, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_quote_insert(
        Converter::CThostFtdcInputQuoteFieldToRust(pInputQuote),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQuoteAction(CThostFtdcInputQuoteActionField* pInputQuoteAction, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_quote_action(
        Converter::CThostFtdcInputQuoteActionFieldToRust(pInputQuoteAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField* pInputBatchOrderAction, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_batch_order_action(
        Converter::CThostFtdcInputBatchOrderActionFieldToRust(pInputBatchOrderAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField* pInputOptionSelfClose, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_option_self_close_insert(
        Converter::CThostFtdcInputOptionSelfCloseFieldToRust(pInputOptionSelfClose),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspOptionSelfCloseAction(CThostFtdcInputOptionSelfCloseActionField* pInputOptionSelfCloseAction, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_option_self_close_action(
        Converter::CThostFtdcInputOptionSelfCloseActionFieldToRust(pInputOptionSelfCloseAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspCombActionInsert(CThostFtdcInputCombActionField* pInputCombAction, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_comb_action_insert(
        Converter::CThostFtdcInputCombActionFieldToRust(pInputCombAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryOrder(CThostFtdcOrderField* pOrder, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_order(
        Converter::CThostFtdcOrderFieldToRust(pOrder),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryTrade(CThostFtdcTradeField* pTrade, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_trade(
        Converter::CThostFtdcTradeFieldToRust(pTrade),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField* pInvestorPosition, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_investor_position(
        Converter::CThostFtdcInvestorPositionFieldToRust(pInvestorPosition),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField* pTradingAccount, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
#if defined(__APPLE__) && defined(CTP_RS_DARWIN_TRADER_DLOPEN)
    auto widened = widen_from_darwin(pTradingAccount, kDarwinTradingAccountSize);
    CThostFtdcTradingAccountField* p = (pTradingAccount != nullptr) ? &widened : nullptr;
#else
    CThostFtdcTradingAccountField* p = pTradingAccount;
#endif
    this->gateway->on_rsp_qry_trading_account(
        Converter::CThostFtdcTradingAccountFieldToRust(p),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInvestor(CThostFtdcInvestorField* pInvestor, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_investor(
        Converter::CThostFtdcInvestorFieldToRust(pInvestor),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryTradingCode(CThostFtdcTradingCodeField* pTradingCode, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_trading_code(
        Converter::CThostFtdcTradingCodeFieldToRust(pTradingCode),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField* pInstrumentMarginRate, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_instrument_margin_rate(
        Converter::CThostFtdcInstrumentMarginRateFieldToRust(pInstrumentMarginRate),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField* pInstrumentCommissionRate, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_instrument_commission_rate(
        Converter::CThostFtdcInstrumentCommissionRateFieldToRust(pInstrumentCommissionRate),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

#if CTP_RS_HAS_LINUX_ONLY_SPI
void CTraderSpi::OnRspQryUserSession(CThostFtdcUserSessionField* pUserSession, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_user_session(
        Converter::CThostFtdcUserSessionFieldToRust(pUserSession),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}
#endif

void CTraderSpi::OnRspQryExchange(CThostFtdcExchangeField* pExchange, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_exchange(
        Converter::CThostFtdcExchangeFieldToRust(pExchange),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryProduct(CThostFtdcProductField* pProduct, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_product(
        Converter::CThostFtdcProductFieldToRust(pProduct),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_instrument(
        Converter::CThostFtdcInstrumentFieldToRust(pInstrument),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_depth_market_data(
        Converter::CThostFtdcDepthMarketDataFieldToRust(pDepthMarketData),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryTraderOffer(CThostFtdcTraderOfferField* pTraderOffer, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_trader_offer(
        Converter::CThostFtdcTraderOfferFieldToRust(pTraderOffer),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField* pSettlementInfo, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_settlement_info(
        Converter::CThostFtdcSettlementInfoFieldToRust(pSettlementInfo),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryTransferBank(CThostFtdcTransferBankField* pTransferBank, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_transfer_bank(
        Converter::CThostFtdcTransferBankFieldToRust(pTransferBank),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField* pInvestorPositionDetail, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_investor_position_detail(
        Converter::CThostFtdcInvestorPositionDetailFieldToRust(pInvestorPositionDetail),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryNotice(CThostFtdcNoticeField* pNotice, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_notice(
        Converter::CThostFtdcNoticeFieldToRust(pNotice),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_settlement_info_confirm(
        Converter::CThostFtdcSettlementInfoConfirmFieldToRust(pSettlementInfoConfirm),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField* pInvestorPositionCombineDetail, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_investor_position_combine_detail(
        Converter::CThostFtdcInvestorPositionCombineDetailFieldToRust(pInvestorPositionCombineDetail),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField* pCFMMCTradingAccountKey, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_cfmmc_trading_account_key(
        Converter::CThostFtdcCFMMCTradingAccountKeyFieldToRust(pCFMMCTradingAccountKey),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField* pEWarrantOffset, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_ewarrant_offset(
        Converter::CThostFtdcEWarrantOffsetFieldToRust(pEWarrantOffset),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField* pInvestorProductGroupMargin, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_investor_product_group_margin(
        Converter::CThostFtdcInvestorProductGroupMarginFieldToRust(pInvestorProductGroupMargin),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField* pExchangeMarginRate, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_exchange_margin_rate(
        Converter::CThostFtdcExchangeMarginRateFieldToRust(pExchangeMarginRate),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField* pExchangeMarginRateAdjust, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_exchange_margin_rate_adjust(
        Converter::CThostFtdcExchangeMarginRateAdjustFieldToRust(pExchangeMarginRateAdjust),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryExchangeRate(CThostFtdcExchangeRateField* pExchangeRate, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_exchange_rate(
        Converter::CThostFtdcExchangeRateFieldToRust(pExchangeRate),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField* pSecAgentACIDMap, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_sec_agent_acid_map(
        Converter::CThostFtdcSecAgentACIDMapFieldToRust(pSecAgentACIDMap),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryProductExchRate(CThostFtdcProductExchRateField* pProductExchRate, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_product_exch_rate(
        Converter::CThostFtdcProductExchRateFieldToRust(pProductExchRate),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryProductGroup(CThostFtdcProductGroupField* pProductGroup, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_product_group(
        Converter::CThostFtdcProductGroupFieldToRust(pProductGroup),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField* pMMInstrumentCommissionRate, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_mm_instrument_commission_rate(
        Converter::CThostFtdcMMInstrumentCommissionRateFieldToRust(pMMInstrumentCommissionRate),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField* pMMOptionInstrCommRate, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_mm_option_instr_comm_rate(
        Converter::CThostFtdcMMOptionInstrCommRateFieldToRust(pMMOptionInstrCommRate),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField* pInstrumentOrderCommRate, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_instrument_order_comm_rate(
        Converter::CThostFtdcInstrumentOrderCommRateFieldToRust(pInstrumentOrderCommRate),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySecAgentTradingAccount(CThostFtdcTradingAccountField* pTradingAccount, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
#if defined(__APPLE__) && defined(CTP_RS_DARWIN_TRADER_DLOPEN)
    auto widened = widen_from_darwin(pTradingAccount, kDarwinTradingAccountSize);
    CThostFtdcTradingAccountField* p = (pTradingAccount != nullptr) ? &widened : nullptr;
#else
    CThostFtdcTradingAccountField* p = pTradingAccount;
#endif
    this->gateway->on_rsp_qry_sec_agent_trading_account(
        Converter::CThostFtdcTradingAccountFieldToRust(p),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySecAgentCheckMode(CThostFtdcSecAgentCheckModeField* pSecAgentCheckMode, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_sec_agent_check_mode(
        Converter::CThostFtdcSecAgentCheckModeFieldToRust(pSecAgentCheckMode),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySecAgentTradeInfo(CThostFtdcSecAgentTradeInfoField* pSecAgentTradeInfo, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_sec_agent_trade_info(
        Converter::CThostFtdcSecAgentTradeInfoFieldToRust(pSecAgentTradeInfo),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField* pOptionInstrTradeCost, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_option_instr_trade_cost(
        Converter::CThostFtdcOptionInstrTradeCostFieldToRust(pOptionInstrTradeCost),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField* pOptionInstrCommRate, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_option_instr_comm_rate(
        Converter::CThostFtdcOptionInstrCommRateFieldToRust(pOptionInstrCommRate),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryExecOrder(CThostFtdcExecOrderField* pExecOrder, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_exec_order(
        Converter::CThostFtdcExecOrderFieldToRust(pExecOrder),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryForQuote(CThostFtdcForQuoteField* pForQuote, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_for_quote(
        Converter::CThostFtdcForQuoteFieldToRust(pForQuote),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryQuote(CThostFtdcQuoteField* pQuote, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_quote(
        Converter::CThostFtdcQuoteFieldToRust(pQuote),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryOptionSelfClose(CThostFtdcOptionSelfCloseField* pOptionSelfClose, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_option_self_close(
        Converter::CThostFtdcOptionSelfCloseFieldToRust(pOptionSelfClose),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInvestUnit(CThostFtdcInvestUnitField* pInvestUnit, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_invest_unit(
        Converter::CThostFtdcInvestUnitFieldToRust(pInvestUnit),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField* pCombInstrumentGuard, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_comb_instrument_guard(
        Converter::CThostFtdcCombInstrumentGuardFieldToRust(pCombInstrumentGuard),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryCombAction(CThostFtdcCombActionField* pCombAction, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_comb_action(
        Converter::CThostFtdcCombActionFieldToRust(pCombAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryTransferSerial(CThostFtdcTransferSerialField* pTransferSerial, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_transfer_serial(
        Converter::CThostFtdcTransferSerialFieldToRust(pTransferSerial),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryAccountregister(CThostFtdcAccountregisterField* pAccountregister, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_account_register(
        Converter::CThostFtdcAccountregisterFieldToRust(pAccountregister),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_error(
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRtnOrder(CThostFtdcOrderField* pOrder) {
    this->gateway->on_rtn_order(
        Converter::CThostFtdcOrderFieldToRust(pOrder)
    );
}

void CTraderSpi::OnRtnTrade(CThostFtdcTradeField* pTrade) {
    this->gateway->on_rtn_trade(
        Converter::CThostFtdcTradeFieldToRust(pTrade)
    );
}

void CTraderSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_order_insert(
        Converter::CThostFtdcInputOrderFieldToRust(pInputOrder),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField* pOrderAction, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_order_action(
        Converter::CThostFtdcOrderActionFieldToRust(pOrderAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField* pInstrumentStatus) {
    this->gateway->on_rtn_instrument_status(
        Converter::CThostFtdcInstrumentStatusFieldToRust(pInstrumentStatus)
    );
}

void CTraderSpi::OnRtnBulletin(CThostFtdcBulletinField* pBulletin) {
    this->gateway->on_rtn_bulletin(
        Converter::CThostFtdcBulletinFieldToRust(pBulletin)
    );
}

void CTraderSpi::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField* pTradingNoticeInfo) {
    this->gateway->on_rtn_trading_notice(
        Converter::CThostFtdcTradingNoticeInfoFieldToRust(pTradingNoticeInfo)
    );
}

void CTraderSpi::OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField* pErrorConditionalOrder) {
    this->gateway->on_rtn_error_conditional_order(
        Converter::CThostFtdcErrorConditionalOrderFieldToRust(pErrorConditionalOrder)
    );
}

void CTraderSpi::OnRtnExecOrder(CThostFtdcExecOrderField* pExecOrder) {
    this->gateway->on_rtn_exec_order(
        Converter::CThostFtdcExecOrderFieldToRust(pExecOrder)
    );
}

void CTraderSpi::OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField* pInputExecOrder, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_exec_order_insert(
        Converter::CThostFtdcInputExecOrderFieldToRust(pInputExecOrder),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField* pExecOrderAction, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_exec_order_action(
        Converter::CThostFtdcExecOrderActionFieldToRust(pExecOrderAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField* pInputForQuote, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_for_quote_insert(
        Converter::CThostFtdcInputForQuoteFieldToRust(pInputForQuote),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnRtnQuote(CThostFtdcQuoteField* pQuote) {
    this->gateway->on_rtn_quote(
        Converter::CThostFtdcQuoteFieldToRust(pQuote)
    );
}

void CTraderSpi::OnErrRtnQuoteInsert(CThostFtdcInputQuoteField* pInputQuote, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_quote_insert(
        Converter::CThostFtdcInputQuoteFieldToRust(pInputQuote),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnErrRtnQuoteAction(CThostFtdcQuoteActionField* pQuoteAction, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_quote_action(
        Converter::CThostFtdcQuoteActionFieldToRust(pQuoteAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField* pForQuoteRsp) {
    this->gateway->on_rtn_for_quote_rsp(
        Converter::CThostFtdcForQuoteRspFieldToRust(pForQuoteRsp)
    );
}

void CTraderSpi::OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField* pCFMMCTradingAccountToken) {
    this->gateway->on_rtn_cfmmc_trading_account_token(
        Converter::CThostFtdcCFMMCTradingAccountTokenFieldToRust(pCFMMCTradingAccountToken)
    );
}

void CTraderSpi::OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField* pBatchOrderAction, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_batch_order_action(
        Converter::CThostFtdcBatchOrderActionFieldToRust(pBatchOrderAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnRtnOptionSelfClose(CThostFtdcOptionSelfCloseField* pOptionSelfClose) {
    this->gateway->on_rtn_option_self_close(
        Converter::CThostFtdcOptionSelfCloseFieldToRust(pOptionSelfClose)
    );
}

void CTraderSpi::OnErrRtnOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField* pInputOptionSelfClose, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_option_self_close_insert(
        Converter::CThostFtdcInputOptionSelfCloseFieldToRust(pInputOptionSelfClose),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnErrRtnOptionSelfCloseAction(CThostFtdcOptionSelfCloseActionField* pOptionSelfCloseAction, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_option_self_close_action(
        Converter::CThostFtdcOptionSelfCloseActionFieldToRust(pOptionSelfCloseAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnRtnCombAction(CThostFtdcCombActionField* pCombAction) {
    this->gateway->on_rtn_comb_action(
        Converter::CThostFtdcCombActionFieldToRust(pCombAction)
    );
}

void CTraderSpi::OnErrRtnCombActionInsert(CThostFtdcInputCombActionField* pInputCombAction, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_comb_action_insert(
        Converter::CThostFtdcInputCombActionFieldToRust(pInputCombAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnRspQryContractBank(CThostFtdcContractBankField* pContractBank, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_contract_bank(
        Converter::CThostFtdcContractBankFieldToRust(pContractBank),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryParkedOrder(CThostFtdcParkedOrderField* pParkedOrder, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_parked_order(
        Converter::CThostFtdcParkedOrderFieldToRust(pParkedOrder),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField* pParkedOrderAction, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_parked_order_action(
        Converter::CThostFtdcParkedOrderActionFieldToRust(pParkedOrderAction),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryTradingNotice(CThostFtdcTradingNoticeField* pTradingNotice, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_trading_notice(
        Converter::CThostFtdcTradingNoticeFieldToRust(pTradingNotice),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField* pBrokerTradingParams, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_broker_trading_params(
        Converter::CThostFtdcBrokerTradingParamsFieldToRust(pBrokerTradingParams),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField* pBrokerTradingAlgos, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_broker_trading_algos(
        Converter::CThostFtdcBrokerTradingAlgosFieldToRust(pBrokerTradingAlgos),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField* pQueryCFMMCTradingAccountToken, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_query_cfmmc_trading_account_token(
        Converter::CThostFtdcQueryCFMMCTradingAccountTokenFieldToRust(pQueryCFMMCTradingAccountToken),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField* pRspTransfer) {
    this->gateway->on_rtn_from_bank_to_future_by_bank(
        Converter::CThostFtdcRspTransferFieldToRust(pRspTransfer)
    );
}

void CTraderSpi::OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField* pRspTransfer) {
    this->gateway->on_rtn_from_future_to_bank_by_bank(
        Converter::CThostFtdcRspTransferFieldToRust(pRspTransfer)
    );
}

void CTraderSpi::OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField* pRspRepeal) {
    this->gateway->on_rtn_repeal_from_bank_to_future_by_bank(
        Converter::CThostFtdcRspRepealFieldToRust(pRspRepeal)
    );
}

void CTraderSpi::OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField* pRspRepeal) {
    this->gateway->on_rtn_repeal_from_future_to_bank_by_bank(
        Converter::CThostFtdcRspRepealFieldToRust(pRspRepeal)
    );
}

void CTraderSpi::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField* pRspTransfer) {
    this->gateway->on_rtn_from_bank_to_future_by_future(
        Converter::CThostFtdcRspTransferFieldToRust(pRspTransfer)
    );
}

void CTraderSpi::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField* pRspTransfer) {
    this->gateway->on_rtn_from_future_to_bank_by_future(
        Converter::CThostFtdcRspTransferFieldToRust(pRspTransfer)
    );
}

void CTraderSpi::OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField* pRspRepeal) {
    this->gateway->on_rtn_repeal_from_bank_to_future_by_future_manual(
        Converter::CThostFtdcRspRepealFieldToRust(pRspRepeal)
    );
}

void CTraderSpi::OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField* pRspRepeal) {
    this->gateway->on_rtn_repeal_from_future_to_bank_by_future_manual(
        Converter::CThostFtdcRspRepealFieldToRust(pRspRepeal)
    );
}

void CTraderSpi::OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField* pNotifyQueryAccount) {
    this->gateway->on_rtn_query_bank_balance_by_future(
        Converter::CThostFtdcNotifyQueryAccountFieldToRust(pNotifyQueryAccount)
    );
}

void CTraderSpi::OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField* pReqTransfer, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_bank_to_future_by_future(
        Converter::CThostFtdcReqTransferFieldToRust(pReqTransfer),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField* pReqTransfer, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_future_to_bank_by_future(
        Converter::CThostFtdcReqTransferFieldToRust(pReqTransfer),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField* pReqRepeal, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_repeal_bank_to_future_by_future_manual(
        Converter::CThostFtdcReqRepealFieldToRust(pReqRepeal),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField* pReqRepeal, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_repeal_future_to_bank_by_future_manual(
        Converter::CThostFtdcReqRepealFieldToRust(pReqRepeal),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField* pReqQueryAccount, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_query_bank_balance_by_future(
        Converter::CThostFtdcReqQueryAccountFieldToRust(pReqQueryAccount),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}

void CTraderSpi::OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField* pRspRepeal) {
    this->gateway->on_rtn_repeal_from_bank_to_future_by_future(
        Converter::CThostFtdcRspRepealFieldToRust(pRspRepeal)
    );
}

void CTraderSpi::OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField* pRspRepeal) {
    this->gateway->on_rtn_repeal_from_future_to_bank_by_future(
        Converter::CThostFtdcRspRepealFieldToRust(pRspRepeal)
    );
}

void CTraderSpi::OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField* pReqTransfer, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_from_bank_to_future_by_future(
        Converter::CThostFtdcReqTransferFieldToRust(pReqTransfer),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField* pReqTransfer, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_from_future_to_bank_by_future(
        Converter::CThostFtdcReqTransferFieldToRust(pReqTransfer),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField* pReqQueryAccount, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_query_bank_account_money_by_future(
        Converter::CThostFtdcReqQueryAccountFieldToRust(pReqQueryAccount),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRtnOpenAccountByBank(CThostFtdcOpenAccountField* pOpenAccount) {
    this->gateway->on_rtn_open_account_by_bank(
        Converter::CThostFtdcOpenAccountFieldToRust(pOpenAccount)
    );
}

void CTraderSpi::OnRtnCancelAccountByBank(CThostFtdcCancelAccountField* pCancelAccount) {
    this->gateway->on_rtn_cancel_account_by_bank(
        Converter::CThostFtdcCancelAccountFieldToRust(pCancelAccount)
    );
}

void CTraderSpi::OnRtnChangeAccountByBank(CThostFtdcChangeAccountField* pChangeAccount) {
    this->gateway->on_rtn_change_account_by_bank(
        Converter::CThostFtdcChangeAccountFieldToRust(pChangeAccount)
    );
}

void CTraderSpi::OnRspQryClassifiedInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_classified_instrument(
        Converter::CThostFtdcInstrumentFieldToRust(pInstrument),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryCombPromotionParam(CThostFtdcCombPromotionParamField* pCombPromotionParam, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_comb_promotion_param(
        Converter::CThostFtdcCombPromotionParamFieldToRust(pCombPromotionParam),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryRiskSettleInvstPosition(CThostFtdcRiskSettleInvstPositionField* pRiskSettleInvstPosition, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_risk_settle_invest_position(
        Converter::CThostFtdcRiskSettleInvstPositionFieldToRust(pRiskSettleInvstPosition),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryRiskSettleProductStatus(CThostFtdcRiskSettleProductStatusField* pRiskSettleProductStatus, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_risk_settle_product_status(
        Converter::CThostFtdcRiskSettleProductStatusFieldToRust(pRiskSettleProductStatus),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySPBMFutureParameter(CThostFtdcSPBMFutureParameterField* pSPBMFutureParameter, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_spbm_future_parameter(
        Converter::CThostFtdcSPBMFutureParameterFieldToRust(pSPBMFutureParameter),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySPBMOptionParameter(CThostFtdcSPBMOptionParameterField* pSPBMOptionParameter, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_spbm_option_parameter(
        Converter::CThostFtdcSPBMOptionParameterFieldToRust(pSPBMOptionParameter),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySPBMIntraParameter(CThostFtdcSPBMIntraParameterField* pSPBMIntraParameter, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_spbm_intra_parameter(
        Converter::CThostFtdcSPBMIntraParameterFieldToRust(pSPBMIntraParameter),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySPBMInterParameter(CThostFtdcSPBMInterParameterField* pSPBMInterParameter, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_spbm_inter_parameter(
        Converter::CThostFtdcSPBMInterParameterFieldToRust(pSPBMInterParameter),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySPBMPortfDefinition(CThostFtdcSPBMPortfDefinitionField* pSPBMPortfDefinition, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_spbm_portf_definition(
        Converter::CThostFtdcSPBMPortfDefinitionFieldToRust(pSPBMPortfDefinition),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySPBMInvestorPortfDef(CThostFtdcSPBMInvestorPortfDefField* pSPBMInvestorPortfDef, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_spbm_investor_portf_def(
        Converter::CThostFtdcSPBMInvestorPortfDefFieldToRust(pSPBMInvestorPortfDef),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInvestorPortfMarginRatio(CThostFtdcInvestorPortfMarginRatioField* pInvestorPortfMarginRatio, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_investor_portf_margin_ratio(
        Converter::CThostFtdcInvestorPortfMarginRatioFieldToRust(pInvestorPortfMarginRatio),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInvestorProdSPBMDetail(CThostFtdcInvestorProdSPBMDetailField* pInvestorProdSPBMDetail, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_investor_prod_spbm_detail(
        Converter::CThostFtdcInvestorProdSPBMDetailFieldToRust(pInvestorProdSPBMDetail),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInvestorCommoditySPMMMargin(CThostFtdcInvestorCommoditySPMMMarginField* pInvestorCommoditySPMMMargin, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_investor_commodity_spmm_margin(
        Converter::CThostFtdcInvestorCommoditySPMMMarginFieldToRust(pInvestorCommoditySPMMMargin),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInvestorCommodityGroupSPMMMargin(CThostFtdcInvestorCommodityGroupSPMMMarginField* pInvestorCommodityGroupSPMMMargin, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_investor_commodity_group_spmm_margin(
        Converter::CThostFtdcInvestorCommodityGroupSPMMMarginFieldToRust(pInvestorCommodityGroupSPMMMargin),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySPMMInstParam(CThostFtdcSPMMInstParamField* pSPMMInstParam, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_spmm_inst_param(
        Converter::CThostFtdcSPMMInstParamFieldToRust(pSPMMInstParam),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySPMMProductParam(CThostFtdcSPMMProductParamField* pSPMMProductParam, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_spmm_product_param(
        Converter::CThostFtdcSPMMProductParamFieldToRust(pSPMMProductParam),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQrySPBMAddOnInterParameter(CThostFtdcSPBMAddOnInterParameterField* pSPBMAddOnInterParameter, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_spbm_add_on_inter_parameter(
        Converter::CThostFtdcSPBMAddOnInterParameterFieldToRust(pSPBMAddOnInterParameter),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryRCAMSCombProductInfo(CThostFtdcRCAMSCombProductInfoField* pRCAMSCombProductInfo, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_rcams_comb_product_info(
        Converter::CThostFtdcRCAMSCombProductInfoFieldToRust(pRCAMSCombProductInfo),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryRCAMSInstrParameter(CThostFtdcRCAMSInstrParameterField* pRCAMSInstrParameter, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_rcams_instr_parameter(
        Converter::CThostFtdcRCAMSInstrParameterFieldToRust(pRCAMSInstrParameter),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryRCAMSIntraParameter(CThostFtdcRCAMSIntraParameterField* pRCAMSIntraParameter, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_rcams_intra_parameter(
        Converter::CThostFtdcRCAMSIntraParameterFieldToRust(pRCAMSIntraParameter),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryRCAMSInterParameter(CThostFtdcRCAMSInterParameterField* pRCAMSInterParameter, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_rcams_inter_parameter(
        Converter::CThostFtdcRCAMSInterParameterFieldToRust(pRCAMSInterParameter),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryRCAMSShortOptAdjustParam(CThostFtdcRCAMSShortOptAdjustParamField* pRCAMSShortOptAdjustParam, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_rcams_short_opt_adjust_param(
        Converter::CThostFtdcRCAMSShortOptAdjustParamFieldToRust(pRCAMSShortOptAdjustParam),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryRCAMSInvestorCombPosition(CThostFtdcRCAMSInvestorCombPositionField* pRCAMSInvestorCombPosition, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_rcams_investor_comb_position(
        Converter::CThostFtdcRCAMSInvestorCombPositionFieldToRust(pRCAMSInvestorCombPosition),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInvestorProdRCAMSMargin(CThostFtdcInvestorProdRCAMSMarginField* pInvestorProdRCAMSMargin, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_investor_prod_rcams_margin(
        Converter::CThostFtdcInvestorProdRCAMSMarginFieldToRust(pInvestorProdRCAMSMargin),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryRULEInstrParameter(CThostFtdcRULEInstrParameterField* pRULEInstrParameter, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_rule_instr_parameter(
        Converter::CThostFtdcRULEInstrParameterFieldToRust(pRULEInstrParameter),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryRULEIntraParameter(CThostFtdcRULEIntraParameterField* pRULEIntraParameter, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_rule_intra_parameter(
        Converter::CThostFtdcRULEIntraParameterFieldToRust(pRULEIntraParameter),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryRULEInterParameter(CThostFtdcRULEInterParameterField* pRULEInterParameter, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_rule_inter_parameter(
        Converter::CThostFtdcRULEInterParameterFieldToRust(pRULEInterParameter),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInvestorProdRULEMargin(CThostFtdcInvestorProdRULEMarginField* pInvestorProdRULEMargin, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_investor_prod_rule_margin(
        Converter::CThostFtdcInvestorProdRULEMarginFieldToRust(pInvestorProdRULEMargin),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

void CTraderSpi::OnRspQryInvestorPortfSetting(CThostFtdcInvestorPortfSettingField* pInvestorPortfSetting, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_investor_portf_setting(
        Converter::CThostFtdcInvestorPortfSettingFieldToRust(pInvestorPortfSetting),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}

#if CTP_RS_HAS_LINUX_ONLY_SPI
void CTraderSpi::OnRspQryInvestorInfoCommRec(CThostFtdcInvestorInfoCommRecField* pInvestorInfoCommRec, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_investor_info_comm_rec(
        Converter::CThostFtdcInvestorInfoCommRecFieldToRust(pInvestorInfoCommRec),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}
#endif

#if CTP_RS_HAS_LINUX_ONLY_SPI
void CTraderSpi::OnRspQryCombLeg(CThostFtdcCombLegField* pCombLeg, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_comb_leg(
        Converter::CThostFtdcCombLegFieldToRust(pCombLeg),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}
#endif

#if CTP_RS_HAS_LINUX_ONLY_SPI
void CTraderSpi::OnRspOffsetSetting(CThostFtdcInputOffsetSettingField* pInputOffsetSetting, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_offset_setting(
        Converter::CThostFtdcInputOffsetSettingFieldToRust(pInputOffsetSetting),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}
#endif

#if CTP_RS_HAS_LINUX_ONLY_SPI
void CTraderSpi::OnRspCancelOffsetSetting(CThostFtdcInputOffsetSettingField* pInputOffsetSetting, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_cancel_offset_setting(
        Converter::CThostFtdcInputOffsetSettingFieldToRust(pInputOffsetSetting),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}
#endif

#if CTP_RS_HAS_LINUX_ONLY_SPI
void CTraderSpi::OnRtnOffsetSetting(CThostFtdcOffsetSettingField* pOffsetSetting) {
    this->gateway->on_rtn_offset_setting(
        Converter::CThostFtdcOffsetSettingFieldToRust(pOffsetSetting)
    );
}
#endif

#if CTP_RS_HAS_LINUX_ONLY_SPI
void CTraderSpi::OnErrRtnOffsetSetting(CThostFtdcInputOffsetSettingField* pInputOffsetSetting, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_offset_setting(
        Converter::CThostFtdcInputOffsetSettingFieldToRust(pInputOffsetSetting),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}
#endif

#if CTP_RS_HAS_LINUX_ONLY_SPI
void CTraderSpi::OnErrRtnCancelOffsetSetting(CThostFtdcCancelOffsetSettingField* pCancelOffsetSetting, CThostFtdcRspInfoField* pRspInfo) {
    this->gateway->on_err_rtn_cancel_offset_setting(
        Converter::CThostFtdcCancelOffsetSettingFieldToRust(pCancelOffsetSetting),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo)
    );
}
#endif

#if CTP_RS_HAS_LINUX_ONLY_SPI
void CTraderSpi::OnRspQryOffsetSetting(CThostFtdcOffsetSettingField* pOffsetSetting, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool is_last) {
    this->gateway->on_rsp_qry_offset_setting(
        Converter::CThostFtdcOffsetSettingFieldToRust(pOffsetSetting),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        is_last
    );
}
#endif
