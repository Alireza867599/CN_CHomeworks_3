#include "datalinkheader.h"

DataLinkHeader::DataLinkHeader() {}

DataLinkHeader::~DataLinkHeader()
{

}

void DataLinkHeader::setSourceMAC(const std::string &mac)
{
    sourceMAC = mac;
}

void DataLinkHeader::setDestinationMAC(const std::string &mac)
{
    destinationMAC = mac;
}

std::string DataLinkHeader::getSourceMAC() const
{
    return sourceMAC;

}

std::string DataLinkHeader::getDestinationMAC() const
{
    return destinationMAC;
}
