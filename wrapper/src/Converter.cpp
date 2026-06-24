#include "ctp-rs/wrapper/include/Converter.h"

#ifdef _WIN32

#include <Windows.h>

std::string Converter::Gb2312ToUtf8(const char *src_str)
{
    int len = MultiByteToWideChar(936, 0, src_str, -1, NULL, 0);
    wchar_t *wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(936, 0, src_str, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char *str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    std::string strTemp = str;
    if (wstr)
        delete[] wstr;
    if (str)
        delete[] str;
    return strTemp;
}

#else

#include <iconv.h>

#define MAX_BUF 3072

void gb2312_to_utf8(const char *src, char *dst, int len)
{
    int ret = 0;
    size_t inlen = strlen(src) + 1;
    size_t outlen = len;

    // duanqn: The iconv function in Linux requires non-const char *
    // So we need to copy the source string
    char *inbuf = (char *)malloc(len);
    char *inbuf_hold = inbuf; // iconv may change the address of inbuf
                              // so we use another pointer to keep the address
    memcpy(inbuf, src, len);

    char *outbuf2 = NULL;
    char *outbuf = dst;
    iconv_t cd;

    // starkwong: if src==dst, the string will become invalid during conversion since UTF-8 is 3 chars in Chinese but GBK is mostly 2 chars
    if (src == dst)
    {
        outbuf2 = (char *)malloc(len);
        memset(outbuf2, 0, len);
        outbuf = outbuf2;
    }

    cd = iconv_open("UTF-8", "GB2312");
    if (cd != (iconv_t)-1)
    {
        ret = iconv(cd, &inbuf, &inlen, &outbuf, &outlen);

        if (outbuf2 != NULL)
        {
            strcpy(dst, outbuf2);
            free(outbuf2);
        }

        iconv_close(cd);
    }

    if (ret != 0)
    {
        printf("iconv failed, buf: [0x%02x", (unsigned char)*inbuf_hold);
        for (char *p = inbuf_hold + 1; p && *p; p++)
            printf(", 0x%02x", (unsigned char)*p);
        printf("], err: %s\n", strerror(errno));
        dst[0] = '\0';
    }
    free(inbuf_hold); // Don't pass in inbuf as it may have been modified
}

std::string Converter::Gb2312ToUtf8(const char *src_str)
{
    char dst_str[MAX_BUF] = {0};
    gb2312_to_utf8(src_str, dst_str, MAX_BUF);
    return std::string(dst_str);
}

#endif

rust::String Converter::Gb2312ToRustString(const char *src_str)
{
    if (src_str == nullptr)
        return rust::String("");
    return rust::String(Converter::Gb2312ToUtf8(src_str));
}

CThostFtdcDisseminationField Converter::DisseminationToCpp(Dissemination x) {
    CThostFtdcDisseminationField y;
    memset(&y, 0, sizeof(y));
    y.SequenceSeries = x.sequence_series;
    y.SequenceNo = x.sequence_no;
    return y;
}

Dissemination Converter::CThostFtdcDisseminationFieldToRust(CThostFtdcDisseminationField* x) {
    if (x == nullptr)
        return Dissemination{.is_null = true};
    Dissemination y{};
    y.sequence_series = x->SequenceSeries;
    y.sequence_no = x->SequenceNo;
    return y;
}

CThostFtdcReqUserLoginField Converter::ReqUserLoginToCpp(ReqUserLogin x) {
    CThostFtdcReqUserLoginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.Password, x.password.c_str());
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.InterfaceProductInfo, x.interface_product_info.c_str());
    strcpy(y.ProtocolInfo, x.protocol_info.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.OneTimePassword, x.one_time_password.c_str());
    strcpy(y.LoginRemark, x.login_remark.c_str());
    y.ClientIPPort = x.client_ip_port;
    strcpy(y.ClientIPAddress, x.client_ip_address.c_str());
    return y;
}

ReqUserLogin Converter::CThostFtdcReqUserLoginFieldToRust(CThostFtdcReqUserLoginField* x) {
    if (x == nullptr)
        return ReqUserLogin{.is_null = true};
    ReqUserLogin y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.interface_product_info = Converter::Gb2312ToRustString(x->InterfaceProductInfo);
    y.protocol_info = Converter::Gb2312ToRustString(x->ProtocolInfo);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.one_time_password = Converter::Gb2312ToRustString(x->OneTimePassword);
    y.login_remark = Converter::Gb2312ToRustString(x->LoginRemark);
    y.client_ip_port = x->ClientIPPort;
    y.client_ip_address = Converter::Gb2312ToRustString(x->ClientIPAddress);
    return y;
}

CThostFtdcRspUserLoginField Converter::RspUserLoginToCpp(RspUserLogin x) {
    CThostFtdcRspUserLoginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.LoginTime, x.login_time.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.SystemName, x.system_name.c_str());
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.MaxOrderRef, x.max_order_ref.c_str());
    strcpy(y.SHFETime, x.shfe_time.c_str());
    strcpy(y.DCETime, x.dce_time.c_str());
    strcpy(y.CZCETime, x.czce_time.c_str());
    strcpy(y.FFEXTime, x.ffex_time.c_str());
    strcpy(y.INETime, x.ine_time.c_str());
    strcpy(y.SysVersion, x.sys_version.c_str());
    strcpy(y.GFEXTime, x.gfex_time.c_str());
    y.LoginDRIdentityID = x.login_dr_identity_id;
    y.UserDRIdentityID = x.user_dr_identity_id;
    strcpy(y.LastLoginTime, x.last_login_time.c_str());
    strcpy(y.ReserveInfo, x.reserve_info.c_str());
    return y;
}

RspUserLogin Converter::CThostFtdcRspUserLoginFieldToRust(CThostFtdcRspUserLoginField* x) {
    if (x == nullptr)
        return RspUserLogin{.is_null = true};
    RspUserLogin y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.login_time = Converter::Gb2312ToRustString(x->LoginTime);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.system_name = Converter::Gb2312ToRustString(x->SystemName);
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.max_order_ref = Converter::Gb2312ToRustString(x->MaxOrderRef);
    y.shfe_time = Converter::Gb2312ToRustString(x->SHFETime);
    y.dce_time = Converter::Gb2312ToRustString(x->DCETime);
    y.czce_time = Converter::Gb2312ToRustString(x->CZCETime);
    y.ffex_time = Converter::Gb2312ToRustString(x->FFEXTime);
    y.ine_time = Converter::Gb2312ToRustString(x->INETime);
    y.sys_version = Converter::Gb2312ToRustString(x->SysVersion);
    y.gfex_time = Converter::Gb2312ToRustString(x->GFEXTime);
    y.login_dr_identity_id = x->LoginDRIdentityID;
    y.user_dr_identity_id = x->UserDRIdentityID;
    y.last_login_time = Converter::Gb2312ToRustString(x->LastLoginTime);
    y.reserve_info = Converter::Gb2312ToRustString(x->ReserveInfo);
    return y;
}

CThostFtdcUserLogoutField Converter::UserLogoutToCpp(UserLogout x) {
    CThostFtdcUserLogoutField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    return y;
}

UserLogout Converter::CThostFtdcUserLogoutFieldToRust(CThostFtdcUserLogoutField* x) {
    if (x == nullptr)
        return UserLogout{.is_null = true};
    UserLogout y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    return y;
}

CThostFtdcForceUserLogoutField Converter::ForceUserLogoutToCpp(ForceUserLogout x) {
    CThostFtdcForceUserLogoutField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    return y;
}

ForceUserLogout Converter::CThostFtdcForceUserLogoutFieldToRust(CThostFtdcForceUserLogoutField* x) {
    if (x == nullptr)
        return ForceUserLogout{.is_null = true};
    ForceUserLogout y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    return y;
}

CThostFtdcReqAuthenticateField Converter::ReqAuthenticateToCpp(ReqAuthenticate x) {
    CThostFtdcReqAuthenticateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.AuthCode, x.auth_code.c_str());
    strcpy(y.AppID, x.app_id.c_str());
    return y;
}

ReqAuthenticate Converter::CThostFtdcReqAuthenticateFieldToRust(CThostFtdcReqAuthenticateField* x) {
    if (x == nullptr)
        return ReqAuthenticate{.is_null = true};
    ReqAuthenticate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.auth_code = Converter::Gb2312ToRustString(x->AuthCode);
    y.app_id = Converter::Gb2312ToRustString(x->AppID);
    return y;
}

CThostFtdcRspAuthenticateField Converter::RspAuthenticateToCpp(RspAuthenticate x) {
    CThostFtdcRspAuthenticateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.AppID, x.app_id.c_str());
    y.AppType = x.app_type;
    return y;
}

RspAuthenticate Converter::CThostFtdcRspAuthenticateFieldToRust(CThostFtdcRspAuthenticateField* x) {
    if (x == nullptr)
        return RspAuthenticate{.is_null = true};
    RspAuthenticate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.app_id = Converter::Gb2312ToRustString(x->AppID);
    y.app_type = x->AppType;
    return y;
}

CThostFtdcAuthenticationInfoField Converter::AuthenticationInfoToCpp(AuthenticationInfo x) {
    CThostFtdcAuthenticationInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.AuthInfo, x.auth_info.c_str());
    y.IsResult = x.is_result;
    strcpy(y.AppID, x.app_id.c_str());
    y.AppType = x.app_type;
    strcpy(y.ClientIPAddress, x.client_ip_address.c_str());
    return y;
}

AuthenticationInfo Converter::CThostFtdcAuthenticationInfoFieldToRust(CThostFtdcAuthenticationInfoField* x) {
    if (x == nullptr)
        return AuthenticationInfo{.is_null = true};
    AuthenticationInfo y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.auth_info = Converter::Gb2312ToRustString(x->AuthInfo);
    y.is_result = x->IsResult;
    y.app_id = Converter::Gb2312ToRustString(x->AppID);
    y.app_type = x->AppType;
    y.client_ip_address = Converter::Gb2312ToRustString(x->ClientIPAddress);
    return y;
}

CThostFtdcRspUserLogin2Field Converter::RspUserLogin2ToCpp(RspUserLogin2 x) {
    CThostFtdcRspUserLogin2Field y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.LoginTime, x.login_time.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.SystemName, x.system_name.c_str());
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.MaxOrderRef, x.max_order_ref.c_str());
    strcpy(y.SHFETime, x.shfe_time.c_str());
    strcpy(y.DCETime, x.dce_time.c_str());
    strcpy(y.CZCETime, x.czce_time.c_str());
    strcpy(y.FFEXTime, x.ffex_time.c_str());
    strcpy(y.INETime, x.ine_time.c_str());
    memcpy(y.RandomString, x.random_string.data(), x.random_string.size() * sizeof(uint8_t));
    return y;
}

RspUserLogin2 Converter::CThostFtdcRspUserLogin2FieldToRust(CThostFtdcRspUserLogin2Field* x) {
    if (x == nullptr)
        return RspUserLogin2{.is_null = true};
    RspUserLogin2 y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.login_time = Converter::Gb2312ToRustString(x->LoginTime);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.system_name = Converter::Gb2312ToRustString(x->SystemName);
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.max_order_ref = Converter::Gb2312ToRustString(x->MaxOrderRef);
    y.shfe_time = Converter::Gb2312ToRustString(x->SHFETime);
    y.dce_time = Converter::Gb2312ToRustString(x->DCETime);
    y.czce_time = Converter::Gb2312ToRustString(x->CZCETime);
    y.ffex_time = Converter::Gb2312ToRustString(x->FFEXTime);
    y.ine_time = Converter::Gb2312ToRustString(x->INETime);
    for (int i = 0; i < 17; i++)
        y.random_string.push_back(x->RandomString[i]);
    return y;
}

CThostFtdcTransferHeaderField Converter::TransferHeaderToCpp(TransferHeader x) {
    CThostFtdcTransferHeaderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.Version, x.version.c_str());
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.TradeSerial, x.trade_serial.c_str());
    strcpy(y.FutureID, x.future_id.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBrchID, x.bank_branch_id.c_str());
    strcpy(y.OperNo, x.oper_no.c_str());
    strcpy(y.DeviceID, x.device_id.c_str());
    memcpy(y.RecordNum, x.record_num.data(), x.record_num.size() * sizeof(uint8_t));
    y.SessionID = x.session_id;
    y.RequestID = x.request_id;
    return y;
}

TransferHeader Converter::CThostFtdcTransferHeaderFieldToRust(CThostFtdcTransferHeaderField* x) {
    if (x == nullptr)
        return TransferHeader{.is_null = true};
    TransferHeader y{};
    y.version = Converter::Gb2312ToRustString(x->Version);
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.trade_serial = Converter::Gb2312ToRustString(x->TradeSerial);
    y.future_id = Converter::Gb2312ToRustString(x->FutureID);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBrchID);
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    for (int i = 0; i < 7; i++)
        y.record_num.push_back(x->RecordNum[i]);
    y.session_id = x->SessionID;
    y.request_id = x->RequestID;
    return y;
}

CThostFtdcTransferBankToFutureReqField Converter::TransferBankToFutureReqToCpp(TransferBankToFutureReq x) {
    CThostFtdcTransferBankToFutureReqField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.FutureAccount, x.future_account.c_str());
    y.FuturePwdFlag = x.future_pwd_flag;
    strcpy(y.FutureAccPwd, x.future_acc_pwd.c_str());
    y.TradeAmt = x.trade_amt;
    y.CustFee = x.cust_fee;
    strcpy(y.CurrencyCode, x.currency_code.c_str());
    return y;
}

TransferBankToFutureReq Converter::CThostFtdcTransferBankToFutureReqFieldToRust(CThostFtdcTransferBankToFutureReqField* x) {
    if (x == nullptr)
        return TransferBankToFutureReq{.is_null = true};
    TransferBankToFutureReq y{};
    y.future_account = Converter::Gb2312ToRustString(x->FutureAccount);
    y.future_pwd_flag = x->FuturePwdFlag;
    y.future_acc_pwd = Converter::Gb2312ToRustString(x->FutureAccPwd);
    y.trade_amt = x->TradeAmt;
    y.cust_fee = x->CustFee;
    y.currency_code = Converter::Gb2312ToRustString(x->CurrencyCode);
    return y;
}

CThostFtdcTransferBankToFutureRspField Converter::TransferBankToFutureRspToCpp(TransferBankToFutureRsp x) {
    CThostFtdcTransferBankToFutureRspField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.RetCode, x.ret_code.c_str());
    strcpy(y.RetInfo, x.ret_info.c_str());
    strcpy(y.FutureAccount, x.future_account.c_str());
    y.TradeAmt = x.trade_amt;
    y.CustFee = x.cust_fee;
    strcpy(y.CurrencyCode, x.currency_code.c_str());
    return y;
}

TransferBankToFutureRsp Converter::CThostFtdcTransferBankToFutureRspFieldToRust(CThostFtdcTransferBankToFutureRspField* x) {
    if (x == nullptr)
        return TransferBankToFutureRsp{.is_null = true};
    TransferBankToFutureRsp y{};
    y.ret_code = Converter::Gb2312ToRustString(x->RetCode);
    y.ret_info = Converter::Gb2312ToRustString(x->RetInfo);
    y.future_account = Converter::Gb2312ToRustString(x->FutureAccount);
    y.trade_amt = x->TradeAmt;
    y.cust_fee = x->CustFee;
    y.currency_code = Converter::Gb2312ToRustString(x->CurrencyCode);
    return y;
}

CThostFtdcTransferFutureToBankReqField Converter::TransferFutureToBankReqToCpp(TransferFutureToBankReq x) {
    CThostFtdcTransferFutureToBankReqField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.FutureAccount, x.future_account.c_str());
    y.FuturePwdFlag = x.future_pwd_flag;
    strcpy(y.FutureAccPwd, x.future_acc_pwd.c_str());
    y.TradeAmt = x.trade_amt;
    y.CustFee = x.cust_fee;
    strcpy(y.CurrencyCode, x.currency_code.c_str());
    return y;
}

TransferFutureToBankReq Converter::CThostFtdcTransferFutureToBankReqFieldToRust(CThostFtdcTransferFutureToBankReqField* x) {
    if (x == nullptr)
        return TransferFutureToBankReq{.is_null = true};
    TransferFutureToBankReq y{};
    y.future_account = Converter::Gb2312ToRustString(x->FutureAccount);
    y.future_pwd_flag = x->FuturePwdFlag;
    y.future_acc_pwd = Converter::Gb2312ToRustString(x->FutureAccPwd);
    y.trade_amt = x->TradeAmt;
    y.cust_fee = x->CustFee;
    y.currency_code = Converter::Gb2312ToRustString(x->CurrencyCode);
    return y;
}

CThostFtdcTransferFutureToBankRspField Converter::TransferFutureToBankRspToCpp(TransferFutureToBankRsp x) {
    CThostFtdcTransferFutureToBankRspField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.RetCode, x.ret_code.c_str());
    strcpy(y.RetInfo, x.ret_info.c_str());
    strcpy(y.FutureAccount, x.future_account.c_str());
    y.TradeAmt = x.trade_amt;
    y.CustFee = x.cust_fee;
    strcpy(y.CurrencyCode, x.currency_code.c_str());
    return y;
}

TransferFutureToBankRsp Converter::CThostFtdcTransferFutureToBankRspFieldToRust(CThostFtdcTransferFutureToBankRspField* x) {
    if (x == nullptr)
        return TransferFutureToBankRsp{.is_null = true};
    TransferFutureToBankRsp y{};
    y.ret_code = Converter::Gb2312ToRustString(x->RetCode);
    y.ret_info = Converter::Gb2312ToRustString(x->RetInfo);
    y.future_account = Converter::Gb2312ToRustString(x->FutureAccount);
    y.trade_amt = x->TradeAmt;
    y.cust_fee = x->CustFee;
    y.currency_code = Converter::Gb2312ToRustString(x->CurrencyCode);
    return y;
}

CThostFtdcTransferQryBankReqField Converter::TransferQryBankReqToCpp(TransferQryBankReq x) {
    CThostFtdcTransferQryBankReqField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.FutureAccount, x.future_account.c_str());
    y.FuturePwdFlag = x.future_pwd_flag;
    strcpy(y.FutureAccPwd, x.future_acc_pwd.c_str());
    strcpy(y.CurrencyCode, x.currency_code.c_str());
    return y;
}

TransferQryBankReq Converter::CThostFtdcTransferQryBankReqFieldToRust(CThostFtdcTransferQryBankReqField* x) {
    if (x == nullptr)
        return TransferQryBankReq{.is_null = true};
    TransferQryBankReq y{};
    y.future_account = Converter::Gb2312ToRustString(x->FutureAccount);
    y.future_pwd_flag = x->FuturePwdFlag;
    y.future_acc_pwd = Converter::Gb2312ToRustString(x->FutureAccPwd);
    y.currency_code = Converter::Gb2312ToRustString(x->CurrencyCode);
    return y;
}

CThostFtdcTransferQryBankRspField Converter::TransferQryBankRspToCpp(TransferQryBankRsp x) {
    CThostFtdcTransferQryBankRspField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.RetCode, x.ret_code.c_str());
    strcpy(y.RetInfo, x.ret_info.c_str());
    strcpy(y.FutureAccount, x.future_account.c_str());
    y.TradeAmt = x.trade_amt;
    y.UseAmt = x.use_amt;
    y.FetchAmt = x.fetch_amt;
    strcpy(y.CurrencyCode, x.currency_code.c_str());
    return y;
}

TransferQryBankRsp Converter::CThostFtdcTransferQryBankRspFieldToRust(CThostFtdcTransferQryBankRspField* x) {
    if (x == nullptr)
        return TransferQryBankRsp{.is_null = true};
    TransferQryBankRsp y{};
    y.ret_code = Converter::Gb2312ToRustString(x->RetCode);
    y.ret_info = Converter::Gb2312ToRustString(x->RetInfo);
    y.future_account = Converter::Gb2312ToRustString(x->FutureAccount);
    y.trade_amt = x->TradeAmt;
    y.use_amt = x->UseAmt;
    y.fetch_amt = x->FetchAmt;
    y.currency_code = Converter::Gb2312ToRustString(x->CurrencyCode);
    return y;
}

CThostFtdcTransferQryDetailReqField Converter::TransferQryDetailReqToCpp(TransferQryDetailReq x) {
    CThostFtdcTransferQryDetailReqField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.FutureAccount, x.future_account.c_str());
    return y;
}

TransferQryDetailReq Converter::CThostFtdcTransferQryDetailReqFieldToRust(CThostFtdcTransferQryDetailReqField* x) {
    if (x == nullptr)
        return TransferQryDetailReq{.is_null = true};
    TransferQryDetailReq y{};
    y.future_account = Converter::Gb2312ToRustString(x->FutureAccount);
    return y;
}

CThostFtdcTransferQryDetailRspField Converter::TransferQryDetailRspToCpp(TransferQryDetailRsp x) {
    CThostFtdcTransferQryDetailRspField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.TradeCode, x.trade_code.c_str());
    y.FutureSerial = x.future_serial;
    strcpy(y.FutureID, x.future_id.c_str());
    strcpy(y.FutureAccount, x.future_account.c_str());
    y.BankSerial = x.bank_serial;
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBrchID, x.bank_branch_id.c_str());
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.CertCode, x.cert_code.c_str());
    strcpy(y.CurrencyCode, x.currency_code.c_str());
    y.TxAmount = x.tx_amount;
    y.Flag = x.flag;
    return y;
}

TransferQryDetailRsp Converter::CThostFtdcTransferQryDetailRspFieldToRust(CThostFtdcTransferQryDetailRspField* x) {
    if (x == nullptr)
        return TransferQryDetailRsp{.is_null = true};
    TransferQryDetailRsp y{};
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.future_serial = x->FutureSerial;
    y.future_id = Converter::Gb2312ToRustString(x->FutureID);
    y.future_account = Converter::Gb2312ToRustString(x->FutureAccount);
    y.bank_serial = x->BankSerial;
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBrchID);
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.cert_code = Converter::Gb2312ToRustString(x->CertCode);
    y.currency_code = Converter::Gb2312ToRustString(x->CurrencyCode);
    y.tx_amount = x->TxAmount;
    y.flag = x->Flag;
    return y;
}

CThostFtdcRspInfoField Converter::RspInfoToCpp(RspInfo x) {
    CThostFtdcRspInfoField y;
    memset(&y, 0, sizeof(y));
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    return y;
}

RspInfo Converter::CThostFtdcRspInfoFieldToRust(CThostFtdcRspInfoField* x) {
    if (x == nullptr)
        return RspInfo{.is_null = true};
    RspInfo y{};
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    return y;
}

CThostFtdcExchangeField Converter::ExchangeToCpp(Exchange x) {
    CThostFtdcExchangeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ExchangeName, x.exchange_name.c_str());
    y.ExchangeProperty = x.exchange_property;
    return y;
}

Exchange Converter::CThostFtdcExchangeFieldToRust(CThostFtdcExchangeField* x) {
    if (x == nullptr)
        return Exchange{.is_null = true};
    Exchange y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.exchange_name = Converter::Gb2312ToRustString(x->ExchangeName);
    y.exchange_property = x->ExchangeProperty;
    return y;
}

CThostFtdcProductField Converter::ProductToCpp(Product x) {
    CThostFtdcProductField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ProductName, x.product_name.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.ProductClass = x.product_class;
    y.VolumeMultiple = x.volume_multiple;
    y.PriceTick = x.price_tick;
    y.MaxMarketOrderVolume = x.max_market_order_volume;
    y.MinMarketOrderVolume = x.min_market_order_volume;
    y.MaxLimitOrderVolume = x.max_limit_order_volume;
    y.MinLimitOrderVolume = x.min_limit_order_volume;
    y.PositionType = x.position_type;
    y.PositionDateType = x.position_date_type;
    y.CloseDealType = x.close_deal_type;
    strcpy(y.TradeCurrencyID, x.trade_currency_id.c_str());
    y.MortgageFundUseRange = x.mortgage_fund_use_range;
    y.UnderlyingMultiple = x.underlying_multiple;
    strcpy(y.ProductID, x.product_id.c_str());
    strcpy(y.ExchangeProductID, x.exchange_product_id.c_str());
    y.OpenLimitControlLevel = x.open_limit_control_level;
    y.OrderFreqControlLevel = x.order_freq_control_level;
    return y;
}

Product Converter::CThostFtdcProductFieldToRust(CThostFtdcProductField* x) {
    if (x == nullptr)
        return Product{.is_null = true};
    Product y{};
    y.product_name = Converter::Gb2312ToRustString(x->ProductName);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_class = x->ProductClass;
    y.volume_multiple = x->VolumeMultiple;
    y.price_tick = x->PriceTick;
    y.max_market_order_volume = x->MaxMarketOrderVolume;
    y.min_market_order_volume = x->MinMarketOrderVolume;
    y.max_limit_order_volume = x->MaxLimitOrderVolume;
    y.min_limit_order_volume = x->MinLimitOrderVolume;
    y.position_type = x->PositionType;
    y.position_date_type = x->PositionDateType;
    y.close_deal_type = x->CloseDealType;
    y.trade_currency_id = Converter::Gb2312ToRustString(x->TradeCurrencyID);
    y.mortgage_fund_use_range = x->MortgageFundUseRange;
    y.underlying_multiple = x->UnderlyingMultiple;
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.exchange_product_id = Converter::Gb2312ToRustString(x->ExchangeProductID);
    y.open_limit_control_level = x->OpenLimitControlLevel;
    y.order_freq_control_level = x->OrderFreqControlLevel;
    return y;
}

CThostFtdcInstrumentField Converter::InstrumentToCpp(Instrument x) {
    CThostFtdcInstrumentField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentName, x.instrument_name.c_str());
    y.ProductClass = x.product_class;
    y.DeliveryYear = x.delivery_year;
    y.DeliveryMonth = x.delivery_month;
    y.MaxMarketOrderVolume = x.max_market_order_volume;
    y.MinMarketOrderVolume = x.min_market_order_volume;
    y.MaxLimitOrderVolume = x.max_limit_order_volume;
    y.MinLimitOrderVolume = x.min_limit_order_volume;
    y.VolumeMultiple = x.volume_multiple;
    y.PriceTick = x.price_tick;
    strcpy(y.CreateDate, x.create_date.c_str());
    strcpy(y.OpenDate, x.open_date.c_str());
    strcpy(y.ExpireDate, x.expire_date.c_str());
    strcpy(y.StartDelivDate, x.start_delivery_date.c_str());
    strcpy(y.EndDelivDate, x.end_delivery_date.c_str());
    y.InstLifePhase = x.inst_life_phase;
    y.IsTrading = x.is_trading;
    y.PositionType = x.position_type;
    y.PositionDateType = x.position_date_type;
    y.LongMarginRatio = x.long_margin_ratio;
    y.ShortMarginRatio = x.short_margin_ratio;
    y.MaxMarginSideAlgorithm = x.max_margin_side_algorithm;
    y.StrikePrice = x.strike_price;
    y.OptionsType = x.options_type;
    y.UnderlyingMultiple = x.underlying_multiple;
    y.CombinationType = x.combination_type;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    strcpy(y.UnderlyingInstrID, x.underlying_instr_id.c_str());
    return y;
}

Instrument Converter::CThostFtdcInstrumentFieldToRust(CThostFtdcInstrumentField* x) {
    if (x == nullptr)
        return Instrument{.is_null = true};
    Instrument y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_name = Converter::Gb2312ToRustString(x->InstrumentName);
    y.product_class = x->ProductClass;
    y.delivery_year = x->DeliveryYear;
    y.delivery_month = x->DeliveryMonth;
    y.max_market_order_volume = x->MaxMarketOrderVolume;
    y.min_market_order_volume = x->MinMarketOrderVolume;
    y.max_limit_order_volume = x->MaxLimitOrderVolume;
    y.min_limit_order_volume = x->MinLimitOrderVolume;
    y.volume_multiple = x->VolumeMultiple;
    y.price_tick = x->PriceTick;
    y.create_date = Converter::Gb2312ToRustString(x->CreateDate);
    y.open_date = Converter::Gb2312ToRustString(x->OpenDate);
    y.expire_date = Converter::Gb2312ToRustString(x->ExpireDate);
    y.start_delivery_date = Converter::Gb2312ToRustString(x->StartDelivDate);
    y.end_delivery_date = Converter::Gb2312ToRustString(x->EndDelivDate);
    y.inst_life_phase = x->InstLifePhase;
    y.is_trading = x->IsTrading;
    y.position_type = x->PositionType;
    y.position_date_type = x->PositionDateType;
    y.long_margin_ratio = x->LongMarginRatio;
    y.short_margin_ratio = x->ShortMarginRatio;
    y.max_margin_side_algorithm = x->MaxMarginSideAlgorithm;
    y.strike_price = x->StrikePrice;
    y.options_type = x->OptionsType;
    y.underlying_multiple = x->UnderlyingMultiple;
    y.combination_type = x->CombinationType;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.underlying_instr_id = Converter::Gb2312ToRustString(x->UnderlyingInstrID);
    return y;
}

CThostFtdcBrokerField Converter::BrokerToCpp(Broker x) {
    CThostFtdcBrokerField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerAbbr, x.broker_abbr.c_str());
    strcpy(y.BrokerName, x.broker_name.c_str());
    y.IsActive = x.is_active;
    return y;
}

Broker Converter::CThostFtdcBrokerFieldToRust(CThostFtdcBrokerField* x) {
    if (x == nullptr)
        return Broker{.is_null = true};
    Broker y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_abbr = Converter::Gb2312ToRustString(x->BrokerAbbr);
    y.broker_name = Converter::Gb2312ToRustString(x->BrokerName);
    y.is_active = x->IsActive;
    return y;
}

CThostFtdcTraderField Converter::TraderToCpp(Trader x) {
    CThostFtdcTraderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.InstallCount = x.install_count;
    strcpy(y.BrokerID, x.broker_id.c_str());
    y.OrderCancelAlg = x.order_cancel_alg;
    y.TradeInstallCount = x.trade_install_count;
    y.MDInstallCount = x.md_install_count;
    return y;
}

Trader Converter::CThostFtdcTraderFieldToRust(CThostFtdcTraderField* x) {
    if (x == nullptr)
        return Trader{.is_null = true};
    Trader y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.install_count = x->InstallCount;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.order_cancel_alg = x->OrderCancelAlg;
    y.trade_install_count = x->TradeInstallCount;
    y.md_install_count = x->MDInstallCount;
    return y;
}

CThostFtdcInvestorField Converter::InvestorToCpp(Investor x) {
    CThostFtdcInvestorField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorGroupID, x.investor_group_id.c_str());
    strcpy(y.InvestorName, x.investor_name.c_str());
    y.IdentifiedCardType = x.identified_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.IsActive = x.is_active;
    strcpy(y.Telephone, x.telephone.c_str());
    strcpy(y.Address, x.address.c_str());
    strcpy(y.OpenDate, x.open_date.c_str());
    strcpy(y.Mobile, x.mobile.c_str());
    strcpy(y.CommModelID, x.comm_model_id.c_str());
    strcpy(y.MarginModelID, x.margin_model_id.c_str());
    y.IsOrderFreq = x.is_order_freq;
    y.IsOpenVolLimit = x.is_open_vol_limit;
    return y;
}

Investor Converter::CThostFtdcInvestorFieldToRust(CThostFtdcInvestorField* x) {
    if (x == nullptr)
        return Investor{.is_null = true};
    Investor y{};
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_group_id = Converter::Gb2312ToRustString(x->InvestorGroupID);
    y.investor_name = Converter::Gb2312ToRustString(x->InvestorName);
    y.identified_card_type = x->IdentifiedCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.is_active = x->IsActive;
    y.telephone = Converter::Gb2312ToRustString(x->Telephone);
    y.address = Converter::Gb2312ToRustString(x->Address);
    y.open_date = Converter::Gb2312ToRustString(x->OpenDate);
    y.mobile = Converter::Gb2312ToRustString(x->Mobile);
    y.comm_model_id = Converter::Gb2312ToRustString(x->CommModelID);
    y.margin_model_id = Converter::Gb2312ToRustString(x->MarginModelID);
    y.is_order_freq = x->IsOrderFreq;
    y.is_open_vol_limit = x->IsOpenVolLimit;
    return y;
}

CThostFtdcTradingCodeField Converter::TradingCodeToCpp(TradingCode x) {
    CThostFtdcTradingCodeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    y.IsActive = x.is_active;
    y.ClientIDType = x.client_idtype;
    strcpy(y.BranchID, x.branch_id.c_str());
    y.BizType = x.biz_type;
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    return y;
}

TradingCode Converter::CThostFtdcTradingCodeFieldToRust(CThostFtdcTradingCodeField* x) {
    if (x == nullptr)
        return TradingCode{.is_null = true};
    TradingCode y{};
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.is_active = x->IsActive;
    y.client_idtype = x->ClientIDType;
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.biz_type = x->BizType;
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    return y;
}

CThostFtdcPartBrokerField Converter::PartBrokerToCpp(PartBroker x) {
    CThostFtdcPartBrokerField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    y.IsActive = x.is_active;
    return y;
}

PartBroker Converter::CThostFtdcPartBrokerFieldToRust(CThostFtdcPartBrokerField* x) {
    if (x == nullptr)
        return PartBroker{.is_null = true};
    PartBroker y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.is_active = x->IsActive;
    return y;
}

CThostFtdcSuperUserField Converter::SuperUserToCpp(SuperUser x) {
    CThostFtdcSuperUserField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.UserName, x.user_name.c_str());
    strcpy(y.Password, x.password.c_str());
    y.IsActive = x.is_active;
    return y;
}

SuperUser Converter::CThostFtdcSuperUserFieldToRust(CThostFtdcSuperUserField* x) {
    if (x == nullptr)
        return SuperUser{.is_null = true};
    SuperUser y{};
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.user_name = Converter::Gb2312ToRustString(x->UserName);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.is_active = x->IsActive;
    return y;
}

CThostFtdcSuperUserFunctionField Converter::SuperUserFunctionToCpp(SuperUserFunction x) {
    CThostFtdcSuperUserFunctionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.UserID, x.user_id.c_str());
    y.FunctionCode = x.function_code;
    return y;
}

SuperUserFunction Converter::CThostFtdcSuperUserFunctionFieldToRust(CThostFtdcSuperUserFunctionField* x) {
    if (x == nullptr)
        return SuperUserFunction{.is_null = true};
    SuperUserFunction y{};
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.function_code = x->FunctionCode;
    return y;
}

CThostFtdcInvestorGroupField Converter::InvestorGroupToCpp(InvestorGroup x) {
    CThostFtdcInvestorGroupField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorGroupID, x.investor_group_id.c_str());
    strcpy(y.InvestorGroupName, x.investor_group_name.c_str());
    return y;
}

InvestorGroup Converter::CThostFtdcInvestorGroupFieldToRust(CThostFtdcInvestorGroupField* x) {
    if (x == nullptr)
        return InvestorGroup{.is_null = true};
    InvestorGroup y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_group_id = Converter::Gb2312ToRustString(x->InvestorGroupID);
    y.investor_group_name = Converter::Gb2312ToRustString(x->InvestorGroupName);
    return y;
}

CThostFtdcTradingAccountField Converter::TradingAccountToCpp(TradingAccount x) {
    CThostFtdcTradingAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    y.PreMortgage = x.pre_mortgage;
    y.PreCredit = x.pre_credit;
    y.PreDeposit = x.pre_deposit;
    y.PreBalance = x.pre_balance;
    y.PreMargin = x.pre_margin;
    y.InterestBase = x.interest_base;
    y.Interest = x.interest;
    y.Deposit = x.deposit;
    y.Withdraw = x.withdraw;
    y.FrozenMargin = x.frozen_margin;
    y.FrozenCash = x.frozen_cash;
    y.FrozenCommission = x.frozen_commission;
    y.CurrMargin = x.curr_margin;
    y.CashIn = x.cash_in;
    y.Commission = x.commission;
    y.CloseProfit = x.close_profit;
    y.PositionProfit = x.position_profit;
    y.Balance = x.balance;
    y.Available = x.available;
    y.WithdrawQuota = x.withdraw_quota;
    y.Reserve = x.reserve;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    y.Credit = x.credit;
    y.Mortgage = x.mortgage;
    y.ExchangeMargin = x.exchange_margin;
    y.DeliveryMargin = x.delivery_margin;
    y.ExchangeDeliveryMargin = x.exchange_delivery_margin;
    y.ReserveBalance = x.reserve_balance;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.PreFundMortgageIn = x.pre_fund_mortgage_in;
    y.PreFundMortgageOut = x.pre_fund_mortgage_out;
    y.FundMortgageIn = x.fund_mortgage_in;
    y.FundMortgageOut = x.fund_mortgage_out;
    y.FundMortgageAvailable = x.fund_mortgage_available;
    y.MortgageableFund = x.mortgageable_fund;
    y.SpecProductMargin = x.spec_product_margin;
    y.SpecProductFrozenMargin = x.spec_product_frozen_margin;
    y.SpecProductCommission = x.spec_product_commission;
    y.SpecProductFrozenCommission = x.spec_product_frozen_commission;
    y.SpecProductPositionProfit = x.spec_product_position_profit;
    y.SpecProductCloseProfit = x.spec_product_close_profit;
    y.SpecProductPositionProfitByAlg = x.spec_product_position_profit_by_alg;
    y.SpecProductExchangeMargin = x.spec_product_exchange_margin;
    y.BizType = x.biz_type;
    y.FrozenSwap = x.frozen_swap;
    y.RemainSwap = x.remain_swap;
    y.OptionValue = x.option_value;
    return y;
}

TradingAccount Converter::CThostFtdcTradingAccountFieldToRust(CThostFtdcTradingAccountField* x) {
    if (x == nullptr)
        return TradingAccount{.is_null = true};
    TradingAccount y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.pre_mortgage = x->PreMortgage;
    y.pre_credit = x->PreCredit;
    y.pre_deposit = x->PreDeposit;
    y.pre_balance = x->PreBalance;
    y.pre_margin = x->PreMargin;
    y.interest_base = x->InterestBase;
    y.interest = x->Interest;
    y.deposit = x->Deposit;
    y.withdraw = x->Withdraw;
    y.frozen_margin = x->FrozenMargin;
    y.frozen_cash = x->FrozenCash;
    y.frozen_commission = x->FrozenCommission;
    y.curr_margin = x->CurrMargin;
    y.cash_in = x->CashIn;
    y.commission = x->Commission;
    y.close_profit = x->CloseProfit;
    y.position_profit = x->PositionProfit;
    y.balance = x->Balance;
    y.available = x->Available;
    y.withdraw_quota = x->WithdrawQuota;
    y.reserve = x->Reserve;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.credit = x->Credit;
    y.mortgage = x->Mortgage;
    y.exchange_margin = x->ExchangeMargin;
    y.delivery_margin = x->DeliveryMargin;
    y.exchange_delivery_margin = x->ExchangeDeliveryMargin;
    y.reserve_balance = x->ReserveBalance;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.pre_fund_mortgage_in = x->PreFundMortgageIn;
    y.pre_fund_mortgage_out = x->PreFundMortgageOut;
    y.fund_mortgage_in = x->FundMortgageIn;
    y.fund_mortgage_out = x->FundMortgageOut;
    y.fund_mortgage_available = x->FundMortgageAvailable;
    y.mortgageable_fund = x->MortgageableFund;
    y.spec_product_margin = x->SpecProductMargin;
    y.spec_product_frozen_margin = x->SpecProductFrozenMargin;
    y.spec_product_commission = x->SpecProductCommission;
    y.spec_product_frozen_commission = x->SpecProductFrozenCommission;
    y.spec_product_position_profit = x->SpecProductPositionProfit;
    y.spec_product_close_profit = x->SpecProductCloseProfit;
    y.spec_product_position_profit_by_alg = x->SpecProductPositionProfitByAlg;
    y.spec_product_exchange_margin = x->SpecProductExchangeMargin;
    y.biz_type = x->BizType;
    y.frozen_swap = x->FrozenSwap;
    y.remain_swap = x->RemainSwap;
    y.option_value = x->OptionValue;
    return y;
}

CThostFtdcInvestorPositionField Converter::InvestorPositionToCpp(InvestorPosition x) {
    CThostFtdcInvestorPositionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.PosiDirection = x.posi_direction;
    y.HedgeFlag = x.hedge_flag;
    y.PositionDate = x.position_date;
    y.YdPosition = x.yd_position;
    y.Position = x.position;
    y.LongFrozen = x.long_frozen;
    y.ShortFrozen = x.short_frozen;
    y.LongFrozenAmount = x.long_frozen_amount;
    y.ShortFrozenAmount = x.short_frozen_amount;
    y.OpenVolume = x.open_volume;
    y.CloseVolume = x.close_volume;
    y.OpenAmount = x.open_amount;
    y.CloseAmount = x.close_amount;
    y.PositionCost = x.position_cost;
    y.PreMargin = x.pre_margin;
    y.UseMargin = x.use_margin;
    y.FrozenMargin = x.frozen_margin;
    y.FrozenCash = x.frozen_cash;
    y.FrozenCommission = x.frozen_commission;
    y.CashIn = x.cash_in;
    y.Commission = x.commission;
    y.CloseProfit = x.close_profit;
    y.PositionProfit = x.position_profit;
    y.PreSettlementPrice = x.pre_settlement_price;
    y.SettlementPrice = x.settlement_price;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    y.OpenCost = x.open_cost;
    y.ExchangeMargin = x.exchange_margin;
    y.CombPosition = x.comb_position;
    y.CombLongFrozen = x.comb_long_frozen;
    y.CombShortFrozen = x.comb_short_frozen;
    y.CloseProfitByDate = x.close_profit_by_date;
    y.CloseProfitByTrade = x.close_profit_by_trade;
    y.TodayPosition = x.today_position;
    y.MarginRateByMoney = x.margin_rate_by_money;
    y.MarginRateByVolume = x.margin_rate_by_volume;
    y.StrikeFrozen = x.strike_frozen;
    y.StrikeFrozenAmount = x.strike_frozen_amount;
    y.AbandonFrozen = x.abandon_frozen;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.YdStrikeFrozen = x.yd_strike_frozen;
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    y.PositionCostOffset = x.position_cost_offset;
    y.TasPosition = x.tas_position;
    y.TasPositionCost = x.tas_position_cost;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.OptionValue = x.option_value;
    return y;
}

InvestorPosition Converter::CThostFtdcInvestorPositionFieldToRust(CThostFtdcInvestorPositionField* x) {
    if (x == nullptr)
        return InvestorPosition{.is_null = true};
    InvestorPosition y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.posi_direction = x->PosiDirection;
    y.hedge_flag = x->HedgeFlag;
    y.position_date = x->PositionDate;
    y.yd_position = x->YdPosition;
    y.position = x->Position;
    y.long_frozen = x->LongFrozen;
    y.short_frozen = x->ShortFrozen;
    y.long_frozen_amount = x->LongFrozenAmount;
    y.short_frozen_amount = x->ShortFrozenAmount;
    y.open_volume = x->OpenVolume;
    y.close_volume = x->CloseVolume;
    y.open_amount = x->OpenAmount;
    y.close_amount = x->CloseAmount;
    y.position_cost = x->PositionCost;
    y.pre_margin = x->PreMargin;
    y.use_margin = x->UseMargin;
    y.frozen_margin = x->FrozenMargin;
    y.frozen_cash = x->FrozenCash;
    y.frozen_commission = x->FrozenCommission;
    y.cash_in = x->CashIn;
    y.commission = x->Commission;
    y.close_profit = x->CloseProfit;
    y.position_profit = x->PositionProfit;
    y.pre_settlement_price = x->PreSettlementPrice;
    y.settlement_price = x->SettlementPrice;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.open_cost = x->OpenCost;
    y.exchange_margin = x->ExchangeMargin;
    y.comb_position = x->CombPosition;
    y.comb_long_frozen = x->CombLongFrozen;
    y.comb_short_frozen = x->CombShortFrozen;
    y.close_profit_by_date = x->CloseProfitByDate;
    y.close_profit_by_trade = x->CloseProfitByTrade;
    y.today_position = x->TodayPosition;
    y.margin_rate_by_money = x->MarginRateByMoney;
    y.margin_rate_by_volume = x->MarginRateByVolume;
    y.strike_frozen = x->StrikeFrozen;
    y.strike_frozen_amount = x->StrikeFrozenAmount;
    y.abandon_frozen = x->AbandonFrozen;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.yd_strike_frozen = x->YdStrikeFrozen;
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.position_cost_offset = x->PositionCostOffset;
    y.tas_position = x->TasPosition;
    y.tas_position_cost = x->TasPositionCost;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.option_value = x->OptionValue;
    return y;
}

CThostFtdcInstrumentMarginRateField Converter::InstrumentMarginRateToCpp(InstrumentMarginRate x) {
    CThostFtdcInstrumentMarginRateField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.LongMarginRatioByMoney = x.long_margin_ratio_by_money;
    y.LongMarginRatioByVolume = x.long_margin_ratio_by_volume;
    y.ShortMarginRatioByMoney = x.short_margin_ratio_by_money;
    y.ShortMarginRatioByVolume = x.short_margin_ratio_by_volume;
    y.IsRelative = x.is_relative;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

InstrumentMarginRate Converter::CThostFtdcInstrumentMarginRateFieldToRust(CThostFtdcInstrumentMarginRateField* x) {
    if (x == nullptr)
        return InstrumentMarginRate{.is_null = true};
    InstrumentMarginRate y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.hedge_flag = x->HedgeFlag;
    y.long_margin_ratio_by_money = x->LongMarginRatioByMoney;
    y.long_margin_ratio_by_volume = x->LongMarginRatioByVolume;
    y.short_margin_ratio_by_money = x->ShortMarginRatioByMoney;
    y.short_margin_ratio_by_volume = x->ShortMarginRatioByVolume;
    y.is_relative = x->IsRelative;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcInstrumentCommissionRateField Converter::InstrumentCommissionRateToCpp(InstrumentCommissionRate x) {
    CThostFtdcInstrumentCommissionRateField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OpenRatioByMoney = x.open_ratio_by_money;
    y.OpenRatioByVolume = x.open_ratio_by_volume;
    y.CloseRatioByMoney = x.close_ratio_by_money;
    y.CloseRatioByVolume = x.close_ratio_by_volume;
    y.CloseTodayRatioByMoney = x.close_today_ratio_by_money;
    y.CloseTodayRatioByVolume = x.close_today_ratio_by_volume;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.BizType = x.biz_type;
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

InstrumentCommissionRate Converter::CThostFtdcInstrumentCommissionRateFieldToRust(CThostFtdcInstrumentCommissionRateField* x) {
    if (x == nullptr)
        return InstrumentCommissionRate{.is_null = true};
    InstrumentCommissionRate y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.open_ratio_by_money = x->OpenRatioByMoney;
    y.open_ratio_by_volume = x->OpenRatioByVolume;
    y.close_ratio_by_money = x->CloseRatioByMoney;
    y.close_ratio_by_volume = x->CloseRatioByVolume;
    y.close_today_ratio_by_money = x->CloseTodayRatioByMoney;
    y.close_today_ratio_by_volume = x->CloseTodayRatioByVolume;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.biz_type = x->BizType;
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcDepthMarketDataField Converter::DepthMarketDataToCpp(DepthMarketData x) {
    CThostFtdcDepthMarketDataField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.LastPrice = x.last_price;
    y.PreSettlementPrice = x.pre_settlement_price;
    y.PreClosePrice = x.pre_close_price;
    y.PreOpenInterest = x.pre_open_interest;
    y.OpenPrice = x.open_price;
    y.HighestPrice = x.highest_price;
    y.LowestPrice = x.lowest_price;
    y.Volume = x.volume;
    y.Turnover = x.turnover;
    y.OpenInterest = x.open_interest;
    y.ClosePrice = x.close_price;
    y.SettlementPrice = x.settlement_price;
    y.UpperLimitPrice = x.upper_limit_price;
    y.LowerLimitPrice = x.lower_limit_price;
    y.PreDelta = x.pre_delta;
    y.CurrDelta = x.curr_delta;
    strcpy(y.UpdateTime, x.update_time.c_str());
    y.UpdateMillisec = x.update_millisecond;
    y.BidPrice1 = x.bid_price1;
    y.BidVolume1 = x.bid_volume1;
    y.AskPrice1 = x.ask_price1;
    y.AskVolume1 = x.ask_volume1;
    y.BidPrice2 = x.bid_price2;
    y.BidVolume2 = x.bid_volume2;
    y.AskPrice2 = x.ask_price2;
    y.AskVolume2 = x.ask_volume2;
    y.BidPrice3 = x.bid_price3;
    y.BidVolume3 = x.bid_volume3;
    y.AskPrice3 = x.ask_price3;
    y.AskVolume3 = x.ask_volume3;
    y.BidPrice4 = x.bid_price4;
    y.BidVolume4 = x.bid_volume4;
    y.AskPrice4 = x.ask_price4;
    y.AskVolume4 = x.ask_volume4;
    y.BidPrice5 = x.bid_price5;
    y.BidVolume5 = x.bid_volume5;
    y.AskPrice5 = x.ask_price5;
    y.AskVolume5 = x.ask_volume5;
    y.AveragePrice = x.average_price;
    strcpy(y.ActionDay, x.action_day.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    y.BandingUpperPrice = x.banding_upper_price;
    y.BandingLowerPrice = x.banding_lower_price;
    return y;
}

DepthMarketData Converter::CThostFtdcDepthMarketDataFieldToRust(CThostFtdcDepthMarketDataField* x) {
    if (x == nullptr)
        return DepthMarketData{.is_null = true};
    DepthMarketData y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.last_price = x->LastPrice;
    y.pre_settlement_price = x->PreSettlementPrice;
    y.pre_close_price = x->PreClosePrice;
    y.pre_open_interest = x->PreOpenInterest;
    y.open_price = x->OpenPrice;
    y.highest_price = x->HighestPrice;
    y.lowest_price = x->LowestPrice;
    y.volume = x->Volume;
    y.turnover = x->Turnover;
    y.open_interest = x->OpenInterest;
    y.close_price = x->ClosePrice;
    y.settlement_price = x->SettlementPrice;
    y.upper_limit_price = x->UpperLimitPrice;
    y.lower_limit_price = x->LowerLimitPrice;
    y.pre_delta = x->PreDelta;
    y.curr_delta = x->CurrDelta;
    y.update_time = Converter::Gb2312ToRustString(x->UpdateTime);
    y.update_millisecond = x->UpdateMillisec;
    y.bid_price1 = x->BidPrice1;
    y.bid_volume1 = x->BidVolume1;
    y.ask_price1 = x->AskPrice1;
    y.ask_volume1 = x->AskVolume1;
    y.bid_price2 = x->BidPrice2;
    y.bid_volume2 = x->BidVolume2;
    y.ask_price2 = x->AskPrice2;
    y.ask_volume2 = x->AskVolume2;
    y.bid_price3 = x->BidPrice3;
    y.bid_volume3 = x->BidVolume3;
    y.ask_price3 = x->AskPrice3;
    y.ask_volume3 = x->AskVolume3;
    y.bid_price4 = x->BidPrice4;
    y.bid_volume4 = x->BidVolume4;
    y.ask_price4 = x->AskPrice4;
    y.ask_volume4 = x->AskVolume4;
    y.bid_price5 = x->BidPrice5;
    y.bid_volume5 = x->BidVolume5;
    y.ask_price5 = x->AskPrice5;
    y.ask_volume5 = x->AskVolume5;
    y.average_price = x->AveragePrice;
    y.action_day = Converter::Gb2312ToRustString(x->ActionDay);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.banding_upper_price = x->BandingUpperPrice;
    y.banding_lower_price = x->BandingLowerPrice;
    return y;
}

CThostFtdcInstrumentTradingRightField Converter::InstrumentTradingRightToCpp(InstrumentTradingRight x) {
    CThostFtdcInstrumentTradingRightField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.TradingRight = x.trading_right;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

InstrumentTradingRight Converter::CThostFtdcInstrumentTradingRightFieldToRust(CThostFtdcInstrumentTradingRightField* x) {
    if (x == nullptr)
        return InstrumentTradingRight{.is_null = true};
    InstrumentTradingRight y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.trading_right = x->TradingRight;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcBrokerUserField Converter::BrokerUserToCpp(BrokerUser x) {
    CThostFtdcBrokerUserField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.UserName, x.user_name.c_str());
    y.UserType = x.user_type;
    y.IsActive = x.is_active;
    y.IsUsingOTP = x.is_using_otp;
    y.IsAuthForce = x.is_auth_force;
    return y;
}

BrokerUser Converter::CThostFtdcBrokerUserFieldToRust(CThostFtdcBrokerUserField* x) {
    if (x == nullptr)
        return BrokerUser{.is_null = true};
    BrokerUser y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.user_name = Converter::Gb2312ToRustString(x->UserName);
    y.user_type = x->UserType;
    y.is_active = x->IsActive;
    y.is_using_otp = x->IsUsingOTP;
    y.is_auth_force = x->IsAuthForce;
    return y;
}

CThostFtdcBrokerUserPasswordField Converter::BrokerUserPasswordToCpp(BrokerUserPassword x) {
    CThostFtdcBrokerUserPasswordField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.Password, x.password.c_str());
    strcpy(y.LastUpdateTime, x.last_update_time.c_str());
    strcpy(y.LastLoginTime, x.last_login_time.c_str());
    strcpy(y.ExpireDate, x.expire_date.c_str());
    strcpy(y.WeakExpireDate, x.weak_expire_date.c_str());
    return y;
}

BrokerUserPassword Converter::CThostFtdcBrokerUserPasswordFieldToRust(CThostFtdcBrokerUserPasswordField* x) {
    if (x == nullptr)
        return BrokerUserPassword{.is_null = true};
    BrokerUserPassword y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.last_update_time = Converter::Gb2312ToRustString(x->LastUpdateTime);
    y.last_login_time = Converter::Gb2312ToRustString(x->LastLoginTime);
    y.expire_date = Converter::Gb2312ToRustString(x->ExpireDate);
    y.weak_expire_date = Converter::Gb2312ToRustString(x->WeakExpireDate);
    return y;
}

CThostFtdcBrokerUserFunctionField Converter::BrokerUserFunctionToCpp(BrokerUserFunction x) {
    CThostFtdcBrokerUserFunctionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.BrokerFunctionCode = x.broker_function_code;
    return y;
}

BrokerUserFunction Converter::CThostFtdcBrokerUserFunctionFieldToRust(CThostFtdcBrokerUserFunctionField* x) {
    if (x == nullptr)
        return BrokerUserFunction{.is_null = true};
    BrokerUserFunction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.broker_function_code = x->BrokerFunctionCode;
    return y;
}

CThostFtdcTraderOfferField Converter::TraderOfferToCpp(TraderOffer x) {
    CThostFtdcTraderOfferField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.InstallID = x.install_id;
    strcpy(y.OrderLocalID, x.order_local_id.c_str());
    y.TraderConnectStatus = x.trader_connect_status;
    strcpy(y.ConnectRequestDate, x.connect_request_date.c_str());
    strcpy(y.ConnectRequestTime, x.connect_request_time.c_str());
    strcpy(y.LastReportDate, x.last_report_date.c_str());
    strcpy(y.LastReportTime, x.last_report_time.c_str());
    strcpy(y.ConnectDate, x.connect_date.c_str());
    strcpy(y.ConnectTime, x.connect_time.c_str());
    strcpy(y.StartDate, x.start_date.c_str());
    strcpy(y.StartTime, x.start_time.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.MaxTradeID, x.max_trade_id.c_str());
    memcpy(y.MaxOrderMessageReference, x.max_order_message_reference.data(), x.max_order_message_reference.size() * sizeof(uint8_t));
    y.OrderCancelAlg = x.order_cancel_alg;
    return y;
}

TraderOffer Converter::CThostFtdcTraderOfferFieldToRust(CThostFtdcTraderOfferField* x) {
    if (x == nullptr)
        return TraderOffer{.is_null = true};
    TraderOffer y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.install_id = x->InstallID;
    y.order_local_id = Converter::Gb2312ToRustString(x->OrderLocalID);
    y.trader_connect_status = x->TraderConnectStatus;
    y.connect_request_date = Converter::Gb2312ToRustString(x->ConnectRequestDate);
    y.connect_request_time = Converter::Gb2312ToRustString(x->ConnectRequestTime);
    y.last_report_date = Converter::Gb2312ToRustString(x->LastReportDate);
    y.last_report_time = Converter::Gb2312ToRustString(x->LastReportTime);
    y.connect_date = Converter::Gb2312ToRustString(x->ConnectDate);
    y.connect_time = Converter::Gb2312ToRustString(x->ConnectTime);
    y.start_date = Converter::Gb2312ToRustString(x->StartDate);
    y.start_time = Converter::Gb2312ToRustString(x->StartTime);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.max_trade_id = Converter::Gb2312ToRustString(x->MaxTradeID);
    for (int i = 0; i < 7; i++)
        y.max_order_message_reference.push_back(x->MaxOrderMessageReference[i]);
    y.order_cancel_alg = x->OrderCancelAlg;
    return y;
}

CThostFtdcSettlementInfoField Converter::SettlementInfoToCpp(SettlementInfo x) {
    CThostFtdcSettlementInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.SequenceNo = x.sequence_no;
    memcpy(y.Content, x.content.data(), x.content.size() * sizeof(uint8_t));
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

SettlementInfo Converter::CThostFtdcSettlementInfoFieldToRust(CThostFtdcSettlementInfoField* x) {
    if (x == nullptr)
        return SettlementInfo{.is_null = true};
    SettlementInfo y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.sequence_no = x->SequenceNo;
    for (int i = 0; i < 501; i++)
        y.content.push_back(x->Content[i]);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcInstrumentMarginRateAdjustField Converter::InstrumentMarginRateAdjustToCpp(InstrumentMarginRateAdjust x) {
    CThostFtdcInstrumentMarginRateAdjustField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.LongMarginRatioByMoney = x.long_margin_ratio_by_money;
    y.LongMarginRatioByVolume = x.long_margin_ratio_by_volume;
    y.ShortMarginRatioByMoney = x.short_margin_ratio_by_money;
    y.ShortMarginRatioByVolume = x.short_margin_ratio_by_volume;
    y.IsRelative = x.is_relative;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

InstrumentMarginRateAdjust Converter::CThostFtdcInstrumentMarginRateAdjustFieldToRust(CThostFtdcInstrumentMarginRateAdjustField* x) {
    if (x == nullptr)
        return InstrumentMarginRateAdjust{.is_null = true};
    InstrumentMarginRateAdjust y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.hedge_flag = x->HedgeFlag;
    y.long_margin_ratio_by_money = x->LongMarginRatioByMoney;
    y.long_margin_ratio_by_volume = x->LongMarginRatioByVolume;
    y.short_margin_ratio_by_money = x->ShortMarginRatioByMoney;
    y.short_margin_ratio_by_volume = x->ShortMarginRatioByVolume;
    y.is_relative = x->IsRelative;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcExchangeMarginRateField Converter::ExchangeMarginRateToCpp(ExchangeMarginRate x) {
    CThostFtdcExchangeMarginRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.LongMarginRatioByMoney = x.long_margin_ratio_by_money;
    y.LongMarginRatioByVolume = x.long_margin_ratio_by_volume;
    y.ShortMarginRatioByMoney = x.short_margin_ratio_by_money;
    y.ShortMarginRatioByVolume = x.short_margin_ratio_by_volume;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

ExchangeMarginRate Converter::CThostFtdcExchangeMarginRateFieldToRust(CThostFtdcExchangeMarginRateField* x) {
    if (x == nullptr)
        return ExchangeMarginRate{.is_null = true};
    ExchangeMarginRate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.hedge_flag = x->HedgeFlag;
    y.long_margin_ratio_by_money = x->LongMarginRatioByMoney;
    y.long_margin_ratio_by_volume = x->LongMarginRatioByVolume;
    y.short_margin_ratio_by_money = x->ShortMarginRatioByMoney;
    y.short_margin_ratio_by_volume = x->ShortMarginRatioByVolume;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcExchangeMarginRateAdjustField Converter::ExchangeMarginRateAdjustToCpp(ExchangeMarginRateAdjust x) {
    CThostFtdcExchangeMarginRateAdjustField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.LongMarginRatioByMoney = x.long_margin_ratio_by_money;
    y.LongMarginRatioByVolume = x.long_margin_ratio_by_volume;
    y.ShortMarginRatioByMoney = x.short_margin_ratio_by_money;
    y.ShortMarginRatioByVolume = x.short_margin_ratio_by_volume;
    y.ExchLongMarginRatioByMoney = x.exch_long_margin_ratio_by_money;
    y.ExchLongMarginRatioByVolume = x.exch_long_margin_ratio_by_volume;
    y.ExchShortMarginRatioByMoney = x.exch_short_margin_ratio_by_money;
    y.ExchShortMarginRatioByVolume = x.exch_short_margin_ratio_by_volume;
    y.NoLongMarginRatioByMoney = x.no_long_margin_ratio_by_money;
    y.NoLongMarginRatioByVolume = x.no_long_margin_ratio_by_volume;
    y.NoShortMarginRatioByMoney = x.no_short_margin_ratio_by_money;
    y.NoShortMarginRatioByVolume = x.no_short_margin_ratio_by_volume;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

ExchangeMarginRateAdjust Converter::CThostFtdcExchangeMarginRateAdjustFieldToRust(CThostFtdcExchangeMarginRateAdjustField* x) {
    if (x == nullptr)
        return ExchangeMarginRateAdjust{.is_null = true};
    ExchangeMarginRateAdjust y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.hedge_flag = x->HedgeFlag;
    y.long_margin_ratio_by_money = x->LongMarginRatioByMoney;
    y.long_margin_ratio_by_volume = x->LongMarginRatioByVolume;
    y.short_margin_ratio_by_money = x->ShortMarginRatioByMoney;
    y.short_margin_ratio_by_volume = x->ShortMarginRatioByVolume;
    y.exch_long_margin_ratio_by_money = x->ExchLongMarginRatioByMoney;
    y.exch_long_margin_ratio_by_volume = x->ExchLongMarginRatioByVolume;
    y.exch_short_margin_ratio_by_money = x->ExchShortMarginRatioByMoney;
    y.exch_short_margin_ratio_by_volume = x->ExchShortMarginRatioByVolume;
    y.no_long_margin_ratio_by_money = x->NoLongMarginRatioByMoney;
    y.no_long_margin_ratio_by_volume = x->NoLongMarginRatioByVolume;
    y.no_short_margin_ratio_by_money = x->NoShortMarginRatioByMoney;
    y.no_short_margin_ratio_by_volume = x->NoShortMarginRatioByVolume;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcExchangeRateField Converter::ExchangeRateToCpp(ExchangeRate x) {
    CThostFtdcExchangeRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.FromCurrencyID, x.from_currency_id.c_str());
    y.FromCurrencyUnit = x.from_currency_unit;
    strcpy(y.ToCurrencyID, x.to_currency_id.c_str());
    y.ExchangeRate = x.exchange_rate;
    return y;
}

ExchangeRate Converter::CThostFtdcExchangeRateFieldToRust(CThostFtdcExchangeRateField* x) {
    if (x == nullptr)
        return ExchangeRate{.is_null = true};
    ExchangeRate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.from_currency_id = Converter::Gb2312ToRustString(x->FromCurrencyID);
    y.from_currency_unit = x->FromCurrencyUnit;
    y.to_currency_id = Converter::Gb2312ToRustString(x->ToCurrencyID);
    y.exchange_rate = x->ExchangeRate;
    return y;
}

CThostFtdcSettlementRefField Converter::SettlementRefToCpp(SettlementRef x) {
    CThostFtdcSettlementRefField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    return y;
}

SettlementRef Converter::CThostFtdcSettlementRefFieldToRust(CThostFtdcSettlementRefField* x) {
    if (x == nullptr)
        return SettlementRef{.is_null = true};
    SettlementRef y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    return y;
}

CThostFtdcCurrentTimeField Converter::CurrentTimeToCpp(CurrentTime x) {
    CThostFtdcCurrentTimeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.CurrDate, x.curr_date.c_str());
    strcpy(y.CurrTime, x.curr_time.c_str());
    y.CurrMillisec = x.curr_millisecond;
    strcpy(y.ActionDay, x.action_day.c_str());
    return y;
}

CurrentTime Converter::CThostFtdcCurrentTimeFieldToRust(CThostFtdcCurrentTimeField* x) {
    if (x == nullptr)
        return CurrentTime{.is_null = true};
    CurrentTime y{};
    y.curr_date = Converter::Gb2312ToRustString(x->CurrDate);
    y.curr_time = Converter::Gb2312ToRustString(x->CurrTime);
    y.curr_millisecond = x->CurrMillisec;
    y.action_day = Converter::Gb2312ToRustString(x->ActionDay);
    return y;
}

CThostFtdcCommPhaseField Converter::CommPhaseToCpp(CommPhase x) {
    CThostFtdcCommPhaseField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.CommPhaseNo = x.comm_phase_no;
    strcpy(y.SystemID, x.system_id.c_str());
    return y;
}

CommPhase Converter::CThostFtdcCommPhaseFieldToRust(CThostFtdcCommPhaseField* x) {
    if (x == nullptr)
        return CommPhase{.is_null = true};
    CommPhase y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.comm_phase_no = x->CommPhaseNo;
    y.system_id = Converter::Gb2312ToRustString(x->SystemID);
    return y;
}

CThostFtdcLoginInfoField Converter::LoginInfoToCpp(LoginInfo x) {
    CThostFtdcLoginInfoField y;
    memset(&y, 0, sizeof(y));
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.LoginDate, x.login_date.c_str());
    strcpy(y.LoginTime, x.login_time.c_str());
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.InterfaceProductInfo, x.interface_product_info.c_str());
    strcpy(y.ProtocolInfo, x.protocol_info.c_str());
    strcpy(y.SystemName, x.system_name.c_str());
    memcpy(y.PasswordDeprecated, x.password_deprecated.data(), x.password_deprecated.size() * sizeof(uint8_t));
    strcpy(y.MaxOrderRef, x.max_order_ref.c_str());
    strcpy(y.SHFETime, x.shfe_time.c_str());
    strcpy(y.DCETime, x.dce_time.c_str());
    strcpy(y.CZCETime, x.czce_time.c_str());
    strcpy(y.FFEXTime, x.ffex_time.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.OneTimePassword, x.one_time_password.c_str());
    strcpy(y.INETime, x.ine_time.c_str());
    y.IsQryControl = x.is_qry_control;
    strcpy(y.LoginRemark, x.login_remark.c_str());
    strcpy(y.Password, x.password.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

LoginInfo Converter::CThostFtdcLoginInfoFieldToRust(CThostFtdcLoginInfoField* x) {
    if (x == nullptr)
        return LoginInfo{.is_null = true};
    LoginInfo y{};
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.login_date = Converter::Gb2312ToRustString(x->LoginDate);
    y.login_time = Converter::Gb2312ToRustString(x->LoginTime);
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.interface_product_info = Converter::Gb2312ToRustString(x->InterfaceProductInfo);
    y.protocol_info = Converter::Gb2312ToRustString(x->ProtocolInfo);
    y.system_name = Converter::Gb2312ToRustString(x->SystemName);
    for (int i = 0; i < 41; i++)
        y.password_deprecated.push_back(x->PasswordDeprecated[i]);
    y.max_order_ref = Converter::Gb2312ToRustString(x->MaxOrderRef);
    y.shfe_time = Converter::Gb2312ToRustString(x->SHFETime);
    y.dce_time = Converter::Gb2312ToRustString(x->DCETime);
    y.czce_time = Converter::Gb2312ToRustString(x->CZCETime);
    y.ffex_time = Converter::Gb2312ToRustString(x->FFEXTime);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.one_time_password = Converter::Gb2312ToRustString(x->OneTimePassword);
    y.ine_time = Converter::Gb2312ToRustString(x->INETime);
    y.is_qry_control = x->IsQryControl;
    y.login_remark = Converter::Gb2312ToRustString(x->LoginRemark);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcLogoutAllField Converter::LogoutAllToCpp(LogoutAll x) {
    CThostFtdcLogoutAllField y;
    memset(&y, 0, sizeof(y));
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.SystemName, x.system_name.c_str());
    return y;
}

LogoutAll Converter::CThostFtdcLogoutAllFieldToRust(CThostFtdcLogoutAllField* x) {
    if (x == nullptr)
        return LogoutAll{.is_null = true};
    LogoutAll y{};
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.system_name = Converter::Gb2312ToRustString(x->SystemName);
    return y;
}

CThostFtdcFrontStatusField Converter::FrontStatusToCpp(FrontStatus x) {
    CThostFtdcFrontStatusField y;
    memset(&y, 0, sizeof(y));
    y.FrontID = x.front_id;
    strcpy(y.LastReportDate, x.last_report_date.c_str());
    strcpy(y.LastReportTime, x.last_report_time.c_str());
    y.IsActive = x.is_active;
    return y;
}

FrontStatus Converter::CThostFtdcFrontStatusFieldToRust(CThostFtdcFrontStatusField* x) {
    if (x == nullptr)
        return FrontStatus{.is_null = true};
    FrontStatus y{};
    y.front_id = x->FrontID;
    y.last_report_date = Converter::Gb2312ToRustString(x->LastReportDate);
    y.last_report_time = Converter::Gb2312ToRustString(x->LastReportTime);
    y.is_active = x->IsActive;
    return y;
}

CThostFtdcUserPasswordUpdateField Converter::UserPasswordUpdateToCpp(UserPasswordUpdate x) {
    CThostFtdcUserPasswordUpdateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.OldPassword, x.old_password.c_str());
    strcpy(y.NewPassword, x.new_password.c_str());
    return y;
}

UserPasswordUpdate Converter::CThostFtdcUserPasswordUpdateFieldToRust(CThostFtdcUserPasswordUpdateField* x) {
    if (x == nullptr)
        return UserPasswordUpdate{.is_null = true};
    UserPasswordUpdate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.old_password = Converter::Gb2312ToRustString(x->OldPassword);
    y.new_password = Converter::Gb2312ToRustString(x->NewPassword);
    return y;
}

CThostFtdcInputOrderField Converter::InputOrderToCpp(InputOrder x) {
    CThostFtdcInputOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.OrderRef, x.order_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.OrderPriceType = x.order_price_type;
    y.Direction = x.direction;
    strcpy(y.CombOffsetFlag, x.comb_offset_flag.c_str());
    strcpy(y.CombHedgeFlag, x.comb_hedge_flag.c_str());
    y.LimitPrice = x.limit_price;
    y.VolumeTotalOriginal = x.volume_total_original;
    y.TimeCondition = x.time_condition;
    strcpy(y.GTDDate, x.gtd_date.c_str());
    y.VolumeCondition = x.volume_condition;
    y.MinVolume = x.min_volume;
    y.ContingentCondition = x.contingent_condition;
    y.StopPrice = x.stop_price;
    y.ForceCloseReason = x.force_close_reason;
    y.IsAutoSuspend = x.is_auto_suspend;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.RequestID = x.request_id;
    y.UserForceClose = x.user_force_close;
    y.IsSwapOrder = x.is_swap_order;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.OrderMemo, x.order_memo.c_str());
    y.SessionReqSeq = x.session_req_seq;
    return y;
}

InputOrder Converter::CThostFtdcInputOrderFieldToRust(CThostFtdcInputOrderField* x) {
    if (x == nullptr)
        return InputOrder{.is_null = true};
    InputOrder y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.order_ref = Converter::Gb2312ToRustString(x->OrderRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.order_price_type = x->OrderPriceType;
    y.direction = x->Direction;
    y.comb_offset_flag = Converter::Gb2312ToRustString(x->CombOffsetFlag);
    y.comb_hedge_flag = Converter::Gb2312ToRustString(x->CombHedgeFlag);
    y.limit_price = x->LimitPrice;
    y.volume_total_original = x->VolumeTotalOriginal;
    y.time_condition = x->TimeCondition;
    y.gtd_date = Converter::Gb2312ToRustString(x->GTDDate);
    y.volume_condition = x->VolumeCondition;
    y.min_volume = x->MinVolume;
    y.contingent_condition = x->ContingentCondition;
    y.stop_price = x->StopPrice;
    y.force_close_reason = x->ForceCloseReason;
    y.is_auto_suspend = x->IsAutoSuspend;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.request_id = x->RequestID;
    y.user_force_close = x->UserForceClose;
    y.is_swap_order = x->IsSwapOrder;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.order_memo = Converter::Gb2312ToRustString(x->OrderMemo);
    y.session_req_seq = x->SessionReqSeq;
    return y;
}

CThostFtdcOrderField Converter::OrderToCpp(Order x) {
    CThostFtdcOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.OrderRef, x.order_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.OrderPriceType = x.order_price_type;
    y.Direction = x.direction;
    strcpy(y.CombOffsetFlag, x.comb_offset_flag.c_str());
    strcpy(y.CombHedgeFlag, x.comb_hedge_flag.c_str());
    y.LimitPrice = x.limit_price;
    y.VolumeTotalOriginal = x.volume_total_original;
    y.TimeCondition = x.time_condition;
    strcpy(y.GTDDate, x.gtd_date.c_str());
    y.VolumeCondition = x.volume_condition;
    y.MinVolume = x.min_volume;
    y.ContingentCondition = x.contingent_condition;
    y.StopPrice = x.stop_price;
    y.ForceCloseReason = x.force_close_reason;
    y.IsAutoSuspend = x.is_auto_suspend;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.RequestID = x.request_id;
    strcpy(y.OrderLocalID, x.order_local_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    y.OrderSubmitStatus = x.order_submit_status;
    y.NotifySequence = x.notify_sequence;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.OrderSysID, x.order_sys_id.c_str());
    y.OrderSource = x.order_source;
    y.OrderStatus = x.order_status;
    y.OrderType = x.order_type;
    y.VolumeTraded = x.volume_traded;
    y.VolumeTotal = x.volume_total;
    strcpy(y.InsertDate, x.insert_date.c_str());
    strcpy(y.InsertTime, x.insert_time.c_str());
    strcpy(y.ActiveTime, x.active_time.c_str());
    strcpy(y.SuspendTime, x.suspend_time.c_str());
    strcpy(y.UpdateTime, x.update_time.c_str());
    strcpy(y.CancelTime, x.cancel_time.c_str());
    strcpy(y.ActiveTraderID, x.active_trader_id.c_str());
    strcpy(y.ClearingPartID, x.clearing_part_id.c_str());
    y.SequenceNo = x.sequence_no;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.StatusMsg, x.status_msg.c_str());
    y.UserForceClose = x.user_force_close;
    strcpy(y.ActiveUserID, x.active_user_id.c_str());
    y.BrokerOrderSeq = x.broker_order_seq;
    strcpy(y.RelativeOrderSysID, x.relative_order_sys_id.c_str());
    y.ZCETotalTradedVolume = x.zce_total_traded_volume;
    y.IsSwapOrder = x.is_swap_order;
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.OrderMemo, x.order_memo.c_str());
    y.SessionReqSeq = x.session_req_seq;
    return y;
}

Order Converter::CThostFtdcOrderFieldToRust(CThostFtdcOrderField* x) {
    if (x == nullptr)
        return Order{.is_null = true};
    Order y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.order_ref = Converter::Gb2312ToRustString(x->OrderRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.order_price_type = x->OrderPriceType;
    y.direction = x->Direction;
    y.comb_offset_flag = Converter::Gb2312ToRustString(x->CombOffsetFlag);
    y.comb_hedge_flag = Converter::Gb2312ToRustString(x->CombHedgeFlag);
    y.limit_price = x->LimitPrice;
    y.volume_total_original = x->VolumeTotalOriginal;
    y.time_condition = x->TimeCondition;
    y.gtd_date = Converter::Gb2312ToRustString(x->GTDDate);
    y.volume_condition = x->VolumeCondition;
    y.min_volume = x->MinVolume;
    y.contingent_condition = x->ContingentCondition;
    y.stop_price = x->StopPrice;
    y.force_close_reason = x->ForceCloseReason;
    y.is_auto_suspend = x->IsAutoSuspend;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.request_id = x->RequestID;
    y.order_local_id = Converter::Gb2312ToRustString(x->OrderLocalID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.order_submit_status = x->OrderSubmitStatus;
    y.notify_sequence = x->NotifySequence;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.order_sys_id = Converter::Gb2312ToRustString(x->OrderSysID);
    y.order_source = x->OrderSource;
    y.order_status = x->OrderStatus;
    y.order_type = x->OrderType;
    y.volume_traded = x->VolumeTraded;
    y.volume_total = x->VolumeTotal;
    y.insert_date = Converter::Gb2312ToRustString(x->InsertDate);
    y.insert_time = Converter::Gb2312ToRustString(x->InsertTime);
    y.active_time = Converter::Gb2312ToRustString(x->ActiveTime);
    y.suspend_time = Converter::Gb2312ToRustString(x->SuspendTime);
    y.update_time = Converter::Gb2312ToRustString(x->UpdateTime);
    y.cancel_time = Converter::Gb2312ToRustString(x->CancelTime);
    y.active_trader_id = Converter::Gb2312ToRustString(x->ActiveTraderID);
    y.clearing_part_id = Converter::Gb2312ToRustString(x->ClearingPartID);
    y.sequence_no = x->SequenceNo;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.user_force_close = x->UserForceClose;
    y.active_user_id = Converter::Gb2312ToRustString(x->ActiveUserID);
    y.broker_order_seq = x->BrokerOrderSeq;
    y.relative_order_sys_id = Converter::Gb2312ToRustString(x->RelativeOrderSysID);
    y.zce_total_traded_volume = x->ZCETotalTradedVolume;
    y.is_swap_order = x->IsSwapOrder;
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.order_memo = Converter::Gb2312ToRustString(x->OrderMemo);
    y.session_req_seq = x->SessionReqSeq;
    return y;
}

CThostFtdcExchangeOrderField Converter::ExchangeOrderToCpp(ExchangeOrder x) {
    CThostFtdcExchangeOrderField y;
    memset(&y, 0, sizeof(y));
    y.OrderPriceType = x.order_price_type;
    y.Direction = x.direction;
    strcpy(y.CombOffsetFlag, x.comb_offset_flag.c_str());
    strcpy(y.CombHedgeFlag, x.comb_hedge_flag.c_str());
    y.LimitPrice = x.limit_price;
    y.VolumeTotalOriginal = x.volume_total_original;
    y.TimeCondition = x.time_condition;
    strcpy(y.GTDDate, x.gtd_date.c_str());
    y.VolumeCondition = x.volume_condition;
    y.MinVolume = x.min_volume;
    y.ContingentCondition = x.contingent_condition;
    y.StopPrice = x.stop_price;
    y.ForceCloseReason = x.force_close_reason;
    y.IsAutoSuspend = x.is_auto_suspend;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.RequestID = x.request_id;
    strcpy(y.OrderLocalID, x.order_local_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    y.OrderSubmitStatus = x.order_submit_status;
    y.NotifySequence = x.notify_sequence;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.OrderSysID, x.order_sys_id.c_str());
    y.OrderSource = x.order_source;
    y.OrderStatus = x.order_status;
    y.OrderType = x.order_type;
    y.VolumeTraded = x.volume_traded;
    y.VolumeTotal = x.volume_total;
    strcpy(y.InsertDate, x.insert_date.c_str());
    strcpy(y.InsertTime, x.insert_time.c_str());
    strcpy(y.ActiveTime, x.active_time.c_str());
    strcpy(y.SuspendTime, x.suspend_time.c_str());
    strcpy(y.UpdateTime, x.update_time.c_str());
    strcpy(y.CancelTime, x.cancel_time.c_str());
    strcpy(y.ActiveTraderID, x.active_trader_id.c_str());
    strcpy(y.ClearingPartID, x.clearing_part_id.c_str());
    y.SequenceNo = x.sequence_no;
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ExchangeOrder Converter::CThostFtdcExchangeOrderFieldToRust(CThostFtdcExchangeOrderField* x) {
    if (x == nullptr)
        return ExchangeOrder{.is_null = true};
    ExchangeOrder y{};
    y.order_price_type = x->OrderPriceType;
    y.direction = x->Direction;
    y.comb_offset_flag = Converter::Gb2312ToRustString(x->CombOffsetFlag);
    y.comb_hedge_flag = Converter::Gb2312ToRustString(x->CombHedgeFlag);
    y.limit_price = x->LimitPrice;
    y.volume_total_original = x->VolumeTotalOriginal;
    y.time_condition = x->TimeCondition;
    y.gtd_date = Converter::Gb2312ToRustString(x->GTDDate);
    y.volume_condition = x->VolumeCondition;
    y.min_volume = x->MinVolume;
    y.contingent_condition = x->ContingentCondition;
    y.stop_price = x->StopPrice;
    y.force_close_reason = x->ForceCloseReason;
    y.is_auto_suspend = x->IsAutoSuspend;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.request_id = x->RequestID;
    y.order_local_id = Converter::Gb2312ToRustString(x->OrderLocalID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.order_submit_status = x->OrderSubmitStatus;
    y.notify_sequence = x->NotifySequence;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.order_sys_id = Converter::Gb2312ToRustString(x->OrderSysID);
    y.order_source = x->OrderSource;
    y.order_status = x->OrderStatus;
    y.order_type = x->OrderType;
    y.volume_traded = x->VolumeTraded;
    y.volume_total = x->VolumeTotal;
    y.insert_date = Converter::Gb2312ToRustString(x->InsertDate);
    y.insert_time = Converter::Gb2312ToRustString(x->InsertTime);
    y.active_time = Converter::Gb2312ToRustString(x->ActiveTime);
    y.suspend_time = Converter::Gb2312ToRustString(x->SuspendTime);
    y.update_time = Converter::Gb2312ToRustString(x->UpdateTime);
    y.cancel_time = Converter::Gb2312ToRustString(x->CancelTime);
    y.active_trader_id = Converter::Gb2312ToRustString(x->ActiveTraderID);
    y.clearing_part_id = Converter::Gb2312ToRustString(x->ClearingPartID);
    y.sequence_no = x->SequenceNo;
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcExchangeOrderInsertErrorField Converter::ExchangeOrderInsertErrorToCpp(ExchangeOrderInsertError x) {
    CThostFtdcExchangeOrderInsertErrorField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.OrderLocalID, x.order_local_id.c_str());
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    return y;
}

ExchangeOrderInsertError Converter::CThostFtdcExchangeOrderInsertErrorFieldToRust(CThostFtdcExchangeOrderInsertErrorField* x) {
    if (x == nullptr)
        return ExchangeOrderInsertError{.is_null = true};
    ExchangeOrderInsertError y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.order_local_id = Converter::Gb2312ToRustString(x->OrderLocalID);
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    return y;
}

CThostFtdcInputOrderActionField Converter::InputOrderActionToCpp(InputOrderAction x) {
    CThostFtdcInputOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OrderActionRef = x.order_action_ref;
    strcpy(y.OrderRef, x.order_ref.c_str());
    y.RequestID = x.request_id;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.OrderSysID, x.order_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    y.LimitPrice = x.limit_price;
    y.VolumeChange = x.volume_change;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.OrderMemo, x.order_memo.c_str());
    y.SessionReqSeq = x.session_req_seq;
    return y;
}

InputOrderAction Converter::CThostFtdcInputOrderActionFieldToRust(CThostFtdcInputOrderActionField* x) {
    if (x == nullptr)
        return InputOrderAction{.is_null = true};
    InputOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.order_action_ref = x->OrderActionRef;
    y.order_ref = Converter::Gb2312ToRustString(x->OrderRef);
    y.request_id = x->RequestID;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.order_sys_id = Converter::Gb2312ToRustString(x->OrderSysID);
    y.action_flag = x->ActionFlag;
    y.limit_price = x->LimitPrice;
    y.volume_change = x->VolumeChange;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.order_memo = Converter::Gb2312ToRustString(x->OrderMemo);
    y.session_req_seq = x->SessionReqSeq;
    return y;
}

CThostFtdcOrderActionField Converter::OrderActionToCpp(OrderAction x) {
    CThostFtdcOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OrderActionRef = x.order_action_ref;
    strcpy(y.OrderRef, x.order_ref.c_str());
    y.RequestID = x.request_id;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.OrderSysID, x.order_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    y.LimitPrice = x.limit_price;
    y.VolumeChange = x.volume_change;
    strcpy(y.ActionDate, x.action_date.c_str());
    strcpy(y.ActionTime, x.action_time.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.OrderLocalID, x.order_local_id.c_str());
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OrderActionStatus = x.order_action_status;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.StatusMsg, x.status_msg.c_str());
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.OrderMemo, x.order_memo.c_str());
    y.SessionReqSeq = x.session_req_seq;
    return y;
}

OrderAction Converter::CThostFtdcOrderActionFieldToRust(CThostFtdcOrderActionField* x) {
    if (x == nullptr)
        return OrderAction{.is_null = true};
    OrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.order_action_ref = x->OrderActionRef;
    y.order_ref = Converter::Gb2312ToRustString(x->OrderRef);
    y.request_id = x->RequestID;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.order_sys_id = Converter::Gb2312ToRustString(x->OrderSysID);
    y.action_flag = x->ActionFlag;
    y.limit_price = x->LimitPrice;
    y.volume_change = x->VolumeChange;
    y.action_date = Converter::Gb2312ToRustString(x->ActionDate);
    y.action_time = Converter::Gb2312ToRustString(x->ActionTime);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.order_local_id = Converter::Gb2312ToRustString(x->OrderLocalID);
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.order_action_status = x->OrderActionStatus;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.order_memo = Converter::Gb2312ToRustString(x->OrderMemo);
    y.session_req_seq = x->SessionReqSeq;
    return y;
}

CThostFtdcExchangeOrderActionField Converter::ExchangeOrderActionToCpp(ExchangeOrderAction x) {
    CThostFtdcExchangeOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.OrderSysID, x.order_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    y.LimitPrice = x.limit_price;
    y.VolumeChange = x.volume_change;
    strcpy(y.ActionDate, x.action_date.c_str());
    strcpy(y.ActionTime, x.action_time.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.OrderLocalID, x.order_local_id.c_str());
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OrderActionStatus = x.order_action_status;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ExchangeOrderAction Converter::CThostFtdcExchangeOrderActionFieldToRust(CThostFtdcExchangeOrderActionField* x) {
    if (x == nullptr)
        return ExchangeOrderAction{.is_null = true};
    ExchangeOrderAction y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.order_sys_id = Converter::Gb2312ToRustString(x->OrderSysID);
    y.action_flag = x->ActionFlag;
    y.limit_price = x->LimitPrice;
    y.volume_change = x->VolumeChange;
    y.action_date = Converter::Gb2312ToRustString(x->ActionDate);
    y.action_time = Converter::Gb2312ToRustString(x->ActionTime);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.order_local_id = Converter::Gb2312ToRustString(x->OrderLocalID);
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.order_action_status = x->OrderActionStatus;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcExchangeOrderActionErrorField Converter::ExchangeOrderActionErrorToCpp(ExchangeOrderActionError x) {
    CThostFtdcExchangeOrderActionErrorField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.OrderSysID, x.order_sys_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.OrderLocalID, x.order_local_id.c_str());
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    return y;
}

ExchangeOrderActionError Converter::CThostFtdcExchangeOrderActionErrorFieldToRust(CThostFtdcExchangeOrderActionErrorField* x) {
    if (x == nullptr)
        return ExchangeOrderActionError{.is_null = true};
    ExchangeOrderActionError y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.order_sys_id = Converter::Gb2312ToRustString(x->OrderSysID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.order_local_id = Converter::Gb2312ToRustString(x->OrderLocalID);
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    return y;
}

CThostFtdcExchangeTradeField Converter::ExchangeTradeToCpp(ExchangeTrade x) {
    CThostFtdcExchangeTradeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TradeID, x.trade_id.c_str());
    y.Direction = x.direction;
    strcpy(y.OrderSysID, x.order_sys_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    y.TradingRole = x.trading_role;
    y.OffsetFlag = x.offset_flag;
    y.HedgeFlag = x.hedge_flag;
    y.Price = x.price;
    y.Volume = x.volume;
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    y.TradeType = x.trade_type;
    y.PriceSource = x.price_source;
    strcpy(y.TraderID, x.trader_id.c_str());
    strcpy(y.OrderLocalID, x.order_local_id.c_str());
    strcpy(y.ClearingPartID, x.clearing_part_id.c_str());
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.SequenceNo = x.sequence_no;
    y.TradeSource = x.trade_source;
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    return y;
}

ExchangeTrade Converter::CThostFtdcExchangeTradeFieldToRust(CThostFtdcExchangeTradeField* x) {
    if (x == nullptr)
        return ExchangeTrade{.is_null = true};
    ExchangeTrade y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trade_id = Converter::Gb2312ToRustString(x->TradeID);
    y.direction = x->Direction;
    y.order_sys_id = Converter::Gb2312ToRustString(x->OrderSysID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trading_role = x->TradingRole;
    y.offset_flag = x->OffsetFlag;
    y.hedge_flag = x->HedgeFlag;
    y.price = x->Price;
    y.volume = x->Volume;
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.trade_type = x->TradeType;
    y.price_source = x->PriceSource;
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.order_local_id = Converter::Gb2312ToRustString(x->OrderLocalID);
    y.clearing_part_id = Converter::Gb2312ToRustString(x->ClearingPartID);
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.sequence_no = x->SequenceNo;
    y.trade_source = x->TradeSource;
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    return y;
}

CThostFtdcTradeField Converter::TradeToCpp(Trade x) {
    CThostFtdcTradeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.OrderRef, x.order_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TradeID, x.trade_id.c_str());
    y.Direction = x.direction;
    strcpy(y.OrderSysID, x.order_sys_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    y.TradingRole = x.trading_role;
    y.OffsetFlag = x.offset_flag;
    y.HedgeFlag = x.hedge_flag;
    y.Price = x.price;
    y.Volume = x.volume;
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    y.TradeType = x.trade_type;
    y.PriceSource = x.price_source;
    strcpy(y.TraderID, x.trader_id.c_str());
    strcpy(y.OrderLocalID, x.order_local_id.c_str());
    strcpy(y.ClearingPartID, x.clearing_part_id.c_str());
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.SequenceNo = x.sequence_no;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    y.BrokerOrderSeq = x.broker_order_seq;
    y.TradeSource = x.trade_source;
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    return y;
}

Trade Converter::CThostFtdcTradeFieldToRust(CThostFtdcTradeField* x) {
    if (x == nullptr)
        return Trade{.is_null = true};
    Trade y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.order_ref = Converter::Gb2312ToRustString(x->OrderRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trade_id = Converter::Gb2312ToRustString(x->TradeID);
    y.direction = x->Direction;
    y.order_sys_id = Converter::Gb2312ToRustString(x->OrderSysID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trading_role = x->TradingRole;
    y.offset_flag = x->OffsetFlag;
    y.hedge_flag = x->HedgeFlag;
    y.price = x->Price;
    y.volume = x->Volume;
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.trade_type = x->TradeType;
    y.price_source = x->PriceSource;
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.order_local_id = Converter::Gb2312ToRustString(x->OrderLocalID);
    y.clearing_part_id = Converter::Gb2312ToRustString(x->ClearingPartID);
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.sequence_no = x->SequenceNo;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.broker_order_seq = x->BrokerOrderSeq;
    y.trade_source = x->TradeSource;
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    return y;
}

CThostFtdcUserSessionField Converter::UserSessionToCpp(UserSession x) {
    CThostFtdcUserSessionField y;
    memset(&y, 0, sizeof(y));
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.LoginDate, x.login_date.c_str());
    strcpy(y.LoginTime, x.login_time.c_str());
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.InterfaceProductInfo, x.interface_product_info.c_str());
    strcpy(y.ProtocolInfo, x.protocol_info.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.LoginRemark, x.login_remark.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

UserSession Converter::CThostFtdcUserSessionFieldToRust(CThostFtdcUserSessionField* x) {
    if (x == nullptr)
        return UserSession{.is_null = true};
    UserSession y{};
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.login_date = Converter::Gb2312ToRustString(x->LoginDate);
    y.login_time = Converter::Gb2312ToRustString(x->LoginTime);
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.interface_product_info = Converter::Gb2312ToRustString(x->InterfaceProductInfo);
    y.protocol_info = Converter::Gb2312ToRustString(x->ProtocolInfo);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.login_remark = Converter::Gb2312ToRustString(x->LoginRemark);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryMaxOrderVolumeField Converter::QryMaxOrderVolumeToCpp(QryMaxOrderVolume x) {
    CThostFtdcQryMaxOrderVolumeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.Direction = x.direction;
    y.OffsetFlag = x.offset_flag;
    y.HedgeFlag = x.hedge_flag;
    y.MaxVolume = x.max_volume;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryMaxOrderVolume Converter::CThostFtdcQryMaxOrderVolumeFieldToRust(CThostFtdcQryMaxOrderVolumeField* x) {
    if (x == nullptr)
        return QryMaxOrderVolume{.is_null = true};
    QryMaxOrderVolume y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.direction = x->Direction;
    y.offset_flag = x->OffsetFlag;
    y.hedge_flag = x->HedgeFlag;
    y.max_volume = x->MaxVolume;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcSettlementInfoConfirmField Converter::SettlementInfoConfirmToCpp(SettlementInfoConfirm x) {
    CThostFtdcSettlementInfoConfirmField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ConfirmDate, x.confirm_date.c_str());
    strcpy(y.ConfirmTime, x.confirm_time.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

SettlementInfoConfirm Converter::CThostFtdcSettlementInfoConfirmFieldToRust(CThostFtdcSettlementInfoConfirmField* x) {
    if (x == nullptr)
        return SettlementInfoConfirm{.is_null = true};
    SettlementInfoConfirm y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.confirm_date = Converter::Gb2312ToRustString(x->ConfirmDate);
    y.confirm_time = Converter::Gb2312ToRustString(x->ConfirmTime);
    y.settlement_id = x->SettlementID;
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcSyncDepositField Converter::SyncDepositToCpp(SyncDeposit x) {
    CThostFtdcSyncDepositField y;
    memset(&y, 0, sizeof(y));
    memcpy(y.DepositSeqNo, x.deposit_seq_no.data(), x.deposit_seq_no.size() * sizeof(uint8_t));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.Deposit = x.deposit;
    y.IsForce = x.is_force;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.IsFromSopt = x.is_from_sopt;
    strcpy(y.TradingPassword, x.trading_password.c_str());
    y.IsSecAgentTranfer = x.is_sec_agent_transfer;
    return y;
}

SyncDeposit Converter::CThostFtdcSyncDepositFieldToRust(CThostFtdcSyncDepositField* x) {
    if (x == nullptr)
        return SyncDeposit{.is_null = true};
    SyncDeposit y{};
    for (int i = 0; i < 15; i++)
        y.deposit_seq_no.push_back(x->DepositSeqNo[i]);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.deposit = x->Deposit;
    y.is_force = x->IsForce;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.is_from_sopt = x->IsFromSopt;
    y.trading_password = Converter::Gb2312ToRustString(x->TradingPassword);
    y.is_sec_agent_transfer = x->IsSecAgentTranfer;
    return y;
}

CThostFtdcSyncFundMortgageField Converter::SyncFundMortgageToCpp(SyncFundMortgage x) {
    CThostFtdcSyncFundMortgageField y;
    memset(&y, 0, sizeof(y));
    memcpy(y.MortgageSeqNo, x.mortgage_seq_no.data(), x.mortgage_seq_no.size() * sizeof(uint8_t));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.FromCurrencyID, x.from_currency_id.c_str());
    y.MortgageAmount = x.mortgage_amount;
    strcpy(y.ToCurrencyID, x.to_currency_id.c_str());
    return y;
}

SyncFundMortgage Converter::CThostFtdcSyncFundMortgageFieldToRust(CThostFtdcSyncFundMortgageField* x) {
    if (x == nullptr)
        return SyncFundMortgage{.is_null = true};
    SyncFundMortgage y{};
    for (int i = 0; i < 15; i++)
        y.mortgage_seq_no.push_back(x->MortgageSeqNo[i]);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.from_currency_id = Converter::Gb2312ToRustString(x->FromCurrencyID);
    y.mortgage_amount = x->MortgageAmount;
    y.to_currency_id = Converter::Gb2312ToRustString(x->ToCurrencyID);
    return y;
}

CThostFtdcBrokerSyncField Converter::BrokerSyncToCpp(BrokerSync x) {
    CThostFtdcBrokerSyncField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    return y;
}

BrokerSync Converter::CThostFtdcBrokerSyncFieldToRust(CThostFtdcBrokerSyncField* x) {
    if (x == nullptr)
        return BrokerSync{.is_null = true};
    BrokerSync y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    return y;
}

CThostFtdcSyncingInvestorField Converter::SyncingInvestorToCpp(SyncingInvestor x) {
    CThostFtdcSyncingInvestorField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorGroupID, x.investor_group_id.c_str());
    strcpy(y.InvestorName, x.investor_name.c_str());
    y.IdentifiedCardType = x.identified_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.IsActive = x.is_active;
    strcpy(y.Telephone, x.telephone.c_str());
    strcpy(y.Address, x.address.c_str());
    strcpy(y.OpenDate, x.open_date.c_str());
    strcpy(y.Mobile, x.mobile.c_str());
    strcpy(y.CommModelID, x.comm_model_id.c_str());
    strcpy(y.MarginModelID, x.margin_model_id.c_str());
    y.IsOrderFreq = x.is_order_freq;
    y.IsOpenVolLimit = x.is_open_vol_limit;
    return y;
}

SyncingInvestor Converter::CThostFtdcSyncingInvestorFieldToRust(CThostFtdcSyncingInvestorField* x) {
    if (x == nullptr)
        return SyncingInvestor{.is_null = true};
    SyncingInvestor y{};
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_group_id = Converter::Gb2312ToRustString(x->InvestorGroupID);
    y.investor_name = Converter::Gb2312ToRustString(x->InvestorName);
    y.identified_card_type = x->IdentifiedCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.is_active = x->IsActive;
    y.telephone = Converter::Gb2312ToRustString(x->Telephone);
    y.address = Converter::Gb2312ToRustString(x->Address);
    y.open_date = Converter::Gb2312ToRustString(x->OpenDate);
    y.mobile = Converter::Gb2312ToRustString(x->Mobile);
    y.comm_model_id = Converter::Gb2312ToRustString(x->CommModelID);
    y.margin_model_id = Converter::Gb2312ToRustString(x->MarginModelID);
    y.is_order_freq = x->IsOrderFreq;
    y.is_open_vol_limit = x->IsOpenVolLimit;
    return y;
}

CThostFtdcSyncingTradingCodeField Converter::SyncingTradingCodeToCpp(SyncingTradingCode x) {
    CThostFtdcSyncingTradingCodeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    y.IsActive = x.is_active;
    y.ClientIDType = x.client_idtype;
    return y;
}

SyncingTradingCode Converter::CThostFtdcSyncingTradingCodeFieldToRust(CThostFtdcSyncingTradingCodeField* x) {
    if (x == nullptr)
        return SyncingTradingCode{.is_null = true};
    SyncingTradingCode y{};
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.is_active = x->IsActive;
    y.client_idtype = x->ClientIDType;
    return y;
}

CThostFtdcSyncingInvestorGroupField Converter::SyncingInvestorGroupToCpp(SyncingInvestorGroup x) {
    CThostFtdcSyncingInvestorGroupField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorGroupID, x.investor_group_id.c_str());
    strcpy(y.InvestorGroupName, x.investor_group_name.c_str());
    return y;
}

SyncingInvestorGroup Converter::CThostFtdcSyncingInvestorGroupFieldToRust(CThostFtdcSyncingInvestorGroupField* x) {
    if (x == nullptr)
        return SyncingInvestorGroup{.is_null = true};
    SyncingInvestorGroup y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_group_id = Converter::Gb2312ToRustString(x->InvestorGroupID);
    y.investor_group_name = Converter::Gb2312ToRustString(x->InvestorGroupName);
    return y;
}

CThostFtdcSyncingTradingAccountField Converter::SyncingTradingAccountToCpp(SyncingTradingAccount x) {
    CThostFtdcSyncingTradingAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    y.PreMortgage = x.pre_mortgage;
    y.PreCredit = x.pre_credit;
    y.PreDeposit = x.pre_deposit;
    y.PreBalance = x.pre_balance;
    y.PreMargin = x.pre_margin;
    y.InterestBase = x.interest_base;
    y.Interest = x.interest;
    y.Deposit = x.deposit;
    y.Withdraw = x.withdraw;
    y.FrozenMargin = x.frozen_margin;
    y.FrozenCash = x.frozen_cash;
    y.FrozenCommission = x.frozen_commission;
    y.CurrMargin = x.curr_margin;
    y.CashIn = x.cash_in;
    y.Commission = x.commission;
    y.CloseProfit = x.close_profit;
    y.PositionProfit = x.position_profit;
    y.Balance = x.balance;
    y.Available = x.available;
    y.WithdrawQuota = x.withdraw_quota;
    y.Reserve = x.reserve;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    y.Credit = x.credit;
    y.Mortgage = x.mortgage;
    y.ExchangeMargin = x.exchange_margin;
    y.DeliveryMargin = x.delivery_margin;
    y.ExchangeDeliveryMargin = x.exchange_delivery_margin;
    y.ReserveBalance = x.reserve_balance;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.PreFundMortgageIn = x.pre_fund_mortgage_in;
    y.PreFundMortgageOut = x.pre_fund_mortgage_out;
    y.FundMortgageIn = x.fund_mortgage_in;
    y.FundMortgageOut = x.fund_mortgage_out;
    y.FundMortgageAvailable = x.fund_mortgage_available;
    y.MortgageableFund = x.mortgageable_fund;
    y.SpecProductMargin = x.spec_product_margin;
    y.SpecProductFrozenMargin = x.spec_product_frozen_margin;
    y.SpecProductCommission = x.spec_product_commission;
    y.SpecProductFrozenCommission = x.spec_product_frozen_commission;
    y.SpecProductPositionProfit = x.spec_product_position_profit;
    y.SpecProductCloseProfit = x.spec_product_close_profit;
    y.SpecProductPositionProfitByAlg = x.spec_product_position_profit_by_alg;
    y.SpecProductExchangeMargin = x.spec_product_exchange_margin;
    y.FrozenSwap = x.frozen_swap;
    y.RemainSwap = x.remain_swap;
    y.OptionValue = x.option_value;
    return y;
}

SyncingTradingAccount Converter::CThostFtdcSyncingTradingAccountFieldToRust(CThostFtdcSyncingTradingAccountField* x) {
    if (x == nullptr)
        return SyncingTradingAccount{.is_null = true};
    SyncingTradingAccount y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.pre_mortgage = x->PreMortgage;
    y.pre_credit = x->PreCredit;
    y.pre_deposit = x->PreDeposit;
    y.pre_balance = x->PreBalance;
    y.pre_margin = x->PreMargin;
    y.interest_base = x->InterestBase;
    y.interest = x->Interest;
    y.deposit = x->Deposit;
    y.withdraw = x->Withdraw;
    y.frozen_margin = x->FrozenMargin;
    y.frozen_cash = x->FrozenCash;
    y.frozen_commission = x->FrozenCommission;
    y.curr_margin = x->CurrMargin;
    y.cash_in = x->CashIn;
    y.commission = x->Commission;
    y.close_profit = x->CloseProfit;
    y.position_profit = x->PositionProfit;
    y.balance = x->Balance;
    y.available = x->Available;
    y.withdraw_quota = x->WithdrawQuota;
    y.reserve = x->Reserve;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.credit = x->Credit;
    y.mortgage = x->Mortgage;
    y.exchange_margin = x->ExchangeMargin;
    y.delivery_margin = x->DeliveryMargin;
    y.exchange_delivery_margin = x->ExchangeDeliveryMargin;
    y.reserve_balance = x->ReserveBalance;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.pre_fund_mortgage_in = x->PreFundMortgageIn;
    y.pre_fund_mortgage_out = x->PreFundMortgageOut;
    y.fund_mortgage_in = x->FundMortgageIn;
    y.fund_mortgage_out = x->FundMortgageOut;
    y.fund_mortgage_available = x->FundMortgageAvailable;
    y.mortgageable_fund = x->MortgageableFund;
    y.spec_product_margin = x->SpecProductMargin;
    y.spec_product_frozen_margin = x->SpecProductFrozenMargin;
    y.spec_product_commission = x->SpecProductCommission;
    y.spec_product_frozen_commission = x->SpecProductFrozenCommission;
    y.spec_product_position_profit = x->SpecProductPositionProfit;
    y.spec_product_close_profit = x->SpecProductCloseProfit;
    y.spec_product_position_profit_by_alg = x->SpecProductPositionProfitByAlg;
    y.spec_product_exchange_margin = x->SpecProductExchangeMargin;
    y.frozen_swap = x->FrozenSwap;
    y.remain_swap = x->RemainSwap;
    y.option_value = x->OptionValue;
    return y;
}

CThostFtdcSyncingInvestorPositionField Converter::SyncingInvestorPositionToCpp(SyncingInvestorPosition x) {
    CThostFtdcSyncingInvestorPositionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.PosiDirection = x.posi_direction;
    y.HedgeFlag = x.hedge_flag;
    y.PositionDate = x.position_date;
    y.YdPosition = x.yd_position;
    y.Position = x.position;
    y.LongFrozen = x.long_frozen;
    y.ShortFrozen = x.short_frozen;
    y.LongFrozenAmount = x.long_frozen_amount;
    y.ShortFrozenAmount = x.short_frozen_amount;
    y.OpenVolume = x.open_volume;
    y.CloseVolume = x.close_volume;
    y.OpenAmount = x.open_amount;
    y.CloseAmount = x.close_amount;
    y.PositionCost = x.position_cost;
    y.PreMargin = x.pre_margin;
    y.UseMargin = x.use_margin;
    y.FrozenMargin = x.frozen_margin;
    y.FrozenCash = x.frozen_cash;
    y.FrozenCommission = x.frozen_commission;
    y.CashIn = x.cash_in;
    y.Commission = x.commission;
    y.CloseProfit = x.close_profit;
    y.PositionProfit = x.position_profit;
    y.PreSettlementPrice = x.pre_settlement_price;
    y.SettlementPrice = x.settlement_price;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    y.OpenCost = x.open_cost;
    y.ExchangeMargin = x.exchange_margin;
    y.CombPosition = x.comb_position;
    y.CombLongFrozen = x.comb_long_frozen;
    y.CombShortFrozen = x.comb_short_frozen;
    y.CloseProfitByDate = x.close_profit_by_date;
    y.CloseProfitByTrade = x.close_profit_by_trade;
    y.TodayPosition = x.today_position;
    y.MarginRateByMoney = x.margin_rate_by_money;
    y.MarginRateByVolume = x.margin_rate_by_volume;
    y.StrikeFrozen = x.strike_frozen;
    y.StrikeFrozenAmount = x.strike_frozen_amount;
    y.AbandonFrozen = x.abandon_frozen;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.YdStrikeFrozen = x.yd_strike_frozen;
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    y.PositionCostOffset = x.position_cost_offset;
    y.TasPosition = x.tas_position;
    y.TasPositionCost = x.tas_position_cost;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

SyncingInvestorPosition Converter::CThostFtdcSyncingInvestorPositionFieldToRust(CThostFtdcSyncingInvestorPositionField* x) {
    if (x == nullptr)
        return SyncingInvestorPosition{.is_null = true};
    SyncingInvestorPosition y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.posi_direction = x->PosiDirection;
    y.hedge_flag = x->HedgeFlag;
    y.position_date = x->PositionDate;
    y.yd_position = x->YdPosition;
    y.position = x->Position;
    y.long_frozen = x->LongFrozen;
    y.short_frozen = x->ShortFrozen;
    y.long_frozen_amount = x->LongFrozenAmount;
    y.short_frozen_amount = x->ShortFrozenAmount;
    y.open_volume = x->OpenVolume;
    y.close_volume = x->CloseVolume;
    y.open_amount = x->OpenAmount;
    y.close_amount = x->CloseAmount;
    y.position_cost = x->PositionCost;
    y.pre_margin = x->PreMargin;
    y.use_margin = x->UseMargin;
    y.frozen_margin = x->FrozenMargin;
    y.frozen_cash = x->FrozenCash;
    y.frozen_commission = x->FrozenCommission;
    y.cash_in = x->CashIn;
    y.commission = x->Commission;
    y.close_profit = x->CloseProfit;
    y.position_profit = x->PositionProfit;
    y.pre_settlement_price = x->PreSettlementPrice;
    y.settlement_price = x->SettlementPrice;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.open_cost = x->OpenCost;
    y.exchange_margin = x->ExchangeMargin;
    y.comb_position = x->CombPosition;
    y.comb_long_frozen = x->CombLongFrozen;
    y.comb_short_frozen = x->CombShortFrozen;
    y.close_profit_by_date = x->CloseProfitByDate;
    y.close_profit_by_trade = x->CloseProfitByTrade;
    y.today_position = x->TodayPosition;
    y.margin_rate_by_money = x->MarginRateByMoney;
    y.margin_rate_by_volume = x->MarginRateByVolume;
    y.strike_frozen = x->StrikeFrozen;
    y.strike_frozen_amount = x->StrikeFrozenAmount;
    y.abandon_frozen = x->AbandonFrozen;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.yd_strike_frozen = x->YdStrikeFrozen;
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.position_cost_offset = x->PositionCostOffset;
    y.tas_position = x->TasPosition;
    y.tas_position_cost = x->TasPositionCost;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcSyncingInstrumentMarginRateField Converter::SyncingInstrumentMarginRateToCpp(SyncingInstrumentMarginRate x) {
    CThostFtdcSyncingInstrumentMarginRateField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.LongMarginRatioByMoney = x.long_margin_ratio_by_money;
    y.LongMarginRatioByVolume = x.long_margin_ratio_by_volume;
    y.ShortMarginRatioByMoney = x.short_margin_ratio_by_money;
    y.ShortMarginRatioByVolume = x.short_margin_ratio_by_volume;
    y.IsRelative = x.is_relative;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

SyncingInstrumentMarginRate Converter::CThostFtdcSyncingInstrumentMarginRateFieldToRust(CThostFtdcSyncingInstrumentMarginRateField* x) {
    if (x == nullptr)
        return SyncingInstrumentMarginRate{.is_null = true};
    SyncingInstrumentMarginRate y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.hedge_flag = x->HedgeFlag;
    y.long_margin_ratio_by_money = x->LongMarginRatioByMoney;
    y.long_margin_ratio_by_volume = x->LongMarginRatioByVolume;
    y.short_margin_ratio_by_money = x->ShortMarginRatioByMoney;
    y.short_margin_ratio_by_volume = x->ShortMarginRatioByVolume;
    y.is_relative = x->IsRelative;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcSyncingInstrumentCommissionRateField Converter::SyncingInstrumentCommissionRateToCpp(SyncingInstrumentCommissionRate x) {
    CThostFtdcSyncingInstrumentCommissionRateField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OpenRatioByMoney = x.open_ratio_by_money;
    y.OpenRatioByVolume = x.open_ratio_by_volume;
    y.CloseRatioByMoney = x.close_ratio_by_money;
    y.CloseRatioByVolume = x.close_ratio_by_volume;
    y.CloseTodayRatioByMoney = x.close_today_ratio_by_money;
    y.CloseTodayRatioByVolume = x.close_today_ratio_by_volume;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

SyncingInstrumentCommissionRate Converter::CThostFtdcSyncingInstrumentCommissionRateFieldToRust(CThostFtdcSyncingInstrumentCommissionRateField* x) {
    if (x == nullptr)
        return SyncingInstrumentCommissionRate{.is_null = true};
    SyncingInstrumentCommissionRate y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.open_ratio_by_money = x->OpenRatioByMoney;
    y.open_ratio_by_volume = x->OpenRatioByVolume;
    y.close_ratio_by_money = x->CloseRatioByMoney;
    y.close_ratio_by_volume = x->CloseRatioByVolume;
    y.close_today_ratio_by_money = x->CloseTodayRatioByMoney;
    y.close_today_ratio_by_volume = x->CloseTodayRatioByVolume;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcSyncingInstrumentTradingRightField Converter::SyncingInstrumentTradingRightToCpp(SyncingInstrumentTradingRight x) {
    CThostFtdcSyncingInstrumentTradingRightField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.TradingRight = x.trading_right;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

SyncingInstrumentTradingRight Converter::CThostFtdcSyncingInstrumentTradingRightFieldToRust(CThostFtdcSyncingInstrumentTradingRightField* x) {
    if (x == nullptr)
        return SyncingInstrumentTradingRight{.is_null = true};
    SyncingInstrumentTradingRight y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.trading_right = x->TradingRight;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryOrderField Converter::QryOrderToCpp(QryOrder x) {
    CThostFtdcQryOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.OrderSysID, x.order_sys_id.c_str());
    strcpy(y.InsertTimeStart, x.insert_time_start.c_str());
    strcpy(y.InsertTimeEnd, x.insert_time_end.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryOrder Converter::CThostFtdcQryOrderFieldToRust(CThostFtdcQryOrderField* x) {
    if (x == nullptr)
        return QryOrder{.is_null = true};
    QryOrder y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.order_sys_id = Converter::Gb2312ToRustString(x->OrderSysID);
    y.insert_time_start = Converter::Gb2312ToRustString(x->InsertTimeStart);
    y.insert_time_end = Converter::Gb2312ToRustString(x->InsertTimeEnd);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryTradeField Converter::QryTradeToCpp(QryTrade x) {
    CThostFtdcQryTradeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TradeID, x.trade_id.c_str());
    strcpy(y.TradeTimeStart, x.trade_time_start.c_str());
    strcpy(y.TradeTimeEnd, x.trade_time_end.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryTrade Converter::CThostFtdcQryTradeFieldToRust(CThostFtdcQryTradeField* x) {
    if (x == nullptr)
        return QryTrade{.is_null = true};
    QryTrade y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trade_id = Converter::Gb2312ToRustString(x->TradeID);
    y.trade_time_start = Converter::Gb2312ToRustString(x->TradeTimeStart);
    y.trade_time_end = Converter::Gb2312ToRustString(x->TradeTimeEnd);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryInvestorPositionField Converter::QryInvestorPositionToCpp(QryInvestorPosition x) {
    CThostFtdcQryInvestorPositionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryInvestorPosition Converter::CThostFtdcQryInvestorPositionFieldToRust(CThostFtdcQryInvestorPositionField* x) {
    if (x == nullptr)
        return QryInvestorPosition{.is_null = true};
    QryInvestorPosition y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryTradingAccountField Converter::QryTradingAccountToCpp(QryTradingAccount x) {
    CThostFtdcQryTradingAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.BizType = x.biz_type;
    strcpy(y.AccountID, x.account_id.c_str());
    return y;
}

QryTradingAccount Converter::CThostFtdcQryTradingAccountFieldToRust(CThostFtdcQryTradingAccountField* x) {
    if (x == nullptr)
        return QryTradingAccount{.is_null = true};
    QryTradingAccount y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.biz_type = x->BizType;
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    return y;
}

CThostFtdcQryInvestorField Converter::QryInvestorToCpp(QryInvestor x) {
    CThostFtdcQryInvestorField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    return y;
}

QryInvestor Converter::CThostFtdcQryInvestorFieldToRust(CThostFtdcQryInvestorField* x) {
    if (x == nullptr)
        return QryInvestor{.is_null = true};
    QryInvestor y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    return y;
}

CThostFtdcQryTradingCodeField Converter::QryTradingCodeToCpp(QryTradingCode x) {
    CThostFtdcQryTradingCodeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    y.ClientIDType = x.client_idtype;
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    return y;
}

QryTradingCode Converter::CThostFtdcQryTradingCodeFieldToRust(CThostFtdcQryTradingCodeField* x) {
    if (x == nullptr)
        return QryTradingCode{.is_null = true};
    QryTradingCode y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.client_idtype = x->ClientIDType;
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    return y;
}

CThostFtdcQryInvestorGroupField Converter::QryInvestorGroupToCpp(QryInvestorGroup x) {
    CThostFtdcQryInvestorGroupField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    return y;
}

QryInvestorGroup Converter::CThostFtdcQryInvestorGroupFieldToRust(CThostFtdcQryInvestorGroupField* x) {
    if (x == nullptr)
        return QryInvestorGroup{.is_null = true};
    QryInvestorGroup y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    return y;
}

CThostFtdcQryInstrumentMarginRateField Converter::QryInstrumentMarginRateToCpp(QryInstrumentMarginRate x) {
    CThostFtdcQryInstrumentMarginRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryInstrumentMarginRate Converter::CThostFtdcQryInstrumentMarginRateFieldToRust(CThostFtdcQryInstrumentMarginRateField* x) {
    if (x == nullptr)
        return QryInstrumentMarginRate{.is_null = true};
    QryInstrumentMarginRate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.hedge_flag = x->HedgeFlag;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryInstrumentCommissionRateField Converter::QryInstrumentCommissionRateToCpp(QryInstrumentCommissionRate x) {
    CThostFtdcQryInstrumentCommissionRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryInstrumentCommissionRate Converter::CThostFtdcQryInstrumentCommissionRateFieldToRust(CThostFtdcQryInstrumentCommissionRateField* x) {
    if (x == nullptr)
        return QryInstrumentCommissionRate{.is_null = true};
    QryInstrumentCommissionRate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryInstrumentTradingRightField Converter::QryInstrumentTradingRightToCpp(QryInstrumentTradingRight x) {
    CThostFtdcQryInstrumentTradingRightField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryInstrumentTradingRight Converter::CThostFtdcQryInstrumentTradingRightFieldToRust(CThostFtdcQryInstrumentTradingRightField* x) {
    if (x == nullptr)
        return QryInstrumentTradingRight{.is_null = true};
    QryInstrumentTradingRight y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryBrokerField Converter::QryBrokerToCpp(QryBroker x) {
    CThostFtdcQryBrokerField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    return y;
}

QryBroker Converter::CThostFtdcQryBrokerFieldToRust(CThostFtdcQryBrokerField* x) {
    if (x == nullptr)
        return QryBroker{.is_null = true};
    QryBroker y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    return y;
}

CThostFtdcQryTraderField Converter::QryTraderToCpp(QryTrader x) {
    CThostFtdcQryTraderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    return y;
}

QryTrader Converter::CThostFtdcQryTraderFieldToRust(CThostFtdcQryTraderField* x) {
    if (x == nullptr)
        return QryTrader{.is_null = true};
    QryTrader y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    return y;
}

CThostFtdcQrySuperUserFunctionField Converter::QrySuperUserFunctionToCpp(QrySuperUserFunction x) {
    CThostFtdcQrySuperUserFunctionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.UserID, x.user_id.c_str());
    return y;
}

QrySuperUserFunction Converter::CThostFtdcQrySuperUserFunctionFieldToRust(CThostFtdcQrySuperUserFunctionField* x) {
    if (x == nullptr)
        return QrySuperUserFunction{.is_null = true};
    QrySuperUserFunction y{};
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    return y;
}

CThostFtdcQryUserSessionField Converter::QryUserSessionToCpp(QryUserSession x) {
    CThostFtdcQryUserSessionField y;
    memset(&y, 0, sizeof(y));
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    return y;
}

QryUserSession Converter::CThostFtdcQryUserSessionFieldToRust(CThostFtdcQryUserSessionField* x) {
    if (x == nullptr)
        return QryUserSession{.is_null = true};
    QryUserSession y{};
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    return y;
}

CThostFtdcQryPartBrokerField Converter::QryPartBrokerToCpp(QryPartBroker x) {
    CThostFtdcQryPartBrokerField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    return y;
}

QryPartBroker Converter::CThostFtdcQryPartBrokerFieldToRust(CThostFtdcQryPartBrokerField* x) {
    if (x == nullptr)
        return QryPartBroker{.is_null = true};
    QryPartBroker y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    return y;
}

CThostFtdcQryFrontStatusField Converter::QryFrontStatusToCpp(QryFrontStatus x) {
    CThostFtdcQryFrontStatusField y;
    memset(&y, 0, sizeof(y));
    y.FrontID = x.front_id;
    return y;
}

QryFrontStatus Converter::CThostFtdcQryFrontStatusFieldToRust(CThostFtdcQryFrontStatusField* x) {
    if (x == nullptr)
        return QryFrontStatus{.is_null = true};
    QryFrontStatus y{};
    y.front_id = x->FrontID;
    return y;
}

CThostFtdcQryExchangeOrderField Converter::QryExchangeOrderToCpp(QryExchangeOrder x) {
    CThostFtdcQryExchangeOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    return y;
}

QryExchangeOrder Converter::CThostFtdcQryExchangeOrderFieldToRust(CThostFtdcQryExchangeOrderField* x) {
    if (x == nullptr)
        return QryExchangeOrder{.is_null = true};
    QryExchangeOrder y{};
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    return y;
}

CThostFtdcQryOrderActionField Converter::QryOrderActionToCpp(QryOrderAction x) {
    CThostFtdcQryOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    return y;
}

QryOrderAction Converter::CThostFtdcQryOrderActionFieldToRust(CThostFtdcQryOrderActionField* x) {
    if (x == nullptr)
        return QryOrderAction{.is_null = true};
    QryOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    return y;
}

CThostFtdcQryExchangeOrderActionField Converter::QryExchangeOrderActionToCpp(QryExchangeOrderAction x) {
    CThostFtdcQryExchangeOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    return y;
}

QryExchangeOrderAction Converter::CThostFtdcQryExchangeOrderActionFieldToRust(CThostFtdcQryExchangeOrderActionField* x) {
    if (x == nullptr)
        return QryExchangeOrderAction{.is_null = true};
    QryExchangeOrderAction y{};
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    return y;
}

CThostFtdcQrySuperUserField Converter::QrySuperUserToCpp(QrySuperUser x) {
    CThostFtdcQrySuperUserField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.UserID, x.user_id.c_str());
    return y;
}

QrySuperUser Converter::CThostFtdcQrySuperUserFieldToRust(CThostFtdcQrySuperUserField* x) {
    if (x == nullptr)
        return QrySuperUser{.is_null = true};
    QrySuperUser y{};
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    return y;
}

CThostFtdcQryExchangeField Converter::QryExchangeToCpp(QryExchange x) {
    CThostFtdcQryExchangeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    return y;
}

QryExchange Converter::CThostFtdcQryExchangeFieldToRust(CThostFtdcQryExchangeField* x) {
    if (x == nullptr)
        return QryExchange{.is_null = true};
    QryExchange y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    return y;
}

CThostFtdcQryProductField Converter::QryProductToCpp(QryProduct x) {
    CThostFtdcQryProductField y;
    memset(&y, 0, sizeof(y));
    y.ProductClass = x.product_class;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    return y;
}

QryProduct Converter::CThostFtdcQryProductFieldToRust(CThostFtdcQryProductField* x) {
    if (x == nullptr)
        return QryProduct{.is_null = true};
    QryProduct y{};
    y.product_class = x->ProductClass;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    return y;
}

CThostFtdcQryInstrumentField Converter::QryInstrumentToCpp(QryInstrument x) {
    CThostFtdcQryInstrumentField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    return y;
}

QryInstrument Converter::CThostFtdcQryInstrumentFieldToRust(CThostFtdcQryInstrumentField* x) {
    if (x == nullptr)
        return QryInstrument{.is_null = true};
    QryInstrument y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    return y;
}

CThostFtdcQryDepthMarketDataField Converter::QryDepthMarketDataToCpp(QryDepthMarketData x) {
    CThostFtdcQryDepthMarketDataField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.ProductClass = x.product_class;
    return y;
}

QryDepthMarketData Converter::CThostFtdcQryDepthMarketDataFieldToRust(CThostFtdcQryDepthMarketDataField* x) {
    if (x == nullptr)
        return QryDepthMarketData{.is_null = true};
    QryDepthMarketData y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.product_class = x->ProductClass;
    return y;
}

CThostFtdcQryBrokerUserField Converter::QryBrokerUserToCpp(QryBrokerUser x) {
    CThostFtdcQryBrokerUserField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    return y;
}

QryBrokerUser Converter::CThostFtdcQryBrokerUserFieldToRust(CThostFtdcQryBrokerUserField* x) {
    if (x == nullptr)
        return QryBrokerUser{.is_null = true};
    QryBrokerUser y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    return y;
}

CThostFtdcQryBrokerUserFunctionField Converter::QryBrokerUserFunctionToCpp(QryBrokerUserFunction x) {
    CThostFtdcQryBrokerUserFunctionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    return y;
}

QryBrokerUserFunction Converter::CThostFtdcQryBrokerUserFunctionFieldToRust(CThostFtdcQryBrokerUserFunctionField* x) {
    if (x == nullptr)
        return QryBrokerUserFunction{.is_null = true};
    QryBrokerUserFunction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    return y;
}

CThostFtdcQryTraderOfferField Converter::QryTraderOfferToCpp(QryTraderOffer x) {
    CThostFtdcQryTraderOfferField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    return y;
}

QryTraderOffer Converter::CThostFtdcQryTraderOfferFieldToRust(CThostFtdcQryTraderOfferField* x) {
    if (x == nullptr)
        return QryTraderOffer{.is_null = true};
    QryTraderOffer y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    return y;
}

CThostFtdcQrySyncDepositField Converter::QrySyncDepositToCpp(QrySyncDeposit x) {
    CThostFtdcQrySyncDepositField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    memcpy(y.DepositSeqNo, x.deposit_seq_no.data(), x.deposit_seq_no.size() * sizeof(uint8_t));
    return y;
}

QrySyncDeposit Converter::CThostFtdcQrySyncDepositFieldToRust(CThostFtdcQrySyncDepositField* x) {
    if (x == nullptr)
        return QrySyncDeposit{.is_null = true};
    QrySyncDeposit y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    for (int i = 0; i < 15; i++)
        y.deposit_seq_no.push_back(x->DepositSeqNo[i]);
    return y;
}

CThostFtdcQrySettlementInfoField Converter::QrySettlementInfoToCpp(QrySettlementInfo x) {
    CThostFtdcQrySettlementInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

QrySettlementInfo Converter::CThostFtdcQrySettlementInfoFieldToRust(CThostFtdcQrySettlementInfoField* x) {
    if (x == nullptr)
        return QrySettlementInfo{.is_null = true};
    QrySettlementInfo y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcQryExchangeMarginRateField Converter::QryExchangeMarginRateToCpp(QryExchangeMarginRate x) {
    CThostFtdcQryExchangeMarginRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryExchangeMarginRate Converter::CThostFtdcQryExchangeMarginRateFieldToRust(CThostFtdcQryExchangeMarginRateField* x) {
    if (x == nullptr)
        return QryExchangeMarginRate{.is_null = true};
    QryExchangeMarginRate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.hedge_flag = x->HedgeFlag;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryExchangeMarginRateAdjustField Converter::QryExchangeMarginRateAdjustToCpp(QryExchangeMarginRateAdjust x) {
    CThostFtdcQryExchangeMarginRateAdjustField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryExchangeMarginRateAdjust Converter::CThostFtdcQryExchangeMarginRateAdjustFieldToRust(CThostFtdcQryExchangeMarginRateAdjustField* x) {
    if (x == nullptr)
        return QryExchangeMarginRateAdjust{.is_null = true};
    QryExchangeMarginRateAdjust y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.hedge_flag = x->HedgeFlag;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryExchangeRateField Converter::QryExchangeRateToCpp(QryExchangeRate x) {
    CThostFtdcQryExchangeRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.FromCurrencyID, x.from_currency_id.c_str());
    strcpy(y.ToCurrencyID, x.to_currency_id.c_str());
    return y;
}

QryExchangeRate Converter::CThostFtdcQryExchangeRateFieldToRust(CThostFtdcQryExchangeRateField* x) {
    if (x == nullptr)
        return QryExchangeRate{.is_null = true};
    QryExchangeRate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.from_currency_id = Converter::Gb2312ToRustString(x->FromCurrencyID);
    y.to_currency_id = Converter::Gb2312ToRustString(x->ToCurrencyID);
    return y;
}

CThostFtdcQrySyncFundMortgageField Converter::QrySyncFundMortgageToCpp(QrySyncFundMortgage x) {
    CThostFtdcQrySyncFundMortgageField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    memcpy(y.MortgageSeqNo, x.mortgage_seq_no.data(), x.mortgage_seq_no.size() * sizeof(uint8_t));
    return y;
}

QrySyncFundMortgage Converter::CThostFtdcQrySyncFundMortgageFieldToRust(CThostFtdcQrySyncFundMortgageField* x) {
    if (x == nullptr)
        return QrySyncFundMortgage{.is_null = true};
    QrySyncFundMortgage y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    for (int i = 0; i < 15; i++)
        y.mortgage_seq_no.push_back(x->MortgageSeqNo[i]);
    return y;
}

CThostFtdcQryHisOrderField Converter::QryHisOrderToCpp(QryHisOrder x) {
    CThostFtdcQryHisOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.OrderSysID, x.order_sys_id.c_str());
    strcpy(y.InsertTimeStart, x.insert_time_start.c_str());
    strcpy(y.InsertTimeEnd, x.insert_time_end.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryHisOrder Converter::CThostFtdcQryHisOrderFieldToRust(CThostFtdcQryHisOrderField* x) {
    if (x == nullptr)
        return QryHisOrder{.is_null = true};
    QryHisOrder y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.order_sys_id = Converter::Gb2312ToRustString(x->OrderSysID);
    y.insert_time_start = Converter::Gb2312ToRustString(x->InsertTimeStart);
    y.insert_time_end = Converter::Gb2312ToRustString(x->InsertTimeEnd);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcOptionInstrMiniMarginField Converter::OptionInstrMiniMarginToCpp(OptionInstrMiniMargin x) {
    CThostFtdcOptionInstrMiniMarginField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.MinMargin = x.min_margin;
    y.ValueMethod = x.value_method;
    y.IsRelative = x.is_relative;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

OptionInstrMiniMargin Converter::CThostFtdcOptionInstrMiniMarginFieldToRust(CThostFtdcOptionInstrMiniMarginField* x) {
    if (x == nullptr)
        return OptionInstrMiniMargin{.is_null = true};
    OptionInstrMiniMargin y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.min_margin = x->MinMargin;
    y.value_method = x->ValueMethod;
    y.is_relative = x->IsRelative;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcOptionInstrMarginAdjustField Converter::OptionInstrMarginAdjustToCpp(OptionInstrMarginAdjust x) {
    CThostFtdcOptionInstrMarginAdjustField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.SShortMarginRatioByMoney = x.s_short_margin_ratio_by_money;
    y.SShortMarginRatioByVolume = x.s_short_margin_ratio_by_volume;
    y.HShortMarginRatioByMoney = x.h_short_margin_ratio_by_money;
    y.HShortMarginRatioByVolume = x.h_short_margin_ratio_by_volume;
    y.AShortMarginRatioByMoney = x.a_short_margin_ratio_by_money;
    y.AShortMarginRatioByVolume = x.a_short_margin_ratio_by_volume;
    y.IsRelative = x.is_relative;
    y.MShortMarginRatioByMoney = x.m_short_margin_ratio_by_money;
    y.MShortMarginRatioByVolume = x.m_short_margin_ratio_by_volume;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

OptionInstrMarginAdjust Converter::CThostFtdcOptionInstrMarginAdjustFieldToRust(CThostFtdcOptionInstrMarginAdjustField* x) {
    if (x == nullptr)
        return OptionInstrMarginAdjust{.is_null = true};
    OptionInstrMarginAdjust y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.s_short_margin_ratio_by_money = x->SShortMarginRatioByMoney;
    y.s_short_margin_ratio_by_volume = x->SShortMarginRatioByVolume;
    y.h_short_margin_ratio_by_money = x->HShortMarginRatioByMoney;
    y.h_short_margin_ratio_by_volume = x->HShortMarginRatioByVolume;
    y.a_short_margin_ratio_by_money = x->AShortMarginRatioByMoney;
    y.a_short_margin_ratio_by_volume = x->AShortMarginRatioByVolume;
    y.is_relative = x->IsRelative;
    y.m_short_margin_ratio_by_money = x->MShortMarginRatioByMoney;
    y.m_short_margin_ratio_by_volume = x->MShortMarginRatioByVolume;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcOptionInstrCommRateField Converter::OptionInstrCommRateToCpp(OptionInstrCommRate x) {
    CThostFtdcOptionInstrCommRateField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OpenRatioByMoney = x.open_ratio_by_money;
    y.OpenRatioByVolume = x.open_ratio_by_volume;
    y.CloseRatioByMoney = x.close_ratio_by_money;
    y.CloseRatioByVolume = x.close_ratio_by_volume;
    y.CloseTodayRatioByMoney = x.close_today_ratio_by_money;
    y.CloseTodayRatioByVolume = x.close_today_ratio_by_volume;
    y.StrikeRatioByMoney = x.strike_ratio_by_money;
    y.StrikeRatioByVolume = x.strike_ratio_by_volume;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

OptionInstrCommRate Converter::CThostFtdcOptionInstrCommRateFieldToRust(CThostFtdcOptionInstrCommRateField* x) {
    if (x == nullptr)
        return OptionInstrCommRate{.is_null = true};
    OptionInstrCommRate y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.open_ratio_by_money = x->OpenRatioByMoney;
    y.open_ratio_by_volume = x->OpenRatioByVolume;
    y.close_ratio_by_money = x->CloseRatioByMoney;
    y.close_ratio_by_volume = x->CloseRatioByVolume;
    y.close_today_ratio_by_money = x->CloseTodayRatioByMoney;
    y.close_today_ratio_by_volume = x->CloseTodayRatioByVolume;
    y.strike_ratio_by_money = x->StrikeRatioByMoney;
    y.strike_ratio_by_volume = x->StrikeRatioByVolume;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcOptionInstrTradeCostField Converter::OptionInstrTradeCostToCpp(OptionInstrTradeCost x) {
    CThostFtdcOptionInstrTradeCostField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.FixedMargin = x.fixed_margin;
    y.MiniMargin = x.mini_margin;
    y.Royalty = x.royalty;
    y.ExchFixedMargin = x.exch_fixed_margin;
    y.ExchMiniMargin = x.exch_mini_margin;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

OptionInstrTradeCost Converter::CThostFtdcOptionInstrTradeCostFieldToRust(CThostFtdcOptionInstrTradeCostField* x) {
    if (x == nullptr)
        return OptionInstrTradeCost{.is_null = true};
    OptionInstrTradeCost y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.hedge_flag = x->HedgeFlag;
    y.fixed_margin = x->FixedMargin;
    y.mini_margin = x->MiniMargin;
    y.royalty = x->Royalty;
    y.exch_fixed_margin = x->ExchFixedMargin;
    y.exch_mini_margin = x->ExchMiniMargin;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryOptionInstrTradeCostField Converter::QryOptionInstrTradeCostToCpp(QryOptionInstrTradeCost x) {
    CThostFtdcQryOptionInstrTradeCostField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.InputPrice = x.input_price;
    y.UnderlyingPrice = x.underlying_price;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryOptionInstrTradeCost Converter::CThostFtdcQryOptionInstrTradeCostFieldToRust(CThostFtdcQryOptionInstrTradeCostField* x) {
    if (x == nullptr)
        return QryOptionInstrTradeCost{.is_null = true};
    QryOptionInstrTradeCost y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.hedge_flag = x->HedgeFlag;
    y.input_price = x->InputPrice;
    y.underlying_price = x->UnderlyingPrice;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryOptionInstrCommRateField Converter::QryOptionInstrCommRateToCpp(QryOptionInstrCommRate x) {
    CThostFtdcQryOptionInstrCommRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryOptionInstrCommRate Converter::CThostFtdcQryOptionInstrCommRateFieldToRust(CThostFtdcQryOptionInstrCommRateField* x) {
    if (x == nullptr)
        return QryOptionInstrCommRate{.is_null = true};
    QryOptionInstrCommRate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcIndexPriceField Converter::IndexPriceToCpp(IndexPrice x) {
    CThostFtdcIndexPriceField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    y.ClosePrice = x.close_price;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

IndexPrice Converter::CThostFtdcIndexPriceFieldToRust(CThostFtdcIndexPriceField* x) {
    if (x == nullptr)
        return IndexPrice{.is_null = true};
    IndexPrice y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.close_price = x->ClosePrice;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcInputExecOrderField Converter::InputExecOrderToCpp(InputExecOrder x) {
    CThostFtdcInputExecOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExecOrderRef, x.exec_order_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.Volume = x.volume;
    y.RequestID = x.request_id;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OffsetFlag = x.offset_flag;
    y.HedgeFlag = x.hedge_flag;
    y.ActionType = x.action_type;
    y.PosiDirection = x.posi_direction;
    y.ReservePositionFlag = x.reserve_position_flag;
    y.CloseFlag = x.close_flag;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

InputExecOrder Converter::CThostFtdcInputExecOrderFieldToRust(CThostFtdcInputExecOrderField* x) {
    if (x == nullptr)
        return InputExecOrder{.is_null = true};
    InputExecOrder y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exec_order_ref = Converter::Gb2312ToRustString(x->ExecOrderRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.volume = x->Volume;
    y.request_id = x->RequestID;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.offset_flag = x->OffsetFlag;
    y.hedge_flag = x->HedgeFlag;
    y.action_type = x->ActionType;
    y.posi_direction = x->PosiDirection;
    y.reserve_position_flag = x->ReservePositionFlag;
    y.close_flag = x->CloseFlag;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcInputExecOrderActionField Converter::InputExecOrderActionToCpp(InputExecOrderAction x) {
    CThostFtdcInputExecOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.ExecOrderActionRef = x.exec_order_action_ref;
    strcpy(y.ExecOrderRef, x.exec_order_ref.c_str());
    y.RequestID = x.request_id;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ExecOrderSysID, x.exec_order_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

InputExecOrderAction Converter::CThostFtdcInputExecOrderActionFieldToRust(CThostFtdcInputExecOrderActionField* x) {
    if (x == nullptr)
        return InputExecOrderAction{.is_null = true};
    InputExecOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exec_order_action_ref = x->ExecOrderActionRef;
    y.exec_order_ref = Converter::Gb2312ToRustString(x->ExecOrderRef);
    y.request_id = x->RequestID;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.exec_order_sys_id = Converter::Gb2312ToRustString(x->ExecOrderSysID);
    y.action_flag = x->ActionFlag;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcExecOrderField Converter::ExecOrderToCpp(ExecOrder x) {
    CThostFtdcExecOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExecOrderRef, x.exec_order_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.Volume = x.volume;
    y.RequestID = x.request_id;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OffsetFlag = x.offset_flag;
    y.HedgeFlag = x.hedge_flag;
    y.ActionType = x.action_type;
    y.PosiDirection = x.posi_direction;
    y.ReservePositionFlag = x.reserve_position_flag;
    y.CloseFlag = x.close_flag;
    strcpy(y.ExecOrderLocalID, x.exec_order_local_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    y.OrderSubmitStatus = x.order_submit_status;
    y.NotifySequence = x.notify_sequence;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.ExecOrderSysID, x.exec_order_sys_id.c_str());
    strcpy(y.InsertDate, x.insert_date.c_str());
    strcpy(y.InsertTime, x.insert_time.c_str());
    strcpy(y.CancelTime, x.cancel_time.c_str());
    y.ExecResult = x.exec_result;
    strcpy(y.ClearingPartID, x.clearing_part_id.c_str());
    y.SequenceNo = x.sequence_no;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.StatusMsg, x.status_msg.c_str());
    strcpy(y.ActiveUserID, x.active_user_id.c_str());
    y.BrokerExecOrderSeq = x.broker_exec_order_seq;
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ExecOrder Converter::CThostFtdcExecOrderFieldToRust(CThostFtdcExecOrderField* x) {
    if (x == nullptr)
        return ExecOrder{.is_null = true};
    ExecOrder y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exec_order_ref = Converter::Gb2312ToRustString(x->ExecOrderRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.volume = x->Volume;
    y.request_id = x->RequestID;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.offset_flag = x->OffsetFlag;
    y.hedge_flag = x->HedgeFlag;
    y.action_type = x->ActionType;
    y.posi_direction = x->PosiDirection;
    y.reserve_position_flag = x->ReservePositionFlag;
    y.close_flag = x->CloseFlag;
    y.exec_order_local_id = Converter::Gb2312ToRustString(x->ExecOrderLocalID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.order_submit_status = x->OrderSubmitStatus;
    y.notify_sequence = x->NotifySequence;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.exec_order_sys_id = Converter::Gb2312ToRustString(x->ExecOrderSysID);
    y.insert_date = Converter::Gb2312ToRustString(x->InsertDate);
    y.insert_time = Converter::Gb2312ToRustString(x->InsertTime);
    y.cancel_time = Converter::Gb2312ToRustString(x->CancelTime);
    y.exec_result = x->ExecResult;
    y.clearing_part_id = Converter::Gb2312ToRustString(x->ClearingPartID);
    y.sequence_no = x->SequenceNo;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.active_user_id = Converter::Gb2312ToRustString(x->ActiveUserID);
    y.broker_exec_order_seq = x->BrokerExecOrderSeq;
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcExecOrderActionField Converter::ExecOrderActionToCpp(ExecOrderAction x) {
    CThostFtdcExecOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.ExecOrderActionRef = x.exec_order_action_ref;
    strcpy(y.ExecOrderRef, x.exec_order_ref.c_str());
    y.RequestID = x.request_id;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ExecOrderSysID, x.exec_order_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    strcpy(y.ActionDate, x.action_date.c_str());
    strcpy(y.ActionTime, x.action_time.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.ExecOrderLocalID, x.exec_order_local_id.c_str());
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OrderActionStatus = x.order_action_status;
    strcpy(y.UserID, x.user_id.c_str());
    y.ActionType = x.action_type;
    strcpy(y.StatusMsg, x.status_msg.c_str());
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ExecOrderAction Converter::CThostFtdcExecOrderActionFieldToRust(CThostFtdcExecOrderActionField* x) {
    if (x == nullptr)
        return ExecOrderAction{.is_null = true};
    ExecOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exec_order_action_ref = x->ExecOrderActionRef;
    y.exec_order_ref = Converter::Gb2312ToRustString(x->ExecOrderRef);
    y.request_id = x->RequestID;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.exec_order_sys_id = Converter::Gb2312ToRustString(x->ExecOrderSysID);
    y.action_flag = x->ActionFlag;
    y.action_date = Converter::Gb2312ToRustString(x->ActionDate);
    y.action_time = Converter::Gb2312ToRustString(x->ActionTime);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.exec_order_local_id = Converter::Gb2312ToRustString(x->ExecOrderLocalID);
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.order_action_status = x->OrderActionStatus;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.action_type = x->ActionType;
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryExecOrderField Converter::QryExecOrderToCpp(QryExecOrder x) {
    CThostFtdcQryExecOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ExecOrderSysID, x.exec_order_sys_id.c_str());
    strcpy(y.InsertTimeStart, x.insert_time_start.c_str());
    strcpy(y.InsertTimeEnd, x.insert_time_end.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryExecOrder Converter::CThostFtdcQryExecOrderFieldToRust(CThostFtdcQryExecOrderField* x) {
    if (x == nullptr)
        return QryExecOrder{.is_null = true};
    QryExecOrder y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.exec_order_sys_id = Converter::Gb2312ToRustString(x->ExecOrderSysID);
    y.insert_time_start = Converter::Gb2312ToRustString(x->InsertTimeStart);
    y.insert_time_end = Converter::Gb2312ToRustString(x->InsertTimeEnd);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcExchangeExecOrderField Converter::ExchangeExecOrderToCpp(ExchangeExecOrder x) {
    CThostFtdcExchangeExecOrderField y;
    memset(&y, 0, sizeof(y));
    y.Volume = x.volume;
    y.RequestID = x.request_id;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OffsetFlag = x.offset_flag;
    y.HedgeFlag = x.hedge_flag;
    y.ActionType = x.action_type;
    y.PosiDirection = x.posi_direction;
    y.ReservePositionFlag = x.reserve_position_flag;
    y.CloseFlag = x.close_flag;
    strcpy(y.ExecOrderLocalID, x.exec_order_local_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    y.OrderSubmitStatus = x.order_submit_status;
    y.NotifySequence = x.notify_sequence;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.ExecOrderSysID, x.exec_order_sys_id.c_str());
    strcpy(y.InsertDate, x.insert_date.c_str());
    strcpy(y.InsertTime, x.insert_time.c_str());
    strcpy(y.CancelTime, x.cancel_time.c_str());
    y.ExecResult = x.exec_result;
    strcpy(y.ClearingPartID, x.clearing_part_id.c_str());
    y.SequenceNo = x.sequence_no;
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ExchangeExecOrder Converter::CThostFtdcExchangeExecOrderFieldToRust(CThostFtdcExchangeExecOrderField* x) {
    if (x == nullptr)
        return ExchangeExecOrder{.is_null = true};
    ExchangeExecOrder y{};
    y.volume = x->Volume;
    y.request_id = x->RequestID;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.offset_flag = x->OffsetFlag;
    y.hedge_flag = x->HedgeFlag;
    y.action_type = x->ActionType;
    y.posi_direction = x->PosiDirection;
    y.reserve_position_flag = x->ReservePositionFlag;
    y.close_flag = x->CloseFlag;
    y.exec_order_local_id = Converter::Gb2312ToRustString(x->ExecOrderLocalID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.order_submit_status = x->OrderSubmitStatus;
    y.notify_sequence = x->NotifySequence;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.exec_order_sys_id = Converter::Gb2312ToRustString(x->ExecOrderSysID);
    y.insert_date = Converter::Gb2312ToRustString(x->InsertDate);
    y.insert_time = Converter::Gb2312ToRustString(x->InsertTime);
    y.cancel_time = Converter::Gb2312ToRustString(x->CancelTime);
    y.exec_result = x->ExecResult;
    y.clearing_part_id = Converter::Gb2312ToRustString(x->ClearingPartID);
    y.sequence_no = x->SequenceNo;
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryExchangeExecOrderField Converter::QryExchangeExecOrderToCpp(QryExchangeExecOrder x) {
    CThostFtdcQryExchangeExecOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    return y;
}

QryExchangeExecOrder Converter::CThostFtdcQryExchangeExecOrderFieldToRust(CThostFtdcQryExchangeExecOrderField* x) {
    if (x == nullptr)
        return QryExchangeExecOrder{.is_null = true};
    QryExchangeExecOrder y{};
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    return y;
}

CThostFtdcQryExecOrderActionField Converter::QryExecOrderActionToCpp(QryExecOrderAction x) {
    CThostFtdcQryExecOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    return y;
}

QryExecOrderAction Converter::CThostFtdcQryExecOrderActionFieldToRust(CThostFtdcQryExecOrderActionField* x) {
    if (x == nullptr)
        return QryExecOrderAction{.is_null = true};
    QryExecOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    return y;
}

CThostFtdcExchangeExecOrderActionField Converter::ExchangeExecOrderActionToCpp(ExchangeExecOrderAction x) {
    CThostFtdcExchangeExecOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ExecOrderSysID, x.exec_order_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    strcpy(y.ActionDate, x.action_date.c_str());
    strcpy(y.ActionTime, x.action_time.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.ExecOrderLocalID, x.exec_order_local_id.c_str());
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OrderActionStatus = x.order_action_status;
    strcpy(y.UserID, x.user_id.c_str());
    y.ActionType = x.action_type;
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    y.Volume = x.volume;
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    return y;
}

ExchangeExecOrderAction Converter::CThostFtdcExchangeExecOrderActionFieldToRust(CThostFtdcExchangeExecOrderActionField* x) {
    if (x == nullptr)
        return ExchangeExecOrderAction{.is_null = true};
    ExchangeExecOrderAction y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.exec_order_sys_id = Converter::Gb2312ToRustString(x->ExecOrderSysID);
    y.action_flag = x->ActionFlag;
    y.action_date = Converter::Gb2312ToRustString(x->ActionDate);
    y.action_time = Converter::Gb2312ToRustString(x->ActionTime);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.exec_order_local_id = Converter::Gb2312ToRustString(x->ExecOrderLocalID);
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.order_action_status = x->OrderActionStatus;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.action_type = x->ActionType;
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.volume = x->Volume;
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    return y;
}

CThostFtdcQryExchangeExecOrderActionField Converter::QryExchangeExecOrderActionToCpp(QryExchangeExecOrderAction x) {
    CThostFtdcQryExchangeExecOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    return y;
}

QryExchangeExecOrderAction Converter::CThostFtdcQryExchangeExecOrderActionFieldToRust(CThostFtdcQryExchangeExecOrderActionField* x) {
    if (x == nullptr)
        return QryExchangeExecOrderAction{.is_null = true};
    QryExchangeExecOrderAction y{};
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    return y;
}

CThostFtdcErrExecOrderField Converter::ErrExecOrderToCpp(ErrExecOrder x) {
    CThostFtdcErrExecOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExecOrderRef, x.exec_order_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.Volume = x.volume;
    y.RequestID = x.request_id;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OffsetFlag = x.offset_flag;
    y.HedgeFlag = x.hedge_flag;
    y.ActionType = x.action_type;
    y.PosiDirection = x.posi_direction;
    y.ReservePositionFlag = x.reserve_position_flag;
    y.CloseFlag = x.close_flag;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ErrExecOrder Converter::CThostFtdcErrExecOrderFieldToRust(CThostFtdcErrExecOrderField* x) {
    if (x == nullptr)
        return ErrExecOrder{.is_null = true};
    ErrExecOrder y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exec_order_ref = Converter::Gb2312ToRustString(x->ExecOrderRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.volume = x->Volume;
    y.request_id = x->RequestID;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.offset_flag = x->OffsetFlag;
    y.hedge_flag = x->HedgeFlag;
    y.action_type = x->ActionType;
    y.posi_direction = x->PosiDirection;
    y.reserve_position_flag = x->ReservePositionFlag;
    y.close_flag = x->CloseFlag;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryErrExecOrderField Converter::QryErrExecOrderToCpp(QryErrExecOrder x) {
    CThostFtdcQryErrExecOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    return y;
}

QryErrExecOrder Converter::CThostFtdcQryErrExecOrderFieldToRust(CThostFtdcQryErrExecOrderField* x) {
    if (x == nullptr)
        return QryErrExecOrder{.is_null = true};
    QryErrExecOrder y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    return y;
}

CThostFtdcErrExecOrderActionField Converter::ErrExecOrderActionToCpp(ErrExecOrderAction x) {
    CThostFtdcErrExecOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.ExecOrderActionRef = x.exec_order_action_ref;
    strcpy(y.ExecOrderRef, x.exec_order_ref.c_str());
    y.RequestID = x.request_id;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ExecOrderSysID, x.exec_order_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ErrExecOrderAction Converter::CThostFtdcErrExecOrderActionFieldToRust(CThostFtdcErrExecOrderActionField* x) {
    if (x == nullptr)
        return ErrExecOrderAction{.is_null = true};
    ErrExecOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exec_order_action_ref = x->ExecOrderActionRef;
    y.exec_order_ref = Converter::Gb2312ToRustString(x->ExecOrderRef);
    y.request_id = x->RequestID;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.exec_order_sys_id = Converter::Gb2312ToRustString(x->ExecOrderSysID);
    y.action_flag = x->ActionFlag;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryErrExecOrderActionField Converter::QryErrExecOrderActionToCpp(QryErrExecOrderAction x) {
    CThostFtdcQryErrExecOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    return y;
}

QryErrExecOrderAction Converter::CThostFtdcQryErrExecOrderActionFieldToRust(CThostFtdcQryErrExecOrderActionField* x) {
    if (x == nullptr)
        return QryErrExecOrderAction{.is_null = true};
    QryErrExecOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    return y;
}

CThostFtdcOptionInstrTradingRightField Converter::OptionInstrTradingRightToCpp(OptionInstrTradingRight x) {
    CThostFtdcOptionInstrTradingRightField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.Direction = x.direction;
    y.TradingRight = x.trading_right;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

OptionInstrTradingRight Converter::CThostFtdcOptionInstrTradingRightFieldToRust(CThostFtdcOptionInstrTradingRightField* x) {
    if (x == nullptr)
        return OptionInstrTradingRight{.is_null = true};
    OptionInstrTradingRight y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.direction = x->Direction;
    y.trading_right = x->TradingRight;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryOptionInstrTradingRightField Converter::QryOptionInstrTradingRightToCpp(QryOptionInstrTradingRight x) {
    CThostFtdcQryOptionInstrTradingRightField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.Direction = x.direction;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryOptionInstrTradingRight Converter::CThostFtdcQryOptionInstrTradingRightFieldToRust(CThostFtdcQryOptionInstrTradingRightField* x) {
    if (x == nullptr)
        return QryOptionInstrTradingRight{.is_null = true};
    QryOptionInstrTradingRight y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.direction = x->Direction;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcInputForQuoteField Converter::InputForQuoteToCpp(InputForQuote x) {
    CThostFtdcInputForQuoteField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ForQuoteRef, x.for_quote_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

InputForQuote Converter::CThostFtdcInputForQuoteFieldToRust(CThostFtdcInputForQuoteField* x) {
    if (x == nullptr)
        return InputForQuote{.is_null = true};
    InputForQuote y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.for_quote_ref = Converter::Gb2312ToRustString(x->ForQuoteRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcForQuoteField Converter::ForQuoteToCpp(ForQuote x) {
    CThostFtdcForQuoteField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ForQuoteRef, x.for_quote_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.ForQuoteLocalID, x.for_quote_local_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.InsertDate, x.insert_date.c_str());
    strcpy(y.InsertTime, x.insert_time.c_str());
    y.ForQuoteStatus = x.for_quote_status;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.StatusMsg, x.status_msg.c_str());
    strcpy(y.ActiveUserID, x.active_user_id.c_str());
    y.BrokerForQutoSeq = x.broker_for_quote_seq;
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ForQuote Converter::CThostFtdcForQuoteFieldToRust(CThostFtdcForQuoteField* x) {
    if (x == nullptr)
        return ForQuote{.is_null = true};
    ForQuote y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.for_quote_ref = Converter::Gb2312ToRustString(x->ForQuoteRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.for_quote_local_id = Converter::Gb2312ToRustString(x->ForQuoteLocalID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.insert_date = Converter::Gb2312ToRustString(x->InsertDate);
    y.insert_time = Converter::Gb2312ToRustString(x->InsertTime);
    y.for_quote_status = x->ForQuoteStatus;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.active_user_id = Converter::Gb2312ToRustString(x->ActiveUserID);
    y.broker_for_quote_seq = x->BrokerForQutoSeq;
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryForQuoteField Converter::QryForQuoteToCpp(QryForQuote x) {
    CThostFtdcQryForQuoteField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InsertTimeStart, x.insert_time_start.c_str());
    strcpy(y.InsertTimeEnd, x.insert_time_end.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryForQuote Converter::CThostFtdcQryForQuoteFieldToRust(CThostFtdcQryForQuoteField* x) {
    if (x == nullptr)
        return QryForQuote{.is_null = true};
    QryForQuote y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.insert_time_start = Converter::Gb2312ToRustString(x->InsertTimeStart);
    y.insert_time_end = Converter::Gb2312ToRustString(x->InsertTimeEnd);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcExchangeForQuoteField Converter::ExchangeForQuoteToCpp(ExchangeForQuote x) {
    CThostFtdcExchangeForQuoteField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ForQuoteLocalID, x.for_quote_local_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.InsertDate, x.insert_date.c_str());
    strcpy(y.InsertTime, x.insert_time.c_str());
    y.ForQuoteStatus = x.for_quote_status;
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ExchangeForQuote Converter::CThostFtdcExchangeForQuoteFieldToRust(CThostFtdcExchangeForQuoteField* x) {
    if (x == nullptr)
        return ExchangeForQuote{.is_null = true};
    ExchangeForQuote y{};
    y.for_quote_local_id = Converter::Gb2312ToRustString(x->ForQuoteLocalID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.insert_date = Converter::Gb2312ToRustString(x->InsertDate);
    y.insert_time = Converter::Gb2312ToRustString(x->InsertTime);
    y.for_quote_status = x->ForQuoteStatus;
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryExchangeForQuoteField Converter::QryExchangeForQuoteToCpp(QryExchangeForQuote x) {
    CThostFtdcQryExchangeForQuoteField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    return y;
}

QryExchangeForQuote Converter::CThostFtdcQryExchangeForQuoteFieldToRust(CThostFtdcQryExchangeForQuoteField* x) {
    if (x == nullptr)
        return QryExchangeForQuote{.is_null = true};
    QryExchangeForQuote y{};
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    return y;
}

CThostFtdcInputQuoteField Converter::InputQuoteToCpp(InputQuote x) {
    CThostFtdcInputQuoteField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.QuoteRef, x.quote_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.AskPrice = x.ask_price;
    y.BidPrice = x.bid_price;
    y.AskVolume = x.ask_volume;
    y.BidVolume = x.bid_volume;
    y.RequestID = x.request_id;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.AskOffsetFlag = x.ask_offset_flag;
    y.BidOffsetFlag = x.bid_offset_flag;
    y.AskHedgeFlag = x.ask_hedge_flag;
    y.BidHedgeFlag = x.bid_hedge_flag;
    strcpy(y.AskOrderRef, x.ask_order_ref.c_str());
    strcpy(y.BidOrderRef, x.bid_order_ref.c_str());
    strcpy(y.ForQuoteSysID, x.for_quote_sys_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.ReplaceSysID, x.replace_sys_id.c_str());
    y.TimeCondition = x.time_condition;
    strcpy(y.OrderMemo, x.order_memo.c_str());
    y.SessionReqSeq = x.session_req_seq;
    return y;
}

InputQuote Converter::CThostFtdcInputQuoteFieldToRust(CThostFtdcInputQuoteField* x) {
    if (x == nullptr)
        return InputQuote{.is_null = true};
    InputQuote y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.quote_ref = Converter::Gb2312ToRustString(x->QuoteRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.ask_price = x->AskPrice;
    y.bid_price = x->BidPrice;
    y.ask_volume = x->AskVolume;
    y.bid_volume = x->BidVolume;
    y.request_id = x->RequestID;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.ask_offset_flag = x->AskOffsetFlag;
    y.bid_offset_flag = x->BidOffsetFlag;
    y.ask_hedge_flag = x->AskHedgeFlag;
    y.bid_hedge_flag = x->BidHedgeFlag;
    y.ask_order_ref = Converter::Gb2312ToRustString(x->AskOrderRef);
    y.bid_order_ref = Converter::Gb2312ToRustString(x->BidOrderRef);
    y.for_quote_sys_id = Converter::Gb2312ToRustString(x->ForQuoteSysID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.replace_sys_id = Converter::Gb2312ToRustString(x->ReplaceSysID);
    y.time_condition = x->TimeCondition;
    y.order_memo = Converter::Gb2312ToRustString(x->OrderMemo);
    y.session_req_seq = x->SessionReqSeq;
    return y;
}

CThostFtdcInputQuoteActionField Converter::InputQuoteActionToCpp(InputQuoteAction x) {
    CThostFtdcInputQuoteActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.QuoteActionRef = x.quote_action_ref;
    strcpy(y.QuoteRef, x.quote_ref.c_str());
    y.RequestID = x.request_id;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.QuoteSysID, x.quote_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.OrderMemo, x.order_memo.c_str());
    y.SessionReqSeq = x.session_req_seq;
    return y;
}

InputQuoteAction Converter::CThostFtdcInputQuoteActionFieldToRust(CThostFtdcInputQuoteActionField* x) {
    if (x == nullptr)
        return InputQuoteAction{.is_null = true};
    InputQuoteAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.quote_action_ref = x->QuoteActionRef;
    y.quote_ref = Converter::Gb2312ToRustString(x->QuoteRef);
    y.request_id = x->RequestID;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.quote_sys_id = Converter::Gb2312ToRustString(x->QuoteSysID);
    y.action_flag = x->ActionFlag;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.order_memo = Converter::Gb2312ToRustString(x->OrderMemo);
    y.session_req_seq = x->SessionReqSeq;
    return y;
}

CThostFtdcQuoteField Converter::QuoteToCpp(Quote x) {
    CThostFtdcQuoteField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.QuoteRef, x.quote_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.AskPrice = x.ask_price;
    y.BidPrice = x.bid_price;
    y.AskVolume = x.ask_volume;
    y.BidVolume = x.bid_volume;
    y.RequestID = x.request_id;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.AskOffsetFlag = x.ask_offset_flag;
    y.BidOffsetFlag = x.bid_offset_flag;
    y.AskHedgeFlag = x.ask_hedge_flag;
    y.BidHedgeFlag = x.bid_hedge_flag;
    strcpy(y.QuoteLocalID, x.quote_local_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    y.NotifySequence = x.notify_sequence;
    y.OrderSubmitStatus = x.order_submit_status;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.QuoteSysID, x.quote_sys_id.c_str());
    strcpy(y.InsertDate, x.insert_date.c_str());
    strcpy(y.InsertTime, x.insert_time.c_str());
    strcpy(y.CancelTime, x.cancel_time.c_str());
    y.QuoteStatus = x.quote_status;
    strcpy(y.ClearingPartID, x.clearing_part_id.c_str());
    y.SequenceNo = x.sequence_no;
    strcpy(y.AskOrderSysID, x.ask_order_sys_id.c_str());
    strcpy(y.BidOrderSysID, x.bid_order_sys_id.c_str());
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.StatusMsg, x.status_msg.c_str());
    strcpy(y.ActiveUserID, x.active_user_id.c_str());
    y.BrokerQuoteSeq = x.broker_quote_seq;
    strcpy(y.AskOrderRef, x.ask_order_ref.c_str());
    strcpy(y.BidOrderRef, x.bid_order_ref.c_str());
    strcpy(y.ForQuoteSysID, x.for_quote_sys_id.c_str());
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.ReplaceSysID, x.replace_sys_id.c_str());
    y.TimeCondition = x.time_condition;
    strcpy(y.OrderMemo, x.order_memo.c_str());
    y.SessionReqSeq = x.session_req_seq;
    return y;
}

Quote Converter::CThostFtdcQuoteFieldToRust(CThostFtdcQuoteField* x) {
    if (x == nullptr)
        return Quote{.is_null = true};
    Quote y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.quote_ref = Converter::Gb2312ToRustString(x->QuoteRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.ask_price = x->AskPrice;
    y.bid_price = x->BidPrice;
    y.ask_volume = x->AskVolume;
    y.bid_volume = x->BidVolume;
    y.request_id = x->RequestID;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.ask_offset_flag = x->AskOffsetFlag;
    y.bid_offset_flag = x->BidOffsetFlag;
    y.ask_hedge_flag = x->AskHedgeFlag;
    y.bid_hedge_flag = x->BidHedgeFlag;
    y.quote_local_id = Converter::Gb2312ToRustString(x->QuoteLocalID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.notify_sequence = x->NotifySequence;
    y.order_submit_status = x->OrderSubmitStatus;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.quote_sys_id = Converter::Gb2312ToRustString(x->QuoteSysID);
    y.insert_date = Converter::Gb2312ToRustString(x->InsertDate);
    y.insert_time = Converter::Gb2312ToRustString(x->InsertTime);
    y.cancel_time = Converter::Gb2312ToRustString(x->CancelTime);
    y.quote_status = x->QuoteStatus;
    y.clearing_part_id = Converter::Gb2312ToRustString(x->ClearingPartID);
    y.sequence_no = x->SequenceNo;
    y.ask_order_sys_id = Converter::Gb2312ToRustString(x->AskOrderSysID);
    y.bid_order_sys_id = Converter::Gb2312ToRustString(x->BidOrderSysID);
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.active_user_id = Converter::Gb2312ToRustString(x->ActiveUserID);
    y.broker_quote_seq = x->BrokerQuoteSeq;
    y.ask_order_ref = Converter::Gb2312ToRustString(x->AskOrderRef);
    y.bid_order_ref = Converter::Gb2312ToRustString(x->BidOrderRef);
    y.for_quote_sys_id = Converter::Gb2312ToRustString(x->ForQuoteSysID);
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.replace_sys_id = Converter::Gb2312ToRustString(x->ReplaceSysID);
    y.time_condition = x->TimeCondition;
    y.order_memo = Converter::Gb2312ToRustString(x->OrderMemo);
    y.session_req_seq = x->SessionReqSeq;
    return y;
}

CThostFtdcQuoteActionField Converter::QuoteActionToCpp(QuoteAction x) {
    CThostFtdcQuoteActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.QuoteActionRef = x.quote_action_ref;
    strcpy(y.QuoteRef, x.quote_ref.c_str());
    y.RequestID = x.request_id;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.QuoteSysID, x.quote_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    strcpy(y.ActionDate, x.action_date.c_str());
    strcpy(y.ActionTime, x.action_time.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.QuoteLocalID, x.quote_local_id.c_str());
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OrderActionStatus = x.order_action_status;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.StatusMsg, x.status_msg.c_str());
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.OrderMemo, x.order_memo.c_str());
    y.SessionReqSeq = x.session_req_seq;
    return y;
}

QuoteAction Converter::CThostFtdcQuoteActionFieldToRust(CThostFtdcQuoteActionField* x) {
    if (x == nullptr)
        return QuoteAction{.is_null = true};
    QuoteAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.quote_action_ref = x->QuoteActionRef;
    y.quote_ref = Converter::Gb2312ToRustString(x->QuoteRef);
    y.request_id = x->RequestID;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.quote_sys_id = Converter::Gb2312ToRustString(x->QuoteSysID);
    y.action_flag = x->ActionFlag;
    y.action_date = Converter::Gb2312ToRustString(x->ActionDate);
    y.action_time = Converter::Gb2312ToRustString(x->ActionTime);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.quote_local_id = Converter::Gb2312ToRustString(x->QuoteLocalID);
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.order_action_status = x->OrderActionStatus;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.order_memo = Converter::Gb2312ToRustString(x->OrderMemo);
    y.session_req_seq = x->SessionReqSeq;
    return y;
}

CThostFtdcQryQuoteField Converter::QryQuoteToCpp(QryQuote x) {
    CThostFtdcQryQuoteField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.QuoteSysID, x.quote_sys_id.c_str());
    strcpy(y.InsertTimeStart, x.insert_time_start.c_str());
    strcpy(y.InsertTimeEnd, x.insert_time_end.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryQuote Converter::CThostFtdcQryQuoteFieldToRust(CThostFtdcQryQuoteField* x) {
    if (x == nullptr)
        return QryQuote{.is_null = true};
    QryQuote y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.quote_sys_id = Converter::Gb2312ToRustString(x->QuoteSysID);
    y.insert_time_start = Converter::Gb2312ToRustString(x->InsertTimeStart);
    y.insert_time_end = Converter::Gb2312ToRustString(x->InsertTimeEnd);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcExchangeQuoteField Converter::ExchangeQuoteToCpp(ExchangeQuote x) {
    CThostFtdcExchangeQuoteField y;
    memset(&y, 0, sizeof(y));
    y.AskPrice = x.ask_price;
    y.BidPrice = x.bid_price;
    y.AskVolume = x.ask_volume;
    y.BidVolume = x.bid_volume;
    y.RequestID = x.request_id;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.AskOffsetFlag = x.ask_offset_flag;
    y.BidOffsetFlag = x.bid_offset_flag;
    y.AskHedgeFlag = x.ask_hedge_flag;
    y.BidHedgeFlag = x.bid_hedge_flag;
    strcpy(y.QuoteLocalID, x.quote_local_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    y.NotifySequence = x.notify_sequence;
    y.OrderSubmitStatus = x.order_submit_status;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.QuoteSysID, x.quote_sys_id.c_str());
    strcpy(y.InsertDate, x.insert_date.c_str());
    strcpy(y.InsertTime, x.insert_time.c_str());
    strcpy(y.CancelTime, x.cancel_time.c_str());
    y.QuoteStatus = x.quote_status;
    strcpy(y.ClearingPartID, x.clearing_part_id.c_str());
    y.SequenceNo = x.sequence_no;
    strcpy(y.AskOrderSysID, x.ask_order_sys_id.c_str());
    strcpy(y.BidOrderSysID, x.bid_order_sys_id.c_str());
    strcpy(y.ForQuoteSysID, x.for_quote_sys_id.c_str());
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    y.TimeCondition = x.time_condition;
    return y;
}

ExchangeQuote Converter::CThostFtdcExchangeQuoteFieldToRust(CThostFtdcExchangeQuoteField* x) {
    if (x == nullptr)
        return ExchangeQuote{.is_null = true};
    ExchangeQuote y{};
    y.ask_price = x->AskPrice;
    y.bid_price = x->BidPrice;
    y.ask_volume = x->AskVolume;
    y.bid_volume = x->BidVolume;
    y.request_id = x->RequestID;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.ask_offset_flag = x->AskOffsetFlag;
    y.bid_offset_flag = x->BidOffsetFlag;
    y.ask_hedge_flag = x->AskHedgeFlag;
    y.bid_hedge_flag = x->BidHedgeFlag;
    y.quote_local_id = Converter::Gb2312ToRustString(x->QuoteLocalID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.notify_sequence = x->NotifySequence;
    y.order_submit_status = x->OrderSubmitStatus;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.quote_sys_id = Converter::Gb2312ToRustString(x->QuoteSysID);
    y.insert_date = Converter::Gb2312ToRustString(x->InsertDate);
    y.insert_time = Converter::Gb2312ToRustString(x->InsertTime);
    y.cancel_time = Converter::Gb2312ToRustString(x->CancelTime);
    y.quote_status = x->QuoteStatus;
    y.clearing_part_id = Converter::Gb2312ToRustString(x->ClearingPartID);
    y.sequence_no = x->SequenceNo;
    y.ask_order_sys_id = Converter::Gb2312ToRustString(x->AskOrderSysID);
    y.bid_order_sys_id = Converter::Gb2312ToRustString(x->BidOrderSysID);
    y.for_quote_sys_id = Converter::Gb2312ToRustString(x->ForQuoteSysID);
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.time_condition = x->TimeCondition;
    return y;
}

CThostFtdcQryExchangeQuoteField Converter::QryExchangeQuoteToCpp(QryExchangeQuote x) {
    CThostFtdcQryExchangeQuoteField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    return y;
}

QryExchangeQuote Converter::CThostFtdcQryExchangeQuoteFieldToRust(CThostFtdcQryExchangeQuoteField* x) {
    if (x == nullptr)
        return QryExchangeQuote{.is_null = true};
    QryExchangeQuote y{};
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    return y;
}

CThostFtdcQryQuoteActionField Converter::QryQuoteActionToCpp(QryQuoteAction x) {
    CThostFtdcQryQuoteActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    return y;
}

QryQuoteAction Converter::CThostFtdcQryQuoteActionFieldToRust(CThostFtdcQryQuoteActionField* x) {
    if (x == nullptr)
        return QryQuoteAction{.is_null = true};
    QryQuoteAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    return y;
}

CThostFtdcExchangeQuoteActionField Converter::ExchangeQuoteActionToCpp(ExchangeQuoteAction x) {
    CThostFtdcExchangeQuoteActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.QuoteSysID, x.quote_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    strcpy(y.ActionDate, x.action_date.c_str());
    strcpy(y.ActionTime, x.action_time.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.QuoteLocalID, x.quote_local_id.c_str());
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OrderActionStatus = x.order_action_status;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ExchangeQuoteAction Converter::CThostFtdcExchangeQuoteActionFieldToRust(CThostFtdcExchangeQuoteActionField* x) {
    if (x == nullptr)
        return ExchangeQuoteAction{.is_null = true};
    ExchangeQuoteAction y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.quote_sys_id = Converter::Gb2312ToRustString(x->QuoteSysID);
    y.action_flag = x->ActionFlag;
    y.action_date = Converter::Gb2312ToRustString(x->ActionDate);
    y.action_time = Converter::Gb2312ToRustString(x->ActionTime);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.quote_local_id = Converter::Gb2312ToRustString(x->QuoteLocalID);
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.order_action_status = x->OrderActionStatus;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryExchangeQuoteActionField Converter::QryExchangeQuoteActionToCpp(QryExchangeQuoteAction x) {
    CThostFtdcQryExchangeQuoteActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    return y;
}

QryExchangeQuoteAction Converter::CThostFtdcQryExchangeQuoteActionFieldToRust(CThostFtdcQryExchangeQuoteActionField* x) {
    if (x == nullptr)
        return QryExchangeQuoteAction{.is_null = true};
    QryExchangeQuoteAction y{};
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    return y;
}

CThostFtdcOptionInstrDeltaField Converter::OptionInstrDeltaToCpp(OptionInstrDelta x) {
    CThostFtdcOptionInstrDeltaField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.Delta = x.delta;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

OptionInstrDelta Converter::CThostFtdcOptionInstrDeltaFieldToRust(CThostFtdcOptionInstrDeltaField* x) {
    if (x == nullptr)
        return OptionInstrDelta{.is_null = true};
    OptionInstrDelta y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.delta = x->Delta;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcForQuoteRspField Converter::ForQuoteRspToCpp(ForQuoteRsp x) {
    CThostFtdcForQuoteRspField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ForQuoteSysID, x.for_quote_sys_id.c_str());
    strcpy(y.ForQuoteTime, x.for_quote_time.c_str());
    strcpy(y.ActionDay, x.action_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

ForQuoteRsp Converter::CThostFtdcForQuoteRspFieldToRust(CThostFtdcForQuoteRspField* x) {
    if (x == nullptr)
        return ForQuoteRsp{.is_null = true};
    ForQuoteRsp y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.for_quote_sys_id = Converter::Gb2312ToRustString(x->ForQuoteSysID);
    y.for_quote_time = Converter::Gb2312ToRustString(x->ForQuoteTime);
    y.action_day = Converter::Gb2312ToRustString(x->ActionDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcStrikeOffsetField Converter::StrikeOffsetToCpp(StrikeOffset x) {
    CThostFtdcStrikeOffsetField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.Offset = x.offset;
    y.OffsetType = x.offset_type;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

StrikeOffset Converter::CThostFtdcStrikeOffsetFieldToRust(CThostFtdcStrikeOffsetField* x) {
    if (x == nullptr)
        return StrikeOffset{.is_null = true};
    StrikeOffset y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.offset = x->Offset;
    y.offset_type = x->OffsetType;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryStrikeOffsetField Converter::QryStrikeOffsetToCpp(QryStrikeOffset x) {
    CThostFtdcQryStrikeOffsetField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryStrikeOffset Converter::CThostFtdcQryStrikeOffsetFieldToRust(CThostFtdcQryStrikeOffsetField* x) {
    if (x == nullptr)
        return QryStrikeOffset{.is_null = true};
    QryStrikeOffset y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcInputBatchOrderActionField Converter::InputBatchOrderActionToCpp(InputBatchOrderAction x) {
    CThostFtdcInputBatchOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OrderActionRef = x.order_action_ref;
    y.RequestID = x.request_id;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

InputBatchOrderAction Converter::CThostFtdcInputBatchOrderActionFieldToRust(CThostFtdcInputBatchOrderActionField* x) {
    if (x == nullptr)
        return InputBatchOrderAction{.is_null = true};
    InputBatchOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.order_action_ref = x->OrderActionRef;
    y.request_id = x->RequestID;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcBatchOrderActionField Converter::BatchOrderActionToCpp(BatchOrderAction x) {
    CThostFtdcBatchOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OrderActionRef = x.order_action_ref;
    y.RequestID = x.request_id;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ActionDate, x.action_date.c_str());
    strcpy(y.ActionTime, x.action_time.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OrderActionStatus = x.order_action_status;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.StatusMsg, x.status_msg.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

BatchOrderAction Converter::CThostFtdcBatchOrderActionFieldToRust(CThostFtdcBatchOrderActionField* x) {
    if (x == nullptr)
        return BatchOrderAction{.is_null = true};
    BatchOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.order_action_ref = x->OrderActionRef;
    y.request_id = x->RequestID;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.action_date = Converter::Gb2312ToRustString(x->ActionDate);
    y.action_time = Converter::Gb2312ToRustString(x->ActionTime);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.order_action_status = x->OrderActionStatus;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcExchangeBatchOrderActionField Converter::ExchangeBatchOrderActionToCpp(ExchangeBatchOrderAction x) {
    CThostFtdcExchangeBatchOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ActionDate, x.action_date.c_str());
    strcpy(y.ActionTime, x.action_time.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OrderActionStatus = x.order_action_status;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ExchangeBatchOrderAction Converter::CThostFtdcExchangeBatchOrderActionFieldToRust(CThostFtdcExchangeBatchOrderActionField* x) {
    if (x == nullptr)
        return ExchangeBatchOrderAction{.is_null = true};
    ExchangeBatchOrderAction y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.action_date = Converter::Gb2312ToRustString(x->ActionDate);
    y.action_time = Converter::Gb2312ToRustString(x->ActionTime);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.order_action_status = x->OrderActionStatus;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryBatchOrderActionField Converter::QryBatchOrderActionToCpp(QryBatchOrderAction x) {
    CThostFtdcQryBatchOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    return y;
}

QryBatchOrderAction Converter::CThostFtdcQryBatchOrderActionFieldToRust(CThostFtdcQryBatchOrderActionField* x) {
    if (x == nullptr)
        return QryBatchOrderAction{.is_null = true};
    QryBatchOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    return y;
}

CThostFtdcCombInstrumentGuardField Converter::CombInstrumentGuardToCpp(CombInstrumentGuard x) {
    CThostFtdcCombInstrumentGuardField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    y.GuarantRatio = x.guaranty_ratio;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

CombInstrumentGuard Converter::CThostFtdcCombInstrumentGuardFieldToRust(CThostFtdcCombInstrumentGuardField* x) {
    if (x == nullptr)
        return CombInstrumentGuard{.is_null = true};
    CombInstrumentGuard y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.guaranty_ratio = x->GuarantRatio;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryCombInstrumentGuardField Converter::QryCombInstrumentGuardToCpp(QryCombInstrumentGuard x) {
    CThostFtdcQryCombInstrumentGuardField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryCombInstrumentGuard Converter::CThostFtdcQryCombInstrumentGuardFieldToRust(CThostFtdcQryCombInstrumentGuardField* x) {
    if (x == nullptr)
        return QryCombInstrumentGuard{.is_null = true};
    QryCombInstrumentGuard y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcInputCombActionField Converter::InputCombActionToCpp(InputCombAction x) {
    CThostFtdcInputCombActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.CombActionRef, x.comb_action_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.Direction = x.direction;
    y.Volume = x.volume;
    y.CombDirection = x.comb_direction;
    y.HedgeFlag = x.hedge_flag;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

InputCombAction Converter::CThostFtdcInputCombActionFieldToRust(CThostFtdcInputCombActionField* x) {
    if (x == nullptr)
        return InputCombAction{.is_null = true};
    InputCombAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.comb_action_ref = Converter::Gb2312ToRustString(x->CombActionRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.direction = x->Direction;
    y.volume = x->Volume;
    y.comb_direction = x->CombDirection;
    y.hedge_flag = x->HedgeFlag;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcCombActionField Converter::CombActionToCpp(CombAction x) {
    CThostFtdcCombActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.CombActionRef, x.comb_action_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.Direction = x.direction;
    y.Volume = x.volume;
    y.CombDirection = x.comb_direction;
    y.HedgeFlag = x.hedge_flag;
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    y.ActionStatus = x.action_status;
    y.NotifySequence = x.notify_sequence;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    y.SequenceNo = x.sequence_no;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.StatusMsg, x.status_msg.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.ComTradeID, x.com_trade_id.c_str());
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

CombAction Converter::CThostFtdcCombActionFieldToRust(CThostFtdcCombActionField* x) {
    if (x == nullptr)
        return CombAction{.is_null = true};
    CombAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.comb_action_ref = Converter::Gb2312ToRustString(x->CombActionRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.direction = x->Direction;
    y.volume = x->Volume;
    y.comb_direction = x->CombDirection;
    y.hedge_flag = x->HedgeFlag;
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.action_status = x->ActionStatus;
    y.notify_sequence = x->NotifySequence;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.sequence_no = x->SequenceNo;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.com_trade_id = Converter::Gb2312ToRustString(x->ComTradeID);
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryCombActionField Converter::QryCombActionToCpp(QryCombAction x) {
    CThostFtdcQryCombActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryCombAction Converter::CThostFtdcQryCombActionFieldToRust(CThostFtdcQryCombActionField* x) {
    if (x == nullptr)
        return QryCombAction{.is_null = true};
    QryCombAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcExchangeCombActionField Converter::ExchangeCombActionToCpp(ExchangeCombAction x) {
    CThostFtdcExchangeCombActionField y;
    memset(&y, 0, sizeof(y));
    y.Direction = x.direction;
    y.Volume = x.volume;
    y.CombDirection = x.comb_direction;
    y.HedgeFlag = x.hedge_flag;
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    y.ActionStatus = x.action_status;
    y.NotifySequence = x.notify_sequence;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    y.SequenceNo = x.sequence_no;
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.ComTradeID, x.com_trade_id.c_str());
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ExchangeCombAction Converter::CThostFtdcExchangeCombActionFieldToRust(CThostFtdcExchangeCombActionField* x) {
    if (x == nullptr)
        return ExchangeCombAction{.is_null = true};
    ExchangeCombAction y{};
    y.direction = x->Direction;
    y.volume = x->Volume;
    y.comb_direction = x->CombDirection;
    y.hedge_flag = x->HedgeFlag;
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.action_status = x->ActionStatus;
    y.notify_sequence = x->NotifySequence;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.sequence_no = x->SequenceNo;
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.com_trade_id = Converter::Gb2312ToRustString(x->ComTradeID);
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryExchangeCombActionField Converter::QryExchangeCombActionToCpp(QryExchangeCombAction x) {
    CThostFtdcQryExchangeCombActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    return y;
}

QryExchangeCombAction Converter::CThostFtdcQryExchangeCombActionFieldToRust(CThostFtdcQryExchangeCombActionField* x) {
    if (x == nullptr)
        return QryExchangeCombAction{.is_null = true};
    QryExchangeCombAction y{};
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    return y;
}

CThostFtdcProductExchRateField Converter::ProductExchRateToCpp(ProductExchRate x) {
    CThostFtdcProductExchRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.QuoteCurrencyID, x.quote_currency_id.c_str());
    y.ExchangeRate = x.exchange_rate;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    return y;
}

ProductExchRate Converter::CThostFtdcProductExchRateFieldToRust(CThostFtdcProductExchRateField* x) {
    if (x == nullptr)
        return ProductExchRate{.is_null = true};
    ProductExchRate y{};
    y.quote_currency_id = Converter::Gb2312ToRustString(x->QuoteCurrencyID);
    y.exchange_rate = x->ExchangeRate;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    return y;
}

CThostFtdcQryProductExchRateField Converter::QryProductExchRateToCpp(QryProductExchRate x) {
    CThostFtdcQryProductExchRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    return y;
}

QryProductExchRate Converter::CThostFtdcQryProductExchRateFieldToRust(CThostFtdcQryProductExchRateField* x) {
    if (x == nullptr)
        return QryProductExchRate{.is_null = true};
    QryProductExchRate y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    return y;
}

CThostFtdcQryForQuoteParamField Converter::QryForQuoteParamToCpp(QryForQuoteParam x) {
    CThostFtdcQryForQuoteParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryForQuoteParam Converter::CThostFtdcQryForQuoteParamFieldToRust(CThostFtdcQryForQuoteParamField* x) {
    if (x == nullptr)
        return QryForQuoteParam{.is_null = true};
    QryForQuoteParam y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcForQuoteParamField Converter::ForQuoteParamToCpp(ForQuoteParam x) {
    CThostFtdcForQuoteParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.LastPrice = x.last_price;
    y.PriceInterval = x.price_interval;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

ForQuoteParam Converter::CThostFtdcForQuoteParamFieldToRust(CThostFtdcForQuoteParamField* x) {
    if (x == nullptr)
        return ForQuoteParam{.is_null = true};
    ForQuoteParam y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.last_price = x->LastPrice;
    y.price_interval = x->PriceInterval;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcMMOptionInstrCommRateField Converter::MMOptionInstrCommRateToCpp(MMOptionInstrCommRate x) {
    CThostFtdcMMOptionInstrCommRateField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OpenRatioByMoney = x.open_ratio_by_money;
    y.OpenRatioByVolume = x.open_ratio_by_volume;
    y.CloseRatioByMoney = x.close_ratio_by_money;
    y.CloseRatioByVolume = x.close_ratio_by_volume;
    y.CloseTodayRatioByMoney = x.close_today_ratio_by_money;
    y.CloseTodayRatioByVolume = x.close_today_ratio_by_volume;
    y.StrikeRatioByMoney = x.strike_ratio_by_money;
    y.StrikeRatioByVolume = x.strike_ratio_by_volume;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

MMOptionInstrCommRate Converter::CThostFtdcMMOptionInstrCommRateFieldToRust(CThostFtdcMMOptionInstrCommRateField* x) {
    if (x == nullptr)
        return MMOptionInstrCommRate{.is_null = true};
    MMOptionInstrCommRate y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.open_ratio_by_money = x->OpenRatioByMoney;
    y.open_ratio_by_volume = x->OpenRatioByVolume;
    y.close_ratio_by_money = x->CloseRatioByMoney;
    y.close_ratio_by_volume = x->CloseRatioByVolume;
    y.close_today_ratio_by_money = x->CloseTodayRatioByMoney;
    y.close_today_ratio_by_volume = x->CloseTodayRatioByVolume;
    y.strike_ratio_by_money = x->StrikeRatioByMoney;
    y.strike_ratio_by_volume = x->StrikeRatioByVolume;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryMMOptionInstrCommRateField Converter::QryMMOptionInstrCommRateToCpp(QryMMOptionInstrCommRate x) {
    CThostFtdcQryMMOptionInstrCommRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryMMOptionInstrCommRate Converter::CThostFtdcQryMMOptionInstrCommRateFieldToRust(CThostFtdcQryMMOptionInstrCommRateField* x) {
    if (x == nullptr)
        return QryMMOptionInstrCommRate{.is_null = true};
    QryMMOptionInstrCommRate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcMMInstrumentCommissionRateField Converter::MMInstrumentCommissionRateToCpp(MMInstrumentCommissionRate x) {
    CThostFtdcMMInstrumentCommissionRateField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OpenRatioByMoney = x.open_ratio_by_money;
    y.OpenRatioByVolume = x.open_ratio_by_volume;
    y.CloseRatioByMoney = x.close_ratio_by_money;
    y.CloseRatioByVolume = x.close_ratio_by_volume;
    y.CloseTodayRatioByMoney = x.close_today_ratio_by_money;
    y.CloseTodayRatioByVolume = x.close_today_ratio_by_volume;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

MMInstrumentCommissionRate Converter::CThostFtdcMMInstrumentCommissionRateFieldToRust(CThostFtdcMMInstrumentCommissionRateField* x) {
    if (x == nullptr)
        return MMInstrumentCommissionRate{.is_null = true};
    MMInstrumentCommissionRate y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.open_ratio_by_money = x->OpenRatioByMoney;
    y.open_ratio_by_volume = x->OpenRatioByVolume;
    y.close_ratio_by_money = x->CloseRatioByMoney;
    y.close_ratio_by_volume = x->CloseRatioByVolume;
    y.close_today_ratio_by_money = x->CloseTodayRatioByMoney;
    y.close_today_ratio_by_volume = x->CloseTodayRatioByVolume;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryMMInstrumentCommissionRateField Converter::QryMMInstrumentCommissionRateToCpp(QryMMInstrumentCommissionRate x) {
    CThostFtdcQryMMInstrumentCommissionRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryMMInstrumentCommissionRate Converter::CThostFtdcQryMMInstrumentCommissionRateFieldToRust(CThostFtdcQryMMInstrumentCommissionRateField* x) {
    if (x == nullptr)
        return QryMMInstrumentCommissionRate{.is_null = true};
    QryMMInstrumentCommissionRate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcInstrumentOrderCommRateField Converter::InstrumentOrderCommRateToCpp(InstrumentOrderCommRate x) {
    CThostFtdcInstrumentOrderCommRateField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.OrderCommByVolume = x.order_comm_by_volume;
    y.OrderActionCommByVolume = x.order_action_comm_by_volume;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.OrderCommByTrade = x.order_comm_by_trade;
    y.OrderActionCommByTrade = x.order_action_comm_by_trade;
    return y;
}

InstrumentOrderCommRate Converter::CThostFtdcInstrumentOrderCommRateFieldToRust(CThostFtdcInstrumentOrderCommRateField* x) {
    if (x == nullptr)
        return InstrumentOrderCommRate{.is_null = true};
    InstrumentOrderCommRate y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.hedge_flag = x->HedgeFlag;
    y.order_comm_by_volume = x->OrderCommByVolume;
    y.order_action_comm_by_volume = x->OrderActionCommByVolume;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.order_comm_by_trade = x->OrderCommByTrade;
    y.order_action_comm_by_trade = x->OrderActionCommByTrade;
    return y;
}

CThostFtdcQryInstrumentOrderCommRateField Converter::QryInstrumentOrderCommRateToCpp(QryInstrumentOrderCommRate x) {
    CThostFtdcQryInstrumentOrderCommRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryInstrumentOrderCommRate Converter::CThostFtdcQryInstrumentOrderCommRateFieldToRust(CThostFtdcQryInstrumentOrderCommRateField* x) {
    if (x == nullptr)
        return QryInstrumentOrderCommRate{.is_null = true};
    QryInstrumentOrderCommRate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcTradeParamField Converter::TradeParamToCpp(TradeParam x) {
    CThostFtdcTradeParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    y.TradeParamID = x.trade_param_id;
    memcpy(y.TradeParamValue, x.trade_param_value.data(), x.trade_param_value.size() * sizeof(uint8_t));
    strcpy(y.Memo, x.memo.c_str());
    return y;
}

TradeParam Converter::CThostFtdcTradeParamFieldToRust(CThostFtdcTradeParamField* x) {
    if (x == nullptr)
        return TradeParam{.is_null = true};
    TradeParam y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.trade_param_id = x->TradeParamID;
    for (int i = 0; i < 256; i++)
        y.trade_param_value.push_back(x->TradeParamValue[i]);
    y.memo = Converter::Gb2312ToRustString(x->Memo);
    return y;
}

CThostFtdcInstrumentMarginRateULField Converter::InstrumentMarginRateULToCpp(InstrumentMarginRateUL x) {
    CThostFtdcInstrumentMarginRateULField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.LongMarginRatioByMoney = x.long_margin_ratio_by_money;
    y.LongMarginRatioByVolume = x.long_margin_ratio_by_volume;
    y.ShortMarginRatioByMoney = x.short_margin_ratio_by_money;
    y.ShortMarginRatioByVolume = x.short_margin_ratio_by_volume;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

InstrumentMarginRateUL Converter::CThostFtdcInstrumentMarginRateULFieldToRust(CThostFtdcInstrumentMarginRateULField* x) {
    if (x == nullptr)
        return InstrumentMarginRateUL{.is_null = true};
    InstrumentMarginRateUL y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.hedge_flag = x->HedgeFlag;
    y.long_margin_ratio_by_money = x->LongMarginRatioByMoney;
    y.long_margin_ratio_by_volume = x->LongMarginRatioByVolume;
    y.short_margin_ratio_by_money = x->ShortMarginRatioByMoney;
    y.short_margin_ratio_by_volume = x->ShortMarginRatioByVolume;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcFutureLimitPosiParamField Converter::FutureLimitPosiParamToCpp(FutureLimitPosiParam x) {
    CThostFtdcFutureLimitPosiParamField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.SpecOpenVolume = x.spec_open_volume;
    y.ArbiOpenVolume = x.arbi_open_volume;
    y.OpenVolume = x.open_volume;
    strcpy(y.ProductID, x.product_id.c_str());
    return y;
}

FutureLimitPosiParam Converter::CThostFtdcFutureLimitPosiParamFieldToRust(CThostFtdcFutureLimitPosiParamField* x) {
    if (x == nullptr)
        return FutureLimitPosiParam{.is_null = true};
    FutureLimitPosiParam y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.spec_open_volume = x->SpecOpenVolume;
    y.arbi_open_volume = x->ArbiOpenVolume;
    y.open_volume = x->OpenVolume;
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    return y;
}

CThostFtdcLoginForbiddenIPField Converter::LoginForbiddenIPToCpp(LoginForbiddenIP x) {
    CThostFtdcLoginForbiddenIPField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

LoginForbiddenIP Converter::CThostFtdcLoginForbiddenIPFieldToRust(CThostFtdcLoginForbiddenIPField* x) {
    if (x == nullptr)
        return LoginForbiddenIP{.is_null = true};
    LoginForbiddenIP y{};
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcIPListField Converter::IPListToCpp(IPList x) {
    CThostFtdcIPListField y;
    memset(&y, 0, sizeof(y));
    y.IsWhite = x.is_white;
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

IPList Converter::CThostFtdcIPListFieldToRust(CThostFtdcIPListField* x) {
    if (x == nullptr)
        return IPList{.is_null = true};
    IPList y{};
    y.is_white = x->IsWhite;
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcInputOptionSelfCloseField Converter::InputOptionSelfCloseToCpp(InputOptionSelfClose x) {
    CThostFtdcInputOptionSelfCloseField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.OptionSelfCloseRef, x.option_self_close_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.Volume = x.volume;
    y.RequestID = x.request_id;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.OptSelfCloseFlag = x.opt_self_close_flag;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

InputOptionSelfClose Converter::CThostFtdcInputOptionSelfCloseFieldToRust(CThostFtdcInputOptionSelfCloseField* x) {
    if (x == nullptr)
        return InputOptionSelfClose{.is_null = true};
    InputOptionSelfClose y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.option_self_close_ref = Converter::Gb2312ToRustString(x->OptionSelfCloseRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.volume = x->Volume;
    y.request_id = x->RequestID;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.hedge_flag = x->HedgeFlag;
    y.opt_self_close_flag = x->OptSelfCloseFlag;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcInputOptionSelfCloseActionField Converter::InputOptionSelfCloseActionToCpp(InputOptionSelfCloseAction x) {
    CThostFtdcInputOptionSelfCloseActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OptionSelfCloseActionRef = x.option_self_close_action_ref;
    strcpy(y.OptionSelfCloseRef, x.option_self_close_ref.c_str());
    y.RequestID = x.request_id;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.OptionSelfCloseSysID, x.option_self_close_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

InputOptionSelfCloseAction Converter::CThostFtdcInputOptionSelfCloseActionFieldToRust(CThostFtdcInputOptionSelfCloseActionField* x) {
    if (x == nullptr)
        return InputOptionSelfCloseAction{.is_null = true};
    InputOptionSelfCloseAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.option_self_close_action_ref = x->OptionSelfCloseActionRef;
    y.option_self_close_ref = Converter::Gb2312ToRustString(x->OptionSelfCloseRef);
    y.request_id = x->RequestID;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.option_self_close_sys_id = Converter::Gb2312ToRustString(x->OptionSelfCloseSysID);
    y.action_flag = x->ActionFlag;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcOptionSelfCloseField Converter::OptionSelfCloseToCpp(OptionSelfClose x) {
    CThostFtdcOptionSelfCloseField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.OptionSelfCloseRef, x.option_self_close_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.Volume = x.volume;
    y.RequestID = x.request_id;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.OptSelfCloseFlag = x.opt_self_close_flag;
    strcpy(y.OptionSelfCloseLocalID, x.option_self_close_local_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    y.OrderSubmitStatus = x.order_submit_status;
    y.NotifySequence = x.notify_sequence;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.OptionSelfCloseSysID, x.option_self_close_sys_id.c_str());
    strcpy(y.InsertDate, x.insert_date.c_str());
    strcpy(y.InsertTime, x.insert_time.c_str());
    strcpy(y.CancelTime, x.cancel_time.c_str());
    y.ExecResult = x.exec_result;
    strcpy(y.ClearingPartID, x.clearing_part_id.c_str());
    y.SequenceNo = x.sequence_no;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.StatusMsg, x.status_msg.c_str());
    strcpy(y.ActiveUserID, x.active_user_id.c_str());
    y.BrokerOptionSelfCloseSeq = x.broker_option_self_close_seq;
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

OptionSelfClose Converter::CThostFtdcOptionSelfCloseFieldToRust(CThostFtdcOptionSelfCloseField* x) {
    if (x == nullptr)
        return OptionSelfClose{.is_null = true};
    OptionSelfClose y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.option_self_close_ref = Converter::Gb2312ToRustString(x->OptionSelfCloseRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.volume = x->Volume;
    y.request_id = x->RequestID;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.hedge_flag = x->HedgeFlag;
    y.opt_self_close_flag = x->OptSelfCloseFlag;
    y.option_self_close_local_id = Converter::Gb2312ToRustString(x->OptionSelfCloseLocalID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.order_submit_status = x->OrderSubmitStatus;
    y.notify_sequence = x->NotifySequence;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.option_self_close_sys_id = Converter::Gb2312ToRustString(x->OptionSelfCloseSysID);
    y.insert_date = Converter::Gb2312ToRustString(x->InsertDate);
    y.insert_time = Converter::Gb2312ToRustString(x->InsertTime);
    y.cancel_time = Converter::Gb2312ToRustString(x->CancelTime);
    y.exec_result = x->ExecResult;
    y.clearing_part_id = Converter::Gb2312ToRustString(x->ClearingPartID);
    y.sequence_no = x->SequenceNo;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.active_user_id = Converter::Gb2312ToRustString(x->ActiveUserID);
    y.broker_option_self_close_seq = x->BrokerOptionSelfCloseSeq;
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcOptionSelfCloseActionField Converter::OptionSelfCloseActionToCpp(OptionSelfCloseAction x) {
    CThostFtdcOptionSelfCloseActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OptionSelfCloseActionRef = x.option_self_close_action_ref;
    strcpy(y.OptionSelfCloseRef, x.option_self_close_ref.c_str());
    y.RequestID = x.request_id;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.OptionSelfCloseSysID, x.option_self_close_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    strcpy(y.ActionDate, x.action_date.c_str());
    strcpy(y.ActionTime, x.action_time.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.OptionSelfCloseLocalID, x.option_self_close_local_id.c_str());
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OrderActionStatus = x.order_action_status;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.StatusMsg, x.status_msg.c_str());
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

OptionSelfCloseAction Converter::CThostFtdcOptionSelfCloseActionFieldToRust(CThostFtdcOptionSelfCloseActionField* x) {
    if (x == nullptr)
        return OptionSelfCloseAction{.is_null = true};
    OptionSelfCloseAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.option_self_close_action_ref = x->OptionSelfCloseActionRef;
    y.option_self_close_ref = Converter::Gb2312ToRustString(x->OptionSelfCloseRef);
    y.request_id = x->RequestID;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.option_self_close_sys_id = Converter::Gb2312ToRustString(x->OptionSelfCloseSysID);
    y.action_flag = x->ActionFlag;
    y.action_date = Converter::Gb2312ToRustString(x->ActionDate);
    y.action_time = Converter::Gb2312ToRustString(x->ActionTime);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.option_self_close_local_id = Converter::Gb2312ToRustString(x->OptionSelfCloseLocalID);
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.order_action_status = x->OrderActionStatus;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryOptionSelfCloseField Converter::QryOptionSelfCloseToCpp(QryOptionSelfClose x) {
    CThostFtdcQryOptionSelfCloseField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.OptionSelfCloseSysID, x.option_self_close_sys_id.c_str());
    strcpy(y.InsertTimeStart, x.insert_time_start.c_str());
    strcpy(y.InsertTimeEnd, x.insert_time_end.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryOptionSelfClose Converter::CThostFtdcQryOptionSelfCloseFieldToRust(CThostFtdcQryOptionSelfCloseField* x) {
    if (x == nullptr)
        return QryOptionSelfClose{.is_null = true};
    QryOptionSelfClose y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.option_self_close_sys_id = Converter::Gb2312ToRustString(x->OptionSelfCloseSysID);
    y.insert_time_start = Converter::Gb2312ToRustString(x->InsertTimeStart);
    y.insert_time_end = Converter::Gb2312ToRustString(x->InsertTimeEnd);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcExchangeOptionSelfCloseField Converter::ExchangeOptionSelfCloseToCpp(ExchangeOptionSelfClose x) {
    CThostFtdcExchangeOptionSelfCloseField y;
    memset(&y, 0, sizeof(y));
    y.Volume = x.volume;
    y.RequestID = x.request_id;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.OptSelfCloseFlag = x.opt_self_close_flag;
    strcpy(y.OptionSelfCloseLocalID, x.option_self_close_local_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    y.OrderSubmitStatus = x.order_submit_status;
    y.NotifySequence = x.notify_sequence;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.OptionSelfCloseSysID, x.option_self_close_sys_id.c_str());
    strcpy(y.InsertDate, x.insert_date.c_str());
    strcpy(y.InsertTime, x.insert_time.c_str());
    strcpy(y.CancelTime, x.cancel_time.c_str());
    y.ExecResult = x.exec_result;
    strcpy(y.ClearingPartID, x.clearing_part_id.c_str());
    y.SequenceNo = x.sequence_no;
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ExchangeOptionSelfClose Converter::CThostFtdcExchangeOptionSelfCloseFieldToRust(CThostFtdcExchangeOptionSelfCloseField* x) {
    if (x == nullptr)
        return ExchangeOptionSelfClose{.is_null = true};
    ExchangeOptionSelfClose y{};
    y.volume = x->Volume;
    y.request_id = x->RequestID;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.hedge_flag = x->HedgeFlag;
    y.opt_self_close_flag = x->OptSelfCloseFlag;
    y.option_self_close_local_id = Converter::Gb2312ToRustString(x->OptionSelfCloseLocalID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.order_submit_status = x->OrderSubmitStatus;
    y.notify_sequence = x->NotifySequence;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.option_self_close_sys_id = Converter::Gb2312ToRustString(x->OptionSelfCloseSysID);
    y.insert_date = Converter::Gb2312ToRustString(x->InsertDate);
    y.insert_time = Converter::Gb2312ToRustString(x->InsertTime);
    y.cancel_time = Converter::Gb2312ToRustString(x->CancelTime);
    y.exec_result = x->ExecResult;
    y.clearing_part_id = Converter::Gb2312ToRustString(x->ClearingPartID);
    y.sequence_no = x->SequenceNo;
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryOptionSelfCloseActionField Converter::QryOptionSelfCloseActionToCpp(QryOptionSelfCloseAction x) {
    CThostFtdcQryOptionSelfCloseActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    return y;
}

QryOptionSelfCloseAction Converter::CThostFtdcQryOptionSelfCloseActionFieldToRust(CThostFtdcQryOptionSelfCloseActionField* x) {
    if (x == nullptr)
        return QryOptionSelfCloseAction{.is_null = true};
    QryOptionSelfCloseAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    return y;
}

CThostFtdcExchangeOptionSelfCloseActionField Converter::ExchangeOptionSelfCloseActionToCpp(ExchangeOptionSelfCloseAction x) {
    CThostFtdcExchangeOptionSelfCloseActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.OptionSelfCloseSysID, x.option_self_close_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    strcpy(y.ActionDate, x.action_date.c_str());
    strcpy(y.ActionTime, x.action_time.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.OptionSelfCloseLocalID, x.option_self_close_local_id.c_str());
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OrderActionStatus = x.order_action_status;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    y.OptSelfCloseFlag = x.opt_self_close_flag;
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    return y;
}

ExchangeOptionSelfCloseAction Converter::CThostFtdcExchangeOptionSelfCloseActionFieldToRust(CThostFtdcExchangeOptionSelfCloseActionField* x) {
    if (x == nullptr)
        return ExchangeOptionSelfCloseAction{.is_null = true};
    ExchangeOptionSelfCloseAction y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.option_self_close_sys_id = Converter::Gb2312ToRustString(x->OptionSelfCloseSysID);
    y.action_flag = x->ActionFlag;
    y.action_date = Converter::Gb2312ToRustString(x->ActionDate);
    y.action_time = Converter::Gb2312ToRustString(x->ActionTime);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.option_self_close_local_id = Converter::Gb2312ToRustString(x->OptionSelfCloseLocalID);
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.order_action_status = x->OrderActionStatus;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.opt_self_close_flag = x->OptSelfCloseFlag;
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    return y;
}

CThostFtdcSyncDelaySwapField Converter::SyncDelaySwapToCpp(SyncDelaySwap x) {
    CThostFtdcSyncDelaySwapField y;
    memset(&y, 0, sizeof(y));
    memcpy(y.DelaySwapSeqNo, x.delay_swap_seq_no.data(), x.delay_swap_seq_no.size() * sizeof(uint8_t));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.FromCurrencyID, x.from_currency_id.c_str());
    y.FromAmount = x.from_amount;
    y.FromFrozenSwap = x.from_frozen_swap;
    y.FromRemainSwap = x.from_remain_swap;
    strcpy(y.ToCurrencyID, x.to_currency_id.c_str());
    y.ToAmount = x.to_amount;
    y.IsManualSwap = x.is_manual_swap;
    y.IsAllRemainSetZero = x.is_all_remain_set_zero;
    return y;
}

SyncDelaySwap Converter::CThostFtdcSyncDelaySwapFieldToRust(CThostFtdcSyncDelaySwapField* x) {
    if (x == nullptr)
        return SyncDelaySwap{.is_null = true};
    SyncDelaySwap y{};
    for (int i = 0; i < 15; i++)
        y.delay_swap_seq_no.push_back(x->DelaySwapSeqNo[i]);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.from_currency_id = Converter::Gb2312ToRustString(x->FromCurrencyID);
    y.from_amount = x->FromAmount;
    y.from_frozen_swap = x->FromFrozenSwap;
    y.from_remain_swap = x->FromRemainSwap;
    y.to_currency_id = Converter::Gb2312ToRustString(x->ToCurrencyID);
    y.to_amount = x->ToAmount;
    y.is_manual_swap = x->IsManualSwap;
    y.is_all_remain_set_zero = x->IsAllRemainSetZero;
    return y;
}

CThostFtdcQrySyncDelaySwapField Converter::QrySyncDelaySwapToCpp(QrySyncDelaySwap x) {
    CThostFtdcQrySyncDelaySwapField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    memcpy(y.DelaySwapSeqNo, x.delay_swap_seq_no.data(), x.delay_swap_seq_no.size() * sizeof(uint8_t));
    return y;
}

QrySyncDelaySwap Converter::CThostFtdcQrySyncDelaySwapFieldToRust(CThostFtdcQrySyncDelaySwapField* x) {
    if (x == nullptr)
        return QrySyncDelaySwap{.is_null = true};
    QrySyncDelaySwap y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    for (int i = 0; i < 15; i++)
        y.delay_swap_seq_no.push_back(x->DelaySwapSeqNo[i]);
    return y;
}

CThostFtdcInvestUnitField Converter::InvestUnitToCpp(InvestUnit x) {
    CThostFtdcInvestUnitField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InvestorUnitName, x.investor_unit_name.c_str());
    strcpy(y.InvestorGroupID, x.investor_group_id.c_str());
    strcpy(y.CommModelID, x.comm_model_id.c_str());
    strcpy(y.MarginModelID, x.margin_model_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

InvestUnit Converter::CThostFtdcInvestUnitFieldToRust(CThostFtdcInvestUnitField* x) {
    if (x == nullptr)
        return InvestUnit{.is_null = true};
    InvestUnit y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.investor_unit_name = Converter::Gb2312ToRustString(x->InvestorUnitName);
    y.investor_group_id = Converter::Gb2312ToRustString(x->InvestorGroupID);
    y.comm_model_id = Converter::Gb2312ToRustString(x->CommModelID);
    y.margin_model_id = Converter::Gb2312ToRustString(x->MarginModelID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcQryInvestUnitField Converter::QryInvestUnitToCpp(QryInvestUnit x) {
    CThostFtdcQryInvestUnitField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    return y;
}

QryInvestUnit Converter::CThostFtdcQryInvestUnitFieldToRust(CThostFtdcQryInvestUnitField* x) {
    if (x == nullptr)
        return QryInvestUnit{.is_null = true};
    QryInvestUnit y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    return y;
}

CThostFtdcSecAgentCheckModeField Converter::SecAgentCheckModeToCpp(SecAgentCheckMode x) {
    CThostFtdcSecAgentCheckModeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.BrokerSecAgentID, x.broker_sec_agent_id.c_str());
    y.CheckSelfAccount = x.check_self_account;
    return y;
}

SecAgentCheckMode Converter::CThostFtdcSecAgentCheckModeFieldToRust(CThostFtdcSecAgentCheckModeField* x) {
    if (x == nullptr)
        return SecAgentCheckMode{.is_null = true};
    SecAgentCheckMode y{};
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.broker_sec_agent_id = Converter::Gb2312ToRustString(x->BrokerSecAgentID);
    y.check_self_account = x->CheckSelfAccount;
    return y;
}

CThostFtdcSecAgentTradeInfoField Converter::SecAgentTradeInfoToCpp(SecAgentTradeInfo x) {
    CThostFtdcSecAgentTradeInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerSecAgentID, x.broker_sec_agent_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

SecAgentTradeInfo Converter::CThostFtdcSecAgentTradeInfoFieldToRust(CThostFtdcSecAgentTradeInfoField* x) {
    if (x == nullptr)
        return SecAgentTradeInfo{.is_null = true};
    SecAgentTradeInfo y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_sec_agent_id = Converter::Gb2312ToRustString(x->BrokerSecAgentID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcMarketDataField Converter::MarketDataToCpp(MarketData x) {
    CThostFtdcMarketDataField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.LastPrice = x.last_price;
    y.PreSettlementPrice = x.pre_settlement_price;
    y.PreClosePrice = x.pre_close_price;
    y.PreOpenInterest = x.pre_open_interest;
    y.OpenPrice = x.open_price;
    y.HighestPrice = x.highest_price;
    y.LowestPrice = x.lowest_price;
    y.Volume = x.volume;
    y.Turnover = x.turnover;
    y.OpenInterest = x.open_interest;
    y.ClosePrice = x.close_price;
    y.SettlementPrice = x.settlement_price;
    y.UpperLimitPrice = x.upper_limit_price;
    y.LowerLimitPrice = x.lower_limit_price;
    y.PreDelta = x.pre_delta;
    y.CurrDelta = x.curr_delta;
    strcpy(y.UpdateTime, x.update_time.c_str());
    y.UpdateMillisec = x.update_millisecond;
    strcpy(y.ActionDay, x.action_day.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    return y;
}

MarketData Converter::CThostFtdcMarketDataFieldToRust(CThostFtdcMarketDataField* x) {
    if (x == nullptr)
        return MarketData{.is_null = true};
    MarketData y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.last_price = x->LastPrice;
    y.pre_settlement_price = x->PreSettlementPrice;
    y.pre_close_price = x->PreClosePrice;
    y.pre_open_interest = x->PreOpenInterest;
    y.open_price = x->OpenPrice;
    y.highest_price = x->HighestPrice;
    y.lowest_price = x->LowestPrice;
    y.volume = x->Volume;
    y.turnover = x->Turnover;
    y.open_interest = x->OpenInterest;
    y.close_price = x->ClosePrice;
    y.settlement_price = x->SettlementPrice;
    y.upper_limit_price = x->UpperLimitPrice;
    y.lower_limit_price = x->LowerLimitPrice;
    y.pre_delta = x->PreDelta;
    y.curr_delta = x->CurrDelta;
    y.update_time = Converter::Gb2312ToRustString(x->UpdateTime);
    y.update_millisecond = x->UpdateMillisec;
    y.action_day = Converter::Gb2312ToRustString(x->ActionDay);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    return y;
}

CThostFtdcMarketDataBaseField Converter::MarketDataBaseToCpp(MarketDataBase x) {
    CThostFtdcMarketDataBaseField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PreSettlementPrice = x.pre_settlement_price;
    y.PreClosePrice = x.pre_close_price;
    y.PreOpenInterest = x.pre_open_interest;
    y.PreDelta = x.pre_delta;
    return y;
}

MarketDataBase Converter::CThostFtdcMarketDataBaseFieldToRust(CThostFtdcMarketDataBaseField* x) {
    if (x == nullptr)
        return MarketDataBase{.is_null = true};
    MarketDataBase y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.pre_settlement_price = x->PreSettlementPrice;
    y.pre_close_price = x->PreClosePrice;
    y.pre_open_interest = x->PreOpenInterest;
    y.pre_delta = x->PreDelta;
    return y;
}

CThostFtdcMarketDataStaticField Converter::MarketDataStaticToCpp(MarketDataStatic x) {
    CThostFtdcMarketDataStaticField y;
    memset(&y, 0, sizeof(y));
    y.OpenPrice = x.open_price;
    y.HighestPrice = x.highest_price;
    y.LowestPrice = x.lowest_price;
    y.ClosePrice = x.close_price;
    y.UpperLimitPrice = x.upper_limit_price;
    y.LowerLimitPrice = x.lower_limit_price;
    y.SettlementPrice = x.settlement_price;
    y.CurrDelta = x.curr_delta;
    return y;
}

MarketDataStatic Converter::CThostFtdcMarketDataStaticFieldToRust(CThostFtdcMarketDataStaticField* x) {
    if (x == nullptr)
        return MarketDataStatic{.is_null = true};
    MarketDataStatic y{};
    y.open_price = x->OpenPrice;
    y.highest_price = x->HighestPrice;
    y.lowest_price = x->LowestPrice;
    y.close_price = x->ClosePrice;
    y.upper_limit_price = x->UpperLimitPrice;
    y.lower_limit_price = x->LowerLimitPrice;
    y.settlement_price = x->SettlementPrice;
    y.curr_delta = x->CurrDelta;
    return y;
}

CThostFtdcMarketDataLastMatchField Converter::MarketDataLastMatchToCpp(MarketDataLastMatch x) {
    CThostFtdcMarketDataLastMatchField y;
    memset(&y, 0, sizeof(y));
    y.LastPrice = x.last_price;
    y.Volume = x.volume;
    y.Turnover = x.turnover;
    y.OpenInterest = x.open_interest;
    return y;
}

MarketDataLastMatch Converter::CThostFtdcMarketDataLastMatchFieldToRust(CThostFtdcMarketDataLastMatchField* x) {
    if (x == nullptr)
        return MarketDataLastMatch{.is_null = true};
    MarketDataLastMatch y{};
    y.last_price = x->LastPrice;
    y.volume = x->Volume;
    y.turnover = x->Turnover;
    y.open_interest = x->OpenInterest;
    return y;
}

CThostFtdcMarketDataBestPriceField Converter::MarketDataBestPriceToCpp(MarketDataBestPrice x) {
    CThostFtdcMarketDataBestPriceField y;
    memset(&y, 0, sizeof(y));
    y.BidPrice1 = x.bid_price1;
    y.BidVolume1 = x.bid_volume1;
    y.AskPrice1 = x.ask_price1;
    y.AskVolume1 = x.ask_volume1;
    return y;
}

MarketDataBestPrice Converter::CThostFtdcMarketDataBestPriceFieldToRust(CThostFtdcMarketDataBestPriceField* x) {
    if (x == nullptr)
        return MarketDataBestPrice{.is_null = true};
    MarketDataBestPrice y{};
    y.bid_price1 = x->BidPrice1;
    y.bid_volume1 = x->BidVolume1;
    y.ask_price1 = x->AskPrice1;
    y.ask_volume1 = x->AskVolume1;
    return y;
}

CThostFtdcMarketDataBid23Field Converter::MarketDataBid23ToCpp(MarketDataBid23 x) {
    CThostFtdcMarketDataBid23Field y;
    memset(&y, 0, sizeof(y));
    y.BidPrice2 = x.bid_price2;
    y.BidVolume2 = x.bid_volume2;
    y.BidPrice3 = x.bid_price3;
    y.BidVolume3 = x.bid_volume3;
    return y;
}

MarketDataBid23 Converter::CThostFtdcMarketDataBid23FieldToRust(CThostFtdcMarketDataBid23Field* x) {
    if (x == nullptr)
        return MarketDataBid23{.is_null = true};
    MarketDataBid23 y{};
    y.bid_price2 = x->BidPrice2;
    y.bid_volume2 = x->BidVolume2;
    y.bid_price3 = x->BidPrice3;
    y.bid_volume3 = x->BidVolume3;
    return y;
}

CThostFtdcMarketDataAsk23Field Converter::MarketDataAsk23ToCpp(MarketDataAsk23 x) {
    CThostFtdcMarketDataAsk23Field y;
    memset(&y, 0, sizeof(y));
    y.AskPrice2 = x.ask_price2;
    y.AskVolume2 = x.ask_volume2;
    y.AskPrice3 = x.ask_price3;
    y.AskVolume3 = x.ask_volume3;
    return y;
}

MarketDataAsk23 Converter::CThostFtdcMarketDataAsk23FieldToRust(CThostFtdcMarketDataAsk23Field* x) {
    if (x == nullptr)
        return MarketDataAsk23{.is_null = true};
    MarketDataAsk23 y{};
    y.ask_price2 = x->AskPrice2;
    y.ask_volume2 = x->AskVolume2;
    y.ask_price3 = x->AskPrice3;
    y.ask_volume3 = x->AskVolume3;
    return y;
}

CThostFtdcMarketDataBid45Field Converter::MarketDataBid45ToCpp(MarketDataBid45 x) {
    CThostFtdcMarketDataBid45Field y;
    memset(&y, 0, sizeof(y));
    y.BidPrice4 = x.bid_price4;
    y.BidVolume4 = x.bid_volume4;
    y.BidPrice5 = x.bid_price5;
    y.BidVolume5 = x.bid_volume5;
    return y;
}

MarketDataBid45 Converter::CThostFtdcMarketDataBid45FieldToRust(CThostFtdcMarketDataBid45Field* x) {
    if (x == nullptr)
        return MarketDataBid45{.is_null = true};
    MarketDataBid45 y{};
    y.bid_price4 = x->BidPrice4;
    y.bid_volume4 = x->BidVolume4;
    y.bid_price5 = x->BidPrice5;
    y.bid_volume5 = x->BidVolume5;
    return y;
}

CThostFtdcMarketDataAsk45Field Converter::MarketDataAsk45ToCpp(MarketDataAsk45 x) {
    CThostFtdcMarketDataAsk45Field y;
    memset(&y, 0, sizeof(y));
    y.AskPrice4 = x.ask_price4;
    y.AskVolume4 = x.ask_volume4;
    y.AskPrice5 = x.ask_price5;
    y.AskVolume5 = x.ask_volume5;
    return y;
}

MarketDataAsk45 Converter::CThostFtdcMarketDataAsk45FieldToRust(CThostFtdcMarketDataAsk45Field* x) {
    if (x == nullptr)
        return MarketDataAsk45{.is_null = true};
    MarketDataAsk45 y{};
    y.ask_price4 = x->AskPrice4;
    y.ask_volume4 = x->AskVolume4;
    y.ask_price5 = x->AskPrice5;
    y.ask_volume5 = x->AskVolume5;
    return y;
}

CThostFtdcMarketDataUpdateTimeField Converter::MarketDataUpdateTimeToCpp(MarketDataUpdateTime x) {
    CThostFtdcMarketDataUpdateTimeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.UpdateTime, x.update_time.c_str());
    y.UpdateMillisec = x.update_millisecond;
    strcpy(y.ActionDay, x.action_day.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

MarketDataUpdateTime Converter::CThostFtdcMarketDataUpdateTimeFieldToRust(CThostFtdcMarketDataUpdateTimeField* x) {
    if (x == nullptr)
        return MarketDataUpdateTime{.is_null = true};
    MarketDataUpdateTime y{};
    y.update_time = Converter::Gb2312ToRustString(x->UpdateTime);
    y.update_millisecond = x->UpdateMillisec;
    y.action_day = Converter::Gb2312ToRustString(x->ActionDay);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcMarketDataBandingPriceField Converter::MarketDataBandingPriceToCpp(MarketDataBandingPrice x) {
    CThostFtdcMarketDataBandingPriceField y;
    memset(&y, 0, sizeof(y));
    y.BandingUpperPrice = x.banding_upper_price;
    y.BandingLowerPrice = x.banding_lower_price;
    return y;
}

MarketDataBandingPrice Converter::CThostFtdcMarketDataBandingPriceFieldToRust(CThostFtdcMarketDataBandingPriceField* x) {
    if (x == nullptr)
        return MarketDataBandingPrice{.is_null = true};
    MarketDataBandingPrice y{};
    y.banding_upper_price = x->BandingUpperPrice;
    y.banding_lower_price = x->BandingLowerPrice;
    return y;
}

CThostFtdcMarketDataExchangeField Converter::MarketDataExchangeToCpp(MarketDataExchange x) {
    CThostFtdcMarketDataExchangeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    return y;
}

MarketDataExchange Converter::CThostFtdcMarketDataExchangeFieldToRust(CThostFtdcMarketDataExchangeField* x) {
    if (x == nullptr)
        return MarketDataExchange{.is_null = true};
    MarketDataExchange y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    return y;
}

CThostFtdcSpecificInstrumentField Converter::SpecificInstrumentToCpp(SpecificInstrument x) {
    CThostFtdcSpecificInstrumentField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

SpecificInstrument Converter::CThostFtdcSpecificInstrumentFieldToRust(CThostFtdcSpecificInstrumentField* x) {
    if (x == nullptr)
        return SpecificInstrument{.is_null = true};
    SpecificInstrument y{};
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcInstrumentStatusField Converter::InstrumentStatusToCpp(InstrumentStatus x) {
    CThostFtdcInstrumentStatusField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.SettlementGroupID, x.settlement_group_id.c_str());
    y.InstrumentStatus = x.instrument_status;
    y.TradingSegmentSN = x.trading_segment_sn;
    strcpy(y.EnterTime, x.enter_time.c_str());
    y.EnterReason = x.enter_reason;
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

InstrumentStatus Converter::CThostFtdcInstrumentStatusFieldToRust(CThostFtdcInstrumentStatusField* x) {
    if (x == nullptr)
        return InstrumentStatus{.is_null = true};
    InstrumentStatus y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.settlement_group_id = Converter::Gb2312ToRustString(x->SettlementGroupID);
    y.instrument_status = x->InstrumentStatus;
    y.trading_segment_sn = x->TradingSegmentSN;
    y.enter_time = Converter::Gb2312ToRustString(x->EnterTime);
    y.enter_reason = x->EnterReason;
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryInstrumentStatusField Converter::QryInstrumentStatusToCpp(QryInstrumentStatus x) {
    CThostFtdcQryInstrumentStatusField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    return y;
}

QryInstrumentStatus Converter::CThostFtdcQryInstrumentStatusFieldToRust(CThostFtdcQryInstrumentStatusField* x) {
    if (x == nullptr)
        return QryInstrumentStatus{.is_null = true};
    QryInstrumentStatus y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    return y;
}

CThostFtdcInvestorAccountField Converter::InvestorAccountToCpp(InvestorAccount x) {
    CThostFtdcInvestorAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

InvestorAccount Converter::CThostFtdcInvestorAccountFieldToRust(CThostFtdcInvestorAccountField* x) {
    if (x == nullptr)
        return InvestorAccount{.is_null = true};
    InvestorAccount y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcPositionProfitAlgorithmField Converter::PositionProfitAlgorithmToCpp(PositionProfitAlgorithm x) {
    CThostFtdcPositionProfitAlgorithmField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    y.Algorithm = x.algorithm;
    strcpy(y.Memo, x.memo.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

PositionProfitAlgorithm Converter::CThostFtdcPositionProfitAlgorithmFieldToRust(CThostFtdcPositionProfitAlgorithmField* x) {
    if (x == nullptr)
        return PositionProfitAlgorithm{.is_null = true};
    PositionProfitAlgorithm y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.algorithm = x->Algorithm;
    y.memo = Converter::Gb2312ToRustString(x->Memo);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcDiscountField Converter::DiscountToCpp(Discount x) {
    CThostFtdcDiscountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    y.InvestorRange = x.investor_range;
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.Discount = x.discount;
    return y;
}

Discount Converter::CThostFtdcDiscountFieldToRust(CThostFtdcDiscountField* x) {
    if (x == nullptr)
        return Discount{.is_null = true};
    Discount y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_range = x->InvestorRange;
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.discount = x->Discount;
    return y;
}

CThostFtdcQryTransferBankField Converter::QryTransferBankToCpp(QryTransferBank x) {
    CThostFtdcQryTransferBankField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBrchID, x.bank_branch_id.c_str());
    return y;
}

QryTransferBank Converter::CThostFtdcQryTransferBankFieldToRust(CThostFtdcQryTransferBankField* x) {
    if (x == nullptr)
        return QryTransferBank{.is_null = true};
    QryTransferBank y{};
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBrchID);
    return y;
}

CThostFtdcTransferBankField Converter::TransferBankToCpp(TransferBank x) {
    CThostFtdcTransferBankField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBrchID, x.bank_branch_id.c_str());
    strcpy(y.BankName, x.bank_name.c_str());
    y.IsActive = x.is_active;
    return y;
}

TransferBank Converter::CThostFtdcTransferBankFieldToRust(CThostFtdcTransferBankField* x) {
    if (x == nullptr)
        return TransferBank{.is_null = true};
    TransferBank y{};
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBrchID);
    y.bank_name = Converter::Gb2312ToRustString(x->BankName);
    y.is_active = x->IsActive;
    return y;
}

CThostFtdcQryInvestorPositionDetailField Converter::QryInvestorPositionDetailToCpp(QryInvestorPositionDetail x) {
    CThostFtdcQryInvestorPositionDetailField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryInvestorPositionDetail Converter::CThostFtdcQryInvestorPositionDetailFieldToRust(CThostFtdcQryInvestorPositionDetailField* x) {
    if (x == nullptr)
        return QryInvestorPositionDetail{.is_null = true};
    QryInvestorPositionDetail y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcInvestorPositionDetailField Converter::InvestorPositionDetailToCpp(InvestorPositionDetail x) {
    CThostFtdcInvestorPositionDetailField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.Direction = x.direction;
    strcpy(y.OpenDate, x.open_date.c_str());
    strcpy(y.TradeID, x.trade_id.c_str());
    y.Volume = x.volume;
    y.OpenPrice = x.open_price;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    y.TradeType = x.trade_type;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.CloseProfitByDate = x.close_profit_by_date;
    y.CloseProfitByTrade = x.close_profit_by_trade;
    y.PositionProfitByDate = x.position_profit_by_date;
    y.PositionProfitByTrade = x.position_profit_by_trade;
    y.Margin = x.margin;
    y.ExchMargin = x.exch_margin;
    y.MarginRateByMoney = x.margin_rate_by_money;
    y.MarginRateByVolume = x.margin_rate_by_volume;
    y.LastSettlementPrice = x.last_settlement_price;
    y.SettlementPrice = x.settlement_price;
    y.CloseVolume = x.close_volume;
    y.CloseAmount = x.close_amount;
    y.TimeFirstVolume = x.time_first_volume;
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    y.SpecPosiType = x.spec_posi_type;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.CombInstrumentID, x.comb_instrument_id.c_str());
    return y;
}

InvestorPositionDetail Converter::CThostFtdcInvestorPositionDetailFieldToRust(CThostFtdcInvestorPositionDetailField* x) {
    if (x == nullptr)
        return InvestorPositionDetail{.is_null = true};
    InvestorPositionDetail y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.hedge_flag = x->HedgeFlag;
    y.direction = x->Direction;
    y.open_date = Converter::Gb2312ToRustString(x->OpenDate);
    y.trade_id = Converter::Gb2312ToRustString(x->TradeID);
    y.volume = x->Volume;
    y.open_price = x->OpenPrice;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.trade_type = x->TradeType;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.close_profit_by_date = x->CloseProfitByDate;
    y.close_profit_by_trade = x->CloseProfitByTrade;
    y.position_profit_by_date = x->PositionProfitByDate;
    y.position_profit_by_trade = x->PositionProfitByTrade;
    y.margin = x->Margin;
    y.exch_margin = x->ExchMargin;
    y.margin_rate_by_money = x->MarginRateByMoney;
    y.margin_rate_by_volume = x->MarginRateByVolume;
    y.last_settlement_price = x->LastSettlementPrice;
    y.settlement_price = x->SettlementPrice;
    y.close_volume = x->CloseVolume;
    y.close_amount = x->CloseAmount;
    y.time_first_volume = x->TimeFirstVolume;
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.spec_posi_type = x->SpecPosiType;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.comb_instrument_id = Converter::Gb2312ToRustString(x->CombInstrumentID);
    return y;
}

CThostFtdcTradingAccountPasswordField Converter::TradingAccountPasswordToCpp(TradingAccountPassword x) {
    CThostFtdcTradingAccountPasswordField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

TradingAccountPassword Converter::CThostFtdcTradingAccountPasswordFieldToRust(CThostFtdcTradingAccountPasswordField* x) {
    if (x == nullptr)
        return TradingAccountPassword{.is_null = true};
    TradingAccountPassword y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcMDTraderOfferField Converter::MDTraderOfferToCpp(MDTraderOffer x) {
    CThostFtdcMDTraderOfferField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.InstallID = x.install_id;
    strcpy(y.OrderLocalID, x.order_local_id.c_str());
    y.TraderConnectStatus = x.trader_connect_status;
    strcpy(y.ConnectRequestDate, x.connect_request_date.c_str());
    strcpy(y.ConnectRequestTime, x.connect_request_time.c_str());
    strcpy(y.LastReportDate, x.last_report_date.c_str());
    strcpy(y.LastReportTime, x.last_report_time.c_str());
    strcpy(y.ConnectDate, x.connect_date.c_str());
    strcpy(y.ConnectTime, x.connect_time.c_str());
    strcpy(y.StartDate, x.start_date.c_str());
    strcpy(y.StartTime, x.start_time.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.MaxTradeID, x.max_trade_id.c_str());
    memcpy(y.MaxOrderMessageReference, x.max_order_message_reference.data(), x.max_order_message_reference.size() * sizeof(uint8_t));
    y.OrderCancelAlg = x.order_cancel_alg;
    return y;
}

MDTraderOffer Converter::CThostFtdcMDTraderOfferFieldToRust(CThostFtdcMDTraderOfferField* x) {
    if (x == nullptr)
        return MDTraderOffer{.is_null = true};
    MDTraderOffer y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.install_id = x->InstallID;
    y.order_local_id = Converter::Gb2312ToRustString(x->OrderLocalID);
    y.trader_connect_status = x->TraderConnectStatus;
    y.connect_request_date = Converter::Gb2312ToRustString(x->ConnectRequestDate);
    y.connect_request_time = Converter::Gb2312ToRustString(x->ConnectRequestTime);
    y.last_report_date = Converter::Gb2312ToRustString(x->LastReportDate);
    y.last_report_time = Converter::Gb2312ToRustString(x->LastReportTime);
    y.connect_date = Converter::Gb2312ToRustString(x->ConnectDate);
    y.connect_time = Converter::Gb2312ToRustString(x->ConnectTime);
    y.start_date = Converter::Gb2312ToRustString(x->StartDate);
    y.start_time = Converter::Gb2312ToRustString(x->StartTime);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.max_trade_id = Converter::Gb2312ToRustString(x->MaxTradeID);
    for (int i = 0; i < 7; i++)
        y.max_order_message_reference.push_back(x->MaxOrderMessageReference[i]);
    y.order_cancel_alg = x->OrderCancelAlg;
    return y;
}

CThostFtdcQryMDTraderOfferField Converter::QryMDTraderOfferToCpp(QryMDTraderOffer x) {
    CThostFtdcQryMDTraderOfferField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    return y;
}

QryMDTraderOffer Converter::CThostFtdcQryMDTraderOfferFieldToRust(CThostFtdcQryMDTraderOfferField* x) {
    if (x == nullptr)
        return QryMDTraderOffer{.is_null = true};
    QryMDTraderOffer y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    return y;
}

CThostFtdcQryNoticeField Converter::QryNoticeToCpp(QryNotice x) {
    CThostFtdcQryNoticeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    return y;
}

QryNotice Converter::CThostFtdcQryNoticeFieldToRust(CThostFtdcQryNoticeField* x) {
    if (x == nullptr)
        return QryNotice{.is_null = true};
    QryNotice y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    return y;
}

CThostFtdcNoticeField Converter::NoticeToCpp(Notice x) {
    CThostFtdcNoticeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    memcpy(y.Content, x.content.data(), x.content.size() * sizeof(uint8_t));
    memcpy(y.SequenceLabel, x.sequence_label.data(), x.sequence_label.size() * sizeof(uint8_t));
    return y;
}

Notice Converter::CThostFtdcNoticeFieldToRust(CThostFtdcNoticeField* x) {
    if (x == nullptr)
        return Notice{.is_null = true};
    Notice y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    for (int i = 0; i < 501; i++)
        y.content.push_back(x->Content[i]);
    for (int i = 0; i < 2; i++)
        y.sequence_label.push_back(x->SequenceLabel[i]);
    return y;
}

CThostFtdcUserRightField Converter::UserRightToCpp(UserRight x) {
    CThostFtdcUserRightField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.UserRightType = x.user_right_type;
    y.IsForbidden = x.is_forbidden;
    return y;
}

UserRight Converter::CThostFtdcUserRightFieldToRust(CThostFtdcUserRightField* x) {
    if (x == nullptr)
        return UserRight{.is_null = true};
    UserRight y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.user_right_type = x->UserRightType;
    y.is_forbidden = x->IsForbidden;
    return y;
}

CThostFtdcQrySettlementInfoConfirmField Converter::QrySettlementInfoConfirmToCpp(QrySettlementInfoConfirm x) {
    CThostFtdcQrySettlementInfoConfirmField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

QrySettlementInfoConfirm Converter::CThostFtdcQrySettlementInfoConfirmFieldToRust(CThostFtdcQrySettlementInfoConfirmField* x) {
    if (x == nullptr)
        return QrySettlementInfoConfirm{.is_null = true};
    QrySettlementInfoConfirm y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcLoadSettlementInfoField Converter::LoadSettlementInfoToCpp(LoadSettlementInfo x) {
    CThostFtdcLoadSettlementInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    return y;
}

LoadSettlementInfo Converter::CThostFtdcLoadSettlementInfoFieldToRust(CThostFtdcLoadSettlementInfoField* x) {
    if (x == nullptr)
        return LoadSettlementInfo{.is_null = true};
    LoadSettlementInfo y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    return y;
}

CThostFtdcBrokerWithdrawAlgorithmField Converter::BrokerWithdrawAlgorithmToCpp(BrokerWithdrawAlgorithm x) {
    CThostFtdcBrokerWithdrawAlgorithmField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    y.WithdrawAlgorithm = x.withdraw_algorithm;
    y.UsingRatio = x.using_ratio;
    y.IncludeCloseProfit = x.include_close_profit;
    y.AllWithoutTrade = x.all_without_trade;
    y.AvailIncludeCloseProfit = x.avail_include_close_profit;
    y.IsBrokerUserEvent = x.is_broker_user_event;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.FundMortgageRatio = x.fund_mortgage_ratio;
    y.BalanceAlgorithm = x.balance_algorithm;
    return y;
}

BrokerWithdrawAlgorithm Converter::CThostFtdcBrokerWithdrawAlgorithmFieldToRust(CThostFtdcBrokerWithdrawAlgorithmField* x) {
    if (x == nullptr)
        return BrokerWithdrawAlgorithm{.is_null = true};
    BrokerWithdrawAlgorithm y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.withdraw_algorithm = x->WithdrawAlgorithm;
    y.using_ratio = x->UsingRatio;
    y.include_close_profit = x->IncludeCloseProfit;
    y.all_without_trade = x->AllWithoutTrade;
    y.avail_include_close_profit = x->AvailIncludeCloseProfit;
    y.is_broker_user_event = x->IsBrokerUserEvent;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.fund_mortgage_ratio = x->FundMortgageRatio;
    y.balance_algorithm = x->BalanceAlgorithm;
    return y;
}

CThostFtdcTradingAccountPasswordUpdateV1Field Converter::TradingAccountPasswordUpdateV1ToCpp(TradingAccountPasswordUpdateV1 x) {
    CThostFtdcTradingAccountPasswordUpdateV1Field y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.OldPassword, x.old_password.c_str());
    strcpy(y.NewPassword, x.new_password.c_str());
    return y;
}

TradingAccountPasswordUpdateV1 Converter::CThostFtdcTradingAccountPasswordUpdateV1FieldToRust(CThostFtdcTradingAccountPasswordUpdateV1Field* x) {
    if (x == nullptr)
        return TradingAccountPasswordUpdateV1{.is_null = true};
    TradingAccountPasswordUpdateV1 y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.old_password = Converter::Gb2312ToRustString(x->OldPassword);
    y.new_password = Converter::Gb2312ToRustString(x->NewPassword);
    return y;
}

CThostFtdcTradingAccountPasswordUpdateField Converter::TradingAccountPasswordUpdateToCpp(TradingAccountPasswordUpdate x) {
    CThostFtdcTradingAccountPasswordUpdateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.OldPassword, x.old_password.c_str());
    strcpy(y.NewPassword, x.new_password.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

TradingAccountPasswordUpdate Converter::CThostFtdcTradingAccountPasswordUpdateFieldToRust(CThostFtdcTradingAccountPasswordUpdateField* x) {
    if (x == nullptr)
        return TradingAccountPasswordUpdate{.is_null = true};
    TradingAccountPasswordUpdate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.old_password = Converter::Gb2312ToRustString(x->OldPassword);
    y.new_password = Converter::Gb2312ToRustString(x->NewPassword);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcQryCombinationLegField Converter::QryCombinationLegToCpp(QryCombinationLeg x) {
    CThostFtdcQryCombinationLegField y;
    memset(&y, 0, sizeof(y));
    y.LegID = x.leg_id;
    strcpy(y.CombInstrumentID, x.comb_instrument_id.c_str());
    strcpy(y.LegInstrumentID, x.leg_instrument_id.c_str());
    return y;
}

QryCombinationLeg Converter::CThostFtdcQryCombinationLegFieldToRust(CThostFtdcQryCombinationLegField* x) {
    if (x == nullptr)
        return QryCombinationLeg{.is_null = true};
    QryCombinationLeg y{};
    y.leg_id = x->LegID;
    y.comb_instrument_id = Converter::Gb2312ToRustString(x->CombInstrumentID);
    y.leg_instrument_id = Converter::Gb2312ToRustString(x->LegInstrumentID);
    return y;
}

CThostFtdcQrySyncStatusField Converter::QrySyncStatusToCpp(QrySyncStatus x) {
    CThostFtdcQrySyncStatusField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    return y;
}

QrySyncStatus Converter::CThostFtdcQrySyncStatusFieldToRust(CThostFtdcQrySyncStatusField* x) {
    if (x == nullptr)
        return QrySyncStatus{.is_null = true};
    QrySyncStatus y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    return y;
}

CThostFtdcCombinationLegField Converter::CombinationLegToCpp(CombinationLeg x) {
    CThostFtdcCombinationLegField y;
    memset(&y, 0, sizeof(y));
    y.LegID = x.leg_id;
    y.Direction = x.direction;
    y.LegMultiple = x.leg_multiple;
    y.ImplyLevel = x.imply_level;
    strcpy(y.CombInstrumentID, x.comb_instrument_id.c_str());
    strcpy(y.LegInstrumentID, x.leg_instrument_id.c_str());
    return y;
}

CombinationLeg Converter::CThostFtdcCombinationLegFieldToRust(CThostFtdcCombinationLegField* x) {
    if (x == nullptr)
        return CombinationLeg{.is_null = true};
    CombinationLeg y{};
    y.leg_id = x->LegID;
    y.direction = x->Direction;
    y.leg_multiple = x->LegMultiple;
    y.imply_level = x->ImplyLevel;
    y.comb_instrument_id = Converter::Gb2312ToRustString(x->CombInstrumentID);
    y.leg_instrument_id = Converter::Gb2312ToRustString(x->LegInstrumentID);
    return y;
}

CThostFtdcSyncStatusField Converter::SyncStatusToCpp(SyncStatus x) {
    CThostFtdcSyncStatusField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.DataSyncStatus = x.data_sync_status;
    return y;
}

SyncStatus Converter::CThostFtdcSyncStatusFieldToRust(CThostFtdcSyncStatusField* x) {
    if (x == nullptr)
        return SyncStatus{.is_null = true};
    SyncStatus y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.data_sync_status = x->DataSyncStatus;
    return y;
}

CThostFtdcQryLinkManField Converter::QryLinkManToCpp(QryLinkMan x) {
    CThostFtdcQryLinkManField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    return y;
}

QryLinkMan Converter::CThostFtdcQryLinkManFieldToRust(CThostFtdcQryLinkManField* x) {
    if (x == nullptr)
        return QryLinkMan{.is_null = true};
    QryLinkMan y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    return y;
}

CThostFtdcLinkManField Converter::LinkManToCpp(LinkMan x) {
    CThostFtdcLinkManField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.PersonType = x.person_type;
    y.IdentifiedCardType = x.identified_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    strcpy(y.PersonName, x.person_name.c_str());
    strcpy(y.Telephone, x.telephone.c_str());
    strcpy(y.Address, x.address.c_str());
    strcpy(y.ZipCode, x.zip_code.c_str());
    y.Priority = x.priority;
    strcpy(y.UOAZipCode, x.uoa_zip_code.c_str());
    strcpy(y.PersonFullName, x.person_full_name.c_str());
    return y;
}

LinkMan Converter::CThostFtdcLinkManFieldToRust(CThostFtdcLinkManField* x) {
    if (x == nullptr)
        return LinkMan{.is_null = true};
    LinkMan y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.person_type = x->PersonType;
    y.identified_card_type = x->IdentifiedCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.person_name = Converter::Gb2312ToRustString(x->PersonName);
    y.telephone = Converter::Gb2312ToRustString(x->Telephone);
    y.address = Converter::Gb2312ToRustString(x->Address);
    y.zip_code = Converter::Gb2312ToRustString(x->ZipCode);
    y.priority = x->Priority;
    y.uoa_zip_code = Converter::Gb2312ToRustString(x->UOAZipCode);
    y.person_full_name = Converter::Gb2312ToRustString(x->PersonFullName);
    return y;
}

CThostFtdcQryBrokerUserEventField Converter::QryBrokerUserEventToCpp(QryBrokerUserEvent x) {
    CThostFtdcQryBrokerUserEventField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.UserEventType = x.user_event_type;
    return y;
}

QryBrokerUserEvent Converter::CThostFtdcQryBrokerUserEventFieldToRust(CThostFtdcQryBrokerUserEventField* x) {
    if (x == nullptr)
        return QryBrokerUserEvent{.is_null = true};
    QryBrokerUserEvent y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.user_event_type = x->UserEventType;
    return y;
}

CThostFtdcBrokerUserEventField Converter::BrokerUserEventToCpp(BrokerUserEvent x) {
    CThostFtdcBrokerUserEventField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.UserEventType = x.user_event_type;
    y.EventSequenceNo = x.event_sequence_no;
    strcpy(y.EventDate, x.event_date.c_str());
    strcpy(y.EventTime, x.event_time.c_str());
    strcpy(y.UserEventInfo, x.user_event_info.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.DRIdentityID = x.dr_identity_id;
    strcpy(y.TradingDay, x.trading_day.c_str());
    return y;
}

BrokerUserEvent Converter::CThostFtdcBrokerUserEventFieldToRust(CThostFtdcBrokerUserEventField* x) {
    if (x == nullptr)
        return BrokerUserEvent{.is_null = true};
    BrokerUserEvent y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.user_event_type = x->UserEventType;
    y.event_sequence_no = x->EventSequenceNo;
    y.event_date = Converter::Gb2312ToRustString(x->EventDate);
    y.event_time = Converter::Gb2312ToRustString(x->EventTime);
    y.user_event_info = Converter::Gb2312ToRustString(x->UserEventInfo);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.dr_identity_id = x->DRIdentityID;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    return y;
}

CThostFtdcQryContractBankField Converter::QryContractBankToCpp(QryContractBank x) {
    CThostFtdcQryContractBankField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBrchID, x.bank_branch_id.c_str());
    return y;
}

QryContractBank Converter::CThostFtdcQryContractBankFieldToRust(CThostFtdcQryContractBankField* x) {
    if (x == nullptr)
        return QryContractBank{.is_null = true};
    QryContractBank y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBrchID);
    return y;
}

CThostFtdcContractBankField Converter::ContractBankToCpp(ContractBank x) {
    CThostFtdcContractBankField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBrchID, x.bank_branch_id.c_str());
    strcpy(y.BankName, x.bank_name.c_str());
    return y;
}

ContractBank Converter::CThostFtdcContractBankFieldToRust(CThostFtdcContractBankField* x) {
    if (x == nullptr)
        return ContractBank{.is_null = true};
    ContractBank y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBrchID);
    y.bank_name = Converter::Gb2312ToRustString(x->BankName);
    return y;
}

CThostFtdcInvestorPositionCombineDetailField Converter::InvestorPositionCombineDetailToCpp(InvestorPositionCombineDetail x) {
    CThostFtdcInvestorPositionCombineDetailField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.OpenDate, x.open_date.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ComTradeID, x.com_trade_id.c_str());
    strcpy(y.TradeID, x.trade_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.Direction = x.direction;
    y.TotalAmt = x.total_amt;
    y.Margin = x.margin;
    y.ExchMargin = x.exch_margin;
    y.MarginRateByMoney = x.margin_rate_by_money;
    y.MarginRateByVolume = x.margin_rate_by_volume;
    y.LegID = x.leg_id;
    y.LegMultiple = x.leg_multiple;
    y.TradeGroupID = x.trade_group_id;
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.CombInstrumentID, x.comb_instrument_id.c_str());
    return y;
}

InvestorPositionCombineDetail Converter::CThostFtdcInvestorPositionCombineDetailFieldToRust(CThostFtdcInvestorPositionCombineDetailField* x) {
    if (x == nullptr)
        return InvestorPositionCombineDetail{.is_null = true};
    InvestorPositionCombineDetail y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.open_date = Converter::Gb2312ToRustString(x->OpenDate);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.settlement_id = x->SettlementID;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.com_trade_id = Converter::Gb2312ToRustString(x->ComTradeID);
    y.trade_id = Converter::Gb2312ToRustString(x->TradeID);
    y.hedge_flag = x->HedgeFlag;
    y.direction = x->Direction;
    y.total_amt = x->TotalAmt;
    y.margin = x->Margin;
    y.exch_margin = x->ExchMargin;
    y.margin_rate_by_money = x->MarginRateByMoney;
    y.margin_rate_by_volume = x->MarginRateByVolume;
    y.leg_id = x->LegID;
    y.leg_multiple = x->LegMultiple;
    y.trade_group_id = x->TradeGroupID;
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.comb_instrument_id = Converter::Gb2312ToRustString(x->CombInstrumentID);
    return y;
}

CThostFtdcParkedOrderField Converter::ParkedOrderToCpp(ParkedOrder x) {
    CThostFtdcParkedOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.OrderRef, x.order_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.OrderPriceType = x.order_price_type;
    y.Direction = x.direction;
    strcpy(y.CombOffsetFlag, x.comb_offset_flag.c_str());
    strcpy(y.CombHedgeFlag, x.comb_hedge_flag.c_str());
    y.LimitPrice = x.limit_price;
    y.VolumeTotalOriginal = x.volume_total_original;
    y.TimeCondition = x.time_condition;
    strcpy(y.GTDDate, x.gtd_date.c_str());
    y.VolumeCondition = x.volume_condition;
    y.MinVolume = x.min_volume;
    y.ContingentCondition = x.contingent_condition;
    y.StopPrice = x.stop_price;
    y.ForceCloseReason = x.force_close_reason;
    y.IsAutoSuspend = x.is_auto_suspend;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.RequestID = x.request_id;
    y.UserForceClose = x.user_force_close;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParkedOrderID, x.parked_order_id.c_str());
    y.UserType = x.user_type;
    y.Status = x.status;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    y.IsSwapOrder = x.is_swap_order;
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ParkedOrder Converter::CThostFtdcParkedOrderFieldToRust(CThostFtdcParkedOrderField* x) {
    if (x == nullptr)
        return ParkedOrder{.is_null = true};
    ParkedOrder y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.order_ref = Converter::Gb2312ToRustString(x->OrderRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.order_price_type = x->OrderPriceType;
    y.direction = x->Direction;
    y.comb_offset_flag = Converter::Gb2312ToRustString(x->CombOffsetFlag);
    y.comb_hedge_flag = Converter::Gb2312ToRustString(x->CombHedgeFlag);
    y.limit_price = x->LimitPrice;
    y.volume_total_original = x->VolumeTotalOriginal;
    y.time_condition = x->TimeCondition;
    y.gtd_date = Converter::Gb2312ToRustString(x->GTDDate);
    y.volume_condition = x->VolumeCondition;
    y.min_volume = x->MinVolume;
    y.contingent_condition = x->ContingentCondition;
    y.stop_price = x->StopPrice;
    y.force_close_reason = x->ForceCloseReason;
    y.is_auto_suspend = x->IsAutoSuspend;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.request_id = x->RequestID;
    y.user_force_close = x->UserForceClose;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.parked_order_id = Converter::Gb2312ToRustString(x->ParkedOrderID);
    y.user_type = x->UserType;
    y.status = x->Status;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.is_swap_order = x->IsSwapOrder;
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcParkedOrderActionField Converter::ParkedOrderActionToCpp(ParkedOrderAction x) {
    CThostFtdcParkedOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OrderActionRef = x.order_action_ref;
    strcpy(y.OrderRef, x.order_ref.c_str());
    y.RequestID = x.request_id;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.OrderSysID, x.order_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    y.LimitPrice = x.limit_price;
    y.VolumeChange = x.volume_change;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.ParkedOrderActionID, x.parked_order_action_id.c_str());
    y.UserType = x.user_type;
    y.Status = x.status;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ParkedOrderAction Converter::CThostFtdcParkedOrderActionFieldToRust(CThostFtdcParkedOrderActionField* x) {
    if (x == nullptr)
        return ParkedOrderAction{.is_null = true};
    ParkedOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.order_action_ref = x->OrderActionRef;
    y.order_ref = Converter::Gb2312ToRustString(x->OrderRef);
    y.request_id = x->RequestID;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.order_sys_id = Converter::Gb2312ToRustString(x->OrderSysID);
    y.action_flag = x->ActionFlag;
    y.limit_price = x->LimitPrice;
    y.volume_change = x->VolumeChange;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.parked_order_action_id = Converter::Gb2312ToRustString(x->ParkedOrderActionID);
    y.user_type = x->UserType;
    y.status = x->Status;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryParkedOrderField Converter::QryParkedOrderToCpp(QryParkedOrder x) {
    CThostFtdcQryParkedOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryParkedOrder Converter::CThostFtdcQryParkedOrderFieldToRust(CThostFtdcQryParkedOrderField* x) {
    if (x == nullptr)
        return QryParkedOrder{.is_null = true};
    QryParkedOrder y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryParkedOrderActionField Converter::QryParkedOrderActionToCpp(QryParkedOrderAction x) {
    CThostFtdcQryParkedOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryParkedOrderAction Converter::CThostFtdcQryParkedOrderActionFieldToRust(CThostFtdcQryParkedOrderActionField* x) {
    if (x == nullptr)
        return QryParkedOrderAction{.is_null = true};
    QryParkedOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcRemoveParkedOrderField Converter::RemoveParkedOrderToCpp(RemoveParkedOrder x) {
    CThostFtdcRemoveParkedOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ParkedOrderID, x.parked_order_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    return y;
}

RemoveParkedOrder Converter::CThostFtdcRemoveParkedOrderFieldToRust(CThostFtdcRemoveParkedOrderField* x) {
    if (x == nullptr)
        return RemoveParkedOrder{.is_null = true};
    RemoveParkedOrder y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.parked_order_id = Converter::Gb2312ToRustString(x->ParkedOrderID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    return y;
}

CThostFtdcRemoveParkedOrderActionField Converter::RemoveParkedOrderActionToCpp(RemoveParkedOrderAction x) {
    CThostFtdcRemoveParkedOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ParkedOrderActionID, x.parked_order_action_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    return y;
}

RemoveParkedOrderAction Converter::CThostFtdcRemoveParkedOrderActionFieldToRust(CThostFtdcRemoveParkedOrderActionField* x) {
    if (x == nullptr)
        return RemoveParkedOrderAction{.is_null = true};
    RemoveParkedOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.parked_order_action_id = Converter::Gb2312ToRustString(x->ParkedOrderActionID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    return y;
}

CThostFtdcInvestorWithdrawAlgorithmField Converter::InvestorWithdrawAlgorithmToCpp(InvestorWithdrawAlgorithm x) {
    CThostFtdcInvestorWithdrawAlgorithmField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    y.InvestorRange = x.investor_range;
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.UsingRatio = x.using_ratio;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.FundMortgageRatio = x.fund_mortgage_ratio;
    return y;
}

InvestorWithdrawAlgorithm Converter::CThostFtdcInvestorWithdrawAlgorithmFieldToRust(CThostFtdcInvestorWithdrawAlgorithmField* x) {
    if (x == nullptr)
        return InvestorWithdrawAlgorithm{.is_null = true};
    InvestorWithdrawAlgorithm y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_range = x->InvestorRange;
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.using_ratio = x->UsingRatio;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.fund_mortgage_ratio = x->FundMortgageRatio;
    return y;
}

CThostFtdcQryInvestorPositionCombineDetailField Converter::QryInvestorPositionCombineDetailToCpp(QryInvestorPositionCombineDetail x) {
    CThostFtdcQryInvestorPositionCombineDetailField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.CombInstrumentID, x.comb_instrument_id.c_str());
    return y;
}

QryInvestorPositionCombineDetail Converter::CThostFtdcQryInvestorPositionCombineDetailFieldToRust(CThostFtdcQryInvestorPositionCombineDetailField* x) {
    if (x == nullptr)
        return QryInvestorPositionCombineDetail{.is_null = true};
    QryInvestorPositionCombineDetail y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.comb_instrument_id = Converter::Gb2312ToRustString(x->CombInstrumentID);
    return y;
}

CThostFtdcMarketDataAveragePriceField Converter::MarketDataAveragePriceToCpp(MarketDataAveragePrice x) {
    CThostFtdcMarketDataAveragePriceField y;
    memset(&y, 0, sizeof(y));
    y.AveragePrice = x.average_price;
    return y;
}

MarketDataAveragePrice Converter::CThostFtdcMarketDataAveragePriceFieldToRust(CThostFtdcMarketDataAveragePriceField* x) {
    if (x == nullptr)
        return MarketDataAveragePrice{.is_null = true};
    MarketDataAveragePrice y{};
    y.average_price = x->AveragePrice;
    return y;
}

CThostFtdcVerifyInvestorPasswordField Converter::VerifyInvestorPasswordToCpp(VerifyInvestorPassword x) {
    CThostFtdcVerifyInvestorPasswordField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.Password, x.password.c_str());
    return y;
}

VerifyInvestorPassword Converter::CThostFtdcVerifyInvestorPasswordFieldToRust(CThostFtdcVerifyInvestorPasswordField* x) {
    if (x == nullptr)
        return VerifyInvestorPassword{.is_null = true};
    VerifyInvestorPassword y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    return y;
}

CThostFtdcUserIPField Converter::UserIPToCpp(UserIP x) {
    CThostFtdcUserIPField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    memcpy(y.IPMask, x.ip_mask.data(), x.ip_mask.size() * sizeof(uint8_t));
    return y;
}

UserIP Converter::CThostFtdcUserIPFieldToRust(CThostFtdcUserIPField* x) {
    if (x == nullptr)
        return UserIP{.is_null = true};
    UserIP y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    for (int i = 0; i < 33; i++)
        y.ip_mask.push_back(x->IPMask[i]);
    return y;
}

CThostFtdcTradingNoticeInfoField Converter::TradingNoticeInfoToCpp(TradingNoticeInfo x) {
    CThostFtdcTradingNoticeInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.SendTime, x.send_time.c_str());
    memcpy(y.FieldContent, x.field_content.data(), x.field_content.size() * sizeof(uint8_t));
    y.SequenceSeries = x.sequence_series;
    y.SequenceNo = x.sequence_no;
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    return y;
}

TradingNoticeInfo Converter::CThostFtdcTradingNoticeInfoFieldToRust(CThostFtdcTradingNoticeInfoField* x) {
    if (x == nullptr)
        return TradingNoticeInfo{.is_null = true};
    TradingNoticeInfo y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.send_time = Converter::Gb2312ToRustString(x->SendTime);
    for (int i = 0; i < 501; i++)
        y.field_content.push_back(x->FieldContent[i]);
    y.sequence_series = x->SequenceSeries;
    y.sequence_no = x->SequenceNo;
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    return y;
}

CThostFtdcTradingNoticeField Converter::TradingNoticeToCpp(TradingNotice x) {
    CThostFtdcTradingNoticeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    y.InvestorRange = x.investor_range;
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.SequenceSeries = x.sequence_series;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.SendTime, x.send_time.c_str());
    y.SequenceNo = x.sequence_no;
    memcpy(y.FieldContent, x.field_content.data(), x.field_content.size() * sizeof(uint8_t));
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    return y;
}

TradingNotice Converter::CThostFtdcTradingNoticeFieldToRust(CThostFtdcTradingNoticeField* x) {
    if (x == nullptr)
        return TradingNotice{.is_null = true};
    TradingNotice y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_range = x->InvestorRange;
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.sequence_series = x->SequenceSeries;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.send_time = Converter::Gb2312ToRustString(x->SendTime);
    y.sequence_no = x->SequenceNo;
    for (int i = 0; i < 501; i++)
        y.field_content.push_back(x->FieldContent[i]);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    return y;
}

CThostFtdcQryTradingNoticeField Converter::QryTradingNoticeToCpp(QryTradingNotice x) {
    CThostFtdcQryTradingNoticeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    return y;
}

QryTradingNotice Converter::CThostFtdcQryTradingNoticeFieldToRust(CThostFtdcQryTradingNoticeField* x) {
    if (x == nullptr)
        return QryTradingNotice{.is_null = true};
    QryTradingNotice y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    return y;
}

CThostFtdcQryErrOrderField Converter::QryErrOrderToCpp(QryErrOrder x) {
    CThostFtdcQryErrOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    return y;
}

QryErrOrder Converter::CThostFtdcQryErrOrderFieldToRust(CThostFtdcQryErrOrderField* x) {
    if (x == nullptr)
        return QryErrOrder{.is_null = true};
    QryErrOrder y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    return y;
}

CThostFtdcErrOrderField Converter::ErrOrderToCpp(ErrOrder x) {
    CThostFtdcErrOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.OrderRef, x.order_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.OrderPriceType = x.order_price_type;
    y.Direction = x.direction;
    strcpy(y.CombOffsetFlag, x.comb_offset_flag.c_str());
    strcpy(y.CombHedgeFlag, x.comb_hedge_flag.c_str());
    y.LimitPrice = x.limit_price;
    y.VolumeTotalOriginal = x.volume_total_original;
    y.TimeCondition = x.time_condition;
    strcpy(y.GTDDate, x.gtd_date.c_str());
    y.VolumeCondition = x.volume_condition;
    y.MinVolume = x.min_volume;
    y.ContingentCondition = x.contingent_condition;
    y.StopPrice = x.stop_price;
    y.ForceCloseReason = x.force_close_reason;
    y.IsAutoSuspend = x.is_auto_suspend;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.RequestID = x.request_id;
    y.UserForceClose = x.user_force_close;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    y.IsSwapOrder = x.is_swap_order;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.OrderMemo, x.order_memo.c_str());
    y.SessionReqSeq = x.session_req_seq;
    return y;
}

ErrOrder Converter::CThostFtdcErrOrderFieldToRust(CThostFtdcErrOrderField* x) {
    if (x == nullptr)
        return ErrOrder{.is_null = true};
    ErrOrder y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.order_ref = Converter::Gb2312ToRustString(x->OrderRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.order_price_type = x->OrderPriceType;
    y.direction = x->Direction;
    y.comb_offset_flag = Converter::Gb2312ToRustString(x->CombOffsetFlag);
    y.comb_hedge_flag = Converter::Gb2312ToRustString(x->CombHedgeFlag);
    y.limit_price = x->LimitPrice;
    y.volume_total_original = x->VolumeTotalOriginal;
    y.time_condition = x->TimeCondition;
    y.gtd_date = Converter::Gb2312ToRustString(x->GTDDate);
    y.volume_condition = x->VolumeCondition;
    y.min_volume = x->MinVolume;
    y.contingent_condition = x->ContingentCondition;
    y.stop_price = x->StopPrice;
    y.force_close_reason = x->ForceCloseReason;
    y.is_auto_suspend = x->IsAutoSuspend;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.request_id = x->RequestID;
    y.user_force_close = x->UserForceClose;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.is_swap_order = x->IsSwapOrder;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.order_memo = Converter::Gb2312ToRustString(x->OrderMemo);
    y.session_req_seq = x->SessionReqSeq;
    return y;
}

CThostFtdcErrorConditionalOrderField Converter::ErrorConditionalOrderToCpp(ErrorConditionalOrder x) {
    CThostFtdcErrorConditionalOrderField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.OrderRef, x.order_ref.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.OrderPriceType = x.order_price_type;
    y.Direction = x.direction;
    strcpy(y.CombOffsetFlag, x.comb_offset_flag.c_str());
    strcpy(y.CombHedgeFlag, x.comb_hedge_flag.c_str());
    y.LimitPrice = x.limit_price;
    y.VolumeTotalOriginal = x.volume_total_original;
    y.TimeCondition = x.time_condition;
    strcpy(y.GTDDate, x.gtd_date.c_str());
    y.VolumeCondition = x.volume_condition;
    y.MinVolume = x.min_volume;
    y.ContingentCondition = x.contingent_condition;
    y.StopPrice = x.stop_price;
    y.ForceCloseReason = x.force_close_reason;
    y.IsAutoSuspend = x.is_auto_suspend;
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.RequestID = x.request_id;
    strcpy(y.OrderLocalID, x.order_local_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    y.OrderSubmitStatus = x.order_submit_status;
    y.NotifySequence = x.notify_sequence;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.OrderSysID, x.order_sys_id.c_str());
    y.OrderSource = x.order_source;
    y.OrderStatus = x.order_status;
    y.OrderType = x.order_type;
    y.VolumeTraded = x.volume_traded;
    y.VolumeTotal = x.volume_total;
    strcpy(y.InsertDate, x.insert_date.c_str());
    strcpy(y.InsertTime, x.insert_time.c_str());
    strcpy(y.ActiveTime, x.active_time.c_str());
    strcpy(y.SuspendTime, x.suspend_time.c_str());
    strcpy(y.UpdateTime, x.update_time.c_str());
    strcpy(y.CancelTime, x.cancel_time.c_str());
    strcpy(y.ActiveTraderID, x.active_trader_id.c_str());
    strcpy(y.ClearingPartID, x.clearing_part_id.c_str());
    y.SequenceNo = x.sequence_no;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.StatusMsg, x.status_msg.c_str());
    y.UserForceClose = x.user_force_close;
    strcpy(y.ActiveUserID, x.active_user_id.c_str());
    y.BrokerOrderSeq = x.broker_order_seq;
    strcpy(y.RelativeOrderSysID, x.relative_order_sys_id.c_str());
    y.ZCETotalTradedVolume = x.zce_total_traded_volume;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    y.IsSwapOrder = x.is_swap_order;
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

ErrorConditionalOrder Converter::CThostFtdcErrorConditionalOrderFieldToRust(CThostFtdcErrorConditionalOrderField* x) {
    if (x == nullptr)
        return ErrorConditionalOrder{.is_null = true};
    ErrorConditionalOrder y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.order_ref = Converter::Gb2312ToRustString(x->OrderRef);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.order_price_type = x->OrderPriceType;
    y.direction = x->Direction;
    y.comb_offset_flag = Converter::Gb2312ToRustString(x->CombOffsetFlag);
    y.comb_hedge_flag = Converter::Gb2312ToRustString(x->CombHedgeFlag);
    y.limit_price = x->LimitPrice;
    y.volume_total_original = x->VolumeTotalOriginal;
    y.time_condition = x->TimeCondition;
    y.gtd_date = Converter::Gb2312ToRustString(x->GTDDate);
    y.volume_condition = x->VolumeCondition;
    y.min_volume = x->MinVolume;
    y.contingent_condition = x->ContingentCondition;
    y.stop_price = x->StopPrice;
    y.force_close_reason = x->ForceCloseReason;
    y.is_auto_suspend = x->IsAutoSuspend;
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.request_id = x->RequestID;
    y.order_local_id = Converter::Gb2312ToRustString(x->OrderLocalID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.order_submit_status = x->OrderSubmitStatus;
    y.notify_sequence = x->NotifySequence;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.order_sys_id = Converter::Gb2312ToRustString(x->OrderSysID);
    y.order_source = x->OrderSource;
    y.order_status = x->OrderStatus;
    y.order_type = x->OrderType;
    y.volume_traded = x->VolumeTraded;
    y.volume_total = x->VolumeTotal;
    y.insert_date = Converter::Gb2312ToRustString(x->InsertDate);
    y.insert_time = Converter::Gb2312ToRustString(x->InsertTime);
    y.active_time = Converter::Gb2312ToRustString(x->ActiveTime);
    y.suspend_time = Converter::Gb2312ToRustString(x->SuspendTime);
    y.update_time = Converter::Gb2312ToRustString(x->UpdateTime);
    y.cancel_time = Converter::Gb2312ToRustString(x->CancelTime);
    y.active_trader_id = Converter::Gb2312ToRustString(x->ActiveTraderID);
    y.clearing_part_id = Converter::Gb2312ToRustString(x->ClearingPartID);
    y.sequence_no = x->SequenceNo;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.user_force_close = x->UserForceClose;
    y.active_user_id = Converter::Gb2312ToRustString(x->ActiveUserID);
    y.broker_order_seq = x->BrokerOrderSeq;
    y.relative_order_sys_id = Converter::Gb2312ToRustString(x->RelativeOrderSysID);
    y.zce_total_traded_volume = x->ZCETotalTradedVolume;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.is_swap_order = x->IsSwapOrder;
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryErrOrderActionField Converter::QryErrOrderActionToCpp(QryErrOrderAction x) {
    CThostFtdcQryErrOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    return y;
}

QryErrOrderAction Converter::CThostFtdcQryErrOrderActionFieldToRust(CThostFtdcQryErrOrderActionField* x) {
    if (x == nullptr)
        return QryErrOrderAction{.is_null = true};
    QryErrOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    return y;
}

CThostFtdcErrOrderActionField Converter::ErrOrderActionToCpp(ErrOrderAction x) {
    CThostFtdcErrOrderActionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OrderActionRef = x.order_action_ref;
    strcpy(y.OrderRef, x.order_ref.c_str());
    y.RequestID = x.request_id;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.OrderSysID, x.order_sys_id.c_str());
    y.ActionFlag = x.action_flag;
    y.LimitPrice = x.limit_price;
    y.VolumeChange = x.volume_change;
    strcpy(y.ActionDate, x.action_date.c_str());
    strcpy(y.ActionTime, x.action_time.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.OrderLocalID, x.order_local_id.c_str());
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.BusinessUnit, x.business_unit.c_str());
    y.OrderActionStatus = x.order_action_status;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.StatusMsg, x.status_msg.c_str());
    strcpy(y.BranchID, x.branch_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.OrderMemo, x.order_memo.c_str());
    y.SessionReqSeq = x.session_req_seq;
    return y;
}

ErrOrderAction Converter::CThostFtdcErrOrderActionFieldToRust(CThostFtdcErrOrderActionField* x) {
    if (x == nullptr)
        return ErrOrderAction{.is_null = true};
    ErrOrderAction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.order_action_ref = x->OrderActionRef;
    y.order_ref = Converter::Gb2312ToRustString(x->OrderRef);
    y.request_id = x->RequestID;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.order_sys_id = Converter::Gb2312ToRustString(x->OrderSysID);
    y.action_flag = x->ActionFlag;
    y.limit_price = x->LimitPrice;
    y.volume_change = x->VolumeChange;
    y.action_date = Converter::Gb2312ToRustString(x->ActionDate);
    y.action_time = Converter::Gb2312ToRustString(x->ActionTime);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.order_local_id = Converter::Gb2312ToRustString(x->OrderLocalID);
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.business_unit = Converter::Gb2312ToRustString(x->BusinessUnit);
    y.order_action_status = x->OrderActionStatus;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.branch_id = Converter::Gb2312ToRustString(x->BranchID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.order_memo = Converter::Gb2312ToRustString(x->OrderMemo);
    y.session_req_seq = x->SessionReqSeq;
    return y;
}

CThostFtdcQryExchangeSequenceField Converter::QryExchangeSequenceToCpp(QryExchangeSequence x) {
    CThostFtdcQryExchangeSequenceField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    return y;
}

QryExchangeSequence Converter::CThostFtdcQryExchangeSequenceFieldToRust(CThostFtdcQryExchangeSequenceField* x) {
    if (x == nullptr)
        return QryExchangeSequence{.is_null = true};
    QryExchangeSequence y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    return y;
}

CThostFtdcExchangeSequenceField Converter::ExchangeSequenceToCpp(ExchangeSequence x) {
    CThostFtdcExchangeSequenceField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.SequenceNo = x.sequence_no;
    y.MarketStatus = x.market_status;
    return y;
}

ExchangeSequence Converter::CThostFtdcExchangeSequenceFieldToRust(CThostFtdcExchangeSequenceField* x) {
    if (x == nullptr)
        return ExchangeSequence{.is_null = true};
    ExchangeSequence y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.sequence_no = x->SequenceNo;
    y.market_status = x->MarketStatus;
    return y;
}

CThostFtdcQryMaxOrderVolumeWithPriceField Converter::QryMaxOrderVolumeWithPriceToCpp(QryMaxOrderVolumeWithPrice x) {
    CThostFtdcQryMaxOrderVolumeWithPriceField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.Direction = x.direction;
    y.OffsetFlag = x.offset_flag;
    y.HedgeFlag = x.hedge_flag;
    y.MaxVolume = x.max_volume;
    y.Price = x.price;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryMaxOrderVolumeWithPrice Converter::CThostFtdcQryMaxOrderVolumeWithPriceFieldToRust(CThostFtdcQryMaxOrderVolumeWithPriceField* x) {
    if (x == nullptr)
        return QryMaxOrderVolumeWithPrice{.is_null = true};
    QryMaxOrderVolumeWithPrice y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.direction = x->Direction;
    y.offset_flag = x->OffsetFlag;
    y.hedge_flag = x->HedgeFlag;
    y.max_volume = x->MaxVolume;
    y.price = x->Price;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryBrokerTradingParamsField Converter::QryBrokerTradingParamsToCpp(QryBrokerTradingParams x) {
    CThostFtdcQryBrokerTradingParamsField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    return y;
}

QryBrokerTradingParams Converter::CThostFtdcQryBrokerTradingParamsFieldToRust(CThostFtdcQryBrokerTradingParamsField* x) {
    if (x == nullptr)
        return QryBrokerTradingParams{.is_null = true};
    QryBrokerTradingParams y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    return y;
}

CThostFtdcBrokerTradingParamsField Converter::BrokerTradingParamsToCpp(BrokerTradingParams x) {
    CThostFtdcBrokerTradingParamsField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.MarginPriceType = x.margin_price_type;
    y.Algorithm = x.algorithm;
    y.AvailIncludeCloseProfit = x.avail_include_close_profit;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.OptionRoyaltyPriceType = x.option_royalty_price_type;
    strcpy(y.AccountID, x.account_id.c_str());
    return y;
}

BrokerTradingParams Converter::CThostFtdcBrokerTradingParamsFieldToRust(CThostFtdcBrokerTradingParamsField* x) {
    if (x == nullptr)
        return BrokerTradingParams{.is_null = true};
    BrokerTradingParams y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.margin_price_type = x->MarginPriceType;
    y.algorithm = x->Algorithm;
    y.avail_include_close_profit = x->AvailIncludeCloseProfit;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.option_royalty_price_type = x->OptionRoyaltyPriceType;
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    return y;
}

CThostFtdcQryBrokerTradingAlgosField Converter::QryBrokerTradingAlgosToCpp(QryBrokerTradingAlgos x) {
    CThostFtdcQryBrokerTradingAlgosField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryBrokerTradingAlgos Converter::CThostFtdcQryBrokerTradingAlgosFieldToRust(CThostFtdcQryBrokerTradingAlgosField* x) {
    if (x == nullptr)
        return QryBrokerTradingAlgos{.is_null = true};
    QryBrokerTradingAlgos y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcBrokerTradingAlgosField Converter::BrokerTradingAlgosToCpp(BrokerTradingAlgos x) {
    CThostFtdcBrokerTradingAlgosField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.HandlePositionAlgoID = x.handle_position_algo_id;
    y.FindMarginRateAlgoID = x.find_margin_rate_algo_id;
    y.HandleTradingAccountAlgoID = x.handle_trading_account_algo_id;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

BrokerTradingAlgos Converter::CThostFtdcBrokerTradingAlgosFieldToRust(CThostFtdcBrokerTradingAlgosField* x) {
    if (x == nullptr)
        return BrokerTradingAlgos{.is_null = true};
    BrokerTradingAlgos y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.handle_position_algo_id = x->HandlePositionAlgoID;
    y.find_margin_rate_algo_id = x->FindMarginRateAlgoID;
    y.handle_trading_account_algo_id = x->HandleTradingAccountAlgoID;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQueryBrokerDepositField Converter::QueryBrokerDepositToCpp(QueryBrokerDeposit x) {
    CThostFtdcQueryBrokerDepositField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    return y;
}

QueryBrokerDeposit Converter::CThostFtdcQueryBrokerDepositFieldToRust(CThostFtdcQueryBrokerDepositField* x) {
    if (x == nullptr)
        return QueryBrokerDeposit{.is_null = true};
    QueryBrokerDeposit y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    return y;
}

CThostFtdcBrokerDepositField Converter::BrokerDepositToCpp(BrokerDeposit x) {
    CThostFtdcBrokerDepositField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.PreBalance = x.pre_balance;
    y.CurrMargin = x.curr_margin;
    y.CloseProfit = x.close_profit;
    y.Balance = x.balance;
    y.Deposit = x.deposit;
    y.Withdraw = x.withdraw;
    y.Available = x.available;
    y.Reserve = x.reserve;
    y.FrozenMargin = x.frozen_margin;
    return y;
}

BrokerDeposit Converter::CThostFtdcBrokerDepositFieldToRust(CThostFtdcBrokerDepositField* x) {
    if (x == nullptr)
        return BrokerDeposit{.is_null = true};
    BrokerDeposit y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.pre_balance = x->PreBalance;
    y.curr_margin = x->CurrMargin;
    y.close_profit = x->CloseProfit;
    y.balance = x->Balance;
    y.deposit = x->Deposit;
    y.withdraw = x->Withdraw;
    y.available = x->Available;
    y.reserve = x->Reserve;
    y.frozen_margin = x->FrozenMargin;
    return y;
}

CThostFtdcQryCFMMCBrokerKeyField Converter::QryCFMMCBrokerKeyToCpp(QryCFMMCBrokerKey x) {
    CThostFtdcQryCFMMCBrokerKeyField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    return y;
}

QryCFMMCBrokerKey Converter::CThostFtdcQryCFMMCBrokerKeyFieldToRust(CThostFtdcQryCFMMCBrokerKeyField* x) {
    if (x == nullptr)
        return QryCFMMCBrokerKey{.is_null = true};
    QryCFMMCBrokerKey y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    return y;
}

CThostFtdcCFMMCBrokerKeyField Converter::CFMMCBrokerKeyToCpp(CFMMCBrokerKey x) {
    CThostFtdcCFMMCBrokerKeyField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.CreateDate, x.create_date.c_str());
    strcpy(y.CreateTime, x.create_time.c_str());
    y.KeyID = x.key_id;
    memcpy(y.CurrentKey, x.current_key.data(), x.current_key.size() * sizeof(uint8_t));
    y.KeyKind = x.key_kind;
    return y;
}

CFMMCBrokerKey Converter::CThostFtdcCFMMCBrokerKeyFieldToRust(CThostFtdcCFMMCBrokerKeyField* x) {
    if (x == nullptr)
        return CFMMCBrokerKey{.is_null = true};
    CFMMCBrokerKey y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.create_date = Converter::Gb2312ToRustString(x->CreateDate);
    y.create_time = Converter::Gb2312ToRustString(x->CreateTime);
    y.key_id = x->KeyID;
    for (int i = 0; i < 21; i++)
        y.current_key.push_back(x->CurrentKey[i]);
    y.key_kind = x->KeyKind;
    return y;
}

CThostFtdcCFMMCTradingAccountKeyField Converter::CFMMCTradingAccountKeyToCpp(CFMMCTradingAccountKey x) {
    CThostFtdcCFMMCTradingAccountKeyField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    y.KeyID = x.key_id;
    memcpy(y.CurrentKey, x.current_key.data(), x.current_key.size() * sizeof(uint8_t));
    return y;
}

CFMMCTradingAccountKey Converter::CThostFtdcCFMMCTradingAccountKeyFieldToRust(CThostFtdcCFMMCTradingAccountKeyField* x) {
    if (x == nullptr)
        return CFMMCTradingAccountKey{.is_null = true};
    CFMMCTradingAccountKey y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.key_id = x->KeyID;
    for (int i = 0; i < 21; i++)
        y.current_key.push_back(x->CurrentKey[i]);
    return y;
}

CThostFtdcQryCFMMCTradingAccountKeyField Converter::QryCFMMCTradingAccountKeyToCpp(QryCFMMCTradingAccountKey x) {
    CThostFtdcQryCFMMCTradingAccountKeyField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    return y;
}

QryCFMMCTradingAccountKey Converter::CThostFtdcQryCFMMCTradingAccountKeyFieldToRust(CThostFtdcQryCFMMCTradingAccountKeyField* x) {
    if (x == nullptr)
        return QryCFMMCTradingAccountKey{.is_null = true};
    QryCFMMCTradingAccountKey y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    return y;
}

CThostFtdcBrokerUserOTPParamField Converter::BrokerUserOTPParamToCpp(BrokerUserOTPParam x) {
    CThostFtdcBrokerUserOTPParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.OTPVendorsID, x.otp_vendors_id.c_str());
    memcpy(y.SerialNumber, x.serial_number.data(), x.serial_number.size() * sizeof(uint8_t));
    memcpy(y.AuthKey, x.auth_key.data(), x.auth_key.size() * sizeof(uint8_t));
    y.LastDrift = x.last_drift;
    y.LastSuccess = x.last_success;
    y.OTPType = x.otp_type;
    return y;
}

BrokerUserOTPParam Converter::CThostFtdcBrokerUserOTPParamFieldToRust(CThostFtdcBrokerUserOTPParamField* x) {
    if (x == nullptr)
        return BrokerUserOTPParam{.is_null = true};
    BrokerUserOTPParam y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.otp_vendors_id = Converter::Gb2312ToRustString(x->OTPVendorsID);
    for (int i = 0; i < 17; i++)
        y.serial_number.push_back(x->SerialNumber[i]);
    for (int i = 0; i < 41; i++)
        y.auth_key.push_back(x->AuthKey[i]);
    y.last_drift = x->LastDrift;
    y.last_success = x->LastSuccess;
    y.otp_type = x->OTPType;
    return y;
}

CThostFtdcManualSyncBrokerUserOTPField Converter::ManualSyncBrokerUserOTPToCpp(ManualSyncBrokerUserOTP x) {
    CThostFtdcManualSyncBrokerUserOTPField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.OTPType = x.otp_type;
    memcpy(y.FirstOTP, x.first_otp.data(), x.first_otp.size() * sizeof(uint8_t));
    memcpy(y.SecondOTP, x.second_otp.data(), x.second_otp.size() * sizeof(uint8_t));
    return y;
}

ManualSyncBrokerUserOTP Converter::CThostFtdcManualSyncBrokerUserOTPFieldToRust(CThostFtdcManualSyncBrokerUserOTPField* x) {
    if (x == nullptr)
        return ManualSyncBrokerUserOTP{.is_null = true};
    ManualSyncBrokerUserOTP y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.otp_type = x->OTPType;
    for (int i = 0; i < 41; i++)
        y.first_otp.push_back(x->FirstOTP[i]);
    for (int i = 0; i < 41; i++)
        y.second_otp.push_back(x->SecondOTP[i]);
    return y;
}

CThostFtdcCommRateModelField Converter::CommRateModelToCpp(CommRateModel x) {
    CThostFtdcCommRateModelField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.CommModelID, x.comm_model_id.c_str());
    strcpy(y.CommModelName, x.comm_model_name.c_str());
    return y;
}

CommRateModel Converter::CThostFtdcCommRateModelFieldToRust(CThostFtdcCommRateModelField* x) {
    if (x == nullptr)
        return CommRateModel{.is_null = true};
    CommRateModel y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.comm_model_id = Converter::Gb2312ToRustString(x->CommModelID);
    y.comm_model_name = Converter::Gb2312ToRustString(x->CommModelName);
    return y;
}

CThostFtdcQryCommRateModelField Converter::QryCommRateModelToCpp(QryCommRateModel x) {
    CThostFtdcQryCommRateModelField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.CommModelID, x.comm_model_id.c_str());
    return y;
}

QryCommRateModel Converter::CThostFtdcQryCommRateModelFieldToRust(CThostFtdcQryCommRateModelField* x) {
    if (x == nullptr)
        return QryCommRateModel{.is_null = true};
    QryCommRateModel y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.comm_model_id = Converter::Gb2312ToRustString(x->CommModelID);
    return y;
}

CThostFtdcMarginModelField Converter::MarginModelToCpp(MarginModel x) {
    CThostFtdcMarginModelField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.MarginModelID, x.margin_model_id.c_str());
    strcpy(y.MarginModelName, x.margin_model_name.c_str());
    return y;
}

MarginModel Converter::CThostFtdcMarginModelFieldToRust(CThostFtdcMarginModelField* x) {
    if (x == nullptr)
        return MarginModel{.is_null = true};
    MarginModel y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.margin_model_id = Converter::Gb2312ToRustString(x->MarginModelID);
    y.margin_model_name = Converter::Gb2312ToRustString(x->MarginModelName);
    return y;
}

CThostFtdcQryMarginModelField Converter::QryMarginModelToCpp(QryMarginModel x) {
    CThostFtdcQryMarginModelField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.MarginModelID, x.margin_model_id.c_str());
    return y;
}

QryMarginModel Converter::CThostFtdcQryMarginModelFieldToRust(CThostFtdcQryMarginModelField* x) {
    if (x == nullptr)
        return QryMarginModel{.is_null = true};
    QryMarginModel y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.margin_model_id = Converter::Gb2312ToRustString(x->MarginModelID);
    return y;
}

CThostFtdcEWarrantOffsetField Converter::EWarrantOffsetToCpp(EWarrantOffset x) {
    CThostFtdcEWarrantOffsetField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.Direction = x.direction;
    y.HedgeFlag = x.hedge_flag;
    y.Volume = x.volume;
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

EWarrantOffset Converter::CThostFtdcEWarrantOffsetFieldToRust(CThostFtdcEWarrantOffsetField* x) {
    if (x == nullptr)
        return EWarrantOffset{.is_null = true};
    EWarrantOffset y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.direction = x->Direction;
    y.hedge_flag = x->HedgeFlag;
    y.volume = x->Volume;
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryEWarrantOffsetField Converter::QryEWarrantOffsetToCpp(QryEWarrantOffset x) {
    CThostFtdcQryEWarrantOffsetField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryEWarrantOffset Converter::CThostFtdcQryEWarrantOffsetFieldToRust(CThostFtdcQryEWarrantOffsetField* x) {
    if (x == nullptr)
        return QryEWarrantOffset{.is_null = true};
    QryEWarrantOffset y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryInvestorProductGroupMarginField Converter::QryInvestorProductGroupMarginToCpp(QryInvestorProductGroupMargin x) {
    CThostFtdcQryInvestorProductGroupMarginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.ProductGroupID, x.product_group_id.c_str());
    return y;
}

QryInvestorProductGroupMargin Converter::CThostFtdcQryInvestorProductGroupMarginFieldToRust(CThostFtdcQryInvestorProductGroupMarginField* x) {
    if (x == nullptr)
        return QryInvestorProductGroupMargin{.is_null = true};
    QryInvestorProductGroupMargin y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.hedge_flag = x->HedgeFlag;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.product_group_id = Converter::Gb2312ToRustString(x->ProductGroupID);
    return y;
}

CThostFtdcInvestorProductGroupMarginField Converter::InvestorProductGroupMarginToCpp(InvestorProductGroupMargin x) {
    CThostFtdcInvestorProductGroupMarginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    y.FrozenMargin = x.frozen_margin;
    y.LongFrozenMargin = x.long_frozen_margin;
    y.ShortFrozenMargin = x.short_frozen_margin;
    y.UseMargin = x.use_margin;
    y.LongUseMargin = x.long_use_margin;
    y.ShortUseMargin = x.short_use_margin;
    y.ExchMargin = x.exch_margin;
    y.LongExchMargin = x.long_exch_margin;
    y.ShortExchMargin = x.short_exch_margin;
    y.CloseProfit = x.close_profit;
    y.FrozenCommission = x.frozen_commission;
    y.Commission = x.commission;
    y.FrozenCash = x.frozen_cash;
    y.CashIn = x.cash_in;
    y.PositionProfit = x.position_profit;
    y.OffsetAmount = x.offset_amount;
    y.LongOffsetAmount = x.long_offset_amount;
    y.ShortOffsetAmount = x.short_offset_amount;
    y.ExchOffsetAmount = x.exch_offset_amount;
    y.LongExchOffsetAmount = x.long_exch_offset_amount;
    y.ShortExchOffsetAmount = x.short_exch_offset_amount;
    y.HedgeFlag = x.hedge_flag;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    strcpy(y.ProductGroupID, x.product_group_id.c_str());
    return y;
}

InvestorProductGroupMargin Converter::CThostFtdcInvestorProductGroupMarginFieldToRust(CThostFtdcInvestorProductGroupMarginField* x) {
    if (x == nullptr)
        return InvestorProductGroupMargin{.is_null = true};
    InvestorProductGroupMargin y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.frozen_margin = x->FrozenMargin;
    y.long_frozen_margin = x->LongFrozenMargin;
    y.short_frozen_margin = x->ShortFrozenMargin;
    y.use_margin = x->UseMargin;
    y.long_use_margin = x->LongUseMargin;
    y.short_use_margin = x->ShortUseMargin;
    y.exch_margin = x->ExchMargin;
    y.long_exch_margin = x->LongExchMargin;
    y.short_exch_margin = x->ShortExchMargin;
    y.close_profit = x->CloseProfit;
    y.frozen_commission = x->FrozenCommission;
    y.commission = x->Commission;
    y.frozen_cash = x->FrozenCash;
    y.cash_in = x->CashIn;
    y.position_profit = x->PositionProfit;
    y.offset_amount = x->OffsetAmount;
    y.long_offset_amount = x->LongOffsetAmount;
    y.short_offset_amount = x->ShortOffsetAmount;
    y.exch_offset_amount = x->ExchOffsetAmount;
    y.long_exch_offset_amount = x->LongExchOffsetAmount;
    y.short_exch_offset_amount = x->ShortExchOffsetAmount;
    y.hedge_flag = x->HedgeFlag;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.product_group_id = Converter::Gb2312ToRustString(x->ProductGroupID);
    return y;
}

CThostFtdcQueryCFMMCTradingAccountTokenField Converter::QueryCFMMCTradingAccountTokenToCpp(QueryCFMMCTradingAccountToken x) {
    CThostFtdcQueryCFMMCTradingAccountTokenField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    return y;
}

QueryCFMMCTradingAccountToken Converter::CThostFtdcQueryCFMMCTradingAccountTokenFieldToRust(CThostFtdcQueryCFMMCTradingAccountTokenField* x) {
    if (x == nullptr)
        return QueryCFMMCTradingAccountToken{.is_null = true};
    QueryCFMMCTradingAccountToken y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    return y;
}

CThostFtdcCFMMCTradingAccountTokenField Converter::CFMMCTradingAccountTokenToCpp(CFMMCTradingAccountToken x) {
    CThostFtdcCFMMCTradingAccountTokenField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    y.KeyID = x.key_id;
    memcpy(y.Token, x.token.data(), x.token.size() * sizeof(uint8_t));
    return y;
}

CFMMCTradingAccountToken Converter::CThostFtdcCFMMCTradingAccountTokenFieldToRust(CThostFtdcCFMMCTradingAccountTokenField* x) {
    if (x == nullptr)
        return CFMMCTradingAccountToken{.is_null = true};
    CFMMCTradingAccountToken y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.key_id = x->KeyID;
    for (int i = 0; i < 21; i++)
        y.token.push_back(x->Token[i]);
    return y;
}

CThostFtdcQryProductGroupField Converter::QryProductGroupToCpp(QryProductGroup x) {
    CThostFtdcQryProductGroupField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    return y;
}

QryProductGroup Converter::CThostFtdcQryProductGroupFieldToRust(CThostFtdcQryProductGroupField* x) {
    if (x == nullptr)
        return QryProductGroup{.is_null = true};
    QryProductGroup y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    return y;
}

CThostFtdcProductGroupField Converter::ProductGroupToCpp(ProductGroup x) {
    CThostFtdcProductGroupField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    strcpy(y.ProductGroupID, x.product_group_id.c_str());
    return y;
}

ProductGroup Converter::CThostFtdcProductGroupFieldToRust(CThostFtdcProductGroupField* x) {
    if (x == nullptr)
        return ProductGroup{.is_null = true};
    ProductGroup y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.product_group_id = Converter::Gb2312ToRustString(x->ProductGroupID);
    return y;
}

CThostFtdcBulletinField Converter::BulletinToCpp(Bulletin x) {
    CThostFtdcBulletinField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.BulletinID = x.bulletin_id;
    y.SequenceNo = x.sequence_no;
    memcpy(y.NewsType, x.news_type.data(), x.news_type.size() * sizeof(uint8_t));
    y.NewsUrgency = x.news_urgency;
    strcpy(y.SendTime, x.send_time.c_str());
    memcpy(y.Abstract, x.abstracts.data(), x.abstracts.size() * sizeof(uint8_t));
    memcpy(y.ComeFrom, x.come_from.data(), x.come_from.size() * sizeof(uint8_t));
    memcpy(y.Content, x.content.data(), x.content.size() * sizeof(uint8_t));
    memcpy(y.URLLink, x.url_link.data(), x.url_link.size() * sizeof(uint8_t));
    strcpy(y.MarketID, x.market_id.c_str());
    return y;
}

Bulletin Converter::CThostFtdcBulletinFieldToRust(CThostFtdcBulletinField* x) {
    if (x == nullptr)
        return Bulletin{.is_null = true};
    Bulletin y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.bulletin_id = x->BulletinID;
    y.sequence_no = x->SequenceNo;
    for (int i = 0; i < 3; i++)
        y.news_type.push_back(x->NewsType[i]);
    y.news_urgency = x->NewsUrgency;
    y.send_time = Converter::Gb2312ToRustString(x->SendTime);
    for (int i = 0; i < 81; i++)
        y.abstracts.push_back(x->Abstract[i]);
    for (int i = 0; i < 21; i++)
        y.come_from.push_back(x->ComeFrom[i]);
    for (int i = 0; i < 501; i++)
        y.content.push_back(x->Content[i]);
    for (int i = 0; i < 201; i++)
        y.url_link.push_back(x->URLLink[i]);
    y.market_id = Converter::Gb2312ToRustString(x->MarketID);
    return y;
}

CThostFtdcQryBulletinField Converter::QryBulletinToCpp(QryBulletin x) {
    CThostFtdcQryBulletinField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.BulletinID = x.bulletin_id;
    y.SequenceNo = x.sequence_no;
    memcpy(y.NewsType, x.news_type.data(), x.news_type.size() * sizeof(uint8_t));
    y.NewsUrgency = x.news_urgency;
    return y;
}

QryBulletin Converter::CThostFtdcQryBulletinFieldToRust(CThostFtdcQryBulletinField* x) {
    if (x == nullptr)
        return QryBulletin{.is_null = true};
    QryBulletin y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.bulletin_id = x->BulletinID;
    y.sequence_no = x->SequenceNo;
    for (int i = 0; i < 3; i++)
        y.news_type.push_back(x->NewsType[i]);
    y.news_urgency = x->NewsUrgency;
    return y;
}

CThostFtdcMulticastInstrumentField Converter::MulticastInstrumentToCpp(MulticastInstrument x) {
    CThostFtdcMulticastInstrumentField y;
    memset(&y, 0, sizeof(y));
    y.TopicID = x.topic_id;
    y.InstrumentNo = x.instrument_no;
    y.CodePrice = x.code_price;
    y.VolumeMultiple = x.volume_multiple;
    y.PriceTick = x.price_tick;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

MulticastInstrument Converter::CThostFtdcMulticastInstrumentFieldToRust(CThostFtdcMulticastInstrumentField* x) {
    if (x == nullptr)
        return MulticastInstrument{.is_null = true};
    MulticastInstrument y{};
    y.topic_id = x->TopicID;
    y.instrument_no = x->InstrumentNo;
    y.code_price = x->CodePrice;
    y.volume_multiple = x->VolumeMultiple;
    y.price_tick = x->PriceTick;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryMulticastInstrumentField Converter::QryMulticastInstrumentToCpp(QryMulticastInstrument x) {
    CThostFtdcQryMulticastInstrumentField y;
    memset(&y, 0, sizeof(y));
    y.TopicID = x.topic_id;
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryMulticastInstrument Converter::CThostFtdcQryMulticastInstrumentFieldToRust(CThostFtdcQryMulticastInstrumentField* x) {
    if (x == nullptr)
        return QryMulticastInstrument{.is_null = true};
    QryMulticastInstrument y{};
    y.topic_id = x->TopicID;
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcAppIDAuthAssignField Converter::AppIDAuthAssignToCpp(AppIDAuthAssign x) {
    CThostFtdcAppIDAuthAssignField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AppID, x.app_id.c_str());
    y.DRIdentityID = x.dr_identity_id;
    return y;
}

AppIDAuthAssign Converter::CThostFtdcAppIDAuthAssignFieldToRust(CThostFtdcAppIDAuthAssignField* x) {
    if (x == nullptr)
        return AppIDAuthAssign{.is_null = true};
    AppIDAuthAssign y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.app_id = Converter::Gb2312ToRustString(x->AppID);
    y.dr_identity_id = x->DRIdentityID;
    return y;
}

CThostFtdcReqOpenAccountField Converter::ReqOpenAccountToCpp(ReqOpenAccount x) {
    CThostFtdcReqOpenAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.Gender = x.gender;
    strcpy(y.CountryCode, x.country_code.c_str());
    y.CustType = x.cust_type;
    strcpy(y.Address, x.address.c_str());
    strcpy(y.ZipCode, x.zip_code.c_str());
    strcpy(y.Telephone, x.telephone.c_str());
    strcpy(y.MobilePhone, x.mobile_phone.c_str());
    strcpy(y.Fax, x.fax.c_str());
    strcpy(y.EMail, x.email.c_str());
    y.MoneyAccountStatus = x.money_account_status;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.InstallID = x.install_id;
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.CashExchangeCode = x.cash_exchange_code;
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.TID = x.tid;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

ReqOpenAccount Converter::CThostFtdcReqOpenAccountFieldToRust(CThostFtdcReqOpenAccountField* x) {
    if (x == nullptr)
        return ReqOpenAccount{.is_null = true};
    ReqOpenAccount y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.gender = x->Gender;
    y.country_code = Converter::Gb2312ToRustString(x->CountryCode);
    y.cust_type = x->CustType;
    y.address = Converter::Gb2312ToRustString(x->Address);
    y.zip_code = Converter::Gb2312ToRustString(x->ZipCode);
    y.telephone = Converter::Gb2312ToRustString(x->Telephone);
    y.mobile_phone = Converter::Gb2312ToRustString(x->MobilePhone);
    y.fax = Converter::Gb2312ToRustString(x->Fax);
    y.email = Converter::Gb2312ToRustString(x->EMail);
    y.money_account_status = x->MoneyAccountStatus;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.install_id = x->InstallID;
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.cash_exchange_code = x->CashExchangeCode;
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.tid = x->TID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcReqCancelAccountField Converter::ReqCancelAccountToCpp(ReqCancelAccount x) {
    CThostFtdcReqCancelAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.Gender = x.gender;
    strcpy(y.CountryCode, x.country_code.c_str());
    y.CustType = x.cust_type;
    strcpy(y.Address, x.address.c_str());
    strcpy(y.ZipCode, x.zip_code.c_str());
    strcpy(y.Telephone, x.telephone.c_str());
    strcpy(y.MobilePhone, x.mobile_phone.c_str());
    strcpy(y.Fax, x.fax.c_str());
    strcpy(y.EMail, x.email.c_str());
    y.MoneyAccountStatus = x.money_account_status;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.InstallID = x.install_id;
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.CashExchangeCode = x.cash_exchange_code;
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.TID = x.tid;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

ReqCancelAccount Converter::CThostFtdcReqCancelAccountFieldToRust(CThostFtdcReqCancelAccountField* x) {
    if (x == nullptr)
        return ReqCancelAccount{.is_null = true};
    ReqCancelAccount y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.gender = x->Gender;
    y.country_code = Converter::Gb2312ToRustString(x->CountryCode);
    y.cust_type = x->CustType;
    y.address = Converter::Gb2312ToRustString(x->Address);
    y.zip_code = Converter::Gb2312ToRustString(x->ZipCode);
    y.telephone = Converter::Gb2312ToRustString(x->Telephone);
    y.mobile_phone = Converter::Gb2312ToRustString(x->MobilePhone);
    y.fax = Converter::Gb2312ToRustString(x->Fax);
    y.email = Converter::Gb2312ToRustString(x->EMail);
    y.money_account_status = x->MoneyAccountStatus;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.install_id = x->InstallID;
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.cash_exchange_code = x->CashExchangeCode;
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.tid = x->TID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcReqChangeAccountField Converter::ReqChangeAccountToCpp(ReqChangeAccount x) {
    CThostFtdcReqChangeAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.Gender = x.gender;
    strcpy(y.CountryCode, x.country_code.c_str());
    y.CustType = x.cust_type;
    strcpy(y.Address, x.address.c_str());
    strcpy(y.ZipCode, x.zip_code.c_str());
    strcpy(y.Telephone, x.telephone.c_str());
    strcpy(y.MobilePhone, x.mobile_phone.c_str());
    strcpy(y.Fax, x.fax.c_str());
    strcpy(y.EMail, x.email.c_str());
    y.MoneyAccountStatus = x.money_account_status;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.NewBankAccount, x.new_bank_account.c_str());
    strcpy(y.NewBankPassWord, x.new_bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.BankAccType = x.bank_acc_type;
    y.InstallID = x.install_id;
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    y.TID = x.tid;
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

ReqChangeAccount Converter::CThostFtdcReqChangeAccountFieldToRust(CThostFtdcReqChangeAccountField* x) {
    if (x == nullptr)
        return ReqChangeAccount{.is_null = true};
    ReqChangeAccount y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.gender = x->Gender;
    y.country_code = Converter::Gb2312ToRustString(x->CountryCode);
    y.cust_type = x->CustType;
    y.address = Converter::Gb2312ToRustString(x->Address);
    y.zip_code = Converter::Gb2312ToRustString(x->ZipCode);
    y.telephone = Converter::Gb2312ToRustString(x->Telephone);
    y.mobile_phone = Converter::Gb2312ToRustString(x->MobilePhone);
    y.fax = Converter::Gb2312ToRustString(x->Fax);
    y.email = Converter::Gb2312ToRustString(x->EMail);
    y.money_account_status = x->MoneyAccountStatus;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.new_bank_account = Converter::Gb2312ToRustString(x->NewBankAccount);
    y.new_bank_pass_word = Converter::Gb2312ToRustString(x->NewBankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.bank_acc_type = x->BankAccType;
    y.install_id = x->InstallID;
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.tid = x->TID;
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcReqTransferField Converter::ReqTransferToCpp(ReqTransfer x) {
    CThostFtdcReqTransferField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.InstallID = x.install_id;
    y.FutureSerial = x.future_serial;
    strcpy(y.UserID, x.user_id.c_str());
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.TradeAmount = x.trade_amount;
    y.FutureFetchAmount = x.future_fetch_amount;
    y.FeePayFlag = x.fee_pay_flag;
    y.CustFee = x.cust_fee;
    y.BrokerFee = x.broker_fee;
    memcpy(y.Message, x.message.data(), x.message.size() * sizeof(uint8_t));
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.TransferStatus = x.transfer_status;
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

ReqTransfer Converter::CThostFtdcReqTransferFieldToRust(CThostFtdcReqTransferField* x) {
    if (x == nullptr)
        return ReqTransfer{.is_null = true};
    ReqTransfer y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.install_id = x->InstallID;
    y.future_serial = x->FutureSerial;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.trade_amount = x->TradeAmount;
    y.future_fetch_amount = x->FutureFetchAmount;
    y.fee_pay_flag = x->FeePayFlag;
    y.cust_fee = x->CustFee;
    y.broker_fee = x->BrokerFee;
    for (int i = 0; i < 129; i++)
        y.message.push_back(x->Message[i]);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.transfer_status = x->TransferStatus;
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcRspTransferField Converter::RspTransferToCpp(RspTransfer x) {
    CThostFtdcRspTransferField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.InstallID = x.install_id;
    y.FutureSerial = x.future_serial;
    strcpy(y.UserID, x.user_id.c_str());
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.TradeAmount = x.trade_amount;
    y.FutureFetchAmount = x.future_fetch_amount;
    y.FeePayFlag = x.fee_pay_flag;
    y.CustFee = x.cust_fee;
    y.BrokerFee = x.broker_fee;
    memcpy(y.Message, x.message.data(), x.message.size() * sizeof(uint8_t));
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.TransferStatus = x.transfer_status;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

RspTransfer Converter::CThostFtdcRspTransferFieldToRust(CThostFtdcRspTransferField* x) {
    if (x == nullptr)
        return RspTransfer{.is_null = true};
    RspTransfer y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.install_id = x->InstallID;
    y.future_serial = x->FutureSerial;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.trade_amount = x->TradeAmount;
    y.future_fetch_amount = x->FutureFetchAmount;
    y.fee_pay_flag = x->FeePayFlag;
    y.cust_fee = x->CustFee;
    y.broker_fee = x->BrokerFee;
    for (int i = 0; i < 129; i++)
        y.message.push_back(x->Message[i]);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.transfer_status = x->TransferStatus;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcReqRepealField Converter::ReqRepealToCpp(ReqRepeal x) {
    CThostFtdcReqRepealField y;
    memset(&y, 0, sizeof(y));
    y.RepealTimeInterval = x.repeal_time_interval;
    y.RepealedTimes = x.repealed_times;
    y.BankRepealFlag = x.bank_repeal_flag;
    y.BrokerRepealFlag = x.broker_repeal_flag;
    y.PlateRepealSerial = x.plate_repeal_serial;
    strcpy(y.BankRepealSerial, x.bank_repeal_serial.c_str());
    y.FutureRepealSerial = x.future_repeal_serial;
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.InstallID = x.install_id;
    y.FutureSerial = x.future_serial;
    strcpy(y.UserID, x.user_id.c_str());
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.TradeAmount = x.trade_amount;
    y.FutureFetchAmount = x.future_fetch_amount;
    y.FeePayFlag = x.fee_pay_flag;
    y.CustFee = x.cust_fee;
    y.BrokerFee = x.broker_fee;
    memcpy(y.Message, x.message.data(), x.message.size() * sizeof(uint8_t));
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.TransferStatus = x.transfer_status;
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

ReqRepeal Converter::CThostFtdcReqRepealFieldToRust(CThostFtdcReqRepealField* x) {
    if (x == nullptr)
        return ReqRepeal{.is_null = true};
    ReqRepeal y{};
    y.repeal_time_interval = x->RepealTimeInterval;
    y.repealed_times = x->RepealedTimes;
    y.bank_repeal_flag = x->BankRepealFlag;
    y.broker_repeal_flag = x->BrokerRepealFlag;
    y.plate_repeal_serial = x->PlateRepealSerial;
    y.bank_repeal_serial = Converter::Gb2312ToRustString(x->BankRepealSerial);
    y.future_repeal_serial = x->FutureRepealSerial;
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.install_id = x->InstallID;
    y.future_serial = x->FutureSerial;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.trade_amount = x->TradeAmount;
    y.future_fetch_amount = x->FutureFetchAmount;
    y.fee_pay_flag = x->FeePayFlag;
    y.cust_fee = x->CustFee;
    y.broker_fee = x->BrokerFee;
    for (int i = 0; i < 129; i++)
        y.message.push_back(x->Message[i]);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.transfer_status = x->TransferStatus;
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcRspRepealField Converter::RspRepealToCpp(RspRepeal x) {
    CThostFtdcRspRepealField y;
    memset(&y, 0, sizeof(y));
    y.RepealTimeInterval = x.repeal_time_interval;
    y.RepealedTimes = x.repealed_times;
    y.BankRepealFlag = x.bank_repeal_flag;
    y.BrokerRepealFlag = x.broker_repeal_flag;
    y.PlateRepealSerial = x.plate_repeal_serial;
    strcpy(y.BankRepealSerial, x.bank_repeal_serial.c_str());
    y.FutureRepealSerial = x.future_repeal_serial;
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.InstallID = x.install_id;
    y.FutureSerial = x.future_serial;
    strcpy(y.UserID, x.user_id.c_str());
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.TradeAmount = x.trade_amount;
    y.FutureFetchAmount = x.future_fetch_amount;
    y.FeePayFlag = x.fee_pay_flag;
    y.CustFee = x.cust_fee;
    y.BrokerFee = x.broker_fee;
    memcpy(y.Message, x.message.data(), x.message.size() * sizeof(uint8_t));
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.TransferStatus = x.transfer_status;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

RspRepeal Converter::CThostFtdcRspRepealFieldToRust(CThostFtdcRspRepealField* x) {
    if (x == nullptr)
        return RspRepeal{.is_null = true};
    RspRepeal y{};
    y.repeal_time_interval = x->RepealTimeInterval;
    y.repealed_times = x->RepealedTimes;
    y.bank_repeal_flag = x->BankRepealFlag;
    y.broker_repeal_flag = x->BrokerRepealFlag;
    y.plate_repeal_serial = x->PlateRepealSerial;
    y.bank_repeal_serial = Converter::Gb2312ToRustString(x->BankRepealSerial);
    y.future_repeal_serial = x->FutureRepealSerial;
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.install_id = x->InstallID;
    y.future_serial = x->FutureSerial;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.trade_amount = x->TradeAmount;
    y.future_fetch_amount = x->FutureFetchAmount;
    y.fee_pay_flag = x->FeePayFlag;
    y.cust_fee = x->CustFee;
    y.broker_fee = x->BrokerFee;
    for (int i = 0; i < 129; i++)
        y.message.push_back(x->Message[i]);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.transfer_status = x->TransferStatus;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcReqQueryAccountField Converter::ReqQueryAccountToCpp(ReqQueryAccount x) {
    CThostFtdcReqQueryAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.FutureSerial = x.future_serial;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

ReqQueryAccount Converter::CThostFtdcReqQueryAccountFieldToRust(CThostFtdcReqQueryAccountField* x) {
    if (x == nullptr)
        return ReqQueryAccount{.is_null = true};
    ReqQueryAccount y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.future_serial = x->FutureSerial;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcRspQueryAccountField Converter::RspQueryAccountToCpp(RspQueryAccount x) {
    CThostFtdcRspQueryAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.FutureSerial = x.future_serial;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.BankUseAmount = x.bank_use_amount;
    y.BankFetchAmount = x.bank_fetch_amount;
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

RspQueryAccount Converter::CThostFtdcRspQueryAccountFieldToRust(CThostFtdcRspQueryAccountField* x) {
    if (x == nullptr)
        return RspQueryAccount{.is_null = true};
    RspQueryAccount y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.future_serial = x->FutureSerial;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.bank_use_amount = x->BankUseAmount;
    y.bank_fetch_amount = x->BankFetchAmount;
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcFutureSignIOField Converter::FutureSignIOToCpp(FutureSignIO x) {
    CThostFtdcFutureSignIOField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.DeviceID, x.device_id.c_str());
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    return y;
}

FutureSignIO Converter::CThostFtdcFutureSignIOFieldToRust(CThostFtdcFutureSignIOField* x) {
    if (x == nullptr)
        return FutureSignIO{.is_null = true};
    FutureSignIO y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    return y;
}

CThostFtdcRspFutureSignInField Converter::RspFutureSignInToCpp(RspFutureSignIn x) {
    CThostFtdcRspFutureSignInField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.DeviceID, x.device_id.c_str());
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    memcpy(y.PinKey, x.pin_key.data(), x.pin_key.size() * sizeof(uint8_t));
    memcpy(y.MacKey, x.mac_key.data(), x.mac_key.size() * sizeof(uint8_t));
    return y;
}

RspFutureSignIn Converter::CThostFtdcRspFutureSignInFieldToRust(CThostFtdcRspFutureSignInField* x) {
    if (x == nullptr)
        return RspFutureSignIn{.is_null = true};
    RspFutureSignIn y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    for (int i = 0; i < 129; i++)
        y.pin_key.push_back(x->PinKey[i]);
    for (int i = 0; i < 129; i++)
        y.mac_key.push_back(x->MacKey[i]);
    return y;
}

CThostFtdcReqFutureSignOutField Converter::ReqFutureSignOutToCpp(ReqFutureSignOut x) {
    CThostFtdcReqFutureSignOutField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.DeviceID, x.device_id.c_str());
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    return y;
}

ReqFutureSignOut Converter::CThostFtdcReqFutureSignOutFieldToRust(CThostFtdcReqFutureSignOutField* x) {
    if (x == nullptr)
        return ReqFutureSignOut{.is_null = true};
    ReqFutureSignOut y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    return y;
}

CThostFtdcRspFutureSignOutField Converter::RspFutureSignOutToCpp(RspFutureSignOut x) {
    CThostFtdcRspFutureSignOutField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.DeviceID, x.device_id.c_str());
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    return y;
}

RspFutureSignOut Converter::CThostFtdcRspFutureSignOutFieldToRust(CThostFtdcRspFutureSignOutField* x) {
    if (x == nullptr)
        return RspFutureSignOut{.is_null = true};
    RspFutureSignOut y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    return y;
}

CThostFtdcReqQueryTradeResultBySerialField Converter::ReqQueryTradeResultBySerialToCpp(ReqQueryTradeResultBySerial x) {
    CThostFtdcReqQueryTradeResultBySerialField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    y.Reference = x.reference;
    y.RefrenceIssureType = x.reference_issuer_type;
    memcpy(y.RefrenceIssure, x.reference_issuer.data(), x.reference_issuer.size() * sizeof(uint8_t));
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.TradeAmount = x.trade_amount;
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

ReqQueryTradeResultBySerial Converter::CThostFtdcReqQueryTradeResultBySerialFieldToRust(CThostFtdcReqQueryTradeResultBySerialField* x) {
    if (x == nullptr)
        return ReqQueryTradeResultBySerial{.is_null = true};
    ReqQueryTradeResultBySerial y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.reference = x->Reference;
    y.reference_issuer_type = x->RefrenceIssureType;
    for (int i = 0; i < 36; i++)
        y.reference_issuer.push_back(x->RefrenceIssure[i]);
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.trade_amount = x->TradeAmount;
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcRspQueryTradeResultBySerialField Converter::RspQueryTradeResultBySerialToCpp(RspQueryTradeResultBySerial x) {
    CThostFtdcRspQueryTradeResultBySerialField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    y.Reference = x.reference;
    y.RefrenceIssureType = x.reference_issuer_type;
    memcpy(y.RefrenceIssure, x.reference_issuer.data(), x.reference_issuer.size() * sizeof(uint8_t));
    strcpy(y.OriginReturnCode, x.origin_return_code.c_str());
    strcpy(y.OriginDescrInfoForReturnCode, x.origin_descr_info_for_return_code.c_str());
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.TradeAmount = x.trade_amount;
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    return y;
}

RspQueryTradeResultBySerial Converter::CThostFtdcRspQueryTradeResultBySerialFieldToRust(CThostFtdcRspQueryTradeResultBySerialField* x) {
    if (x == nullptr)
        return RspQueryTradeResultBySerial{.is_null = true};
    RspQueryTradeResultBySerial y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.reference = x->Reference;
    y.reference_issuer_type = x->RefrenceIssureType;
    for (int i = 0; i < 36; i++)
        y.reference_issuer.push_back(x->RefrenceIssure[i]);
    y.origin_return_code = Converter::Gb2312ToRustString(x->OriginReturnCode);
    y.origin_descr_info_for_return_code = Converter::Gb2312ToRustString(x->OriginDescrInfoForReturnCode);
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.trade_amount = x->TradeAmount;
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    return y;
}

CThostFtdcReqDayEndFileReadyField Converter::ReqDayEndFileReadyToCpp(ReqDayEndFileReady x) {
    CThostFtdcReqDayEndFileReadyField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    y.FileBusinessCode = x.file_business_code;
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    return y;
}

ReqDayEndFileReady Converter::CThostFtdcReqDayEndFileReadyFieldToRust(CThostFtdcReqDayEndFileReadyField* x) {
    if (x == nullptr)
        return ReqDayEndFileReady{.is_null = true};
    ReqDayEndFileReady y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.file_business_code = x->FileBusinessCode;
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    return y;
}

CThostFtdcReturnResultField Converter::ReturnResultToCpp(ReturnResult x) {
    CThostFtdcReturnResultField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ReturnCode, x.return_code.c_str());
    strcpy(y.DescrInfoForReturnCode, x.descr_info_for_return_code.c_str());
    return y;
}

ReturnResult Converter::CThostFtdcReturnResultFieldToRust(CThostFtdcReturnResultField* x) {
    if (x == nullptr)
        return ReturnResult{.is_null = true};
    ReturnResult y{};
    y.return_code = Converter::Gb2312ToRustString(x->ReturnCode);
    y.descr_info_for_return_code = Converter::Gb2312ToRustString(x->DescrInfoForReturnCode);
    return y;
}

CThostFtdcVerifyFuturePasswordField Converter::VerifyFuturePasswordToCpp(VerifyFuturePassword x) {
    CThostFtdcVerifyFuturePasswordField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    y.InstallID = x.install_id;
    y.TID = x.tid;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

VerifyFuturePassword Converter::CThostFtdcVerifyFuturePasswordFieldToRust(CThostFtdcVerifyFuturePasswordField* x) {
    if (x == nullptr)
        return VerifyFuturePassword{.is_null = true};
    VerifyFuturePassword y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.install_id = x->InstallID;
    y.tid = x->TID;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcVerifyCustInfoField Converter::VerifyCustInfoToCpp(VerifyCustInfo x) {
    CThostFtdcVerifyCustInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

VerifyCustInfo Converter::CThostFtdcVerifyCustInfoFieldToRust(CThostFtdcVerifyCustInfoField* x) {
    if (x == nullptr)
        return VerifyCustInfo{.is_null = true};
    VerifyCustInfo y{};
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcVerifyFuturePasswordAndCustInfoField Converter::VerifyFuturePasswordAndCustInfoToCpp(VerifyFuturePasswordAndCustInfo x) {
    CThostFtdcVerifyFuturePasswordAndCustInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

VerifyFuturePasswordAndCustInfo Converter::CThostFtdcVerifyFuturePasswordAndCustInfoFieldToRust(CThostFtdcVerifyFuturePasswordAndCustInfoField* x) {
    if (x == nullptr)
        return VerifyFuturePasswordAndCustInfo{.is_null = true};
    VerifyFuturePasswordAndCustInfo y{};
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcDepositResultInformField Converter::DepositResultInformToCpp(DepositResultInform x) {
    CThostFtdcDepositResultInformField y;
    memset(&y, 0, sizeof(y));
    memcpy(y.DepositSeqNo, x.deposit_seq_no.data(), x.deposit_seq_no.size() * sizeof(uint8_t));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.Deposit = x.deposit;
    y.RequestID = x.request_id;
    strcpy(y.ReturnCode, x.return_code.c_str());
    strcpy(y.DescrInfoForReturnCode, x.descr_info_for_return_code.c_str());
    return y;
}

DepositResultInform Converter::CThostFtdcDepositResultInformFieldToRust(CThostFtdcDepositResultInformField* x) {
    if (x == nullptr)
        return DepositResultInform{.is_null = true};
    DepositResultInform y{};
    for (int i = 0; i < 15; i++)
        y.deposit_seq_no.push_back(x->DepositSeqNo[i]);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.deposit = x->Deposit;
    y.request_id = x->RequestID;
    y.return_code = Converter::Gb2312ToRustString(x->ReturnCode);
    y.descr_info_for_return_code = Converter::Gb2312ToRustString(x->DescrInfoForReturnCode);
    return y;
}

CThostFtdcReqSyncKeyField Converter::ReqSyncKeyToCpp(ReqSyncKey x) {
    CThostFtdcReqSyncKeyField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    memcpy(y.Message, x.message.data(), x.message.size() * sizeof(uint8_t));
    strcpy(y.DeviceID, x.device_id.c_str());
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    return y;
}

ReqSyncKey Converter::CThostFtdcReqSyncKeyFieldToRust(CThostFtdcReqSyncKeyField* x) {
    if (x == nullptr)
        return ReqSyncKey{.is_null = true};
    ReqSyncKey y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    for (int i = 0; i < 129; i++)
        y.message.push_back(x->Message[i]);
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    return y;
}

CThostFtdcRspSyncKeyField Converter::RspSyncKeyToCpp(RspSyncKey x) {
    CThostFtdcRspSyncKeyField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    memcpy(y.Message, x.message.data(), x.message.size() * sizeof(uint8_t));
    strcpy(y.DeviceID, x.device_id.c_str());
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    return y;
}

RspSyncKey Converter::CThostFtdcRspSyncKeyFieldToRust(CThostFtdcRspSyncKeyField* x) {
    if (x == nullptr)
        return RspSyncKey{.is_null = true};
    RspSyncKey y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    for (int i = 0; i < 129; i++)
        y.message.push_back(x->Message[i]);
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    return y;
}

CThostFtdcNotifyQueryAccountField Converter::NotifyQueryAccountToCpp(NotifyQueryAccount x) {
    CThostFtdcNotifyQueryAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.FutureSerial = x.future_serial;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.BankUseAmount = x.bank_use_amount;
    y.BankFetchAmount = x.bank_fetch_amount;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

NotifyQueryAccount Converter::CThostFtdcNotifyQueryAccountFieldToRust(CThostFtdcNotifyQueryAccountField* x) {
    if (x == nullptr)
        return NotifyQueryAccount{.is_null = true};
    NotifyQueryAccount y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.future_serial = x->FutureSerial;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.bank_use_amount = x->BankUseAmount;
    y.bank_fetch_amount = x->BankFetchAmount;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcTransferSerialField Converter::TransferSerialToCpp(TransferSerial x) {
    CThostFtdcTransferSerialField y;
    memset(&y, 0, sizeof(y));
    y.PlateSerial = x.plate_serial;
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.TradeCode, x.trade_code.c_str());
    y.SessionID = x.session_id;
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    y.BankAccType = x.bank_acc_type;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    y.FutureAccType = x.future_acc_type;
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.FutureSerial = x.future_serial;
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.TradeAmount = x.trade_amount;
    y.CustFee = x.cust_fee;
    y.BrokerFee = x.broker_fee;
    y.AvailabilityFlag = x.availability_flag;
    strcpy(y.OperatorCode, x.operator_code.c_str());
    strcpy(y.BankNewAccount, x.bank_new_account.c_str());
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    return y;
}

TransferSerial Converter::CThostFtdcTransferSerialFieldToRust(CThostFtdcTransferSerialField* x) {
    if (x == nullptr)
        return TransferSerial{.is_null = true};
    TransferSerial y{};
    y.plate_serial = x->PlateSerial;
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.session_id = x->SessionID;
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.bank_acc_type = x->BankAccType;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.future_acc_type = x->FutureAccType;
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.future_serial = x->FutureSerial;
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.trade_amount = x->TradeAmount;
    y.cust_fee = x->CustFee;
    y.broker_fee = x->BrokerFee;
    y.availability_flag = x->AvailabilityFlag;
    y.operator_code = Converter::Gb2312ToRustString(x->OperatorCode);
    y.bank_new_account = Converter::Gb2312ToRustString(x->BankNewAccount);
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    return y;
}

CThostFtdcQryTransferSerialField Converter::QryTransferSerialToCpp(QryTransferSerial x) {
    CThostFtdcQryTransferSerialField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

QryTransferSerial Converter::CThostFtdcQryTransferSerialFieldToRust(CThostFtdcQryTransferSerialField* x) {
    if (x == nullptr)
        return QryTransferSerial{.is_null = true};
    QryTransferSerial y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcNotifyFutureSignInField Converter::NotifyFutureSignInToCpp(NotifyFutureSignIn x) {
    CThostFtdcNotifyFutureSignInField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.DeviceID, x.device_id.c_str());
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    memcpy(y.PinKey, x.pin_key.data(), x.pin_key.size() * sizeof(uint8_t));
    memcpy(y.MacKey, x.mac_key.data(), x.mac_key.size() * sizeof(uint8_t));
    return y;
}

NotifyFutureSignIn Converter::CThostFtdcNotifyFutureSignInFieldToRust(CThostFtdcNotifyFutureSignInField* x) {
    if (x == nullptr)
        return NotifyFutureSignIn{.is_null = true};
    NotifyFutureSignIn y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    for (int i = 0; i < 129; i++)
        y.pin_key.push_back(x->PinKey[i]);
    for (int i = 0; i < 129; i++)
        y.mac_key.push_back(x->MacKey[i]);
    return y;
}

CThostFtdcNotifyFutureSignOutField Converter::NotifyFutureSignOutToCpp(NotifyFutureSignOut x) {
    CThostFtdcNotifyFutureSignOutField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.DeviceID, x.device_id.c_str());
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    return y;
}

NotifyFutureSignOut Converter::CThostFtdcNotifyFutureSignOutFieldToRust(CThostFtdcNotifyFutureSignOutField* x) {
    if (x == nullptr)
        return NotifyFutureSignOut{.is_null = true};
    NotifyFutureSignOut y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    return y;
}

CThostFtdcNotifySyncKeyField Converter::NotifySyncKeyToCpp(NotifySyncKey x) {
    CThostFtdcNotifySyncKeyField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    memcpy(y.Message, x.message.data(), x.message.size() * sizeof(uint8_t));
    strcpy(y.DeviceID, x.device_id.c_str());
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    return y;
}

NotifySyncKey Converter::CThostFtdcNotifySyncKeyFieldToRust(CThostFtdcNotifySyncKeyField* x) {
    if (x == nullptr)
        return NotifySyncKey{.is_null = true};
    NotifySyncKey y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    for (int i = 0; i < 129; i++)
        y.message.push_back(x->Message[i]);
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    return y;
}

CThostFtdcQryAccountregisterField Converter::QryAccountRegisterToCpp(QryAccountRegister x) {
    CThostFtdcQryAccountregisterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

QryAccountRegister Converter::CThostFtdcQryAccountregisterFieldToRust(CThostFtdcQryAccountregisterField* x) {
    if (x == nullptr)
        return QryAccountRegister{.is_null = true};
    QryAccountRegister y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcAccountregisterField Converter::AccountRegisterToCpp(AccountRegister x) {
    CThostFtdcAccountregisterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeDay, x.trade_day.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    strcpy(y.CustomerName, x.customer_name.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.OpenOrDestroy = x.open_or_destroy;
    strcpy(y.RegDate, x.reg_date.c_str());
    strcpy(y.OutDate, x.out_date.c_str());
    y.TID = x.tid;
    y.CustType = x.cust_type;
    y.BankAccType = x.bank_acc_type;
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

AccountRegister Converter::CThostFtdcAccountregisterFieldToRust(CThostFtdcAccountregisterField* x) {
    if (x == nullptr)
        return AccountRegister{.is_null = true};
    AccountRegister y{};
    y.trade_day = Converter::Gb2312ToRustString(x->TradeDay);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.open_or_destroy = x->OpenOrDestroy;
    y.reg_date = Converter::Gb2312ToRustString(x->RegDate);
    y.out_date = Converter::Gb2312ToRustString(x->OutDate);
    y.tid = x->TID;
    y.cust_type = x->CustType;
    y.bank_acc_type = x->BankAccType;
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcOpenAccountField Converter::OpenAccountToCpp(OpenAccount x) {
    CThostFtdcOpenAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.Gender = x.gender;
    strcpy(y.CountryCode, x.country_code.c_str());
    y.CustType = x.cust_type;
    strcpy(y.Address, x.address.c_str());
    strcpy(y.ZipCode, x.zip_code.c_str());
    strcpy(y.Telephone, x.telephone.c_str());
    strcpy(y.MobilePhone, x.mobile_phone.c_str());
    strcpy(y.Fax, x.fax.c_str());
    strcpy(y.EMail, x.email.c_str());
    y.MoneyAccountStatus = x.money_account_status;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.InstallID = x.install_id;
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.CashExchangeCode = x.cash_exchange_code;
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.TID = x.tid;
    strcpy(y.UserID, x.user_id.c_str());
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

OpenAccount Converter::CThostFtdcOpenAccountFieldToRust(CThostFtdcOpenAccountField* x) {
    if (x == nullptr)
        return OpenAccount{.is_null = true};
    OpenAccount y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.gender = x->Gender;
    y.country_code = Converter::Gb2312ToRustString(x->CountryCode);
    y.cust_type = x->CustType;
    y.address = Converter::Gb2312ToRustString(x->Address);
    y.zip_code = Converter::Gb2312ToRustString(x->ZipCode);
    y.telephone = Converter::Gb2312ToRustString(x->Telephone);
    y.mobile_phone = Converter::Gb2312ToRustString(x->MobilePhone);
    y.fax = Converter::Gb2312ToRustString(x->Fax);
    y.email = Converter::Gb2312ToRustString(x->EMail);
    y.money_account_status = x->MoneyAccountStatus;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.install_id = x->InstallID;
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.cash_exchange_code = x->CashExchangeCode;
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.tid = x->TID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcCancelAccountField Converter::CancelAccountToCpp(CancelAccount x) {
    CThostFtdcCancelAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.Gender = x.gender;
    strcpy(y.CountryCode, x.country_code.c_str());
    y.CustType = x.cust_type;
    strcpy(y.Address, x.address.c_str());
    strcpy(y.ZipCode, x.zip_code.c_str());
    strcpy(y.Telephone, x.telephone.c_str());
    strcpy(y.MobilePhone, x.mobile_phone.c_str());
    strcpy(y.Fax, x.fax.c_str());
    strcpy(y.EMail, x.email.c_str());
    y.MoneyAccountStatus = x.money_account_status;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.InstallID = x.install_id;
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.CashExchangeCode = x.cash_exchange_code;
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.TID = x.tid;
    strcpy(y.UserID, x.user_id.c_str());
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

CancelAccount Converter::CThostFtdcCancelAccountFieldToRust(CThostFtdcCancelAccountField* x) {
    if (x == nullptr)
        return CancelAccount{.is_null = true};
    CancelAccount y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.gender = x->Gender;
    y.country_code = Converter::Gb2312ToRustString(x->CountryCode);
    y.cust_type = x->CustType;
    y.address = Converter::Gb2312ToRustString(x->Address);
    y.zip_code = Converter::Gb2312ToRustString(x->ZipCode);
    y.telephone = Converter::Gb2312ToRustString(x->Telephone);
    y.mobile_phone = Converter::Gb2312ToRustString(x->MobilePhone);
    y.fax = Converter::Gb2312ToRustString(x->Fax);
    y.email = Converter::Gb2312ToRustString(x->EMail);
    y.money_account_status = x->MoneyAccountStatus;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.install_id = x->InstallID;
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.cash_exchange_code = x->CashExchangeCode;
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.tid = x->TID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcChangeAccountField Converter::ChangeAccountToCpp(ChangeAccount x) {
    CThostFtdcChangeAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.Gender = x.gender;
    strcpy(y.CountryCode, x.country_code.c_str());
    y.CustType = x.cust_type;
    strcpy(y.Address, x.address.c_str());
    strcpy(y.ZipCode, x.zip_code.c_str());
    strcpy(y.Telephone, x.telephone.c_str());
    strcpy(y.MobilePhone, x.mobile_phone.c_str());
    strcpy(y.Fax, x.fax.c_str());
    strcpy(y.EMail, x.email.c_str());
    y.MoneyAccountStatus = x.money_account_status;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.NewBankAccount, x.new_bank_account.c_str());
    strcpy(y.NewBankPassWord, x.new_bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.BankAccType = x.bank_acc_type;
    y.InstallID = x.install_id;
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    y.TID = x.tid;
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    return y;
}

ChangeAccount Converter::CThostFtdcChangeAccountFieldToRust(CThostFtdcChangeAccountField* x) {
    if (x == nullptr)
        return ChangeAccount{.is_null = true};
    ChangeAccount y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.gender = x->Gender;
    y.country_code = Converter::Gb2312ToRustString(x->CountryCode);
    y.cust_type = x->CustType;
    y.address = Converter::Gb2312ToRustString(x->Address);
    y.zip_code = Converter::Gb2312ToRustString(x->ZipCode);
    y.telephone = Converter::Gb2312ToRustString(x->Telephone);
    y.mobile_phone = Converter::Gb2312ToRustString(x->MobilePhone);
    y.fax = Converter::Gb2312ToRustString(x->Fax);
    y.email = Converter::Gb2312ToRustString(x->EMail);
    y.money_account_status = x->MoneyAccountStatus;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.new_bank_account = Converter::Gb2312ToRustString(x->NewBankAccount);
    y.new_bank_pass_word = Converter::Gb2312ToRustString(x->NewBankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.bank_acc_type = x->BankAccType;
    y.install_id = x->InstallID;
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.tid = x->TID;
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    return y;
}

CThostFtdcSecAgentACIDMapField Converter::SecAgentACIDMapToCpp(SecAgentACIDMap x) {
    CThostFtdcSecAgentACIDMapField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    strcpy(y.BrokerSecAgentID, x.broker_sec_agent_id.c_str());
    return y;
}

SecAgentACIDMap Converter::CThostFtdcSecAgentACIDMapFieldToRust(CThostFtdcSecAgentACIDMapField* x) {
    if (x == nullptr)
        return SecAgentACIDMap{.is_null = true};
    SecAgentACIDMap y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.broker_sec_agent_id = Converter::Gb2312ToRustString(x->BrokerSecAgentID);
    return y;
}

CThostFtdcQrySecAgentACIDMapField Converter::QrySecAgentACIDMapToCpp(QrySecAgentACIDMap x) {
    CThostFtdcQrySecAgentACIDMapField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

QrySecAgentACIDMap Converter::CThostFtdcQrySecAgentACIDMapFieldToRust(CThostFtdcQrySecAgentACIDMapField* x) {
    if (x == nullptr)
        return QrySecAgentACIDMap{.is_null = true};
    QrySecAgentACIDMap y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcUserRightsAssignField Converter::UserRightsAssignToCpp(UserRightsAssign x) {
    CThostFtdcUserRightsAssignField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.DRIdentityID = x.dr_identity_id;
    return y;
}

UserRightsAssign Converter::CThostFtdcUserRightsAssignFieldToRust(CThostFtdcUserRightsAssignField* x) {
    if (x == nullptr)
        return UserRightsAssign{.is_null = true};
    UserRightsAssign y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.dr_identity_id = x->DRIdentityID;
    return y;
}

CThostFtdcBrokerUserRightAssignField Converter::BrokerUserRightAssignToCpp(BrokerUserRightAssign x) {
    CThostFtdcBrokerUserRightAssignField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    y.DRIdentityID = x.dr_identity_id;
    y.Tradeable = x.tradeable;
    return y;
}

BrokerUserRightAssign Converter::CThostFtdcBrokerUserRightAssignFieldToRust(CThostFtdcBrokerUserRightAssignField* x) {
    if (x == nullptr)
        return BrokerUserRightAssign{.is_null = true};
    BrokerUserRightAssign y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.dr_identity_id = x->DRIdentityID;
    y.tradeable = x->Tradeable;
    return y;
}

CThostFtdcDRTransferField Converter::DRTransferToCpp(DRTransfer x) {
    CThostFtdcDRTransferField y;
    memset(&y, 0, sizeof(y));
    y.OrigDRIdentityID = x.orig_dr_identity_id;
    y.DestDRIdentityID = x.dest_dr_identity_id;
    strcpy(y.OrigBrokerID, x.orig_broker_id.c_str());
    strcpy(y.DestBrokerID, x.dest_broker_id.c_str());
    return y;
}

DRTransfer Converter::CThostFtdcDRTransferFieldToRust(CThostFtdcDRTransferField* x) {
    if (x == nullptr)
        return DRTransfer{.is_null = true};
    DRTransfer y{};
    y.orig_dr_identity_id = x->OrigDRIdentityID;
    y.dest_dr_identity_id = x->DestDRIdentityID;
    y.orig_broker_id = Converter::Gb2312ToRustString(x->OrigBrokerID);
    y.dest_broker_id = Converter::Gb2312ToRustString(x->DestBrokerID);
    return y;
}

CThostFtdcFensUserInfoField Converter::FensUserInfoToCpp(FensUserInfo x) {
    CThostFtdcFensUserInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.LoginMode = x.login_mode;
    return y;
}

FensUserInfo Converter::CThostFtdcFensUserInfoFieldToRust(CThostFtdcFensUserInfoField* x) {
    if (x == nullptr)
        return FensUserInfo{.is_null = true};
    FensUserInfo y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.login_mode = x->LoginMode;
    return y;
}

CThostFtdcCurrTransferIdentityField Converter::CurrTransferIdentityToCpp(CurrTransferIdentity x) {
    CThostFtdcCurrTransferIdentityField y;
    memset(&y, 0, sizeof(y));
    y.IdentityID = x.identity_id;
    return y;
}

CurrTransferIdentity Converter::CThostFtdcCurrTransferIdentityFieldToRust(CThostFtdcCurrTransferIdentityField* x) {
    if (x == nullptr)
        return CurrTransferIdentity{.is_null = true};
    CurrTransferIdentity y{};
    y.identity_id = x->IdentityID;
    return y;
}

CThostFtdcLoginForbiddenUserField Converter::LoginForbiddenUserToCpp(LoginForbiddenUser x) {
    CThostFtdcLoginForbiddenUserField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

LoginForbiddenUser Converter::CThostFtdcLoginForbiddenUserFieldToRust(CThostFtdcLoginForbiddenUserField* x) {
    if (x == nullptr)
        return LoginForbiddenUser{.is_null = true};
    LoginForbiddenUser y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryLoginForbiddenUserField Converter::QryLoginForbiddenUserToCpp(QryLoginForbiddenUser x) {
    CThostFtdcQryLoginForbiddenUserField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    return y;
}

QryLoginForbiddenUser Converter::CThostFtdcQryLoginForbiddenUserFieldToRust(CThostFtdcQryLoginForbiddenUserField* x) {
    if (x == nullptr)
        return QryLoginForbiddenUser{.is_null = true};
    QryLoginForbiddenUser y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    return y;
}

CThostFtdcTradingAccountReserveField Converter::TradingAccountReserveToCpp(TradingAccountReserve x) {
    CThostFtdcTradingAccountReserveField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    y.Reserve = x.reserve;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

TradingAccountReserve Converter::CThostFtdcTradingAccountReserveFieldToRust(CThostFtdcTradingAccountReserveField* x) {
    if (x == nullptr)
        return TradingAccountReserve{.is_null = true};
    TradingAccountReserve y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.reserve = x->Reserve;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcQryLoginForbiddenIPField Converter::QryLoginForbiddenIPToCpp(QryLoginForbiddenIP x) {
    CThostFtdcQryLoginForbiddenIPField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

QryLoginForbiddenIP Converter::CThostFtdcQryLoginForbiddenIPFieldToRust(CThostFtdcQryLoginForbiddenIPField* x) {
    if (x == nullptr)
        return QryLoginForbiddenIP{.is_null = true};
    QryLoginForbiddenIP y{};
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryIPListField Converter::QryIPListToCpp(QryIPList x) {
    CThostFtdcQryIPListField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

QryIPList Converter::CThostFtdcQryIPListFieldToRust(CThostFtdcQryIPListField* x) {
    if (x == nullptr)
        return QryIPList{.is_null = true};
    QryIPList y{};
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryUserRightsAssignField Converter::QryUserRightsAssignToCpp(QryUserRightsAssign x) {
    CThostFtdcQryUserRightsAssignField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    return y;
}

QryUserRightsAssign Converter::CThostFtdcQryUserRightsAssignFieldToRust(CThostFtdcQryUserRightsAssignField* x) {
    if (x == nullptr)
        return QryUserRightsAssign{.is_null = true};
    QryUserRightsAssign y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    return y;
}

CThostFtdcReserveOpenAccountConfirmField Converter::ReserveOpenAccountConfirmToCpp(ReserveOpenAccountConfirm x) {
    CThostFtdcReserveOpenAccountConfirmField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.Gender = x.gender;
    strcpy(y.CountryCode, x.country_code.c_str());
    y.CustType = x.cust_type;
    strcpy(y.Address, x.address.c_str());
    strcpy(y.ZipCode, x.zip_code.c_str());
    strcpy(y.Telephone, x.telephone.c_str());
    strcpy(y.MobilePhone, x.mobile_phone.c_str());
    strcpy(y.Fax, x.fax.c_str());
    strcpy(y.EMail, x.email.c_str());
    y.MoneyAccountStatus = x.money_account_status;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    y.InstallID = x.install_id;
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    y.TID = x.tid;
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    memcpy(y.BankReserveOpenSeq, x.bank_reserve_open_seq.data(), x.bank_reserve_open_seq.size() * sizeof(uint8_t));
    strcpy(y.BookDate, x.book_date.c_str());
    memcpy(y.BookPsw, x.book_psw.data(), x.book_psw.size() * sizeof(uint8_t));
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    return y;
}

ReserveOpenAccountConfirm Converter::CThostFtdcReserveOpenAccountConfirmFieldToRust(CThostFtdcReserveOpenAccountConfirmField* x) {
    if (x == nullptr)
        return ReserveOpenAccountConfirm{.is_null = true};
    ReserveOpenAccountConfirm y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.gender = x->Gender;
    y.country_code = Converter::Gb2312ToRustString(x->CountryCode);
    y.cust_type = x->CustType;
    y.address = Converter::Gb2312ToRustString(x->Address);
    y.zip_code = Converter::Gb2312ToRustString(x->ZipCode);
    y.telephone = Converter::Gb2312ToRustString(x->Telephone);
    y.mobile_phone = Converter::Gb2312ToRustString(x->MobilePhone);
    y.fax = Converter::Gb2312ToRustString(x->Fax);
    y.email = Converter::Gb2312ToRustString(x->EMail);
    y.money_account_status = x->MoneyAccountStatus;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.install_id = x->InstallID;
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    y.tid = x->TID;
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    for (int i = 0; i < 13; i++)
        y.bank_reserve_open_seq.push_back(x->BankReserveOpenSeq[i]);
    y.book_date = Converter::Gb2312ToRustString(x->BookDate);
    for (int i = 0; i < 41; i++)
        y.book_psw.push_back(x->BookPsw[i]);
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    return y;
}

CThostFtdcReserveOpenAccountField Converter::ReserveOpenAccountToCpp(ReserveOpenAccount x) {
    CThostFtdcReserveOpenAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.Gender = x.gender;
    strcpy(y.CountryCode, x.country_code.c_str());
    y.CustType = x.cust_type;
    strcpy(y.Address, x.address.c_str());
    strcpy(y.ZipCode, x.zip_code.c_str());
    strcpy(y.Telephone, x.telephone.c_str());
    strcpy(y.MobilePhone, x.mobile_phone.c_str());
    strcpy(y.Fax, x.fax.c_str());
    strcpy(y.EMail, x.email.c_str());
    y.MoneyAccountStatus = x.money_account_status;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    y.InstallID = x.install_id;
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    y.TID = x.tid;
    y.ReserveOpenAccStas = x.reserve_open_acc_stas;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    return y;
}

ReserveOpenAccount Converter::CThostFtdcReserveOpenAccountFieldToRust(CThostFtdcReserveOpenAccountField* x) {
    if (x == nullptr)
        return ReserveOpenAccount{.is_null = true};
    ReserveOpenAccount y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.gender = x->Gender;
    y.country_code = Converter::Gb2312ToRustString(x->CountryCode);
    y.cust_type = x->CustType;
    y.address = Converter::Gb2312ToRustString(x->Address);
    y.zip_code = Converter::Gb2312ToRustString(x->ZipCode);
    y.telephone = Converter::Gb2312ToRustString(x->Telephone);
    y.mobile_phone = Converter::Gb2312ToRustString(x->MobilePhone);
    y.fax = Converter::Gb2312ToRustString(x->Fax);
    y.email = Converter::Gb2312ToRustString(x->EMail);
    y.money_account_status = x->MoneyAccountStatus;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.install_id = x->InstallID;
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    y.tid = x->TID;
    y.reserve_open_acc_stas = x->ReserveOpenAccStas;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    return y;
}

CThostFtdcAccountPropertyField Converter::AccountPropertyToCpp(AccountProperty x) {
    CThostFtdcAccountPropertyField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.OpenName, x.open_name.c_str());
    memcpy(y.OpenBank, x.open_bank.data(), x.open_bank.size() * sizeof(uint8_t));
    y.IsActive = x.is_active;
    y.AccountSourceType = x.account_source_type;
    strcpy(y.OpenDate, x.open_date.c_str());
    strcpy(y.CancelDate, x.cancel_date.c_str());
    strcpy(y.OperatorID, x.operator_id.c_str());
    strcpy(y.OperateDate, x.operate_date.c_str());
    strcpy(y.OperateTime, x.operate_time.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    return y;
}

AccountProperty Converter::CThostFtdcAccountPropertyFieldToRust(CThostFtdcAccountPropertyField* x) {
    if (x == nullptr)
        return AccountProperty{.is_null = true};
    AccountProperty y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.open_name = Converter::Gb2312ToRustString(x->OpenName);
    for (int i = 0; i < 101; i++)
        y.open_bank.push_back(x->OpenBank[i]);
    y.is_active = x->IsActive;
    y.account_source_type = x->AccountSourceType;
    y.open_date = Converter::Gb2312ToRustString(x->OpenDate);
    y.cancel_date = Converter::Gb2312ToRustString(x->CancelDate);
    y.operator_id = Converter::Gb2312ToRustString(x->OperatorID);
    y.operate_date = Converter::Gb2312ToRustString(x->OperateDate);
    y.operate_time = Converter::Gb2312ToRustString(x->OperateTime);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    return y;
}

CThostFtdcQryCurrDRIdentityField Converter::QryCurrDRIdentityToCpp(QryCurrDRIdentity x) {
    CThostFtdcQryCurrDRIdentityField y;
    memset(&y, 0, sizeof(y));
    y.DRIdentityID = x.dr_identity_id;
    return y;
}

QryCurrDRIdentity Converter::CThostFtdcQryCurrDRIdentityFieldToRust(CThostFtdcQryCurrDRIdentityField* x) {
    if (x == nullptr)
        return QryCurrDRIdentity{.is_null = true};
    QryCurrDRIdentity y{};
    y.dr_identity_id = x->DRIdentityID;
    return y;
}

CThostFtdcCurrDRIdentityField Converter::CurrDRIdentityToCpp(CurrDRIdentity x) {
    CThostFtdcCurrDRIdentityField y;
    memset(&y, 0, sizeof(y));
    y.DRIdentityID = x.dr_identity_id;
    return y;
}

CurrDRIdentity Converter::CThostFtdcCurrDRIdentityFieldToRust(CThostFtdcCurrDRIdentityField* x) {
    if (x == nullptr)
        return CurrDRIdentity{.is_null = true};
    CurrDRIdentity y{};
    y.dr_identity_id = x->DRIdentityID;
    return y;
}

CThostFtdcQrySecAgentCheckModeField Converter::QrySecAgentCheckModeToCpp(QrySecAgentCheckMode x) {
    CThostFtdcQrySecAgentCheckModeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    return y;
}

QrySecAgentCheckMode Converter::CThostFtdcQrySecAgentCheckModeFieldToRust(CThostFtdcQrySecAgentCheckModeField* x) {
    if (x == nullptr)
        return QrySecAgentCheckMode{.is_null = true};
    QrySecAgentCheckMode y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    return y;
}

CThostFtdcQrySecAgentTradeInfoField Converter::QrySecAgentTradeInfoToCpp(QrySecAgentTradeInfo x) {
    CThostFtdcQrySecAgentTradeInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerSecAgentID, x.broker_sec_agent_id.c_str());
    return y;
}

QrySecAgentTradeInfo Converter::CThostFtdcQrySecAgentTradeInfoFieldToRust(CThostFtdcQrySecAgentTradeInfoField* x) {
    if (x == nullptr)
        return QrySecAgentTradeInfo{.is_null = true};
    QrySecAgentTradeInfo y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_sec_agent_id = Converter::Gb2312ToRustString(x->BrokerSecAgentID);
    return y;
}

CThostFtdcReqUserAuthMethodField Converter::ReqUserAuthMethodToCpp(ReqUserAuthMethod x) {
    CThostFtdcReqUserAuthMethodField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    return y;
}

ReqUserAuthMethod Converter::CThostFtdcReqUserAuthMethodFieldToRust(CThostFtdcReqUserAuthMethodField* x) {
    if (x == nullptr)
        return ReqUserAuthMethod{.is_null = true};
    ReqUserAuthMethod y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    return y;
}

CThostFtdcRspUserAuthMethodField Converter::RspUserAuthMethodToCpp(RspUserAuthMethod x) {
    CThostFtdcRspUserAuthMethodField y;
    memset(&y, 0, sizeof(y));
    y.UsableAuthMethod = x.usable_auth_method;
    return y;
}

RspUserAuthMethod Converter::CThostFtdcRspUserAuthMethodFieldToRust(CThostFtdcRspUserAuthMethodField* x) {
    if (x == nullptr)
        return RspUserAuthMethod{.is_null = true};
    RspUserAuthMethod y{};
    y.usable_auth_method = x->UsableAuthMethod;
    return y;
}

CThostFtdcReqGenUserCaptchaField Converter::ReqGenUserCaptchaToCpp(ReqGenUserCaptcha x) {
    CThostFtdcReqGenUserCaptchaField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    return y;
}

ReqGenUserCaptcha Converter::CThostFtdcReqGenUserCaptchaFieldToRust(CThostFtdcReqGenUserCaptchaField* x) {
    if (x == nullptr)
        return ReqGenUserCaptcha{.is_null = true};
    ReqGenUserCaptcha y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    return y;
}

CThostFtdcRspGenUserCaptchaField Converter::RspGenUserCaptchaToCpp(RspGenUserCaptcha x) {
    CThostFtdcRspGenUserCaptchaField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.CaptchaInfoLen = x.captcha_info_len;
    strcpy(y.CaptchaInfo, x.captcha_info.c_str());
    return y;
}

RspGenUserCaptcha Converter::CThostFtdcRspGenUserCaptchaFieldToRust(CThostFtdcRspGenUserCaptchaField* x) {
    if (x == nullptr)
        return RspGenUserCaptcha{.is_null = true};
    RspGenUserCaptcha y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.captcha_info_len = x->CaptchaInfoLen;
    y.captcha_info = Converter::Gb2312ToRustString(x->CaptchaInfo);
    return y;
}

CThostFtdcReqGenUserTextField Converter::ReqGenUserTextToCpp(ReqGenUserText x) {
    CThostFtdcReqGenUserTextField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    return y;
}

ReqGenUserText Converter::CThostFtdcReqGenUserTextFieldToRust(CThostFtdcReqGenUserTextField* x) {
    if (x == nullptr)
        return ReqGenUserText{.is_null = true};
    ReqGenUserText y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    return y;
}

CThostFtdcRspGenUserTextField Converter::RspGenUserTextToCpp(RspGenUserText x) {
    CThostFtdcRspGenUserTextField y;
    memset(&y, 0, sizeof(y));
    y.UserTextSeq = x.user_text_seq;
    return y;
}

RspGenUserText Converter::CThostFtdcRspGenUserTextFieldToRust(CThostFtdcRspGenUserTextField* x) {
    if (x == nullptr)
        return RspGenUserText{.is_null = true};
    RspGenUserText y{};
    y.user_text_seq = x->UserTextSeq;
    return y;
}

CThostFtdcReqUserLoginWithCaptchaField Converter::ReqUserLoginWithCaptchaToCpp(ReqUserLoginWithCaptcha x) {
    CThostFtdcReqUserLoginWithCaptchaField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.Password, x.password.c_str());
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.InterfaceProductInfo, x.interface_product_info.c_str());
    strcpy(y.ProtocolInfo, x.protocol_info.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.LoginRemark, x.login_remark.c_str());
    memcpy(y.Captcha, x.captcha.data(), x.captcha.size() * sizeof(uint8_t));
    y.ClientIPPort = x.client_ip_port;
    strcpy(y.ClientIPAddress, x.client_ip_address.c_str());
    return y;
}

ReqUserLoginWithCaptcha Converter::CThostFtdcReqUserLoginWithCaptchaFieldToRust(CThostFtdcReqUserLoginWithCaptchaField* x) {
    if (x == nullptr)
        return ReqUserLoginWithCaptcha{.is_null = true};
    ReqUserLoginWithCaptcha y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.interface_product_info = Converter::Gb2312ToRustString(x->InterfaceProductInfo);
    y.protocol_info = Converter::Gb2312ToRustString(x->ProtocolInfo);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.login_remark = Converter::Gb2312ToRustString(x->LoginRemark);
    for (int i = 0; i < 41; i++)
        y.captcha.push_back(x->Captcha[i]);
    y.client_ip_port = x->ClientIPPort;
    y.client_ip_address = Converter::Gb2312ToRustString(x->ClientIPAddress);
    return y;
}

CThostFtdcReqUserLoginWithTextField Converter::ReqUserLoginWithTextToCpp(ReqUserLoginWithText x) {
    CThostFtdcReqUserLoginWithTextField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.Password, x.password.c_str());
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.InterfaceProductInfo, x.interface_product_info.c_str());
    strcpy(y.ProtocolInfo, x.protocol_info.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.LoginRemark, x.login_remark.c_str());
    memcpy(y.Text, x.text.data(), x.text.size() * sizeof(uint8_t));
    y.ClientIPPort = x.client_ip_port;
    strcpy(y.ClientIPAddress, x.client_ip_address.c_str());
    return y;
}

ReqUserLoginWithText Converter::CThostFtdcReqUserLoginWithTextFieldToRust(CThostFtdcReqUserLoginWithTextField* x) {
    if (x == nullptr)
        return ReqUserLoginWithText{.is_null = true};
    ReqUserLoginWithText y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.interface_product_info = Converter::Gb2312ToRustString(x->InterfaceProductInfo);
    y.protocol_info = Converter::Gb2312ToRustString(x->ProtocolInfo);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.login_remark = Converter::Gb2312ToRustString(x->LoginRemark);
    for (int i = 0; i < 41; i++)
        y.text.push_back(x->Text[i]);
    y.client_ip_port = x->ClientIPPort;
    y.client_ip_address = Converter::Gb2312ToRustString(x->ClientIPAddress);
    return y;
}

CThostFtdcReqUserLoginWithOTPField Converter::ReqUserLoginWithOTPToCpp(ReqUserLoginWithOTP x) {
    CThostFtdcReqUserLoginWithOTPField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.Password, x.password.c_str());
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.InterfaceProductInfo, x.interface_product_info.c_str());
    strcpy(y.ProtocolInfo, x.protocol_info.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.LoginRemark, x.login_remark.c_str());
    strcpy(y.OTPPassword, x.otp_password.c_str());
    y.ClientIPPort = x.client_ip_port;
    strcpy(y.ClientIPAddress, x.client_ip_address.c_str());
    return y;
}

ReqUserLoginWithOTP Converter::CThostFtdcReqUserLoginWithOTPFieldToRust(CThostFtdcReqUserLoginWithOTPField* x) {
    if (x == nullptr)
        return ReqUserLoginWithOTP{.is_null = true};
    ReqUserLoginWithOTP y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.interface_product_info = Converter::Gb2312ToRustString(x->InterfaceProductInfo);
    y.protocol_info = Converter::Gb2312ToRustString(x->ProtocolInfo);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.login_remark = Converter::Gb2312ToRustString(x->LoginRemark);
    y.otp_password = Converter::Gb2312ToRustString(x->OTPPassword);
    y.client_ip_port = x->ClientIPPort;
    y.client_ip_address = Converter::Gb2312ToRustString(x->ClientIPAddress);
    return y;
}

CThostFtdcReqApiHandshakeField Converter::ReqApiHandshakeToCpp(ReqApiHandshake x) {
    CThostFtdcReqApiHandshakeField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.CryptoKeyVersion, x.crypto_key_version.c_str());
    return y;
}

ReqApiHandshake Converter::CThostFtdcReqApiHandshakeFieldToRust(CThostFtdcReqApiHandshakeField* x) {
    if (x == nullptr)
        return ReqApiHandshake{.is_null = true};
    ReqApiHandshake y{};
    y.crypto_key_version = Converter::Gb2312ToRustString(x->CryptoKeyVersion);
    return y;
}

CThostFtdcRspApiHandshakeField Converter::RspApiHandshakeToCpp(RspApiHandshake x) {
    CThostFtdcRspApiHandshakeField y;
    memset(&y, 0, sizeof(y));
    y.FrontHandshakeDataLen = x.front_handshake_data_len;
    memcpy(y.FrontHandshakeData, x.front_handshake_data.data(), x.front_handshake_data.size() * sizeof(uint8_t));
    y.IsApiAuthEnabled = x.is_api_auth_enabled;
    return y;
}

RspApiHandshake Converter::CThostFtdcRspApiHandshakeFieldToRust(CThostFtdcRspApiHandshakeField* x) {
    if (x == nullptr)
        return RspApiHandshake{.is_null = true};
    RspApiHandshake y{};
    y.front_handshake_data_len = x->FrontHandshakeDataLen;
    for (int i = 0; i < 301; i++)
        y.front_handshake_data.push_back(x->FrontHandshakeData[i]);
    y.is_api_auth_enabled = x->IsApiAuthEnabled;
    return y;
}

CThostFtdcReqVerifyApiKeyField Converter::ReqVerifyApiKeyToCpp(ReqVerifyApiKey x) {
    CThostFtdcReqVerifyApiKeyField y;
    memset(&y, 0, sizeof(y));
    y.ApiHandshakeDataLen = x.api_handshake_data_len;
    memcpy(y.ApiHandshakeData, x.api_handshake_data.data(), x.api_handshake_data.size() * sizeof(uint8_t));
    return y;
}

ReqVerifyApiKey Converter::CThostFtdcReqVerifyApiKeyFieldToRust(CThostFtdcReqVerifyApiKeyField* x) {
    if (x == nullptr)
        return ReqVerifyApiKey{.is_null = true};
    ReqVerifyApiKey y{};
    y.api_handshake_data_len = x->ApiHandshakeDataLen;
    for (int i = 0; i < 301; i++)
        y.api_handshake_data.push_back(x->ApiHandshakeData[i]);
    return y;
}

CThostFtdcDepartmentUserField Converter::DepartmentUserToCpp(DepartmentUser x) {
    CThostFtdcDepartmentUserField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.InvestorRange = x.investor_range;
    strcpy(y.InvestorID, x.investor_id.c_str());
    return y;
}

DepartmentUser Converter::CThostFtdcDepartmentUserFieldToRust(CThostFtdcDepartmentUserField* x) {
    if (x == nullptr)
        return DepartmentUser{.is_null = true};
    DepartmentUser y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.investor_range = x->InvestorRange;
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    return y;
}

CThostFtdcQueryFreqField Converter::QueryFreqToCpp(QueryFreq x) {
    CThostFtdcQueryFreqField y;
    memset(&y, 0, sizeof(y));
    y.QueryFreq = x.query_freq;
    y.FTDPkgFreq = x.ftd_pkg_freq;
    return y;
}

QueryFreq Converter::CThostFtdcQueryFreqFieldToRust(CThostFtdcQueryFreqField* x) {
    if (x == nullptr)
        return QueryFreq{.is_null = true};
    QueryFreq y{};
    y.query_freq = x->QueryFreq;
    y.ftd_pkg_freq = x->FTDPkgFreq;
    return y;
}

CThostFtdcAuthForbiddenIPField Converter::AuthForbiddenIPToCpp(AuthForbiddenIP x) {
    CThostFtdcAuthForbiddenIPField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

AuthForbiddenIP Converter::CThostFtdcAuthForbiddenIPFieldToRust(CThostFtdcAuthForbiddenIPField* x) {
    if (x == nullptr)
        return AuthForbiddenIP{.is_null = true};
    AuthForbiddenIP y{};
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryAuthForbiddenIPField Converter::QryAuthForbiddenIPToCpp(QryAuthForbiddenIP x) {
    CThostFtdcQryAuthForbiddenIPField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

QryAuthForbiddenIP Converter::CThostFtdcQryAuthForbiddenIPFieldToRust(CThostFtdcQryAuthForbiddenIPField* x) {
    if (x == nullptr)
        return QryAuthForbiddenIP{.is_null = true};
    QryAuthForbiddenIP y{};
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcSyncDelaySwapFrozenField Converter::SyncDelaySwapFrozenToCpp(SyncDelaySwapFrozen x) {
    CThostFtdcSyncDelaySwapFrozenField y;
    memset(&y, 0, sizeof(y));
    memcpy(y.DelaySwapSeqNo, x.delay_swap_seq_no.data(), x.delay_swap_seq_no.size() * sizeof(uint8_t));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.FromCurrencyID, x.from_currency_id.c_str());
    y.FromRemainSwap = x.from_remain_swap;
    y.IsManualSwap = x.is_manual_swap;
    return y;
}

SyncDelaySwapFrozen Converter::CThostFtdcSyncDelaySwapFrozenFieldToRust(CThostFtdcSyncDelaySwapFrozenField* x) {
    if (x == nullptr)
        return SyncDelaySwapFrozen{.is_null = true};
    SyncDelaySwapFrozen y{};
    for (int i = 0; i < 15; i++)
        y.delay_swap_seq_no.push_back(x->DelaySwapSeqNo[i]);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.from_currency_id = Converter::Gb2312ToRustString(x->FromCurrencyID);
    y.from_remain_swap = x->FromRemainSwap;
    y.is_manual_swap = x->IsManualSwap;
    return y;
}

CThostFtdcUserSystemInfoField Converter::UserSystemInfoToCpp(UserSystemInfo x) {
    CThostFtdcUserSystemInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.ClientSystemInfoLen = x.client_system_info_len;
    strcpy(y.ClientSystemInfo, x.client_system_info.c_str());
    y.ClientIPPort = x.client_ip_port;
    strcpy(y.ClientLoginTime, x.client_login_time.c_str());
    strcpy(y.ClientAppID, x.client_app_id.c_str());
    strcpy(y.ClientPublicIP, x.client_public_ip.c_str());
    strcpy(y.ClientLoginRemark, x.client_login_remark.c_str());
    return y;
}

UserSystemInfo Converter::CThostFtdcUserSystemInfoFieldToRust(CThostFtdcUserSystemInfoField* x) {
    if (x == nullptr)
        return UserSystemInfo{.is_null = true};
    UserSystemInfo y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.client_system_info_len = x->ClientSystemInfoLen;
    y.client_system_info = Converter::Gb2312ToRustString(x->ClientSystemInfo);
    y.client_ip_port = x->ClientIPPort;
    y.client_login_time = Converter::Gb2312ToRustString(x->ClientLoginTime);
    y.client_app_id = Converter::Gb2312ToRustString(x->ClientAppID);
    y.client_public_ip = Converter::Gb2312ToRustString(x->ClientPublicIP);
    y.client_login_remark = Converter::Gb2312ToRustString(x->ClientLoginRemark);
    return y;
}

CThostFtdcAuthUserIDField Converter::AuthUserIDToCpp(AuthUserID x) {
    CThostFtdcAuthUserIDField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AppID, x.app_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.AuthType = x.auth_type;
    return y;
}

AuthUserID Converter::CThostFtdcAuthUserIDFieldToRust(CThostFtdcAuthUserIDField* x) {
    if (x == nullptr)
        return AuthUserID{.is_null = true};
    AuthUserID y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.app_id = Converter::Gb2312ToRustString(x->AppID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.auth_type = x->AuthType;
    return y;
}

CThostFtdcAuthIPField Converter::AuthIPToCpp(AuthIP x) {
    CThostFtdcAuthIPField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AppID, x.app_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    return y;
}

AuthIP Converter::CThostFtdcAuthIPFieldToRust(CThostFtdcAuthIPField* x) {
    if (x == nullptr)
        return AuthIP{.is_null = true};
    AuthIP y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.app_id = Converter::Gb2312ToRustString(x->AppID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    return y;
}

CThostFtdcQryClassifiedInstrumentField Converter::QryClassifiedInstrumentToCpp(QryClassifiedInstrument x) {
    CThostFtdcQryClassifiedInstrumentField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    y.TradingType = x.trading_type;
    y.ClassType = x.class_type;
    return y;
}

QryClassifiedInstrument Converter::CThostFtdcQryClassifiedInstrumentFieldToRust(CThostFtdcQryClassifiedInstrumentField* x) {
    if (x == nullptr)
        return QryClassifiedInstrument{.is_null = true};
    QryClassifiedInstrument y{};
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.trading_type = x->TradingType;
    y.class_type = x->ClassType;
    return y;
}

CThostFtdcQryCombPromotionParamField Converter::QryCombPromotionParamToCpp(QryCombPromotionParam x) {
    CThostFtdcQryCombPromotionParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryCombPromotionParam Converter::CThostFtdcQryCombPromotionParamFieldToRust(CThostFtdcQryCombPromotionParamField* x) {
    if (x == nullptr)
        return QryCombPromotionParam{.is_null = true};
    QryCombPromotionParam y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcCombPromotionParamField Converter::CombPromotionParamToCpp(CombPromotionParam x) {
    CThostFtdcCombPromotionParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.CombHedgeFlag, x.comb_hedge_flag.c_str());
    y.Xparameter = x.xparameter;
    return y;
}

CombPromotionParam Converter::CThostFtdcCombPromotionParamFieldToRust(CThostFtdcCombPromotionParamField* x) {
    if (x == nullptr)
        return CombPromotionParam{.is_null = true};
    CombPromotionParam y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.comb_hedge_flag = Converter::Gb2312ToRustString(x->CombHedgeFlag);
    y.xparameter = x->Xparameter;
    return y;
}

CThostFtdcReqUserLoginSMField Converter::ReqUserLoginSMToCpp(ReqUserLoginSM x) {
    CThostFtdcReqUserLoginSMField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.Password, x.password.c_str());
    strcpy(y.UserProductInfo, x.user_product_info.c_str());
    strcpy(y.InterfaceProductInfo, x.interface_product_info.c_str());
    strcpy(y.ProtocolInfo, x.protocol_info.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.OneTimePassword, x.one_time_password.c_str());
    strcpy(y.LoginRemark, x.login_remark.c_str());
    y.ClientIPPort = x.client_ip_port;
    strcpy(y.ClientIPAddress, x.client_ip_address.c_str());
    strcpy(y.BrokerName, x.broker_name.c_str());
    strcpy(y.AuthCode, x.auth_code.c_str());
    strcpy(y.AppID, x.app_id.c_str());
    memcpy(y.PIN, x.pin.data(), x.pin.size() * sizeof(uint8_t));
    return y;
}

ReqUserLoginSM Converter::CThostFtdcReqUserLoginSMFieldToRust(CThostFtdcReqUserLoginSMField* x) {
    if (x == nullptr)
        return ReqUserLoginSM{.is_null = true};
    ReqUserLoginSM y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.user_product_info = Converter::Gb2312ToRustString(x->UserProductInfo);
    y.interface_product_info = Converter::Gb2312ToRustString(x->InterfaceProductInfo);
    y.protocol_info = Converter::Gb2312ToRustString(x->ProtocolInfo);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.one_time_password = Converter::Gb2312ToRustString(x->OneTimePassword);
    y.login_remark = Converter::Gb2312ToRustString(x->LoginRemark);
    y.client_ip_port = x->ClientIPPort;
    y.client_ip_address = Converter::Gb2312ToRustString(x->ClientIPAddress);
    y.broker_name = Converter::Gb2312ToRustString(x->BrokerName);
    y.auth_code = Converter::Gb2312ToRustString(x->AuthCode);
    y.app_id = Converter::Gb2312ToRustString(x->AppID);
    for (int i = 0; i < 41; i++)
        y.pin.push_back(x->PIN[i]);
    return y;
}

CThostFtdcQryRiskSettleInvstPositionField Converter::QryRiskSettleInvestPositionToCpp(QryRiskSettleInvestPosition x) {
    CThostFtdcQryRiskSettleInvstPositionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryRiskSettleInvestPosition Converter::CThostFtdcQryRiskSettleInvstPositionFieldToRust(CThostFtdcQryRiskSettleInvstPositionField* x) {
    if (x == nullptr)
        return QryRiskSettleInvestPosition{.is_null = true};
    QryRiskSettleInvestPosition y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryRiskSettleProductStatusField Converter::QryRiskSettleProductStatusToCpp(QryRiskSettleProductStatus x) {
    CThostFtdcQryRiskSettleProductStatusField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ProductID, x.product_id.c_str());
    return y;
}

QryRiskSettleProductStatus Converter::CThostFtdcQryRiskSettleProductStatusFieldToRust(CThostFtdcQryRiskSettleProductStatusField* x) {
    if (x == nullptr)
        return QryRiskSettleProductStatus{.is_null = true};
    QryRiskSettleProductStatus y{};
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    return y;
}

CThostFtdcRiskSettleInvstPositionField Converter::RiskSettleInvestPositionToCpp(RiskSettleInvestPosition x) {
    CThostFtdcRiskSettleInvstPositionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.PosiDirection = x.posi_direction;
    y.HedgeFlag = x.hedge_flag;
    y.PositionDate = x.position_date;
    y.YdPosition = x.yd_position;
    y.Position = x.position;
    y.LongFrozen = x.long_frozen;
    y.ShortFrozen = x.short_frozen;
    y.LongFrozenAmount = x.long_frozen_amount;
    y.ShortFrozenAmount = x.short_frozen_amount;
    y.OpenVolume = x.open_volume;
    y.CloseVolume = x.close_volume;
    y.OpenAmount = x.open_amount;
    y.CloseAmount = x.close_amount;
    y.PositionCost = x.position_cost;
    y.PreMargin = x.pre_margin;
    y.UseMargin = x.use_margin;
    y.FrozenMargin = x.frozen_margin;
    y.FrozenCash = x.frozen_cash;
    y.FrozenCommission = x.frozen_commission;
    y.CashIn = x.cash_in;
    y.Commission = x.commission;
    y.CloseProfit = x.close_profit;
    y.PositionProfit = x.position_profit;
    y.PreSettlementPrice = x.pre_settlement_price;
    y.SettlementPrice = x.settlement_price;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    y.OpenCost = x.open_cost;
    y.ExchangeMargin = x.exchange_margin;
    y.CombPosition = x.comb_position;
    y.CombLongFrozen = x.comb_long_frozen;
    y.CombShortFrozen = x.comb_short_frozen;
    y.CloseProfitByDate = x.close_profit_by_date;
    y.CloseProfitByTrade = x.close_profit_by_trade;
    y.TodayPosition = x.today_position;
    y.MarginRateByMoney = x.margin_rate_by_money;
    y.MarginRateByVolume = x.margin_rate_by_volume;
    y.StrikeFrozen = x.strike_frozen;
    y.StrikeFrozenAmount = x.strike_frozen_amount;
    y.AbandonFrozen = x.abandon_frozen;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.YdStrikeFrozen = x.yd_strike_frozen;
    strcpy(y.InvestUnitID, x.invest_unit_id.c_str());
    y.PositionCostOffset = x.position_cost_offset;
    y.TasPosition = x.tas_position;
    y.TasPositionCost = x.tas_position_cost;
    return y;
}

RiskSettleInvestPosition Converter::CThostFtdcRiskSettleInvstPositionFieldToRust(CThostFtdcRiskSettleInvstPositionField* x) {
    if (x == nullptr)
        return RiskSettleInvestPosition{.is_null = true};
    RiskSettleInvestPosition y{};
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.posi_direction = x->PosiDirection;
    y.hedge_flag = x->HedgeFlag;
    y.position_date = x->PositionDate;
    y.yd_position = x->YdPosition;
    y.position = x->Position;
    y.long_frozen = x->LongFrozen;
    y.short_frozen = x->ShortFrozen;
    y.long_frozen_amount = x->LongFrozenAmount;
    y.short_frozen_amount = x->ShortFrozenAmount;
    y.open_volume = x->OpenVolume;
    y.close_volume = x->CloseVolume;
    y.open_amount = x->OpenAmount;
    y.close_amount = x->CloseAmount;
    y.position_cost = x->PositionCost;
    y.pre_margin = x->PreMargin;
    y.use_margin = x->UseMargin;
    y.frozen_margin = x->FrozenMargin;
    y.frozen_cash = x->FrozenCash;
    y.frozen_commission = x->FrozenCommission;
    y.cash_in = x->CashIn;
    y.commission = x->Commission;
    y.close_profit = x->CloseProfit;
    y.position_profit = x->PositionProfit;
    y.pre_settlement_price = x->PreSettlementPrice;
    y.settlement_price = x->SettlementPrice;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.open_cost = x->OpenCost;
    y.exchange_margin = x->ExchangeMargin;
    y.comb_position = x->CombPosition;
    y.comb_long_frozen = x->CombLongFrozen;
    y.comb_short_frozen = x->CombShortFrozen;
    y.close_profit_by_date = x->CloseProfitByDate;
    y.close_profit_by_trade = x->CloseProfitByTrade;
    y.today_position = x->TodayPosition;
    y.margin_rate_by_money = x->MarginRateByMoney;
    y.margin_rate_by_volume = x->MarginRateByVolume;
    y.strike_frozen = x->StrikeFrozen;
    y.strike_frozen_amount = x->StrikeFrozenAmount;
    y.abandon_frozen = x->AbandonFrozen;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.yd_strike_frozen = x->YdStrikeFrozen;
    y.invest_unit_id = Converter::Gb2312ToRustString(x->InvestUnitID);
    y.position_cost_offset = x->PositionCostOffset;
    y.tas_position = x->TasPosition;
    y.tas_position_cost = x->TasPositionCost;
    return y;
}

CThostFtdcRiskSettleProductStatusField Converter::RiskSettleProductStatusToCpp(RiskSettleProductStatus x) {
    CThostFtdcRiskSettleProductStatusField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    y.ProductStatus = x.product_status;
    return y;
}

RiskSettleProductStatus Converter::CThostFtdcRiskSettleProductStatusFieldToRust(CThostFtdcRiskSettleProductStatusField* x) {
    if (x == nullptr)
        return RiskSettleProductStatus{.is_null = true};
    RiskSettleProductStatus y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.product_status = x->ProductStatus;
    return y;
}

CThostFtdcSyncDeltaInfoField Converter::SyncDeltaInfoToCpp(SyncDeltaInfo x) {
    CThostFtdcSyncDeltaInfoField y;
    memset(&y, 0, sizeof(y));
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    y.SyncDeltaStatus = x.sync_delta_status;
    memcpy(y.SyncDescription, x.sync_description.data(), x.sync_description.size() * sizeof(uint8_t));
    y.IsOnlyTrdDelta = x.is_only_trd_delta;
    return y;
}

SyncDeltaInfo Converter::CThostFtdcSyncDeltaInfoFieldToRust(CThostFtdcSyncDeltaInfoField* x) {
    if (x == nullptr)
        return SyncDeltaInfo{.is_null = true};
    SyncDeltaInfo y{};
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    y.sync_delta_status = x->SyncDeltaStatus;
    for (int i = 0; i < 257; i++)
        y.sync_description.push_back(x->SyncDescription[i]);
    y.is_only_trd_delta = x->IsOnlyTrdDelta;
    return y;
}

CThostFtdcSyncDeltaProductStatusField Converter::SyncDeltaProductStatusToCpp(SyncDeltaProductStatus x) {
    CThostFtdcSyncDeltaProductStatusField y;
    memset(&y, 0, sizeof(y));
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    y.ProductStatus = x.product_status;
    return y;
}

SyncDeltaProductStatus Converter::CThostFtdcSyncDeltaProductStatusFieldToRust(CThostFtdcSyncDeltaProductStatusField* x) {
    if (x == nullptr)
        return SyncDeltaProductStatus{.is_null = true};
    SyncDeltaProductStatus y{};
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.product_status = x->ProductStatus;
    return y;
}

CThostFtdcSyncDeltaInvstPosDtlField Converter::SyncDeltaInvestPosDtlToCpp(SyncDeltaInvestPosDtl x) {
    CThostFtdcSyncDeltaInvstPosDtlField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.Direction = x.direction;
    strcpy(y.OpenDate, x.open_date.c_str());
    strcpy(y.TradeID, x.trade_id.c_str());
    y.Volume = x.volume;
    y.OpenPrice = x.open_price;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    y.TradeType = x.trade_type;
    strcpy(y.CombInstrumentID, x.comb_instrument_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.CloseProfitByDate = x.close_profit_by_date;
    y.CloseProfitByTrade = x.close_profit_by_trade;
    y.PositionProfitByDate = x.position_profit_by_date;
    y.PositionProfitByTrade = x.position_profit_by_trade;
    y.Margin = x.margin;
    y.ExchMargin = x.exch_margin;
    y.MarginRateByMoney = x.margin_rate_by_money;
    y.MarginRateByVolume = x.margin_rate_by_volume;
    y.LastSettlementPrice = x.last_settlement_price;
    y.SettlementPrice = x.settlement_price;
    y.CloseVolume = x.close_volume;
    y.CloseAmount = x.close_amount;
    y.TimeFirstVolume = x.time_first_volume;
    y.SpecPosiType = x.spec_posi_type;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaInvestPosDtl Converter::CThostFtdcSyncDeltaInvstPosDtlFieldToRust(CThostFtdcSyncDeltaInvstPosDtlField* x) {
    if (x == nullptr)
        return SyncDeltaInvestPosDtl{.is_null = true};
    SyncDeltaInvestPosDtl y{};
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.hedge_flag = x->HedgeFlag;
    y.direction = x->Direction;
    y.open_date = Converter::Gb2312ToRustString(x->OpenDate);
    y.trade_id = Converter::Gb2312ToRustString(x->TradeID);
    y.volume = x->Volume;
    y.open_price = x->OpenPrice;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.trade_type = x->TradeType;
    y.comb_instrument_id = Converter::Gb2312ToRustString(x->CombInstrumentID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.close_profit_by_date = x->CloseProfitByDate;
    y.close_profit_by_trade = x->CloseProfitByTrade;
    y.position_profit_by_date = x->PositionProfitByDate;
    y.position_profit_by_trade = x->PositionProfitByTrade;
    y.margin = x->Margin;
    y.exch_margin = x->ExchMargin;
    y.margin_rate_by_money = x->MarginRateByMoney;
    y.margin_rate_by_volume = x->MarginRateByVolume;
    y.last_settlement_price = x->LastSettlementPrice;
    y.settlement_price = x->SettlementPrice;
    y.close_volume = x->CloseVolume;
    y.close_amount = x->CloseAmount;
    y.time_first_volume = x->TimeFirstVolume;
    y.spec_posi_type = x->SpecPosiType;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaInvstPosCombDtlField Converter::SyncDeltaInvestPosCombDtlToCpp(SyncDeltaInvestPosCombDtl x) {
    CThostFtdcSyncDeltaInvstPosCombDtlField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.OpenDate, x.open_date.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ComTradeID, x.com_trade_id.c_str());
    strcpy(y.TradeID, x.trade_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.Direction = x.direction;
    y.TotalAmt = x.total_amt;
    y.Margin = x.margin;
    y.ExchMargin = x.exch_margin;
    y.MarginRateByMoney = x.margin_rate_by_money;
    y.MarginRateByVolume = x.margin_rate_by_volume;
    y.LegID = x.leg_id;
    y.LegMultiple = x.leg_multiple;
    y.TradeGroupID = x.trade_group_id;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaInvestPosCombDtl Converter::CThostFtdcSyncDeltaInvstPosCombDtlFieldToRust(CThostFtdcSyncDeltaInvstPosCombDtlField* x) {
    if (x == nullptr)
        return SyncDeltaInvestPosCombDtl{.is_null = true};
    SyncDeltaInvestPosCombDtl y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.open_date = Converter::Gb2312ToRustString(x->OpenDate);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.settlement_id = x->SettlementID;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.com_trade_id = Converter::Gb2312ToRustString(x->ComTradeID);
    y.trade_id = Converter::Gb2312ToRustString(x->TradeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.hedge_flag = x->HedgeFlag;
    y.direction = x->Direction;
    y.total_amt = x->TotalAmt;
    y.margin = x->Margin;
    y.exch_margin = x->ExchMargin;
    y.margin_rate_by_money = x->MarginRateByMoney;
    y.margin_rate_by_volume = x->MarginRateByVolume;
    y.leg_id = x->LegID;
    y.leg_multiple = x->LegMultiple;
    y.trade_group_id = x->TradeGroupID;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaTradingAccountField Converter::SyncDeltaTradingAccountToCpp(SyncDeltaTradingAccount x) {
    CThostFtdcSyncDeltaTradingAccountField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    y.PreMortgage = x.pre_mortgage;
    y.PreCredit = x.pre_credit;
    y.PreDeposit = x.pre_deposit;
    y.PreBalance = x.pre_balance;
    y.PreMargin = x.pre_margin;
    y.InterestBase = x.interest_base;
    y.Interest = x.interest;
    y.Deposit = x.deposit;
    y.Withdraw = x.withdraw;
    y.FrozenMargin = x.frozen_margin;
    y.FrozenCash = x.frozen_cash;
    y.FrozenCommission = x.frozen_commission;
    y.CurrMargin = x.curr_margin;
    y.CashIn = x.cash_in;
    y.Commission = x.commission;
    y.CloseProfit = x.close_profit;
    y.PositionProfit = x.position_profit;
    y.Balance = x.balance;
    y.Available = x.available;
    y.WithdrawQuota = x.withdraw_quota;
    y.Reserve = x.reserve;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    y.Credit = x.credit;
    y.Mortgage = x.mortgage;
    y.ExchangeMargin = x.exchange_margin;
    y.DeliveryMargin = x.delivery_margin;
    y.ExchangeDeliveryMargin = x.exchange_delivery_margin;
    y.ReserveBalance = x.reserve_balance;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.PreFundMortgageIn = x.pre_fund_mortgage_in;
    y.PreFundMortgageOut = x.pre_fund_mortgage_out;
    y.FundMortgageIn = x.fund_mortgage_in;
    y.FundMortgageOut = x.fund_mortgage_out;
    y.FundMortgageAvailable = x.fund_mortgage_available;
    y.MortgageableFund = x.mortgageable_fund;
    y.SpecProductMargin = x.spec_product_margin;
    y.SpecProductFrozenMargin = x.spec_product_frozen_margin;
    y.SpecProductCommission = x.spec_product_commission;
    y.SpecProductFrozenCommission = x.spec_product_frozen_commission;
    y.SpecProductPositionProfit = x.spec_product_position_profit;
    y.SpecProductCloseProfit = x.spec_product_close_profit;
    y.SpecProductPositionProfitByAlg = x.spec_product_position_profit_by_alg;
    y.SpecProductExchangeMargin = x.spec_product_exchange_margin;
    y.FrozenSwap = x.frozen_swap;
    y.RemainSwap = x.remain_swap;
    y.OptionValue = x.option_value;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaTradingAccount Converter::CThostFtdcSyncDeltaTradingAccountFieldToRust(CThostFtdcSyncDeltaTradingAccountField* x) {
    if (x == nullptr)
        return SyncDeltaTradingAccount{.is_null = true};
    SyncDeltaTradingAccount y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.pre_mortgage = x->PreMortgage;
    y.pre_credit = x->PreCredit;
    y.pre_deposit = x->PreDeposit;
    y.pre_balance = x->PreBalance;
    y.pre_margin = x->PreMargin;
    y.interest_base = x->InterestBase;
    y.interest = x->Interest;
    y.deposit = x->Deposit;
    y.withdraw = x->Withdraw;
    y.frozen_margin = x->FrozenMargin;
    y.frozen_cash = x->FrozenCash;
    y.frozen_commission = x->FrozenCommission;
    y.curr_margin = x->CurrMargin;
    y.cash_in = x->CashIn;
    y.commission = x->Commission;
    y.close_profit = x->CloseProfit;
    y.position_profit = x->PositionProfit;
    y.balance = x->Balance;
    y.available = x->Available;
    y.withdraw_quota = x->WithdrawQuota;
    y.reserve = x->Reserve;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.credit = x->Credit;
    y.mortgage = x->Mortgage;
    y.exchange_margin = x->ExchangeMargin;
    y.delivery_margin = x->DeliveryMargin;
    y.exchange_delivery_margin = x->ExchangeDeliveryMargin;
    y.reserve_balance = x->ReserveBalance;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.pre_fund_mortgage_in = x->PreFundMortgageIn;
    y.pre_fund_mortgage_out = x->PreFundMortgageOut;
    y.fund_mortgage_in = x->FundMortgageIn;
    y.fund_mortgage_out = x->FundMortgageOut;
    y.fund_mortgage_available = x->FundMortgageAvailable;
    y.mortgageable_fund = x->MortgageableFund;
    y.spec_product_margin = x->SpecProductMargin;
    y.spec_product_frozen_margin = x->SpecProductFrozenMargin;
    y.spec_product_commission = x->SpecProductCommission;
    y.spec_product_frozen_commission = x->SpecProductFrozenCommission;
    y.spec_product_position_profit = x->SpecProductPositionProfit;
    y.spec_product_close_profit = x->SpecProductCloseProfit;
    y.spec_product_position_profit_by_alg = x->SpecProductPositionProfitByAlg;
    y.spec_product_exchange_margin = x->SpecProductExchangeMargin;
    y.frozen_swap = x->FrozenSwap;
    y.remain_swap = x->RemainSwap;
    y.option_value = x->OptionValue;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaInitInvstMarginField Converter::SyncDeltaInitInvestMarginToCpp(SyncDeltaInitInvestMargin x) {
    CThostFtdcSyncDeltaInitInvstMarginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.LastRiskTotalInvstMargin = x.last_risk_total_invest_margin;
    y.LastRiskTotalExchMargin = x.last_risk_total_exch_margin;
    y.ThisSyncInvstMargin = x.this_sync_invest_margin;
    y.ThisSyncExchMargin = x.this_sync_exch_margin;
    y.RemainRiskInvstMargin = x.remain_risk_invest_margin;
    y.RemainRiskExchMargin = x.remain_risk_exch_margin;
    y.LastRiskSpecTotalInvstMargin = x.last_risk_spec_total_invest_margin;
    y.LastRiskSpecTotalExchMargin = x.last_risk_spec_total_exch_margin;
    y.ThisSyncSpecInvstMargin = x.this_sync_spec_invest_margin;
    y.ThisSyncSpecExchMargin = x.this_sync_spec_exch_margin;
    y.RemainRiskSpecInvstMargin = x.remain_risk_spec_invest_margin;
    y.RemainRiskSpecExchMargin = x.remain_risk_spec_exch_margin;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaInitInvestMargin Converter::CThostFtdcSyncDeltaInitInvstMarginFieldToRust(CThostFtdcSyncDeltaInitInvstMarginField* x) {
    if (x == nullptr)
        return SyncDeltaInitInvestMargin{.is_null = true};
    SyncDeltaInitInvestMargin y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.last_risk_total_invest_margin = x->LastRiskTotalInvstMargin;
    y.last_risk_total_exch_margin = x->LastRiskTotalExchMargin;
    y.this_sync_invest_margin = x->ThisSyncInvstMargin;
    y.this_sync_exch_margin = x->ThisSyncExchMargin;
    y.remain_risk_invest_margin = x->RemainRiskInvstMargin;
    y.remain_risk_exch_margin = x->RemainRiskExchMargin;
    y.last_risk_spec_total_invest_margin = x->LastRiskSpecTotalInvstMargin;
    y.last_risk_spec_total_exch_margin = x->LastRiskSpecTotalExchMargin;
    y.this_sync_spec_invest_margin = x->ThisSyncSpecInvstMargin;
    y.this_sync_spec_exch_margin = x->ThisSyncSpecExchMargin;
    y.remain_risk_spec_invest_margin = x->RemainRiskSpecInvstMargin;
    y.remain_risk_spec_exch_margin = x->RemainRiskSpecExchMargin;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaDceCombInstrumentField Converter::SyncDeltaDceCombInstrumentToCpp(SyncDeltaDceCombInstrument x) {
    CThostFtdcSyncDeltaDceCombInstrumentField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.CombInstrumentID, x.comb_instrument_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    y.TradeGroupID = x.trade_group_id;
    y.CombHedgeFlag = x.comb_hedge_flag;
    y.CombinationType = x.combination_type;
    y.Direction = x.direction;
    strcpy(y.ProductID, x.product_id.c_str());
    y.Xparameter = x.xparameter;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaDceCombInstrument Converter::CThostFtdcSyncDeltaDceCombInstrumentFieldToRust(CThostFtdcSyncDeltaDceCombInstrumentField* x) {
    if (x == nullptr)
        return SyncDeltaDceCombInstrument{.is_null = true};
    SyncDeltaDceCombInstrument y{};
    y.comb_instrument_id = Converter::Gb2312ToRustString(x->CombInstrumentID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.trade_group_id = x->TradeGroupID;
    y.comb_hedge_flag = x->CombHedgeFlag;
    y.combination_type = x->CombinationType;
    y.direction = x->Direction;
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.xparameter = x->Xparameter;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaInvstMarginRateField Converter::SyncDeltaInvestMarginRateToCpp(SyncDeltaInvestMarginRate x) {
    CThostFtdcSyncDeltaInvstMarginRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.LongMarginRatioByMoney = x.long_margin_ratio_by_money;
    y.LongMarginRatioByVolume = x.long_margin_ratio_by_volume;
    y.ShortMarginRatioByMoney = x.short_margin_ratio_by_money;
    y.ShortMarginRatioByVolume = x.short_margin_ratio_by_volume;
    y.IsRelative = x.is_relative;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaInvestMarginRate Converter::CThostFtdcSyncDeltaInvstMarginRateFieldToRust(CThostFtdcSyncDeltaInvstMarginRateField* x) {
    if (x == nullptr)
        return SyncDeltaInvestMarginRate{.is_null = true};
    SyncDeltaInvestMarginRate y{};
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.hedge_flag = x->HedgeFlag;
    y.long_margin_ratio_by_money = x->LongMarginRatioByMoney;
    y.long_margin_ratio_by_volume = x->LongMarginRatioByVolume;
    y.short_margin_ratio_by_money = x->ShortMarginRatioByMoney;
    y.short_margin_ratio_by_volume = x->ShortMarginRatioByVolume;
    y.is_relative = x->IsRelative;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaExchMarginRateField Converter::SyncDeltaExchMarginRateToCpp(SyncDeltaExchMarginRate x) {
    CThostFtdcSyncDeltaExchMarginRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.LongMarginRatioByMoney = x.long_margin_ratio_by_money;
    y.LongMarginRatioByVolume = x.long_margin_ratio_by_volume;
    y.ShortMarginRatioByMoney = x.short_margin_ratio_by_money;
    y.ShortMarginRatioByVolume = x.short_margin_ratio_by_volume;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaExchMarginRate Converter::CThostFtdcSyncDeltaExchMarginRateFieldToRust(CThostFtdcSyncDeltaExchMarginRateField* x) {
    if (x == nullptr)
        return SyncDeltaExchMarginRate{.is_null = true};
    SyncDeltaExchMarginRate y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.hedge_flag = x->HedgeFlag;
    y.long_margin_ratio_by_money = x->LongMarginRatioByMoney;
    y.long_margin_ratio_by_volume = x->LongMarginRatioByVolume;
    y.short_margin_ratio_by_money = x->ShortMarginRatioByMoney;
    y.short_margin_ratio_by_volume = x->ShortMarginRatioByVolume;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaOptExchMarginField Converter::SyncDeltaOptExchMarginToCpp(SyncDeltaOptExchMargin x) {
    CThostFtdcSyncDeltaOptExchMarginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.SShortMarginRatioByMoney = x.s_short_margin_ratio_by_money;
    y.SShortMarginRatioByVolume = x.s_short_margin_ratio_by_volume;
    y.HShortMarginRatioByMoney = x.h_short_margin_ratio_by_money;
    y.HShortMarginRatioByVolume = x.h_short_margin_ratio_by_volume;
    y.AShortMarginRatioByMoney = x.a_short_margin_ratio_by_money;
    y.AShortMarginRatioByVolume = x.a_short_margin_ratio_by_volume;
    y.MShortMarginRatioByMoney = x.m_short_margin_ratio_by_money;
    y.MShortMarginRatioByVolume = x.m_short_margin_ratio_by_volume;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaOptExchMargin Converter::CThostFtdcSyncDeltaOptExchMarginFieldToRust(CThostFtdcSyncDeltaOptExchMarginField* x) {
    if (x == nullptr)
        return SyncDeltaOptExchMargin{.is_null = true};
    SyncDeltaOptExchMargin y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.s_short_margin_ratio_by_money = x->SShortMarginRatioByMoney;
    y.s_short_margin_ratio_by_volume = x->SShortMarginRatioByVolume;
    y.h_short_margin_ratio_by_money = x->HShortMarginRatioByMoney;
    y.h_short_margin_ratio_by_volume = x->HShortMarginRatioByVolume;
    y.a_short_margin_ratio_by_money = x->AShortMarginRatioByMoney;
    y.a_short_margin_ratio_by_volume = x->AShortMarginRatioByVolume;
    y.m_short_margin_ratio_by_money = x->MShortMarginRatioByMoney;
    y.m_short_margin_ratio_by_volume = x->MShortMarginRatioByVolume;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaOptInvstMarginField Converter::SyncDeltaOptInvestMarginToCpp(SyncDeltaOptInvestMargin x) {
    CThostFtdcSyncDeltaOptInvstMarginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.SShortMarginRatioByMoney = x.s_short_margin_ratio_by_money;
    y.SShortMarginRatioByVolume = x.s_short_margin_ratio_by_volume;
    y.HShortMarginRatioByMoney = x.h_short_margin_ratio_by_money;
    y.HShortMarginRatioByVolume = x.h_short_margin_ratio_by_volume;
    y.AShortMarginRatioByMoney = x.a_short_margin_ratio_by_money;
    y.AShortMarginRatioByVolume = x.a_short_margin_ratio_by_volume;
    y.IsRelative = x.is_relative;
    y.MShortMarginRatioByMoney = x.m_short_margin_ratio_by_money;
    y.MShortMarginRatioByVolume = x.m_short_margin_ratio_by_volume;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaOptInvestMargin Converter::CThostFtdcSyncDeltaOptInvstMarginFieldToRust(CThostFtdcSyncDeltaOptInvstMarginField* x) {
    if (x == nullptr)
        return SyncDeltaOptInvestMargin{.is_null = true};
    SyncDeltaOptInvestMargin y{};
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.s_short_margin_ratio_by_money = x->SShortMarginRatioByMoney;
    y.s_short_margin_ratio_by_volume = x->SShortMarginRatioByVolume;
    y.h_short_margin_ratio_by_money = x->HShortMarginRatioByMoney;
    y.h_short_margin_ratio_by_volume = x->HShortMarginRatioByVolume;
    y.a_short_margin_ratio_by_money = x->AShortMarginRatioByMoney;
    y.a_short_margin_ratio_by_volume = x->AShortMarginRatioByVolume;
    y.is_relative = x->IsRelative;
    y.m_short_margin_ratio_by_money = x->MShortMarginRatioByMoney;
    y.m_short_margin_ratio_by_volume = x->MShortMarginRatioByVolume;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaInvstMarginRateULField Converter::SyncDeltaInvestMarginRateULToCpp(SyncDeltaInvestMarginRateUL x) {
    CThostFtdcSyncDeltaInvstMarginRateULField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.LongMarginRatioByMoney = x.long_margin_ratio_by_money;
    y.LongMarginRatioByVolume = x.long_margin_ratio_by_volume;
    y.ShortMarginRatioByMoney = x.short_margin_ratio_by_money;
    y.ShortMarginRatioByVolume = x.short_margin_ratio_by_volume;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaInvestMarginRateUL Converter::CThostFtdcSyncDeltaInvstMarginRateULFieldToRust(CThostFtdcSyncDeltaInvstMarginRateULField* x) {
    if (x == nullptr)
        return SyncDeltaInvestMarginRateUL{.is_null = true};
    SyncDeltaInvestMarginRateUL y{};
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.hedge_flag = x->HedgeFlag;
    y.long_margin_ratio_by_money = x->LongMarginRatioByMoney;
    y.long_margin_ratio_by_volume = x->LongMarginRatioByVolume;
    y.short_margin_ratio_by_money = x->ShortMarginRatioByMoney;
    y.short_margin_ratio_by_volume = x->ShortMarginRatioByVolume;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaOptInvstCommRateField Converter::SyncDeltaOptInvestCommRateToCpp(SyncDeltaOptInvestCommRate x) {
    CThostFtdcSyncDeltaOptInvstCommRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OpenRatioByMoney = x.open_ratio_by_money;
    y.OpenRatioByVolume = x.open_ratio_by_volume;
    y.CloseRatioByMoney = x.close_ratio_by_money;
    y.CloseRatioByVolume = x.close_ratio_by_volume;
    y.CloseTodayRatioByMoney = x.close_today_ratio_by_money;
    y.CloseTodayRatioByVolume = x.close_today_ratio_by_volume;
    y.StrikeRatioByMoney = x.strike_ratio_by_money;
    y.StrikeRatioByVolume = x.strike_ratio_by_volume;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaOptInvestCommRate Converter::CThostFtdcSyncDeltaOptInvstCommRateFieldToRust(CThostFtdcSyncDeltaOptInvstCommRateField* x) {
    if (x == nullptr)
        return SyncDeltaOptInvestCommRate{.is_null = true};
    SyncDeltaOptInvestCommRate y{};
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.open_ratio_by_money = x->OpenRatioByMoney;
    y.open_ratio_by_volume = x->OpenRatioByVolume;
    y.close_ratio_by_money = x->CloseRatioByMoney;
    y.close_ratio_by_volume = x->CloseRatioByVolume;
    y.close_today_ratio_by_money = x->CloseTodayRatioByMoney;
    y.close_today_ratio_by_volume = x->CloseTodayRatioByVolume;
    y.strike_ratio_by_money = x->StrikeRatioByMoney;
    y.strike_ratio_by_volume = x->StrikeRatioByVolume;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaInvstCommRateField Converter::SyncDeltaInvestCommRateToCpp(SyncDeltaInvestCommRate x) {
    CThostFtdcSyncDeltaInvstCommRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.OpenRatioByMoney = x.open_ratio_by_money;
    y.OpenRatioByVolume = x.open_ratio_by_volume;
    y.CloseRatioByMoney = x.close_ratio_by_money;
    y.CloseRatioByVolume = x.close_ratio_by_volume;
    y.CloseTodayRatioByMoney = x.close_today_ratio_by_money;
    y.CloseTodayRatioByVolume = x.close_today_ratio_by_volume;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaInvestCommRate Converter::CThostFtdcSyncDeltaInvstCommRateFieldToRust(CThostFtdcSyncDeltaInvstCommRateField* x) {
    if (x == nullptr)
        return SyncDeltaInvestCommRate{.is_null = true};
    SyncDeltaInvestCommRate y{};
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.open_ratio_by_money = x->OpenRatioByMoney;
    y.open_ratio_by_volume = x->OpenRatioByVolume;
    y.close_ratio_by_money = x->CloseRatioByMoney;
    y.close_ratio_by_volume = x->CloseRatioByVolume;
    y.close_today_ratio_by_money = x->CloseTodayRatioByMoney;
    y.close_today_ratio_by_volume = x->CloseTodayRatioByVolume;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaProductExchRateField Converter::SyncDeltaProductExchRateToCpp(SyncDeltaProductExchRate x) {
    CThostFtdcSyncDeltaProductExchRateField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ProductID, x.product_id.c_str());
    strcpy(y.QuoteCurrencyID, x.quote_currency_id.c_str());
    y.ExchangeRate = x.exchange_rate;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaProductExchRate Converter::CThostFtdcSyncDeltaProductExchRateFieldToRust(CThostFtdcSyncDeltaProductExchRateField* x) {
    if (x == nullptr)
        return SyncDeltaProductExchRate{.is_null = true};
    SyncDeltaProductExchRate y{};
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.quote_currency_id = Converter::Gb2312ToRustString(x->QuoteCurrencyID);
    y.exchange_rate = x->ExchangeRate;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaDepthMarketDataField Converter::SyncDeltaDepthMarketDataToCpp(SyncDeltaDepthMarketData x) {
    CThostFtdcSyncDeltaDepthMarketDataField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    y.LastPrice = x.last_price;
    y.PreSettlementPrice = x.pre_settlement_price;
    y.PreClosePrice = x.pre_close_price;
    y.PreOpenInterest = x.pre_open_interest;
    y.OpenPrice = x.open_price;
    y.HighestPrice = x.highest_price;
    y.LowestPrice = x.lowest_price;
    y.Volume = x.volume;
    y.Turnover = x.turnover;
    y.OpenInterest = x.open_interest;
    y.ClosePrice = x.close_price;
    y.SettlementPrice = x.settlement_price;
    y.UpperLimitPrice = x.upper_limit_price;
    y.LowerLimitPrice = x.lower_limit_price;
    y.PreDelta = x.pre_delta;
    y.CurrDelta = x.curr_delta;
    strcpy(y.UpdateTime, x.update_time.c_str());
    y.UpdateMillisec = x.update_millisecond;
    y.BidPrice1 = x.bid_price1;
    y.BidVolume1 = x.bid_volume1;
    y.AskPrice1 = x.ask_price1;
    y.AskVolume1 = x.ask_volume1;
    y.BidPrice2 = x.bid_price2;
    y.BidVolume2 = x.bid_volume2;
    y.AskPrice2 = x.ask_price2;
    y.AskVolume2 = x.ask_volume2;
    y.BidPrice3 = x.bid_price3;
    y.BidVolume3 = x.bid_volume3;
    y.AskPrice3 = x.ask_price3;
    y.AskVolume3 = x.ask_volume3;
    y.BidPrice4 = x.bid_price4;
    y.BidVolume4 = x.bid_volume4;
    y.AskPrice4 = x.ask_price4;
    y.AskVolume4 = x.ask_volume4;
    y.BidPrice5 = x.bid_price5;
    y.BidVolume5 = x.bid_volume5;
    y.AskPrice5 = x.ask_price5;
    y.AskVolume5 = x.ask_volume5;
    y.AveragePrice = x.average_price;
    strcpy(y.ActionDay, x.action_day.c_str());
    y.BandingUpperPrice = x.banding_upper_price;
    y.BandingLowerPrice = x.banding_lower_price;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaDepthMarketData Converter::CThostFtdcSyncDeltaDepthMarketDataFieldToRust(CThostFtdcSyncDeltaDepthMarketDataField* x) {
    if (x == nullptr)
        return SyncDeltaDepthMarketData{.is_null = true};
    SyncDeltaDepthMarketData y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.last_price = x->LastPrice;
    y.pre_settlement_price = x->PreSettlementPrice;
    y.pre_close_price = x->PreClosePrice;
    y.pre_open_interest = x->PreOpenInterest;
    y.open_price = x->OpenPrice;
    y.highest_price = x->HighestPrice;
    y.lowest_price = x->LowestPrice;
    y.volume = x->Volume;
    y.turnover = x->Turnover;
    y.open_interest = x->OpenInterest;
    y.close_price = x->ClosePrice;
    y.settlement_price = x->SettlementPrice;
    y.upper_limit_price = x->UpperLimitPrice;
    y.lower_limit_price = x->LowerLimitPrice;
    y.pre_delta = x->PreDelta;
    y.curr_delta = x->CurrDelta;
    y.update_time = Converter::Gb2312ToRustString(x->UpdateTime);
    y.update_millisecond = x->UpdateMillisec;
    y.bid_price1 = x->BidPrice1;
    y.bid_volume1 = x->BidVolume1;
    y.ask_price1 = x->AskPrice1;
    y.ask_volume1 = x->AskVolume1;
    y.bid_price2 = x->BidPrice2;
    y.bid_volume2 = x->BidVolume2;
    y.ask_price2 = x->AskPrice2;
    y.ask_volume2 = x->AskVolume2;
    y.bid_price3 = x->BidPrice3;
    y.bid_volume3 = x->BidVolume3;
    y.ask_price3 = x->AskPrice3;
    y.ask_volume3 = x->AskVolume3;
    y.bid_price4 = x->BidPrice4;
    y.bid_volume4 = x->BidVolume4;
    y.ask_price4 = x->AskPrice4;
    y.ask_volume4 = x->AskVolume4;
    y.bid_price5 = x->BidPrice5;
    y.bid_volume5 = x->BidVolume5;
    y.ask_price5 = x->AskPrice5;
    y.ask_volume5 = x->AskVolume5;
    y.average_price = x->AveragePrice;
    y.action_day = Converter::Gb2312ToRustString(x->ActionDay);
    y.banding_upper_price = x->BandingUpperPrice;
    y.banding_lower_price = x->BandingLowerPrice;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaIndexPriceField Converter::SyncDeltaIndexPriceToCpp(SyncDeltaIndexPrice x) {
    CThostFtdcSyncDeltaIndexPriceField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.ClosePrice = x.close_price;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaIndexPrice Converter::CThostFtdcSyncDeltaIndexPriceFieldToRust(CThostFtdcSyncDeltaIndexPriceField* x) {
    if (x == nullptr)
        return SyncDeltaIndexPrice{.is_null = true};
    SyncDeltaIndexPrice y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.close_price = x->ClosePrice;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaEWarrantOffsetField Converter::SyncDeltaEWarrantOffsetToCpp(SyncDeltaEWarrantOffset x) {
    CThostFtdcSyncDeltaEWarrantOffsetField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.Direction = x.direction;
    y.HedgeFlag = x.hedge_flag;
    y.Volume = x.volume;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaEWarrantOffset Converter::CThostFtdcSyncDeltaEWarrantOffsetFieldToRust(CThostFtdcSyncDeltaEWarrantOffsetField* x) {
    if (x == nullptr)
        return SyncDeltaEWarrantOffset{.is_null = true};
    SyncDeltaEWarrantOffset y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.direction = x->Direction;
    y.hedge_flag = x->HedgeFlag;
    y.volume = x->Volume;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSPBMFutureParameterField Converter::SPBMFutureParameterToCpp(SPBMFutureParameter x) {
    CThostFtdcSPBMFutureParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    y.Cvf = x.cvf;
    y.TimeRange = x.time_range;
    y.MarginRate = x.margin_rate;
    y.LockRateX = x.lock_rate_x;
    y.AddOnRate = x.add_on_rate;
    y.PreSettlementPrice = x.pre_settlement_price;
    y.AddOnLockRateX2 = x.add_on_lock_rate_x2;
    return y;
}

SPBMFutureParameter Converter::CThostFtdcSPBMFutureParameterFieldToRust(CThostFtdcSPBMFutureParameterField* x) {
    if (x == nullptr)
        return SPBMFutureParameter{.is_null = true};
    SPBMFutureParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    y.cvf = x->Cvf;
    y.time_range = x->TimeRange;
    y.margin_rate = x->MarginRate;
    y.lock_rate_x = x->LockRateX;
    y.add_on_rate = x->AddOnRate;
    y.pre_settlement_price = x->PreSettlementPrice;
    y.add_on_lock_rate_x2 = x->AddOnLockRateX2;
    return y;
}

CThostFtdcSPBMOptionParameterField Converter::SPBMOptionParameterToCpp(SPBMOptionParameter x) {
    CThostFtdcSPBMOptionParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    y.Cvf = x.cvf;
    y.DownPrice = x.down_price;
    y.Delta = x.delta;
    y.SlimiDelta = x.slim_delta;
    y.PreSettlementPrice = x.pre_settlement_price;
    return y;
}

SPBMOptionParameter Converter::CThostFtdcSPBMOptionParameterFieldToRust(CThostFtdcSPBMOptionParameterField* x) {
    if (x == nullptr)
        return SPBMOptionParameter{.is_null = true};
    SPBMOptionParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    y.cvf = x->Cvf;
    y.down_price = x->DownPrice;
    y.delta = x->Delta;
    y.slim_delta = x->SlimiDelta;
    y.pre_settlement_price = x->PreSettlementPrice;
    return y;
}

CThostFtdcSPBMIntraParameterField Converter::SPBMIntraParameterToCpp(SPBMIntraParameter x) {
    CThostFtdcSPBMIntraParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    y.IntraRateY = x.intra_rate_y;
    y.AddOnIntraRateY2 = x.add_on_intra_rate_y2;
    return y;
}

SPBMIntraParameter Converter::CThostFtdcSPBMIntraParameterFieldToRust(CThostFtdcSPBMIntraParameterField* x) {
    if (x == nullptr)
        return SPBMIntraParameter{.is_null = true};
    SPBMIntraParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    y.intra_rate_y = x->IntraRateY;
    y.add_on_intra_rate_y2 = x->AddOnIntraRateY2;
    return y;
}

CThostFtdcSPBMInterParameterField Converter::SPBMInterParameterToCpp(SPBMInterParameter x) {
    CThostFtdcSPBMInterParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.SpreadId = x.spread_id;
    y.InterRateZ = x.inter_rate_z;
    strcpy(y.Leg1ProdFamilyCode, x.leg1_prod_family_code.c_str());
    strcpy(y.Leg2ProdFamilyCode, x.leg2_prod_family_code.c_str());
    return y;
}

SPBMInterParameter Converter::CThostFtdcSPBMInterParameterFieldToRust(CThostFtdcSPBMInterParameterField* x) {
    if (x == nullptr)
        return SPBMInterParameter{.is_null = true};
    SPBMInterParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.spread_id = x->SpreadId;
    y.inter_rate_z = x->InterRateZ;
    y.leg1_prod_family_code = Converter::Gb2312ToRustString(x->Leg1ProdFamilyCode);
    y.leg2_prod_family_code = Converter::Gb2312ToRustString(x->Leg2ProdFamilyCode);
    return y;
}

CThostFtdcSyncSPBMParameterEndField Converter::SyncSPBMParameterEndToCpp(SyncSPBMParameterEnd x) {
    CThostFtdcSyncSPBMParameterEndField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    return y;
}

SyncSPBMParameterEnd Converter::CThostFtdcSyncSPBMParameterEndFieldToRust(CThostFtdcSyncSPBMParameterEndField* x) {
    if (x == nullptr)
        return SyncSPBMParameterEnd{.is_null = true};
    SyncSPBMParameterEnd y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    return y;
}

CThostFtdcQrySPBMFutureParameterField Converter::QrySPBMFutureParameterToCpp(QrySPBMFutureParameter x) {
    CThostFtdcQrySPBMFutureParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    return y;
}

QrySPBMFutureParameter Converter::CThostFtdcQrySPBMFutureParameterFieldToRust(CThostFtdcQrySPBMFutureParameterField* x) {
    if (x == nullptr)
        return QrySPBMFutureParameter{.is_null = true};
    QrySPBMFutureParameter y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    return y;
}

CThostFtdcQrySPBMOptionParameterField Converter::QrySPBMOptionParameterToCpp(QrySPBMOptionParameter x) {
    CThostFtdcQrySPBMOptionParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    return y;
}

QrySPBMOptionParameter Converter::CThostFtdcQrySPBMOptionParameterFieldToRust(CThostFtdcQrySPBMOptionParameterField* x) {
    if (x == nullptr)
        return QrySPBMOptionParameter{.is_null = true};
    QrySPBMOptionParameter y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    return y;
}

CThostFtdcQrySPBMIntraParameterField Converter::QrySPBMIntraParameterToCpp(QrySPBMIntraParameter x) {
    CThostFtdcQrySPBMIntraParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    return y;
}

QrySPBMIntraParameter Converter::CThostFtdcQrySPBMIntraParameterFieldToRust(CThostFtdcQrySPBMIntraParameterField* x) {
    if (x == nullptr)
        return QrySPBMIntraParameter{.is_null = true};
    QrySPBMIntraParameter y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    return y;
}

CThostFtdcQrySPBMInterParameterField Converter::QrySPBMInterParameterToCpp(QrySPBMInterParameter x) {
    CThostFtdcQrySPBMInterParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.Leg1ProdFamilyCode, x.leg1_prod_family_code.c_str());
    strcpy(y.Leg2ProdFamilyCode, x.leg2_prod_family_code.c_str());
    return y;
}

QrySPBMInterParameter Converter::CThostFtdcQrySPBMInterParameterFieldToRust(CThostFtdcQrySPBMInterParameterField* x) {
    if (x == nullptr)
        return QrySPBMInterParameter{.is_null = true};
    QrySPBMInterParameter y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.leg1_prod_family_code = Converter::Gb2312ToRustString(x->Leg1ProdFamilyCode);
    y.leg2_prod_family_code = Converter::Gb2312ToRustString(x->Leg2ProdFamilyCode);
    return y;
}

CThostFtdcSPBMPortfDefinitionField Converter::SPBMPortfDefinitionToCpp(SPBMPortfDefinition x) {
    CThostFtdcSPBMPortfDefinitionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.PortfolioDefID = x.portfolio_def_id;
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    y.IsSPBM = x.is_spbm;
    return y;
}

SPBMPortfDefinition Converter::CThostFtdcSPBMPortfDefinitionFieldToRust(CThostFtdcSPBMPortfDefinitionField* x) {
    if (x == nullptr)
        return SPBMPortfDefinition{.is_null = true};
    SPBMPortfDefinition y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.portfolio_def_id = x->PortfolioDefID;
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    y.is_spbm = x->IsSPBM;
    return y;
}

CThostFtdcSPBMInvestorPortfDefField Converter::SPBMInvestorPortfDefToCpp(SPBMInvestorPortfDef x) {
    CThostFtdcSPBMInvestorPortfDefField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.PortfolioDefID = x.portfolio_def_id;
    return y;
}

SPBMInvestorPortfDef Converter::CThostFtdcSPBMInvestorPortfDefFieldToRust(CThostFtdcSPBMInvestorPortfDefField* x) {
    if (x == nullptr)
        return SPBMInvestorPortfDef{.is_null = true};
    SPBMInvestorPortfDef y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.portfolio_def_id = x->PortfolioDefID;
    return y;
}

CThostFtdcInvestorPortfMarginRatioField Converter::InvestorPortfMarginRatioToCpp(InvestorPortfMarginRatio x) {
    CThostFtdcInvestorPortfMarginRatioField y;
    memset(&y, 0, sizeof(y));
    y.InvestorRange = x.investor_range;
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.MarginRatio = x.margin_ratio;
    strcpy(y.ProductGroupID, x.product_group_id.c_str());
    return y;
}

InvestorPortfMarginRatio Converter::CThostFtdcInvestorPortfMarginRatioFieldToRust(CThostFtdcInvestorPortfMarginRatioField* x) {
    if (x == nullptr)
        return InvestorPortfMarginRatio{.is_null = true};
    InvestorPortfMarginRatio y{};
    y.investor_range = x->InvestorRange;
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.margin_ratio = x->MarginRatio;
    y.product_group_id = Converter::Gb2312ToRustString(x->ProductGroupID);
    return y;
}

CThostFtdcQrySPBMPortfDefinitionField Converter::QrySPBMPortfDefinitionToCpp(QrySPBMPortfDefinition x) {
    CThostFtdcQrySPBMPortfDefinitionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.PortfolioDefID = x.portfolio_def_id;
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    return y;
}

QrySPBMPortfDefinition Converter::CThostFtdcQrySPBMPortfDefinitionFieldToRust(CThostFtdcQrySPBMPortfDefinitionField* x) {
    if (x == nullptr)
        return QrySPBMPortfDefinition{.is_null = true};
    QrySPBMPortfDefinition y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.portfolio_def_id = x->PortfolioDefID;
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    return y;
}

CThostFtdcQrySPBMInvestorPortfDefField Converter::QrySPBMInvestorPortfDefToCpp(QrySPBMInvestorPortfDef x) {
    CThostFtdcQrySPBMInvestorPortfDefField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    return y;
}

QrySPBMInvestorPortfDef Converter::CThostFtdcQrySPBMInvestorPortfDefFieldToRust(CThostFtdcQrySPBMInvestorPortfDefField* x) {
    if (x == nullptr)
        return QrySPBMInvestorPortfDef{.is_null = true};
    QrySPBMInvestorPortfDef y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    return y;
}

CThostFtdcQryInvestorPortfMarginRatioField Converter::QryInvestorPortfMarginRatioToCpp(QryInvestorPortfMarginRatio x) {
    CThostFtdcQryInvestorPortfMarginRatioField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductGroupID, x.product_group_id.c_str());
    return y;
}

QryInvestorPortfMarginRatio Converter::CThostFtdcQryInvestorPortfMarginRatioFieldToRust(CThostFtdcQryInvestorPortfMarginRatioField* x) {
    if (x == nullptr)
        return QryInvestorPortfMarginRatio{.is_null = true};
    QryInvestorPortfMarginRatio y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_group_id = Converter::Gb2312ToRustString(x->ProductGroupID);
    return y;
}

CThostFtdcInvestorProdSPBMDetailField Converter::InvestorProdSPBMDetailToCpp(InvestorProdSPBMDetail x) {
    CThostFtdcInvestorProdSPBMDetailField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    y.IntraInstrMargin = x.intra_instr_margin;
    y.BCollectingMargin = x.b_collecting_margin;
    y.SCollectingMargin = x.s_collecting_margin;
    y.IntraProdMargin = x.intra_prod_margin;
    y.NetMargin = x.net_margin;
    y.InterProdMargin = x.inter_prod_margin;
    y.SingleMargin = x.single_margin;
    y.AddOnMargin = x.add_on_margin;
    y.DeliveryMargin = x.delivery_margin;
    y.CallOptionMinRisk = x.call_option_min_risk;
    y.PutOptionMinRisk = x.put_option_min_risk;
    y.OptionMinRisk = x.option_min_risk;
    y.OptionValueOffset = x.option_value_offset;
    y.OptionRoyalty = x.option_royalty;
    y.RealOptionValueOffset = x.real_option_value_offset;
    y.Margin = x.margin;
    y.ExchMargin = x.exch_margin;
    return y;
}

InvestorProdSPBMDetail Converter::CThostFtdcInvestorProdSPBMDetailFieldToRust(CThostFtdcInvestorProdSPBMDetailField* x) {
    if (x == nullptr)
        return InvestorProdSPBMDetail{.is_null = true};
    InvestorProdSPBMDetail y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    y.intra_instr_margin = x->IntraInstrMargin;
    y.b_collecting_margin = x->BCollectingMargin;
    y.s_collecting_margin = x->SCollectingMargin;
    y.intra_prod_margin = x->IntraProdMargin;
    y.net_margin = x->NetMargin;
    y.inter_prod_margin = x->InterProdMargin;
    y.single_margin = x->SingleMargin;
    y.add_on_margin = x->AddOnMargin;
    y.delivery_margin = x->DeliveryMargin;
    y.call_option_min_risk = x->CallOptionMinRisk;
    y.put_option_min_risk = x->PutOptionMinRisk;
    y.option_min_risk = x->OptionMinRisk;
    y.option_value_offset = x->OptionValueOffset;
    y.option_royalty = x->OptionRoyalty;
    y.real_option_value_offset = x->RealOptionValueOffset;
    y.margin = x->Margin;
    y.exch_margin = x->ExchMargin;
    return y;
}

CThostFtdcQryInvestorProdSPBMDetailField Converter::QryInvestorProdSPBMDetailToCpp(QryInvestorProdSPBMDetail x) {
    CThostFtdcQryInvestorProdSPBMDetailField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    return y;
}

QryInvestorProdSPBMDetail Converter::CThostFtdcQryInvestorProdSPBMDetailFieldToRust(CThostFtdcQryInvestorProdSPBMDetailField* x) {
    if (x == nullptr)
        return QryInvestorProdSPBMDetail{.is_null = true};
    QryInvestorProdSPBMDetail y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    return y;
}

CThostFtdcPortfTradeParamSettingField Converter::PortfTradeParamSettingToCpp(PortfTradeParamSetting x) {
    CThostFtdcPortfTradeParamSettingField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.Portfolio = x.portfolio;
    y.IsActionVerify = x.is_action_verify;
    y.IsCloseVerify = x.is_close_verify;
    return y;
}

PortfTradeParamSetting Converter::CThostFtdcPortfTradeParamSettingFieldToRust(CThostFtdcPortfTradeParamSettingField* x) {
    if (x == nullptr)
        return PortfTradeParamSetting{.is_null = true};
    PortfTradeParamSetting y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.portfolio = x->Portfolio;
    y.is_action_verify = x->IsActionVerify;
    y.is_close_verify = x->IsCloseVerify;
    return y;
}

CThostFtdcInvestorTradingRightField Converter::InvestorTradingRightToCpp(InvestorTradingRight x) {
    CThostFtdcInvestorTradingRightField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.InvstTradingRight = x.invest_trading_right;
    return y;
}

InvestorTradingRight Converter::CThostFtdcInvestorTradingRightFieldToRust(CThostFtdcInvestorTradingRightField* x) {
    if (x == nullptr)
        return InvestorTradingRight{.is_null = true};
    InvestorTradingRight y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.invest_trading_right = x->InvstTradingRight;
    return y;
}

CThostFtdcMortgageParamField Converter::MortgageParamToCpp(MortgageParam x) {
    CThostFtdcMortgageParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    y.MortgageBalance = x.mortgage_balance;
    y.CheckMortgageRatio = x.check_mortgage_ratio;
    return y;
}

MortgageParam Converter::CThostFtdcMortgageParamFieldToRust(CThostFtdcMortgageParamField* x) {
    if (x == nullptr)
        return MortgageParam{.is_null = true};
    MortgageParam y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.mortgage_balance = x->MortgageBalance;
    y.check_mortgage_ratio = x->CheckMortgageRatio;
    return y;
}

CThostFtdcWithDrawParamField Converter::WithDrawParamToCpp(WithDrawParam x) {
    CThostFtdcWithDrawParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    y.WithDrawParamID = x.with_draw_param_id;
    memcpy(y.WithDrawParamValue, x.with_draw_param_value.data(), x.with_draw_param_value.size() * sizeof(uint8_t));
    return y;
}

WithDrawParam Converter::CThostFtdcWithDrawParamFieldToRust(CThostFtdcWithDrawParamField* x) {
    if (x == nullptr)
        return WithDrawParam{.is_null = true};
    WithDrawParam y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.with_draw_param_id = x->WithDrawParamID;
    for (int i = 0; i < 41; i++)
        y.with_draw_param_value.push_back(x->WithDrawParamValue[i]);
    return y;
}

CThostFtdcThostUserFunctionField Converter::ThostUserFunctionToCpp(ThostUserFunction x) {
    CThostFtdcThostUserFunctionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.ThostFunctionCode = x.thost_function_code;
    return y;
}

ThostUserFunction Converter::CThostFtdcThostUserFunctionFieldToRust(CThostFtdcThostUserFunctionField* x) {
    if (x == nullptr)
        return ThostUserFunction{.is_null = true};
    ThostUserFunction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.thost_function_code = x->ThostFunctionCode;
    return y;
}

CThostFtdcQryThostUserFunctionField Converter::QryThostUserFunctionToCpp(QryThostUserFunction x) {
    CThostFtdcQryThostUserFunctionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    return y;
}

QryThostUserFunction Converter::CThostFtdcQryThostUserFunctionFieldToRust(CThostFtdcQryThostUserFunctionField* x) {
    if (x == nullptr)
        return QryThostUserFunction{.is_null = true};
    QryThostUserFunction y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    return y;
}

CThostFtdcSPBMAddOnInterParameterField Converter::SPBMAddOnInterParameterToCpp(SPBMAddOnInterParameter x) {
    CThostFtdcSPBMAddOnInterParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.SpreadId = x.spread_id;
    y.AddOnInterRateZ2 = x.add_on_inter_rate_z2;
    strcpy(y.Leg1ProdFamilyCode, x.leg1_prod_family_code.c_str());
    strcpy(y.Leg2ProdFamilyCode, x.leg2_prod_family_code.c_str());
    return y;
}

SPBMAddOnInterParameter Converter::CThostFtdcSPBMAddOnInterParameterFieldToRust(CThostFtdcSPBMAddOnInterParameterField* x) {
    if (x == nullptr)
        return SPBMAddOnInterParameter{.is_null = true};
    SPBMAddOnInterParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.spread_id = x->SpreadId;
    y.add_on_inter_rate_z2 = x->AddOnInterRateZ2;
    y.leg1_prod_family_code = Converter::Gb2312ToRustString(x->Leg1ProdFamilyCode);
    y.leg2_prod_family_code = Converter::Gb2312ToRustString(x->Leg2ProdFamilyCode);
    return y;
}

CThostFtdcQrySPBMAddOnInterParameterField Converter::QrySPBMAddOnInterParameterToCpp(QrySPBMAddOnInterParameter x) {
    CThostFtdcQrySPBMAddOnInterParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.Leg1ProdFamilyCode, x.leg1_prod_family_code.c_str());
    strcpy(y.Leg2ProdFamilyCode, x.leg2_prod_family_code.c_str());
    return y;
}

QrySPBMAddOnInterParameter Converter::CThostFtdcQrySPBMAddOnInterParameterFieldToRust(CThostFtdcQrySPBMAddOnInterParameterField* x) {
    if (x == nullptr)
        return QrySPBMAddOnInterParameter{.is_null = true};
    QrySPBMAddOnInterParameter y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.leg1_prod_family_code = Converter::Gb2312ToRustString(x->Leg1ProdFamilyCode);
    y.leg2_prod_family_code = Converter::Gb2312ToRustString(x->Leg2ProdFamilyCode);
    return y;
}

CThostFtdcQryInvestorCommoditySPMMMarginField Converter::QryInvestorCommoditySPMMMarginToCpp(QryInvestorCommoditySPMMMargin x) {
    CThostFtdcQryInvestorCommoditySPMMMarginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.CommodityID, x.commodity_id.c_str());
    return y;
}

QryInvestorCommoditySPMMMargin Converter::CThostFtdcQryInvestorCommoditySPMMMarginFieldToRust(CThostFtdcQryInvestorCommoditySPMMMarginField* x) {
    if (x == nullptr)
        return QryInvestorCommoditySPMMMargin{.is_null = true};
    QryInvestorCommoditySPMMMargin y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.commodity_id = Converter::Gb2312ToRustString(x->CommodityID);
    return y;
}

CThostFtdcQryInvestorCommodityGroupSPMMMarginField Converter::QryInvestorCommodityGroupSPMMMarginToCpp(QryInvestorCommodityGroupSPMMMargin x) {
    CThostFtdcQryInvestorCommodityGroupSPMMMarginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.CommodityGroupID, x.commodity_group_id.c_str());
    return y;
}

QryInvestorCommodityGroupSPMMMargin Converter::CThostFtdcQryInvestorCommodityGroupSPMMMarginFieldToRust(CThostFtdcQryInvestorCommodityGroupSPMMMarginField* x) {
    if (x == nullptr)
        return QryInvestorCommodityGroupSPMMMargin{.is_null = true};
    QryInvestorCommodityGroupSPMMMargin y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.commodity_group_id = Converter::Gb2312ToRustString(x->CommodityGroupID);
    return y;
}

CThostFtdcQrySPMMInstParamField Converter::QrySPMMInstParamToCpp(QrySPMMInstParam x) {
    CThostFtdcQrySPMMInstParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QrySPMMInstParam Converter::CThostFtdcQrySPMMInstParamFieldToRust(CThostFtdcQrySPMMInstParamField* x) {
    if (x == nullptr)
        return QrySPMMInstParam{.is_null = true};
    QrySPMMInstParam y{};
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQrySPMMProductParamField Converter::QrySPMMProductParamToCpp(QrySPMMProductParam x) {
    CThostFtdcQrySPMMProductParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ProductID, x.product_id.c_str());
    return y;
}

QrySPMMProductParam Converter::CThostFtdcQrySPMMProductParamFieldToRust(CThostFtdcQrySPMMProductParamField* x) {
    if (x == nullptr)
        return QrySPMMProductParam{.is_null = true};
    QrySPMMProductParam y{};
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    return y;
}

CThostFtdcInvestorCommoditySPMMMarginField Converter::InvestorCommoditySPMMMarginToCpp(InvestorCommoditySPMMMargin x) {
    CThostFtdcInvestorCommoditySPMMMarginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.CommodityID, x.commodity_id.c_str());
    y.MarginBeforeDiscount = x.margin_before_discount;
    y.MarginNoDiscount = x.margin_no_discount;
    y.LongPosRisk = x.long_pos_risk;
    y.LongOpenFrozenRisk = x.long_open_frozen_risk;
    y.LongCloseFrozenRisk = x.long_close_frozen_risk;
    y.ShortPosRisk = x.short_pos_risk;
    y.ShortOpenFrozenRisk = x.short_open_frozen_risk;
    y.ShortCloseFrozenRisk = x.short_close_frozen_risk;
    y.IntraCommodityRate = x.intra_commodity_rate;
    y.OptionDiscountRate = x.option_discount_rate;
    y.PosDiscount = x.pos_discount;
    y.OpenFrozenDiscount = x.open_frozen_discount;
    y.NetRisk = x.net_risk;
    y.CloseFrozenMargin = x.close_frozen_margin;
    y.FrozenCommission = x.frozen_commission;
    y.Commission = x.commission;
    y.FrozenCash = x.frozen_cash;
    y.CashIn = x.cash_in;
    y.StrikeFrozenMargin = x.strike_frozen_margin;
    return y;
}

InvestorCommoditySPMMMargin Converter::CThostFtdcInvestorCommoditySPMMMarginFieldToRust(CThostFtdcInvestorCommoditySPMMMarginField* x) {
    if (x == nullptr)
        return InvestorCommoditySPMMMargin{.is_null = true};
    InvestorCommoditySPMMMargin y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.commodity_id = Converter::Gb2312ToRustString(x->CommodityID);
    y.margin_before_discount = x->MarginBeforeDiscount;
    y.margin_no_discount = x->MarginNoDiscount;
    y.long_pos_risk = x->LongPosRisk;
    y.long_open_frozen_risk = x->LongOpenFrozenRisk;
    y.long_close_frozen_risk = x->LongCloseFrozenRisk;
    y.short_pos_risk = x->ShortPosRisk;
    y.short_open_frozen_risk = x->ShortOpenFrozenRisk;
    y.short_close_frozen_risk = x->ShortCloseFrozenRisk;
    y.intra_commodity_rate = x->IntraCommodityRate;
    y.option_discount_rate = x->OptionDiscountRate;
    y.pos_discount = x->PosDiscount;
    y.open_frozen_discount = x->OpenFrozenDiscount;
    y.net_risk = x->NetRisk;
    y.close_frozen_margin = x->CloseFrozenMargin;
    y.frozen_commission = x->FrozenCommission;
    y.commission = x->Commission;
    y.frozen_cash = x->FrozenCash;
    y.cash_in = x->CashIn;
    y.strike_frozen_margin = x->StrikeFrozenMargin;
    return y;
}

CThostFtdcInvestorCommodityGroupSPMMMarginField Converter::InvestorCommodityGroupSPMMMarginToCpp(InvestorCommodityGroupSPMMMargin x) {
    CThostFtdcInvestorCommodityGroupSPMMMarginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.CommodityGroupID, x.commodity_group_id.c_str());
    y.MarginBeforeDiscount = x.margin_before_discount;
    y.MarginNoDiscount = x.margin_no_discount;
    y.LongRisk = x.long_risk;
    y.ShortRisk = x.short_risk;
    y.CloseFrozenMargin = x.close_frozen_margin;
    y.InterCommodityRate = x.inter_commodity_rate;
    y.MiniMarginRatio = x.mini_margin_ratio;
    y.AdjustRatio = x.adjust_ratio;
    y.IntraCommodityDiscount = x.intra_commodity_discount;
    y.InterCommodityDiscount = x.inter_commodity_discount;
    y.ExchMargin = x.exch_margin;
    y.InvestorMargin = x.investor_margin;
    y.FrozenCommission = x.frozen_commission;
    y.Commission = x.commission;
    y.FrozenCash = x.frozen_cash;
    y.CashIn = x.cash_in;
    y.StrikeFrozenMargin = x.strike_frozen_margin;
    return y;
}

InvestorCommodityGroupSPMMMargin Converter::CThostFtdcInvestorCommodityGroupSPMMMarginFieldToRust(CThostFtdcInvestorCommodityGroupSPMMMarginField* x) {
    if (x == nullptr)
        return InvestorCommodityGroupSPMMMargin{.is_null = true};
    InvestorCommodityGroupSPMMMargin y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.commodity_group_id = Converter::Gb2312ToRustString(x->CommodityGroupID);
    y.margin_before_discount = x->MarginBeforeDiscount;
    y.margin_no_discount = x->MarginNoDiscount;
    y.long_risk = x->LongRisk;
    y.short_risk = x->ShortRisk;
    y.close_frozen_margin = x->CloseFrozenMargin;
    y.inter_commodity_rate = x->InterCommodityRate;
    y.mini_margin_ratio = x->MiniMarginRatio;
    y.adjust_ratio = x->AdjustRatio;
    y.intra_commodity_discount = x->IntraCommodityDiscount;
    y.inter_commodity_discount = x->InterCommodityDiscount;
    y.exch_margin = x->ExchMargin;
    y.investor_margin = x->InvestorMargin;
    y.frozen_commission = x->FrozenCommission;
    y.commission = x->Commission;
    y.frozen_cash = x->FrozenCash;
    y.cash_in = x->CashIn;
    y.strike_frozen_margin = x->StrikeFrozenMargin;
    return y;
}

CThostFtdcSPMMInstParamField Converter::SPMMInstParamToCpp(SPMMInstParam x) {
    CThostFtdcSPMMInstParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.InstMarginCalID = x.inst_margin_cal_id;
    strcpy(y.CommodityID, x.commodity_id.c_str());
    strcpy(y.CommodityGroupID, x.commodity_group_id.c_str());
    return y;
}

SPMMInstParam Converter::CThostFtdcSPMMInstParamFieldToRust(CThostFtdcSPMMInstParamField* x) {
    if (x == nullptr)
        return SPMMInstParam{.is_null = true};
    SPMMInstParam y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.inst_margin_cal_id = x->InstMarginCalID;
    y.commodity_id = Converter::Gb2312ToRustString(x->CommodityID);
    y.commodity_group_id = Converter::Gb2312ToRustString(x->CommodityGroupID);
    return y;
}

CThostFtdcSPMMProductParamField Converter::SPMMProductParamToCpp(SPMMProductParam x) {
    CThostFtdcSPMMProductParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    strcpy(y.CommodityID, x.commodity_id.c_str());
    strcpy(y.CommodityGroupID, x.commodity_group_id.c_str());
    return y;
}

SPMMProductParam Converter::CThostFtdcSPMMProductParamFieldToRust(CThostFtdcSPMMProductParamField* x) {
    if (x == nullptr)
        return SPMMProductParam{.is_null = true};
    SPMMProductParam y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.commodity_id = Converter::Gb2312ToRustString(x->CommodityID);
    y.commodity_group_id = Converter::Gb2312ToRustString(x->CommodityGroupID);
    return y;
}

CThostFtdcQryTraderAssignField Converter::QryTraderAssignToCpp(QryTraderAssign x) {
    CThostFtdcQryTraderAssignField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TraderID, x.trader_id.c_str());
    return y;
}

QryTraderAssign Converter::CThostFtdcQryTraderAssignFieldToRust(CThostFtdcQryTraderAssignField* x) {
    if (x == nullptr)
        return QryTraderAssign{.is_null = true};
    QryTraderAssign y{};
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    return y;
}

CThostFtdcTraderAssignField Converter::TraderAssignToCpp(TraderAssign x) {
    CThostFtdcTraderAssignField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    y.DRIdentityID = x.dr_identity_id;
    return y;
}

TraderAssign Converter::CThostFtdcTraderAssignFieldToRust(CThostFtdcTraderAssignField* x) {
    if (x == nullptr)
        return TraderAssign{.is_null = true};
    TraderAssign y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.dr_identity_id = x->DRIdentityID;
    return y;
}

CThostFtdcInvestorInfoCntSettingField Converter::InvestorInfoCntSettingToCpp(InvestorInfoCntSetting x) {
    CThostFtdcInvestorInfoCntSettingField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    y.IsCalInfoComm = x.is_cal_info_comm;
    y.IsLimitInfoMax = x.is_limit_info_max;
    y.InfoMaxLimit = x.info_max_limit;
    return y;
}

InvestorInfoCntSetting Converter::CThostFtdcInvestorInfoCntSettingFieldToRust(CThostFtdcInvestorInfoCntSettingField* x) {
    if (x == nullptr)
        return InvestorInfoCntSetting{.is_null = true};
    InvestorInfoCntSetting y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.is_cal_info_comm = x->IsCalInfoComm;
    y.is_limit_info_max = x->IsLimitInfoMax;
    y.info_max_limit = x->InfoMaxLimit;
    return y;
}

CThostFtdcRCAMSCombProductInfoField Converter::RCAMSCombProductInfoToCpp(RCAMSCombProductInfo x) {
    CThostFtdcRCAMSCombProductInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    strcpy(y.CombProductID, x.comb_product_id.c_str());
    strcpy(y.ProductGroupID, x.product_group_id.c_str());
    return y;
}

RCAMSCombProductInfo Converter::CThostFtdcRCAMSCombProductInfoFieldToRust(CThostFtdcRCAMSCombProductInfoField* x) {
    if (x == nullptr)
        return RCAMSCombProductInfo{.is_null = true};
    RCAMSCombProductInfo y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.comb_product_id = Converter::Gb2312ToRustString(x->CombProductID);
    y.product_group_id = Converter::Gb2312ToRustString(x->ProductGroupID);
    return y;
}

CThostFtdcRCAMSInstrParameterField Converter::RCAMSInstrParameterToCpp(RCAMSInstrParameter x) {
    CThostFtdcRCAMSInstrParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    y.HedgeRate = x.hedge_rate;
    return y;
}

RCAMSInstrParameter Converter::CThostFtdcRCAMSInstrParameterFieldToRust(CThostFtdcRCAMSInstrParameterField* x) {
    if (x == nullptr)
        return RCAMSInstrParameter{.is_null = true};
    RCAMSInstrParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.hedge_rate = x->HedgeRate;
    return y;
}

CThostFtdcRCAMSIntraParameterField Converter::RCAMSIntraParameterToCpp(RCAMSIntraParameter x) {
    CThostFtdcRCAMSIntraParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.CombProductID, x.comb_product_id.c_str());
    y.HedgeRate = x.hedge_rate;
    return y;
}

RCAMSIntraParameter Converter::CThostFtdcRCAMSIntraParameterFieldToRust(CThostFtdcRCAMSIntraParameterField* x) {
    if (x == nullptr)
        return RCAMSIntraParameter{.is_null = true};
    RCAMSIntraParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.comb_product_id = Converter::Gb2312ToRustString(x->CombProductID);
    y.hedge_rate = x->HedgeRate;
    return y;
}

CThostFtdcRCAMSInterParameterField Converter::RCAMSInterParameterToCpp(RCAMSInterParameter x) {
    CThostFtdcRCAMSInterParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductGroupID, x.product_group_id.c_str());
    y.Priority = x.priority;
    y.CreditRate = x.credit_rate;
    strcpy(y.CombProduct1, x.comb_product1.c_str());
    strcpy(y.CombProduct2, x.comb_product2.c_str());
    return y;
}

RCAMSInterParameter Converter::CThostFtdcRCAMSInterParameterFieldToRust(CThostFtdcRCAMSInterParameterField* x) {
    if (x == nullptr)
        return RCAMSInterParameter{.is_null = true};
    RCAMSInterParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_group_id = Converter::Gb2312ToRustString(x->ProductGroupID);
    y.priority = x->Priority;
    y.credit_rate = x->CreditRate;
    y.comb_product1 = Converter::Gb2312ToRustString(x->CombProduct1);
    y.comb_product2 = Converter::Gb2312ToRustString(x->CombProduct2);
    return y;
}

CThostFtdcRCAMSShortOptAdjustParamField Converter::RCAMSShortOptAdjustParamToCpp(RCAMSShortOptAdjustParam x) {
    CThostFtdcRCAMSShortOptAdjustParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.CombProductID, x.comb_product_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.AdjustValue = x.adjust_value;
    return y;
}

RCAMSShortOptAdjustParam Converter::CThostFtdcRCAMSShortOptAdjustParamFieldToRust(CThostFtdcRCAMSShortOptAdjustParamField* x) {
    if (x == nullptr)
        return RCAMSShortOptAdjustParam{.is_null = true};
    RCAMSShortOptAdjustParam y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.comb_product_id = Converter::Gb2312ToRustString(x->CombProductID);
    y.hedge_flag = x->HedgeFlag;
    y.adjust_value = x->AdjustValue;
    return y;
}

CThostFtdcRCAMSInvestorCombPositionField Converter::RCAMSInvestorCombPositionToCpp(RCAMSInvestorCombPosition x) {
    CThostFtdcRCAMSInvestorCombPositionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.PosiDirection = x.posi_direction;
    strcpy(y.CombInstrumentID, x.comb_instrument_id.c_str());
    y.LegID = x.leg_id;
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    y.TotalAmt = x.total_amt;
    y.ExchMargin = x.exch_margin;
    y.Margin = x.margin;
    return y;
}

RCAMSInvestorCombPosition Converter::CThostFtdcRCAMSInvestorCombPositionFieldToRust(CThostFtdcRCAMSInvestorCombPositionField* x) {
    if (x == nullptr)
        return RCAMSInvestorCombPosition{.is_null = true};
    RCAMSInvestorCombPosition y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.hedge_flag = x->HedgeFlag;
    y.posi_direction = x->PosiDirection;
    y.comb_instrument_id = Converter::Gb2312ToRustString(x->CombInstrumentID);
    y.leg_id = x->LegID;
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.total_amt = x->TotalAmt;
    y.exch_margin = x->ExchMargin;
    y.margin = x->Margin;
    return y;
}

CThostFtdcInvestorProdRCAMSMarginField Converter::InvestorProdRCAMSMarginToCpp(InvestorProdRCAMSMargin x) {
    CThostFtdcInvestorProdRCAMSMarginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.CombProductID, x.comb_product_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    strcpy(y.ProductGroupID, x.product_group_id.c_str());
    y.RiskBeforeDiscount = x.risk_before_discount;
    y.IntraInstrRisk = x.intra_instr_risk;
    y.BPosRisk = x.b_pos_risk;
    y.SPosRisk = x.spos_risk;
    y.IntraProdRisk = x.intra_prod_risk;
    y.NetRisk = x.net_risk;
    y.InterProdRisk = x.inter_prod_risk;
    y.ShortOptRiskAdj = x.short_opt_risk_adj;
    y.OptionRoyalty = x.option_royalty;
    y.MMSACloseFrozenMargin = x.mmsa_close_frozen_margin;
    y.CloseCombFrozenMargin = x.close_comb_frozen_margin;
    y.CloseFrozenMargin = x.close_frozen_margin;
    y.MMSAOpenFrozenMargin = x.mmsa_open_frozen_margin;
    y.DeliveryOpenFrozenMargin = x.delivery_open_frozen_margin;
    y.OpenFrozenMargin = x.open_frozen_margin;
    y.UseFrozenMargin = x.use_frozen_margin;
    y.MMSAExchMargin = x.mmsa_exch_margin;
    y.DeliveryExchMargin = x.delivery_exch_margin;
    y.CombExchMargin = x.comb_exch_margin;
    y.ExchMargin = x.exch_margin;
    y.UseMargin = x.use_margin;
    return y;
}

InvestorProdRCAMSMargin Converter::CThostFtdcInvestorProdRCAMSMarginFieldToRust(CThostFtdcInvestorProdRCAMSMarginField* x) {
    if (x == nullptr)
        return InvestorProdRCAMSMargin{.is_null = true};
    InvestorProdRCAMSMargin y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.comb_product_id = Converter::Gb2312ToRustString(x->CombProductID);
    y.hedge_flag = x->HedgeFlag;
    y.product_group_id = Converter::Gb2312ToRustString(x->ProductGroupID);
    y.risk_before_discount = x->RiskBeforeDiscount;
    y.intra_instr_risk = x->IntraInstrRisk;
    y.b_pos_risk = x->BPosRisk;
    y.spos_risk = x->SPosRisk;
    y.intra_prod_risk = x->IntraProdRisk;
    y.net_risk = x->NetRisk;
    y.inter_prod_risk = x->InterProdRisk;
    y.short_opt_risk_adj = x->ShortOptRiskAdj;
    y.option_royalty = x->OptionRoyalty;
    y.mmsa_close_frozen_margin = x->MMSACloseFrozenMargin;
    y.close_comb_frozen_margin = x->CloseCombFrozenMargin;
    y.close_frozen_margin = x->CloseFrozenMargin;
    y.mmsa_open_frozen_margin = x->MMSAOpenFrozenMargin;
    y.delivery_open_frozen_margin = x->DeliveryOpenFrozenMargin;
    y.open_frozen_margin = x->OpenFrozenMargin;
    y.use_frozen_margin = x->UseFrozenMargin;
    y.mmsa_exch_margin = x->MMSAExchMargin;
    y.delivery_exch_margin = x->DeliveryExchMargin;
    y.comb_exch_margin = x->CombExchMargin;
    y.exch_margin = x->ExchMargin;
    y.use_margin = x->UseMargin;
    return y;
}

CThostFtdcQryRCAMSCombProductInfoField Converter::QryRCAMSCombProductInfoToCpp(QryRCAMSCombProductInfo x) {
    CThostFtdcQryRCAMSCombProductInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ProductID, x.product_id.c_str());
    strcpy(y.CombProductID, x.comb_product_id.c_str());
    strcpy(y.ProductGroupID, x.product_group_id.c_str());
    return y;
}

QryRCAMSCombProductInfo Converter::CThostFtdcQryRCAMSCombProductInfoFieldToRust(CThostFtdcQryRCAMSCombProductInfoField* x) {
    if (x == nullptr)
        return QryRCAMSCombProductInfo{.is_null = true};
    QryRCAMSCombProductInfo y{};
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.comb_product_id = Converter::Gb2312ToRustString(x->CombProductID);
    y.product_group_id = Converter::Gb2312ToRustString(x->ProductGroupID);
    return y;
}

CThostFtdcQryRCAMSInstrParameterField Converter::QryRCAMSInstrParameterToCpp(QryRCAMSInstrParameter x) {
    CThostFtdcQryRCAMSInstrParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ProductID, x.product_id.c_str());
    return y;
}

QryRCAMSInstrParameter Converter::CThostFtdcQryRCAMSInstrParameterFieldToRust(CThostFtdcQryRCAMSInstrParameterField* x) {
    if (x == nullptr)
        return QryRCAMSInstrParameter{.is_null = true};
    QryRCAMSInstrParameter y{};
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    return y;
}

CThostFtdcQryRCAMSIntraParameterField Converter::QryRCAMSIntraParameterToCpp(QryRCAMSIntraParameter x) {
    CThostFtdcQryRCAMSIntraParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.CombProductID, x.comb_product_id.c_str());
    return y;
}

QryRCAMSIntraParameter Converter::CThostFtdcQryRCAMSIntraParameterFieldToRust(CThostFtdcQryRCAMSIntraParameterField* x) {
    if (x == nullptr)
        return QryRCAMSIntraParameter{.is_null = true};
    QryRCAMSIntraParameter y{};
    y.comb_product_id = Converter::Gb2312ToRustString(x->CombProductID);
    return y;
}

CThostFtdcQryRCAMSInterParameterField Converter::QryRCAMSInterParameterToCpp(QryRCAMSInterParameter x) {
    CThostFtdcQryRCAMSInterParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ProductGroupID, x.product_group_id.c_str());
    strcpy(y.CombProduct1, x.comb_product1.c_str());
    strcpy(y.CombProduct2, x.comb_product2.c_str());
    return y;
}

QryRCAMSInterParameter Converter::CThostFtdcQryRCAMSInterParameterFieldToRust(CThostFtdcQryRCAMSInterParameterField* x) {
    if (x == nullptr)
        return QryRCAMSInterParameter{.is_null = true};
    QryRCAMSInterParameter y{};
    y.product_group_id = Converter::Gb2312ToRustString(x->ProductGroupID);
    y.comb_product1 = Converter::Gb2312ToRustString(x->CombProduct1);
    y.comb_product2 = Converter::Gb2312ToRustString(x->CombProduct2);
    return y;
}

CThostFtdcQryRCAMSShortOptAdjustParamField Converter::QryRCAMSShortOptAdjustParamToCpp(QryRCAMSShortOptAdjustParam x) {
    CThostFtdcQryRCAMSShortOptAdjustParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.CombProductID, x.comb_product_id.c_str());
    return y;
}

QryRCAMSShortOptAdjustParam Converter::CThostFtdcQryRCAMSShortOptAdjustParamFieldToRust(CThostFtdcQryRCAMSShortOptAdjustParamField* x) {
    if (x == nullptr)
        return QryRCAMSShortOptAdjustParam{.is_null = true};
    QryRCAMSShortOptAdjustParam y{};
    y.comb_product_id = Converter::Gb2312ToRustString(x->CombProductID);
    return y;
}

CThostFtdcQryRCAMSInvestorCombPositionField Converter::QryRCAMSInvestorCombPositionToCpp(QryRCAMSInvestorCombPosition x) {
    CThostFtdcQryRCAMSInvestorCombPositionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.CombInstrumentID, x.comb_instrument_id.c_str());
    return y;
}

QryRCAMSInvestorCombPosition Converter::CThostFtdcQryRCAMSInvestorCombPositionFieldToRust(CThostFtdcQryRCAMSInvestorCombPositionField* x) {
    if (x == nullptr)
        return QryRCAMSInvestorCombPosition{.is_null = true};
    QryRCAMSInvestorCombPosition y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.comb_instrument_id = Converter::Gb2312ToRustString(x->CombInstrumentID);
    return y;
}

CThostFtdcQryInvestorProdRCAMSMarginField Converter::QryInvestorProdRCAMSMarginToCpp(QryInvestorProdRCAMSMargin x) {
    CThostFtdcQryInvestorProdRCAMSMarginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.CombProductID, x.comb_product_id.c_str());
    strcpy(y.ProductGroupID, x.product_group_id.c_str());
    return y;
}

QryInvestorProdRCAMSMargin Converter::CThostFtdcQryInvestorProdRCAMSMarginFieldToRust(CThostFtdcQryInvestorProdRCAMSMarginField* x) {
    if (x == nullptr)
        return QryInvestorProdRCAMSMargin{.is_null = true};
    QryInvestorProdRCAMSMargin y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.comb_product_id = Converter::Gb2312ToRustString(x->CombProductID);
    y.product_group_id = Converter::Gb2312ToRustString(x->ProductGroupID);
    return y;
}

CThostFtdcRULEInstrParameterField Converter::RULEInstrParameterToCpp(RULEInstrParameter x) {
    CThostFtdcRULEInstrParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.InstrumentClass = x.instrument_class;
    strcpy(y.StdInstrumentID, x.std_instrument_id.c_str());
    y.BSpecRatio = x.b_spec_ratio;
    y.SSpecRatio = x.s_spec_ratio;
    y.BHedgeRatio = x.b_hedge_ratio;
    y.SHedgeRatio = x.s_hedge_ratio;
    y.BAddOnMargin = x.badd_on_margin;
    y.SAddOnMargin = x.s_add_on_margin;
    y.CommodityGroupID = x.commodity_group_id;
    return y;
}

RULEInstrParameter Converter::CThostFtdcRULEInstrParameterFieldToRust(CThostFtdcRULEInstrParameterField* x) {
    if (x == nullptr)
        return RULEInstrParameter{.is_null = true};
    RULEInstrParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.instrument_class = x->InstrumentClass;
    y.std_instrument_id = Converter::Gb2312ToRustString(x->StdInstrumentID);
    y.b_spec_ratio = x->BSpecRatio;
    y.s_spec_ratio = x->SSpecRatio;
    y.b_hedge_ratio = x->BHedgeRatio;
    y.s_hedge_ratio = x->SHedgeRatio;
    y.badd_on_margin = x->BAddOnMargin;
    y.s_add_on_margin = x->SAddOnMargin;
    y.commodity_group_id = x->CommodityGroupID;
    return y;
}

CThostFtdcRULEIntraParameterField Converter::RULEIntraParameterToCpp(RULEIntraParameter x) {
    CThostFtdcRULEIntraParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    strcpy(y.StdInstrumentID, x.std_instrument_id.c_str());
    y.StdInstrMargin = x.std_instr_margin;
    y.UsualIntraRate = x.usual_intra_rate;
    y.DeliveryIntraRate = x.delivery_intra_rate;
    return y;
}

RULEIntraParameter Converter::CThostFtdcRULEIntraParameterFieldToRust(CThostFtdcRULEIntraParameterField* x) {
    if (x == nullptr)
        return RULEIntraParameter{.is_null = true};
    RULEIntraParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    y.std_instrument_id = Converter::Gb2312ToRustString(x->StdInstrumentID);
    y.std_instr_margin = x->StdInstrMargin;
    y.usual_intra_rate = x->UsualIntraRate;
    y.delivery_intra_rate = x->DeliveryIntraRate;
    return y;
}

CThostFtdcRULEInterParameterField Converter::RULEInterParameterToCpp(RULEInterParameter x) {
    CThostFtdcRULEInterParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.SpreadId = x.spread_id;
    y.InterRate = x.inter_rate;
    strcpy(y.Leg1ProdFamilyCode, x.leg1_prod_family_code.c_str());
    strcpy(y.Leg2ProdFamilyCode, x.leg2_prod_family_code.c_str());
    y.Leg1PropFactor = x.leg1_prop_factor;
    y.Leg2PropFactor = x.leg2_prop_factor;
    y.CommodityGroupID = x.commodity_group_id;
    strcpy(y.CommodityGroupName, x.commodity_group_name.c_str());
    return y;
}

RULEInterParameter Converter::CThostFtdcRULEInterParameterFieldToRust(CThostFtdcRULEInterParameterField* x) {
    if (x == nullptr)
        return RULEInterParameter{.is_null = true};
    RULEInterParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.spread_id = x->SpreadId;
    y.inter_rate = x->InterRate;
    y.leg1_prod_family_code = Converter::Gb2312ToRustString(x->Leg1ProdFamilyCode);
    y.leg2_prod_family_code = Converter::Gb2312ToRustString(x->Leg2ProdFamilyCode);
    y.leg1_prop_factor = x->Leg1PropFactor;
    y.leg2_prop_factor = x->Leg2PropFactor;
    y.commodity_group_id = x->CommodityGroupID;
    y.commodity_group_name = Converter::Gb2312ToRustString(x->CommodityGroupName);
    return y;
}

CThostFtdcQryRULEInstrParameterField Converter::QryRULEInstrParameterToCpp(QryRULEInstrParameter x) {
    CThostFtdcQryRULEInstrParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    return y;
}

QryRULEInstrParameter Converter::CThostFtdcQryRULEInstrParameterFieldToRust(CThostFtdcQryRULEInstrParameterField* x) {
    if (x == nullptr)
        return QryRULEInstrParameter{.is_null = true};
    QryRULEInstrParameter y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    return y;
}

CThostFtdcQryRULEIntraParameterField Converter::QryRULEIntraParameterToCpp(QryRULEIntraParameter x) {
    CThostFtdcQryRULEIntraParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    return y;
}

QryRULEIntraParameter Converter::CThostFtdcQryRULEIntraParameterFieldToRust(CThostFtdcQryRULEIntraParameterField* x) {
    if (x == nullptr)
        return QryRULEIntraParameter{.is_null = true};
    QryRULEIntraParameter y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    return y;
}

CThostFtdcQryRULEInterParameterField Converter::QryRULEInterParameterToCpp(QryRULEInterParameter x) {
    CThostFtdcQryRULEInterParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.Leg1ProdFamilyCode, x.leg1_prod_family_code.c_str());
    strcpy(y.Leg2ProdFamilyCode, x.leg2_prod_family_code.c_str());
    y.CommodityGroupID = x.commodity_group_id;
    return y;
}

QryRULEInterParameter Converter::CThostFtdcQryRULEInterParameterFieldToRust(CThostFtdcQryRULEInterParameterField* x) {
    if (x == nullptr)
        return QryRULEInterParameter{.is_null = true};
    QryRULEInterParameter y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.leg1_prod_family_code = Converter::Gb2312ToRustString(x->Leg1ProdFamilyCode);
    y.leg2_prod_family_code = Converter::Gb2312ToRustString(x->Leg2ProdFamilyCode);
    y.commodity_group_id = x->CommodityGroupID;
    return y;
}

CThostFtdcInvestorProdRULEMarginField Converter::InvestorProdRULEMarginToCpp(InvestorProdRULEMargin x) {
    CThostFtdcInvestorProdRULEMarginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    y.InstrumentClass = x.instrument_class;
    y.CommodityGroupID = x.commodity_group_id;
    y.BStdPosition = x.b_std_position;
    y.SStdPosition = x.s_std_position;
    y.BStdOpenFrozen = x.b_std_open_frozen;
    y.SStdOpenFrozen = x.s_std_open_frozen;
    y.BStdCloseFrozen = x.b_std_close_frozen;
    y.SStdCloseFrozen = x.s_std_close_frozen;
    y.IntraProdStdPosition = x.intra_prod_std_position;
    y.NetStdPosition = x.net_std_position;
    y.InterProdStdPosition = x.inter_prod_std_position;
    y.SingleStdPosition = x.single_std_position;
    y.IntraProdMargin = x.intra_prod_margin;
    y.InterProdMargin = x.inter_prod_margin;
    y.SingleMargin = x.single_margin;
    y.NonCombMargin = x.non_comb_margin;
    y.AddOnMargin = x.add_on_margin;
    y.ExchMargin = x.exch_margin;
    y.AddOnFrozenMargin = x.add_on_frozen_margin;
    y.OpenFrozenMargin = x.open_frozen_margin;
    y.CloseFrozenMargin = x.close_frozen_margin;
    y.Margin = x.margin;
    y.FrozenMargin = x.frozen_margin;
    return y;
}

InvestorProdRULEMargin Converter::CThostFtdcInvestorProdRULEMarginFieldToRust(CThostFtdcInvestorProdRULEMarginField* x) {
    if (x == nullptr)
        return InvestorProdRULEMargin{.is_null = true};
    InvestorProdRULEMargin y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    y.instrument_class = x->InstrumentClass;
    y.commodity_group_id = x->CommodityGroupID;
    y.b_std_position = x->BStdPosition;
    y.s_std_position = x->SStdPosition;
    y.b_std_open_frozen = x->BStdOpenFrozen;
    y.s_std_open_frozen = x->SStdOpenFrozen;
    y.b_std_close_frozen = x->BStdCloseFrozen;
    y.s_std_close_frozen = x->SStdCloseFrozen;
    y.intra_prod_std_position = x->IntraProdStdPosition;
    y.net_std_position = x->NetStdPosition;
    y.inter_prod_std_position = x->InterProdStdPosition;
    y.single_std_position = x->SingleStdPosition;
    y.intra_prod_margin = x->IntraProdMargin;
    y.inter_prod_margin = x->InterProdMargin;
    y.single_margin = x->SingleMargin;
    y.non_comb_margin = x->NonCombMargin;
    y.add_on_margin = x->AddOnMargin;
    y.exch_margin = x->ExchMargin;
    y.add_on_frozen_margin = x->AddOnFrozenMargin;
    y.open_frozen_margin = x->OpenFrozenMargin;
    y.close_frozen_margin = x->CloseFrozenMargin;
    y.margin = x->Margin;
    y.frozen_margin = x->FrozenMargin;
    return y;
}

CThostFtdcQryInvestorProdRULEMarginField Converter::QryInvestorProdRULEMarginToCpp(QryInvestorProdRULEMargin x) {
    CThostFtdcQryInvestorProdRULEMarginField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    y.CommodityGroupID = x.commodity_group_id;
    return y;
}

QryInvestorProdRULEMargin Converter::CThostFtdcQryInvestorProdRULEMarginFieldToRust(CThostFtdcQryInvestorProdRULEMarginField* x) {
    if (x == nullptr)
        return QryInvestorProdRULEMargin{.is_null = true};
    QryInvestorProdRULEMargin y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    y.commodity_group_id = x->CommodityGroupID;
    return y;
}

CThostFtdcSyncDeltaSPBMPortfDefinitionField Converter::SyncDeltaSPBMPortfDefinitionToCpp(SyncDeltaSPBMPortfDefinition x) {
    CThostFtdcSyncDeltaSPBMPortfDefinitionField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.PortfolioDefID = x.portfolio_def_id;
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    y.IsSPBM = x.is_spbm;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaSPBMPortfDefinition Converter::CThostFtdcSyncDeltaSPBMPortfDefinitionFieldToRust(CThostFtdcSyncDeltaSPBMPortfDefinitionField* x) {
    if (x == nullptr)
        return SyncDeltaSPBMPortfDefinition{.is_null = true};
    SyncDeltaSPBMPortfDefinition y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.portfolio_def_id = x->PortfolioDefID;
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    y.is_spbm = x->IsSPBM;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaSPBMInvstPortfDefField Converter::SyncDeltaSPBMInvestPortfDefToCpp(SyncDeltaSPBMInvestPortfDef x) {
    CThostFtdcSyncDeltaSPBMInvstPortfDefField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.PortfolioDefID = x.portfolio_def_id;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaSPBMInvestPortfDef Converter::CThostFtdcSyncDeltaSPBMInvstPortfDefFieldToRust(CThostFtdcSyncDeltaSPBMInvstPortfDefField* x) {
    if (x == nullptr)
        return SyncDeltaSPBMInvestPortfDef{.is_null = true};
    SyncDeltaSPBMInvestPortfDef y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.portfolio_def_id = x->PortfolioDefID;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaSPBMFutureParameterField Converter::SyncDeltaSPBMFutureParameterToCpp(SyncDeltaSPBMFutureParameter x) {
    CThostFtdcSyncDeltaSPBMFutureParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    y.Cvf = x.cvf;
    y.TimeRange = x.time_range;
    y.MarginRate = x.margin_rate;
    y.LockRateX = x.lock_rate_x;
    y.AddOnRate = x.add_on_rate;
    y.PreSettlementPrice = x.pre_settlement_price;
    y.AddOnLockRateX2 = x.add_on_lock_rate_x2;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaSPBMFutureParameter Converter::CThostFtdcSyncDeltaSPBMFutureParameterFieldToRust(CThostFtdcSyncDeltaSPBMFutureParameterField* x) {
    if (x == nullptr)
        return SyncDeltaSPBMFutureParameter{.is_null = true};
    SyncDeltaSPBMFutureParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    y.cvf = x->Cvf;
    y.time_range = x->TimeRange;
    y.margin_rate = x->MarginRate;
    y.lock_rate_x = x->LockRateX;
    y.add_on_rate = x->AddOnRate;
    y.pre_settlement_price = x->PreSettlementPrice;
    y.add_on_lock_rate_x2 = x->AddOnLockRateX2;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaSPBMOptionParameterField Converter::SyncDeltaSPBMOptionParameterToCpp(SyncDeltaSPBMOptionParameter x) {
    CThostFtdcSyncDeltaSPBMOptionParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    y.Cvf = x.cvf;
    y.DownPrice = x.down_price;
    y.Delta = x.delta;
    y.SlimiDelta = x.slim_delta;
    y.PreSettlementPrice = x.pre_settlement_price;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaSPBMOptionParameter Converter::CThostFtdcSyncDeltaSPBMOptionParameterFieldToRust(CThostFtdcSyncDeltaSPBMOptionParameterField* x) {
    if (x == nullptr)
        return SyncDeltaSPBMOptionParameter{.is_null = true};
    SyncDeltaSPBMOptionParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    y.cvf = x->Cvf;
    y.down_price = x->DownPrice;
    y.delta = x->Delta;
    y.slim_delta = x->SlimiDelta;
    y.pre_settlement_price = x->PreSettlementPrice;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaSPBMIntraParameterField Converter::SyncDeltaSPBMIntraParameterToCpp(SyncDeltaSPBMIntraParameter x) {
    CThostFtdcSyncDeltaSPBMIntraParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    y.IntraRateY = x.intra_rate_y;
    y.AddOnIntraRateY2 = x.add_on_intra_rate_y2;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaSPBMIntraParameter Converter::CThostFtdcSyncDeltaSPBMIntraParameterFieldToRust(CThostFtdcSyncDeltaSPBMIntraParameterField* x) {
    if (x == nullptr)
        return SyncDeltaSPBMIntraParameter{.is_null = true};
    SyncDeltaSPBMIntraParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    y.intra_rate_y = x->IntraRateY;
    y.add_on_intra_rate_y2 = x->AddOnIntraRateY2;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaSPBMInterParameterField Converter::SyncDeltaSPBMInterParameterToCpp(SyncDeltaSPBMInterParameter x) {
    CThostFtdcSyncDeltaSPBMInterParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.SpreadId = x.spread_id;
    y.InterRateZ = x.inter_rate_z;
    strcpy(y.Leg1ProdFamilyCode, x.leg1_prod_family_code.c_str());
    strcpy(y.Leg2ProdFamilyCode, x.leg2_prod_family_code.c_str());
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaSPBMInterParameter Converter::CThostFtdcSyncDeltaSPBMInterParameterFieldToRust(CThostFtdcSyncDeltaSPBMInterParameterField* x) {
    if (x == nullptr)
        return SyncDeltaSPBMInterParameter{.is_null = true};
    SyncDeltaSPBMInterParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.spread_id = x->SpreadId;
    y.inter_rate_z = x->InterRateZ;
    y.leg1_prod_family_code = Converter::Gb2312ToRustString(x->Leg1ProdFamilyCode);
    y.leg2_prod_family_code = Converter::Gb2312ToRustString(x->Leg2ProdFamilyCode);
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaSPBMAddOnInterParamField Converter::SyncDeltaSPBMAddOnInterParamToCpp(SyncDeltaSPBMAddOnInterParam x) {
    CThostFtdcSyncDeltaSPBMAddOnInterParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.SpreadId = x.spread_id;
    y.AddOnInterRateZ2 = x.add_on_inter_rate_z2;
    strcpy(y.Leg1ProdFamilyCode, x.leg1_prod_family_code.c_str());
    strcpy(y.Leg2ProdFamilyCode, x.leg2_prod_family_code.c_str());
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaSPBMAddOnInterParam Converter::CThostFtdcSyncDeltaSPBMAddOnInterParamFieldToRust(CThostFtdcSyncDeltaSPBMAddOnInterParamField* x) {
    if (x == nullptr)
        return SyncDeltaSPBMAddOnInterParam{.is_null = true};
    SyncDeltaSPBMAddOnInterParam y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.spread_id = x->SpreadId;
    y.add_on_inter_rate_z2 = x->AddOnInterRateZ2;
    y.leg1_prod_family_code = Converter::Gb2312ToRustString(x->Leg1ProdFamilyCode);
    y.leg2_prod_family_code = Converter::Gb2312ToRustString(x->Leg2ProdFamilyCode);
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaSPMMInstParamField Converter::SyncDeltaSPMMInstParamToCpp(SyncDeltaSPMMInstParam x) {
    CThostFtdcSyncDeltaSPMMInstParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.InstMarginCalID = x.inst_margin_cal_id;
    strcpy(y.CommodityID, x.commodity_id.c_str());
    strcpy(y.CommodityGroupID, x.commodity_group_id.c_str());
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaSPMMInstParam Converter::CThostFtdcSyncDeltaSPMMInstParamFieldToRust(CThostFtdcSyncDeltaSPMMInstParamField* x) {
    if (x == nullptr)
        return SyncDeltaSPMMInstParam{.is_null = true};
    SyncDeltaSPMMInstParam y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.inst_margin_cal_id = x->InstMarginCalID;
    y.commodity_id = Converter::Gb2312ToRustString(x->CommodityID);
    y.commodity_group_id = Converter::Gb2312ToRustString(x->CommodityGroupID);
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaSPMMProductParamField Converter::SyncDeltaSPMMProductParamToCpp(SyncDeltaSPMMProductParam x) {
    CThostFtdcSyncDeltaSPMMProductParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    strcpy(y.CommodityID, x.commodity_id.c_str());
    strcpy(y.CommodityGroupID, x.commodity_group_id.c_str());
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaSPMMProductParam Converter::CThostFtdcSyncDeltaSPMMProductParamFieldToRust(CThostFtdcSyncDeltaSPMMProductParamField* x) {
    if (x == nullptr)
        return SyncDeltaSPMMProductParam{.is_null = true};
    SyncDeltaSPMMProductParam y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.commodity_id = Converter::Gb2312ToRustString(x->CommodityID);
    y.commodity_group_id = Converter::Gb2312ToRustString(x->CommodityGroupID);
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaInvestorSPMMModelField Converter::SyncDeltaInvestorSPMMModelToCpp(SyncDeltaInvestorSPMMModel x) {
    CThostFtdcSyncDeltaInvestorSPMMModelField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.SPMMModelID, x.spmm_model_id.c_str());
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaInvestorSPMMModel Converter::CThostFtdcSyncDeltaInvestorSPMMModelFieldToRust(CThostFtdcSyncDeltaInvestorSPMMModelField* x) {
    if (x == nullptr)
        return SyncDeltaInvestorSPMMModel{.is_null = true};
    SyncDeltaInvestorSPMMModel y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.spmm_model_id = Converter::Gb2312ToRustString(x->SPMMModelID);
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaSPMMModelParamField Converter::SyncDeltaSPMMModelParamToCpp(SyncDeltaSPMMModelParam x) {
    CThostFtdcSyncDeltaSPMMModelParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.SPMMModelID, x.spmm_model_id.c_str());
    strcpy(y.CommodityGroupID, x.commodity_group_id.c_str());
    y.IntraCommodityRate = x.intra_commodity_rate;
    y.InterCommodityRate = x.inter_commodity_rate;
    y.OptionDiscountRate = x.option_discount_rate;
    y.MiniMarginRatio = x.mini_margin_ratio;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaSPMMModelParam Converter::CThostFtdcSyncDeltaSPMMModelParamFieldToRust(CThostFtdcSyncDeltaSPMMModelParamField* x) {
    if (x == nullptr)
        return SyncDeltaSPMMModelParam{.is_null = true};
    SyncDeltaSPMMModelParam y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.spmm_model_id = Converter::Gb2312ToRustString(x->SPMMModelID);
    y.commodity_group_id = Converter::Gb2312ToRustString(x->CommodityGroupID);
    y.intra_commodity_rate = x->IntraCommodityRate;
    y.inter_commodity_rate = x->InterCommodityRate;
    y.option_discount_rate = x->OptionDiscountRate;
    y.mini_margin_ratio = x->MiniMarginRatio;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaRCAMSCombProdInfoField Converter::SyncDeltaRCAMSCombProdInfoToCpp(SyncDeltaRCAMSCombProdInfo x) {
    CThostFtdcSyncDeltaRCAMSCombProdInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    strcpy(y.CombProductID, x.comb_product_id.c_str());
    strcpy(y.ProductGroupID, x.product_group_id.c_str());
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaRCAMSCombProdInfo Converter::CThostFtdcSyncDeltaRCAMSCombProdInfoFieldToRust(CThostFtdcSyncDeltaRCAMSCombProdInfoField* x) {
    if (x == nullptr)
        return SyncDeltaRCAMSCombProdInfo{.is_null = true};
    SyncDeltaRCAMSCombProdInfo y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.comb_product_id = Converter::Gb2312ToRustString(x->CombProductID);
    y.product_group_id = Converter::Gb2312ToRustString(x->ProductGroupID);
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaRCAMSInstrParameterField Converter::SyncDeltaRCAMSInstrParameterToCpp(SyncDeltaRCAMSInstrParameter x) {
    CThostFtdcSyncDeltaRCAMSInstrParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    y.HedgeRate = x.hedge_rate;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaRCAMSInstrParameter Converter::CThostFtdcSyncDeltaRCAMSInstrParameterFieldToRust(CThostFtdcSyncDeltaRCAMSInstrParameterField* x) {
    if (x == nullptr)
        return SyncDeltaRCAMSInstrParameter{.is_null = true};
    SyncDeltaRCAMSInstrParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.hedge_rate = x->HedgeRate;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaRCAMSIntraParameterField Converter::SyncDeltaRCAMSIntraParameterToCpp(SyncDeltaRCAMSIntraParameter x) {
    CThostFtdcSyncDeltaRCAMSIntraParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.CombProductID, x.comb_product_id.c_str());
    y.HedgeRate = x.hedge_rate;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaRCAMSIntraParameter Converter::CThostFtdcSyncDeltaRCAMSIntraParameterFieldToRust(CThostFtdcSyncDeltaRCAMSIntraParameterField* x) {
    if (x == nullptr)
        return SyncDeltaRCAMSIntraParameter{.is_null = true};
    SyncDeltaRCAMSIntraParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.comb_product_id = Converter::Gb2312ToRustString(x->CombProductID);
    y.hedge_rate = x->HedgeRate;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaRCAMSInterParameterField Converter::SyncDeltaRCAMSInterParameterToCpp(SyncDeltaRCAMSInterParameter x) {
    CThostFtdcSyncDeltaRCAMSInterParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProductGroupID, x.product_group_id.c_str());
    y.Priority = x.priority;
    y.CreditRate = x.credit_rate;
    strcpy(y.CombProduct1, x.comb_product1.c_str());
    strcpy(y.CombProduct2, x.comb_product2.c_str());
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaRCAMSInterParameter Converter::CThostFtdcSyncDeltaRCAMSInterParameterFieldToRust(CThostFtdcSyncDeltaRCAMSInterParameterField* x) {
    if (x == nullptr)
        return SyncDeltaRCAMSInterParameter{.is_null = true};
    SyncDeltaRCAMSInterParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.product_group_id = Converter::Gb2312ToRustString(x->ProductGroupID);
    y.priority = x->Priority;
    y.credit_rate = x->CreditRate;
    y.comb_product1 = Converter::Gb2312ToRustString(x->CombProduct1);
    y.comb_product2 = Converter::Gb2312ToRustString(x->CombProduct2);
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaRCAMSSOptAdjParamField Converter::SyncDeltaRCAMSSOptAdjParamToCpp(SyncDeltaRCAMSSOptAdjParam x) {
    CThostFtdcSyncDeltaRCAMSSOptAdjParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.CombProductID, x.comb_product_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.AdjustValue = x.adjust_value;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaRCAMSSOptAdjParam Converter::CThostFtdcSyncDeltaRCAMSSOptAdjParamFieldToRust(CThostFtdcSyncDeltaRCAMSSOptAdjParamField* x) {
    if (x == nullptr)
        return SyncDeltaRCAMSSOptAdjParam{.is_null = true};
    SyncDeltaRCAMSSOptAdjParam y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.comb_product_id = Converter::Gb2312ToRustString(x->CombProductID);
    y.hedge_flag = x->HedgeFlag;
    y.adjust_value = x->AdjustValue;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaRCAMSCombRuleDtlField Converter::SyncDeltaRCAMSCombRuleDtlToCpp(SyncDeltaRCAMSCombRuleDtl x) {
    CThostFtdcSyncDeltaRCAMSCombRuleDtlField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    memcpy(y.ProdGroup, x.prod_group.data(), x.prod_group.size() * sizeof(uint8_t));
    memcpy(y.RuleId, x.rule_id.data(), x.rule_id.size() * sizeof(uint8_t));
    y.Priority = x.priority;
    y.HedgeFlag = x.hedge_flag;
    y.CombMargin = x.comb_margin;
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    y.LegID = x.leg_id;
    strcpy(y.LegInstrumentID, x.leg_instrument_id.c_str());
    y.Direction = x.direction;
    y.LegMultiple = x.leg_multiple;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaRCAMSCombRuleDtl Converter::CThostFtdcSyncDeltaRCAMSCombRuleDtlFieldToRust(CThostFtdcSyncDeltaRCAMSCombRuleDtlField* x) {
    if (x == nullptr)
        return SyncDeltaRCAMSCombRuleDtl{.is_null = true};
    SyncDeltaRCAMSCombRuleDtl y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    for (int i = 0; i < 41; i++)
        y.prod_group.push_back(x->ProdGroup[i]);
    for (int i = 0; i < 51; i++)
        y.rule_id.push_back(x->RuleId[i]);
    y.priority = x->Priority;
    y.hedge_flag = x->HedgeFlag;
    y.comb_margin = x->CombMargin;
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.leg_id = x->LegID;
    y.leg_instrument_id = Converter::Gb2312ToRustString(x->LegInstrumentID);
    y.direction = x->Direction;
    y.leg_multiple = x->LegMultiple;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaRCAMSInvstCombPosField Converter::SyncDeltaRCAMSInvestCombPosToCpp(SyncDeltaRCAMSInvestCombPos x) {
    CThostFtdcSyncDeltaRCAMSInvstCombPosField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.PosiDirection = x.posi_direction;
    strcpy(y.CombInstrumentID, x.comb_instrument_id.c_str());
    y.LegID = x.leg_id;
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    y.TotalAmt = x.total_amt;
    y.ExchMargin = x.exch_margin;
    y.Margin = x.margin;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaRCAMSInvestCombPos Converter::CThostFtdcSyncDeltaRCAMSInvstCombPosFieldToRust(CThostFtdcSyncDeltaRCAMSInvstCombPosField* x) {
    if (x == nullptr)
        return SyncDeltaRCAMSInvestCombPos{.is_null = true};
    SyncDeltaRCAMSInvestCombPos y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.hedge_flag = x->HedgeFlag;
    y.posi_direction = x->PosiDirection;
    y.comb_instrument_id = Converter::Gb2312ToRustString(x->CombInstrumentID);
    y.leg_id = x->LegID;
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    y.total_amt = x->TotalAmt;
    y.exch_margin = x->ExchMargin;
    y.margin = x->Margin;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaRULEInstrParameterField Converter::SyncDeltaRULEInstrParameterToCpp(SyncDeltaRULEInstrParameter x) {
    CThostFtdcSyncDeltaRULEInstrParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.InstrumentClass = x.instrument_class;
    strcpy(y.StdInstrumentID, x.std_instrument_id.c_str());
    y.BSpecRatio = x.b_spec_ratio;
    y.SSpecRatio = x.s_spec_ratio;
    y.BHedgeRatio = x.b_hedge_ratio;
    y.SHedgeRatio = x.s_hedge_ratio;
    y.BAddOnMargin = x.badd_on_margin;
    y.SAddOnMargin = x.s_add_on_margin;
    y.CommodityGroupID = x.commodity_group_id;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaRULEInstrParameter Converter::CThostFtdcSyncDeltaRULEInstrParameterFieldToRust(CThostFtdcSyncDeltaRULEInstrParameterField* x) {
    if (x == nullptr)
        return SyncDeltaRULEInstrParameter{.is_null = true};
    SyncDeltaRULEInstrParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.instrument_class = x->InstrumentClass;
    y.std_instrument_id = Converter::Gb2312ToRustString(x->StdInstrumentID);
    y.b_spec_ratio = x->BSpecRatio;
    y.s_spec_ratio = x->SSpecRatio;
    y.b_hedge_ratio = x->BHedgeRatio;
    y.s_hedge_ratio = x->SHedgeRatio;
    y.badd_on_margin = x->BAddOnMargin;
    y.s_add_on_margin = x->SAddOnMargin;
    y.commodity_group_id = x->CommodityGroupID;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaRULEIntraParameterField Converter::SyncDeltaRULEIntraParameterToCpp(SyncDeltaRULEIntraParameter x) {
    CThostFtdcSyncDeltaRULEIntraParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.ProdFamilyCode, x.prod_family_code.c_str());
    strcpy(y.StdInstrumentID, x.std_instrument_id.c_str());
    y.StdInstrMargin = x.std_instr_margin;
    y.UsualIntraRate = x.usual_intra_rate;
    y.DeliveryIntraRate = x.delivery_intra_rate;
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaRULEIntraParameter Converter::CThostFtdcSyncDeltaRULEIntraParameterFieldToRust(CThostFtdcSyncDeltaRULEIntraParameterField* x) {
    if (x == nullptr)
        return SyncDeltaRULEIntraParameter{.is_null = true};
    SyncDeltaRULEIntraParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.prod_family_code = Converter::Gb2312ToRustString(x->ProdFamilyCode);
    y.std_instrument_id = Converter::Gb2312ToRustString(x->StdInstrumentID);
    y.std_instr_margin = x->StdInstrMargin;
    y.usual_intra_rate = x->UsualIntraRate;
    y.delivery_intra_rate = x->DeliveryIntraRate;
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcSyncDeltaRULEInterParameterField Converter::SyncDeltaRULEInterParameterToCpp(SyncDeltaRULEInterParameter x) {
    CThostFtdcSyncDeltaRULEInterParameterField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradingDay, x.trading_day.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    y.SpreadId = x.spread_id;
    y.InterRate = x.inter_rate;
    strcpy(y.Leg1ProdFamilyCode, x.leg1_prod_family_code.c_str());
    strcpy(y.Leg2ProdFamilyCode, x.leg2_prod_family_code.c_str());
    y.Leg1PropFactor = x.leg1_prop_factor;
    y.Leg2PropFactor = x.leg2_prop_factor;
    y.CommodityGroupID = x.commodity_group_id;
    strcpy(y.CommodityGroupName, x.commodity_group_name.c_str());
    y.ActionDirection = x.action_direction;
    y.SyncDeltaSequenceNo = x.sync_delta_sequence_no;
    return y;
}

SyncDeltaRULEInterParameter Converter::CThostFtdcSyncDeltaRULEInterParameterFieldToRust(CThostFtdcSyncDeltaRULEInterParameterField* x) {
    if (x == nullptr)
        return SyncDeltaRULEInterParameter{.is_null = true};
    SyncDeltaRULEInterParameter y{};
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.spread_id = x->SpreadId;
    y.inter_rate = x->InterRate;
    y.leg1_prod_family_code = Converter::Gb2312ToRustString(x->Leg1ProdFamilyCode);
    y.leg2_prod_family_code = Converter::Gb2312ToRustString(x->Leg2ProdFamilyCode);
    y.leg1_prop_factor = x->Leg1PropFactor;
    y.leg2_prop_factor = x->Leg2PropFactor;
    y.commodity_group_id = x->CommodityGroupID;
    y.commodity_group_name = Converter::Gb2312ToRustString(x->CommodityGroupName);
    y.action_direction = x->ActionDirection;
    y.sync_delta_sequence_no = x->SyncDeltaSequenceNo;
    return y;
}

CThostFtdcIpAddrParamField Converter::IpAddrParamToCpp(IpAddrParam x) {
    CThostFtdcIpAddrParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.Address, x.address.c_str());
    y.DRIdentityID = x.dr_identity_id;
    strcpy(y.DRIdentityName, x.dr_identity_name.c_str());
    y.AddrSrvMode = x.addr_srv_mode;
    y.AddrVer = x.addr_ver;
    y.AddrNo = x.addr_no;
    strcpy(y.AddrName, x.addr_name.c_str());
    y.IsSM = x.is_sm;
    y.IsLocalAddr = x.is_local_addr;
    strcpy(y.Remark, x.remark.c_str());
    memcpy(y.Site, x.site.data(), x.site.size() * sizeof(uint8_t));
    memcpy(y.NetOperator, x.net_operator.data(), x.net_operator.size() * sizeof(uint8_t));
    return y;
}

IpAddrParam Converter::CThostFtdcIpAddrParamFieldToRust(CThostFtdcIpAddrParamField* x) {
    if (x == nullptr)
        return IpAddrParam{.is_null = true};
    IpAddrParam y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.address = Converter::Gb2312ToRustString(x->Address);
    y.dr_identity_id = x->DRIdentityID;
    y.dr_identity_name = Converter::Gb2312ToRustString(x->DRIdentityName);
    y.addr_srv_mode = x->AddrSrvMode;
    y.addr_ver = x->AddrVer;
    y.addr_no = x->AddrNo;
    y.addr_name = Converter::Gb2312ToRustString(x->AddrName);
    y.is_sm = x->IsSM;
    y.is_local_addr = x->IsLocalAddr;
    y.remark = Converter::Gb2312ToRustString(x->Remark);
    for (int i = 0; i < 51; i++)
        y.site.push_back(x->Site[i]);
    for (int i = 0; i < 9; i++)
        y.net_operator.push_back(x->NetOperator[i]);
    return y;
}

CThostFtdcQryIpAddrParamField Converter::QryIpAddrParamToCpp(QryIpAddrParam x) {
    CThostFtdcQryIpAddrParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    return y;
}

QryIpAddrParam Converter::CThostFtdcQryIpAddrParamFieldToRust(CThostFtdcQryIpAddrParamField* x) {
    if (x == nullptr)
        return QryIpAddrParam{.is_null = true};
    QryIpAddrParam y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    return y;
}

CThostFtdcTGIpAddrParamField Converter::TGIpAddrParamToCpp(TGIpAddrParam x) {
    CThostFtdcTGIpAddrParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.Address, x.address.c_str());
    y.DRIdentityID = x.dr_identity_id;
    strcpy(y.DRIdentityName, x.dr_identity_name.c_str());
    y.AddrSrvMode = x.addr_srv_mode;
    y.AddrVer = x.addr_ver;
    y.AddrNo = x.addr_no;
    strcpy(y.AddrName, x.addr_name.c_str());
    y.IsSM = x.is_sm;
    y.IsLocalAddr = x.is_local_addr;
    strcpy(y.Remark, x.remark.c_str());
    memcpy(y.Site, x.site.data(), x.site.size() * sizeof(uint8_t));
    memcpy(y.NetOperator, x.net_operator.data(), x.net_operator.size() * sizeof(uint8_t));
    return y;
}

TGIpAddrParam Converter::CThostFtdcTGIpAddrParamFieldToRust(CThostFtdcTGIpAddrParamField* x) {
    if (x == nullptr)
        return TGIpAddrParam{.is_null = true};
    TGIpAddrParam y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.address = Converter::Gb2312ToRustString(x->Address);
    y.dr_identity_id = x->DRIdentityID;
    y.dr_identity_name = Converter::Gb2312ToRustString(x->DRIdentityName);
    y.addr_srv_mode = x->AddrSrvMode;
    y.addr_ver = x->AddrVer;
    y.addr_no = x->AddrNo;
    y.addr_name = Converter::Gb2312ToRustString(x->AddrName);
    y.is_sm = x->IsSM;
    y.is_local_addr = x->IsLocalAddr;
    y.remark = Converter::Gb2312ToRustString(x->Remark);
    for (int i = 0; i < 51; i++)
        y.site.push_back(x->Site[i]);
    for (int i = 0; i < 9; i++)
        y.net_operator.push_back(x->NetOperator[i]);
    return y;
}

CThostFtdcQryTGIpAddrParamField Converter::QryTGIpAddrParamToCpp(QryTGIpAddrParam x) {
    CThostFtdcQryTGIpAddrParamField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.AppID, x.app_id.c_str());
    return y;
}

QryTGIpAddrParam Converter::CThostFtdcQryTGIpAddrParamFieldToRust(CThostFtdcQryTGIpAddrParamField* x) {
    if (x == nullptr)
        return QryTGIpAddrParam{.is_null = true};
    QryTGIpAddrParam y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.app_id = Converter::Gb2312ToRustString(x->AppID);
    return y;
}

CThostFtdcTGSessionQryStatusField Converter::TGSessionQryStatusToCpp(TGSessionQryStatus x) {
    CThostFtdcTGSessionQryStatusField y;
    memset(&y, 0, sizeof(y));
    y.LastQryFreq = x.last_qry_freq;
    y.QryStatus = x.qry_status;
    return y;
}

TGSessionQryStatus Converter::CThostFtdcTGSessionQryStatusFieldToRust(CThostFtdcTGSessionQryStatusField* x) {
    if (x == nullptr)
        return TGSessionQryStatus{.is_null = true};
    TGSessionQryStatus y{};
    y.last_qry_freq = x->LastQryFreq;
    y.qry_status = x->QryStatus;
    return y;
}

CThostFtdcLocalAddrConfigField Converter::LocalAddrConfigToCpp(LocalAddrConfig x) {
    CThostFtdcLocalAddrConfigField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.PeerAddr, x.peer_addr.c_str());
    memcpy(y.NetMask, x.net_mask.data(), x.net_mask.size() * sizeof(uint8_t));
    y.DRIdentityID = x.dr_identity_id;
    strcpy(y.LocalAddress, x.local_address.c_str());
    return y;
}

LocalAddrConfig Converter::CThostFtdcLocalAddrConfigFieldToRust(CThostFtdcLocalAddrConfigField* x) {
    if (x == nullptr)
        return LocalAddrConfig{.is_null = true};
    LocalAddrConfig y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.peer_addr = Converter::Gb2312ToRustString(x->PeerAddr);
    for (int i = 0; i < 129; i++)
        y.net_mask.push_back(x->NetMask[i]);
    y.dr_identity_id = x->DRIdentityID;
    y.local_address = Converter::Gb2312ToRustString(x->LocalAddress);
    return y;
}

CThostFtdcQryLocalAddrConfigField Converter::QryLocalAddrConfigToCpp(QryLocalAddrConfig x) {
    CThostFtdcQryLocalAddrConfigField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    return y;
}

QryLocalAddrConfig Converter::CThostFtdcQryLocalAddrConfigFieldToRust(CThostFtdcQryLocalAddrConfigField* x) {
    if (x == nullptr)
        return QryLocalAddrConfig{.is_null = true};
    QryLocalAddrConfig y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    return y;
}

CThostFtdcReqQueryBankAccountBySecField Converter::ReqQueryBankAccountBySecToCpp(ReqQueryBankAccountBySec x) {
    CThostFtdcReqQueryBankAccountBySecField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.FutureSerial = x.future_serial;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    y.DRIdentityID = x.dr_identity_id;
    y.SecFutureSerial = x.sec_future_serial;
    return y;
}

ReqQueryBankAccountBySec Converter::CThostFtdcReqQueryBankAccountBySecFieldToRust(CThostFtdcReqQueryBankAccountBySecField* x) {
    if (x == nullptr)
        return ReqQueryBankAccountBySec{.is_null = true};
    ReqQueryBankAccountBySec y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.future_serial = x->FutureSerial;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    y.dr_identity_id = x->DRIdentityID;
    y.sec_future_serial = x->SecFutureSerial;
    return y;
}

CThostFtdcRspQueryBankAccountBySecField Converter::RspQueryBankAccountBySecToCpp(RspQueryBankAccountBySec x) {
    CThostFtdcRspQueryBankAccountBySecField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.FutureSerial = x.future_serial;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.BankUseAmount = x.bank_use_amount;
    y.BankFetchAmount = x.bank_fetch_amount;
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    y.DRIdentityID = x.dr_identity_id;
    y.SecFutureSerial = x.sec_future_serial;
    return y;
}

RspQueryBankAccountBySec Converter::CThostFtdcRspQueryBankAccountBySecFieldToRust(CThostFtdcRspQueryBankAccountBySecField* x) {
    if (x == nullptr)
        return RspQueryBankAccountBySec{.is_null = true};
    RspQueryBankAccountBySec y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.future_serial = x->FutureSerial;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.bank_use_amount = x->BankUseAmount;
    y.bank_fetch_amount = x->BankFetchAmount;
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    y.dr_identity_id = x->DRIdentityID;
    y.sec_future_serial = x->SecFutureSerial;
    return y;
}

CThostFtdcReqTransferBySecField Converter::ReqTransferBySecToCpp(ReqTransferBySec x) {
    CThostFtdcReqTransferBySecField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.InstallID = x.install_id;
    y.FutureSerial = x.future_serial;
    strcpy(y.UserID, x.user_id.c_str());
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.TradeAmount = x.trade_amount;
    y.FutureFetchAmount = x.future_fetch_amount;
    y.FeePayFlag = x.fee_pay_flag;
    y.CustFee = x.cust_fee;
    y.BrokerFee = x.broker_fee;
    memcpy(y.Message, x.message.data(), x.message.size() * sizeof(uint8_t));
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.TransferStatus = x.transfer_status;
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    y.DRIdentityID = x.dr_identity_id;
    y.SecFutureSerial = x.sec_future_serial;
    return y;
}

ReqTransferBySec Converter::CThostFtdcReqTransferBySecFieldToRust(CThostFtdcReqTransferBySecField* x) {
    if (x == nullptr)
        return ReqTransferBySec{.is_null = true};
    ReqTransferBySec y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.install_id = x->InstallID;
    y.future_serial = x->FutureSerial;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.trade_amount = x->TradeAmount;
    y.future_fetch_amount = x->FutureFetchAmount;
    y.fee_pay_flag = x->FeePayFlag;
    y.cust_fee = x->CustFee;
    y.broker_fee = x->BrokerFee;
    for (int i = 0; i < 129; i++)
        y.message.push_back(x->Message[i]);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.transfer_status = x->TransferStatus;
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    y.dr_identity_id = x->DRIdentityID;
    y.sec_future_serial = x->SecFutureSerial;
    return y;
}

CThostFtdcRspTransferBySecField Converter::RspTransferBySecToCpp(RspTransferBySec x) {
    CThostFtdcRspTransferBySecField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.InstallID = x.install_id;
    y.FutureSerial = x.future_serial;
    strcpy(y.UserID, x.user_id.c_str());
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.TradeAmount = x.trade_amount;
    y.FutureFetchAmount = x.future_fetch_amount;
    y.FeePayFlag = x.fee_pay_flag;
    y.CustFee = x.cust_fee;
    y.BrokerFee = x.broker_fee;
    memcpy(y.Message, x.message.data(), x.message.size() * sizeof(uint8_t));
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.TransferStatus = x.transfer_status;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    y.DRIdentityID = x.dr_identity_id;
    y.SecFutureSerial = x.sec_future_serial;
    return y;
}

RspTransferBySec Converter::CThostFtdcRspTransferBySecFieldToRust(CThostFtdcRspTransferBySecField* x) {
    if (x == nullptr)
        return RspTransferBySec{.is_null = true};
    RspTransferBySec y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.install_id = x->InstallID;
    y.future_serial = x->FutureSerial;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.trade_amount = x->TradeAmount;
    y.future_fetch_amount = x->FutureFetchAmount;
    y.fee_pay_flag = x->FeePayFlag;
    y.cust_fee = x->CustFee;
    y.broker_fee = x->BrokerFee;
    for (int i = 0; i < 129; i++)
        y.message.push_back(x->Message[i]);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.transfer_status = x->TransferStatus;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    y.dr_identity_id = x->DRIdentityID;
    y.sec_future_serial = x->SecFutureSerial;
    return y;
}

CThostFtdcNotifyQueryFutureAccountBySecField Converter::NotifyQueryFutureAccountBySecToCpp(NotifyQueryFutureAccountBySec x) {
    CThostFtdcNotifyQueryFutureAccountBySecField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.TradeCode, x.trade_code.c_str());
    strcpy(y.BankID, x.bank_id.c_str());
    strcpy(y.BankBranchID, x.bank_branch_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.BrokerBranchID, x.broker_branch_id.c_str());
    strcpy(y.TradeDate, x.trade_date.c_str());
    strcpy(y.TradeTime, x.trade_time.c_str());
    strcpy(y.BankSerial, x.bank_serial.c_str());
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.PlateSerial = x.plate_serial;
    y.LastFragment = x.last_fragment;
    y.SessionID = x.session_id;
    strcpy(y.CustomerName, x.customer_name.c_str());
    y.IdCardType = x.id_card_type;
    strcpy(y.IdentifiedCardNo, x.identified_card_no.c_str());
    y.CustType = x.cust_type;
    strcpy(y.BankAccount, x.bank_account.c_str());
    strcpy(y.BankPassWord, x.bank_pass_word.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.Password, x.password.c_str());
    y.FutureSerial = x.future_serial;
    y.InstallID = x.install_id;
    strcpy(y.UserID, x.user_id.c_str());
    y.VerifyCertNoFlag = x.verify_cert_no_flag;
    strcpy(y.CurrencyID, x.currency_id.c_str());
    memcpy(y.Digest, x.digest.data(), x.digest.size() * sizeof(uint8_t));
    y.BankAccType = x.bank_acc_type;
    strcpy(y.DeviceID, x.device_id.c_str());
    y.BankSecuAccType = x.bank_secu_acc_type;
    memcpy(y.BrokerIDByBank, x.broker_id_by_bank.data(), x.broker_id_by_bank.size() * sizeof(uint8_t));
    memcpy(y.BankSecuAcc, x.bank_secu_acc.data(), x.bank_secu_acc.size() * sizeof(uint8_t));
    y.BankPwdFlag = x.bank_pwd_flag;
    y.SecuPwdFlag = x.secu_pwd_flag;
    strcpy(y.OperNo, x.oper_no.c_str());
    y.RequestID = x.request_id;
    y.TID = x.tid;
    y.BankUseAmount = x.bank_use_amount;
    y.BankFetchAmount = x.bank_fetch_amount;
    y.ErrorID = x.error_id;
    strcpy(y.ErrorMsg, x.error_msg.c_str());
    strcpy(y.LongCustomerName, x.long_customer_name.c_str());
    y.DRIdentityID = x.dr_identity_id;
    y.SecFutureSerial = x.sec_future_serial;
    return y;
}

NotifyQueryFutureAccountBySec Converter::CThostFtdcNotifyQueryFutureAccountBySecFieldToRust(CThostFtdcNotifyQueryFutureAccountBySecField* x) {
    if (x == nullptr)
        return NotifyQueryFutureAccountBySec{.is_null = true};
    NotifyQueryFutureAccountBySec y{};
    y.trade_code = Converter::Gb2312ToRustString(x->TradeCode);
    y.bank_id = Converter::Gb2312ToRustString(x->BankID);
    y.bank_branch_id = Converter::Gb2312ToRustString(x->BankBranchID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.broker_branch_id = Converter::Gb2312ToRustString(x->BrokerBranchID);
    y.trade_date = Converter::Gb2312ToRustString(x->TradeDate);
    y.trade_time = Converter::Gb2312ToRustString(x->TradeTime);
    y.bank_serial = Converter::Gb2312ToRustString(x->BankSerial);
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.plate_serial = x->PlateSerial;
    y.last_fragment = x->LastFragment;
    y.session_id = x->SessionID;
    y.customer_name = Converter::Gb2312ToRustString(x->CustomerName);
    y.id_card_type = x->IdCardType;
    y.identified_card_no = Converter::Gb2312ToRustString(x->IdentifiedCardNo);
    y.cust_type = x->CustType;
    y.bank_account = Converter::Gb2312ToRustString(x->BankAccount);
    y.bank_pass_word = Converter::Gb2312ToRustString(x->BankPassWord);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.password = Converter::Gb2312ToRustString(x->Password);
    y.future_serial = x->FutureSerial;
    y.install_id = x->InstallID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.verify_cert_no_flag = x->VerifyCertNoFlag;
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    for (int i = 0; i < 36; i++)
        y.digest.push_back(x->Digest[i]);
    y.bank_acc_type = x->BankAccType;
    y.device_id = Converter::Gb2312ToRustString(x->DeviceID);
    y.bank_secu_acc_type = x->BankSecuAccType;
    for (int i = 0; i < 33; i++)
        y.broker_id_by_bank.push_back(x->BrokerIDByBank[i]);
    for (int i = 0; i < 41; i++)
        y.bank_secu_acc.push_back(x->BankSecuAcc[i]);
    y.bank_pwd_flag = x->BankPwdFlag;
    y.secu_pwd_flag = x->SecuPwdFlag;
    y.oper_no = Converter::Gb2312ToRustString(x->OperNo);
    y.request_id = x->RequestID;
    y.tid = x->TID;
    y.bank_use_amount = x->BankUseAmount;
    y.bank_fetch_amount = x->BankFetchAmount;
    y.error_id = x->ErrorID;
    y.error_msg = Converter::Gb2312ToRustString(x->ErrorMsg);
    y.long_customer_name = Converter::Gb2312ToRustString(x->LongCustomerName);
    y.dr_identity_id = x->DRIdentityID;
    y.sec_future_serial = x->SecFutureSerial;
    return y;
}

CThostFtdcExitEmergencyField Converter::ExitEmergencyToCpp(ExitEmergency x) {
    CThostFtdcExitEmergencyField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    return y;
}

ExitEmergency Converter::CThostFtdcExitEmergencyFieldToRust(CThostFtdcExitEmergencyField* x) {
    if (x == nullptr)
        return ExitEmergency{.is_null = true};
    ExitEmergency y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    return y;
}

CThostFtdcInvestorPortfMarginModelField Converter::InvestorPortfMarginModelToCpp(InvestorPortfMarginModel x) {
    CThostFtdcInvestorPortfMarginModelField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.MarginModelID, x.margin_model_id.c_str());
    return y;
}

InvestorPortfMarginModel Converter::CThostFtdcInvestorPortfMarginModelFieldToRust(CThostFtdcInvestorPortfMarginModelField* x) {
    if (x == nullptr)
        return InvestorPortfMarginModel{.is_null = true};
    InvestorPortfMarginModel y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.margin_model_id = Converter::Gb2312ToRustString(x->MarginModelID);
    return y;
}

CThostFtdcInvestorPortfSettingField Converter::InvestorPortfSettingToCpp(InvestorPortfSetting x) {
    CThostFtdcInvestorPortfSettingField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    y.HedgeFlag = x.hedge_flag;
    y.UsePortf = x.use_portf;
    return y;
}

InvestorPortfSetting Converter::CThostFtdcInvestorPortfSettingFieldToRust(CThostFtdcInvestorPortfSettingField* x) {
    if (x == nullptr)
        return InvestorPortfSetting{.is_null = true};
    InvestorPortfSetting y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.hedge_flag = x->HedgeFlag;
    y.use_portf = x->UsePortf;
    return y;
}

CThostFtdcQryInvestorPortfSettingField Converter::QryInvestorPortfSettingToCpp(QryInvestorPortfSetting x) {
    CThostFtdcQryInvestorPortfSettingField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    return y;
}

QryInvestorPortfSetting Converter::CThostFtdcQryInvestorPortfSettingFieldToRust(CThostFtdcQryInvestorPortfSettingField* x) {
    if (x == nullptr)
        return QryInvestorPortfSetting{.is_null = true};
    QryInvestorPortfSetting y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    return y;
}

CThostFtdcUserPasswordUpdateFromSecField Converter::UserPasswordUpdateFromSecToCpp(UserPasswordUpdateFromSec x) {
    CThostFtdcUserPasswordUpdateFromSecField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.OldPassword, x.old_password.c_str());
    strcpy(y.NewPassword, x.new_password.c_str());
    y.FromSec = x.from_sec;
    return y;
}

UserPasswordUpdateFromSec Converter::CThostFtdcUserPasswordUpdateFromSecFieldToRust(CThostFtdcUserPasswordUpdateFromSecField* x) {
    if (x == nullptr)
        return UserPasswordUpdateFromSec{.is_null = true};
    UserPasswordUpdateFromSec y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.old_password = Converter::Gb2312ToRustString(x->OldPassword);
    y.new_password = Converter::Gb2312ToRustString(x->NewPassword);
    y.from_sec = x->FromSec;
    return y;
}

CThostFtdcSettlementInfoConfirmFromSecField Converter::SettlementInfoConfirmFromSecToCpp(SettlementInfoConfirmFromSec x) {
    CThostFtdcSettlementInfoConfirmFromSecField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ConfirmDate, x.confirm_date.c_str());
    strcpy(y.ConfirmTime, x.confirm_time.c_str());
    y.FromSec = x.from_sec;
    return y;
}

SettlementInfoConfirmFromSec Converter::CThostFtdcSettlementInfoConfirmFromSecFieldToRust(CThostFtdcSettlementInfoConfirmFromSecField* x) {
    if (x == nullptr)
        return SettlementInfoConfirmFromSec{.is_null = true};
    SettlementInfoConfirmFromSec y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.confirm_date = Converter::Gb2312ToRustString(x->ConfirmDate);
    y.confirm_time = Converter::Gb2312ToRustString(x->ConfirmTime);
    y.from_sec = x->FromSec;
    return y;
}

CThostFtdcTradingAccountPasswordUpdateFromSecField Converter::TradingAccountPasswordUpdateFromSecToCpp(TradingAccountPasswordUpdateFromSec x) {
    CThostFtdcTradingAccountPasswordUpdateFromSecField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.AccountID, x.account_id.c_str());
    strcpy(y.OldPassword, x.old_password.c_str());
    strcpy(y.NewPassword, x.new_password.c_str());
    strcpy(y.CurrencyID, x.currency_id.c_str());
    y.FromSec = x.from_sec;
    return y;
}

TradingAccountPasswordUpdateFromSec Converter::CThostFtdcTradingAccountPasswordUpdateFromSecFieldToRust(CThostFtdcTradingAccountPasswordUpdateFromSecField* x) {
    if (x == nullptr)
        return TradingAccountPasswordUpdateFromSec{.is_null = true};
    TradingAccountPasswordUpdateFromSec y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.account_id = Converter::Gb2312ToRustString(x->AccountID);
    y.old_password = Converter::Gb2312ToRustString(x->OldPassword);
    y.new_password = Converter::Gb2312ToRustString(x->NewPassword);
    y.currency_id = Converter::Gb2312ToRustString(x->CurrencyID);
    y.from_sec = x->FromSec;
    return y;
}

CThostFtdcRiskForbiddenRightField Converter::RiskForbiddenRightToCpp(RiskForbiddenRight x) {
    CThostFtdcRiskForbiddenRightField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    return y;
}

RiskForbiddenRight Converter::CThostFtdcRiskForbiddenRightFieldToRust(CThostFtdcRiskForbiddenRightField* x) {
    if (x == nullptr)
        return RiskForbiddenRight{.is_null = true};
    RiskForbiddenRight y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    return y;
}

CThostFtdcInvestorInfoCommRecField Converter::InvestorInfoCommRecToCpp(InvestorInfoCommRec x) {
    CThostFtdcInvestorInfoCommRecField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    y.OrderCount = x.order_count;
    y.OrderActionCount = x.order_action_count;
    y.ForQuoteCnt = x.for_quote_cnt;
    y.InfoComm = x.info_comm;
    y.IsOptSeries = x.is_opt_series;
    strcpy(y.ProductID, x.product_id.c_str());
    y.InfoCnt = x.info_cnt;
    return y;
}

InvestorInfoCommRec Converter::CThostFtdcInvestorInfoCommRecFieldToRust(CThostFtdcInvestorInfoCommRecField* x) {
    if (x == nullptr)
        return InvestorInfoCommRec{.is_null = true};
    InvestorInfoCommRec y{};
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.order_count = x->OrderCount;
    y.order_action_count = x->OrderActionCount;
    y.for_quote_cnt = x->ForQuoteCnt;
    y.info_comm = x->InfoComm;
    y.is_opt_series = x->IsOptSeries;
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.info_cnt = x->InfoCnt;
    return y;
}

CThostFtdcQryInvestorInfoCommRecField Converter::QryInvestorInfoCommRecToCpp(QryInvestorInfoCommRec x) {
    CThostFtdcQryInvestorInfoCommRecField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.BrokerID, x.broker_id.c_str());
    return y;
}

QryInvestorInfoCommRec Converter::CThostFtdcQryInvestorInfoCommRecFieldToRust(CThostFtdcQryInvestorInfoCommRecField* x) {
    if (x == nullptr)
        return QryInvestorInfoCommRec{.is_null = true};
    QryInvestorInfoCommRec y{};
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    return y;
}

CThostFtdcCombLegField Converter::CombLegToCpp(CombLeg x) {
    CThostFtdcCombLegField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.CombInstrumentID, x.comb_instrument_id.c_str());
    y.LegID = x.leg_id;
    strcpy(y.LegInstrumentID, x.leg_instrument_id.c_str());
    y.Direction = x.direction;
    y.LegMultiple = x.leg_multiple;
    y.ImplyLevel = x.imply_level;
    return y;
}

CombLeg Converter::CThostFtdcCombLegFieldToRust(CThostFtdcCombLegField* x) {
    if (x == nullptr)
        return CombLeg{.is_null = true};
    CombLeg y{};
    y.comb_instrument_id = Converter::Gb2312ToRustString(x->CombInstrumentID);
    y.leg_id = x->LegID;
    y.leg_instrument_id = Converter::Gb2312ToRustString(x->LegInstrumentID);
    y.direction = x->Direction;
    y.leg_multiple = x->LegMultiple;
    y.imply_level = x->ImplyLevel;
    return y;
}

CThostFtdcQryCombLegField Converter::QryCombLegToCpp(QryCombLeg x) {
    CThostFtdcQryCombLegField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.LegInstrumentID, x.leg_instrument_id.c_str());
    return y;
}

QryCombLeg Converter::CThostFtdcQryCombLegFieldToRust(CThostFtdcQryCombLegField* x) {
    if (x == nullptr)
        return QryCombLeg{.is_null = true};
    QryCombLeg y{};
    y.leg_instrument_id = Converter::Gb2312ToRustString(x->LegInstrumentID);
    return y;
}

CThostFtdcInputOffsetSettingField Converter::InputOffsetSettingToCpp(InputOffsetSetting x) {
    CThostFtdcInputOffsetSettingField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.UnderlyingInstrID, x.underlying_instr_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    y.OffsetType = x.offset_type;
    y.Volume = x.volume;
    y.IsOffset = x.is_offset;
    y.RequestID = x.request_id;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    return y;
}

InputOffsetSetting Converter::CThostFtdcInputOffsetSettingFieldToRust(CThostFtdcInputOffsetSettingField* x) {
    if (x == nullptr)
        return InputOffsetSetting{.is_null = true};
    InputOffsetSetting y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.underlying_instr_id = Converter::Gb2312ToRustString(x->UnderlyingInstrID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.offset_type = x->OffsetType;
    y.volume = x->Volume;
    y.is_offset = x->IsOffset;
    y.request_id = x->RequestID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    return y;
}

CThostFtdcOffsetSettingField Converter::OffsetSettingToCpp(OffsetSetting x) {
    CThostFtdcOffsetSettingField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.UnderlyingInstrID, x.underlying_instr_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    y.OffsetType = x.offset_type;
    y.Volume = x.volume;
    y.IsOffset = x.is_offset;
    y.RequestID = x.request_id;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    memcpy(y.ExchangeSerialNo, x.exchange_serial_no.data(), x.exchange_serial_no.size() * sizeof(uint8_t));
    strcpy(y.ExchangeProductID, x.exchange_product_id.c_str());
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    y.OrderSubmitStatus = x.order_submit_status;
    strcpy(y.TradingDay, x.trading_day.c_str());
    y.SettlementID = x.settlement_id;
    strcpy(y.InsertDate, x.insert_date.c_str());
    strcpy(y.InsertTime, x.insert_time.c_str());
    strcpy(y.CancelTime, x.cancel_time.c_str());
    y.ExecResult = x.exec_result;
    y.SequenceNo = x.sequence_no;
    y.FrontID = x.front_id;
    y.SessionID = x.session_id;
    strcpy(y.StatusMsg, x.status_msg.c_str());
    strcpy(y.ActiveUserID, x.active_user_id.c_str());
    y.BrokerOffsetSettingSeq = x.broker_offset_setting_seq;
    y.ApplySrc = x.apply_src;
    return y;
}

OffsetSetting Converter::CThostFtdcOffsetSettingFieldToRust(CThostFtdcOffsetSettingField* x) {
    if (x == nullptr)
        return OffsetSetting{.is_null = true};
    OffsetSetting y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.underlying_instr_id = Converter::Gb2312ToRustString(x->UnderlyingInstrID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.offset_type = x->OffsetType;
    y.volume = x->Volume;
    y.is_offset = x->IsOffset;
    y.request_id = x->RequestID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    for (int i = 0; i < 81; i++)
        y.exchange_serial_no.push_back(x->ExchangeSerialNo[i]);
    y.exchange_product_id = Converter::Gb2312ToRustString(x->ExchangeProductID);
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.order_submit_status = x->OrderSubmitStatus;
    y.trading_day = Converter::Gb2312ToRustString(x->TradingDay);
    y.settlement_id = x->SettlementID;
    y.insert_date = Converter::Gb2312ToRustString(x->InsertDate);
    y.insert_time = Converter::Gb2312ToRustString(x->InsertTime);
    y.cancel_time = Converter::Gb2312ToRustString(x->CancelTime);
    y.exec_result = x->ExecResult;
    y.sequence_no = x->SequenceNo;
    y.front_id = x->FrontID;
    y.session_id = x->SessionID;
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.active_user_id = Converter::Gb2312ToRustString(x->ActiveUserID);
    y.broker_offset_setting_seq = x->BrokerOffsetSettingSeq;
    y.apply_src = x->ApplySrc;
    return y;
}

CThostFtdcCancelOffsetSettingField Converter::CancelOffsetSettingToCpp(CancelOffsetSetting x) {
    CThostFtdcCancelOffsetSettingField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.InstrumentID, x.instrument_id.c_str());
    strcpy(y.UnderlyingInstrID, x.underlying_instr_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    y.OffsetType = x.offset_type;
    y.Volume = x.volume;
    y.IsOffset = x.is_offset;
    y.RequestID = x.request_id;
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.ExchangeID, x.exchange_id.c_str());
    strcpy(y.IPAddress, x.ip_address.c_str());
    strcpy(y.MacAddress, x.mac_address.c_str());
    strcpy(y.ExchangeInstID, x.exchange_inst_id.c_str());
    memcpy(y.ExchangeSerialNo, x.exchange_serial_no.data(), x.exchange_serial_no.size() * sizeof(uint8_t));
    strcpy(y.ExchangeProductID, x.exchange_product_id.c_str());
    strcpy(y.TraderID, x.trader_id.c_str());
    y.InstallID = x.install_id;
    strcpy(y.ParticipantID, x.participant_id.c_str());
    strcpy(y.ClientID, x.client_id.c_str());
    y.OrderActionStatus = x.order_action_status;
    strcpy(y.StatusMsg, x.status_msg.c_str());
    strcpy(y.ActionLocalID, x.action_local_id.c_str());
    strcpy(y.ActionDate, x.action_date.c_str());
    strcpy(y.ActionTime, x.action_time.c_str());
    return y;
}

CancelOffsetSetting Converter::CThostFtdcCancelOffsetSettingFieldToRust(CThostFtdcCancelOffsetSettingField* x) {
    if (x == nullptr)
        return CancelOffsetSetting{.is_null = true};
    CancelOffsetSetting y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.instrument_id = Converter::Gb2312ToRustString(x->InstrumentID);
    y.underlying_instr_id = Converter::Gb2312ToRustString(x->UnderlyingInstrID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.offset_type = x->OffsetType;
    y.volume = x->Volume;
    y.is_offset = x->IsOffset;
    y.request_id = x->RequestID;
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.exchange_id = Converter::Gb2312ToRustString(x->ExchangeID);
    y.ip_address = Converter::Gb2312ToRustString(x->IPAddress);
    y.mac_address = Converter::Gb2312ToRustString(x->MacAddress);
    y.exchange_inst_id = Converter::Gb2312ToRustString(x->ExchangeInstID);
    for (int i = 0; i < 81; i++)
        y.exchange_serial_no.push_back(x->ExchangeSerialNo[i]);
    y.exchange_product_id = Converter::Gb2312ToRustString(x->ExchangeProductID);
    y.trader_id = Converter::Gb2312ToRustString(x->TraderID);
    y.install_id = x->InstallID;
    y.participant_id = Converter::Gb2312ToRustString(x->ParticipantID);
    y.client_id = Converter::Gb2312ToRustString(x->ClientID);
    y.order_action_status = x->OrderActionStatus;
    y.status_msg = Converter::Gb2312ToRustString(x->StatusMsg);
    y.action_local_id = Converter::Gb2312ToRustString(x->ActionLocalID);
    y.action_date = Converter::Gb2312ToRustString(x->ActionDate);
    y.action_time = Converter::Gb2312ToRustString(x->ActionTime);
    return y;
}

CThostFtdcQryOffsetSettingField Converter::QryOffsetSettingToCpp(QryOffsetSetting x) {
    CThostFtdcQryOffsetSettingField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.ProductID, x.product_id.c_str());
    y.OffsetType = x.offset_type;
    return y;
}

QryOffsetSetting Converter::CThostFtdcQryOffsetSettingFieldToRust(CThostFtdcQryOffsetSettingField* x) {
    if (x == nullptr)
        return QryOffsetSetting{.is_null = true};
    QryOffsetSetting y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.product_id = Converter::Gb2312ToRustString(x->ProductID);
    y.offset_type = x->OffsetType;
    return y;
}

CThostFtdcAddrAppIDRelationField Converter::AddrAppIDRelationToCpp(AddrAppIDRelation x) {
    CThostFtdcAddrAppIDRelationField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.Address, x.address.c_str());
    y.DRIdentityID = x.dr_identity_id;
    strcpy(y.AppID, x.app_id.c_str());
    return y;
}

AddrAppIDRelation Converter::CThostFtdcAddrAppIDRelationFieldToRust(CThostFtdcAddrAppIDRelationField* x) {
    if (x == nullptr)
        return AddrAppIDRelation{.is_null = true};
    AddrAppIDRelation y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.address = Converter::Gb2312ToRustString(x->Address);
    y.dr_identity_id = x->DRIdentityID;
    y.app_id = Converter::Gb2312ToRustString(x->AppID);
    return y;
}

CThostFtdcQryAddrAppIDRelationField Converter::QryAddrAppIDRelationToCpp(QryAddrAppIDRelation x) {
    CThostFtdcQryAddrAppIDRelationField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    return y;
}

QryAddrAppIDRelation Converter::CThostFtdcQryAddrAppIDRelationFieldToRust(CThostFtdcQryAddrAppIDRelationField* x) {
    if (x == nullptr)
        return QryAddrAppIDRelation{.is_null = true};
    QryAddrAppIDRelation y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    return y;
}

CThostFtdcWechatUserSystemInfoField Converter::WechatUserSystemInfoToCpp(WechatUserSystemInfo x) {
    CThostFtdcWechatUserSystemInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    y.WechatCltSysInfoLen = x.wechat_clt_sys_info_len;
    strcpy(y.WechatCltSysInfo, x.wechat_clt_sys_info.c_str());
    y.ClientIPPort = x.client_ip_port;
    strcpy(y.ClientLoginTime, x.client_login_time.c_str());
    strcpy(y.ClientAppID, x.client_app_id.c_str());
    strcpy(y.ClientPublicIP, x.client_public_ip.c_str());
    strcpy(y.ClientLoginRemark, x.client_login_remark.c_str());
    return y;
}

WechatUserSystemInfo Converter::CThostFtdcWechatUserSystemInfoFieldToRust(CThostFtdcWechatUserSystemInfoField* x) {
    if (x == nullptr)
        return WechatUserSystemInfo{.is_null = true};
    WechatUserSystemInfo y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.wechat_clt_sys_info_len = x->WechatCltSysInfoLen;
    y.wechat_clt_sys_info = Converter::Gb2312ToRustString(x->WechatCltSysInfo);
    y.client_ip_port = x->ClientIPPort;
    y.client_login_time = Converter::Gb2312ToRustString(x->ClientLoginTime);
    y.client_app_id = Converter::Gb2312ToRustString(x->ClientAppID);
    y.client_public_ip = Converter::Gb2312ToRustString(x->ClientPublicIP);
    y.client_login_remark = Converter::Gb2312ToRustString(x->ClientLoginRemark);
    return y;
}

CThostFtdcInvestorReserveInfoField Converter::InvestorReserveInfoToCpp(InvestorReserveInfo x) {
    CThostFtdcInvestorReserveInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.UserID, x.user_id.c_str());
    strcpy(y.ReserveInfo, x.reserve_info.c_str());
    return y;
}

InvestorReserveInfo Converter::CThostFtdcInvestorReserveInfoFieldToRust(CThostFtdcInvestorReserveInfoField* x) {
    if (x == nullptr)
        return InvestorReserveInfo{.is_null = true};
    InvestorReserveInfo y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.user_id = Converter::Gb2312ToRustString(x->UserID);
    y.reserve_info = Converter::Gb2312ToRustString(x->ReserveInfo);
    return y;
}

CThostFtdcQryInvestorDepartmentFlatField Converter::QryInvestorDepartmentFlatToCpp(QryInvestorDepartmentFlat x) {
    CThostFtdcQryInvestorDepartmentFlatField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    return y;
}

QryInvestorDepartmentFlat Converter::CThostFtdcQryInvestorDepartmentFlatFieldToRust(CThostFtdcQryInvestorDepartmentFlatField* x) {
    if (x == nullptr)
        return QryInvestorDepartmentFlat{.is_null = true};
    QryInvestorDepartmentFlat y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    return y;
}

CThostFtdcInvestorDepartmentFlatField Converter::InvestorDepartmentFlatToCpp(InvestorDepartmentFlat x) {
    CThostFtdcInvestorDepartmentFlatField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    strcpy(y.InvestorID, x.investor_id.c_str());
    strcpy(y.DepartmentID, x.department_id.c_str());
    return y;
}

InvestorDepartmentFlat Converter::CThostFtdcInvestorDepartmentFlatFieldToRust(CThostFtdcInvestorDepartmentFlatField* x) {
    if (x == nullptr)
        return InvestorDepartmentFlat{.is_null = true};
    InvestorDepartmentFlat y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    y.investor_id = Converter::Gb2312ToRustString(x->InvestorID);
    y.department_id = Converter::Gb2312ToRustString(x->DepartmentID);
    return y;
}

CThostFtdcQryDepartmentUserField Converter::QryDepartmentUserToCpp(QryDepartmentUser x) {
    CThostFtdcQryDepartmentUserField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.BrokerID, x.broker_id.c_str());
    return y;
}

QryDepartmentUser Converter::CThostFtdcQryDepartmentUserFieldToRust(CThostFtdcQryDepartmentUserField* x) {
    if (x == nullptr)
        return QryDepartmentUser{.is_null = true};
    QryDepartmentUser y{};
    y.broker_id = Converter::Gb2312ToRustString(x->BrokerID);
    return y;
}

CThostFtdcFrontInfoField Converter::FrontInfoToCpp(FrontInfo x) {
    CThostFtdcFrontInfoField y;
    memset(&y, 0, sizeof(y));
    strcpy(y.FrontAddr, x.front_addr.c_str());
    y.QryFreq = x.qry_freq;
    y.FTDPkgFreq = x.ftd_pkg_freq;
    return y;
}

FrontInfo Converter::CThostFtdcFrontInfoFieldToRust(CThostFtdcFrontInfoField* x) {
    if (x == nullptr)
        return FrontInfo{.is_null = true};
    FrontInfo y{};
    y.front_addr = Converter::Gb2312ToRustString(x->FrontAddr);
    y.qry_freq = x->QryFreq;
    y.ftd_pkg_freq = x->FTDPkgFreq;
    return y;
}
