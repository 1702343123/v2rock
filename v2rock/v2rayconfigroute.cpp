#include "v2rayconfigroute.h"

V2RayConfigRoute::V2RayConfigRoute() : balancers(0)
{

}

QString V2RayConfigRoute::getDomainStrategy() const
{
    return domainStrategy;
}

void V2RayConfigRoute::setDomainStrategy(const QString &value)
{
    domainStrategy = value;
}

QList<RoutingRuleObject> V2RayConfigRoute::getRules() const
{
    return rules;
}

void V2RayConfigRoute::setRules(const QList<RoutingRuleObject> &value)
{
    rules = value;
}

QList<RoutingBalancerObject> *V2RayConfigRoute::getBalancers() const
{
    return balancers;
}

void V2RayConfigRoute::setBalancers(QList<RoutingBalancerObject> *value)
{
    balancers = value;
}

void V2RayConfigRoute::toJson(QJsonObject &json) const
{
    json["domainStrategy"] = domainStrategy;

    QJsonArray rulesArray;
    foreach (const RoutingRuleObject rule, rules) {
        QJsonObject ruleObj;
        ruleObj["type"] = rule.type; // "field"
        if (rule.port) {
            ruleObj["port"] = *(rule.port);
        }
        if (rule.network) {
            ruleObj["network"] = *(rule.network);
        }
        if (rule.attrs) {
            ruleObj["attrs"] = *(rule.attrs);
        }
        if (rule.outboundTag) {
            ruleObj["outboundTag"] = *(rule.outboundTag);
        }
        if (rule.balancerTag) {
            ruleObj["balancerTag"] = *(rule.balancerTag);
        }
        if (rule.domain) {
            QJsonArray jArray;
            foreach(const QString val, *(rule.domain)) {
                jArray.append(val);
            }
            ruleObj["domain"] = jArray;
        }
        if (rule.ip) {
            QJsonArray jArray;
            foreach(const QString val, *(rule.ip)) {
                jArray.append(val);
            }
            ruleObj["ip"] = jArray;
        }
        if (rule.source) {
            QJsonArray jArray;
            foreach(const QString val, *(rule.source)) {
                jArray.append(val);
            }
            ruleObj["source"] = jArray;
        }
        if (rule.user) {
            QJsonArray jArray;
            foreach(const QString val, *(rule.user)) {
                jArray.append(val);
            }
            ruleObj["user"] = jArray;
        }
        if (rule.inboundTag) {
            QJsonArray jArray;
            foreach(const QString val, *(rule.inboundTag)) {
                jArray.append(val);
            }
            ruleObj["inboundTag"] = jArray;
        }
        if (rule.protocol) {
            QJsonArray jArray;
            foreach(const QString val, *(rule.protocol)) {
                jArray.append(val);
            }
            ruleObj["protocol"] = jArray;
        }

        rulesArray.append(ruleObj);
    }
    json["rules"] = rulesArray;

    if (balancers) {
        QJsonArray balancersArray;
        foreach (const RoutingBalancerObject balancer, *balancers) {
            QJsonObject balancerObj;
            balancerObj["tag"] = balancer.tag;
            QJsonArray jArray;
            foreach(const QString val, balancer.selector) {
                jArray.append(val);
            }
            balancerObj["selector"] = jArray;
            balancersArray.append(balancerObj);
        }
        json["balancers"] = balancersArray;
    }

}
