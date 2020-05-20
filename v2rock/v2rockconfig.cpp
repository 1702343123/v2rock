#include "v2rockconfig.h"
#include "config.h"

V2RockConfig::V2RockConfig(QObject *parent) :
    QObject(parent), empty(true), socksConfig(0), httpConfig(0)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    if (path.isEmpty())
    {
        qFatal("Cannot determine settings storage location");
    }
    workDir = path + "/v2rock";
    QDir *dir = new QDir(workDir);
    if (!dir->exists())
    {
        dir->mkpath(".");
        qDebug() << "Create nonexists path: " << workDir;
    }
}

V2RockConfig::~V2RockConfig()
{
    bypassIps.clear();
    bypassDomains.clear();
    nodes.clear();
}

bool V2RockConfig::isEmpty() const
{
    return empty;
}

QString V2RockConfig::getWorkDir() const
{
    return workDir;
}

void V2RockConfig::setWorkDir(const QString &value)
{
    workDir = value;
}

QString V2RockConfig::getV2executablePath() const
{
    return v2executablePath;
}

void V2RockConfig::setV2executablePath(const QString &value)
{
    v2executablePath = value;
}

QString V2RockConfig::getSubscribeUrl() const
{
    return subscribeUrl;
}

void V2RockConfig::setSubscribeUrl(const QString &value)
{
    subscribeUrl = value;
}

V2RayConfigInbound *V2RockConfig::getSocksConfig() const
{
    return socksConfig;
}

void V2RockConfig::setSocksConfig(V2RayConfigInbound *value)
{
    socksConfig = value;
}

void V2RockConfig::initSocksConfig(const QString &listen, const int port)
{
    socksConfig = new V2RayConfigInbound;
    socksConfig->setTag("socks-inbound");
    socksConfig->setProtocol("socks");
    socksConfig->setPort(port);
    socksConfig->setListen(listen);
    InboundSocksConfigurationObject *socksSettings = new InboundSocksConfigurationObject;
    socksSettings->accounts = 0;
    socksSettings->auth = "noauth";
    socksSettings->ip = listen;
    socksSettings->udp = true;
    socksSettings->userLevel = 0;
    socksConfig->setSocksSettings(socksSettings);
    SniffingObject *sniffing = new SniffingObject;
    sniffing->enabled = true;
    sniffing->destOverride.append("http");
    sniffing->destOverride.append("tls");
    socksConfig->setSniffing(sniffing);
}

void V2RockConfig::deleteSocksConfig()
{
    delete socksConfig;
    socksConfig = 0;
}

V2RayConfigInbound *V2RockConfig::getHttpConfig() const
{
    return httpConfig;
}

void V2RockConfig::setHttpConfig(V2RayConfigInbound *value)
{
    httpConfig = value;
}

void V2RockConfig::initHttpConfig(const QString &listen, const int port)
{
    httpConfig = new V2RayConfigInbound;
    httpConfig->setTag("http-inbound");
    httpConfig->setProtocol("http");
    httpConfig->setPort(port);
    httpConfig->setListen(listen);
    InboundHTTPConfigurationObject *httpSettings = new InboundHTTPConfigurationObject;
    httpSettings->accounts = 0;
    httpSettings->allowTransparent = false;
    httpSettings->timeout = 60 * 1000;
    httpSettings->userLevel = 0;
    httpConfig->setHttpSettings(httpSettings);
    SniffingObject *sniffing = new SniffingObject;
    sniffing->enabled = true;
    sniffing->destOverride.append("http");
    sniffing->destOverride.append("tls");
    httpConfig->setSniffing(sniffing);
}

void V2RockConfig::deleteHttpConfig()
{
    delete httpConfig;
    httpConfig = 0;
}

QList<QString> V2RockConfig::getBypassIps() const
{
    return bypassIps;
}

void V2RockConfig::setBypassIps(const QList<QString> &value)
{
    bypassIps = value;
}

QList<QString> V2RockConfig::getBypassDomains() const
{
    return bypassDomains;
}

void V2RockConfig::setBypassDomains(const QList<QString> &value)
{
    bypassDomains = value;
}

QList<V2RockNode> V2RockConfig::getNodes() const
{
    return nodes;
}

void V2RockConfig::setNodes(const QList<V2RockNode> &value)
{
    nodes = value;
}

int V2RockConfig::getNodeIndex() const
{
    return nodeIndex;
}

void V2RockConfig::setNodeIndex(int value)
{
    nodeIndex = value;
}

void V2RockConfig::init()
{
    v2executablePath = "/usr/bin/v2ray/v2ray";
    subscribeUrl = "";

    initSocksConfig("127.0.0.1", 1080);
    initHttpConfig("127.0.0.1", 1081);

    nodes.clear();

    bypassIps.clear();
    bypassIps.append("0.0.0.0/8");
    bypassIps.append("10.0.0.0/8");
    bypassIps.append("127.0.0.0/8");
    bypassIps.append("172.16.0.0/12");
    bypassIps.append("192.168.0.0/16");
    bypassIps.append("::1/128");
    bypassIps.append("fc00::/7");
    bypassIps.append("fe80::/10");

    bypassDomains.clear();
    bypassDomains.append("domain:baidu.com");
    bypassDomains.append("domain:bdstatic.com");
    bypassDomains.append("domain:baidustatic.com");
    bypassDomains.append("domain:qq.com");
    bypassDomains.append("domain:sogou.com");
    bypassDomains.append("domain:sogoucdn.com");
}

void V2RockConfig::read()
{
    QFile file(workDir + "/" + v2rock_config::main_config);
    if (!file.exists()) {
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit logReceived("Configuration file cannot be read.");
        return;
    }
    QString data = file.readAll();
    file.close();

    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8(), &errorPtr);
    if (doc.isNull()) {
        emit logReceived("Configuration file format is invalid.");
        return;
    }
    fromJson(doc.object());
    empty = false;
}

void V2RockConfig::write()
{
    emit logReceived("Writing configs...");
    QJsonObject jsonObj;
    toJson(jsonObj);
    QJsonDocument doc(jsonObj);
    QFile file(workDir + "/" + v2rock_config::main_config);
    if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
        emit logReceived("Error: file is not writable: " + workDir + "/" + v2rock_config::main_config);
    }
    file.write(doc.toJson());
    file.close();
    emit logReceived("Configs have been saved.");
}

void V2RockConfig::fromJson(const QJsonObject &json)
{
    if (json.contains("v2executablePath") && json["v2executablePath"].isString()) {
        v2executablePath = json["v2executablePath"].toString();
    }
    if (json.contains("subscribeUrl") && json["subscribeUrl"].isString()) {
        subscribeUrl = json["subscribeUrl"].toString();
    }

    if (json.contains("nodeIndex")) {
        nodeIndex = json["nodeIndex"].toInt();
    }

    // Read httpConfig, delete the old one before reading
    if (httpConfig) {
        delete httpConfig;
        httpConfig = 0;
    }
    if (json.contains("httpConfig")) {
        httpConfig = new V2RayConfigInbound;
        httpConfig->fromJson(json["httpConfig"].toObject());
    }

    // Read socksConfig, delete the old one before reading
    if (socksConfig) {
        delete socksConfig;
        socksConfig = 0;
    }
    if (json.contains("socksConfig")) {
        socksConfig = new V2RayConfigInbound;
        socksConfig->fromJson(json["socksConfig"].toObject());
    }

    bypassIps.clear();
    if (json.contains("bypassIps") && json["bypassIps"].isArray()) {
        QJsonArray ips = json["bypassIps"].toArray();
        for (int i = 0; i < ips.size(); ++i) {
            if (ips[i].isString()) {
                bypassIps.append(ips[i].toString());
            }
        }
    }

    bypassDomains.clear();
    if (json.contains("bypassDomains") && json["bypassDomains"].isArray()) {
        QJsonArray domains = json["bypassDomains"].toArray();
        for (int i = 0; i < domains.size(); ++i) {
            if (domains[i].isString()) {
                bypassDomains.append(domains[i].toString());
            }
        }
    }

    nodes.clear();
    QJsonArray jnodes = json["nodes"].toArray();
    for (int i = 0; i < jnodes.size(); ++i) {
        QJsonObject jnode = jnodes[i].toObject();
        V2RockNode node;
        node.read(jnode);
        nodes.append(node);
    }
    // Make sure nodeIndex is valid
    if (nodeIndex < 0 || nodeIndex >= nodes.size()) {
        nodeIndex = 0;
    }
}

void V2RockConfig::toJson(QJsonObject &json) const
{
    json["v2executablePath"] = v2executablePath;
    json["subscribeUrl"] = subscribeUrl;
    json["nodeIndex"] = nodeIndex;

    QJsonArray ipsArray;
    foreach (const QString ip, bypassIps) {
        ipsArray.append(ip);
    }
    json["bypassIps"] = ipsArray;

    QJsonArray domainsArray;
    foreach (const QString domain, bypassDomains) {
        domainsArray.append(domain);
    }
    json["bypassDomains"] = domainsArray;

    if (socksConfig) {
        QJsonObject socksConfigObj;
        socksConfig->toJson(socksConfigObj);
        json["socksConfig"] = socksConfigObj;
    }

    if (httpConfig) {
        QJsonObject httpConfigObj;
        httpConfig->toJson(httpConfigObj);
        json["httpConfig"] = httpConfigObj;
    }

    QJsonArray nodesArray;
    foreach (const V2RockNode node, nodes) {
        QJsonObject jobject;
        node.write(jobject);
        nodesArray.append(jobject);
    }
    json["nodes"] = nodesArray;
}

void V2RockConfig::print(int indentation) const
{
    const QString indent(indentation * 2, ' ');
    QTextStream(stdout) << indent << "v2executablePath:\t" << v2executablePath << "\n";
    QTextStream(stdout) << indent << "subscribeUrl:\t" << subscribeUrl << "\n";
    QTextStream(stdout) << indent << "nodeIndex:\t" << nodeIndex << "\n";
    QTextStream(stdout) << indent << "Bypass IPs:\n";
    for (const QString &ip : bypassIps) {
        QTextStream(stdout) << indent << indent << ip << "\n";
    }
    QTextStream(stdout) << indent << "Bypass Domains:\n";
    for (const QString &domain : bypassDomains) {
        QTextStream(stdout) << indent << indent << domain << "\n";
    }
    QTextStream(stdout) << indent << "Nodes:\n";
    for (const V2RockNode &node : nodes)
        node.print(2);
}

QString *V2RockConfig::toV2RayJson(QJsonObject &json)
{
    if (nodeIndex < 0 || nodeIndex >= nodes.size()) {
        emit logReceived("Error: Invalid selection " + QString::number(nodeIndex));
        return 0;
    }
    V2RockNode node = nodes.at(nodeIndex);

    // Start to compose the V2Ray configs
    V2RayConfig v2rayConfig;
    // policy, stats, api, dns
    v2rayConfig.setPolicy(0);
    v2rayConfig.setStats(false);
    v2rayConfig.setApi(0);
    v2rayConfig.setDns(0);

    // log
    V2RayConfigLog v2rayConfigLog;
    v2rayConfigLog.setLoglevel("info");
    v2rayConfigLog.setAccess(0);
    v2rayConfigLog.setError(0);
    v2rayConfig.setLog(v2rayConfigLog);

    // inbounds
    QList<V2RayConfigInbound> inbounds;
    if (socksConfig) {
        inbounds.append(*socksConfig);
    }
    if (httpConfig) {
        inbounds.append(*httpConfig);
    }
    v2rayConfig.setInbounds(inbounds);

    // outbounds
    // proxy outbound
    V2RayConfigOutbound proxyOutbound;
    proxyOutbound.setTag("proxy");
    proxyOutbound.setProtocol(node.getProtocol());
    if (proxyOutbound.getProtocol() == "vmess") {
        OutboundVMessConfigurationObject *vmessSettings = new OutboundVMessConfigurationObject;
        proxyOutbound.setVMessSettings(vmessSettings);
        VMessServerObject vMessServer;
        vMessServer.address = node.getAddress();
        vMessServer.port = node.getPort();
        vMessServer.users = new QList<struct UserObject>;
        UserObject user;
        user.id = node.getUserId();
        user.alterId = node.getUserAid();
        user.level = 0;
        user.security = node.getUserSecurity();
        vMessServer.users->append(user);
        vmessSettings->vnext.append(vMessServer);
    }

    StreamSettingsObject *streamSettings = new StreamSettingsObject;
    proxyOutbound.setStreamSettings(streamSettings);
    streamSettings->network = "ws";
    streamSettings->security = "none";
    streamSettings->sockopt = 0;
    streamSettings->tlsSettings = 0;
    streamSettings->tcpSettings = 0;
    streamSettings->kcpSettings = 0;
    streamSettings->httpSettings = 0;
    streamSettings->dsSettings = 0;
    streamSettings->quicSettings = 0;

    streamSettings->wsSettings = new TransportWebSocketObject;
    streamSettings->wsSettings->path = node.getStreamWsPath();
    streamSettings->wsSettings->headers.insert("Host", "");

    MuxObject *mux = new MuxObject;
    proxyOutbound.setMux(mux);
    mux->enabled = true;
    mux->concurrency = 8;

    // direct outbound
    V2RayConfigOutbound directOutbound;
    directOutbound.setTag("direct");
    directOutbound.setProtocol("freedom");
    OutboundFreedomConfigurationObject *freedomSettings = new OutboundFreedomConfigurationObject;
    proxyOutbound.setFreedomSettings(freedomSettings);
    freedomSettings->domainStrategy = "AsIs";
    freedomSettings->redirect = 0;
    freedomSettings->userLevel = 0;

    // block outbound
    V2RayConfigOutbound blockOutbound;
    blockOutbound.setTag("block");
    blockOutbound.setProtocol("blackhole");
    OutboundBlackholeConfigurationObject *blockSettings = new OutboundBlackholeConfigurationObject;
    blockOutbound.setBlackholeSettings(blockSettings);
    blockSettings->response.type = "http";

    QList<V2RayConfigOutbound> outbounds;
    outbounds.append(proxyOutbound);
    outbounds.append(directOutbound);
    outbounds.append(blockOutbound);
    v2rayConfig.setOutbounds(outbounds);

    // routing
    V2RayConfigRoute v2rayConfigRoute;
    v2rayConfigRoute.setDomainStrategy("IPIfNonMatch");
    v2rayConfigRoute.setBalancers(0);

    RoutingRuleObject ipRule;
    ipRule.type = "field";
    ipRule.domain = 0;
    ipRule.port = 0;
    ipRule.network = 0;
    ipRule.source = 0;
    ipRule.user = 0;
    ipRule.inboundTag = 0;
    ipRule.protocol = 0;
    ipRule.attrs = 0;
    ipRule.outboundTag = new QString("direct");
    ipRule.balancerTag = 0;
    QStringList *ips = new QStringList;
    foreach (const QString ip, bypassIps) {
        ips->append(ip);
    }
    ipRule.ip = ips;

    RoutingRuleObject domainRule;
    domainRule.type = "field";
    domainRule.ip = 0;
    domainRule.port = 0;
    domainRule.network = 0;
    domainRule.source = 0;
    domainRule.user = 0;
    domainRule.inboundTag = 0;
    domainRule.protocol = 0;
    domainRule.attrs = 0;
    domainRule.outboundTag = new QString("direct");
    domainRule.balancerTag = 0;
    QStringList *domains = new QStringList;
    foreach (const QString domain, bypassDomains) {
        domains->append(domain);
    }
    domainRule.domain = domains;

    QList<RoutingRuleObject> rules;
    rules.append(ipRule);
    rules.append(domainRule);
    v2rayConfigRoute.setRules(rules);

    v2rayConfig.setRouting(v2rayConfigRoute);

    v2rayConfig.toJson(json);
    QJsonDocument doc(json);
    QString *configFilePath = new QString(workDir + "/" + v2rock_config::v2ray_config);
    QFile file(*configFilePath);
    if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
        emit logReceived("Error: file not writable: " + *configFilePath);
    }
    file.write(doc.toJson());
    file.close();
    emit logReceived("Configs have been saved.");
    return configFilePath;
}

