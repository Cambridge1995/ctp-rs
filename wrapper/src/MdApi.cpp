#include "ctp-rs/wrapper/include/MdApi.h"
#include "ctp-rs/wrapper/include/Converter.h"

#ifdef __APPLE__
// Both implemented in MdApiDarwinShim.cpp. CreateFtdcMdApi takes only three
// booleans on darwin (is_production_mode is silently dropped); GetApiVersion
// is a static class method that we resolve via dlsym at runtime since the
// dylib is loaded dynamically rather than linked.
extern "C" void* CtpRsDarwinCreateFtdcMdApi(const char*, bool, bool);
extern "C" const char* CtpRsDarwinGetMdApiVersion();
#endif

MdApi::MdApi(rust::Box<MdSpi> gateway, rust::String flow_path, bool is_using_udp, bool is_multicast, bool is_production_mode) {
    spi = new CMdSpi(std::move(gateway));
#ifdef __APPLE__
    (void)is_production_mode;
    api = static_cast<CThostFtdcMdApi*>(
        CtpRsDarwinCreateFtdcMdApi(flow_path.c_str(), is_using_udp, is_multicast));
#else
    api = CThostFtdcMdApi::CreateFtdcMdApi(flow_path.c_str(), is_using_udp, is_multicast, is_production_mode);
#endif
    api->RegisterSpi(spi);
}

MdApi::~MdApi() {
    if (api) {
        api->Release();
        api = nullptr;
    }
    delete spi;
    spi = nullptr;
}

std::unique_ptr<MdApi> CreateMdApi(rust::Box<MdSpi> gateway, rust::String flow_path, bool is_using_udp, bool is_multicast, bool is_production_mode) {
    return std::make_unique<MdApi>(std::move(gateway), flow_path, is_using_udp, is_multicast, is_production_mode);
}

rust::String MdApi::get_api_version() const {
#ifdef __APPLE__
    return CtpRsDarwinGetMdApiVersion();
#else
    return api->GetApiVersion(
    );
#endif
}

void MdApi::init() const {
    return api->Init(
    );
}

int32_t MdApi::join() const {
    return api->Join(
    );
}

rust::String MdApi::get_trading_day() const {
    return api->GetTradingDay(
    );
}

void MdApi::register_front(rust::String front_address) const {
    return api->RegisterFront(
        const_cast<char *>(front_address.c_str())
    );
}

void MdApi::register_name_server(rust::String ns_address) const {
    return api->RegisterNameServer(
        const_cast<char *>(ns_address.c_str())
    );
}

void MdApi::register_fens_user_info(FensUserInfo fens_user_info) const {
    CThostFtdcFensUserInfoField req(Converter::FensUserInfoToCpp(fens_user_info));
    return api->RegisterFensUserInfo(
        &req
    );
}

int32_t MdApi::subscribe_market_data(rust::Vec<rust::String> instrument_ids) const {
    char **instrument_ids_raw = new char *[instrument_ids.size()];
    for (int i = 0; i < instrument_ids.size(); i++)
        instrument_ids_raw[i] = (char *)instrument_ids[i].c_str();
    int ret = api->SubscribeMarketData(instrument_ids_raw, instrument_ids.size());
    delete[] instrument_ids_raw;
    return ret;
}

int32_t MdApi::un_subscribe_market_data(rust::Vec<rust::String> instrument_ids) const {
    char **instrument_ids_raw = new char *[instrument_ids.size()];
    for (int i = 0; i < instrument_ids.size(); i++)
        instrument_ids_raw[i] = (char *)instrument_ids[i].c_str();
    int ret = api->UnSubscribeMarketData(instrument_ids_raw, instrument_ids.size());
    delete[] instrument_ids_raw;
    return ret;
}

int32_t MdApi::subscribe_for_quote_rsp(rust::Vec<rust::String> instrument_ids) const {
    char **instrument_ids_raw = new char *[instrument_ids.size()];
    for (int i = 0; i < instrument_ids.size(); i++)
        instrument_ids_raw[i] = (char *)instrument_ids[i].c_str();
    int ret = api->SubscribeForQuoteRsp(instrument_ids_raw, instrument_ids.size());
    delete[] instrument_ids_raw;
    return ret;
}

int32_t MdApi::un_subscribe_for_quote_rsp(rust::Vec<rust::String> instrument_ids) const {
    char **instrument_ids_raw = new char *[instrument_ids.size()];
    for (int i = 0; i < instrument_ids.size(); i++)
        instrument_ids_raw[i] = (char *)instrument_ids[i].c_str();
    int ret = api->UnSubscribeForQuoteRsp(instrument_ids_raw, instrument_ids.size());
    delete[] instrument_ids_raw;
    return ret;
}

int32_t MdApi::req_user_login(ReqUserLogin req_user_login, int32_t request_id) const {
    CThostFtdcReqUserLoginField req(Converter::ReqUserLoginToCpp(req_user_login));
    return api->ReqUserLogin(
        &req,
        request_id
    );
}

int32_t MdApi::req_user_logout(UserLogout user_logout, int32_t request_id) const {
    CThostFtdcUserLogoutField req(Converter::UserLogoutToCpp(user_logout));
    return api->ReqUserLogout(
        &req,
        request_id
    );
}

int32_t MdApi::req_qry_multicast_instrument(QryMulticastInstrument qry_multicast_instrument, int32_t request_id) const {
    CThostFtdcQryMulticastInstrumentField req(Converter::QryMulticastInstrumentToCpp(qry_multicast_instrument));
    return api->ReqQryMulticastInstrument(
        &req,
        request_id
    );
}
