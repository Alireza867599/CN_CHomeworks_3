#include "ip.h"
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QTextStream>


AbstractIP::AbstractIP(QObject *parent) : QObject(parent) {}


// Default Constructor for IPv4 specialization
IP<UT::IPVersion::IPv4>::IP(QObject* parent)
    : AbstractIP(parent), m_versionHeaderLength(0), m_typeOfService(0), m_totalLength(0),m_flagsFragmentOffset(0),
     m_ttl(0),m_protocol(0),m_headerChecksum(0),m_sourceIp{},
    m_destIp{} {}


// Constructor from QString (IPv4)
IP<UT::IPVersion::IPv4>::IP(const QString &ipString, QObject *parent)
    : AbstractIP(parent)
{
    QStringList parts = ipString.split(".");
    if (parts.size() != 4)
    {
        throw std::invalid_argument("Invalid IPv4 address format");
    }

    for (int i = 0; i < 4; ++i)
    {
        bool ok;
        int value = parts[i].toInt(&ok);
        if (!ok || value < 0 || value > 255)
        {
            throw std::invalid_argument("Invalid IPv4 address part");
        }
        m_sourceIp[i] = static_cast<uint8_t>(value);
    }
}

// Constructor from uint64_t (IPv4)
IP<UT::IPVersion::IPv4>::IP(uint64_t ipValue, QObject *parent)
    : AbstractIP(parent)
{
    for (int i = 3; i >= 0; --i)
    {
        m_sourceIp[i] = static_cast<uint8_t>(ipValue & 0xFF);
        ipValue >>= 8;
    }
}

// Destructor
IP<UT::IPVersion::IPv4>::~IP() = default;

// Convert to 64-bit value
uint64_t IP<UT::IPVersion::IPv4>::toValue() const
{
    uint64_t value = 0;
    for (size_t i = 0; i < 4; ++i)
    {
        value |= (static_cast<uint64_t>(m_sourceIp[i]) << (24 - i * 8));
    }
    return value;
}

// Convert to String (IPv4)
QString IP<UT::IPVersion::IPv4>::toString() const
{
    return QString("%1.%2.%3.%4")
    .arg(m_sourceIp[0])
        .arg(m_sourceIp[1])
        .arg(m_sourceIp[2])
        .arg(m_sourceIp[3]);
}

// Parse from String (IPv4)
void IP<UT::IPVersion::IPv4>::fromString(const QString &ipString)
{
    QStringList parts = ipString.split(".");
    if (parts.size() != 4)
    {
        throw std::invalid_argument("Invalid IPv4 address string");
    }

    for (int i = 0; i < 4; ++i)
    {
        bool ok;
        int value = parts[i].toInt(&ok);
        if (!ok || value < 0 || value > 255)
        {
            throw std::invalid_argument("Invalid IPv4 address part");
        }
        m_sourceIp[i] = static_cast<uint8_t>(value);
    }
}

// Parse from uint64_t (IPv4)
void IP<UT::IPVersion::IPv4>::fromValue(uint64_t ipValue)
{
    for (int i = 3; i >= 0; --i)
    {
        m_sourceIp[i] = static_cast<uint8_t>(ipValue & 0xFF);
        ipValue >>= 8;
    }
}



void IP<UT::IPVersion::IPv4>::setSourceIp(const std::array<uint8_t, 4>& newSourceIp) {
    m_sourceIp = newSourceIp;
}

std::array<uint8_t, 4> IP<UT::IPVersion::IPv4>::sourceIp() const {
    return m_sourceIp;
}

void IP<UT::IPVersion::IPv4>::setDestIp(const std::array<uint8_t, 4>& newDestIp) {
    m_destIp = newDestIp;
}

std::array<uint8_t, 4> IP<UT::IPVersion::IPv4>::destIp()  {
    return m_destIp;
}


void IP<UT::IPVersion::IPv4>::setHeaderChecksum(uint16_t newHeaderChecksum)
{
    m_headerChecksum = newHeaderChecksum;
}

uint16_t IP<UT::IPVersion::IPv4>::headerChecksum() const
{
    return m_headerChecksum;
}

uint8_t IP<UT::IPVersion::IPv4>::protocol() const
{
    return m_protocol;
}

void  IP<UT::IPVersion::IPv4>::setProtocol(uint8_t newProtocol){
    m_protocol = newProtocol;
}


void IP<UT::IPVersion::IPv4>::setTtl(uint8_t newTtl)
{
    m_ttl = newTtl;
}

uint8_t IP<UT::IPVersion::IPv4>::ttl() const
{
    return m_ttl;
}

void IP<UT::IPVersion::IPv4>::setFlagsFragmentOffset(uint16_t newFlagsFragmentOffset)
{
    m_flagsFragmentOffset = newFlagsFragmentOffset;
}

uint16_t IP<UT::IPVersion::IPv4>::flagsFragmentOffset() const
{
    return m_flagsFragmentOffset;
}

void IP<UT::IPVersion::IPv4>::setTotalLength(uint16_t newTotalLength)
{
    m_totalLength = newTotalLength;
}

uint16_t IP<UT::IPVersion::IPv4>::totalLength() const
{
    return m_totalLength;
}

void IP<UT::IPVersion::IPv4>::setTypeOfService(uint8_t newTypeOfService)
{
    m_typeOfService = newTypeOfService;
}

uint8_t IP<UT::IPVersion::IPv4>::typeOfService() const
{
    return m_typeOfService;
}

void IP<UT::IPVersion::IPv4>::setVersionHeaderLenth(uint8_t newVersionHeaderLenth)
{

    m_versionHeaderLength = newVersionHeaderLenth;
}




uint8_t IP<UT::IPVersion::IPv4>::versionHeaderLenth() const
{
    return m_versionHeaderLength;
}






IP<UT::IPVersion::IPv6>::IP(QObject *parent)
    : AbstractIP(parent), m_versionTrafficClassFlowLabel(0), m_payloadLength(0),
    m_nextHeader(0), m_hopLimit(0), m_sourceaddress{}, m_destinationaddress{} {}

// Constructor: From QString
IP<UT::IPVersion::IPv6>::IP(const QString &ipString, QObject *parent)
    : AbstractIP(parent), m_versionTrafficClassFlowLabel(0), m_payloadLength(0),
    m_nextHeader(0), m_hopLimit(0), m_sourceaddress{}, m_destinationaddress{} {
    fromString(ipString);
}

// Constructor: From Value
IP<UT::IPVersion::IPv6>::IP(uint64_t ipValue, QObject *parent)
    : AbstractIP(parent), m_versionTrafficClassFlowLabel(0), m_payloadLength(0),
    m_nextHeader(0), m_hopLimit(0), m_sourceaddress{}, m_destinationaddress{} {
    fromValue(ipValue);
}

IP<UT::IPVersion::IPv6>::~IP() = default;




uint64_t IP<UT::IPVersion::IPv6>::toValue() const {
    // Simplified: returns the first 8 bytes as a 64-bit value
    uint64_t value = 0;
    for (int i = 0; i < 8; ++i) {
        value |= static_cast<uint64_t>(m_sourceaddress[i]) << ((7 - i) * 8);
    }
    return value;
}

QString IP<UT::IPVersion::IPv6>::toString() const {
    QStringList parts;
    for (size_t i = 0; i < 16; i += 2) {
        uint16_t segment = (m_sourceaddress[i] << 8) | m_sourceaddress[i + 1];
        parts.append(QString::number(segment, 16));
    }
    return parts.join(":");
}

void IP<UT::IPVersion::IPv6>::fromString(const QString &ipString) {
    QStringList parts = ipString.split(":");
    if (parts.size() != 8) {
        throw std::invalid_argument("Invalid IPv6 address format");
    }

    for (int i = 0; i < 8; ++i) {
        bool ok;
        uint16_t segment = parts[i].toUShort(&ok, 16);
        if (!ok) {
            throw std::invalid_argument("Invalid IPv6 address segment");
        }
        m_sourceaddress[i * 2] = (segment >> 8) & 0xFF;
        m_sourceaddress[i * 2 + 1] = segment & 0xFF;
    }
}

void IP<UT::IPVersion::IPv6>::fromValue(uint64_t ipValue) {
    for (int i = 7; i >= 0; --i) {
        m_sourceaddress[i] = static_cast<uint8_t>(ipValue & 0xFF);
        ipValue >>= 8;
    }
    // Set the remaining 8 bytes to zero
    for (int i = 8; i < 16; ++i) {
        m_sourceaddress[i] = 0;
    }
}





uint32_t IP<UT::IPVersion::IPv6>::versionTrafficClassFlowLabel() const {
    return m_versionTrafficClassFlowLabel;
}

void IP<UT::IPVersion::IPv6>::setVersionTrafficClassFlowLabel(uint32_t newVersionTrafficClassFlowLabel) {
    m_versionTrafficClassFlowLabel = newVersionTrafficClassFlowLabel;
}

uint16_t IP<UT::IPVersion::IPv6>::payloadLength() const {
    return m_payloadLength;
}

void IP<UT::IPVersion::IPv6>::setPayloadLength(uint16_t newPayloadLength) {
    m_payloadLength = newPayloadLength;
}

uint8_t IP<UT::IPVersion::IPv6>::nextHeader() const {
    return m_nextHeader;
}

void IP<UT::IPVersion::IPv6>::setNextHeader(uint8_t newNextHeader) {
    m_nextHeader = newNextHeader;
}

uint8_t IP<UT::IPVersion::IPv6>::hopLimit() const {
    return m_hopLimit;
}

void IP<UT::IPVersion::IPv6>::setHopLimit(uint8_t newHopLimit) {
    m_hopLimit = newHopLimit;
}

std::array<uint8_t, 16> IP<UT::IPVersion::IPv6>::sourceaddress() const {
    return m_sourceaddress;
}

void IP<UT::IPVersion::IPv6>::setSourceaddress(const std::array<uint8_t, 16> &newSourceaddress) {
    m_sourceaddress = newSourceaddress;
}

std::array<uint8_t, 16> IP<UT::IPVersion::IPv6>::destinationaddress() const {
    return m_destinationaddress;
}

void IP<UT::IPVersion::IPv6>::setDestinationaddress(const std::array<uint8_t, 16> &newDestinationaddress) {
    m_destinationaddress = newDestinationaddress;
}


