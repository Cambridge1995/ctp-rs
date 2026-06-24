pub enum ResumeType {
    Restart = 0,
    Resume,
    Quick,
    NONE,
}

/// ExchangeProperty 是一个交易所属性类型
#[repr(u8)]
pub enum ExchangeProperty {
    /// 正常
    Normal = b'0',
    /// 根据成交生成报单
    GenOrderByTrade = b'1',
}
/// IdCardType 是一个证件类型类型
#[repr(u8)]
pub enum IdCardType {
    /// 组织机构代码
    EID = b'0',
    /// 中国公民身份证
    IDCard = b'1',
    /// 军官证
    OfficerIDCard = b'2',
    /// 警官证
    PoliceIDCard = b'3',
    /// 士兵证
    SoldierIDCard = b'4',
    /// 户口簿
    HouseholdRegister = b'5',
    /// 护照
    Passport = b'6',
    /// 台胞证
    TaiwanCompatriotIDCard = b'7',
    /// 回乡证
    HomeComingCard = b'8',
    /// 营业执照号
    LicenseNo = b'9',
    /// 税务登记号/当地纳税ID
    TaxNo = b'A',
    /// 港澳居民来往内地通行证
    HMMainlandTravelPermit = b'B',
    /// 台湾居民来往大陆通行证
    TwMainlandTravelPermit = b'C',
    /// 驾照
    DrivingLicense = b'D',
    /// 当地社保ID
    SocialID = b'F',
    /// 当地身份证
    LocalID = b'G',
    /// 商业登记证
    BusinessRegistration = b'H',
    /// 港澳永久性居民身份证
    HKMCIDCard = b'I',
    /// 人行开户许可证
    AccountsPermits = b'J',
    /// 外国人永久居留证
    FrgPrmtRdCard = b'K',
    /// 资管产品备案函
    CptMngPrdLetter = b'L',
    /// 港澳台居民居住证
    HKMCTwResidencePermit = b'M',
    /// 统一社会信用代码
    UniformSocialCreditCode = b'N',
    /// 机构成立证明文件
    CorporationCertNo = b'O',
    /// 其他证件
    OtherCard = b'x',
}
/// InvestorRange 是一个投资者范围类型
#[repr(u8)]
pub enum InvestorRange {
    /// 所有
    All = b'1',
    /// 投资者组
    Group = b'2',
    /// 单一投资者
    Single = b'3',
}

/// DepartmentRange 是一个部门范围类型
#[repr(u8)]
pub enum DepartmentRange {
    /// 所有
    All = b'1',
    /// 组织架构
    Group = b'2',
    /// 单一投资者
    Single = b'3',
}

/// DataSyncStatus 是一个数据同步状态类型
#[repr(u8)]
pub enum DataSyncStatus {
    /// 未同步
    Asynchronous = b'1',
    /// 同步中
    Synchronizing = b'2',
    /// 已同步
    Synchronized = b'3',
}

/// BrokerDataSyncStatus 是一个经纪公司数据同步状态类型
#[repr(u8)]
pub enum BrokerDataSyncStatus {
    /// 已同步
    Synchronized = b'1',
    /// 同步中
    Synchronizing = b'2',
}

/// ExchangeConnectStatus 是一个交易所连接状态类型
#[repr(u8)]
pub enum ExchangeConnectStatus {
    /// 没有任何连接
    NoConnection = b'1',
    /// 已经发出合约查询请求
    QryInstrumentSent = b'2',
    /// 已经获取信息
    GotInformation = b'9',
}

/// TraderConnectStatus 是一个交易所交易员连接状态类型
#[repr(u8)]
pub enum TraderConnectStatus {
    /// 没有任何连接
    NotConnected = b'1',
    /// 已经连接
    Connected = b'2',
    /// 已经发出合约查询请求
    QryInstrumentSent = b'3',
    /// 订阅私有流
    SubPrivateFlow = b'4',
}

/// FunctionCode 是一个功能代码类型
#[repr(u8)]
pub enum FunctionCode {
    /// 数据异步化
    DataAsync = b'1',
    /// 强制用户登出
    ForceUserLogout = b'2',
    /// 变更管理用户口令
    UserPasswordUpdate = b'3',
    /// 变更经纪公司口令
    BrokerPasswordUpdate = b'4',
    /// 变更投资者口令
    InvestorPasswordUpdate = b'5',
    /// 报单插入
    OrderInsert = b'6',
    /// 报单操作
    OrderAction = b'7',
    /// 同步系统数据
    SyncSystemData = b'8',
    /// 同步经纪公司数据
    SyncBrokerData = b'9',
    /// 批量同步经纪公司数据
    BachSyncBrokerData = b'A',
    /// 超级查询
    SuperQuery = b'B',
    /// 预埋报单插入
    ParkedOrderInsert = b'C',
    /// 预埋报单操作
    ParkedOrderAction = b'D',
    /// 同步动态令牌
    SyncOTP = b'E',
    /// 删除未知单
    DeleteOrder = b'F',
    /// 退出紧急状态
    ExitEmergency = b'G',
}
/// BrokerFunctionCode 是一个经纪公司功能代码类型
#[repr(u8)]
pub enum BrokerFunctionCode {
    /// 强制用户登出
    ForceUserLogout = b'1',
    /// 变更用户口令
    UserPasswordUpdate = b'2',
    /// 同步经纪公司数据
    SyncBrokerData = b'3',
    /// 批量同步经纪公司数据
    BachSyncBrokerData = b'4',
    /// 报单插入
    OrderInsert = b'5',
    /// 报单操作
    OrderAction = b'6',
    /// 全部查询
    AllQuery = b'7',
    /// 系统功能：登入/登出/修改密码等
    Log = b'a',
    /// 基本查询：查询基础数据，如合约，交易所等常量
    BaseQry = b'b',
    /// 交易查询：如查成交，委托
    TradeQry = b'c',
    /// 交易功能：报单，撤单
    Trade = b'd',
    /// 银期转账
    Virement = b'e',
    /// 风险监控
    Risk = b'f',
    /// 查询/管理：查询会话，踢人等
    Session = b'g',
    /// 风控通知控制
    RiskNoticeCtl = b'h',
    /// 风控通知发送
    RiskNotice = b'i',
    /// 察看经纪公司资金权限
    BrokerDeposit = b'j',
    /// 资金查询
    QueryFund = b'k',
    /// 报单查询
    QueryOrder = b'l',
    /// 成交查询
    QueryTrade = b'm',
    /// 持仓查询
    QueryPosition = b'n',
    /// 行情查询
    QueryMarketData = b'o',
    /// 用户事件查询
    QueryUserEvent = b'p',
    /// 风险通知查询
    QueryRiskNotify = b'q',
    /// 出入金查询
    QueryFundChange = b'r',
    /// 投资者信息查询
    QueryInvestor = b's',
    /// 交易编码查询
    QueryTradingCode = b't',
    /// 强平
    ForceClose = b'u',
    /// 压力测试
    PressTest = b'v',
    /// 权益反算
    RemainCalc = b'w',
    /// 净持仓保证金指标
    NetPositionInd = b'x',
    /// 风险预算
    RiskPredict = b'y',
    /// 数据导出
    DataExport = b'z',
    /// 风控指标设置
    RiskTargetSetup = b'A',
    /// 行情预警
    MarketDataWarn = b'B',
    /// 业务通知查询
    QryBizNotice = b'C',
    /// 业务通知模板设置
    CfgBizNotice = b'D',
    /// 同步动态令牌
    SyncOTP = b'E',
    /// 发送业务通知
    SendBizNotice = b'F',
    /// 风险级别标准设置
    CfgRiskLevelStd = b'G',
    /// 交易终端应急功能
    TbCommand = b'H',
    /// 删除未知单
    DeleteOrder = b'J',
    /// 预埋报单插入
    ParkedOrderInsert = b'K',
    /// 预埋报单操作
    ParkedOrderAction = b'L',
    /// 资金不够仍允许行权
    ExecOrderNoCheck = b'M',
    /// 指定
    Designate = b'N',
    /// 证券处置
    StockDisposal = b'O',
    /// 席位资金预警
    BrokerDepositWarn = b'Q',
    /// 备兑不足预警
    CoverWarn = b'S',
    /// 行权试算
    PreExecOrder = b'T',
    /// 行权交收风险
    ExecOrderRisk = b'P',
    /// 持仓限额预警
    PosiLimitWarn = b'U',
    /// 持仓限额查询
    QryPosiLimit = b'V',
    /// 银期签到签退
    FBSign = b'W',
    /// 银期签约解约
    FBAccount = b'X',
}
/// OrderActionStatus 是一个报单操作状态类型
#[repr(u8)]
pub enum OrderActionStatus {
    /// 已经提交
    Submitted = b'a',
    /// 已经接受
    Accepted = b'b',
    /// 已经被拒绝
    Rejected = b'c',
}
/// OrderStatus 是一个报单状态类型
#[repr(u8)]
pub enum OrderStatus {
    /// 全部成交
    AllTraded = b'0',
    /// 部分成交还在队列中
    PartTradedQueueing = b'1',
    /// 部分成交不在队列中
    PartTradedNotQueueing = b'2',
    /// 未成交还在队列中
    NoTradeQueueing = b'3',
    /// 未成交不在队列中
    NoTradeNotQueueing = b'4',
    /// 撤单
    Canceled = b'5',
    /// 未知
    Unknown = b'a',
    /// 尚未触发
    NotTouched = b'b',
    /// 已触发
    Touched = b'c',
}
/// OrderSubmitStatus 是一个报单提交状态类型
#[repr(u8)]
pub enum OrderSubmitStatus {
    /// 已经提交
    InsertSubmitted = b'0',
    /// 撤单已经提交
    CancelSubmitted = b'1',
    /// 修改已经提交
    ModifySubmitted = b'2',
    /// 已经接受
    Accepted = b'3',
    /// 报单已经被拒绝
    InsertRejected = b'4',
    /// 撤单已经被拒绝
    CancelRejected = b'5',
    /// 改单已经被拒绝
    ModifyRejected = b'6',
}
/// PositionDate 是一个持仓日期类型
#[repr(u8)]
pub enum PositionDate {
    /// 今日持仓
    Today = b'1',
    /// 历史持仓
    History = b'2',
}
/// PositionDateType 是一个持仓日期类型类型
#[repr(u8)]
pub enum PositionDateType {
    /// 使用历史持仓
    UseHistory = b'1',
    /// 不使用历史持仓
    NoUseHistory = b'2',
}
/// TradingRole 是一个交易角色类型
#[repr(u8)]
pub enum TradingRole {
    /// 代理
    Broker = b'1',
    /// 自营
    Host = b'2',
    /// 做市商
    Maker = b'3',
}
/// ProductClass 是一个产品类型类型
#[repr(u8)]
pub enum ProductClass {
    /// 期货
    Futures = b'1',
    /// 期货期权
    Options = b'2',
    /// 组合
    Combination = b'3',
    /// 即期
    Spot = b'4',
    /// 期转现
    EFP = b'5',
    /// 现货期权
    SpotOption = b'6',
    /// TAS合约
    TAS = b'7',
    /// 金属指数
    MI = b'I',
}
/// APIProductClass 是一个产品类型类型
#[repr(u8)]
pub enum APIProductClass {
    /// 期货单一合约
    FutureSingle = b'1',
    /// 期权单一合约
    OptionSingle = b'2',
    /// 可交易期货(含期货组合和期货单一合约)
    Futures = b'3',
    /// 可交易期权(含期权组合和期权单一合约)
    Options = b'4',
    /// 可下单套利组合
    TradingComb = b'5',
    /// 可申请的组合（可以申请的组合合约 包含可以交易的合约）
    UnTradingComb = b'6',
    /// 所有可以交易合约
    AllTrading = b'7',
    /// 所有合约（包含不能交易合约 慎用）
    All = b'8',
}
/// InstLifePhase 是一个合约生命周期状态类型
#[repr(u8)]
pub enum InstLifePhase {
    /// 未上市
    NotStart = b'0',
    /// 上市
    Started = b'1',
    /// 停牌
    Pause = b'2',
    /// 到期
    Expired = b'3',
}
/// Direction 是一个买卖方向类型
#[repr(u8)]
pub enum Direction {
    /// 买
    Buy = b'0',
    /// 卖
    Sell = b'1',
}
/// PositionType 是一个持仓类型类型
#[repr(u8)]
pub enum PositionType {
    /// 净持仓
    Net = b'1',
    /// 综合持仓
    Gross = b'2',
}
/// PosiDirection 是一个持仓多空方向类型
#[repr(u8)]
pub enum PosiDirection {
    /// 净
    Net = b'1',
    /// 多头
    Long = b'2',
    /// 空头
    Short = b'3',
}
/// SysSettlementStatus 是一个系统结算状态类型
#[repr(u8)]
pub enum SysSettlementStatus {
    /// 不活跃
    NonActive = b'1',
    /// 启动
    Startup = b'2',
    /// 操作
    Operating = b'3',
    /// 结算
    Settlement = b'4',
    /// 结算完成
    SettlementFinished = b'5',
}
/// RatioAttr 是一个费率属性类型
#[repr(u8)]
pub enum RatioAttr {
    /// 交易费率
    Trade = b'0',
    /// 结算费率
    Settlement = b'1',
}
/// HedgeFlag 是一个投机套保标志类型
#[repr(u8)]
pub enum HedgeFlag {
    /// 投机
    Speculation = b'1',
    /// 套利
    Arbitrage = b'2',
    /// 套保
    Hedge = b'3',
    /// 做市商
    MarketMaker = b'5',
    /// 第一腿投机第二腿套保
    SpecHedge = b'6',
    /// 第一腿套保第二腿投机
    HedgeSpec = b'7',
}
/// BillHedgeFlag 是一个投机套保标志类型
#[repr(u8)]
pub enum BillHedgeFlag {
    /// 投机
    Speculation = b'1',
    /// 套利
    Arbitrage = b'2',
    /// 套保
    Hedge = b'3',
}
/// ClientIDType 是一个交易编码类型类型
#[repr(u8)]
pub enum ClientIDType {
    /// 投机
    Speculation = b'1',
    /// 套利
    Arbitrage = b'2',
    /// 套保
    Hedge = b'3',
    /// 做市商
    MarketMaker = b'5',
}
/// OrderPriceType 是一个报单价格条件类型
#[repr(u8)]
pub enum OrderPriceType {
    /// 任意价
    AnyPrice = b'1',
    /// 限价
    LimitPrice = b'2',
    /// 最优价
    BestPrice = b'3',
    /// 最新价
    LastPrice = b'4',
    /// 最新价浮动上浮1个ticks
    LastPricePlusOneTicks = b'5',
    /// 最新价浮动上浮2个ticks
    LastPricePlusTwoTicks = b'6',
    /// 最新价浮动上浮3个ticks
    LastPricePlusThreeTicks = b'7',
    /// 卖一价
    AskPrice1 = b'8',
    /// 卖一价浮动上浮1个ticks
    AskPrice1PlusOneTicks = b'9',
    /// 卖一价浮动上浮2个ticks
    AskPrice1PlusTwoTicks = b'A',
    /// 卖一价浮动上浮3个ticks
    AskPrice1PlusThreeTicks = b'B',
    /// 买一价
    BidPrice1 = b'C',
    /// 买一价浮动上浮1个ticks
    BidPrice1PlusOneTicks = b'D',
    /// 买一价浮动上浮2个ticks
    BidPrice1PlusTwoTicks = b'E',
    /// 买一价浮动上浮3个ticks
    BidPrice1PlusThreeTicks = b'F',
    /// 五档价
    FiveLevelPrice = b'G',
}
/// OffsetFlag 是一个开平标志类型
#[repr(u8)]
pub enum OffsetFlag {
    /// 开仓
    Open = b'0',
    /// 平仓
    Close = b'1',
    /// 强平
    ForceClose = b'2',
    /// 平今
    CloseToday = b'3',
    /// 平昨
    CloseYesterday = b'4',
    /// 强减
    ForceOff = b'5',
    /// 本地强平
    LocalForceClose = b'6',
}
/// ForceCloseReason 是一个强平原因类型
#[repr(u8)]
pub enum ForceCloseReason {
    /// 非强平
    NotForceClose = b'0',
    /// 资金不足
    LackDeposit = b'1',
    /// 客户超仓
    ClientOverPositionLimit = b'2',
    /// 会员超仓
    MemberOverPositionLimit = b'3',
    /// 持仓非整数倍
    NotMultiple = b'4',
    /// 违规
    Violation = b'5',
    /// 其它
    Other = b'6',
    /// 自然人临近交割
    PersonDelivery = b'7',
    /// 本地强平资金不足,忽略敞口
    NotVerifyCapital = b'8',
    /// 本地强平资金不足
    LocalLackDeposit = b'9',
    /// 本地强平违规持仓忽略敞口
    LocalViolationNocheck = b'a',
    /// 本地强平违规持仓
    LocalViolation = b'b',
}
/// OrderType 是一个报单类型类型
#[repr(u8)]
pub enum OrderType {
    /// 正常
    Normal = b'0',
    /// 报价衍生
    DeriveFromQuote = b'1',
    /// 组合衍生
    DeriveFromCombination = b'2',
    /// 组合报单
    Combination = b'3',
    /// 条件单
    ConditionalOrder = b'4',
    /// 互换单
    Swap = b'5',
    /// 大宗交易成交衍生
    DeriveFromBlockTrade = b'6',
    /// 期转现成交衍生
    DeriveFromEFPTrade = b'7',
}
/// TimeCondition 是一个有效期类型类型
#[repr(u8)]
pub enum TimeCondition {
    /// 立即完成，否则撤销
    IOC = b'1',
    /// 本节有效
    GFS = b'2',
    /// 当日有效
    GFD = b'3',
    /// 指定日期前有效
    GTD = b'4',
    /// 撤销前有效
    GTC = b'5',
    /// 集合竞价有效
    GFA = b'6',
}
/// VolumeCondition 是一个成交量类型类型
#[repr(u8)]
pub enum VolumeCondition {
    /// 任何数量
    AV = b'1',
    /// 最小数量
    MV = b'2',
    /// 全部数量
    CV = b'3',
}
/// ContingentCondition 是一个触发条件类型
#[repr(u8)]
pub enum ContingentCondition {
    /// 立即
    Immediately = b'1',
    /// 止损
    Touch = b'2',
    /// 止赢
    TouchProfit = b'3',
    /// 预埋单
    ParkedOrder = b'4',
    /// 最新价大于条件价
    LastPriceGreaterThanStopPrice = b'5',
    /// 最新价大于等于条件价
    LastPriceGreaterEqualStopPrice = b'6',
    /// 最新价小于条件价
    LastPriceLesserThanStopPrice = b'7',
    /// 最新价小于等于条件价
    LastPriceLesserEqualStopPrice = b'8',
    /// 卖一价大于条件价
    AskPriceGreaterThanStopPrice = b'9',
    /// 卖一价大于等于条件价
    AskPriceGreaterEqualStopPrice = b'A',
    /// 卖一价小于条件价
    AskPriceLesserThanStopPrice = b'B',
    /// 卖一价小于等于条件价
    AskPriceLesserEqualStopPrice = b'C',
    /// 买一价大于条件价
    BidPriceGreaterThanStopPrice = b'D',
    /// 买一价大于等于条件价
    BidPriceGreaterEqualStopPrice = b'E',
    /// 买一价小于条件价
    BidPriceLesserThanStopPrice = b'F',
    /// 买一价小于等于条件价
    BidPriceLesserEqualStopPrice = b'H',
}
/// ActionFlag 是一个操作标志类型
#[repr(u8)]
pub enum ActionFlag {
    /// 删除
    Delete = b'0',
    /// 修改
    Modify = b'3',
}
/// TradingRight 是一个交易权限类型
#[repr(u8)]
pub enum TradingRight {
    /// 可以交易
    Allow = b'0',
    /// 只能平仓
    CloseOnly = b'1',
    /// 不能交易
    Forbidden = b'2',
}
/// OrderSource 是一个报单来源类型
#[repr(u8)]
pub enum OrderSource {
    /// 来自参与者
    Participant = b'0',
    /// 来自管理员
    Administrator = b'1',
}
/// TradeType 是一个成交类型类型
#[repr(u8)]
pub enum TradeType {
    /// 组合持仓拆分为单一持仓,初始化不应包含该类型的持仓
    SplitCombination = b'#',
    /// 普通成交
    Common = b'0',
    /// 期权执行
    OptionsExecution = b'1',
    /// OTC成交
    OTC = b'2',
    /// 期转现衍生成交
    EFPDerived = b'3',
    /// 组合衍生成交
    CombinationDerived = b'4',
    /// 大宗交易成交
    BlockTrade = b'5',
}
/// SpecPosiType 是一个特殊持仓明细标识类型
#[repr(u8)]
pub enum SpecPosiType {
    /// 普通持仓明细
    Common = b'#',
    /// TAS合约成交产生的标的合约持仓明细
    Tas = b'0',
}
/// PriceSource 是一个成交价来源类型
#[repr(u8)]
pub enum PriceSource {
    /// 前成交价
    LastPrice = b'0',
    /// 买委托价
    Buy = b'1',
    /// 卖委托价
    Sell = b'2',
    /// 场外成交价
    OTC = b'3',
}
/// InstrumentStatusType 是一个合约交易状态类型
#[repr(u8)]
pub enum InstrumentStatusType {
    /// 开盘前
    BeforeTrading = b'0',
    /// 非交易
    NoTrading = b'1',
    /// 连续交易
    Continuous = b'2',
    /// 集合竞价报单
    AuctionOrdering = b'3',
    /// 集合竞价价格平衡
    AuctionBalance = b'4',
    /// 集合竞价撮合
    AuctionMatch = b'5',
    /// 收盘
    Closed = b'6',
    /// 交易业务处理
    TransactionProcessing = b'7',
}

/// InstStatusEnterReason 是一个品种进入交易状态原因类型
#[repr(u8)]
pub enum InstStatusEnterReason {
    /// 自动切换
    Automatic = b'1',
    /// 手动切换
    Manual = b'2',
    /// 熔断
    Fuse = b'3',
}

/// BatchStatus 是一个处理状态类型
#[repr(u8)]
pub enum BatchStatus {
    /// 未上传
    NoUpload = b'1',
    /// 已上传
    Uploaded = b'2',
    /// 审核失败
    Failed = b'3',
}

/// ReturnStyle 是一个按品种返还方式类型
#[repr(u8)]
pub enum ReturnStyle {
    /// 按所有品种
    All = b'1',
    /// 按品种
    ByProduct = b'2',
}

/// ReturnPattern 是一个返还模式类型
#[repr(u8)]
pub enum ReturnPattern {
    /// 按成交手数
    ByVolume = b'1',
    /// 按留存手续费
    ByFeeOnHand = b'2',
}

/// ReturnLevel 是一个返还级别类型
#[repr(u8)]
pub enum ReturnLevel {
    /// 级别1
    Level1 = b'1',
    /// 级别2
    Level2 = b'2',
    /// 级别3
    Level3 = b'3',
    /// 级别4
    Level4 = b'4',
    /// 级别5
    Level5 = b'5',
    /// 级别6
    Level6 = b'6',
    /// 级别7
    Level7 = b'7',
    /// 级别8
    Level8 = b'8',
    /// 级别9
    Level9 = b'9',
}

/// ReturnStandard 是一个返还标准类型
#[repr(u8)]
pub enum ReturnStandard {
    /// 分阶段返还
    ByPeriod = b'1',
    /// 按某一标准
    ByStandard = b'2',
}

/// MortgageType 是一个质押类型类型
#[repr(u8)]
pub enum MortgageType {
    /// 质出
    Out = b'0',
    /// 质入
    In = b'1',
}
/// InvestorSettlementParamID 是一个投资者结算参数代码类型
#[repr(u8)]
pub enum InvestorSettlementParamID {
    /// 质押比例
    MortgageRatio = b'4',
    /// 保证金算法
    MarginWay = b'5',
    /// 结算单结存是否包含质押
    BillDeposit = b'9',
}
/// ExchangeSettlementParamID 是一个交易所结算参数代码类型
#[repr(u8)]
pub enum ExchangeSettlementParamID {
    /// 质押比例
    MortgageRatio = b'1',
    /// 分项资金导入项
    OtherFundItem = b'2',
    /// 分项资金入交易所出入金
    OtherFundImport = b'3',
    /// 中金所开户最低可用金额
    CFFEXMinPrepay = b'6',
    /// 郑商所结算方式
    CZCESettlementType = b'7',
    /// 交易所交割手续费收取方式
    ExchDeliveryFeeMode = b'9',
    /// 投资者交割手续费收取方式
    DeliveryFeeMode = b'0',
    /// 郑商所组合持仓保证金收取方式
    CZCEComMarginType = b'A',
    /// 大商所套利保证金是否优惠
    DceComMarginType = b'B',
    /// 虚值期权保证金优惠比率
    OptOutDisCountRate = b'a',
    /// 最低保障系数
    OptMiniGuarantee = b'b',
}
/// SystemParamID 是一个系统参数代码类型
#[repr(u8)]
pub enum SystemParamID {
    /// 投资者代码最小长度
    InvestorIDMinLength = b'1',
    /// 投资者帐号代码最小长度
    AccountIDMinLength = b'2',
    /// 投资者开户默认登录权限
    UserRightLogon = b'3',
    /// 投资者交易结算单成交汇总方式
    SettlementBillTrade = b'4',
    /// 统一开户更新交易编码方式
    TradingCode = b'5',
    /// 结算是否判断存在未复核的出入金和分项资金
    CheckFund = b'6',
    /// 是否启用手续费模板数据权限
    CommModelRight = b'7',
    /// 是否启用保证金率模板数据权限
    MarginModelRight = b'9',
    /// 是否规范用户才能激活
    IsStandardActive = b'8',
    /// 上传的交易所结算文件路径
    UploadSettlementFile = b'U',
    /// 上报保证金监控中心文件路径
    DownloadCSRCFile = b'D',
    /// 生成的结算单文件路径
    SettlementBillFile = b'S',
    /// 证监会文件标识
    CSRCOthersFile = b'C',
    /// 投资者照片路径
    InvestorPhoto = b'P',
    /// 全结经纪公司上传文件路径
    CSRCData = b'R',
    /// 开户密码录入方式
    InvestorPwdModel = b'I',
    /// 投资者中金所结算文件下载路径
    CFFEXInvestorSettleFile = b'F',
    /// 投资者代码编码方式
    InvestorIDType = b'a',
    /// 休眠户最高权益
    FreezeMaxReMain = b'r',
    /// 手续费相关操作实时上场开关
    IsSync = b'A',
    /// 解除开仓权限限制
    RelieveOpenLimit = b'O',
    /// 是否规范用户才能休眠
    IsStandardFreeze = b'X',
    /// 郑商所是否开放所有品种套保交易
    CZCENormalProductHedge = b'B',
}
/// TradeParamID 是一个交易系统参数代码类型
#[repr(u8)]
pub enum TradeParamID {
    /// 系统加密算法
    EncryptionStandard = b'E',
    /// 系统风险算法
    RiskMode = b'R',
    /// 系统风险算法是否全局 0-否 1-是
    RiskModeGlobal = b'G',
    /// 密码加密算法
    ModeEncode = b'P',
    /// 价格小数位数参数
    TickMode = b'T',
    /// 用户最大会话数
    SingleUserSessionMaxNum = b'S',
    /// 最大连续登录失败数
    LoginFailMaxNum = b'L',
    /// 是否强制认证
    IsAuthForce = b'A',
    /// 是否冻结证券持仓
    IsPosiFreeze = b'F',
    /// 是否限仓
    IsPosiLimit = b'M',
    /// 郑商所询价时间间隔
    ForQuoteTimeInterval = b'Q',
    /// 是否期货限仓
    IsFuturePosiLimit = b'B',
    /// 是否期货下单频率限制
    IsFutureOrderFreq = b'C',
    /// 行权冻结是否计算盈利
    IsExecOrderProfit = b'H',
    /// 银期开户是否验证开户银行卡号是否是预留银行账户
    IsCheckBankAcc = b'I',
    /// 弱密码最后修改日期
    PasswordDeadLine = b'J',
    /// 强密码校验
    IsStrongPassword = b'K',
    /// 自有资金质押比
    BalanceMortgage = b'a',
    /// 最小密码长度
    MinPwdLen = b'O',
    /// IP当日最大登陆失败次数
    LoginFailMaxNumForIP = b'U',
    /// 密码有效期
    PasswordPeriod = b'V',
    /// 历史密码重复限制次数
    PwdHistoryCmp = b'X',
    /// 转账是否验证预留银行账户
    TransferChkProperty = b'i',
    /// 非交易时间异常报单校验参数
    TradeChkPhase = b'j',
    /// 其他异常报单校验参数（价格和手数）
    TradeChkPriceVol = b'k',
    /// 卖出垂直价差组合新算法
    NewBESMarginAlgo = b'l',
}
/// FileID 是一个文件标识类型
#[repr(u8)]
pub enum FileID {
    /// 资金数据
    SettlementFund = b'F',
    /// 成交数据
    Trade = b'T',
    /// 投资者持仓数据
    InvestorPosition = b'P',
    /// 投资者分项资金数据
    SubEntryFund = b'O',
    /// 组合持仓数据
    CZCECombinationPos = b'C',
    /// 上报保证金监控中心数据
    CSRCData = b'R',
    /// 郑商所平仓了结数据
    CZCEClose = b'L',
    /// 郑商所非平仓了结数据
    CZCENoClose = b'N',
    /// 持仓明细数据
    PositionDtl = b'D',
    /// 期权执行文件
    OptionStrike = b'S',
    /// 结算价比对文件
    SettlementPriceComparison = b'M',
    /// 上期所非持仓变动明细
    NonTradePosChange = b'B',
}
/// FileType 是一个文件上传类型类型
#[repr(u8)]
pub enum FileType {
    /// 结算
    Settlement = b'0',
    /// 核对
    Check = b'1',
}
/// FileFormat 是一个文件格式类型
#[repr(u8)]
pub enum FileFormat {
    /// 文本文件(.txt)
    Txt = b'0',
    /// 压缩文件(.zip)
    Zip = b'1',
    /// DBF文件(.dbf)
    DBF = b'2',
}
/// FileUploadStatus 是一个文件状态类型
#[repr(u8)]
pub enum FileUploadStatus {
    /// 上传成功
    SucceedUpload = b'1',
    /// 上传失败
    FailedUpload = b'2',
    /// 导入成功
    SucceedLoad = b'3',
    /// 导入部分成功
    PartSucceedLoad = b'4',
    /// 导入失败
    FailedLoad = b'5',
}
/// TransferDirection 是一个移仓方向类型
#[repr(u8)]
pub enum TransferDirection {
    /// 移出
    Out = b'0',
    /// 移入
    In = b'1',
}
/// SpecialCreateRule 是一个特殊的创建规则类型
#[repr(u8)]
pub enum SpecialCreateRule {
    /// 没有特殊创建规则
    NoSpecialRule = b'0',
    /// 不包含春节
    NoSpringFestival = b'1',
}
/// BasisPriceType 是一个挂牌基准价类型类型
#[repr(u8)]
pub enum BasisPriceType {
    /// 上一合约结算价
    LastSettlement = b'1',
    /// 上一合约收盘价
    LaseClose = b'2',
}
/// ProductLifePhase 是一个产品生命周期状态类型
#[repr(u8)]
pub enum ProductLifePhase {
    /// 活跃
    Active = b'1',
    /// 不活跃
    NonActive = b'2',
    /// 注销
    Canceled = b'3',
}
/// DeliveryMode 是一个交割方式类型
#[repr(u8)]
pub enum DeliveryMode {
    /// 现金交割
    Cash = b'1',
    /// 实物交割
    Commodity = b'2',
}
/// FundIOType 是一个出入金类型类型
#[repr(u8)]
pub enum FundIOType {
    /// 出入金
    FundIO = b'1',
    /// 银期转帐
    Transfer = b'2',
    /// 银期换汇
    SwapCurrency = b'3',
}
/// FundType 是一个资金类型类型
#[repr(u8)]
pub enum FundType {
    /// 银行存款
    Deposit = b'1',
    /// 分项资金
    ItemFund = b'2',
    /// 公司调整
    Company = b'3',
    /// 资金内转
    InnerTransfer = b'4',
}
/// FundDirection 是一个出入金方向类型
#[repr(u8)]
pub enum FundDirection {
    /// 入金
    In = b'1',
    /// 出金
    Out = b'2',
}
/// FundStatus 是一个资金状态类型
#[repr(u8)]
pub enum FundStatus {
    /// 已录入
    Record = b'1',
    /// 已复核
    Check = b'2',
    /// 已冲销
    Charge = b'3',
}
/// PublishStatus 是一个发布状态类型
#[repr(u8)]
pub enum PublishStatus {
    /// 未发布
    None = b'1',
    /// 正在发布
    Publishing = b'2',
    /// 已发布
    Published = b'3',
}
/// SystemStatus 是一个系统状态类型
#[repr(u8)]
pub enum SystemStatus {
    /// 不活跃
    NonActive = b'1',
    /// 启动
    Startup = b'2',
    /// 交易开始初始化
    Initialize = b'3',
    /// 交易完成初始化
    Initialized = b'4',
    /// 收市开始
    Close = b'5',
    /// 收市完成
    Closed = b'6',
    /// 结算
    Settlement = b'7',
}
/// SettlementStatus 是一个结算状态类型
#[repr(u8)]
pub enum SettlementStatus {
    /// 初始
    Initialize = b'0',
    /// 结算中
    Settling = b'1',
    /// 已结算
    Settled = b'2',
    /// 结算完成
    Finished = b'3',
}
/// InvestorType 是一个投资者类型类型
#[repr(u8)]
pub enum InvestorType {
    /// 自然人
    Person = b'0',
    /// 法人
    Company = b'1',
    /// 投资基金
    Fund = b'2',
    /// 特殊法人
    SpecialOrgan = b'3',
    /// 资管户
    Asset = b'4',
}
/// BrokerType 是一个经纪公司类型类型
#[repr(u8)]
pub enum BrokerType {
    /// 交易会员
    Trade = b'0',
    /// 交易结算会员
    TradeSettle = b'1',
}
/// RiskLevel 是一个风险等级类型
#[repr(u8)]
pub enum RiskLevel {
    /// 低风险客户
    Low = b'1',
    /// 普通客户
    Normal = b'2',
    /// 关注客户
    Focus = b'3',
    /// 风险客户
    Risk = b'4',
}
/// FeeAcceptStyle 是一个手续费收取方式类型
#[repr(u8)]
pub enum FeeAcceptStyle {
    /// 按交易收取
    ByTrade = b'1',
    /// 按交割收取
    ByDelivery = b'2',
    /// 不收
    None = b'3',
    /// 按指定手续费收取
    FixFee = b'4',
}
/// PasswordType 是一个密码类型类型
#[repr(u8)]
pub enum PasswordType {
    /// 交易密码
    Trade = b'1',
    /// 资金密码
    Account = b'2',
}
/// Algorithm 是一个盈亏算法类型
#[repr(u8)]
pub enum Algorithm {
    /// 浮盈浮亏都计算
    All = b'1',
    /// 浮盈不计，浮亏计
    OnlyLost = b'2',
    /// 浮盈计，浮亏不计
    OnlyGain = b'3',
    /// 浮盈浮亏都不计算
    None = b'4',
}
/// IncludeCloseProfit 是一个是否包含平仓盈利类型
#[repr(u8)]
pub enum IncludeCloseProfit {
    /// 包含平仓盈利
    Include = b'0',
    /// 不包含平仓盈利
    NotInclude = b'2',
}
/// AllWithoutTrade 是一个是否受可提比例限制类型
#[repr(u8)]
pub enum AllWithoutTrade {
    /// 无仓无成交不受可提比例限制
    Enable = b'0',
    /// 受可提比例限制
    Disable = b'2',
    /// 无仓不受可提比例限制
    NoHoldEnable = b'3',
}
/// FuturePwdFlag 是一个资金密码核对标志类型
#[repr(u8)]
pub enum FuturePwdFlag {
    /// 不核对
    UnCheck = b'0',
    /// 核对
    Check = b'1',
}
/// TransferType 是一个银期转账类型类型
#[repr(u8)]
pub enum TransferType {
    /// 银行转期货
    BankToFuture = b'0',
    /// 期货转银行
    FutureToBank = b'1',
}
/// TransferValidFlag 是一个转账有效标志类型
#[repr(u8)]
pub enum TransferValidFlag {
    /// 无效或失败
    Invalid = b'0',
    /// 有效
    Valid = b'1',
    /// 冲正
    Reverse = b'2',
}
/// Reason 是一个事由类型
#[repr(u8)]
pub enum Reason {
    /// 错单
    CD = b'0',
    /// 资金在途
    ZT = b'1',
    /// 其它
    QT = b'2',
}
/// Sex 是一个性别类型
#[repr(u8)]
pub enum Sex {
    /// 未知
    None_ = b'0',
    /// 男
    Man = b'1',
    /// 女
    Woman = b'2',
}
/// UserType 是一个用户类型类型
#[repr(u8)]
pub enum UserType {
    /// 投资者
    Investor = b'0',
    /// 操作员
    Operator = b'1',
    /// 管理员
    SuperUser = b'2',
}
/// RateType 是一个费率类型类型
#[repr(u8)]
pub enum RateType {
    /// 保证金率
    MarginRate = b'2',
}
/// NoteType 是一个通知类型类型
#[repr(u8)]
pub enum NoteType {
    /// 交易结算单
    TradeSettleBill = b'1',
    /// 交易结算月报
    TradeSettleMonth = b'2',
    /// 追加保证金通知书
    CallMarginNotes = b'3',
    /// 强行平仓通知书
    ForceCloseNotes = b'4',
    /// 成交通知书
    TradeNotes = b'5',
    /// 交割通知书
    DeliveryNotes = b'6',
}
/// SettlementStyle 是一个结算单方式类型
#[repr(u8)]
pub enum SettlementStyle {
    /// 逐日盯市
    Day = b'1',
    /// 逐笔对冲
    Volume = b'2',
}
/// SettlementBillType 是一个结算单类型类型
#[repr(u8)]
pub enum SettlementBillType {
    /// 日报
    Day = b'0',
    /// 月报
    Month = b'1',
}
/// UserRightType 是一个客户权限类型类型
#[repr(u8)]
pub enum UserRightType {
    /// 登录
    Logon = b'1',
    /// 银期转帐
    Transfer = b'2',
    /// 邮寄结算单
    EMail = b'3',
    /// 传真结算单
    Fax = b'4',
    /// 条件单
    ConditionOrder = b'5',
}
/// MarginPriceType 是一个保证金价格类型类型
#[repr(u8)]
pub enum MarginPriceType {
    /// 昨结算价
    PreSettlementPrice = b'1',
    /// 最新价
    SettlementPrice = b'2',
    /// 成交均价
    AveragePrice = b'3',
    /// 开仓价
    OpenPrice = b'4',
}
/// BillGenStatus 是一个结算单生成状态类型
#[repr(u8)]
pub enum BillGenStatus {
    /// 未生成
    None = b'0',
    /// 生成中
    NoGenerated = b'1',
    /// 已生成
    Generated = b'2',
}
/// AlgoType 是一个算法类型类型
#[repr(u8)]
pub enum AlgoType {
    /// 持仓处理算法
    HandlePositionAlgo = b'1',
    /// 寻找保证金率算法
    FindMarginRateAlgo = b'2',
}
/// HandlePositionAlgoID 是一个持仓处理算法编号类型
#[repr(u8)]
pub enum HandlePositionAlgoID {
    /// 基本
    Base = b'1',
    /// 大连商品交易所
    DCE = b'2',
    /// 郑州商品交易所
    CZCE = b'3',
}
/// FindMarginRateAlgoID 是一个寻找保证金率算法编号类型
#[repr(u8)]
pub enum FindMarginRateAlgoID {
    /// 基本
    Base = b'1',
    /// 大连商品交易所
    DCE = b'2',
    /// 郑州商品交易所
    CZCE = b'3',
}
/// HandleTradingAccountAlgoID 是一个资金处理算法编号类型
#[repr(u8)]
pub enum HandleTradingAccountAlgoID {
    /// 基本
    Base = b'1',
    /// 大连商品交易所
    DCE = b'2',
    /// 郑州商品交易所
    CZCE = b'3',
}
/// PersonType 是一个联系人类型类型
#[repr(u8)]
pub enum PersonType {
    /// 指定下单人
    Order = b'1',
    /// 开户授权人
    Open = b'2',
    /// 资金调拨人
    Fund = b'3',
    /// 结算单确认人
    Settlement = b'4',
    /// 法人
    Company = b'5',
    /// 法人代表
    Corporation = b'6',
    /// 投资者联系人
    LinkMan = b'7',
    /// 分户管理资产负责人
    Ledger = b'8',
    /// 托（保）管人
    Trustee = b'9',
    /// 托（保）管机构法人代表
    TrusteeCorporation = b'A',
    /// 托（保）管机构开户授权人
    TrusteeOpen = b'B',
    /// 托（保）管机构联系人
    TrusteeContact = b'C',
    /// 境外自然人参考证件
    ForeignerRefer = b'D',
    /// 法人代表参考证件
    CorporationRefer = b'E',
}
/// QueryInvestorRange 是一个查询范围类型
#[repr(u8)]
pub enum QueryInvestorRange {
    /// 所有
    All = b'1',
    /// 查询分类
    Group = b'2',
    /// 单一投资者
    Single = b'3',
}
/// InvestorRiskStatus 是一个投资者风险状态类型
#[repr(u8)]
pub enum InvestorRiskStatus {
    /// 正常
    Normal = b'1',
    /// 警告
    Warn = b'2',
    /// 追保
    Call = b'3',
    /// 强平
    Force = b'4',
    /// 异常
    Exception = b'5',
}
/// UserEventType 是一个用户事件类型类型
#[repr(u8)]
pub enum UserEventType {
    /// 登录
    Login = b'1',
    /// 登出
    Logout = b'2',
    /// CTP校验通过
    Trading = b'3',
    /// CTP校验失败
    TradingError = b'4',
    /// 修改密码
    UpdatePassword = b'5',
    /// 客户端认证
    Authenticate = b'6',
    /// 终端信息上报
    SubmitSysInfo = b'7',
    /// 转账
    Transfer = b'8',
    /// 其他
    Other = b'9',
    /// 修改资金密码
    UpdateTradingAccountPassword = b'a',
}
/// CloseStyle 是一个平仓方式类型
#[repr(u8)]
pub enum CloseStyle {
    /// 先开先平
    Close = b'0',
    /// 先平今再平昨
    CloseToday = b'1',
}
/// StatMode 是一个统计方式类型
#[repr(u8)]
pub enum StatMode {
    /// ----
    Non = b'0',
    /// 按合约统计
    Instrument = b'1',
    /// 按产品统计
    Product = b'2',
    /// 按投资者统计
    Investor = b'3',
}
/// ParkedOrderStatus 是一个预埋单状态类型
#[repr(u8)]
pub enum ParkedOrderStatus {
    /// 未发送
    NotSend = b'1',
    /// 已发送
    Send = b'2',
    /// 已删除
    Deleted = b'3',
}
/// VirDealStatus 是一个处理状态类型
#[repr(u8)]
pub enum VirDealStatus {
    /// 正在处理
    Dealing = b'1',
    /// 处理成功
    DealSucceed = b'2',
}
/// OrgSystemID 是一个原有系统代码类型
#[repr(u8)]
pub enum OrgSystemID {
    /// 综合交易平台
    Standard = b'0',
    /// 易盛系统
    ESunny = b'1',
    /// 金仕达V6系统
    KingStarV6 = b'2',
}
/// VirTradeStatus 是一个交易状态类型
#[repr(u8)]
pub enum VirTradeStatus {
    /// 正常处理中
    NaturalDeal = b'0',
    /// 成功结束
    SucceedEnd = b'1',
    /// 失败结束
    FailedEND = b'2',
    /// 异常中
    Exception = b'3',
    /// 已人工异常处理
    ManualDeal = b'4',
    /// 通讯异常 ，请人工处理
    MesException = b'5',
    /// 系统出错，请人工处理
    SysException = b'6',
}
/// VirBankAccType 是一个银行帐户类型类型
#[repr(u8)]
pub enum VirBankAccType {
    /// 存折
    BankBook = b'1',
    /// 储蓄卡
    BankCard = b'2',
    /// 信用卡
    CreditCard = b'3',
}
/// VirementStatus 是一个银行帐户类型类型
#[repr(u8)]
pub enum VirementStatus {
    /// 正常
    Natural = b'0',
    /// 销户
    Canceled = b'9',
}
/// VirementAvailAbility 是一个有效标志类型
#[repr(u8)]
pub enum VirementAvailAbility {
    /// 未确认
    NoAvailAbility = b'0',
    /// 有效
    AvailAbility = b'1',
    /// 冲正
    Repeal = b'2',
}
/// AMLGenStatus 是一个Aml生成方式类型
#[repr(u8)]
pub enum AMLGenStatus {
    /// 程序生成
    Program = b'0',
    /// 人工生成
    HandWork = b'1',
}
/// CFMMCKeyKind 是一个动态密钥类别(保证金监管)类型
#[repr(u8)]
pub enum CFMMCKeyKind {
    /// 主动请求更新
    REQUEST = b'R',
    /// CFMMC自动更新
    AUTO = b'A',
    /// CFMMC手动更新
    MANUAL = b'M',
}
/// CertificationType 是一个证件类型类型
#[repr(u8)]
pub enum CertificationType {
    /// 身份证
    IDCard = b'0',
    /// 护照
    Passport = b'1',
    /// 军官证
    OfficerIDCard = b'2',
    /// 士兵证
    SoldierIDCard = b'3',
    /// 回乡证
    HomeComingCard = b'4',
    /// 户口簿
    HouseholdRegister = b'5',
    /// 营业执照号
    LicenseNo = b'6',
    /// 组织机构代码证
    InstitutionCodeCard = b'7',
    /// 临时营业执照号
    TempLicenseNo = b'8',
    /// 民办非企业登记证书
    NoEnterpriseLicenseNo = b'9',
    /// 其他证件
    OtherCard = b'x',
    /// 主管部门批文
    SuperDepAgree = b'a',
}
/// FileBusinessCode 是一个文件业务功能类型
#[repr(u8)]
pub enum FileBusinessCode {
    /// 其他
    Others = b'0',
    /// 转账交易明细对账
    TransferDetails = b'1',
    /// 客户账户状态对账
    CustAccStatus = b'2',
    /// 账户类交易明细对账
    AccountTradeDetails = b'3',
    /// 期货账户信息变更明细对账
    FutureAccountChangeInfoDetails = b'4',
    /// 客户资金台账余额明细对账
    CustMoneyDetail = b'5',
    /// 客户销户结息明细对账
    CustCancelAccountInfo = b'6',
    /// 客户资金余额对账结果
    CustMoneyResult = b'7',
    /// 其它对账异常结果文件
    OthersExceptionResult = b'8',
    /// 客户结息净额明细
    CustInterestNetMoneyDetails = b'9',
    /// 客户资金交收明细
    CustMoneySendAndReceiveDetails = b'a',
    /// 法人存管银行资金交收汇总
    CorporationMoneyTotal = b'b',
    /// 主体间资金交收汇总
    MainbodyMoneyTotal = b'c',
    /// 总分平衡监管数据
    MainPartMonitorData = b'd',
    /// 存管银行备付金余额
    PreparationMoney = b'e',
    /// 协办存管银行资金监管数据
    BankMoneyMonitorData = b'f',
}
/// CashExchangeCode 是一个汇钞标志类型
#[repr(u8)]
pub enum CashExchangeCode {
    /// 汇
    Exchange = b'1',
    /// 钞
    Cash = b'2',
}
/// YesNoIndicator 是一个是或否标识类型
#[repr(u8)]
pub enum YesNoIndicator {
    /// 是
    Yes = b'0',
    /// 否
    No = b'1',
}
/// BalanceType 是一个余额类型类型
#[repr(u8)]
pub enum BalanceType {
    /// 当前余额
    CurrentMoney = b'0',
    /// 可用余额
    UsableMoney = b'1',
    /// 可取余额
    FetchableMoney = b'2',
    /// 冻结余额
    FreezeMoney = b'3',
}
/// Gender 是一个性别类型
#[repr(u8)]
pub enum Gender {
    /// 未知状态
    Unknown = b'0',
    /// 男
    Male = b'1',
    /// 女
    Female = b'2',
}
/// FeePayFlag 是一个费用支付标志类型
#[repr(u8)]
pub enum FeePayFlag {
    /// 由受益方支付费用
    BEN = b'0',
    /// 由发送方支付费用
    OUR = b'1',
    /// 由发送方支付发起的费用，受益方支付接受的费用
    SHA = b'2',
}
/// PassWordKeyType 是一个密钥类型类型
#[repr(u8)]
pub enum PassWordKeyType {
    /// 交换密钥
    ExchangeKey = b'0',
    /// 密码密钥
    PassWordKey = b'1',
    /// MAC密钥
    MACKey = b'2',
    /// 报文密钥
    MessageKey = b'3',
}
/// FBTPassWordType 是一个密码类型类型
#[repr(u8)]
pub enum FBTPassWordType {
    /// 查询
    Query = b'0',
    /// 取款
    Fetch = b'1',
    /// 转帐
    Transfer = b'2',
    /// 交易
    Trade = b'3',
}
/// FBTEncryMode 是一个加密方式类型
#[repr(u8)]
pub enum FBTEncryMode {
    /// 不加密
    NoEncry = b'0',
    /// DES
    DES = b'1',
    /// 3DES
    _3DES = b'2',
}
/// BankRepealFlag 是一个银行冲正标志类型
#[repr(u8)]
pub enum BankRepealFlag {
    /// 银行无需自动冲正
    BankNotNeedRepeal = b'0',
    /// 银行待自动冲正
    BankWaitingRepeal = b'1',
    /// 银行已自动冲正
    BankBeenRepealed = b'2',
}
/// BrokerRepealFlag 是一个期商冲正标志类型
#[repr(u8)]
pub enum BrokerRepealFlag {
    /// 期商无需自动冲正
    BrokerNotNeedRepeal = b'0',
    /// 期商待自动冲正
    BrokerWaitingRepeal = b'1',
    /// 期商已自动冲正
    BrokerBeenRepealed = b'2',
}
/// InstitutionType 是一个机构类别类型
#[repr(u8)]
pub enum InstitutionType {
    /// 银行
    Bank = b'0',
    /// 期商
    Future = b'1',
    /// 券商
    Store = b'2',
}
/// LastFragment 是一个最后分片标志类型
#[repr(u8)]
pub enum LastFragment {
    /// 是最后分片
    Yes = b'0',
    /// 不是最后分片
    No = b'1',
}
/// BankAccStatus 是一个银行账户状态类型
#[repr(u8)]
pub enum BankAccStatus {
    /// 正常
    Normal = b'0',
    /// 冻结
    Freeze = b'1',
    /// 挂失
    ReportLoss = b'2',
}
/// MoneyAccountStatus 是一个资金账户状态类型
#[repr(u8)]
pub enum MoneyAccountStatus {
    /// 正常
    Normal = b'0',
    /// 销户
    Cancel = b'1',
}
/// ManageStatus 是一个存管状态类型
#[repr(u8)]
pub enum ManageStatus {
    /// 指定存管
    Point = b'0',
    /// 预指定
    PrePoint = b'1',
    /// 撤销指定
    CancelPoint = b'2',
}
/// SystemType 是一个应用系统类型类型
#[repr(u8)]
pub enum SystemType {
    /// 银期转帐
    FutureBankTransfer = b'0',
    /// 银证转帐
    StockBankTransfer = b'1',
    /// 第三方存管
    TheThirdPartStore = b'2',
}
/// TxnEndFlag 是一个银期转帐划转结果标志类型
#[repr(u8)]
pub enum TxnEndFlag {
    /// 正常处理中
    NormalProcessing = b'0',
    /// 成功结束
    Success = b'1',
    /// 失败结束
    Failed = b'2',
    /// 异常中
    Abnormal = b'3',
    /// 已人工异常处理
    ManualProcessedForException = b'4',
    /// 通讯异常 ，请人工处理
    CommFailedNeedManualProcess = b'5',
    /// 系统出错，请人工处理
    SysErrorNeedManualProcess = b'6',
}
/// ProcessStatus 是一个银期转帐服务处理状态类型
#[repr(u8)]
pub enum ProcessStatus {
    /// 未处理
    NotProcess = b'0',
    /// 开始处理
    StartProcess = b'1',
    /// 处理完成
    Finished = b'2',
}
/// CustType 是一个客户类型类型
#[repr(u8)]
pub enum CustType {
    /// 自然人
    Person = b'0',
    /// 机构户
    Institution = b'1',
}
/// FBTTransferDirection 是一个银期转帐方向类型
#[repr(u8)]
pub enum FBTTransferDirection {
    /// 入金，银行转期货
    FromBankToFuture = b'1',
    /// 出金，期货转银行
    FromFutureToBank = b'2',
}
/// OpenOrDestroy 是一个开销户类别类型
#[repr(u8)]
pub enum OpenOrDestroy {
    /// 开户
    Open = b'1',
    /// 销户
    Destroy = b'0',
}
/// AvailabilityFlag 是一个有效标志类型
#[repr(u8)]
pub enum AvailabilityFlag {
    /// 未确认
    Invalid = b'0',
    /// 有效
    Valid = b'1',
    /// 冲正
    Repeal = b'2',
}
/// OrganType 是一个机构类型类型
#[repr(u8)]
pub enum OrganType {
    /// 银行代理
    Bank = b'1',
    /// 交易前置
    Future = b'2',
    /// 银期转帐平台管理
    PlateForm = b'9',
}
/// OrganLevel 是一个机构级别类型
#[repr(u8)]
pub enum OrganLevel {
    /// 银行总行或期商总部
    HeadQuarters = b'1',
    /// 银行分中心或期货公司营业部
    Branch = b'2',
}
/// ProtocolID 是一个协议类型类型
#[repr(u8)]
pub enum ProtocolID {
    /// 期商协议
    FutureProtocol = b'0',
    /// 工行协议
    ICBCProtocol = b'1',
    /// 农行协议
    ABCProtocol = b'2',
    /// 中国银行协议
    CBCProtocol = b'3',
    /// 建行协议
    CCBProtocol = b'4',
    /// 交行协议
    BOCOMProtocol = b'5',
    /// 银期转帐平台协议
    FBTPlateFormProtocol = b'X',
}
/// ConnectMode 是一个套接字连接方式类型
#[repr(u8)]
pub enum ConnectMode {
    /// 短连接
    ShortConnect = b'0',
    /// 长连接
    LongConnect = b'1',
}
/// SyncMode 是一个套接字通信方式类型
#[repr(u8)]
pub enum SyncMode {
    /// 异步
    ASync = b'0',
    /// 同步
    Sync = b'1',
}
/// BankAccType 是一个银行帐号类型类型
#[repr(u8)]
pub enum BankAccType {
    /// 银行存折
    BankBook = b'1',
    /// 储蓄卡
    SavingCard = b'2',
    /// 信用卡
    CreditCard = b'3',
}
/// FutureAccType 是一个期货公司帐号类型类型
#[repr(u8)]
pub enum FutureAccType {
    /// 银行存折
    BankBook = b'1',
    /// 储蓄卡
    SavingCard = b'2',
    /// 信用卡
    CreditCard = b'3',
}
/// OrganStatus 是一个接入机构状态类型
#[repr(u8)]
pub enum OrganStatus {
    /// 启用
    Ready = b'0',
    /// 签到
    CheckIn = b'1',
    /// 签退
    CheckOut = b'2',
    /// 对帐文件到达
    CheckFileArrived = b'3',
    /// 对帐
    CheckDetail = b'4',
    /// 日终清理
    DayEndClean = b'5',
    /// 注销
    Invalid = b'9',
}
/// CCBFeeMode 是一个建行收费模式类型
#[repr(u8)]
pub enum CCBFeeMode {
    /// 按金额扣收
    ByAmount = b'1',
    /// 按月扣收
    ByMonth = b'2',
}
/// CommApiType 是一个通讯API类型类型
#[repr(u8)]
pub enum CommApiType {
    /// 客户端
    Client = b'1',
    /// 服务端
    Server = b'2',
    /// 交易系统的UserApi
    UserApi = b'3',
}
/// LinkStatus 是一个连接状态类型
#[repr(u8)]
pub enum LinkStatus {
    /// 已经连接
    Connected = b'1',
    /// 没有连接
    Disconnected = b'2',
}
/// PwdFlag 是一个密码核对标志类型
#[repr(u8)]
pub enum PwdFlag {
    /// 不核对
    NoCheck = b'0',
    /// 明文核对
    BlankCheck = b'1',
    /// 密文核对
    EncryptCheck = b'2',
}
/// SecuAccType 是一个期货帐号类型类型
#[repr(u8)]
pub enum SecuAccType {
    /// 资金帐号
    AccountID = b'1',
    /// 资金卡号
    CardID = b'2',
    /// 上海股东帐号
    SHStockholderID = b'3',
    /// 深圳股东帐号
    SZStockholderID = b'4',
}
/// TransferStatus 是一个转账交易状态类型
#[repr(u8)]
pub enum TransferStatus {
    /// 正常
    Normal = b'0',
    /// 被冲正
    Repealed = b'1',
}
/// SponsorType 是一个发起方类型
#[repr(u8)]
pub enum SponsorType {
    /// 期商
    Broker = b'0',
    /// 银行
    Bank = b'1',
}
/// ReqRspType 是一个请求响应类别类型
#[repr(u8)]
pub enum ReqRspType {
    /// 请求
    Request = b'0',
    /// 响应
    Response = b'1',
}
/// FBTUserEventType 是一个银期转帐用户事件类型类型
#[repr(u8)]
pub enum FBTUserEventType {
    /// 签到
    SignIn = b'0',
    /// 银行转期货
    FromBankToFuture = b'1',
    /// 期货转银行
    FromFutureToBank = b'2',
    /// 开户
    OpenAccount = b'3',
    /// 销户
    CancelAccount = b'4',
    /// 变更银行账户
    ChangeAccount = b'5',
    /// 冲正银行转期货
    RepealFromBankToFuture = b'6',
    /// 冲正期货转银行
    RepealFromFutureToBank = b'7',
    /// 查询银行账户
    QueryBankAccount = b'8',
    /// 查询期货账户
    QueryFutureAccount = b'9',
    /// 签退
    SignOut = b'A',
    /// 密钥同步
    SyncKey = b'B',
    /// 预约开户
    ReserveOpenAccount = b'C',
    /// 撤销预约开户
    CancelReserveOpenAccount = b'D',
    /// 预约开户确认
    ReserveOpenAccountConfirm = b'E',
    /// 其他
    Other = b'Z',
}
/// DBOperation 是一个记录操作类型类型
#[repr(u8)]
pub enum DBOperation {
    /// 插入
    Insert = b'0',
    /// 更新
    Update = b'1',
    /// 删除
    Delete = b'2',
}
/// SyncFlag 是一个同步标记类型
#[repr(u8)]
pub enum SyncFlag {
    /// 已同步
    Yes = b'0',
    /// 未同步
    No = b'1',
}
/// SyncType 是一个同步类型类型
#[repr(u8)]
pub enum SyncType {
    /// 一次同步
    OneOffSync = b'0',
    /// 定时同步
    TimerSync = b'1',
    /// 定时完全同步
    TimerFullSync = b'2',
}
/// ExDirection 是一个换汇方向类型
#[repr(u8)]
pub enum ExDirection {
    /// 结汇
    Settlement = b'0',
    /// 售汇
    Sale = b'1',
}
/// FBEResultFlag 是一个换汇成功标志类型
#[repr(u8)]
pub enum FBEResultFlag {
    /// 成功
    Success = b'0',
    /// 账户余额不足
    InsufficientBalance = b'1',
    /// 交易结果未知
    UnknownTrading = b'8',
    /// 失败
    Fail = b'x',
}
/// FBEExchStatus 是一个换汇交易状态类型
#[repr(u8)]
pub enum FBEExchStatus {
    /// 正常
    Normal = b'0',
    /// 交易重发
    ReExchange = b'1',
}
/// FBEFileFlag 是一个换汇文件标志类型
#[repr(u8)]
pub enum FBEFileFlag {
    /// 数据包
    DataPackage = b'0',
    /// 文件
    File = b'1',
}
/// FBEAlreadyTrade 是一个换汇已交易标志类型
#[repr(u8)]
pub enum FBEAlreadyTrade {
    /// 未交易
    NotTrade = b'0',
    /// 已交易
    Trade = b'1',
}
/// FBEUserEventType 是一个银期换汇用户事件类型类型
#[repr(u8)]
pub enum FBEUserEventType {
    /// 签到
    SignIn = b'0',
    /// 换汇
    Exchange = b'1',
    /// 换汇重发
    ReExchange = b'2',
    /// 银行账户查询
    QueryBankAccount = b'3',
    /// 换汇明细查询
    QueryExchDetail = b'4',
    /// 换汇汇总查询
    QueryExchSummary = b'5',
    /// 换汇汇率查询
    QueryExchRate = b'6',
    /// 对账文件通知
    CheckBankAccount = b'7',
    /// 签退
    SignOut = b'8',
    /// 其他
    Other = b'Z',
}
/// FBEReqFlag 是一个换汇发送标志类型
#[repr(u8)]
pub enum FBEReqFlag {
    /// 未处理
    UnProcessed = b'0',
    /// 等待发送
    WaitSend = b'1',
    /// 发送成功
    SendSuccess = b'2',
    /// 发送失败
    SendFailed = b'3',
    /// 等待重发
    WaitReSend = b'4',
}
/// NotifyClass 是一个风险通知类型类型
#[repr(u8)]
pub enum NotifyClass {
    /// 正常
    Normal = b'0',
    /// 警示
    Warn = b'1',
    /// 追保
    Call = b'2',
    /// 强平
    Force = b'3',
    /// 穿仓
    Blowout = b'4',
    /// 异常
    Exception = b'5',
}
/// ForceCloseType 是一个强平单类型类型
#[repr(u8)]
pub enum ForceCloseType {
    /// 手工强平
    Manual = b'0',
    /// 单一投资者辅助强平
    Single = b'1',
    /// 批量投资者辅助强平
    Group = b'2',
}
/// RiskNotifyMethod 是一个风险通知途径类型
#[repr(u8)]
pub enum RiskNotifyMethod {
    /// 系统通知
    System = b'0',
    /// 短信通知
    SMS = b'1',
    /// 邮件通知
    EMail = b'2',
    /// 人工通知
    Manual = b'3',
}
/// RiskNotifyStatus 是一个风险通知状态类型
#[repr(u8)]
pub enum RiskNotifyStatus {
    /// 未生成
    NotGen = b'0',
    /// 已生成未发送
    Generated = b'1',
    /// 发送失败
    SendError = b'2',
    /// 已发送未接收
    SendOk = b'3',
    /// 已接收未确认
    Received = b'4',
    /// 已确认
    Confirmed = b'5',
}
/// RiskUserEvent 是一个风控用户操作事件类型
#[repr(u8)]
pub enum RiskUserEvent {
    /// 导出数据
    ExportData = b'0',
}
/// ConditionalOrderSortType 是一个条件单索引条件类型
#[repr(u8)]
pub enum ConditionalOrderSortType {
    /// 使用最新价升序
    LastPriceAsc = b'0',
    /// 使用最新价降序
    LastPriceDesc = b'1',
    /// 使用卖价升序
    AskPriceAsc = b'2',
    /// 使用卖价降序
    AskPriceDesc = b'3',
    /// 使用买价升序
    BidPriceAsc = b'4',
    /// 使用买价降序
    BidPriceDesc = b'5',
}
/// SendType 是一个报送状态类型
#[repr(u8)]
pub enum SendType {
    /// 未发送
    NoSend = b'0',
    /// 已发送
    Sent  = b'1',
    /// 已生成
    Generated = b'2',
    /// 报送失败
    SendFail = b'3',
    /// 接收成功
    Success = b'4',
    /// 接收失败
    Fail = b'5',
    /// 取消报送
    Cancel = b'6',
}
/// ClientIDStatus 是一个交易编码状态类型
#[repr(u8)]
pub enum ClientIDStatus {
    /// 未申请
    NoApply = b'1',
    /// 已提交申请
    Submitted = b'2',
    /// 已发送申请
    Sent = b'3',
    /// 完成
    Success = b'4',
    /// 拒绝
    Refuse = b'5',
    /// 已撤销编码
    Cancel = b'6',
}
/// QuestionType 是一个特有信息类型类型
#[repr(u8)]
pub enum QuestionType {
    /// 单选
    Radio = b'1',
    /// 多选
    Option = b'2',
    /// 填空
    Blank = b'3',
}
/// BusinessType 是一个业务类型类型
#[repr(u8)]
pub enum BusinessType {
    /// 请求
    Request = b'1',
    /// 应答
    Response = b'2',
    /// 通知
    Notice = b'3',
}
/// CfmmcReturnCode 是一个监控中心返回码类型
#[repr(u8)]
pub enum CfmmcReturnCode {
    /// 成功
    Success = b'0',
    /// 该客户已经有流程在处理中
    Working = b'1',
    /// 监控中客户资料检查失败
    InfoFail = b'2',
    /// 监控中实名制检查失败
    IDCardFail = b'3',
    /// 其他错误
    OtherFail = b'4',
}
/// ClientType 是一个客户类型类型
#[repr(u8)]
pub enum ClientType {
    /// 所有
    All = b'0',
    /// 个人
    Person = b'1',
    /// 单位
    Company = b'2',
    /// 其他
    Other = b'3',
    /// 特殊法人
    SpecialOrgan = b'4',
    /// 资管户
    Asset = b'5',
}
/// ExchangeIDType 是一个交易所编号类型
#[repr(u8)]
pub enum ExchangeIDType {
    /// 上海期货交易所
    SHFE = b'S',
    /// 郑州商品交易所
    CZCE = b'Z',
    /// 大连商品交易所
    DCE = b'D',
    /// 中国金融期货交易所
    CFFEX = b'J',
    /// 上海国际能源交易中心股份有限公司
    INE = b'N',
}
/// ExClientIDType 是一个交易编码类型类型
#[repr(u8)]
pub enum ExClientIDType {
    /// 套保
    Hedge = b'1',
    /// 套利
    Arbitrage = b'2',
    /// 投机
    Speculation = b'3',
}
/// UpdateFlag 是一个更新状态类型
#[repr(u8)]
pub enum UpdateFlag {
    /// 未更新
    NoUpdate = b'0',
    /// 更新全部信息成功
    Success = b'1',
    /// 更新全部信息失败
    Fail = b'2',
    /// 更新交易编码成功
    TCSuccess = b'3',
    /// 更新交易编码失败
    TCFail = b'4',
    /// 已丢弃
    Cancel = b'5',
}
/// ApplyOperateID 是一个申请动作类型
#[repr(u8)]
pub enum ApplyOperateID {
    /// 开户
    OpenInvestor = b'1',
    /// 修改身份信息
    ModifyIDCard = b'2',
    /// 修改一般信息
    ModifyNoIDCard = b'3',
    /// 申请交易编码
    ApplyTradingCode = b'4',
    /// 撤销交易编码
    CancelTradingCode = b'5',
    /// 销户
    CancelInvestor = b'6',
    /// 账户休眠
    FreezeAccount = b'8',
    /// 激活休眠账户
    ActiveFreezeAccount = b'9',
}
/// ApplyStatusID 是一个申请状态类型
#[repr(u8)]
pub enum ApplyStatusID {
    /// 未补全
    NoComplete = b'1',
    /// 已提交
    Submitted = b'2',
    /// 已审核
    Checked = b'3',
    /// 已拒绝
    Refused = b'4',
    /// 已删除
    Deleted = b'5',
}
/// SendMethod 是一个发送方式类型
#[repr(u8)]
pub enum SendMethod {
    /// 文件发送
    ByAPI = b'1',
    /// 电子发送
    ByFile = b'2',
}
/// EventMode 是一个操作方法类型
#[repr(u8)]
pub enum EventMode {
    /// 增加
    ADD = b'1',
    /// 修改
    UPDATE = b'2',
    /// 删除
    DELETE = b'3',
    /// 复核
    CHECK = b'4',
    /// 复制
    COPY = b'5',
    /// 注销
    CANCEL = b'6',
    /// 冲销
    Reverse = b'7',
}
/// UOAAutoSend 是一个统一开户申请自动发送类型
#[repr(u8)]
pub enum UOAAutoSend {
    /// 自动发送并接收
    ASR = b'1',
    /// 自动发送，不自动接收
    ASNR = b'2',
    /// 不自动发送，自动接收
    NSAR = b'3',
    /// 不自动发送，也不自动接收
    NSR = b'4',
}
/// FlowID 是一个流程ID类型
#[repr(u8)]
pub enum FlowID {
    /// 投资者对应投资者组设置
    InvestorGroupFlow = b'1',
    /// 投资者手续费率设置
    InvestorRate = b'2',
    /// 投资者手续费率模板关系设置
    InvestorCommRateModel = b'3',
}
/// CheckLevel 是一个复核级别类型
#[repr(u8)]
pub enum CheckLevel {
    /// 零级复核
    Zero = b'0',
    /// 一级复核
    One = b'1',
    /// 二级复核
    Two = b'2',
}
/// CheckStatus 是一个复核级别类型
#[repr(u8)]
pub enum CheckStatus {
    /// 未复核
    Init = b'0',
    /// 复核中
    Checking = b'1',
    /// 已复核
    Checked = b'2',
    /// 拒绝
    Refuse = b'3',
    /// 作废
    Cancel = b'4',
}
/// UsedStatus 是一个生效状态类型
#[repr(u8)]
pub enum UsedStatus {
    /// 未生效
    Unused = b'0',
    /// 已生效
    Used = b'1',
    /// 生效失败
    Fail = b'2',
}
/// BankAccountOrigin 是一个账户来源类型
#[repr(u8)]
pub enum BankAccountOrigin {
    /// 手工录入
    ByAccProperty = b'0',
    /// 银期转账
    ByFBTransfer = b'1',
}
/// MonthBillTradeSum 是一个结算单月报成交汇总方式类型
#[repr(u8)]
pub enum MonthBillTradeSum {
    /// 同日同合约
    ByInstrument = b'0',
    /// 同日同合约同价格
    ByDayInsPrc = b'1',
    /// 同合约
    ByDayIns = b'2',
}
/// OTPType 是一个动态令牌类型类型
#[repr(u8)]
pub enum OTPType {
    /// 无动态令牌
    NONE = b'0',
    /// 时间令牌
    TOTP = b'1',
}
/// OTPStatus 是一个动态令牌状态类型
#[repr(u8)]
pub enum OTPStatus {
    /// 未使用
    Unused = b'0',
    /// 已使用
    Used = b'1',
    /// 注销
    Disuse = b'2',
}
/// BrokerUserType 是一个经济公司用户类型类型
#[repr(u8)]
pub enum BrokerUserType {
    /// 投资者
    Investor = b'1',
    /// 操作员
    BrokerUser = b'2',
}
/// FutureType 是一个期货类型类型
#[repr(u8)]
pub enum FutureType {
    /// 商品期货
    Commodity = b'1',
    /// 金融期货
    Financial = b'2',
}
/// FundEventType 是一个资金管理操作类型类型
#[repr(u8)]
pub enum FundEventType {
    /// 转账限额
    Restriction = b'0',
    /// 当日转账限额
    TodayRestriction = b'1',
    /// 期商流水
    Transfer = b'2',
    /// 资金冻结
    Credit = b'3',
    /// 投资者可提资金比例
    InvestorWithdrawAlm = b'4',
    /// 单个银行账户转账限额
    BankRestriction = b'5',
    /// 银期签约账户
    AccountRegister = b'6',
    /// 交易所出入金
    ExchangeFundIO = b'7',
    /// 投资者出入金
    InvestorFundIO = b'8',
}
/// AccountSourceType 是一个资金账户来源类型
#[repr(u8)]
pub enum AccountSourceType {
    /// 银期同步
    FBTransfer = b'0',
    /// 手工录入
    ManualEntry = b'1',
}
/// CodeSourceType 是一个交易编码来源类型
#[repr(u8)]
pub enum CodeSourceType {
    /// 统一开户(已规范)
    UnifyAccount = b'0',
    /// 手工录入(未规范)
    ManualEntry = b'1',
}
/// UserRange 是一个操作员范围类型
#[repr(u8)]
pub enum UserRange {
    /// 所有
    All = b'0',
    /// 单一操作员
    Single = b'1',
}
/// ByGroup 是一个交易统计表按客户统计方式类型
#[repr(u8)]
pub enum ByGroup {
    /// 按投资者统计
    Investor = b'2',
    /// 按类统计
    Group = b'1',
}
/// TradeSumStatMode 是一个交易统计表按范围统计方式类型
#[repr(u8)]
pub enum TradeSumStatMode {
    /// 按合约统计
    Instrument = b'1',
    /// 按产品统计
    Product = b'2',
    /// 按交易所统计
    Exchange = b'3',
}
/// ExprSetMode 是一个日期表达式设置类型类型
#[repr(u8)]
pub enum ExprSetMode {
    /// 相对已有规则设置
    Relative = b'1',
    /// 典型设置
    Typical = b'2',
}
/// RateInvestorRange 是一个投资者范围类型
#[repr(u8)]
pub enum RateInvestorRange {
    /// 公司标准
    All = b'1',
    /// 模板
    Model = b'2',
    /// 单一投资者
    Single = b'3',
}
/// SyncDataStatus 是一个主次用系统数据同步状态类型
#[repr(u8)]
pub enum SyncDataStatus {
    /// 未同步
    Initialize = b'0',
    /// 同步中
    Settling = b'1',
    /// 已同步
    Settled = b'2',
}
/// TradeSource 是一个成交来源类型
#[repr(u8)]
pub enum TradeSource {
    /// 来自交易所普通回报
    NORMAL = b'0',
    /// 来自查询
    QUERY = b'1',
}
/// FlexStatMode 是一个产品合约统计方式类型
#[repr(u8)]
pub enum FlexStatMode {
    /// 产品统计
    Product = b'1',
    /// 交易所统计
    Exchange = b'2',
    /// 统计所有
    All = b'3',
}
/// ByInvestorRange 是一个投资者范围统计方式类型
#[repr(u8)]
pub enum ByInvestorRange {
    /// 属性统计
    Property = b'1',
    /// 统计所有
    All = b'2',
}
/// PropertyInvestorRange 是一个投资者范围类型
#[repr(u8)]
pub enum PropertyInvestorRange {
    /// 所有
    All = b'1',
    /// 投资者属性
    Property = b'2',
    /// 单一投资者
    Single = b'3',
}
/// FileStatus 是一个文件状态类型
#[repr(u8)]
pub enum FileStatus {
    /// 未生成
    NoCreate = b'0',
    /// 已生成
    Created = b'1',
    /// 生成失败
    Failed = b'2',
}
/// FileGenStyle 是一个文件生成方式类型
#[repr(u8)]
pub enum FileGenStyle {
    /// 下发
    FileTransmit = b'0',
    /// 生成
    FileGen = b'1',
}
/// SysOperMode 是一个系统日志操作方法类型
#[repr(u8)]
pub enum SysOperMode {
    /// 增加
    Add = b'1',
    /// 修改
    Update = b'2',
    /// 删除
    Delete = b'3',
    /// 复制
    Copy = b'4',
    /// 激活
    AcTive = b'5',
    /// 注销
    CanCel = b'6',
    /// 重置
    ReSet = b'7',
}
/// SysOperType 是一个系统日志操作类型类型
#[repr(u8)]
pub enum SysOperType {
    /// 修改操作员密码
    UpdatePassword = b'0',
    /// 操作员组织架构关系
    UserDepartment = b'1',
    /// 角色管理
    RoleManager = b'2',
    /// 角色功能设置
    RoleFunction = b'3',
    /// 基础参数设置
    BaseParam = b'4',
    /// 设置操作员
    SetUserID = b'5',
    /// 用户角色设置
    SetUserRole = b'6',
    /// 用户IP限制
    UserIpRestriction = b'7',
    /// 组织架构管理
    DepartmentManager = b'8',
    /// 组织架构向查询分类复制
    DepartmentCopy = b'9',
    /// 交易编码管理
    TradingCode = b'A',
    /// 投资者状态维护
    InvestorStatus = b'B',
    /// 投资者权限管理
    InvestorAuthority = b'C',
    /// 属性设置
    PropertySet = b'D',
    /// 重置投资者密码
    ReSetInvestorPasswd = b'E',
    /// 投资者个性信息维护
    InvestorPersonalityInfo = b'F',
}
/// CSRCDataQueryType 是一个上报数据查询类型类型
#[repr(u8)]
pub enum CSRCDataQueryType {
    /// 查询当前交易日报送的数据
    Current = b'0',
    /// 查询历史报送的代理经纪公司的数据
    History = b'1',
}
/// FreezeStatus 是一个休眠状态类型
#[repr(u8)]
pub enum FreezeStatus {
    /// 活跃
    Normal = b'1',
    /// 休眠
    Freeze = b'0',
}
/// StandardStatus 是一个规范状态类型
#[repr(u8)]
pub enum StandardStatus {
    /// 已规范
    Standard = b'0',
    /// 未规范
    NonStandard = b'1',
}
/// RightParamType 是一个配置类型类型
#[repr(u8)]
pub enum RightParamType {
    /// 休眠户
    Freeze = b'1',
    /// 激活休眠户
    FreezeActive = b'2',
    /// 开仓权限限制
    OpenLimit = b'3',
    /// 解除开仓权限限制
    RelieveOpenLimit = b'4',
}
/// DataStatus 是一个反洗钱审核表数据状态类型
#[repr(u8)]
pub enum DataStatus {
    /// 正常
    Normal = b'0',
    /// 已删除
    Deleted = b'1',
}
/// AMLCheckStatus 是一个审核状态类型
#[repr(u8)]
pub enum AMLCheckStatus {
    /// 未复核
    Init = b'0',
    /// 复核中
    Checking = b'1',
    /// 已复核
    Checked = b'2',
    /// 拒绝上报
    RefuseReport = b'3',
}
/// AmlDateType 是一个日期类型类型
#[repr(u8)]
pub enum AmlDateType {
    /// 检查日期
    DrawDay = b'0',
    /// 发生日期
    TouchDay = b'1',
}
/// AmlCheckLevel 是一个审核级别类型
#[repr(u8)]
pub enum AmlCheckLevel {
    /// 零级审核
    CheckLevel0 = b'0',
    /// 一级审核
    CheckLevel1 = b'1',
    /// 二级审核
    CheckLevel2 = b'2',
    /// 三级审核
    CheckLevel3 = b'3',
}
/// ExportFileType 是一个导出文件类型类型
#[repr(u8)]
pub enum ExportFileType {
    /// CSV
    CSV = b'0',
    /// Excel
    EXCEL = b'1',
    /// DBF
    DBF = b'2',
}
/// SettleManagerType 是一个结算配置类型类型
#[repr(u8)]
pub enum SettleManagerType {
    /// 结算前准备
    Before = b'1',
    /// 结算
    Settlement = b'2',
    /// 结算后核对
    After = b'3',
    /// 结算后处理
    Settled = b'4',
}
/// SettleManagerLevel 是一个结算配置等级类型
#[repr(u8)]
pub enum SettleManagerLevel {
    /// 必要
    Must = b'1',
    /// 警告
    Alarm = b'2',
    /// 提示
    Prompt = b'3',
    /// 不检查
    Ignore = b'4',
}
/// SettleManagerGroup 是一个模块分组类型
#[repr(u8)]
pub enum SettleManagerGroup {
    /// 交易所核对
    Exchange = b'1',
    /// 内部核对
    ASP = b'2',
    /// 上报数据核对
    CSRC = b'3',
}
/// LimitUseType 是一个保值额度使用类型类型
#[repr(u8)]
pub enum LimitUseType {
    /// 可重复使用
    Repeatable = b'1',
    /// 不可重复使用
    Unrepeatable = b'2',
}
/// DataResource 是一个数据来源类型
#[repr(u8)]
pub enum DataResource {
    /// 本系统
    Settle = b'1',
    /// 交易所
    Exchange = b'2',
    /// 报送数据
    CSRC = b'3',
}
/// MarginType 是一个保证金类型类型
#[repr(u8)]
pub enum MarginType {
    /// 交易所保证金率
    ExchMarginRate = b'0',
    /// 投资者保证金率
    InstrMarginRate = b'1',
    /// 投资者交易保证金率
    InstrMarginRateTrade = b'2',
}
/// ActiveType 是一个生效类型类型
#[repr(u8)]
pub enum ActiveType {
    /// 仅当日生效
    Intraday = b'1',
    /// 长期生效
    Long = b'2',
}
/// MarginRateType 是一个冲突保证金率类型类型
#[repr(u8)]
pub enum MarginRateType {
    /// 交易所保证金率
    Exchange = b'1',
    /// 投资者保证金率
    Investor = b'2',
    /// 投资者交易保证金率
    InvestorTrade = b'3',
}
/// BackUpStatus 是一个备份数据状态类型
#[repr(u8)]
pub enum BackUpStatus {
    /// 未生成备份数据
    UnBackUp = b'0',
    /// 备份数据生成中
    BackingUp = b'1',
    /// 已生成备份数据
    BackedUp = b'2',
    /// 备份数据失败
    BackFail = b'3',
}
/// InitSettlement 是一个结算初始化状态类型
#[repr(u8)]
pub enum InitSettlement {
    /// 结算初始化未开始
    UnInitialize = b'0',
    /// 结算初始化中
    Initialize = b'1',
    /// 结算初始化完成
    Initialized = b'2',
}
/// ReportStatus 是一个报表数据生成状态类型
#[repr(u8)]
pub enum ReportStatus {
    /// 未生成报表数据
    NoCreate = b'0',
    /// 报表数据生成中
    Create = b'1',
    /// 已生成报表数据
    Created = b'2',
    /// 生成报表数据失败
    CreateFail = b'3',
}
/// SaveStatus 是一个数据归档状态类型
#[repr(u8)]
pub enum SaveStatus {
    /// 归档未完成
    UnSaveData = b'0',
    /// 归档完成
    Archived = b'1',
}
/// SettArchiveStatus 是一个结算确认数据归档状态类型
#[repr(u8)]
pub enum SettArchiveStatus {
    /// 未归档数据
    UnArchived = b'0',
    /// 数据归档中
    Archiving = b'1',
    /// 已归档数据
    Archived = b'2',
    /// 归档数据失败
    ArchiveFail = b'3',
}
/// CTPType 是一个CTP交易系统类型类型
#[repr(u8)]
pub enum CTPType {
    /// 未知类型
    Unknown = b'0',
    /// 主中心
    MainCenter = b'1',
    /// 备中心
    BackUp = b'2',
}
/// CloseDealType 是一个平仓处理类型类型
#[repr(u8)]
pub enum CloseDealType {
    /// 正常
    Normal = b'0',
    /// 投机平仓优先
    SpecFirst = b'1',
}
/// MortgageFundUseRange 是一个货币质押资金可用范围类型
#[repr(u8)]
pub enum MortgageFundUseRange {
    /// 不能使用
    None = b'0',
    /// 用于保证金
    Margin = b'1',
    /// 用于手续费、盈亏、保证金
    All = b'2',
    /// 人民币方案3
    CNY3 = b'3',
}
/// SpecProductType 是一个特殊产品类型类型
#[repr(u8)]
pub enum SpecProductType {
    /// 郑商所套保产品
    CzceHedge = b'1',
    /// 货币质押产品
    IneForeignCurrency = b'2',
    /// 大连短线开平仓产品
    DceOpenClose = b'3',
}
/// FundMortgageType 是一个货币质押类型类型
#[repr(u8)]
pub enum FundMortgageType {
    /// 质押
    Mortgage = b'1',
    /// 解质
    Redemption = b'2',
}
/// AccountSettlementParamID 是一个投资者账户结算参数代码类型
#[repr(u8)]
pub enum AccountSettlementParamID {
    /// 基础保证金
    BaseMargin = b'1',
    /// 最低权益标准
    LowestInterest = b'2',
}
/// FundMortDirection 是一个货币质押方向类型
#[repr(u8)]
pub enum FundMortDirection {
    /// 货币质入
    In = b'1',
    /// 货币质出
    Out = b'2',
}
/// BusinessClass 是一个换汇类别类型
#[repr(u8)]
pub enum BusinessClass {
    /// 盈利
    Profit = b'0',
    /// 亏损
    Loss = b'1',
    /// 其他
    Other = b'Z',
}
/// SwapSourceType 是一个换汇数据来源类型
#[repr(u8)]
pub enum SwapSourceType {
    /// 手工
    Manual = b'0',
    /// 自动生成
    Automatic = b'1',
}
/// CurrExDirection 是一个换汇类型类型
#[repr(u8)]
pub enum CurrExDirection {
    /// 结汇
    Settlement = b'0',
    /// 售汇
    Sale = b'1',
}
/// CurrencySwapStatus 是一个申请状态类型
#[repr(u8)]
pub enum CurrencySwapStatus {
    /// 已录入
    Entry = b'1',
    /// 已审核
    Approve = b'2',
    /// 已拒绝
    Refuse = b'3',
    /// 已撤销
    Revoke = b'4',
    /// 已发送
    Send = b'5',
    /// 换汇成功
    Success = b'6',
    /// 换汇失败
    Failure = b'7',
}
/// ReqFlag 是一个换汇发送标志类型
#[repr(u8)]
pub enum ReqFlag {
    /// 未发送
    NoSend = b'0',
    /// 发送成功
    SendSuccess = b'1',
    /// 发送失败
    SendFailed = b'2',
    /// 等待重发
    WaitReSend = b'3',
}
/// ResFlag 是一个换汇返回成功标志类型
#[repr(u8)]
pub enum ResFlag {
    /// 成功
    Success = b'0',
    /// 账户余额不足
    Insufficient = b'1',
    /// 交易结果未知
    UnKnown = b'8',
}
/// ExStatus 是一个修改状态类型
#[repr(u8)]
pub enum ExStatus {
    /// 修改前
    Before = b'0',
    /// 修改后
    After = b'1',
}
/// ClientRegion 是一个开户客户地域类型
#[repr(u8)]
pub enum ClientRegion {
    /// 国内客户
    Domestic = b'1',
    /// 港澳台客户
    GMT = b'2',
    /// 国外客户
    Foreign = b'3',
}
/// HasBoard 是一个是否有董事会类型
#[repr(u8)]
pub enum HasBoard {
    /// 没有
    No = b'0',
    /// 有
    Yes = b'1',
}
/// StartMode 是一个启动模式类型
#[repr(u8)]
pub enum StartMode {
    /// 正常
    Normal = b'1',
    /// 应急
    Emerge = b'2',
    /// 恢复
    Restore = b'3',
}
/// TemplateType 是一个模型类型类型
#[repr(u8)]
pub enum TemplateType {
    /// 全量
    Full = b'1',
    /// 增量
    Increment = b'2',
    /// 备份
    BackUp = b'3',
}
/// LoginMode 是一个登录模式类型
#[repr(u8)]
pub enum LoginMode {
    /// 交易
    Trade = b'0',
    /// 转账
    Transfer = b'1',
}
/// PromptType 是一个日历提示类型类型
#[repr(u8)]
pub enum PromptType {
    /// 合约上下市
    Instrument = b'1',
    /// 保证金分段生效
    Margin = b'2',
}
/// HasTrustee 是一个是否有托管人类型
#[repr(u8)]
pub enum HasTrustee {
    /// 有
    Yes = b'1',
    /// 没有
    No = b'0',
}
/// AmType 是一个机构类型类型
#[repr(u8)]
pub enum AmType {
    /// 银行
    Bank = b'1',
    /// 证券公司
    Securities = b'2',
    /// 基金公司
    Fund = b'3',
    /// 保险公司
    Insurance = b'4',
    /// 信托公司
    Trust = b'5',
    /// 其他
    Other = b'9',
}
/// CSRCFundIOType 是一个出入金类型类型
#[repr(u8)]
pub enum CSRCFundIOType {
    /// 出入金
    FundIO = b'0',
    /// 银期换汇
    SwapCurrency = b'1',
}
/// CusAccountType 是一个结算账户类型类型
#[repr(u8)]
pub enum CusAccountType {
    /// 期货结算账户
    Futures = b'1',
    /// 纯期货资管业务下的资管结算账户
    AssetMgrFuture = b'2',
    /// 综合类资管业务下的期货资管托管账户
    AssetMgrTrustee = b'3',
    /// 综合类资管业务下的资金中转账户
    AssetMgrTransfer = b'4',
}
/// LanguageType 是一个通知语言类型类型
#[repr(u8)]
pub enum LanguageType {
    /// 中文
    Chinese = b'1',
    /// 英文
    English = b'2',
}
/// AssetMgrClientType 是一个资产管理客户类型类型
#[repr(u8)]
pub enum AssetMgrClientType {
    /// 个人资管客户
    Person = b'1',
    /// 单位资管客户
    Organ = b'2',
    /// 特殊单位资管客户
    SpecialOrgan = b'4',
}
/// AssetMgrType 是一个投资类型类型
#[repr(u8)]
pub enum AssetMgrType {
    /// 期货类
    Futures = b'3',
    /// 综合类
    SpecialOrgan = b'4',
}
/// CheckInstrType 是一个合约比较类型类型
#[repr(u8)]
pub enum CheckInstrType {
    /// 合约交易所不存在
    HasExch = b'0',
    /// 合约本系统不存在
    HasATP = b'1',
    /// 合约比较不一致
    HasDiff = b'2',
}
/// DeliveryType 是一个交割类型类型
#[repr(u8)]
pub enum DeliveryType {
    /// 手工交割
    HandDelivery = b'1',
    /// 到期交割
    PersonDelivery = b'2',
}
/// MaxMarginSideAlgorithm 是一个大额单边保证金算法类型
#[repr(u8)]
pub enum MaxMarginSideAlgorithm {
    /// 不使用大额单边保证金算法
    NO = b'0',
    /// 使用大额单边保证金算法
    YES = b'1',
}
/// DAClientType 是一个资产管理客户类型类型
#[repr(u8)]
pub enum DAClientType {
    /// 自然人
    Person = b'0',
    /// 法人
    Company = b'1',
    /// 其他
    Other = b'2',
}
/// UOAAssetMgrType 是一个投资类型类型
#[repr(u8)]
pub enum UOAAssetMgrType {
    /// 期货类
    Futures = b'1',
    /// 综合类
    SpecialOrgan = b'2',
}
/// DirectionEn 是一个买卖方向类型
#[repr(u8)]
pub enum DirectionEn {
    /// Buy
    Buy = b'0',
    /// Sell
    Sell = b'1',
}
/// OffsetFlagEn 是一个开平标志类型
#[repr(u8)]
pub enum OffsetFlagEn {
    /// Position Opening
    Open = b'0',
    /// Position Close
    Close = b'1',
    /// Forced Liquidation
    ForceClose = b'2',
    /// Close Today
    CloseToday = b'3',
    /// Close Prev.
    CloseYesterday = b'4',
    /// Forced Reduction
    ForceOff = b'5',
    /// Local Forced Liquidation
    LocalForceClose = b'6',
}
/// HedgeFlagEn 是一个投机套保标志类型
#[repr(u8)]
pub enum HedgeFlagEn {
    /// Speculation
    Speculation = b'1',
    /// Arbitrage
    Arbitrage = b'2',
    /// Hedge
    Hedge = b'3',
}
/// FundIOTypeEn 是一个出入金类型类型
#[repr(u8)]
pub enum FundIOTypeEn {
    /// Deposit/Withdrawal
    FundIO = b'1',
    /// Bank-Futures Transfer
    Transfer = b'2',
    /// Bank-Futures FX Exchange
    SwapCurrency = b'3',
}
/// FundTypeEn 是一个资金类型类型
#[repr(u8)]
pub enum FundTypeEn {
    /// Bank Deposit
    Deposit = b'1',
    /// Payment/Fee
    ItemFund = b'2',
    /// Brokerage Adj
    Company = b'3',
    /// Internal Transfer
    InnerTransfer = b'4',
}
/// FundDirectionEn 是一个出入金方向类型
#[repr(u8)]
pub enum FundDirectionEn {
    /// Deposit
    In = b'1',
    /// Withdrawal
    Out = b'2',
}
/// FundMortDirectionEn 是一个货币质押方向类型
#[repr(u8)]
pub enum FundMortDirectionEn {
    /// Pledge
    In = b'1',
    /// Redemption
    Out = b'2',
}
/// OptionsType 是一个期权类型类型
#[repr(u8)]
pub enum OptionsType {
    /// 看涨
    CallOptions = b'1',
    /// 看跌
    PutOptions = b'2',
}
/// StrikeMode 是一个执行方式类型
#[repr(u8)]
pub enum StrikeMode {
    /// 欧式
    Continental = b'0',
    /// 美式
    American = b'1',
    /// 百慕大
    Bermuda = b'2',
}
/// StrikeType 是一个执行类型类型
#[repr(u8)]
pub enum StrikeType {
    /// 自身对冲
    Hedge = b'0',
    /// 匹配执行
    Match = b'1',
}
/// ApplyType 是一个中金所期权放弃执行申请类型类型
#[repr(u8)]
pub enum ApplyType {
    /// 不执行数量
    NotStrikeNum = b'4',
}
/// GiveUpDataSource 是一个放弃执行申请数据来源类型
#[repr(u8)]
pub enum GiveUpDataSource {
    /// 系统生成
    Gen = b'0',
    /// 手工添加
    Hand = b'1',
}
/// ExecResult 是一个执行结果类型
#[repr(u8)]
pub enum ExecResult {
    /// 没有执行
    NoExec = b'n',
    /// 已经取消
    Canceled = b'c',
    /// 执行成功
    OK = b'0',
    /// 期权持仓不够
    NoPosition = b'1',
    /// 资金不够
    NoDeposit = b'2',
    /// 会员不存在
    NoParticipant = b'3',
    /// 客户不存在
    NoClient = b'4',
    /// 合约不存在
    NoInstrument = b'6',
    /// 没有执行权限
    NoRight = b'7',
    /// 不合理的数量
    InvalidVolume = b'8',
    /// 没有足够的历史成交
    NoEnoughHistoryTrade = b'9',
    /// 未知
    Unknown = b'a',
}
/// CombinationType 是一个组合类型类型
#[repr(u8)]
pub enum CombinationType {
    /// 期货组合
    Future = b'0',
    /// 垂直价差BUL
    BUL = b'1',
    /// 垂直价差BER
    BER = b'2',
    /// 跨式组合
    STD = b'3',
    /// 宽跨式组合
    STG = b'4',
    /// 备兑组合
    PRT = b'5',
    /// 时间价差组合
    CAS = b'6',
    /// 期权对锁组合
    OPL = b'7',
    /// 买备兑组合
    BFO = b'8',
    /// 买入期权垂直价差组合
    BLS = b'9',
    /// 卖出期权垂直价差组合
    BES = b'a',
}
/// DceCombinationType 是一个组合类型类型
#[repr(u8)]
pub enum DceCombinationType {
    /// 期货对锁组合
    SPL = b'0',
    /// 期权对锁组合
    OPL = b'1',
    /// 期货跨期组合
    SP = b'2',
    /// 期货跨品种组合
    SPC = b'3',
    /// 买入期权垂直价差组合
    BLS = b'4',
    /// 卖出期权垂直价差组合
    BES = b'5',
    /// 期权日历价差组合
    CAS = b'6',
    /// 期权跨式组合
    STD = b'7',
    /// 期权宽跨式组合
    STG = b'8',
    /// 买入期货期权组合
    BFO = b'9',
    /// 卖出期货期权组合
    SFO = b'a',
}
/// OptionRoyaltyPriceType 是一个期权权利金价格类型类型
#[repr(u8)]
pub enum OptionRoyaltyPriceType {
    /// 昨结算价
    PreSettlementPrice = b'1',
    /// 开仓价
    OpenPrice = b'4',
    /// 最新价与昨结算价较大值
    MaxPreSettlementPrice = b'5',
}
/// BalanceAlgorithm 是一个权益算法类型
#[repr(u8)]
pub enum BalanceAlgorithm {
    /// 不计算期权市值盈亏
    Default = b'1',
    /// 计算期权市值亏损
    IncludeOptValLost = b'2',
}
/// ActionType 是一个执行类型类型
#[repr(u8)]
pub enum ActionType {
    /// 执行
    Exec = b'1',
    /// 放弃
    Abandon = b'2',
}
/// ForQuoteStatus 是一个询价状态类型
#[repr(u8)]
pub enum ForQuoteStatus {
    /// 已经提交
    Submitted = b'a',
    /// 已经接受
    Accepted = b'b',
    /// 已经被拒绝
    Rejected = b'c',
}
/// ValueMethod 是一个取值方式类型
#[repr(u8)]
pub enum ValueMethod {
    /// 按绝对值
    Absolute = b'0',
    /// 按比率
    Ratio = b'1',
}
/// ExecOrderPositionFlag 是一个期权行权后是否保留期货头寸的标记类型
#[repr(u8)]
pub enum ExecOrderPositionFlag {
    /// 保留
    Reserve = b'0',
    /// 不保留
    UnReserve = b'1',
}
/// ExecOrderCloseFlag 是一个期权行权后生成的头寸是否自动平仓类型
#[repr(u8)]
pub enum ExecOrderCloseFlag {
    /// 自动平仓
    AutoClose = b'0',
    /// 免于自动平仓
    NotToClose = b'1',
}
/// ProductType 是一个产品类型类型
#[repr(u8)]
pub enum ProductType {
    /// 期货
    Futures = b'1',
    /// 期权
    Options = b'2',
}
/// CZCEUploadFileName 是一个郑商所结算文件名类型
#[repr(u8)]
pub enum CZCEUploadFileName {
    /// ^\d{8}_zz_\d{4}
    O = b'O',
    /// ^\d{8}成交表
    T = b'T',
    /// ^\d{8}单腿持仓表new
    P = b'P',
    /// ^\d{8}非平仓了结表
    N = b'N',
    /// ^\d{8}平仓表
    L = b'L',
    /// ^\d{8}资金表
    F = b'F',
    /// ^\d{8}组合持仓表
    C = b'C',
    /// ^\d{8}保证金参数表
    M = b'M',
}
/// DCEUploadFileName 是一个大商所结算文件名类型
#[repr(u8)]
pub enum DCEUploadFileName {
    /// ^\d{8}_dl_\d{3}
    O = b'O',
    /// ^\d{8}_成交表
    T = b'T',
    /// ^\d{8}_持仓表
    P = b'P',
    /// ^\d{8}_资金结算表
    F = b'F',
    /// ^\d{8}_优惠组合持仓明细表
    C = b'C',
    /// ^\d{8}_持仓明细表
    D = b'D',
    /// ^\d{8}_保证金参数表
    M = b'M',
    /// ^\d{8}_期权执行表
    S = b'S',
}
/// SHFEUploadFileName 是一个上期所结算文件名类型
#[repr(u8)]
pub enum SHFEUploadFileName {
    /// ^\d{4}_\d{8}_\d{8}_DailyFundChg  (4位机构代码_日期_日期_每日资金变动文件)
    O = b'O',
    /// ^\d{4}_\d{8}_\d{8}_Trade (4位机构代码_日期_日期_成交明细文件)
    T = b'T',
    /// ^\d{4}_\d{8}_\d{8}_SettlementDetail (4位机构代码_日期_日期_结算明细文件)
    P = b'P',
    /// ^\d{4}_\d{8}_\d{8}_Capital (4位机构代码_日期_日期_资金总账文件 / 客户资金汇总文件)
    F = b'F',
}
/// CFFEXUploadFileName 是一个中金所结算文件名类型
#[repr(u8)]
pub enum CFFEXUploadFileName {
    /// ^\d{4}_SG\d{1}_\d{8}_\d{1}_Trade (4位机构代码_日期_日期_成交明细文件)
    T = b'T',
    /// ^\d{4}_SG\d{1}_\d{8}_\d{1}_SettlementDetail (4位机构代码_日期_日期_结算明细文件)
    P = b'P',
    /// ^\d{4}_SG\d{1}_\d{8}_\d{1}_Capital (4位机构代码_日期_日期_资金总账文件 / 客户资金汇总文件)
    F = b'F',
    /// ^\d{4}_SG\d{1}_\d{8}_\d{1}_OptionExec (4位机构代码_日期_日期_期权行权 / 履约明细文件)
    S = b'S',
}
/// CombDirection 是一个组合指令方向类型
#[repr(u8)]
pub enum CombDirection {
    /// 申请组合
    Comb = b'0',
    /// 申请拆分
    UnComb = b'1',
    /// 操作员删组合单
    DelComb = b'2',
}
/// StrikeOffsetType 是一个行权偏移类型类型
#[repr(u8)]
pub enum StrikeOffsetType {
    /// 实值额
    RealValue = b'1',
    /// 盈利额
    ProfitValue = b'2',
    /// 实值比例
    RealRatio = b'3',
    /// 盈利比例
    ProfitRatio = b'4',
}
/// ReserveOpenAccStas 是一个预约开户状态类型
#[repr(u8)]
pub enum ReserveOpenAccStas {
    /// 等待处理中
    Processing = b'0',
    /// 已撤销
    Cancelled = b'1',
    /// 已开户
    Opened = b'2',
    /// 无效请求
    Invalid = b'3',
}
/// WeakPasswordSource 是一个弱密码来源类型
#[repr(u8)]
pub enum WeakPasswordSource {
    /// 弱密码库
    Lib = b'1',
    /// 手工录入
    Manual = b'2',
}
/// OptSelfCloseFlag 是一个期权行权的头寸是否自对冲类型
#[repr(u8)]
pub enum OptSelfCloseFlag {
    /// 自对冲期权仓位
    CloseSelfOptionPosition = b'1',
    /// 保留期权仓位
    ReserveOptionPosition = b'2',
    /// 自对冲卖方履约后的期货仓位
    SellCloseSelfFuturePosition = b'3',
    /// 保留卖方履约后的期货仓位
    ReserveFuturePosition = b'4',
}
/// BizType 是一个业务类型类型
#[repr(u8)]
pub enum BizType {
    /// 期货
    Future = b'1',
    /// 证券
    Stock = b'2',
}
/// AppType 是一个用户App类型类型
#[repr(u8)]
pub enum AppType {
    /// 直连的投资者
    Investor = b'1',
    /// 为每个投资者都创建连接的中继
    InvestorRelay = b'2',
    /// 所有投资者共享一个操作员连接的中继
    OperatorRelay = b'3',
    /// 未知
    UnKnown = b'4',
}
/// ResponseValue 是一个应答类型类型
#[repr(u8)]
pub enum ResponseValue {
    /// 检查成功
    Right = b'0',
    /// 检查失败
    Refuse = b'1',
}
/// OTCTradeType 是一个OTC成交类型类型
#[repr(u8)]
pub enum OTCTradeType {
    /// 大宗交易
    Block = b'0',
    /// 期转现
    EFP = b'1',
}
/// MatchType 是一个期现风险匹配方式类型
#[repr(u8)]
pub enum MatchType {
    /// 基点价值
    DV01 = b'1',
    /// 面值
    ParValue = b'2',
}
/// AuthType 是一个用户终端认证方式类型
#[repr(u8)]
pub enum AuthType {
    /// 白名单校验
    WHITE = b'0',
    /// 黑名单校验
    BLACK = b'1',
}
/// ClassType 是一个合约分类方式类型
#[repr(u8)]
pub enum ClassType {
    /// 所有合约
    ALL = b'0',
    /// 期货、即期、期转现、Tas、金属指数合约
    FUTURE = b'1',
    /// 期货、现货期权合约
    OPTION = b'2',
    /// 组合合约
    COMB = b'3',
}
/// TradingType 是一个合约交易状态分类方式类型
#[repr(u8)]
pub enum TradingType {
    /// 所有状态
    All = b'0',
    /// 交易
    Trade = b'1',
    /// 非交易
    UnTrade = b'2',
}
/// ProductStatus 是一个产品状态类型
#[repr(u8)]
pub enum ProductStatus {
    /// 可交易
    Tradeable = b'1',
    /// 不可交易
    Untradeable = b'2',
}
/// SyncDeltaStatus 是一个追平状态类型
#[repr(u8)]
pub enum SyncDeltaStatus {
    /// 交易可读
    Readable = b'1',
    /// 交易在读
    Reading = b'2',
    /// 交易读取完成
    ReadEnd = b'3',
    /// 追平失败 交易本地状态结算不存在
    OptErr = b'e',
}
/// ActionDirection 是一个操作标志类型
#[repr(u8)]
pub enum ActionDirection {
    /// 增加
    Add = b'1',
    /// 删除
    Del = b'2',
    /// 更新
    Upd = b'3',
}
/// OrderCancelAlg 是一个撤单时选择席位算法类型
#[repr(u8)]
pub enum OrderCancelAlg {
    /// 轮询席位撤单
    Balance = b'1',
    /// 优先原报单席位撤单
    OrigFirst = b'2',
}
/// OpenLimitControlLevel 是一个开仓量限制粒度类型
#[repr(u8)]
pub enum OpenLimitControlLevel {
    /// 不控制
    None = b'0',
    /// 产品级别
    Product = b'1',
    /// 合约级别
    Inst = b'2',
}
/// OrderFreqControlLevel 是一个报单频率控制粒度类型
#[repr(u8)]
pub enum OrderFreqControlLevel {
    /// 不控制
    None = b'0',
    /// 产品级别
    Product = b'1',
    /// 合约级别
    Inst = b'2',
}
/// EnumBool 是一个枚举bool类型类型
#[repr(u8)]
pub enum EnumBool {
    /// false
    False = b'0',
    /// true
    True = b'1',
}
/// TimeRange 是一个期货合约阶段标识类型
#[repr(u8)]
pub enum TimeRange {
    /// 一般月份
    USUAL = b'1',
    /// 交割月前一个月上半月
    FNSP = b'2',
    /// 交割月前一个月下半月
    BNSP = b'3',
    /// 交割月份
    SPOT = b'4',
}
/// Portfolio 是一个新型组保算法类型
#[repr(u8)]
pub enum Portfolio {
    /// 不使用新型组保算法
    None = b'0',
    /// SPBM算法
    SPBM = b'1',
    /// RULE算法
    RULE = b'2',
    /// SPMM算法
    SPMM = b'3',
    /// RCAMS算法
    RCAMS = b'4',
}
/// WithDrawParamID 是一个可提参数代码类型
#[repr(u8)]
pub enum WithDrawParamID {
    /// 权利金收支是否可提 1 代表可提 0 不可提
    CashIn = b'C',
}
/// InvestTradingRight 是一个投资者交易权限类型
#[repr(u8)]
pub enum InvestTradingRight {
    /// 只能平仓
    CloseOnly = b'1',
    /// 不能交易
    Forbidden = b'2',
}
/// InstMarginCalID 是一个SPMM合约保证金算法类型
#[repr(u8)]
pub enum InstMarginCalID {
    /// 标准算法收取双边
    BothSide = b'1',
    /// 单向大边
    MMSA = b'2',
    /// 新组保SPMM
    SPMM = b'3',
}
/// RCAMSCombinationType 是一个RCAMS组合类型类型
#[repr(u8)]
pub enum RCAMSCombinationType {
    /// 牛市看涨价差组合
    BUC = b'0',
    /// 熊市看涨价差组合
    BEC = b'1',
    /// 熊市看跌价差组合
    BEP = b'2',
    /// 牛市看跌价差组合
    BUP = b'3',
    /// 日历价差组合
    CAS = b'4',
}
/// PortfType 是一个新组保算法启用类型类型
#[repr(u8)]
pub enum PortfType {
    /// 使用初版交易所算法
    None = b'0',
    /// SPBM算法V1.1.0_附加保证金调整
    SpbmAddOnHedge = b'1',
}
/// InstrumentClass 是一个合约类型类型
#[repr(u8)]
pub enum InstrumentClass {
    /// 一般月份合约
    Usual = b'1',
    /// 临近交割合约
    Delivery = b'2',
    /// 非组合合约
    NonComb = b'3',
}
/// ProdChangeFlag 是一个品种记录改变状态类型
#[repr(u8)]
pub enum ProdChangeFlag {
    /// 持仓量和冻结量均无变化
    None = b'0',
    /// 持仓量无变化，冻结量有变化
    OnlyFrozen = b'1',
    /// 持仓量有变化
    PositionChange = b'2',
}
/// PwdRcdSrc 是一个历史密码来源类型
#[repr(u8)]
pub enum PwdRcdSrc {
    /// 来源于Sync初始化数据
    Init = b'0',
    /// 来源于实时上场数据
    Sync = b'1',
    /// 来源于用户修改
    UserUpd = b'2',
    /// 来源于超户修改，很可能来自主席同步数据
    SuperUserUpd = b'3',
    /// 来源于次席同步的修改
    SecUpd = b'4',
}
/// AddrSrvMode 是一个地址服务类型类型
#[repr(u8)]
pub enum AddrSrvMode {
    /// 交易地址
    Trade = b'0',
    /// 行情地址
    MarketData = b'1',
    /// 其他
    Other = b'2',
}
/// AddrVer 是一个地址版本类型
#[repr(u8)]
pub enum AddrVer {
    /// IPV4
    V4 = b'0',
    /// IPV6
    V6 = b'1',
}
/// TGSessionQryStatusType 是一个TGATE会话查询状态类型
#[repr(u8)]
pub enum TGSessionQryStatusType {
    /// 查询状态空闲
    QryIdle = b'1',
    /// 查询状态频繁
    QryBusy = b'2',
}
/// OffsetType 是一个对冲类型类型
#[repr(u8)]
pub enum OffsetType {
    /// 期权对冲
    OptOffset = b'0',
    /// 期货对冲
    FutOffset = b'1',
    /// 行权后期货对冲
    ExecOffset = b'2',
    /// 履约后期货对冲
    PerformOffset = b'3',
}
/// ApplySrc 是一个申请来源类型
#[repr(u8)]
pub enum ApplySrc {
    /// 交易
    Trade = b'0',
    /// 会服
    Member = b'1',
}
