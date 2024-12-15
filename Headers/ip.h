#ifndef IP_H
#define IP_H

#include "globals.h"

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QTextStream>

template <UT::IPVersion version>
class IP;

typedef IP<UT::IPVersion::IPv4> IPv4_t;
typedef IP<UT::IPVersion::IPv6> IPv6_t;
typedef QSharedPointer<IPv4_t>  IPv4Ptr_t;
typedef QSharedPointer<IPv6_t>  IPv6Ptr_t;

class AbstractIP : public QObject
{
    Q_OBJECT

public:
    explicit AbstractIP(QObject *parent = nullptr);
    virtual ~AbstractIP() = default;

    // Pure virtual methods
    virtual uint64_t toValue() const = 0;
    virtual QString toString() const = 0;
    virtual void fromString(const QString &ipString) = 0;
    virtual void fromValue(uint64_t ipValue) = 0;
};

template <>
class IP<UT::IPVersion::IPv4> : public AbstractIP
{
    // removed Q_OBJECT macro to avoid moc error
    // https://doc.qt.io/qt-6/moc.html
    // Q_OBJECT

public: // Constructors
    explicit IP(QObject *parent = nullptr);
    explicit IP(const QString &ipString, QObject *parent = nullptr);
    explicit IP(uint64_t ipValue, QObject *parent = nullptr);
    ~IP() override;

public:    // methods
    uint64_t toValue() const override;
    QString toString() const override;
    void fromString(const QString &ipString) override;
    void fromValue(uint64_t ipValue) override;
public:    // operators
    bool
    operator==(const IP<UT::IPVersion::IPv4> &other) const
    {
        return toValue() == other.toValue();
    }


    uint8_t versionHeaderLenth() const;
    void setVersionHeaderLenth(uint8_t newVersionHeaderLenth);

    uint8_t typeOfService() const;
    void setTypeOfService(uint8_t newTypeOfService);

    uint16_t totalLength() const;
    void setTotalLength(uint16_t newTotalLength);

    uint16_t flagsFragmentOffset() const;
    void setFlagsFragmentOffset(uint16_t newFlagsFragmentOffset);

    uint8_t ttl() const;
    void setTtl(uint8_t newTtl);

    uint8_t protocol() const;
    void setProtocol(uint8_t newProtocol);

    uint16_t headerChecksum() const;
    void setHeaderChecksum(uint16_t newHeaderChecksum);

    std::array<uint8_t, 4> sourceIp() const;
    void setSourceIp(const std::array<uint8_t, 4>& newSourceIp);

    std::array<uint8_t, 4> destIp() ;
    void setDestIp(const std::array<uint8_t, 4>& newDestIp);

private: // Methods
    void parseIPString(const QString &ipString);
    QString convertToIPString(uint64_t ipValue) const;

private: // Members
    uint8_t  m_versionHeaderLength = 0;
    uint8_t  m_typeOfService = 0;
    uint16_t m_totalLength = 0;
    uint16_t m_identification = 0;
    uint16_t m_flagsFragmentOffset = 0;
    uint8_t  m_ttl = 64; // Default TTL value
    uint8_t  m_protocol = 0;
    uint16_t m_headerChecksum = 0;
    std::array<uint8_t, 4> m_sourceIp; // Source IP address as 4 octets
    std::array<uint8_t, 4> m_destIp;   // Destination IP address as 4 octets
};
/**
 * ===========================================
 * ===========================================
 * ===========================================
 * @brief The IP class for IPv6.
 * ===========================================
 * ===========================================
 * ===========================================
 */
template <>
class IP<UT::IPVersion::IPv6> : public AbstractIP
{
    // removed Q_OBJECT macro to avoid moc error
    // https://doc.qt.io/qt-6/moc.html
    // Q_OBJECT

public:    // constructors
    explicit IP(QObject *parent = nullptr);
    explicit IP(const QString &ipString, QObject *parent = nullptr);
    explicit IP(uint64_t ipValue, QObject *parent = nullptr);
    ~IP() override;

public:    // methods

    uint64_t toValue() const override;
    QString toString() const override;
    void fromString(const QString &ipString) override;
    void fromValue(uint64_t ipValue) override;

public:    // operators
    bool
    operator==(const IP<UT::IPVersion::IPv6> &other) const
    {
        return toValue() == other.toValue();
    }

    uint32_t versionTrafficClassFlowLabel() const;
    void setVersionTrafficClassFlowLabel(uint32_t newVersionTrafficClassFlowLabel);

    uint16_t payloadLength() const;
    void setPayloadLength(uint16_t newPayloadLength);

    uint8_t nextHeader() const;
    void setNextHeader(uint8_t newNextHeader);

    uint8_t hopLimit() const;
    void setHopLimit(uint8_t newHopLimit);

    std::array<uint8_t, 16> sourceaddress() const;
    void setSourceaddress(const std::array<uint8_t, 16> &newSourceaddress);

    std::array<uint8_t, 16> destinationaddress() const;
    void setDestinationaddress(const std::array<uint8_t, 16> &newDestinationaddress);

private:    // methods


private: // Members
    uint32_t m_versionTrafficClassFlowLabel = 0; // Version (4 bits), Traffic Class, Flow Label
    uint16_t m_payloadLength = 0;
    uint8_t m_nextHeader = 0;
    uint8_t m_hopLimit = 0;
    std::array<uint8_t, 16> m_sourceaddress; // 128-bit IPv6 address (16 bytes)
    std::array<uint8_t, 16> m_destinationaddress; // 128-bit IPv6 address (16 bytes)

};


#endif    // IP_H
