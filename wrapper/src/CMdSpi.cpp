#include "ctp-rs/wrapper/include/CMdSpi.h"
#include "ctp-rs/wrapper/include/Converter.h"

#if defined(__APPLE__)
#include <cstring>
namespace {
constexpr std::size_t kDarwinRspUserLoginSize = 204;
} // namespace
#endif

CMdSpi::CMdSpi(rust::Box<MdSpi> gateway) : gateway(std::move(gateway)) { }

void CMdSpi::OnFrontConnected() {
    this->gateway->on_front_connected(
    );
}

void CMdSpi::OnFrontDisconnected(int32_t nReason) {
    this->gateway->on_front_disconnected(
        nReason
    );
}

void CMdSpi::OnHeartBeatWarning(int32_t nTimeLapse) {
    this->gateway->on_heart_beat_warning(
        nTimeLapse
    );
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool bIsLast) {
#if defined(__APPLE__)
    CThostFtdcRspUserLoginField widened{};
    if (pRspUserLogin != nullptr) {
        std::memcpy(&widened, pRspUserLogin, kDarwinRspUserLoginSize);
        pRspUserLogin = &widened;
    }
#endif
    this->gateway->on_rsp_user_login(
        Converter::CThostFtdcRspUserLoginFieldToRust(pRspUserLogin),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        bIsLast
    );
}

void CMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool bIsLast) {
    this->gateway->on_rsp_user_logout(
        Converter::CThostFtdcUserLogoutFieldToRust(pUserLogout),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        bIsLast
    );
}

void CMdSpi::OnRspQryMulticastInstrument(CThostFtdcMulticastInstrumentField* pMulticastInstrument, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool bIsLast) {
    this->gateway->on_rsp_qry_multicast_instrument(
        Converter::CThostFtdcMulticastInstrumentFieldToRust(pMulticastInstrument),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        bIsLast
    );
}

void CMdSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool bIsLast) {
    this->gateway->on_rsp_error(
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        bIsLast
    );
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool bIsLast) {
    this->gateway->on_rsp_sub_market_data(
        Converter::CThostFtdcSpecificInstrumentFieldToRust(pSpecificInstrument),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        bIsLast
    );
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool bIsLast) {
    this->gateway->on_rsp_un_sub_market_data(
        Converter::CThostFtdcSpecificInstrumentFieldToRust(pSpecificInstrument),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        bIsLast
    );
}

void CMdSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool bIsLast) {
    this->gateway->on_rsp_sub_for_quote_rsp(
        Converter::CThostFtdcSpecificInstrumentFieldToRust(pSpecificInstrument),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        bIsLast
    );
}

void CMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int32_t nRequestID, bool bIsLast) {
    this->gateway->on_rsp_un_sub_for_quote_rsp(
        Converter::CThostFtdcSpecificInstrumentFieldToRust(pSpecificInstrument),
        Converter::CThostFtdcRspInfoFieldToRust(pRspInfo),
        nRequestID,
        bIsLast
    );
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData) {
    this->gateway->on_rtn_depth_market_data(
        Converter::CThostFtdcDepthMarketDataFieldToRust(pDepthMarketData)
    );
}

void CMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField* pForQuoteRsp) {
    this->gateway->on_rtn_for_quote_rsp(
        Converter::CThostFtdcForQuoteRspFieldToRust(pForQuoteRsp)
    );
}
